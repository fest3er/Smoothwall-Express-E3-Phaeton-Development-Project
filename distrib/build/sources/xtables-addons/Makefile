# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = xtables-addons
VERSION =  1.12

EXTENSION = .tar.bz2

BASE_URL = http://downloads.sourceforge.net/project/xtables-addons/Xtables-addons/$(VERSION)

CONFIG_OPTS = --with-gnu-ld \
	      --with-kbuild=/build/sources/kernel-phaeton/linux/ \
	      --with-ksource=/lib/modules/$(K_SWE_VERSION)/source


include ../Makefile.rules
