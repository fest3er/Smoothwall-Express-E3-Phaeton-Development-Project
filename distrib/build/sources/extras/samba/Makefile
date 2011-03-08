# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../Makefile.conf

#Samba added by Daniel for development only.

PACKAGE = samba
VERSION = 3.5.8

BASE_URL = http://samba.org/samba/ftp/

DIR = $(PACKAGE)-$(VERSION)/source

CONFIG_OPTS = --disable-nls --sysconfdir=/etc --localstatedir=/var --with-configdir=/etc/samba --with-logfilebase=/var

CLEAN = yes

clean:
	@rm -fr $(PACKAGE)-$(VERSION)
	@rm -rf $(COMPILE_DIR)

include ../../Makefile.rules
