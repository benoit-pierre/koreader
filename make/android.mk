ANDROID_ARCH = $(TARGET:android-%=%)
ifeq ($(ANDROID_ARCH), arm)
  ANDROID_ABI ?= armeabi-v7a
else ifeq ($(ANDROID_ARCH), arm64)
  ANDROID_ABI ?= arm64-v8a
else ifeq ($(ANDROID_ARCH), x86)
  ANDROID_ABI ?= $(ANDROID_ARCH)
else ifeq ($(ANDROID_ARCH), x86_64)
  ANDROID_ABI ?= $(ANDROID_ARCH)
else
  $(warning unsupported android architecture: $(ANDROID_ARCH))
  ANDROID_ABI ?= $(ANDROID_ARCH)
endif

ANDROID_ASSETS_COMPRESSION ?=

ifneq (,$(wildcard meson/android.ini))
  $(eval $(shell sed -n -e 's/^\(ANDROID_\w\+\) = '"'"'\?\([^'"'"']*\)'"'"'\?$$/$$(eval \1 := \2)/p' meson/android.ini))
endif
ANDROID_7Z_LZMA2 := $(or $(ANDROID_7Z_LZMA2),true)
ANDROID_7Z_ZSTD := $(or $(ANDROID_7Z_ZSTD),disabled)
ANDROID_NDK_ROOT := $(or $(ANDROID_NDK_ROOT),$(ANDROID_NDK_HOME),/opt/android-ndk)
ANDROID_SDK_ROOT := $(or $(ANDROID_SDK_ROOT),$(ANDROID_SDK_HOME),/opt/android-sdk)
ifeq (,$(wildcard $(ANDROID_NDK_ROOT)))
  $(error ANDROID_NDK_ROOT does not exist: $(ANDROID_NDK_ROOT))
endif
ifeq (,$(wildcard $(ANDROID_SDK_ROOT)))
  $(error ANDROID_SDK_ROOT does not exist: $(ANDROID_SDK_ROOT))
endif
undefine ANDROID_NDK_HOME
undefine ANDROID_SDK_HOME
export ANDROID_NDK_ROOT
export ANDROID_SDK_ROOT

# Tools
APKANALYZER ?= $(ANDROID_SDK_ROOT)/tools/bin/apkanalyzer

# Use the git commit count as the (integer) Android version code
ANDROID_VERSION ?= $(shell git rev-list --count HEAD)
ANDROID_NAME ?= $(VERSION)
ANDROID_LAUNCHER_DIR = $(BUILD_DIR)/subprojects/luajit-launcher/outputs/apk/$(ANDROID_ARCH)Rocks/$(if $(KODEBUG),debug,release)
ANDROID_APK ?= koreader-android-$(ANDROID_ARCH)$(KODEDUG_SUFFIX)-$(ANDROID_NAME).apk

meson/android.ini:
	printf '%s\n' \
		'[constants]' \
		"ANDROID_7Z_LZMA2 = $(ANDROID_7Z_LZMA2)" \
		"ANDROID_7Z_ZSTD = '$(ANDROID_7Z_ZSTD)'" \
		"ANDROID_NDK_ROOT = '$(ANDROID_NDK_ROOT)'" \
		"ANDROID_SDK_ROOT = '$(ANDROID_SDK_ROOT)'" \
		"ANDROID_TOOLCHAIN = ANDROID_NDK_ROOT / 'toolchains/llvm/prebuilt/linux-x86_64/bin'" \
		>$@

LJL_SRC_DIR = $(CURDIR)/subprojects/luajit-launcher
LJL_BUILD_DIR = $(abspath $(patsubst $(CURDIR)/%,$(BUILD_DIR)/%,$(LJL_SRC_DIR)))
LJL_ASSETS_DIR = $(LJL_BUILD_DIR)/assets
LJL_LIBS_DIR = $(LJL_BUILD_DIR)/libs

ifeq (enabled,$(ANDROID_7Z_ZSTD))
  LJL_ASSETS_COMPRESSION = -m0=zstd -mx=16
else ifeq (true,$(ANDROID_7Z_LZMA2))
  LJL_ASSETS_COMPRESSION = -m0=lzma2 -mx=9
endif

LJL_LUAJIT_INC = $(CURDIR)/subprojects/luajit/src
LJL_LUAJIT_LIB = $(LJL_LIBS_DIR)/$(ANDROID_ABI)/libluajit-5.1.so
LJL_SEVENZIP_INC = $(CURDIR)/subprojects/7z/C
LJL_SEVENZIP_LIB = $(LJL_LIBS_DIR)/$(ANDROID_ABI)/lib7z.so

ifeq (true,$(CI))
  GRADLE_FLAGS ?= \
		  --console=plain \
		  --no-build-cache \
		  --no-configuration-cache \
		  --no-daemon \
		  -x lintVital$(ANDROID_ARCH)RocksRelease
