if(GLIBC_VERSION_MAX AND GLIBC_VERSION_MAX VERSION_LESS "2.8")
    # Avoid pulling-in `qsort_r@@GLIBC_2.8`.
    list(APPEND PATCH_FILES no_qsort_r.patch)
endif()

list(APPEND CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    # Project options.
    -DZSTD_BUILD_PROGRAMS=FALSE
    -DZSTD_BUILD_SHARED=$<NOT:$<BOOL:${MONOLIBTIC}>>
    -DZSTD_BUILD_STATIC=TRUE
    -DZSTD_LEGACY_SUPPORT=FALSE
    -DZSTD_MULTITHREAD_SUPPORT=FALSE
)

list(APPEND BUILD_CMD COMMAND ninja)

list(APPEND INSTALL_CMD COMMAND ${CMAKE_COMMAND} --install .)

if(NOT MONOLIBTIC)
    append_shared_lib_install_commands(INSTALL_CMD zstd VERSION 1)
endif()

external_project(
    DOWNLOAD URL b9ed8b476cd65a9c0122d630684237fc
    https://github.com/facebook/zstd/releases/download/v1.6.0/zstd-1.6.0.tar.gz
    PATCH_FILES ${PATCH_FILES}
    SOURCE_SUBDIR build/cmake
    CMAKE_ARGS ${CMAKE_ARGS}
    BUILD_COMMAND ${BUILD_CMD}
    INSTALL_COMMAND ${INSTALL_CMD}
)
