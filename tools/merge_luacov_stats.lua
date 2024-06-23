#!./luajit

local stats = require("luacov.stats")
local runner = require("luacov.runner")

local outfile = nil
local aggregated = {}

for i, a in ipairs(arg) do
    if outfile then
        for name, file_data in pairs(stats.load(a)) do
            if aggregated[name] then
                runner.update_stats(aggregated[name], file_data)
            else
                aggregated[name] = file_data
            end
        end
    else
        outfile = a
    end
end
stats.save(outfile, aggregated)
