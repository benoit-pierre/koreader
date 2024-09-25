#!/usr/bin/env bash

CI_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=/dev/null
source "${CI_DIR}/common.sh"

# Already set in `common.sh`, but this avoid some shellcheck warnings later in the file.
set -e

# if [ -n "${CIRCLE_PULL_REQUEST}" ] || [ "${CIRCLE_BRANCH}" != 'master' ]; then
#     # Pull request / not on master: do a regular testsuite run.
#     make testfront --assume-old=all T="-o '${PWD}/test-results.xml'"
#     exit
# fi

# We're on master: do a full testsuite run with coverage.

# NOTE: `cluacov` needs to be compiled for the version of luajit we're
# using, not the version provided by the docker image (to account for
# different internal types in `lj_obj.h`).
cluacov_rev='a4cce077b718f30ac0d56942f21363a118a59a1c'
luajit_rev="$(sed -n 's/^ *DOWNLOAD GIT *//p' base/thirdparty/luajit/CMakeLists.txt)"
curl --fail --location "https://github.com/luarocks/cluacov/archive/${cluacov_rev}.tar.gz" | tar xz
pushd "cluacov-${cluacov_rev}"
patch -p1 <<\EOF
--- i/src/cluacov/deepactivelines.c
+++ w/src/cluacov/deepactivelines.c
@@ -19,15 +19,8 @@
 #endif
 #else /* LuaJIT */
 #include "luajit.h"
-#if LUAJIT_VERSION_NUM == 20199
 #include "lj2/lua_assert.h"
 #include "lj2/lj_obj.h"
-#elif LUAJIT_VERSION_NUM == 20100
-#include "luajit-2.1.0-beta3/lua_assert.h"
-#include "luajit-2.1.0-beta3/lj_obj.h"
-#else
-#error unsupported LuaJIT version
-#endif
 #endif
 
 #ifdef PUCRIOLUA
EOF
curl --fail --location --output src/cluacov/lj2/lj_obj.h "https://github.com/LuaJIT/LuaJIT/raw/${luajit_rev}/src/lj_obj.h"
luarocks make --local
popd

make coverage --assume-old=all T="-o '${PWD}/test-results.xml'"

# vim: sw=4
