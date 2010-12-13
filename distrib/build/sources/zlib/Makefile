# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = zlib
VERSION = 1.2.5

BASE_URL = http://www.zlib.net/

INSTALL = yes

CONFIG_OPTS = --prefix=$(PKG_DIR) --shared --libdir=/lib

install: compile
	@mkdir -p $(PKG_ROOT)
	make -C $(DIR) prefix=$(PKG_ROOT)$(PKG_DIR) libdir=$(PKG_ROOT)$(PKG_DIR)/lib install
	rm -vf $(PKG_ROOT)/lib/libz.so;
	ln -sfv ../../lib/libz.so.1.2.3 $(PKG_ROOT)$(PKG_DIR)/lib/libz.so
	cd $(COMPILE_DIR); make clean;
	cd $(COMPILE_DIR); ./configure --prefix=$(PKG_DIR);
	cd $(COMPILE_DIR); make;
	cd $(COMPILE_DIR); make prefix=$(PKG_ROOT)$(PKG_DIR) libdir=$(PKG_ROOT)$(PKG_DIR)/lib install;
	cd $(COMPILE_DIR); chmod -v 644 $(PKG_ROOT)$(PKG_DIR)/lib/libz.a

include ../Makefile.rules
