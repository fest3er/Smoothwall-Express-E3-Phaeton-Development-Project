# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = grep
VERSION = 2.5

BASE_URL = ftp://ftp.gnu.org/gnu/grep/

PATCH_FILE1 = grep-links.patch

ifeq ($(BUILDENV), 1)
CONFIG_OPTS = --disable-perl-regexp --with-included-regex
else
CONFIG_OPTS = --bindir=/bin --with-included-regex
endif

include ../Makefile.rules
