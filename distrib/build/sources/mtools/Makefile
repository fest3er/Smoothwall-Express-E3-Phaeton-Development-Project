# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = mtools
VERSION = 3.9.10

BASE_URL = http://mtools.linux.lu/

INSTALL = yes

install: compile
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) install

include ../Makefile.rules
