# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = gmp
VERSION = $(GMP_VER)

BASE_URL = ftp://ftp.gnu.org/gnu/gmp
EXTENSION = .tar.bz2

ifeq ($(ARCH), i686)
CONFIG_OPTS = --disable-nls --host=i686-pc-linux-gnu
endif

include ../Makefile.rules
