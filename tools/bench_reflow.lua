require "setupkoenv"

local ffi = require("ffi")
local lfs = require("libs/libkoreader-lfs")
local sha1 = require("ffi/sha2").sha1
local C = ffi.C

require "ffi/posix_h"

local DataStorage
local Device
local DocCache
local DocumentRegistry

C.setenv("KO_HOME", "/tmp/kocache", 1)

G_defaults = require("luadefaults"):open()
DataStorage = require("datastorage")
G_reader_settings = require("luasettings"):open(DataStorage:getDataDir().."/settings.reader.lua")
Device = require("device")
require("document/canvascontext"):init(Device)

DocCache = require("document/doccache")

DocumentRegistry = require("document/documentregistry")

local function pause()
    print("press a key to continue")
    io.stdin:read("*l")
end

local function dump_cache()
    DocCache:serialize(sample_pdf)
    local cache = {}
    for f in lfs.dir(DataStorage:getDataDir().."/cache") do
        if f ~= "." and f ~= ".." and f:match("^[0-9a-f]+$") then
            table.insert(cache, f)
        end
    end
    table.sort(cache)
    print(sha1(table.concat(cache, "\n")))
end

local function clear_cache()
    DocCache:clear()
    local cache_dir = DataStorage:getDataDir() .. "/cache"
    for f in lfs.dir(cache_dir) do
        if f ~= "." and f ~= ".." and f:match("^[0-9a-f]+$") then
            assert(os.remove(cache_dir .. "/" .. f))
        end
    end
    DocCache:refreshSnapshot()
end

local function term_size()
    local f = io.popen("stty size", "r")
    local out = f:read("*a")
    f:close()
    local row, cols = out:match("(%d+) (%d+)")
    return tonumber(row), tonumber(cols)
end

local term_cols = select(2, term_size())

local function pad_to_term_width(s)
    if #s >= term_cols then
        return s
    end
    return s .. string.rep(" ", term_cols - #s)
end

local sample_pdf = "spec/front/unit/data/sample.pdf"
local doc = DocumentRegistry:openDocument(sample_pdf)
doc.configurable.text_wrap = 1
local loops = 100
for l = 1, loops do
    clear_cache()
    -- pause()
    for pageno = 1, doc.info.number_of_pages do
        local progress = string.format("loop: %d/%d, page: %d/%d ", l, loops, pageno, doc.info.number_of_pages)
        io.stdout:write("\r"..pad_to_term_width(progress))
        io.stdout:flush()
        doc:renderPage(pageno, nil, 1, 0, 1.0, false)
    end
    print()
    dump_cache()
    -- pause()
end
doc:close()
