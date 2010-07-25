# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = module-init-tools
VERSION = 3.11.1

BASE_URL = http://ftp.kernel.org/pub/linux/kernel/people/rusty/modules

PKG_DIR = 

#CONFIG_OPTS = --mandir=/usr/man --enable-insmod-static

COMPILE = yes

compile: configure
	@make -C $(DIR) DOCBOOKTOMAN=/bin/true

include ../Makefile.rules
