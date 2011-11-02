# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = pango
MAJMIN = 1.29
VERSION = $(MAJMIN).4
EXTENSION = .tar.xz

BASE_URL = http://ftp.gnome.org/pub/GNOME/sources/$(PACKAGE)/$(MAJMIN)

INSTALL = yes
install: compile
	mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) install DESTDIR=$(PKG_ROOT)


include ../Makefile.rules

