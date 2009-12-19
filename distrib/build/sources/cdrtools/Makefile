# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = cdrtools
VERSION = 2.01

BASE_URL = ftp://ftp.berlios.de/pub/cdrecord/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@true

compile: configure
	@make -C $(COMPILE_DIR)

install: compile
	@install -D $(COMPILE_DIR)/mkisofs/OBJ/*/mkisofs $(PKG_ROOT)/sbin/mkisofs

include ../Makefile.rules
