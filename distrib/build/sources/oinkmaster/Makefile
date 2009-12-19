# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = oinkmaster
VERSION = 1.2

BASE_URL = http://www.snort.org/dl/contrib/rule_management/oinkmaster

INSTALL = yes

install: patch
	@install -D $(COMPILE_DIR)/oinkmaster.pl $(PKG_ROOT)/usr/bin/oinkmaster.pl
	@install -D $(COMPILE_DIR)/oinkmaster.conf $(PKG_ROOT)/usr/lib/smoothwall/oinkmaster.conf

include ../Makefile.rules
