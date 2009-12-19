# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = lilo
VERSION = 22.7.3

BASE_URL = http://home.san.rr.com/johninsd/pub/linux/lilo/

TARBALL = $(PACKAGE)-$(VERSION).src

DIR = $(PACKAGE)-$(VERSION)

CONFIGURE = yes
INSTALL = yes

configure: $(DIR)/
	 true

install: compile
	@mkdir -p $(PKG_ROOT)/$(PKG_DIR)/sbin
	@make -C $(COMPILE_DIR) ROOT=$(PKG_ROOT) install
	@cp $(COMPILE_DIR)/{bitmap.b,bootsect.b} $(PKG_ROOT)/boot

include ../Makefile.rules
