# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = module-init-tools
VERSION = 3.15

BASE_URL = http://www.kernel.org/pub/linux/utils/kernel/module-init-tools

PKG_DIR = 

#CONFIG_OPTS = --mandir=/usr/man --enable-insmod-static

COMPILE = yes

compile: configure
	@make $(JOBS) -C $(DIR) DOCBOOKTOMAN=/bin/true

include ../Makefile.rules
