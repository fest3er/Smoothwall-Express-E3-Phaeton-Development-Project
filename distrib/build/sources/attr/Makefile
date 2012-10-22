# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = attr
VERSION = 2.4.46

BASE_URL = http://http.us.debian.org/debian/pool/main/a/attr/

DIR = $(PACKAGE)-$(VERSION)
TARBALL = $(PACKAGE)_$(VERSION).orig
EXTENSION = .tar.bz2

CONFIG_OPTS = --prefix=$(PKG_DIR)

INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)
	@DIST_ROOT=${PKG_ROOT} make -C $(COMPILE_DIR) install install-lib install-dev

include ../Makefile.rules
