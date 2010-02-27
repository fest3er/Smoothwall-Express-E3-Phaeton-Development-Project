# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = less
VERSION = 394

BASE_URL = http://www.greenwoodsoftware.com/less/

INSTALL = yes

install: compile
	@(cd $(DIR); \
	install -D less $(PKG_ROOT)$(PKG_DIR)/bin/less);

include ../Makefile.rules
