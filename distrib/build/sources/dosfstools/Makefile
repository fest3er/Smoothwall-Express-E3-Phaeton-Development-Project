# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = dosfstools
VERSION = 2.11

BASE_URL = ftp://ftp.uni-erlangen.de/pub/Linux/LOCAL/dosfstools/

TARBALL = $(PACKAGE)-$(VERSION).src

DIR = $(PACKAGE)-$(VERSION)

CONFIGURE = yes
INSTALL = yes

configure: patch
	@true

install: compile
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT); mkdir -p $(PKG_ROOT)
endif
	@make -C $(COMPILE_DIR) PREFIX=$(PKG_ROOT) install

include ../Makefile.rules
