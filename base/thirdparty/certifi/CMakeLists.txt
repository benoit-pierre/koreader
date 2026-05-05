# Build in source tree.
set(BINARY_DIR ${SOURCE_DIR})

list(APPEND INSTALL_CMD COMMAND ${CMAKE_COMMAND} -E copy_if_different certifi/cacert.pem ${OUTPUT_DIR}/data/ca-bundle.crt)

external_project(
    DOWNLOAD URL a524df3261ff972bbe811eb7307a79ed
    https://files.pythonhosted.org/packages/22/30/7cd8fdcdfbc5b869528b079bfb76dcdf6056b1a2097a662e5e8c04f42965/certifi-2026.4.22-py3-none-any.whl
    INSTALL_COMMAND ${INSTALL_CMD}
)
