# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

OPENSSL_VER = 0.9.8o

PACKAGE = openssl
VERSION = $(OPENSSL_VER)

BASE_URL = http://www.openssl.org/source/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

PKG_DIR = usr/ssl

configure: $(DIR)/
	@(cd $(DIR); \
	./config 386 shared --prefix=/$(PKG_DIR) --openssldir=/$(PKG_DIR));

compile: configure
	@make -C openssl-$(OPENSSL_VER)

install: compile
	@make -C openssl-$(OPENSSL_VER) INSTALL_PREFIX=$(PKG_ROOT) install

include ../Makefile.rules
