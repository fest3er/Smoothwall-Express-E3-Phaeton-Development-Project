# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = suricata
VERSION = 1.4

DIR = $(PACKAGE)-$(VERSION)

BASE_URL = http://www.openinfosecfoundation.org/download/

CONFIG_OPTS = --disable-nls --prefix=/usr --enable-nfqueue --sysconfdir=/etc --localstatedir=/var

INSTALL = yes
install: compile
	cd $(COMPILE_DIR); \
	  make DESTDIR=$(PKG_ROOT) install

include ../Makefile.rules
