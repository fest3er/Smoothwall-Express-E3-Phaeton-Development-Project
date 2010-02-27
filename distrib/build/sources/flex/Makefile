# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = flex
VERSION = 2.5.4

BASE_URL = ftp://ftp.gnu.org/non-gnu/flex/

TARBALL = $(PACKAGE)-$(VERSION)a

DIR = $(PACKAGE)-$(VERSION)

INSTALL = yes

install: compile
ifdef BITS_64
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) libdir=$(PKG_ROOT)$(PKG_DIR)/lib64 install
	@(cd $(PKG_ROOT)$(PKG_DIR)/lib64; ln -s libfl.a libl.a)
else
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) libdir=$(PKG_ROOT)$(PKG_DIR)/lib install
	@(cd $(PKG_ROOT)$(PKG_DIR)/lib; ln -s libfl.a libl.a)
endif
	@cp lex $(PKG_ROOT)$(PKG_DIR)/bin
	@chmod 775 $(PKG_ROOT)$(PKG_DIR)/bin/lex
	
include ../Makefile.rules
