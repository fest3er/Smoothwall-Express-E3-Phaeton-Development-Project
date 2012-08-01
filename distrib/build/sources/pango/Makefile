# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = pango
VER_MM = 1.29
VERSION = $(VER_MM).4
EXTENSION = .tar.xz

BASE_URL = http://ftp.gnome.org/pub/GNOME/sources/$(PACKAGE)/$(VER_MM)

CONFIG_OPTS = --prefix=/usr --without-x
INSTALL = yes
install: compile
	mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) install DESTDIR=$(PKG_ROOT)
	install -dv $(PKG_ROOT)$(PKG_DIR)/etc/pango

include ../Makefile.rules

