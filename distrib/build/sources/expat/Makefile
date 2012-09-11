# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = expat
VERSION = 2.1.0

BASE_URL = http://hivelocity.dl.sourceforge.net/project/$(PACKAGE)/$(PACKAGE)/$(VERSION)/

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) install prefix=$(PKG_ROOT)$(PKG_DIR)

include ../Makefile.rules
