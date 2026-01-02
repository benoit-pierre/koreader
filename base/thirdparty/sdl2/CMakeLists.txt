list(APPEND PATCH_FILES cmake_tweaks.patch)

list(APPEND CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    # Project options.
    -DSDL2COMPAT_TESTS=OFF
)

list(APPEND BUILD_CMD COMMAND ninja)

list(APPEND INSTALL_CMD COMMAND ${CMAKE_COMMAND} --install .)

append_shared_lib_install_commands(INSTALL_CMD SDL2-2.0 VERSION 0)

external_project(
    DOWNLOAD URL 87a86bb196197eaa701bd60c06c2de72
    https://github.com/libsdl-org/sdl2-compat/releases/download/release-2.32.60/sdl2-compat-2.32.60.tar.gz
    PATCH_FILES ${PATCH_FILES}
    CMAKE_ARGS ${CMAKE_ARGS}
    BUILD_COMMAND ${BUILD_CMD}
    INSTALL_COMMAND ${INSTALL_CMD}
)
