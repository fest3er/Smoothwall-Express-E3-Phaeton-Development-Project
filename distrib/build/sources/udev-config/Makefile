# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = udev-config
VERSION = 20090523

EXTENSION = .tar.bz2

BASE_URL = http://www.linuxfromscratch.org/lfs/downloads/6.5/

INSTALL = yes
install: patch
	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install-doc
	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install-extra-doc

CLEAN = yes
clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(PKG_ROOT)
	@rm -rf $(PACKAGE)-$(VERSION)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz


include ../Makefile.rules

