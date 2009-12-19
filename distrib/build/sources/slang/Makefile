# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = slang
VERSION = 1.4.9

BASE_URL = ftp://space.mit.edu/pub/davis/slang/v1.4/

INSTALL = yes

install: compile
	@mkdir $(PKG_ROOT)
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@rm -f $(PKG_ROOT)/$(PKG_DIR)/lib/libsnewt*.so*

include ../Makefile.rules
