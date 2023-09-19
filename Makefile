ifeq (,$(VERBOSE))
.SILENT:
endif

SHELL = /bin/bash
.SHELLFLAGS = -eo pipefail -c

ifeq ($(OS),Windows_NT)
  BUILD_ARCH = $(PROCESSOR_ARCHITECTURE)
  BUILD_OS = windows
else
  UNAME:=$(shell uname -s -m)
  BUILD_ARCH = $(lastword $(UNAME))
  ifeq ($(firstword $(UNAME)),Linux)
    BUILD_OS = linux
  endif
  ifeq ($(firstword $(UNAME)),Darwin)
    BUILD_OS = macos
  endif
endif

WGET ?= wget --no-verbose --progress=dot:mega --show-progress

ifeq ($(BUILD_OS),macos)
  RCP ?= cp -R
else
  RCP ?= cp -r
endif

ifdef CCACHE_DISABLE
  CCACHE = env
else
  CCACHE ?= $(or \
	    $(shell which ccache 2>/dev/null),\
	    $(shell which sccache 2>/dev/null),\
	    $(shell which buildcache 2>/dev/null),\
	    env)
endif

ifeq ($(TARGET),)
  override TARGET := emulator
  KODEBUG ?= 1
else
  ifneq (,$(filter %-,$(TARGET)))
    $(error unsupported target: $(TARGET)!)
  endif
endif

ifeq ($(TARGET),emulator)
  override TARGET := $(TARGET)-$(BUILD_OS)-$(BUILD_ARCH)
else ifneq ($(filter emulator-%,$(TARGET)),)
  ifeq ($(words $(subst -,$(empty) $(empty),$(TARGET))),2)
    override TARGET := $(TARGET)-$(BUILD_ARCH)
  endif
endif

VERSION = $(shell cat $(INSTALL_DIR)/koreader/git-rev)

MACHINE = $(TARGET)

ifdef KODEBUG
  KODEDUG_SUFFIX = -debug
endif

DIST = $(TARGET)
BUILD_DIR = build/$(DIST)$(KODEDUG_SUFFIX)
INSTALL_DIR = koreader-$(DIST)$(KODEDUG_SUFFIX)
INSTALL_SYMLINKS ?= $(and $(filter linux macos,$(BUILD_OS)),1)

# platform directories
PLATFORM_DIR=platform
COMMON_DIR=$(PLATFORM_DIR)/common
CERVANTES_DIR=$(PLATFORM_DIR)/cervantes
DEBIAN_DIR=$(PLATFORM_DIR)/debian
POCKETBOOK_DIR=$(PLATFORM_DIR)/pocketbook
SONY_PRSTUX_DIR=$(PLATFORM_DIR)/sony-prstux
UBUNTUTOUCH_DIR=$(PLATFORM_DIR)/ubuntu-touch
UBUNTUTOUCH_SDL_DIR:=$(UBUNTUTOUCH_DIR)/ubuntu-touch-sdl
WIN32_DIR=$(PLATFORM_DIR)/win32

.DEFAULT_GOAL := all

all: build install

BUILD_INFO = $(BUILD_DIR)/meson-info/intro-buildoptions.json

MESON ?= meson

MESON_CROSS_FILES :=
MESON_NATIVE_FILES :=

# At most 3 parts to a target (type, os, arch).
TARGET_PARTS := $(subst -,$(empty) $(empty),$(TARGET))
TARGET_VARIANTS := $(foreach size,$(wordlist 1,$(words $(TARGET_PARTS)),1 2 3),$(subst $(empty) $(empty),-,$(wordlist 1,$(size),$(TARGET_PARTS))))

TARGET_FILES := $(wildcard $(patsubst %,meson/target_%-.ini,$(TARGET_VARIANTS))) meson/target_$(TARGET).ini
ifeq (,$(wildcard $(lastword $(TARGET_FILES))))
  $(error unsupported target: $(TARGET)!)
endif
TARGET_FILES := $(wildcard $(TARGET_FILES))

# Native files.
NATIVE_FILES := meson/native.ini $(wildcard $(patsubst %,meson/native_%.ini,$(TARGET_VARIANTS)))

# Default options.
OPTIONS_FILES += meson/options.ini meson/options_$(if $(KODEBUG),debug,release).ini
ifneq (,$(INSTALL_SYMLINKS))
  OPTIONS_FILES += meson/options_install_symlinks.ini
