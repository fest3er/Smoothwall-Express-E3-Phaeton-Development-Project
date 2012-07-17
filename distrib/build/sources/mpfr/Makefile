# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = mpfr
VERSION = $(MPFR_VER)

BASE_URL = http://www.mpfr.org/

EXTENSION = .tar.bz2

CONFIG_OPTS = --prefix=$(PKG_DIR) \
	      --enable-thread-safe \
	      --docdir=/usr/share/doc/mpfr-3.1.0

PATCH_FILE1 = mpfr-3.1.0-fixes-1.patch

include ../Makefile.rules
