# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = iputils
#VERSION = 20020927
VERSION = s20100214

BASE_URL = http://www.skbuff.net/iputils

#DIR = $(PACKAGE)
EXTENSION=.tar.bz2
#TARBALL = $(PACKAGE)_$(VERSION).orig

#PATCH_FILE1 = iputils_20020927-6.diff.gz

CONFIGURE = yes
INSTALL = yes

configure: patch
	@true;

install: compile
	@mkdir -p $(PKG_ROOT)
	@install -m 4755 -D $(DIR)/ping $(PKG_ROOT)$(PKG_DIR)/bin/ping

include ../Makefile.rules
