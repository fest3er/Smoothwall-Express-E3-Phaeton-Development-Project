# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = cron
VERSION = 3.0pl1

BASE_URL = http://ftp.debian.org/debian/pool/main/c/cron/

DIR = $(PACKAGE)-$(VERSION).orig
TARBALL = $(PACKAGE)_$(VERSION).orig

PATCH_FILE1 = cron_3.0pl1-87.diff.gz
PATCH_FILE2 = smoothwall-pid.patch

CONFIGURE = yes
INSTALL = yes

configure: patch
	@true;

install: compile
	-mkdir $(PKG_ROOT)
	@mkdir -p $(PKG_ROOT)/usr/{bin,sbin} $(PKG_ROOT)/usr/share/man/cat{1,5,8}
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install

include ../Makefile.rules
