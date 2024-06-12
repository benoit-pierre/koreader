# Use the git commit count as the (integer) Android version code
ANDROID_VERSION ?= $(shell git rev-list --count HEAD)
ANDROID_NAME ?= $(VERSION)
ANDROID_DIR = $(PLATFORM_DIR)/android
ANDROID_LAUNCHER_DIR = $(ANDROID_DIR)/luajit-launcher
ANDROID_LAUNCHER_BUILD = $(INSTALL_DIR)/luajit-launcher
ANDROID_ASSETS = $(ANDROID_LAUNCHER_BUILD)/assets/module
ANDROID_LIBS_ROOT = $(ANDROID_LAUNCHER_BUILD)/libs
ANDROID_LIBS_ABI = $(ANDROID_LIBS_ROOT)/$(ANDROID_ABI)
ANDROID_FLAVOR ?= Rocks
ANDROID_APK = koreader-android-$(ANDROID_ARCH)$(KODEDUG_SUFFIX)-$(ANDROID_NAME).apk

ifneq (,$(CI))
  GRADLE_FLAGS ?= --console=plain --no-daemon -x lintVitalArmRocksRelease
endif
GRADLE_FLAGS += $(PARALLEL_JOBS:%=--max-workers=%)

androiddev: update
	$(MAKE) -C $(ANDROID_LAUNCHER_DIR) dev

update: all
	# Note: do not remove the module directory so there's no need
	# for `mk7z.sh` to always recreate `assets.7z` from scratch.
	rm -rfv $(ANDROID_LIBS_ROOT)
	mkdir -p $(ANDROID_ASSETS) $(ANDROID_LIBS_ABI)
	# APK version
	echo $(VERSION) > $(ANDROID_ASSETS)/version.txt
	# We need strip the version, or versioned
	# libraries won't be included in the APK.
	for src in $(INSTALL_DIR)/koreader/libs/*; do \
	  dst="$${src##*/}"; \
	  dst="$${dst%%.[0-9]*}"; \
	  $(STRIP) --strip-unneeded "$$src" -o $(ANDROID_LIBS_ABI)/"$$dst"; \
	done
	# binaries are stored as shared libraries to prevent W^X exception on Android 10+
	# https://developer.android.com/about/versions/10/behavior-changes-10#execute-permission
	$(STRIP) --strip-unneeded $(INSTALL_DIR)/koreader/sdcv -o $(ANDROID_LIBS_ABI)/libsdcv.so
	printf '%s\n' 'libs .' 'sdcv libsdcv.so' >$(ANDROID_ASSETS)/map.txt
	# assets are compressed manually and stored inside the APK.
	cd $(INSTALL_DIR)/koreader && \
		./tools/mk7z.sh \
		../../$(ANDROID_ASSETS)/koreader.7z \
		"$$(git show -s --format='%ci')" \
		-m0=lzma2 -mx=9 \
		-- . \
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
		'-x!rocks/bin' \
		'-x!rocks/lib/luarocks' \
		'-x!screenshots' \
		'-x!sdcv' \
		'-x!spec' \
		'-x!tools' \
		'-xr!.*' \
		'-xr!COPYING' \
		'-xr!NOTES.txt' \
		'-xr!NOTICE' \
		'-xr!README.md' \
		;
	# make the android APK
	env \
		ANDROID_ARCH='$(ANDROID_ARCH)' \
		ANDROID_ABI='$(ANDROID_ABI)' \
		ANDROID_FULL_ARCH='$(ANDROID_ABI)' \
		NDK=$(ANDROID_NDK_ROOT) \
		SDK=$(ANDROID_SDK_ROOT) \
		$(ANDROID_LAUNCHER_DIR)/gradlew \
		--project-dir='$(abspath $(ANDROID_LAUNCHER_DIR))' \
		--project-cache-dir='$(abspath $(ANDROID_LAUNCHER_BUILD)/gradle)' \
		-PassetsPath='$(abspath $(dir $(ANDROID_ASSETS)))' \
		-PbuildDir='$(abspath $(ANDROID_LAUNCHER_BUILD))' \
		-PbuildJni=false \
		-PlibsPath='$(abspath $(ANDROID_LIBS_ROOT))' \
		-PndkCustomPath='$(ANDROID_NDK_ROOT)' \
		-PprojectName='KOReader' \
		-PversCode='$(ANDROID_VERSION)' \
		-PversName='$(ANDROID_NAME)' \
		$(GRADLE_FLAGS) \
		'app:assemble$(ANDROID_ARCH)$(ANDROID_FLAVOR)$(if $(KODEBUG),Debug,Release)'
	cp $(ANDROID_LAUNCHER_BUILD)/outputs/apk/$(ANDROID_ARCH)$(ANDROID_FLAVOR)/$(if $(KODEBUG),debug,release)/NativeActivity.apk $(ANDROID_APK)
	# sign the APK with uber-apk-signer if it's available
	$$(command -v uber-apk-signer true) --overwrite --apks $(ANDROID_APK)

PHONY += androiddev update
