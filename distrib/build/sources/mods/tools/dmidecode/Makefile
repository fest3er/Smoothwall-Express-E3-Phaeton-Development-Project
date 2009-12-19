# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../../Makefile.conf

PACKAGE = dmidecode
VERSION = 2.9

BASE_URL = http://download.savannah.gnu.org/releases/dmidecode/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: patch
	@true

compile: configure
	@make -C $(DIR) prefix=/$(PKG_DIR)

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(DIR) DESTDIR=$(PKG_ROOT) prefix=/$(PKG_DIR) install

include ../../../Makefile.rules
