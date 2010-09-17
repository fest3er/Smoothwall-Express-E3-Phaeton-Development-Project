# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = bash
VERSION = 4.1

BASE_URL = ftp://ftp.gnu.org/gnu/bash/

PATCH_URL1 = http://www.linuxfromscratch.org/patches/lfs/6.4/bash-3.2-fixes-8.patch

ifeq ($(ARCH), i686)
BUILD = --build=i686-pc-linux-gnu
endif

ifeq ($(BUILD_PHASE), Stage_3)
CONFIG_OPTS = --prefix=${TOOLS_DIR} --without-bash-malloc

INSTALL = yes
install: compile
	@make -C $(DIR) DESTDIR=$(PKG_ROOT) install
	@(cd $(PKG_ROOT)$(PKG_DIR)/bin; ln -s bash sh);
endif

ifeq ($(BUILD_PHASE), Final)
CONFIG_OPTS = --bindir=/bin $(BUILD)

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(DIR) DESTDIR=$(PKG_ROOT) install
	@(cd $(PKG_ROOT)/bin; ln -s bash sh);
endif

include ../Makefile.rules
