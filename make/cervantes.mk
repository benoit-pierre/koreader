CERVANTES_DIR = $(PLATFORM_DIR)/cervantes
CERVANTES_PACKAGE_OTA = koreader-cervantes$(KODEDUG_SUFFIX)-$(VERSION).tar.xz

define UPDATE_PATH_EXCLUDES +=
tools
endef

update: all
	# ensure that the binaries were built for ARM
	file --dereference $(INSTALL_DIR)/koreader/luajit | grep ARM
	# Cervantes launching scripts
	$(SYMLINK) $(CERVANTES_DIR)/*.sh $(INSTALL_DIR)/koreader
	# Create packages.
	$(strip $(call mkupdate,$(CERVANTES_PACKAGE_OTA)))

PHONY += update