else
  GRADLE_FLAGS ?=
endif

update: all update-git-rev $(LUAJIT_INC) $(SEVENZIP_INC)
	# Note: do not remove the module directory so there's no need
	# for `mk7z.sh` to always recreate `assets.7z` from scratch.
	rm -rfv $(LJL_LIBS_DIR)
	mkdir -p $(LJL_ASSETS_DIR)/module $(LJL_LIBS_DIR)/$(ANDROID_ABI)
	# Assets are compressed manually and stored inside the APK.
	cd $(INSTALL_DIR)/koreader && \
	  '$(CURDIR)/tools/mkarchive.sh' \
	  --epoch="$$(stat --format=%y git-rev)" \
	  --options='$(LJL_ASSETS_COMPRESSION)' \
	  '$(LJL_ASSETS_DIR)/module/koreader.7z' \
	  '-x!cache' \
	  '-x!clipboard' \
	  '-x!data/dict' \
	  '-x!data/tessdata' \
	  '-x!history' \
	  '-x!l10n/templates' \
	  '-x!libs' \
	  '-x!ota' \
	  '-x!resources/fonts*' \
	  '-x!resources/icons/src*' \
	  '-x!runtests' \
	  '-x!screenshots' \
	  '-x!sdcv' \
	  '-x!spec' \
	  '-x!testrunner' \
	  '-x!tools' \
	  '-xr!COPYING' \
	  '-xr!NOTES.txt' \
	  '-xr!NOTICE' \
	  '-xr!README.md' \
	  ;
	# APK version
	echo $(VERSION) >$(LJL_ASSETS_DIR)/module/version.txt
	# Binaries are stored as shared libraries to prevent W^X exception on Android 10+
	# https://developer.android.com/about/versions/10/behavior-changes-10#execute-permission
	cp -av $(INSTALL_DIR)/koreader/sdcv $(LJL_LIBS_DIR)/$(ANDROID_ABI)/libsdcv.so
	# Module map.
	printf '%s\n' 'libs .' 'sdcv libsdcv.so' >$(LJL_ASSETS_DIR)/module/map.txt
	# Shared libraries are stored as platform libraries
	cp -av $(INSTALL_DIR)/koreader/libs/* $(LJL_LIBS_DIR)/$(ANDROID_ABI)/
	test -r '$(LJL_LUAJIT_LIB)'
	test -r '$(LJL_SEVENZIP_LIB)'
	env \
		ANDROID_ARCH='$(ANDROID_ARCH)' \
		ANDROID_ABI='$(ANDROID_ABI)' \
		ANDROID_FULL_ARCH='$(ANDROID_ABI)' \
		LUAJIT_INC='$(LJL_LUAJIT_INC)' \
		LUAJIT_LIB='$(LJL_LUAJIT_LIB)' \
		SEVENZIP_INC='$(LJL_SEVENZIP_INC)' \
		SEVENZIP_LIB='$(LJL_SEVENZIP_LIB)' \
		NDK=$(ANDROID_NDK_ROOT) \
		SDK=$(ANDROID_SDK_ROOT) \
		'$(LJL_SRC_DIR)/gradlew' \
		--project-dir='$(LJL_SRC_DIR)' \
		--project-cache-dir='$(LJL_BUILD_DIR)/.gradle' \
		-PversName='$(ANDROID_NAME)' \
		-PversCode='$(ANDROID_VERSION)' \
		-PprojectName='KOReader' \
		-PndkCustomPath='$(ANDROID_NDK_ROOT)' \
		-PbuildDir='$(LJL_BUILD_DIR)' \
		-PassetsPath='$(LJL_ASSETS_DIR)' \
		-PlibsPath='$(LJL_LIBS_DIR)' \
		$(GRADLE_FLAGS) \
		'app:assemble$(ANDROID_ARCH)Rocks$(if $(KODEBUG),Debug,Release)'
	cp $(ANDROID_LAUNCHER_DIR)/NativeActivity.apk $(ANDROID_APK)

run: update
	# get android app id
	exit $(eval ANDROID_APP_ID := $(shell $(APKANALYZER) manifest application-id $(ANDROID_APK)))$(.SHELLSTATUS)
	# clear logcat to get rid of useless cruft
	adb logcat -c
	# uninstall existing package to make sure *everything* is gone from memory
	-adb uninstall $(ANDROID_APP_ID)
	# wake up target
	-adb shell input keyevent KEYCODE_WAKEUP '&'
	# install
	adb install $(ADB_INSTALL_FLAGS) "$(ANDROID_APK)"
	# there's no adb run so we do this…
	adb shell monkey -p $(ANDROID_APP_ID) -c android.intent.category.LAUNCHER 1
	# monitor logs
	adb logcat KOReader:V luajit-launcher:V dlopen:V "*:E"
