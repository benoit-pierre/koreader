list(APPEND PATCH_FILES cmake_tweaks.patch)

list(APPEND CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    # Project options.
    -DBUILD_TESTING=FALSE
    -DWITH_GTEST=FALSE
    -DZLIB_COMPAT=TRUE
)
if(MONOLIBTIC)
    list(APPEND CMAKE_ARGS -DBUILD_SHARED_LIBS=FALSE)
endif()

list(APPEND BUILD_CMD COMMAND ninja)

list(APPEND INSTALL_CMD COMMAND ${CMAKE_COMMAND} --install .)

if(NOT MONOLIBTIC)
    append_shared_lib_install_commands(INSTALL_CMD z VERSION 1)
endif()

external_project(
    DOWNLOAD URL 72337e6a7d2662af50a4ed0274c61b7e
    https://github.com/zlib-ng/zlib-ng/archive/refs/tags/2.3.3.tar.gz
    PATCH_FILES ${PATCH_FILES}
    CMAKE_ARGS ${CMAKE_ARGS}
    BUILD_COMMAND ${BUILD_CMD}
    INSTALL_COMMAND ${INSTALL_CMD}
)
