# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = newt
VERSION = 0.51.6

BASE_URL = http://distro.ibiblio.org/pub/linux/distributions/amigolinux/download/Libs/$(PACKAGE)-$(VERSION)

EXTENSION = .tar.bz2

INSTALL = yes

install: compile
	@make -C $(COMPILE_DIR) instroot=$(PKG_ROOT) install

include ../Makefile.rules
