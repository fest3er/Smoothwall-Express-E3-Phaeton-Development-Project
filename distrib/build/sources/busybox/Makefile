# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = busybox
VERSION = 1.15.0

BASE_URL = http://busybox.net/downloads/
EXTENSION = .tar.bz2

PKG_DIR = /opt/busybox

CONFIGURE = yes
INSTALL = yes

configure: $(DIR)/
	@cp -avR config $(COMPILE_DIR)/.config

install: compile
	@mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) CONFIG_PREFIX=$(PKG_ROOT)$(PKG_DIR) install

include ../Makefile.rules
