# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = logrotate
VERSION = 3.7.1

BASE_URL = http://ftp.debian.org/debian/pool/main/l/logrotate

DIR = $(PACKAGE)-$(VERSION)
TARBALL = $(PACKAGE)_$(VERSION).orig

PATCH_FILE1 = logrotate_3.7-2.diff.gz

CONFIGURE = yes
INSTALL = yes

configure: patch
	@true;

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(DIR) PREFIX=$(PKG_ROOT) install

include ../Makefile.rules
