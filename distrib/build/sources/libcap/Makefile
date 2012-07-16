# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = libcap
VERSION = 2.22

BASE_URL = http://http.us.debian.org/debian/pool/main/libc/libcap2/

DIR = $(PACKAGE)-$(VERSION)
TARBALL = $(PACKAGE)2_$(VERSION).orig

CONFIG_OPTS = --prefix=$(PKG_DIR)

CONFIGURE = yes

configure: patch
	@true;

INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) install
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install

include ../Makefile.rules
