# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = bin86
VERSION = 0.16.17

BASE_URL = http://homepage.ntlworld.com/robert.debath/dev86/

CONFIGURE = yes
INSTALL = yes

configure: patch
	@true

install: compile
	mkdir -p $(PKG_ROOT)$(PKG_DIR)
	@make -C $(COMPILE_DIR)/as PREFIX=$(PKG_ROOT)$(PKG_DIR) install
	@make -C $(COMPILE_DIR)/ld PREFIX=$(PKG_ROOT)$(PKG_DIR) install

include ../Makefile.rules
