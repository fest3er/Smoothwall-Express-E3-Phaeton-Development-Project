# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = vlan
VERSION = 1.8

TARBALL = $(PACKAGE).$(VERSION)

DIR = $(PACKAGE)

BASE_URL = http://www.candelatech.com/~greear/vlan

CONFIGURE = yes
INSTALL = yes

configure: patch
	@true

install: compile
	@install -D $(COMPILE_DIR)/vconfig $(PKG_ROOT)/sbin/vconfig

include ../Makefile.rules