endif

# Custom user options.
USER_FILES += $(wildcard meson/user_$(TARGET)$(or $(KODEDUG_SUFFIX),-release).ini)

ifeq ($(TARGET),emulator-$(BUILD_OS)-$(BUILD_ARCH))
  # Native build.
  MESON_NATIVE_FILES += meson/ccache.ini $(NATIVE_FILES) $(OPTIONS_FILES) $(TARGET_FILES) $(USER_FILES)
else
  # Cross build.
  CROSS_FILES := $(patsubst %,meson/cross_%.ini,$(TARGET_VARIANTS))
  ifeq (,$(wildcard $(lastword $(CROSS_FILES))))
    $(error no cross-compilation profile for $(TARGET)!)
  endif
  CROSS_FILES := $(wildcard $(CROSS_FILES))
  ifneq (,$(filter android-%,$(TARGET)))
    CROSS_FILES := meson/android.ini $(CROSS_FILES)
    NATIVE_FILES := meson/android.ini $(NATIVE_FILES)
  endif
  MESON_CROSS_FILES += meson/ccache.ini $(CROSS_FILES) $(OPTIONS_FILES) $(TARGET_FILES) $(USER_FILES)
  MESON_NATIVE_FILES += meson/ccache.ini $(NATIVE_FILES)
endif

define meson_setup
$(MESON) setup $(BUILD_DIR) \
  -Dauto_features=disabled -Dpkgconfig.relocatable=true --wrap-mode=forcefallback \
  $(patsubst %,--cross-file=%,$(MESON_CROSS_FILES)) \
  $(patsubst %,--native-file=%,$(MESON_NATIVE_FILES)) \
  --prefix=/ --bindir=. --libdir=libs.staging --libexecdir=libs.staging \
  $(MESON_SETUP_ARGS);
endef

define meson_install
$(MESON) install -C $(BUILD_DIR) \
  --destdir='$(CURDIR)/$(INSTALL_DIR)/$2' \
  --no-rebuild --tags='$1' $(if $(VERBOSE),,--quiet) \
  ;
endef

# We want to carry the version of the KOReader main repo, not
# that of koreader-base, and only append date if we're not on
# a whole version, like `v2018.11`.
define update_git_rev
  $(eval VERSION := $(shell git describe HEAD))
  $(eval VERSION := $(VERSION)$(if $(findstring -,$(VERSION)),_$(shell git show -s --format=format:'%cd' --date=short)))
  echo '$(VERSION)' >'$(INSTALL_DIR)/koreader/git-rev'
  # Note: use a precision of hours to reduce gradle cache misses…
  touch -t "$$(git show --no-patch --date='format-local:%Y%m%d%H00' --format='%cd')" '$(INSTALL_DIR)/koreader/git-rev'
endef

meson/ccache.ini:
	printf '%s\n' \
		'[constants]' \
		"CCACHE = '$(CCACHE)'" \
		>$@

$(BUILD_INFO): $(MESON_CROSS_FILES) $(MESON_NATIVE_FILES)
	$(meson_setup)

setup: $(BUILD_INFO)

empty :=
space := $(empty) $(empty)
comma := ,

build: $(BUILD_INFO)
	$(MESON) compile -C $(BUILD_DIR)$(if $(NINJAFLAGS), --ninja-args='$(subst $(space),$(comma),$(NINJAFLAGS))')$(if $(VERBOSE), --verbose)

install: $(BUILD_INFO)
	$(call meson_install,runtime,koreader)
	$(call update_git_rev)

install-dev: $(BUILD_INFO)
	$(call meson_install,devel,dev)

update-git-rev:
ifneq (,$(INSTALL_SYMLINKS))
	$(call update_git_rev)
endif

fetch-thirdparty:
ifneq (,$(shell git submodule status | grep -E '^-'))
	git submodule init
	git submodule sync
	git submodule update --depth 1 --jobs 3
endif
	$(MESON) subprojects download --num-processes 3

clean:
	rm -rf $(BUILD_DIR) $(INSTALL_DIR)

dist-clean: clean
	# $(MESON) subprojects purge --confirm
	rm -rf subprojects/packagecache
	$(MAKE) -C doc clean

dist-reset:
	$(MESON) subprojects update --reset

