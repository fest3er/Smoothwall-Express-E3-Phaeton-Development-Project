# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = man
VERSION = 1.6f

BASE_URL = http://primates.ximian.com/~flucifredi/man/

PATCH_FILE1 = man-utf8-nroff-fix.patch

CONFIGURE = yes

configure: patch
	@(cd $(DIR); ./configure -d)

include ../Makefile.rules
