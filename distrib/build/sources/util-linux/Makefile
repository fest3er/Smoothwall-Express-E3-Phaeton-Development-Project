# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = util-linux
VER_MM = 2.21
VERSION = $(VER_MM).2

BASE_URL = http://www.kernel.org/pub/linux/utils/util-linux/v$(VER_MM)/

ifeq ($(BUILD_PHASE), Stage_3)

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	cd $(DIR); ./configure --prefix=$(TOOLS_DIR)

compile: configure
	make $(JOBS) -C $(COMPILE_DIR)/mount BLKID_LIBS="-lblkid -luuid" mount umount
	make $(JOBS) -C $(COMPILE_DIR)/text-utils more

install: compile
	cd $(COMPILE_DIR); cp mount/.libs/umount mount/.libs/mount text-utils/more $(TOOLS_DIR)/bin
	cd $(COMPILE_DIR); cp libblkid/src/.libs/libblk*.so*  $(TOOLS_DIR)/lib
	cd $(COMPILE_DIR); cp libuuid/src/.libs/libuuid*.so*  $(TOOLS_DIR)/lib
endif


ifeq ($(BUILD_PHASE), Final)

CONFIG_OPTS = --enable-arch --enable-partx --enable-write

endif


include ../Makefile.rules
