CACHE_KEY_FILES := $(sort \
  kodev meson.build meson_options.txt \
  $(MAKEFILE_LIST) $(MESON_CROSS_FILES) $(MESON_NATIVE_FILES) \
  :!base/ffi-cdecl/include \
  base/*.c base/*.cpp base/*.h \
  base/fix_libs.py \
  base/input/*.c base/input/*.h \
  base/libkoreader.* \
  subprojects/*.wrap \
  subprojects/crengine/crengine/include \
  subprojects/crengine/crengine/qimagescale \
  subprojects/crengine/crengine/src/*.cpp \
  subprojects/crengine/meson* \
  subprojects/crengine/thirdparty/antiword/*.c \
  subprojects/crengine/thirdparty/antiword/*.h \
  subprojects/crengine/thirdparty/antiword/meson.build \
  subprojects/crengine/thirdparty/chmlib/meson.build \
  subprojects/crengine/thirdparty/chmlib/src/*.c \
  subprojects/crengine/thirdparty/chmlib/src/*.h \
  subprojects/koreader-runtests/meson* \
  subprojects/lua-ljsqlite3/meson* \
  subprojects/packagefiles/ \
)

cache-key:
	@$(eval versioned := $(shell git ls-files $(CACHE_KEY_FILES)))
	@$(eval unversioned := $(shell git ls-files --other $(CACHE_KEY_FILES)))
	@( \
	  git ls-tree @ $(versioned); \
	  printf 'unversioned %b\n' $(join $(shell git hash-object $(unversioned)),$(unversioned:%=\\t%)); \
	) >cache_key.txt

trim-thirdparty:
	find subprojects -type d \( \
		-name 'm4' -o \
		-name 'rockspecs' \
		\) -print0 | xargs -0 rm -vrf
	find subprojects \( -name crengine -o -name libcurl \) \
		-type d -prune -o -type f \( \
		-name '*.m4' -o \
		-name 'CMakeLists.txt' -o \
		-name 'Doxyfile' -o \
		-name 'ltmain.sh' \
		\) -print0 | xargs -0 rm -vf
	rm -vrf $(filter-out subprojects/lua-ljsqlite3/README.md,$(wildcard subprojects/*/*.md))
	rm -vrf subprojects/*/.git/
	rm -vrf subprojects/*/{ChangeLog,CHANGES,config.guess,config.sub,configure,configure.ac,NEWS}
	rm -vrf subprojects/*lua*/spec/
	rm -vrf subprojects/7z-*/CPP
	rm -vrf subprojects/curl-*/{packages,plan9,projects,scripts,tests,winbuild}
	rm -vrf $(filter-out %/libcurl.m4,$(wildcard subprojects/curl-*/docs/libcurl/*))
	rm -vrf $(filter-out %/libcurl,$(wildcard subprojects/curl-*/docs/*))
	rm -vrf subprojects/czmq-*/{builds,doc,examples,images,model,packaging}
	rm -vrf subprojects/djvulibre-*/{debian,desktopfiles,doc,share,tools,win32,xmltools}
	rm -vrf subprojects/dropbear-*/{debian,fuzz,libtomcrypt/notes,test}
	rm -vrf subprojects/FBInk-*/fonts/unifont*.h
	rm -vrf subprojects/FBInk-*/i2c-tools/{eeprom,tools}
	rm -vrf subprojects/FBInk-*/libevdev
	rm -vrf subprojects/freetype-*/builds/unix/configure
	rm -vrf subprojects/freetype-*/builds/{amiga,wince,windows,mac}
	rm -vrf subprojects/freetype-*/docs
	rm -vrf subprojects/fribidi-*/test
	rm -vrf subprojects/giflib-*/{doc,pic,tests,util}
	rm -vrf subprojects/glib-*/*/tests
	rm -vrf subprojects/glib-*/po
	rm -vrf subprojects/gumbo-parser-*/benchmark
	rm -vrf subprojects/harfbuzz-*/{docs,perf,test}
	rm -vrf subprojects/leptonica-*/prog/{*/,*.ba,*.c,*.jpg,*.pa*,*.png,*.tif}
	rm -vrf subprojects/libarchive-*/{build/autoconf,contrib,doc,examples,libarchive/test,Makefile.in,test_utils}
	rm -vrf subprojects/libffi-*/{doc,testsuite}
	rm -vrf subprojects/libjpeg-turbo-*/{cmakescripts,doc,fuzz,java,release,testimages}
	rm -vrf subprojects/libpng-*/{contrib,libpng.3,libpng-manual.txt,projects,scripts/makefile*}
	rm -vrf subprojects/libwebp-*/{doc,gradle,infra,man,swig,webp_js}
	rm -vrf subprojects/libzmq/{builds/msvc,foreign,doc,tests}
	rm -vrf subprojects/lodepng-*/examples
	rm -vrf subprojects/lodepng-*/lodepng_{benchmark,fuzzer,unittest,util}.cpp
	rm -vrf subprojects/luajit/doc
	rm -vrf subprojects/luasec-*/samples
	rm -vrf subprojects/luasocket-*/{docs,examples,samples,test}
	rm -vrf subprojects/miniconv-*/scripts
	rm -vrf subprojects/mupdf-*/resources/{fonts,icc}
	rm -vrf subprojects/mupdf-*/{docs,platform}
	rm -vrf subprojects/nanosvg-*/example
	rm -vrf subprojects/openjpeg-*/thirdparty
	rm -vrf subprojects/openssh-portable-*/{contrib,regress}
	rm -vrf subprojects/openssl-*/generated-config/archs/{BSD*,solaris*,VC*}
	rm -vrf subprojects/openssl-*/generated-config/archs/{darwin-i386-cc,linux-ppc64le,linux64-mips64,linux64-s390x,linux64-riscv64}
	rm -vrf subprojects/openssl-*/{Configurations,Configure,fuzz,demos,doc,test,util}
	rm -vrf subprojects/packagecache
	rm -vrf subprojects/pcre2-*/{doc,testdata}
	rm -vrf subprojects/Penlight-*/{docs,docs_topics,tests}
	rm -vrf subprojects/SDL2-*/{acinclude,android-project,build-scripts,docs,test,VisualC*,Xcode*}
	rm -vrf subprojects/srell-*/single-header
	rm -vrf subprojects/tesseract-*/{doc,testing,training}
	rm -vrf subprojects/turbo-*/{doc,examples}
	rm -vrf subprojects/utf8proc-*/{bench,data,test}
	rm -vrf subprojects/zlib-*/{contrib,doc,examples,msdos,os400}
	rm -vrf subprojects/zstd-*/build/{VS*,single_file_libs}
	rm -vrf subprojects/zstd-*/{contrib,doc,examples,programs,tests,zlibWrapper}

.PHONY: cache-key trim-thirdparty
