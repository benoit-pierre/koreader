list(APPEND PATCH_FILES
    powf.patch
    simd_opt.patch
)

list(APPEND CFG_CMD COMMAND
    ${MESON_SETUP} --default-library=$<IF:$<BOOL:${MONOLIBTIC}>,static,shared>
    # Project options.
    -Dbuild_examples=false
    -Denable_opt=$<IF:$<BOOL:${WANT_SIMD}>,true,false>
    ${SOURCE_DIR}
)

list(APPEND BUILD_CMD COMMAND ninja)

list(APPEND INSTALL_CMD COMMAND ${MESON_INSTALL})

if(NOT MONOLIBTIC)
    append_shared_lib_install_commands(INSTALL_CMD spng VERSION 0)
endif()

external_project(
    DOWNLOAD URL 5072ee5d309c593080ea5f43e81e61e4
    https://github.com/randy408/libspng/archive/refs/tags/v0.7.4.tar.gz
    PATCH_FILES ${PATCH_FILES}
    CONFIGURE_COMMAND ${CFG_CMD}
    BUILD_COMMAND ${BUILD_CMD}
    INSTALL_COMMAND ${INSTALL_CMD}
)