ifneq (,$(filter android-%,$(TARGET)))
  include make/android.mk
else ifneq (,$(filter emulator-%,$(TARGET)))
  include make/emulator.mk
else ifneq (,$(filter kindle%,$(TARGET)))
  include make/kindle.mk
else
  include make/$(TARGET).mk
endif

ifeq (true,$(CI))
  include make/ci.mk
endif

PB_PACKAGE=koreader-pocketbook$(KODEDUG_SUFFIX)-$(VERSION).zip
PB_PACKAGE_OTA=koreader-pocketbook$(KODEDUG_SUFFIX)-$(VERSION).targz
pbupdate: all
	# ensure that the binaries were built for ARM
	file $(INSTALL_DIR)/koreader/luajit | grep ARM || exit 1
	# remove old package if any
	rm -f $(PB_PACKAGE)
	# Pocketbook launching scripts
	mkdir -p $(INSTALL_DIR)/applications
	mkdir -p $(INSTALL_DIR)/system/bin
	cp $(POCKETBOOK_DIR)/koreader.app $(INSTALL_DIR)/applications
	cp $(POCKETBOOK_DIR)/system_koreader.app $(INSTALL_DIR)/system/bin/koreader.app
	cp $(COMMON_DIR)/spinning_zsync $(INSTALL_DIR)/koreader
	cp -rfL $(INSTALL_DIR)/koreader $(INSTALL_DIR)/applications
	find $(INSTALL_DIR)/applications/koreader \
		-type f \( -name "*.gif" -o -name "*.html" -o -name "*.md" -o -name "*.txt" \) \
		-exec rm -vf {} \;
	# create new package
	cd $(INSTALL_DIR) && \
		zip -9 -r \
			../$(PB_PACKAGE) \
			applications -x "applications/koreader/resources/fonts/*" \
			"applications/koreader/resources/icons/src/*" "applications/koreader/spec/*" \
			$(ZIP_EXCLUDE)
	# generate koboupdate package index file
	zipinfo -1 $(PB_PACKAGE) > \
		$(INSTALL_DIR)/applications/koreader/ota/package.index
	echo "applications/koreader/ota/package.index" >> \
		$(INSTALL_DIR)/applications/koreader/ota/package.index
	# hack file path when running tar in parent directory of koreader
	sed -i -e 's/^/..\//' \
		$(INSTALL_DIR)/applications/koreader/ota/package.index
	# update index file in zip package
	cd $(INSTALL_DIR) && zip -ru ../$(PB_PACKAGE) \
		applications/koreader/ota/package.index system
	# make gzip pbupdate for zsync OTA update
	cd $(INSTALL_DIR)/applications && \
		tar --hard-dereference -I"gzip --rsyncable" -cah --no-recursion -f ../../$(PB_PACKAGE_OTA) \
		-T koreader/ota/package.index

utupdate: all
	# ensure that the binaries were built for ARM
	file $(INSTALL_DIR)/koreader/luajit | grep ARM || exit 1
	# remove old package if any
	rm -f koreader-ubuntu-touch-$(MACHINE)-$(VERSION).click

	ln -sf ../../$(UBUNTUTOUCH_DIR)/koreader.sh $(INSTALL_DIR)/koreader
	ln -sf ../../$(UBUNTUTOUCH_DIR)/manifest.json $(INSTALL_DIR)/koreader
	ln -sf ../../$(UBUNTUTOUCH_DIR)/koreader.apparmor $(INSTALL_DIR)/koreader
	ln -sf ../../$(UBUNTUTOUCH_DIR)/koreader.apparmor.openstore $(INSTALL_DIR)/koreader
	ln -sf ../../$(UBUNTUTOUCH_DIR)/koreader.desktop $(INSTALL_DIR)/koreader
	ln -sf ../../$(UBUNTUTOUCH_DIR)/koreader.png $(INSTALL_DIR)/koreader
	ln -sf ../../../$(UBUNTUTOUCH_DIR)/libSDL2.so $(INSTALL_DIR)/koreader/libs

	# create new package
	cd $(INSTALL_DIR) && pwd && \
		zip -9 -r \
			../koreader-$(DIST)-$(MACHINE)-$(VERSION).zip \
			koreader -x "koreader/resources/fonts/*" "koreader/ota/*" \
			"koreader/resources/icons/src/*" "koreader/spec/*" \
			$(ZIP_EXCLUDE)

	# generate ubuntu touch click package
	rm -rf $(INSTALL_DIR)/tmp && mkdir -p $(INSTALL_DIR)/tmp
	cd $(INSTALL_DIR)/tmp && \
		unzip ../../koreader-$(DIST)-$(MACHINE)-$(VERSION).zip && \
		click build koreader && \
		mv *.click ../../koreader-$(DIST)-$(MACHINE)-$(VERSION).click

