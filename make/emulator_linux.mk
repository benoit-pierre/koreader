APPIMAGE_DIR = $(PLATFORM_DIR)/appimage
APPIMAGETOOL = appimagetool-x86_64.AppImage
APPIMAGETOOL_URL = https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage

appimageupdate: all update-git-rev
	# remove old package if any
	rm -f koreader-appimage-$(MACHINE)-$(VERSION).appimage
	$(SYMLINK) $(abspath $(APPIMAGE_DIR)/AppRun) $(INSTALL_DIR)/koreader/
	$(SYMLINK) $(abspath $(APPIMAGE_DIR)/koreader.desktop) $(INSTALL_DIR)/koreader/
	$(SYMLINK) $(abspath resources/koreader.png) $(INSTALL_DIR)/koreader/
	sed -e 's/%%VERSION%%/$(VERSION)/' -e 's/%%DATE%%/$(RELEASE_DATE)/' $(COMMON_DIR)/koreader.metainfo.xml >$(INSTALL_DIR)/koreader/koreader.appdata.xml
ifeq ("$(wildcard $(APPIMAGETOOL))","")
	# download appimagetool
	$(WGET) "$(APPIMAGETOOL_URL)"
	chmod a+x "$(APPIMAGETOOL)"
endif
	cd $(INSTALL_DIR) && pwd && \
		rm -rf tmp && mkdir -p tmp && \
		cp -Lr koreader tmp && \
		rm -rf tmp/koreader/ota && \
		rm -rf tmp/koreader/resources/icons/src && \
		rm -rf tmp/koreader/spec
	# generate AppImage
	cd $(INSTALL_DIR)/tmp && \
		ARCH=x86_64 "$$OLDPWD/$(APPIMAGETOOL)" --appimage-extract-and-run koreader && \
		mv *.AppImage ../../koreader-$(DIST)-$(MACHINE)-$(VERSION).AppImage

