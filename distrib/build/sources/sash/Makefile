# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = sash
VERSION = 3.4

BASE_URL = http://www.canb.auug.org.au/~dbell/programs/

PATCH_FILE1 = sash-3.4-scriptarg.patch

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: patch
	@true

compile: configure
	@make -C $(COMPILE_DIR) CFLAGS="$(CFLAGS) -DHAVE_GZIP"

install: compile
	@install -D $(COMPILE_DIR)/sash $(PKG_ROOT)/bin/sash

include ../Makefile.rules