debianupdate: all
	mkdir -pv \
		$(INSTALL_DIR)/debian/usr/bin \
		$(INSTALL_DIR)/debian/usr/lib \
		$(INSTALL_DIR)/debian/usr/share/pixmaps \
		$(INSTALL_DIR)/debian/usr/share/applications \
		$(INSTALL_DIR)/debian/usr/share/doc/koreader \
		$(INSTALL_DIR)/debian/usr/share/man/man1

	cp -pv resources/koreader.png $(INSTALL_DIR)/debian/usr/share/pixmaps
	cp -pv $(DEBIAN_DIR)/koreader.desktop $(INSTALL_DIR)/debian/usr/share/applications
	cp -pv $(DEBIAN_DIR)/copyright COPYING $(INSTALL_DIR)/debian/usr/share/doc/koreader
	cp -pv $(DEBIAN_DIR)/koreader.sh $(INSTALL_DIR)/debian/usr/bin/koreader
	cp -Lr $(INSTALL_DIR)/koreader $(INSTALL_DIR)/debian/usr/lib

	gzip -cn9 $(DEBIAN_DIR)/changelog > $(INSTALL_DIR)/debian/usr/share/doc/koreader/changelog.Debian.gz
	gzip -cn9 $(DEBIAN_DIR)/koreader.1 > $(INSTALL_DIR)/debian/usr/share/man/man1/koreader.1.gz

	chmod 644 \
		$(INSTALL_DIR)/debian/usr/share/doc/koreader/changelog.Debian.gz \
		$(INSTALL_DIR)/debian/usr/share/doc/koreader/copyright \
		$(INSTALL_DIR)/debian/usr/share/man/man1/koreader.1.gz

	rm -rf \
		$(INSTALL_DIR)/debian/usr/lib/koreader/{ota,cache,clipboard,screenshots,spec,tools,resources/fonts,resources/icons/src}

