# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = diffutils
VERSION = 3.2

BASE_URL = ftp://ftp.gnu.org/gnu/diffutils/

CONFIGURE = yes

configure: patch
	cd $(DIR); sed -i -e '/gets is a/d' lib/stdio.in.h
	cd $(DIR); ./configure --prefix=$(PKG_DIR)

include ../Makefile.rules
