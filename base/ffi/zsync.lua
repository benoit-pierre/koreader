local buffer = require("string.buffer")
local ffi = require("ffi")
local lfs = require("libs/libkoreader-lfs")
local posix = require("ffi/posix")

require "ffi/zsync_h"

local C = ffi.C
local zsync = ffi.loadlib("zsync")

-- Updater. {{{

local Updater = {}

function Updater:new(manifest_url, state_dir, seed)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    local Downloader = require("ffi/downloader")
    o.dl = Downloader:new()
    o.manifest_url = manifest_url
    o.seed = seed
    o.state_dir = state_dir
    o.update_url = nil
    o.zs_receiver = nil
    o.zs_state = nil
    return o
end

function Updater:free()
    if self.zs_receiver ~= nil then
        zsync.zsync_end_receive(self.zs_receiver)
    end
    if self.zs_state ~= nil then
        zsync.zsync_end(self.zs_state)
    end
    self.dl:free()
    self.dl = nil
    self.seed = nil
    self.state_dir = nil
    self.update_url = nil
    self.zs_receiver = nil
    self.zs_state = nil
end

function Updater:fetch_manifest()
    local url = require("socket.url")
    local url_path = url.parse(self.manifest_url).path
    local basename = table.remove(url.parse_path(url_path))
    local manifest_file = self.state_dir.."/"..basename
    local etag_file = manifest_file..".etag"
    local etag
    -- Load existing manifest if present in state directory.
    if lfs.attributes(manifest_file, "mode") == "file" then
        local fp = io.open(manifest_file, "rb")
        self.zs_state = zsync.zsync_begin(fp, 0, self.state_dir)
        fp.close()
    end
    -- And the associated ETag if applicable.
    if self.zs_state ~= nil and lfs.attributes(etag_file, "mode") == "file" then
        etag = io.lines(etag_file)()
    end
    -- Try fetching an updated version.
    local buf = buffer:new()
    if not self.dl:fetch(self.manifest_url, function(ptr, len)
        buf:putcdata(ptr, len)
        return true
    end, nil, etag) then
        error(self.dl.err)
    end
    if self.dl.status_code ~= 304 then -- 304: Not Modified.
        local fp = io.open(manifest_file, "wb")
        fp:write(buf)
        fp:close()
        -- Update ETag file.
        if self.dl.etag then
            fp = io.open(etag_file, "w")
            fp:write(self.dl.etag)
            fp:close()
        else
            os.remove(etag_file)
        end
        fp = posix.fopen(manifest_file)
        self.zs_state = zsync.zsync_begin(fp, 0, self.state_dir)
        C.fclose(fp)
        if self.zs_state == nil then
            error("zsync_begin failed")
        end
    end
    if zsync.zsync_hint_decompress(self.zs_state) ~= 0 then
        error("zsync_hint_decompress returned non zero")
    end
    self.filename = ffi.string(ffi.gc(zsync.zsync_filename(self.zs_state), C.free))
    self.update_url = url.absolute(self.manifest_url, self.filename)
    return self.filename
end

function Updater:prepare_update(progress_cb)
    local reused_size = 0
    if self.seed then
        if lfs.attributes(self.seed, "mode") ~= "file" then
            error("seed is not a file!")
        end
        local fp = posix.fopen(self.seed)
        reused_size = tonumber(zsync.zsync_submit_source_file(self.zs_state, fp, progress_cb and function(got, total)
            return progress_cb(tonumber(got), tonumber(total))
        end))
        C.fclose(fp)
    end
    local nb_ranges = ffi.new('int[1]')
    local raw_ranges = zsync.zsync_needed_byte_ranges(self.zs_state, nb_ranges, 0)
    local download_size = 0
    local ranges = {}
    for n = 0, nb_ranges[0] - 1 do
        local start, stop = tonumber(raw_ranges[n * 2]), tonumber(raw_ranges[n * 2 + 1])
        assert(stop > start)
        table.insert(ranges, {start, stop})
        download_size = download_size + stop - start + 1
    end
    self.download_size = download_size
    self.ranges = ranges
    return {
        reused_size = reused_size,
        download_size = download_size,
    }
end

function Updater:download_update(progress_cb)
    self.zs_receiver = zsync.zsync_begin_receive(self.zs_state, 0)
    local range_index = 0
    local downloaded = 0
    local offset = 0
    local size = 0
    if not self.dl:fetch(self.update_url, function(ptr, len)
        while len > 0 do
            if size == 0 then
                range_index = range_index + 1
                offset = self.ranges[range_index][1]
                size = self.ranges[range_index][2] - offset + 1
            end
            local count = math.min(len, size)
            if zsync.zsync_receive_data(self.zs_receiver, ptr, offset, count) ~= 0 then
                return false
            end
            downloaded = downloaded + count
            if progress_cb and not progress_cb(downloaded, self.download_size) then
                return false
            end
            offset = offset + count
            size = size - count
            ptr = ptr + count
            len = len - count
        end
        return true
    end, self.ranges) then
        error(self.dl.err)
    end
    if zsync.zsync_receive_data(self.zs_receiver, nil, offset, 0) ~= 0
        or zsync.zsync_status(self.zs_state) ~= 2
        or zsync.zsync_complete(self.zs_state) ~= 1 then
        error("zsync download failed")
    end
    zsync.zsync_end_receive(self.zs_receiver)
    self.zs_receiver = nil
    local tmpfile = ffi.string(ffi.gc(zsync.zsync_end(self.zs_state), C.free))
    self.zs_state = nil
    local ok, err = os.rename(tmpfile, self.state_dir .. "/" .. self.filename)
    if not ok then
        error(err)
    end
end

-- }}}

return {
    Updater = Updater,
}
