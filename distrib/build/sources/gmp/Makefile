# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = gmp
VERSION = $(GMP_VER)

BASE_URL = http://www.gnu.org/software/gmp/

PRE_CONFIGURE = ABI=32
EXTENSION = .tar.bz2

CONFIG_OPTS = --prefix=$(PKG_DIR) --enable-cxx --enable-mpbsd

include ../Makefile.rules
