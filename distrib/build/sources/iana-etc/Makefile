# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = iana-etc
VERSION = 2.30

EXTENSION = .tar.bz2

BASE_URL = http://www.sethwklein.net/projects/iana-etc/downloads/

DOWNLOAD = yes
CONFIGURE = yes
INSTALL = yes
CLEAN = yes

download:
	@true

configure: 
	@true
	
install: compile
	@mkdir -p $(PKG_ROOT)/etc
	@make -C $(COMPILE_DIR) PREFIX=$(PKG_ROOT) install

clean:
	@make -C $(COMPILE_DIR) clean

include ../Makefile.rules
