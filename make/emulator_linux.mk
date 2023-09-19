APPIMAGE_DIR = $(PLATFORM_DIR)/appimage
APPIMAGETOOL = appimagetool-x86_64.AppImage
APPIMAGETOOL_URL = https://github.com/AppImage/AppImageKit/releases/download/13/appimagetool-x86_64.AppImage

# set to 1 if in Docker
DOCKER := $(shell grep -q docker /proc/1/cgroup 2>/dev/null && echo 1)

appimageupdate: all update-git-rev
	# remove old package if any
	rm -f koreader-appimage-$(MACHINE)-$(VERSION).appimage
	ln -sf ../../$(APPIMAGE_DIR)/AppRun $(INSTALL_DIR)/koreader
	ln -sf ../../$(APPIMAGE_DIR)/koreader.appdata.xml $(INSTALL_DIR)/koreader
	ln -sf ../../$(APPIMAGE_DIR)/koreader.desktop $(INSTALL_DIR)/koreader
	ln -sf ../../resources/koreader.png $(INSTALL_DIR)/koreader
	# Copy libbsd.so.0, cf. https://github.com/koreader/koreader/issues/4627
	# ln -sf /lib/x86_64-linux-gnu/libbsd.so.0 $(INSTALL_DIR)/koreader/libs/
ifeq ("$(wildcard $(APPIMAGETOOL))","")
	# download appimagetool
	$(WGET) "$(APPIMAGETOOL_URL)"
	chmod a+x "$(APPIMAGETOOL)"
endif
ifeq ($(DOCKER), 1)
	# remove previously extracted appimagetool, if any
	rm -rf squashfs-root
	./$(APPIMAGETOOL) --appimage-extract
endif
	cd $(INSTALL_DIR) && pwd && \
		rm -rf tmp && mkdir -p tmp && \
		cp -Lr koreader tmp && \
		rm -rf tmp/koreader/ota && \
		rm -rf tmp/koreader/resources/icons/src && \
		rm -rf tmp/koreader/spec
	# generate AppImage
	cd $(INSTALL_DIR)/tmp && \
		ARCH=x86_64 ../../$(if $(DOCKER),squashfs-root/AppRun,$(APPIMAGETOOL)) koreader && \
		mv *.AppImage ../../koreader-$(DIST)-$(MACHINE)-$(VERSION).AppImage

