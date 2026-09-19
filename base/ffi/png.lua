--[[--
Module for PNG decoding/encoding.

Currently, this is a LuaJIT FFI wrapper for lodepng lib.

@module ffi.png
]]

local ffi = require("ffi")
local posix = require("ffi/posix")

require "ffi/libspng_h"

local spng = ffi.loadlib("spng", "0")
local C = ffi.C

local Png = {}

local spng_strerror = function(err)
    return ffi.string(spng.spng_strerror(err))
end

function Png.encodeToFile(filename, mem, w, h, n)

    local ihdr = ffi.new("struct spng_ihdr")

    ihdr.width = w
    ihdr.height = h
    -- We'll always want 8-bits per component
    ihdr.bit_depth = 8
    -- Devise the output color type based on the number of components passed
    if n == 1 then
        ihdr.color_type = spng.SPNG_COLOR_TYPE_GRAYSCALE
    elseif n == 2 then
        ihdr.color_type = spng.SPNG_COLOR_TYPE_GRAYSCALE_ALPHA
    elseif n == 3 then
        ihdr.color_type = spng.SPNG_COLOR_TYPE_TRUECOLOR
    elseif n == 4 then
        ihdr.color_type = spng.SPNG_COLOR_TYPE_TRUECOLOR_ALPHA
    else
        return false, "passed an invalid number of color components"
    end

    local ctx, err

    ctx = spng.spng_ctx_new(spng.SPNG_CTX_ENCODER)
    assert(ctx ~= nil)
    ctx = ffi.gc(ctx, spng.spng_ctx_free)

    local fp = C.fopen(filename, "wb")
    if fp == nil then
        return false, posix.strerror()
    end
    fp = ffi.gc(fp, C.fclose)

    err = spng.spng_set_png_file(ctx, fp)
    if err ~= 0 then
        return false, spng_strerror(err)
    end

    err = spng.spng_set_option(ctx, spng.SPNG_IMG_COMPRESSION_LEVEL, 5)
    if err ~= 0 then
        return false, spng_strerror(err)
    end

    err = spng.spng_set_ihdr(ctx, ihdr)
    if err ~= 0 then
        return false, spng_strerror(err)
    end

    err = spng.spng_encode_image(ctx, mem, w * h * n, spng.SPNG_FMT_PNG, spng.SPNG_ENCODE_FINALIZE)
    if err ~= 0 then
        return false, spng_strerror(err)
    end

    spng.spng_ctx_free(ffi.gc(ctx, nil))
    C.fclose(ffi.gc(fp, nil))

    return true
end

function Png.decodeFromFile(filename, req_n)

    local ctx, err

    ctx = spng.spng_ctx_new(0)
    assert(ctx ~= nil)
    ctx = ffi.gc(ctx, spng.spng_ctx_free)

    local fp = C.fopen(filename, "rb")
    if fp == nil then
        return false, posix.strerror()
    end
    fp = ffi.gc(fp, C.fclose)

    err = spng.spng_set_png_file(ctx, fp)
    if err ~= 0 then
        return false, spng_strerror(err)
    end

    local ihdr = ffi.new("struct spng_ihdr")
    err = spng.spng_get_ihdr(ctx, ihdr)
    if err ~= 0 then
        return false, spng_strerror(err)
    end

    local out_fmt, out_n

    -- Try to keep grayscale PNGs as-is if we requested so...
    if req_n == 1 then
        if ihdr.color_type == spng.SPNG_COLOR_TYPE_GRAYSCALE then
            out_fmt = spng.SPNG_FMT_PNG
        elseif ihdr.color_type == spng.SPNG_COLOR_TYPE_GRAYSCALE_ALPHA then
            out_fmt = spng.SPNG_FMT_G8
        -- elseif ihdr.color_type == spng.SPNG_COLOR_TYPE_INDEXED and state[0].info_png.color.palettesize <= 16 then
        --     -- If input is sRGB, but paletted to 16c or less, assume it's the eInk palette, and honor it.
        --     -- Just expand it to grayscale so BB knows what to do with it ;).
        --     -- NOTE: A properly encoded image targeting eInk should actually be both dithered down to the 16c eInk palette,
        --     --       AND flagged color-type 0 (Grayscale) too! Those already fall under the first branch ;).
        --     --       As such, this only affects stuff explicitly encoded color-type 3 (Paletted sRGB).
        --     out_fmt = spng.SPNG_FMT_G8
        else
            out_fmt = spng.SPNG_FMT_RGB8
            -- Don't forget to update out_n so the caller is aware of the conversion
            out_n = 3
        end
    elseif req_n == 2 then
        if ihdr.color_type == spng.SPNG_COLOR_TYPE_GRAYSCALE_ALPHA then
            out_fmt = spng.SPNG_FMT_PNG
        elseif ihdr.color_type == spng.SPNG_COLOR_TYPE_GRAYSCALE then
            out_fmt = spng.SPNG_FMT_GA8
        -- elseif ihdr.color_type == lodepng.LCT_PALETTE and state[0].info_png.color.palettesize <= 16 then
        --     -- If input is sRGB, but paletted to 16c or less, assume it's the eInk palette, and honor it.
        --     -- Just expand it to grayscale w/ alpha so BB knows what to do with it ;).
        --     out_fmt = spng.SPNG_FMT_GA8
        else
            out_fmt = spng.SPNG_FMT_RGBA8
            -- Don't forget to update out_n so the caller is aware of the conversion
            out_n = 4
        end
    elseif req_n == 3 then
        out_fmt = spng.SPNG_FMT_RGB8
    elseif req_n == 4 then
        out_fmt = spng.SPNG_FMT_RGBA8
    else
        return false, "requested an invalid number of color components"
    end

    local out_size = ffi.new("size_t[1]")

    err = spng.spng_decoded_image_size(ctx, out_fmt, out_size)
    if err ~= 0 then
        return false, spng_strerror(err)
    end

    local out_ptr = C.malloc(out_size[0])
    if out_ptr == nil then
        return false, posix.strerror()
    end

    err = spng.spng_decode_image(ctx, out_ptr, out_size[0], out_fmt, spng.SPNG_DECODE_TRNS)
    if err ~= 0 then
        C.free(out_ptr)
        return false, spng_strerror(err)
    end

    spng.spng_ctx_free(ffi.gc(ctx, nil))
    C.fclose(ffi.gc(fp, nil))

    return true, {
        width = ihdr.width,
        height = ihdr.height,
        data = out_ptr,
        ncomp = out_n or req_n,
    }
end

return Png
