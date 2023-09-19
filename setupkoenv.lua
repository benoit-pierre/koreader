local ffi = require("ffi")
local android = ffi.os == "Linux" and os.getenv("IS_ANDROID") and require("android")
local log = android and android.LOGI or print

local has_libkoreader, libkoreader = pcall(require, "libkoreader")
local lib_search_path
local lib_basic_format
local lib_version_format
if android then
    -- Note: our libraries are not versioned on Android.
    lib_search_path = android.dir .. "/libs/?"
    lib_basic_format = "lib%s.so"
    lib_version_format = "lib%s.so"
elseif ffi.os == "Linux" then
    -- Note: we need the `libs/lib?` variant for versioned libraries
    -- (e.g. `ffi.loadlib("utf8proc", 2)` will trigger a search for
    -- `utf8proc.so.2` on Linux).
    lib_search_path = "libs/?"
    lib_basic_format = "lib%s.so"
    lib_version_format = "lib%s.so.%s"
elseif ffi.os == "OSX" then
    lib_search_path = "libs/?"
    lib_basic_format = "lib%s.dylib"
    lib_version_format = "lib%s.%s.dylib"
end

log("has libkoreader? " .. (has_libkoreader and 'yes' or 'no'))
log("lib_search_path: " .. lib_search_path)
log("lib_basic_format: " .. lib_basic_format)
log("lib_version_format: " .. lib_version_format)

local function libname(name, version)
    return string.format(version and lib_version_format or lib_basic_format, name, version)
end

local function findlib(...)
    local name, version = ...
    if not name then
        return
    end
    log("ffi.findlib: " .. name .. (version and ("." .. version) or ""))
    if has_libkoreader then
        local key = version and (name .. '.' .. version) or name
        if libkoreader.redirects[key] then
            return libkoreader.library
        end
    end
    local lib = libname(name, version)
    local path = package.searchpath(lib, lib_search_path, "/", "/")
    if path then
        return path
    end
    return findlib(select(3, ...))
end

local ffi_load = ffi.load
ffi.load = function(lib, global)
    log("ffi.load: " .. lib .. (global and " (RTLD_GLOBAL)" or ""))
    if android then
        return android.dl.dlopen(lib, ffi_load, global)
    end
    return ffi_load(lib, global)
end
ffi.loadlib = function(...)
    local lib = findlib(...) or libname(...)
    return ffi.load(lib)
end
