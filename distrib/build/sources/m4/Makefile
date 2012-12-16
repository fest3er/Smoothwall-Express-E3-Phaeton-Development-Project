# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = m4
VERSION = 1.4.16

BASE_URL = ftp://ftp.gnu.org/gnu/m4

CONFIGURE = yes
INSTALL = yes

configure: patch
	cd $(DIR); sed -i -e '/gets is a/d' lib/stdio.in.h
	cd $(DIR); ./configure --prefix=$(PKG_DIR)

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) install

include ../Makefile.rules
