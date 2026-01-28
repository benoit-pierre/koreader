list(APPEND PATCH_FILES
    cmake_tweaks.patch
    # Remove workarounds for standalone applications and add
    # a couple of actions to be used from the osx main menu.
    cocoa.patch
)
if(APPIMAGE OR EMULATE_READER)
    list(APPEND PATCH_FILES pkg-config_jailbreak.patch)
endif()
if(APPIMAGE)
    # Tweak pkg-config search path (allow system packages),
    # help stupid cmake find system libraries, and fail if
    # required features / subsystems can't be enabled.
    list(APPEND PATCH_FILES appimage.patch)
endif()

list(APPEND CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    # Project options.
    -DSDL_ANDROID_JAR=OFF
    -DSDL_EXAMPLES=OFF
    -DSDL_INSTALL_CPACK=OFF
    -DSDL_INSTALL_DOCS=OFF
    -DSDL_RPATH=OFF
    -DSDL_SHARED=ON
    -DSDL_STATIC=OFF
    -DSDL_TEST_LIBRARY=OFF
    # Disable unnecessary sub-systems.
    -DSDL_AUDIO=OFF
    -DSDL_CAMERA=OFF
    -DSDL_DIALOG=OFF
    -DSDL_GPU=OFF
    -DSDL_HAPTIC=OFF
    -DSDL_SENSOR=OFF
)
if(APPIMAGE)
    list(APPEND CMAKE_ARGS
        # Miscellaneous video features/subsystems we don't care about.
        -DSDL_KMSDRM=OFF
        -DSDL_ROCKCHIP=OFF
    )
endif()

list(APPEND BUILD_CMD COMMAND ninja)

list(APPEND INSTALL_CMD COMMAND ${CMAKE_COMMAND} --install .)

append_shared_lib_install_commands(INSTALL_CMD SDL3 VERSION 0)

external_project(
    DOWNLOAD URL 7eedab7982f37bfdf342dc6d2dbd24ce
    https://github.com/libsdl-org/SDL/releases/download/release-3.2.28/SDL3-3.2.28.tar.gz
    PATCH_FILES ${PATCH_FILES}
    CMAKE_ARGS ${CMAKE_ARGS}
    BUILD_COMMAND ${BUILD_CMD}
    INSTALL_COMMAND ${INSTALL_CMD}
)
