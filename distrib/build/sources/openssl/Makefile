# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

OPENSSL_VER = 1.0.1c

PACKAGE = openssl
VERSION = $(OPENSSL_VER)

BASE_URL = http://www.openssl.org/source/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

PKG_DIR = usr

configure: $(DIR)/
	@(cd $(DIR); \
	./config shared --install_prefix=/build/buildroot/$(PACKAGE) --prefix=/$(PKG_DIR) --openssldir=/$(PKG_DIR)/lib/ssl; \
	patch < ../Makefile-MANDIR.patch);

compile: configure
	@make -C $(PACKAGE)-$(VERSION)

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(PACKAGE)-$(OPENSSL_VER) INSTALL_PREFIX=$(PKG_ROOT) install

include ../Makefile.rules
