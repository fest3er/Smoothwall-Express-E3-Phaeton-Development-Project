# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = xen
VERSION = 4.1.2

BASE_URL = http://bits.xensource.com/oss-xen/release/$(VERSION)

CONFIGURE = yes
configure: $(DIR)/
	true

COMPILE = yes
compile: configure
	chmod 644 $(PACKAGE)-$(VERSION)/tools/check/check_x11_devel
	make $(JOBS) -C $(COMPILE_DIR) tools

INSTALL = yes
install: compile
	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install-tools

include ../Makefile.rules
