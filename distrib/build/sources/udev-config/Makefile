# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

# These are the udev rules from Squebian Deeze on 1/26/11.

include ../Makefile.conf

PACKAGE = udev-config
VERSION = 1.0

DOWNLOAD = yes
PREPARE = yes
PATCH = yes
CONFIGURE = yes
COMPILE = yes

download:
$(DIR)/:
patch:
configure:
compile:
	true


INSTALL = yes
install:
	@mkdir -p $(PKG_ROOT)
	cp -avR lib $(PKG_ROOT)/

CLEAN = yes
clean:
	@echo -n " $(PACKAGE)"
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz
endif


include ../Makefile.rules
