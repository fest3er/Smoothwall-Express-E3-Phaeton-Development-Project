# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = ntp
VERSION = 4.2.4p2

CONFIG_OPTS = --enable-linuxcaps

BASE_URL = http://www.eecis.udel.edu/~ntp/ntp_spool/ntp4/

INSTALL = yes

install: compile
	@install -D $(COMPILE_DIR)/ntpdate/ntpdate $(PKG_ROOT)/usr/sbin/ntpdate 
	@install -D $(COMPILE_DIR)/ntpq/ntpq $(PKG_ROOT)/usr/sbin/ntpq 

include ../Makefile.rules
