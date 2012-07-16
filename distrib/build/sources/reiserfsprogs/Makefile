# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = reiserfsprogs
VERSION = 3.6.21

BASE_URL = http://fossies.org/linux/misc/

#COMPILE_DIR = $(DIR)-compile


ifeq ($(BUILD_PHASE), Final)

CONFIG_OPTS = --prefix=$(PKG_DIR) --sbindir=/sbin


INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	ln -sf reiserfsck $(PKG_ROOT)/sbin/fsck.reiserfs
	ln -sf mkreiserfs $(PKG_ROOT)/sbin/mkfs.reiserfs
endif
	
include ../Makefile.rules
