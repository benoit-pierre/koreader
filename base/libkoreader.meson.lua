local library = '@library@'
local lua_modules = {
    @lua_modules@
}
local ffi_libraries = {
    @ffi_libraries@
}

local function loader(modulename)
    local fn = lua_modules[modulename]
    if fn then
        return package.loadlib(library, fn)
    end
end

table.insert(package.loaders, 1, loader)

return {
    library = library,
    redirects = ffi_libraries,
}
