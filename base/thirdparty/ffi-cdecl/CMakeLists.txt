# Build in source tree.
set(BINARY_DIR ${SOURCE_DIR})

list(APPEND PATCH_FILES ffi-cdecl.patch)

set(MAKE_CMD make CROSSCC=${GCC} CROSSCXX=${GXX} HOSTCC=${GCC})
if(EMULATE_READER)
    # Native compiler, use our own luajit.
    set(LUACFLAGS "-I${STAGING_DIR}/include/luajit-2.1")
    set(LUALIBS "${STAGING_DIR}/lib/libluajit-5.1.a -ldl -lm")
else()
    # We're building for a cross-compiler, let's hope Lua is installed
    # in the standard locations (as pkg-config can't be used since
    # we've overriden its environment in `Makefile.defs`). 🤞
    find_path(
        LUA_INCLUDE_DIR
        NAMES lua.h
        PATHS /usr/local/include /usr/include
        PATH_SUFFIXES luajit-2.1 lua5.4 lua5.3 lua5.2
        NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH
        NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH NO_CMAKE_INSTALL_PREFIX NO_CMAKE_FIND_ROOT_PATH
    )
    find_library(
        LUA_LIBRARIES
        NAMES libluajit-5.1.so liblua5.4.so liblua5.3.so liblua5.2.so liblua.so
        PATHS /usr/local/lib /usr/lib
        PATH_SUFFIXES x86_64-linux-gnu
        NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH
        NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH NO_CMAKE_INSTALL_PREFIX NO_CMAKE_FIND_ROOT_PATH
    )
    if(LUA_INCLUDE_DIR AND LUA_LIBRARIES)
        set(LUACFLAGS "-I${LUA_INCLUDE_DIR}")
        set(LUALIBS "${LUA_LIBRARIES}")
    else()
        # Last chance fallback…
        set(LUACFLAGS "")
        set(LUALIBS "-llua")
    endif()
endif()
list(APPEND MAKE_CMD LUAMOD= LUACFLAGS=${LUACFLAGS} LUALIBS=${LUALIBS})
if(APPLE)
    list(APPEND MAKE_CMD "CPPFLAGS=-I${BREW_PREFIX}/include -DENABLE_BUILD_WITH_CXX" "INSTALL_L=install -m 644")
endif()

list(APPEND PATCH_CMD COMMAND ${MAKE_CMD} patch)

list(APPEND BUILD_CMD COMMAND ${MAKE_CMD})

# Install GCC plugin.
list(APPEND INSTALL_CMD COMMAND ${MAKE_CMD} -C gcc-lua install DESTDIR=${STAGING_DIR} INSTALL_GCC_PLUGIN=/lib/gcc/plugin)

# Install Lua code.
set(INSTALL_LUADIR ${STAGING_DIR}/share/lua/5.1)
append_install_commands(INSTALL_CMD ffi-cdecl.lua DESTINATION ${INSTALL_LUADIR})
append_install_commands(INSTALL_CMD gcc-lua-cdecl/ffi-cdecl/ffi-cdecl.lua DESTINATION ${INSTALL_LUADIR}/ffi-cdecl)
append_install_commands(INSTALL_CMD gcc-lua-cdecl/gcc/cdecl.lua DESTINATION ${INSTALL_LUADIR}/gcc)

# Install headers.
append_install_commands(
    INSTALL_CMD DESTINATION ${STAGING_DIR}/include/ffi-cdecl
    gcc-lua-cdecl/ffi-cdecl/C.c
    gcc-lua-cdecl/ffi-cdecl/C99.c
    gcc-lua-cdecl/ffi-cdecl/ffi-cdecl.h
)

set(TARGET_FLAGS "")
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(TARGET_FLAGS "-fno-blocks")
endif()
set(COMMON_CFLAGS "-std=gnu11 -fno-builtin")
set(COMMON_CXXFLAGS "-std=gnu++17 -fno-builtin")
set(GCC_FLAGS "-Wno-attributes -D_Nonnull='' -D_Nullable=''")
if(NOT EMULATE_READER)
    # Add -m32 when cross compile on a 64bit host for 32bit target.
    execute_process(COMMAND ${GCC} -dumpmachine OUTPUT_VARIABLE GCC_MACHINE)
    if(GCC_MACHINE MATCHES "^.+64-.+$" AND NOT CMAKE_SYSTEM_PROCESSOR MATCHES ".+64$")
        string(APPEND GCC_FLAGS  " -m32")
    endif()
endif()

# Install helper.
list(APPEND INSTALL_CMD COMMAND mkdir -p ${STAGING_DIR}/bin)
list(APPEND INSTALL_CMD COMMAND
    sh -c "in=\"$1\" out=\"$2\" && shift 2 && sed \"$@\" <\"$in\" >\"$out\"" sed
    ${CMAKE_CURRENT_SOURCE_DIR}/ffi-cdecl.sh.in ${STAGING_DIR}/bin/ffi-cdecl
    -e "s,@CC@,${CMAKE_C_COMPILER} ${COMMON_CFLAGS} ${TARGET_FLAGS},"
    -e "s,@CXX@,${CMAKE_CXX_COMPILER} ${COMMON_CXXFLAGS} ${TARGET_FLAGS},"
    -e "s,@GCC@,${GCC} ${COMMON_CFLAGS} ${GCC_FLAGS},"
    -e "s,@GXX@,${GXX} ${COMMON_CXXFLAGS} ${GCC_FLAGS},"
    -e "s,@PREFIX@,${STAGING_DIR},"
    -e "s,@PLUGIN@,${STAGING_DIR}/lib/gcc/plugin/gcclua.so,"
    -e "s,@SCRIPT@,${INSTALL_LUADIR}/ffi-cdecl.lua,"
)
list(APPEND INSTALL_CMD COMMAND chmod +x ${STAGING_DIR}/bin/ffi-cdecl)

external_project(
    DOWNLOAD GIT 712ab4cda30fb19498f18b01340c2d8862e03694
    https://github.com/koreader/ffi-cdecl.git
    PATCH_FILES ${PATCH_FILES}
    PATCH_COMMAND ${PATCH_CMD}
    BUILD_COMMAND ${BUILD_CMD}
    INSTALL_COMMAND ${INSTALL_CMD}
)
