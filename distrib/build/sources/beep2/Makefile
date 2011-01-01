# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = beep2
VERSION = 1.2a

BASE_URL =  http://www.kip.iis.toyama-u.ac.jp/~shingo/beep/package/src/

DOWNLOAD = yes
CONFIGURE = yes
INSTALL = yes
CLEAN = yes

download:
        @true

configure:
        @true
        
install: compile
        @mkdir -p $(PKG_ROOT)
        @make -C $(DIR) PREFIX=$(PKG_ROOT)/usr/ install

clean:
        @make -C $(DIR) clean >/dev/null 2>&1

include ../Makefile.rules
