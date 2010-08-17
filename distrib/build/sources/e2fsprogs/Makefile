# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = e2fsprogs
VERSION = 1.41.12

BASE_URL = http://internap.dl.sourceforge.net/sourceforge/e2fsprogs/

COMPILE_DIR = $(DIR)-compile


ifeq ($(BUILD_PHASE), Stage_3)

CONFIG_OPTS = --prefix=$(TOOLS_DIR)

INSTALL = yes
install: compile
	@make -C $(COMPILE_DIR) install-libs
	chmod -v u+w $(TOOLS_DIR)/lib/{libblkid,libuuid,libcom_err,libe2p,libext2fs,libss}.a

endif


ifeq ($(BUILD_PHASE), Final)

CONFIG_OPTS = --prefix=$(PKG_DIR) --with-root-prefix="" --enable-elf-shlibs

PATCH = yes

patch: $(DIR)/
	sed -i 's@/bin/rm@$(TOOLS_DIR)&@' $(DIR)/lib/blkid/test_probe.in
#	sed -i -e "s/July/Jul/" $(DIR)/version.h

INSTALL = yes
install: compile
	tput smso
	echo $(PKG_ROOT)
	tput rmso
	-mkdir $(PKG_ROOT)
	make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) root_prefix=$(PKG_ROOT) install
	make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) root_prefix=$(PKG_ROOT) install-libs
	chmod -v u+w $(PKG_ROOT)$(PKG_DIR)/lib/{libblkid,libcom_err,libe2p,libext2fs,libss,libuuid}.a
endif
	
include ../Makefile.rules
