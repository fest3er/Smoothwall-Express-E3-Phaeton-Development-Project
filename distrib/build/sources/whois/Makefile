# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = whois
VERSION = 5.0

EXTENSION = .tgz

BASE_URL = http://whois.bw.org/dist/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@true
	
compile: configure
	@true
	
install: compile
	@mkdir -p $(PKG_ROOT)
	@install -D $(DIR)/whois $(PKG_ROOT)/usr/bin/whois
	@install -D $(DIR)/tld.conf $(PKG_ROOT)/etc/whois/tld.conf
	
include ../Makefile.rules
