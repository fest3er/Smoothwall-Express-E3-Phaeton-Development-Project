# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = slang
#VERSION = 1.4.9
MAJORVERSION = 2.0
VERSION = $(MAJORVERSION).7

BASE_URL = ftp://space.mit.edu/pub/davis/slang/v$(MAJORVERSION)/

INSTALL = yes

install: compile
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT); mkdir -p $(PKG_ROOT)
endif
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@make -C $(COMPILE_DIR) DESTDIR=/ install
	@rm -f $(PKG_ROOT)$(PKG_DIR)/lib/libsnewt*.so*

include ../Makefile.rules
