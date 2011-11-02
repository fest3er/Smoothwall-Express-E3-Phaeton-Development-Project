# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = acpica
VERSION = unix2-20110922

BASE_URL = http://acpica.org/download

CONFIGURE = yes
configure: $(DIR)/
	true

COMPILE = yes
compile: configure
	cd $(COMPILE_DIR)/compiler; sed -i -e '/Werror/d' Makefile; make $(JOBS)

INSTALL = yes
install: compile
	mkdir -p $(PKG_ROOT)$(PKG_DIR)/bin
	cd $(COMPILE_DIR)/compiler; make INSTALLDIR=$(PKG_ROOT)$(PKG_DIR)/bin install

include ../Makefile.rules
