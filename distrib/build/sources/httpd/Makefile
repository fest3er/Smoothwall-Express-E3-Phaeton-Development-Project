# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2010
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = httpd
VERSION = 2.2.16

LICENCE_FILE = $(DIR)/COPYING

BASE_URL = http://apache.mirror.rbftpnetworks.com/httpd/

CONFIG_OPTS = --enable-ssl --with-ssl=/usr/ssl \
	--with-included-apr --enable-mods-shared=all --enable-layout=RedHat

CONFIGURE = yes
INSTALL = yes

configure: patch
	@(cd $(DIR); ./configure $(CONFIG_OPTS) --prefix=/usr/apache2)

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(DIR) DESTDIR=$(PKG_ROOT) install
	@rm -rf $(PKG_ROOT)/{etc,var}

include ../Makefile.rules
