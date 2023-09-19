KOBO_DIR=$(PLATFORM_DIR)/kobo
KOBO_PACKAGE=koreader-kobo$(KODEDUG_SUFFIX)-$(VERSION).zip
KOBO_PACKAGE_OTA=koreader-kobo$(KODEDUG_SUFFIX)-$(VERSION).targz

update: all update-git-rev
	# ensure that the binaries were built for ARM
	file $(INSTALL_DIR)/koreader/luajit | grep ARM || exit 1
	# Kobo launching scripts
	cp $(KOBO_DIR)/koreader.png $(INSTALL_DIR)
	cp $(KOBO_DIR)/koreader.png $(INSTALL_DIR)/koreader.png
	cp $(KOBO_DIR)/*.sh $(INSTALL_DIR)/koreader
	cp $(COMMON_DIR)/spinning_zsync $(INSTALL_DIR)/koreader
	# Update packages.
	cd $(INSTALL_DIR) && \
	  for archive in '$(KOBO_PACKAGE)' '$(KOBO_PACKAGE_OTA)'; do \
	    "$$OLDPWD/tools/mkarchive.sh" \
	    --epoch="$$(stat --format=%y koreader/git-rev)" \
	    --manifest='koreader/ota/package.index' \
	    "$$OLDPWD/$$archive" \
	    koreader koreader.png \
	    '-x!koreader/resources/fonts' \
	    '-x!koreader/resources/icons/src' \
	    '-x!koreader/spec' \
	    ; \
	  done
