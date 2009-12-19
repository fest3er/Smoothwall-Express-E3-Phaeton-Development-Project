# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = expect
VERSION = 5.42

BASE_URL = http://expect.nist.gov/old/

PATCH_URL1 = http://www.linuxfromscratch.org/patches/lfs/6.0/expect-5.42.1-spawn-1.patch


TARBALL = $(PACKAGE)-$(VERSION).1

DIR = $(PACKAGE)-$(VERSION)

CONFIG_OPTS = --with-tcl=/$(PKG_DIR)/lib \
	--with-tclinclude=$(SOURCES_DIR)/tcl/tcl$(TCL_VER) --with-x=no

include ../Makefile.rules
