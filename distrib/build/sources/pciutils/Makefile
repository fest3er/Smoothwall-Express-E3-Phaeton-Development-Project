# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = pciutils
VERSION = 3.1.7

BASE_URL = ftp://ftp.kernel.org/pub/software/utils/pciutils/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@true

compile: configure
	@make -C $(COMPILE_DIR) PREFIX=/$(PKG_DIR) ZLIB=no

install: compile
	@make -C $(COMPILE_DIR) PREFIX=$(PKG_ROOT)$(PKG_DIR) install

include ../Makefile.rules
