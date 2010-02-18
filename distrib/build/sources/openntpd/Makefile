# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = openntpd
VERSION = 3.9p1

#BASE_URL = http://www.openntpd.org/dist/portable
BASE_URL = ftp://ftp.openbsd.org/pub/OpenBSD/OpenNTPD/

CONFIG_OPTS = --with-ssl-dir=/usr/ssl

PATCH_FILE1 = openntpd-username.patch
PATCH_FILE2 = openntpd-pidfile.patch

INSTALL = yes

install: compile
	@install -D $(COMPILE_DIR)/ntpd $(PKG_ROOT)/$(PKG_DIR)/sbin/ntpd

include ../Makefile.rules
