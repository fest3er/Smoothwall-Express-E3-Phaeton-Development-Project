# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = syslinux
VERSION = 2.09

BASE_URL = http://www.kernel.org/pub/linux/utils/boot/syslinux/Old/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@true

compile: configure
	@make -C $(COMPILE_DIR) syslinux

install: compile
	@make -C $(COMPILE_DIR) INSTALLROOT=$(PKG_ROOT) install

include ../Makefile.rules
