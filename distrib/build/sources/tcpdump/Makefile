# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = tcpdump
VERSION = 4.1.1

BASE_URL = http://www.tcpdump.org/release/

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)
	@install -D $(DIR)/tcpdump $(PKG_ROOT)$(PKG_DIR)/sbin/tcpdump

include ../Makefile.rules
