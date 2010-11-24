# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = syslinux
VERSION = 4.03

BASE_URL = http://www.kernel.org/pub/linux/utils/boot/syslinux/4.xx

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@true

compile: configure
	@make $(JOBS) -C $(COMPILE_DIR) installer

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) INSTALLROOT=$(PKG_ROOT) install

include ../Makefile.rules
