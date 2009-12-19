# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = tcpdump
VERSION = 2007.02.12

BASE_URL = http://www.tcpdump.org/weekly/

INSTALL = yes

install: compile
	@install -D $(DIR)/tcpdump $(PKG_ROOT)/$(PKG_DIR)/sbin/tcpdump

include ../Makefile.rules
