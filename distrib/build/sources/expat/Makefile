# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = expat
VERSION = 2.0.0
SFMIRROR = heanet
BASE_URL = http://$(SFMIRROR).dl.sourceforge.net/sourceforge/$(PACKAGE)/

INSTALL = yes

install: compile
	@make -C $(COMPILE_DIR) install prefix=$(PKG_ROOT)/$(PKG_DIR)

include ../Makefile.rules
