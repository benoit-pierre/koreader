KINDLE_DIR = $(PLATFORM_DIR)/kindle
KINDLE_PACKAGE = koreader-$(DIST)$(KODEDUG_SUFFIX)-$(VERSION).zip
# OTA update, gzipped for zsync. Note that the targz file extension
# is intended to keep ISP from caching the file, see koreader#1644.
KINDLE_PACKAGE_OTA = koreader-$(DIST)$(KODEDUG_SUFFIX)-$(VERSION).targz
# Don't bundle launchpad on touch devices..
ifeq ($(TARGET), kindle-legacy)
KINDLE_LEGACY_LAUNCHER := launchpad
endif

update: all update-git-rev
	# ensure that the binaries were built for ARM
	file $(INSTALL_DIR)/koreader/luajit | grep ARM || exit 1
	# Kindle launching scripts
	ln -sf ../$(KINDLE_DIR)/extensions $(INSTALL_DIR)/
	ln -sf ../$(KINDLE_DIR)/launchpad $(INSTALL_DIR)/
	ln -sf ../../$(KINDLE_DIR)/koreader.sh $(INSTALL_DIR)/koreader
	ln -sf ../../$(KINDLE_DIR)/libkohelper.sh $(INSTALL_DIR)/koreader
	ln -sf ../../../../../$(KINDLE_DIR)/libkohelper.sh $(INSTALL_DIR)/extensions/koreader/bin
	ln -sf ../../$(COMMON_DIR)/spinning_zsync $(INSTALL_DIR)/koreader
	ln -sf ../../$(KINDLE_DIR)/wmctrl $(INSTALL_DIR)/koreader
	# Update packages.
	cd $(INSTALL_DIR) && \
	  for archive in '$(KINDLE_PACKAGE)' '$(KINDLE_PACKAGE_OTA)'; do \
	    "$$OLDPWD/tools/mkarchive.sh" \
	    --epoch="$$(stat --format=%y koreader/git-rev)" \
	    --manifest='koreader/ota/package.index' \
	    "$$OLDPWD/$$archive" \
	    extensions \
	    koreader \
	    $(KINDLE_LEGACY_LAUNCHER) \
	    '-x!koreader/resources/fonts' \
	    '-x!koreader/resources/icons/src' \
	    '-x!koreader/spec' \
	    ; \
	  done
