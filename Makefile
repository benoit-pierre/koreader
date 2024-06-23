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

# ln --symbolic --no-dereference --force --relative
SYMLINK = ln -snf$(if $(BUILD_OS),,r)

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

.DEFAULT_GOAL := all

all: build install

BUILD_INFO = $(BUILD_DIR)/meson-info/intro-buildoptions.json

MESON ?= meson
NINJA ?= ninja

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
$(MESON) setup $(BUILD_DIR)
  -Dauto_features=disabled -Dpkgconfig.relocatable=true --wrap-mode=forcefallback
  $(patsubst %,--cross-file=%,$(MESON_CROSS_FILES))
  $(patsubst %,--native-file=%,$(MESON_NATIVE_FILES))
  --prefix=/ --bindir=. --libdir=libs.staging --libexecdir=libs.staging
  $(MESON_SETUP_ARGS);
endef

ifneq (,$(wildcard $(BUILD_DIR)/build.ninja))
  # Calling ninja directly is faster then going through meson.
  define meson_compile
  $(NINJA) -C $(BUILD_DIR)
    $(if $(VERBOSE),--verbose)
    $(NINJAFLAGS)
  endef
else
  define meson_compile
  $(MESON) compile -C $(BUILD_DIR)
    $(if $(VERBOSE),--verbose)
    $(if $(NINJAFLAGS),--ninja-args='$(subst $(space),$(comma),$(NINJAFLAGS))')
  endef
endif

define meson_install
$(MESON) install -C $(BUILD_DIR)
  --destdir='$(CURDIR)/$(INSTALL_DIR)/$2'
  --no-rebuild --tags='$1' $(if $(VERBOSE),,--quiet)
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
	$(strip $(meson_setup))

setup: $(BUILD_INFO)

empty :=
space := $(empty) $(empty)
comma := ,

build: $(BUILD_INFO)
	$(strip $(call meson_compile))

install: $(BUILD_INFO)
	$(strip $(call meson_install,runtime,koreader))
	$(call update_git_rev)

install-dev: $(BUILD_INFO)
	$(strip $(call meson_install,devel,dev))

update-git-rev: | install
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

distclean: clean
	# $(MESON) subprojects purge --confirm
	rm -rf subprojects/packagecache
	$(MAKE) -C doc clean

mrproper:
	$(MESON) subprojects purge --confirm

re: clean
	$(MAKE) all

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

.PHONY: all build clean distclean doc fetch-thirdparty install install-dev mrproper re setup update update-git-rev
.NOTPARALLEL: all update
