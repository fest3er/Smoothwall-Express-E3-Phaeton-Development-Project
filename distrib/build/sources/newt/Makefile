# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = newt
VERSION = 0.51.6
#VERSION = 0.52.2

BASE_URL = http://distro.ibiblio.org/pub/linux/distributions/amigolinux/download/Libs/$(PACKAGE)-$(VERSION)

EXTENSION = .tar.bz2

CONFIG_OPTS = --without-tcl
INSTALL = yes

install: compile
	#find $(PACKAGE)-$(VERSION) -type d -exec chmod o+rx {} \;
	#find $(PACKAGE)-$(VERSION) -type f -exec chmod o+r {} \;
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) instroot=$(PKG_ROOT) install

include ../Makefile.rules
