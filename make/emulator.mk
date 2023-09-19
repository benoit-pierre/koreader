HOST_OS = $(word 2,$(TARGET_PARTS))
HOST_ARCH = $(word 3,$(TARGET_PARTS))

define launch
$(RPREFIX) ./luajit -l 'setupkoenv' '$(1)' $(RARGS)
endef

run: all
	cd $(INSTALL_DIR)/koreader;                   \
	while true; do                                \
	    status=0;                                 \
	    $(call launch,reader.lua) || status=$$?;  \
	    [ $${status} -eq 85 ] || exit $${status}; \
	done

run-wbuilder: all
	cd $(INSTALL_DIR)/koreader; $(call launch,tools/wbuilder.luareader.lua)

include make/emulator_$(HOST_OS).mk

ifeq ($(TARGET),emulator-$(BUILD_OS)-$(BUILD_ARCH))
  include make/emulator_tests.mk
endif

.PHONY: run run-wbuilder
