require "setupkoenv"

G_defaults = require("luadefaults"):open()
local DataStorage = require("datastorage")
G_reader_settings = require("luasettings"):open(DataStorage:getDataDir().."/settings.reader.lua")
local Device = require("device")
local Screen = Device.screen
require("document/canvascontext"):init(Device)

local cre = require('document/credocument'):engineInit()
local ffiutil = require("ffi/util")
local lfs = require("libs/libkoreader-lfs")
local sha1 = require("ffi/sha2").sha1

local DocumentRegistry = require("document/documentregistry")
local ReaderUI
local UIManager

local use_reader_ui = false

if use_reader_ui then
    ReaderUI = require("apps/reader/readerui")
    UIManager = require("ui/uimanager")
end

local function fastforward_ui_events()
    -- Fast forward all scheduled tasks.
    UIManager:shiftScheduledTasksBy(-1e9)
    UIManager:run()
end

local cachedir = "/tmp/cr3cache"

local function dump_cr3_cache_manifest()
    print('crengine cache contents:')
    if not lfs.attributes(cachedir) then
        return
    end
    local entries = {}
    for entry in lfs.dir(cachedir) do
        if entry ~= 'cr3cache.inx' and not entry:match('^[.]') then
            table.insert(entries, entry)
        end
    end
    table.sort(entries)
    local manifest = {}
    for i, entry in ipairs(entries) do
        local f = io.open(cachedir.."/"..entry, 'rb')
        table.insert(manifest, string.format('  %s  %s', sha1(f:read('*a')), entry))
        f:close()
    end
    manifest = table.concat(manifest, '\n')
    print(manifest)
    print(string.format('  %s', sha1(manifest)))
end

local function reset_cr3_cache()
    print('reseting crengine cache')
    ffiutil.purgeDir(cachedir)
    cre.initCache(cachedir, 64*1024*1024, true, 40)
end

-- Koreader
local cre_style_sheet = './data/epub.css'
local cre_style_tweaks = [[
*[type~="note"],
*[type~="endnote"],
*[type~="footnote"],
*[type~="rearnote"],
*[role~="doc-note"],
*[role~="doc-endnote"],
*[role~="doc-footnote"],
*[role~="doc-rearnote"]
{
    -cr-only-if: -fb2-document;
        -cr-hint: footnote-inpage;
        margin: 0 !important;
        font-size: 0.8rem !important;
}
body[name="notes"] section {
    -cr-only-if: fb2-document;
        -cr-hint: footnote-inpage;
        margin: 0 !important;
}
body[name="notes"] > section {
    -cr-only-if: fb2-document;
        font-size: 0.75rem;
}
body[name="notes"] > title {
    -cr-only-if: fb2-document;
        margin-bottom: 0;
        padding-bottom: 0.5em;
}
]]

local function load_document(b, no_render)
    local doc = DocumentRegistry:openDocument(b)
    if not doc then
        return
    end
    if not use_reader_ui then
        doc:setStyleSheet(cre_style_sheet, cre_style_tweaks)
        doc:setEmbeddedStyleSheet(1)
        doc:loadDocument()
        if not no_render and doc.render then
            doc:render()
        end
        doc:close()
        return
    end
    local readerui = ReaderUI:new{
        dimen = Screen:getSize(),
        document = doc,
    }
    UIManager:quit()
    UIManager:show(readerui)
    UIManager:scheduleIn(1, function()
        UIManager:close(readerui)
        ReaderUI.instance = readerui
    end)
    fastforward_ui_events()
    readerui:closeDocument()
    readerui:onClose()
    readerui = nil
    UIManager:quit()
    UIManager._exit_code = nil
end

local runs = 1
for r = 1, runs do
    reset_cr3_cache()
    -- Note: do 2 passes (to exercise loading from cache).
    for i = 0, #arg * 2 - 1 do
        local n = (i % #arg) + 1
        local b = arg[n]
        local caching = i < #arg and 'cold' or 'hot'
        io.stdout:write(string.format("%d/%d %d/%d %s [%s]", r, runs, n, #arg, b, caching))
        io.stdout:flush()
        load_document(b)
        if #arg == n then
            dump_cr3_cache_manifest()
        end
    end
end

Device:exit()
