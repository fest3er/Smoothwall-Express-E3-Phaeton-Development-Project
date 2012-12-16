# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = GeoIP
VERSION = 1.4.8

BASE_URL = http://www.maxmind.com/download/geoip/api/c

CONFIGURE = yes
configure: patch
	cd $(COMPILE_DIR); \
	  LT_VER=`/usr/bin/libtool --config |grep macro_version=|sed -e 's/.*=//'`; \
	  LT_REV=`/usr/bin/libtool --config |grep macro_revision=|sed -e 's/.*=//'`; \
	  sed -i -e 's/^VERSION=.*/VERSION="'$$LT_VER'"/' \
	         -e 's/^package_revision=.*/package_revision='$$LT_REV'/' ltmain.sh
	cd $(COMPILE_DIR); $(PRE_CONFIGURE) ./configure --prefix=$(PKG_DIR) $(CONFIG_OPTS)

include ../Makefile.rules
