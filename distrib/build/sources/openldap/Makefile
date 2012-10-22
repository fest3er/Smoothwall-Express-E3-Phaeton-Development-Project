# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = openldap
VERSION = 2.4.32

EXTENSION = .tgz

BASE_URL = ftp://ftp.openldap.org/pub/OpenLDAP/openldap-release/

CONFIG_OPTS = --with-tls=openssl

#PATCH_FILE1 = configure.patch

#CONFIGURE = yes

#configure:
#	@( \
#	  cd $(DIR); \
#	  sed -i -e 's@openssl/@/usr/ssl/openssl/include/@g; \
#	  ./configure $(CONFIG_OPTS); \
#	)

include ../Makefile.rules
