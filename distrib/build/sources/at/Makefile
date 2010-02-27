# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = at
VERSION = 3.1.10.2

BASE_URL = http://ftp.debian.org/debian/pool/main/a/at
TARBALL = $(PACKAGE)_$(VERSION)
DIR = $(PACKAGE)-$(VERSION)

PATCH_FILE1 = no-sendmail.patch

CONFIG_OPTS = --with-jobdir=/var/spool/atjobs --with-atspool=/var/spool/atspool --without-sendmail \
	       --with-daemon_username=nobody --with-daemon_groupname=nobody

INSTALL = yes

install: compile
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) install

include ../Makefile.rules
