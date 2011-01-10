# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = ntp
VERSION = 4.2.6p3

PATCH_FILE1 = mod_nano.patch

#CONFIG_OPTS = --enable-linuxcaps --with-privsep=ntpd
CONFIG_OPTS = --prefix=${PKG_DIR} --sysconfdir=/etc \
		--with-binsubdir=sbin

BASE_URL = http://www.eecis.udel.edu/~ntp/ntp_spool/ntp4/

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)
	@install -D $(COMPILE_DIR)/ntpdate/ntpdate $(PKG_ROOT)/usr/sbin/ntpdate 
	@install -D $(COMPILE_DIR)/ntpq/ntpq $(PKG_ROOT)/usr/sbin/ntpq 

include ../Makefile.rules
