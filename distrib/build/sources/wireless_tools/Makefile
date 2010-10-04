# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = wireless_tools
VERSION = 28

BASE_URL = http://pcmcia-cs.sourceforge.net/ftp/contrib/

TARBALL = $(PACKAGE).$(VERSION)

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: patch
	@true

compile: configure
	@make -C $(DIR) PREFIX=/usr

install: compile
	@mkdir -p $(PKG_ROOT)
	make -C $(DIR) PREFIX=$(PKG_ROOT)$(PKG_DIR) install

include ../Makefile.rules
