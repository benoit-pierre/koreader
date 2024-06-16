UBUNTUTOUCH_DIR = $(PLATFORM_DIR)/ubuntu-touch
UBUNTUTOUCH_SDL_DIR = $(UBUNTUTOUCH_DIR)/ubuntu-touch-sdl

update: all
	# ensure that the binaries were built for ARM
	file --dereference $(INSTALL_DIR)/koreader/luajit | grep ARM
	# remove old package if any
	rm -f koreader-ubuntu-touch-$(MACHINE)-$(VERSION).click
	$(SYMLINK) $(abspath $(UBUNTUTOUCH_DIR)/koreader.sh) $(INSTALL_DIR)/koreader/
	$(SYMLINK) $(abspath $(UBUNTUTOUCH_DIR)/manifest.json) $(INSTALL_DIR)/koreader/
	$(SYMLINK) $(abspath $(UBUNTUTOUCH_DIR)/koreader.apparmor) $(INSTALL_DIR)/koreader/
	$(SYMLINK) $(abspath $(UBUNTUTOUCH_DIR)/koreader.apparmor.openstore) $(INSTALL_DIR)/koreader/
	$(SYMLINK) $(abspath $(UBUNTUTOUCH_DIR)/koreader.desktop) $(INSTALL_DIR)/koreader/
	$(SYMLINK) $(abspath $(UBUNTUTOUCH_DIR)/koreader.png) $(INSTALL_DIR)/koreader/
	$(SYMLINK) $(abspath $(UBUNTUTOUCH_DIR)/libSDL2.so) $(INSTALL_DIR)/koreader/libs/
	# create new package
	cd $(INSTALL_DIR) && zip -9 -r ../koreader-$(DIST)-$(MACHINE)-$(VERSION).zip koreader -x $(release_excludes)
	# generate ubuntu touch click package
	rm -rf $(INSTALL_DIR)/tmp && mkdir -p $(INSTALL_DIR)/tmp
	cd $(INSTALL_DIR)/tmp && \
		unzip ../../koreader-$(DIST)-$(MACHINE)-$(VERSION).zip && \
		click build koreader && \
		mv *.click ../../koreader-$(DIST)-$(MACHINE)-$(VERSION).click

PHONY += update
