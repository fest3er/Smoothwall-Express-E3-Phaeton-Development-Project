# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = hddtemp
#VERSION = 0.3-beta13
VERSION = 0.3-beta15

BASE_URL = http://ftp.debian.org/debian/pool/main/h/hddtemp/

DIR = $(PACKAGE)-$(VERSION)
TARBALL = $(PACKAGE)_$(VERSION).orig
HDTEMPDB = hddtemp.db

#PATCH_FILE1 = hddtemp_0.3-beta12-6.diff.gz
#PATCH_FILE2 = smoothwall-locale-fix.patch

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)
	@install -D $(COMPILE_DIR)/src/hddtemp $(PKG_ROOT)/usr/sbin/hddtemp
	@install -D hddtemp.db $(PKG_ROOT)/usr/share/misc/hddtemp.db

include ../Makefile.rules
