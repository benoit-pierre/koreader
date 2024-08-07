-- vim: ft=lua

local lfs = require("lfs")

local testsuites = {
    _all = {
        helper = "setupkoenv.lua",
        output = "gtest",
        verbose = true,
    },
}

local roots = {}
local lpaths = {}
for entry in lfs.dir("spec") do
    local testroot = "spec/" .. entry .. "/unit"
    if lfs.attributes(testroot) ~= nil then
        local testpath = testroot .. "/?.lua"
        testsuites[entry] = {}
        testsuites[entry].ROOT = {testroot}
        testsuites[entry].lpath = testpath
        table.insert(roots, testroot)
        table.insert(lpaths, testpath)
    end
end

testsuites.all = {
    ROOT = roots,
    lpath = table.concat(lpaths, ";"),
}

return testsuites
