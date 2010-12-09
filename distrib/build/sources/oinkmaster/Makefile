# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = oinkmaster
VERSION = 2.0

BASE_URL = http://heanet.dl.sourceforge.net/project/$(PACKAGE)/$(PACKAGE)/$(VERSION)

PATCH_FILE1 = wget-no-check-certificate.patch

INSTALL = yes

PATCH_FILE1 = oinkmaster.pl.patch

install: patch
	@find $(COMPILE_DIR) -type d -exec chmod +rx {} \\;
	@mkdir -p $(PKG_ROOT)
	@install -D $(COMPILE_DIR)/oinkmaster.pl $(PKG_ROOT)/usr/bin/oinkmaster.pl
	@install -D $(COMPILE_DIR)/oinkmaster.conf $(PKG_ROOT)/usr/lib/smoothwall/oinkmaster.conf

include ../Makefile.rules
