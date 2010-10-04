# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = iproute2
#VERSION = $(KERNEL_MMR)
VERSION = 2.6.33

BASE_URL = http://devresources.linuxfoundation.org/dev/iproute2/download
EXTENSION=.tar.bz2

CONFIGURE = yes
COMPILE = yes
INSTALL = yes
CLEAN = yes

configure: patch
	@sed -i '/^TARGETS/s@ arpd@@g' $(COMPILE_DIR)/misc/Makefile

compile: configure
	@make -C $(COMPILE_DIR) KERNEL_INCLUDE=/usr/include
	@ln -s $(DIR) $(PACKAGE)

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@mkdir -p $(PKG_ROOT)/usr/sbin
	@ln -s /sbin/tc $(PKG_ROOT)/usr/sbin

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(DIR)
	@rm -rf $(COMPILE_DIR)
	@rm -f $(PACKAGE)
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz
endif

include ../Makefile.rules
