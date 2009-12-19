# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = beep2
VERSION = 1.2a

BASE_URL =  http://www.kip.iis.toyama-u.ac.jp/~shingo/beep/package/src/

CONFIGURE = yes
INSTALL = yes

configure: patch
	@true
	
install: compile
	@make -C $(DIR) PREFIX=$(PKG_ROOT)/usr/ install

include ../Makefile.rules
