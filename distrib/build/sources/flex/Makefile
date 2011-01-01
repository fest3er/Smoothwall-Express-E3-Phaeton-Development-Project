# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = flex
VERSION = 2.5.35

#BASE_URL = ftp://ftp.gnu.org/non-gnu/flex/
BASE_URL = http://prdownloads.sourceforge.net/flex/

#TARBALL = $(PACKAGE)-$(VERSION)a  # For V2.5.4
TARBALL = $(PACKAGE)-$(VERSION)

DIR = $(PACKAGE)-$(VERSION)

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)
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
