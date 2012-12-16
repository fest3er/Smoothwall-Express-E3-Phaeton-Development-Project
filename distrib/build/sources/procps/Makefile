# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = procps
VERSION = 3.2.8

BASE_URL = http://procps.sourceforge.net/

CONFIGURE = yes
INSTALL = yes

configure: $(DIR)/
	cd $(DIR); sed -i -e 's/-lncurses/& -ltinfo/' Makefile
	#@true

install: compile
	mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	rm -f $(PKG_ROOT)/lib/libproc.so
		
include ../Makefile.rules
