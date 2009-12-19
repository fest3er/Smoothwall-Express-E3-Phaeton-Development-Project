# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = dhcpcd
VERSION =  3.2.3

BASE_URL = http://ftp.debian.org/debian/pool/main/d/dhcpcd/

TARBALL = $(PACKAGE)_$(VERSION).orig

DIR =  $(PACKAGE)-$(VERSION).orig

CONFIG_OPTS = --prefix=$(PKG_DIR)

CONFIGURE = yes

configure: patch
	@true

INSTALL = yes

install: compile
	make -C $(DIR) DESTDIR=$(PKG_ROOT) install
	@mkdir -p $(PKG_ROOT)$(PKG_DIR)/sbin
	@(cd $(PKG_ROOT)$(PKG_DIR)/sbin; rm -f dhcpcd; ln -s /sbin/dhcpcd)

include ../Makefile.rules
