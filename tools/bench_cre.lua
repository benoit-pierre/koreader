require "setupkoenv"

local SQ3 = require("lua-ljsqlite3/init")
local ffiutil = require("ffi/util")
local lfs = require("libs/libkoreader-lfs")
local sha1 = require("ffi/sha2").sha1
local util = require("util")

-- delayed imports…
local DataStorage
local Device
local DocumentRegistry

local VERSION = require('version'):getCurrentRevision()

local STATS_DB_SCHEMA = [[
    PRAGMA busy_timeout = 1000;

    CREATE TABLE IF NOT EXISTS stats (
        method           TEXT,
        duration         INTEGER,
        version          TEXT,
        raw_text_size    INTEGER,
        raw_text_length  INTEGER,
        cache_sha1       TEXT,
        directory        TEXT,
        filename         TEXT,
        filesize         INTEGER
    );
    ]]

local STATS_COLS = {
    'method',
    'duration',
    'version',
    'raw_text_size',
    'raw_text_length',
    'cache_sha1',
    'directory',
    'filename',
    'filesize'
}

local SAVE_STATS_SQL = (
    'INSERT INTO stats (' .. table.concat(STATS_COLS, ', ') ..
    ') VALUES (?' .. string.rep(', ?', #STATS_COLS - 1) .. ');'
)

local STYLE_TWEAKS = [[
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
*, autoBoxing {
    -cr-hint: late;
    -cr-only-if: -fb2-document inpage-footnote;
        font-size: 0.8rem !important;
}
]]

local LOADING_MODE = {
    minimal = false,
    -- crc32 = 2,
    raw_text_size = 2,
    raw_text_length_slow = 4,
    raw_text_length_fast = 8,
    full_load = true,
    render = true,
}

local function find_files(path, recursive, include_unsupported)
    local dirs = {path}
    local files = {}
    while #dirs ~= 0 do
        local new_dirs = {}
        for _, d in pairs(dirs) do
            -- handle files in d
            for f in lfs.dir(d) do
                local fullpath = d.."/"..f
                local attributes = lfs.attributes(fullpath)
                if recursive and attributes.mode == "directory" and not util.stringStartsWith(f, ".") then
                    table.insert(new_dirs, fullpath)
                -- Always ignore macOS resource forks, too.
                elseif attributes.mode == "file" and not util.stringStartsWith(f, "._") then
                    if include_unsupported or DocumentRegistry:hasProvider(fullpath) then
                        table.insert(files, fullpath)
                    end
                end
            end
        end
        dirs = new_dirs
    end
    return files
end

local function override_cr3cache(dir)
    local cre = require('document/credocument'):engineInit()
    ffiutil.purgeDir(dir)
    cre.initCache(dir, 0, true, 40)
end

local Benchmark = {
    initialized = false,
}

local function term_size()
    local f = io.popen("stty size", "r")
    local out = f:read("*a")
    f:close()
    local row, cols = out:match("(%d+) (%d+)")
    return tonumber(row), tonumber(cols)
end

local term_cols

local function pad_to_term_width(s)
    if not term_cols then
        term_cols = select(2, term_size())
    end
    if #s >= term_cols then
        return s
    end
    return s .. string.rep(" ", term_cols - #s)
end

function Benchmark:init(full)
    if self.initialized then
        return
    end

    if full then
        G_defaults = require("luadefaults"):open()
        DataStorage = require("datastorage")
        G_reader_settings = require("luasettings"):open(DataStorage:getDataDir().."/settings.reader.lua")
        Device = require("device")
        require("document/canvascontext"):init(Device)
    else
        DataStorage = require("datastorage")
        Device = require("device")
    end

    DocumentRegistry = require("document/documentregistry")

    -- self.db_location = DataStorage:getSettingsDir() .. "/benchmark_stats.sqlite3"
    -- self.tmpcr3cache = DataStorage:getDataDir() .. '/cache/tmpcr3cache'
    self.db_location = "/tmp/benchmark_stats.sqlite3"
    self.tmpcr3cache = '/tmp/tmpcr3cache'
    self.cr3cachesaves = '/tmp/cr3cache/'
    assert(lfs.attributes(self.cr3cachesaves) or lfs.mkdir(self.cr3cachesaves))
    self.cr3cachesaves = self.cr3cachesaves .. VERSION
    assert(lfs.attributes(self.cr3cachesaves) or lfs.mkdir(self.cr3cachesaves))
    local db_conn = SQ3.open(self.db_location)
    db_conn:exec(STATS_DB_SCHEMA)
    self.db_conn = db_conn
    self.save_stats_stmt = self.db_conn:prepare(SAVE_STATS_SQL)

    self.initialized = true
end

function Benchmark:extractBookInfo(filepath, metadata_extraction)

    local directory, filename = util.splitFilePathName(filepath)
    local dbrow = { }

    dbrow.directory = directory
    dbrow.filename = filename

    local file_attr = lfs.attributes(filepath)
    dbrow.filesize = file_attr.size
    dbrow.filemtime = file_attr.modification

    local load_time
    local render_time
    local document = DocumentRegistry:openDocument(filepath)
    local loaded = true
    if document then
        document:setStyleSheet('./data/epub.css', STYLE_TWEAKS)
        if document.loadDocument then -- needed for crengine
            local loading_mode = LOADING_MODE[metadata_extraction]
            if document:loadDocument(loading_mode) then
                load_time = document._document:getStringProperty("doc.load.time")
                local props = document:getProps()
                if string.find(metadata_extraction or '', '^raw_text_length') then
                    dbrow.raw_text_length = props.raw_text_length
                elseif metadata_extraction == 'raw_text_size' then
                    dbrow.raw_text_size = props.raw_text_size
                elseif metadata_extraction == 'render' then
                    document:render()
                    render_time = document._document:getStringProperty("doc.render.time")
                end
            else
                -- failed loading, calling other methods would segfault
                print('loading '..filepath..' failed')
                loaded = false
            end
        end
        document:close()
    else
        loaded = false
    end

    if metadata_extraction == 'render' then
        dbrow.method = 'full_load'
    else
        dbrow.method = metadata_extraction
    end
    dbrow.duration = load_time
    if metadata_extraction == 'render' then
        for i, cache_entry in ipairs(find_files(self.tmpcr3cache, false, true)) do
            local _, name = util.splitFilePathName(cache_entry)
            if name ~= 'cr3cache.inx' then
                assert(not dbrow.cache_sha1, filepath)
                local f = io.open(cache_entry, 'rb')
                dbrow.cache_sha1 = sha1(f:read('*a'))
                f:close()
            end
        end
    end
    dbrow.version = VERSION
    for num, col in ipairs(STATS_COLS) do
        self.save_stats_stmt:bind1(num, dbrow[col])
    end
    self.save_stats_stmt:step()
    if metadata_extraction == 'render' then
        assert(STATS_COLS[1] == 'method')
        assert(STATS_COLS[2] == 'duration')
        self.save_stats_stmt:reset()
        self.save_stats_stmt:bind1(1, 'render')
        self.save_stats_stmt:bind1(2, render_time)
        self.save_stats_stmt:step()
    end
    self.save_stats_stmt:clearbind():reset()

    return loaded
end

function Benchmark:purgeStats(method)
    self:init()
    if method then
        self.db_conn:prepare('DELETE FROM stats WHERE method = ?;'):reset():bind(method):step()
    else
        self.db_conn:exec('DELETE FROM stats;')
    end
end

function Benchmark:run(path_list, extraction_methods, recursive, purge_stats)

    self:init()

    local save_cache_entries = false
    local refresh_frequency = 1
    if not extraction_methods or #extraction_methods == 0 then
        extraction_methods = {
            -- {method = 'minimal', iterations = 15},
            -- {method = 'full_load', iterations = 25},
            {method = 'render', iterations = 20},
            -- {method = 'crc32', iterations = 10},
            -- {method = 'raw_text_size', iterations = 3},
            -- {method = 'raw_text_length_slow', iterations = 3},
            -- {method = 'raw_text_length_fast', iterations = 5},
            -- {method = 'minimal', iterations = 1},
            -- {method = 'raw_text_size', iterations = 1},
            -- {method = 'raw_text_length_slow', iterations = 1},
        }
    end

    local info_text
    local info_text_stack = {}
    local function push_info()
        table.insert(info_text_stack, info_text)
    end
    local function pop_info()
        info_text = table.remove(info_text_stack)
    end
    local function show_info(s)
        if s then
            info_text = s
        end
    end

    if purge_stats then
        self:purgeStats()
    end

    local all_files = {}
    for i, path in ipairs(type(path_list) == "string" and {path_list} or path_list) do
        if lfs.attributes(path).mode == 'file' then
            table.insert(all_files, path)
        else
            -- Strip trailing '/'
            local len = #path
            while string.byte(path, len) == 47 do
                len = len - 1
            end
            path = string.sub(path, 0, len)
            local files = find_files(path, recursive)
            table.sort(files)
            for j, f in ipairs(files) do
                table.insert(all_files, f)
            end
        end
    end

    info_text = string.format('Benchmarking (%u files):\n\n', #all_files)
    local total_start_time = os.time()
    for _, test in ipairs(extraction_methods) do
        push_info()
        local progress = {
            step = 0,
            count = 0,
            filename = '',
        }
        local timestamp
        local start_time = os.time()
        local function update_info(final)
            local new_timestamp = os.time()
            if not final and timestamp and os.difftime(new_timestamp, timestamp) < refresh_frequency then
                return
            end
            timestamp = new_timestamp
            local elapsed = os.difftime(timestamp, start_time)
            local minutes = math.floor(elapsed / 60)
            local seconds = elapsed - minutes * 60
            local percent
            local filename
            if final then
                percent = 100
                filename = ''
            else
                percent = ((progress.step-1) * #all_files + progress.count-1)*100/(#all_files*test.iterations)
                filename = '  [' .. progress.filename .. ']'
            end
            local progress_text = string.format(
                '%s %3u/%u %2u/%u %5.1f%% %2um%02us%s',
                test.method,
                progress.step, test.iterations,
                progress.count, #all_files,
                percent, minutes, seconds,
                filename)
            pop_info()
            push_info()
            show_info(info_text .. ' ' .. progress_text)
            io.stdout:write('\r'..pad_to_term_width(progress_text))
            io.stdout:flush()
        end
        local function task (send)
            for step = 1, test.iterations do
                override_cr3cache(self.tmpcr3cache)
                for count, filepath in ipairs(all_files) do
                    -- print(filepath)
                    local _, filename = util.splitFilePathName(filepath)
                    send{count = count, step = step, filename = filename}
                    if not self:extractBookInfo(filepath, test.method) then
                        return
                    end
                    for f in lfs.dir(self.tmpcr3cache) do
                        if f ~= "." and f ~= ".." then
                            local src = self.tmpcr3cache .. "/" .. f
                            if save_cache_entries and f:match("%.cr3$") then
                                local copy_count = 1
                                while true do
                                    local dst = string.format("%s/%s.%u", self.cr3cachesaves, f, copy_count)
                                    if not lfs.attributes(dst) then
                                        assert(os.rename(src, dst))
                                        break
                                    end
                                    copy_count = copy_count + 1
                                end
                            else
                                assert(os.remove(src))
                            end
                        end
                    end
                end
            end
        end
        local function callback(new_progress)
            if new_progress then
                progress = new_progress
            end
            update_info()
        end
        task(function (n) callback(n) callback() end)
        update_info(true)
        show_info(info_text .. '\n')
        io.stdout:write('\n')
    end

    local elapsed = os.difftime(os.time(), total_start_time)
    local time_text = string.format('TOTAL: %us', elapsed)
    show_info(info_text .. '\n' .. time_text)
    io.write(time_text .. '\n')
end

function Benchmark:run_from_prompt()
    Benchmark:init(true)
    local path_list = {}
    local options = {
        recursive = true,
        purge_stats = false,
    }
    local extraction_methods = {}
    for i, a in ipairs(arg) do
        local k, v = a:match("^([a-z_]*)=(.*)")
        if k then
            if options[k] ~= nil then
                options[k] = v == "1" or v == "on" or v == "true" or v == "yes"
            else
                table.insert(extraction_methods, {method = k, iterations = 0 + v})
            end
        else
            table.insert(path_list, a)
        end
    end
    local success = Benchmark:run(path_list, extraction_methods, options.recursive, options.purge_stats)
    os.exit(success and 0 or 1)
end

Benchmark:run_from_prompt()
