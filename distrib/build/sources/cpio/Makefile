# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = cpio
VERSION = 2.9

BASE_URL = ftp://ftp.gnu.org/gnu/cpio

PATCH_FILE1 = cpio-2.9-gcc4_3.patch

CONFIG_OPTS = --prefix=/usr \
              --bindir=/bin \
              --libexecdir=/tmp \
              --with-rmt=/usr/sbin/rmt

include ../Makefile.rules
