# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = sysvinit
VERSION = 2.86

BASE_URL = ftp://ftp.cistron.nl/pub/people/miquels/sysvinit/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@true

compile: configure
	@make -C $(DIR)/src CFLAGS="$(CFLAGS) -D_GNU_SOURCE"

install: compile
	@mkdir -p $(PKG_ROOT)/{sbin,/bin,/usr/bin,/usr/include}
	@mkdir -p $(PKG_ROOT)/usr/share/man/man{1,5,8}
	@make -C $(DIR)/src ROOT=$(PKG_ROOT) install
	@mkdir $(PKG_ROOT)/dev
	@mknod -m 600 $(PKG_ROOT)/dev/initctl p

include ../Makefile.rules
