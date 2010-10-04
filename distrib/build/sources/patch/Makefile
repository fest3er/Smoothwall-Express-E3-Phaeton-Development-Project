# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = patch
VERSION = 2.6.1

BASE_URL = ftp://ftp.gnu.org/gnu/patch

ifeq ($(BUILD_PHASE), Phase_3)
CONFIG_OPTS = --prefix=$(TOOLS_DIR)
endif

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) install

include ../Makefile.rules
