# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = shadow
VERSION = 4.1.4.2

BASE_URL = ftp://pkg-shadow.alioth.debian.org/pub/pkg-shadow

INSTALL = yes

CONFIG_OPTS= --with-selinux=no

install: compile
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@rm -f $(PKG_ROOT)/bin/sg $(PKG_ROOT)/bin/vigr

include ../Makefile.rules