SONY_PRSTUX_PACKAGE=koreader-sony-prstux$(KODEDUG_SUFFIX)-$(VERSION).zip
SONY_PRSTUX_PACKAGE_OTA=koreader-sony-prstux$(KODEDUG_SUFFIX)-$(VERSION).targz
sony-prstuxupdate: all
	# ensure that the binaries were built for ARM
	file $(INSTALL_DIR)/koreader/luajit | grep ARM || exit 1
	# remove old package if any
	rm -f $(SONY_PRSTUX_PACKAGE)
	# Sony PRSTUX launching scripts
	cp $(SONY_PRSTUX_DIR)/*.sh $(INSTALL_DIR)/koreader
	# create new package
	cd $(INSTALL_DIR) && \
	        zip -9 -r \
	                ../$(SONY_PRSTUX_PACKAGE) \
	                koreader -x "koreader/resources/fonts/*" \
	                "koreader/resources/icons/src/*" "koreader/spec/*" \
	                $(ZIP_EXCLUDE)
	# generate update package index file
	zipinfo -1 $(SONY_PRSTUX_PACKAGE) > \
	        $(INSTALL_DIR)/koreader/ota/package.index
	echo "koreader/ota/package.index" >> $(INSTALL_DIR)/koreader/ota/package.index
	# update index file in zip package
	cd $(INSTALL_DIR) && zip -u ../$(SONY_PRSTUX_PACKAGE) \
	        koreader/ota/package.index
	# make gzip sonyprstux update for zsync OTA update
	cd $(INSTALL_DIR) && \
	        tar --hard-dereference -I"gzip --rsyncable" -cah --no-recursion -f ../$(SONY_PRSTUX_PACKAGE_OTA) \
	        -T koreader/ota/package.index

CERVANTES_PACKAGE=koreader-cervantes$(KODEDUG_SUFFIX)-$(VERSION).zip
CERVANTES_PACKAGE_OTA=koreader-cervantes$(KODEDUG_SUFFIX)-$(VERSION).targz
cervantesupdate: all
	# ensure that the binaries were built for ARM
	file $(INSTALL_DIR)/koreader/luajit | grep ARM || exit 1
	# remove old package if any
	rm -f $(CERVANTES_PACKAGE)
	# Cervantes launching scripts
	cp $(COMMON_DIR)/spinning_zsync $(INSTALL_DIR)/koreader/spinning_zsync.sh
	cp $(CERVANTES_DIR)/*.sh $(INSTALL_DIR)/koreader
	cp $(CERVANTES_DIR)/spinning_zsync $(INSTALL_DIR)/koreader
	# create new package
	cd $(INSTALL_DIR) && \
		zip -9 -r \
			../$(CERVANTES_PACKAGE) \
			koreader -x "koreader/resources/fonts/*" \
			"koreader/resources/icons/src/*" "koreader/spec/*" \
			$(ZIP_EXCLUDE)
	# generate update package index file
	zipinfo -1 $(CERVANTES_PACKAGE) > \
		$(INSTALL_DIR)/koreader/ota/package.index
	echo "koreader/ota/package.index" >> $(INSTALL_DIR)/koreader/ota/package.index
	# update index file in zip package
	cd $(INSTALL_DIR) && zip -u ../$(CERVANTES_PACKAGE) \
	koreader/ota/package.index
	# make gzip cervantes update for zsync OTA update
	cd $(INSTALL_DIR) && \
	tar --hard-dereference -I"gzip --rsyncable" -cah --no-recursion -f ../$(CERVANTES_PACKAGE_OTA) \
	-T koreader/ota/package.index

# update:
# ifeq ($(TARGET), android)
# 	make androidupdate
# else ifeq ($(TARGET), appimage)
# 	make appimageupdate
# else ifeq ($(TARGET), cervantes)
# 	make cervantesupdate
# else ifeq ($(TARGET), kindle)
# 	make kindleupdate
# else ifeq ($(TARGET), kindle-legacy)
# 	make kindleupdate
# else ifeq ($(TARGET), kindlepw2)
# 	make kindleupdate
# else ifeq ($(TARGET), kobo)
# 	make koboupdate
# else ifeq ($(TARGET), pocketbook)
# 	make pbupdate
# else ifeq ($(TARGET), sony-prstux)
# 	make sony-prstuxupdate
# else ifeq ($(TARGET), ubuntu-touch)
# 	make utupdate
# else ifeq ($(TARGET), debian)
# 	make debianupdate
# 	$(CURDIR)/platform/debian/do_debian_package.sh $(INSTALL_DIR)
# else ifeq ($(TARGET), debian-armel)
# 	make debianupdate
# 	$(CURDIR)/platform/debian/do_debian_package.sh $(INSTALL_DIR) armel
# else ifeq ($(TARGET), debian-armhf)
# 	make debianupdate
# 	$(CURDIR)/platform/debian/do_debian_package.sh $(INSTALL_DIR) armhf
# else ifeq ($(TARGET), debian-arm64)
# 	make debianupdate
# 	$(CURDIR)/platform/debian/do_debian_package.sh $(INSTALL_DIR) arm64
# endif


# for gettext
DOMAIN=koreader
TEMPLATE_DIR=l10n/templates
XGETTEXT_BIN=xgettext

pot: po
	mkdir -p $(TEMPLATE_DIR)
	$(XGETTEXT_BIN) --from-code=utf-8 \
		--keyword=C_:1c,2 --keyword=N_:1,2 --keyword=NC_:1c,2,3 \
		--add-comments=@translators \
		reader.lua `find frontend -iname "*.lua" | sort` \
		`find plugins -iname "*.lua" | sort` \
		`find tools -iname "*.lua" | sort` \
		-o $(TEMPLATE_DIR)/$(DOMAIN).pot

po:
	git submodule update --remote l10n


static-check:
	@if which luacheck > /dev/null; then \
			luacheck -q {reader,setupkoenv,datastorage}.lua frontend plugins spec; \
		else \
			echo "[!] luacheck not found. "\
			"you can install it with 'luarocks install luacheck'"; \
		fi

doc:
	make -C doc

.PHONY: all build clean dist-clean dist-reset doc fetch-thirdparty install install-dev setup update update-git-rev
.NOTPARALLEL: all
