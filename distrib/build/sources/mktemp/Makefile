# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = mktemp
VERSION = 1.7

#BASE_URL = http://www.aslak.net/smoothmirror/
#BASE_URL = http://www.mktemp.org/mktemp/dist/OLD/
BASE_URL = http://www.mktemp.org/mktemp/dist/

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) install

include ../Makefile.rules
