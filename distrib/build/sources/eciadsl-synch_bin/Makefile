# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = eciadsl-synch_bin
VERSION = 0.0

BASE_URL = http://eciadsl.flashtux.org/download/

TARBALL = $(PACKAGE)
DIR = $(PACKAGE)

EXTENSION = .tar.bz2

PKG_DIR = /

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@true

compile: configure
	@true

install: compile
	@mkdir -p $(PKG_ROOT)$(PKG_DIR)/etc/eciadsl
	@install -D $(DIR)/*.bin $(PKG_ROOT)$(PKG_DIR)/etc/eciadsl/

include ../Makefile.rules
