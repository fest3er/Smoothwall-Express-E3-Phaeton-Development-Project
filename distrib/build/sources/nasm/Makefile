# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = nasm
VERSION = 0.98.39

BASE_URL = http://heanet.dl.sourceforge.net/sourceforge/nasm

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)$(PKG_DIR)/bin $(PKG_ROOT)$(PKG_DIR)/man/man1
	@make -C $(COMPILE_DIR) INSTALLROOT=$(PKG_ROOT) install

include ../Makefile.rules
