list(APPEND CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    # Project options.
    -DAMALGAMATED=ON
)

list(APPEND BUILD_CMD COMMAND ninja)

list(APPEND INSTALL_CMD COMMAND ${CMAKE_COMMAND} --install .)

append_shared_lib_install_commands(INSTALL_CMD tree-sitter VERSION 0.26)

external_project(
    DOWNLOAD URL e1fa97f3c073058339e2a621249d6292
    https://github.com/tree-sitter/tree-sitter/archive/refs/tags/v0.26.13.tar.gz
    CMAKE_ARGS ${CMAKE_ARGS}
    BUILD_COMMAND ${BUILD_CMD}
    INSTALL_COMMAND ${INSTALL_CMD}
)
