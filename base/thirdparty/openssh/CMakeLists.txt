if(NOT APPLE AND MONOLIBTIC)
    # Need `-pthread` for static OpenSSL's crypto library.
    string(APPEND LDFLAGS " -pthread")
endif()
list(APPEND CFG_CMD COMMAND env)
if(LEGACY OR POCKETBOOK)
    # Avoid pulling-in `utimensat@@GLIBC_2.6` in `sftp-server`.
    list(APPEND CFG_CMD ac_cv_func_utimensat=no)
endif()
if(ASAN)
    # When compiling with `-fsanitize=address`, configure will erroneously detect
    # `reallocarray` as available (because an interceptor is provided by libasan),
    # and this latter breaks the build ("error: implicit declaration of function
    # 'reallocarray'") so we manually check ourselves if it really is.
    enable_language(C)
    include(CheckCSourceCompiles)
    check_c_source_compiles([==[
        #include <limits.h>
        #include <stddef.h>
        #if __GNUC_PREREQ (2, 26)
            __asm__(".symver reallocarray,reallocarray@GLIBC_2.26");
        #else
            #error glibc < 2.26
        #endif
        void *reallocarray(void *, size_t, size_t);
        int main(void) {
            return reallocarray(NULL, 0, 0) == 0;
        }
        ]==] HAS_REALLOCARRAY)
    if(NOT HAS_REALLOCARRAY)
        list(APPEND CFG_CMD ac_cv_func_reallocarray=no)
    endif()
endif()
append_autotools_vars(CFG_CMD)
list(APPEND CFG_CMD
    ${SOURCE_DIR}/configure --host=${CHOST}
    --disable-etc-default-login
    --disable-lastlog
    --without-hardening
    --without-openssl
    --without-stackprotect
    --without-zlib
)

set(PROGRAMS sftp-server)

list(APPEND BUILD_CMD COMMAND make ${PROGRAMS})

append_binary_install_command(INSTALL_CMD ${PROGRAMS})

external_project(
    DOWNLOAD URL c5fb91ded926b38e8956074cac2cd44f
    https://cdn.openbsd.org/pub/OpenBSD/OpenSSH/portable/openssh-10.4p1.tar.gz
    https://cloudflare.cdn.openbsd.org/pub/OpenBSD/OpenSSH/portable/openssh-10.4p1.tar.gz
    CONFIGURE_COMMAND ${CFG_CMD}
    BUILD_COMMAND ${BUILD_CMD}
    INSTALL_COMMAND ${INSTALL_CMD}
)
