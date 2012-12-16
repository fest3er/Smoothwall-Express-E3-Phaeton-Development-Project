# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = db
VERSION = 5.3.21

BASE_URL = http://download.oracle.com/berkeley-db/

CONFIG_OPTS= --prefix=/usr --enable-compat185 --enable-dbm --enable-cxx \
	     --disable-static

PATCH_FILE1 = ../Makefile.in.patch

DIR = $(PACKAGE)-$(VERSION)/build_unix

PATCH = yes
CONFIGURE = yes

patch: $(DIR)/
	@(cd $(DIR)/..; cat $(PATCH_FILE1) | patch -p1)

configure: patch
	cd $(DIR); ../dist/configure $(CONFIG_OPTS)

include ../Makefile.rules
