MACOS_DIR = $(PLATFORM_DIR)/mac

update: all
	mkdir -p \
		$(INSTALL_DIR)/bundle/Contents/MacOS \
		$(INSTALL_DIR)/bundle/Contents/Resources
	cp -pv $(MACOS_DIR)/koreader.icns $(INSTALL_DIR)/bundle/Contents/Resources/icon.icns
	cd $(INSTALL_DIR)/koreader && find -L * \
	  '(' \
	     -path './cache' -o \
	     -path './clipboard' -o \
	     -path './data/dict' -o \
	     -path './data/tessdata' -o \
	     -path './history' -o \
	     -path './l10n/LICENSE' -o \
	     -path './l10n/Makefile' -o \
	     -path './l10n/README.md' -o \
	     -path './l10n/templates' -o \
	     -path './luajit' -o \
	     -path './ota' -o \
	     -path './plugins/SSH.koplugin' -o \
	     -path './plugins/autofrontlight.koplugin' -o \
	     -path './plugins/hello.koplugin' -o \
	     -path './plugins/timesync.koplugin' -o \
	     -path './resources/fonts' -o \
	     -path './resources/icons/src' -o \
	     -path './screenshots' -o \
	     -path './spec' -o \
	     -path './tools' -o \
	     -name '.*' \
	  ')' -prune -o -type f -print0 | cpio -Ldpm0 --quiet ../bundle/Contents/
	cp -pRv $(MACOS_DIR)/menu.xml $(INSTALL_DIR)/bundle/Contents/MainMenu.xib
	ibtool --compile $(INSTALL_DIR)/bundle/Contents/Resources/Base.lproj/MainMenu.nib $(INSTALL_DIR)/bundle/Contents/MainMenu.xib
	rm -vf $(INSTALL_DIR)/bundle/Contents/MainMenu.xib
	$(CURDIR)/platform/mac/do_mac_bundle.sh $(INSTALL_DIR)

PHONY += update
