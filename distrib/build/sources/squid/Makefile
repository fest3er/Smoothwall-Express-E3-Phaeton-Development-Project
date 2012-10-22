# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = squid
VERSION = 3.1.20

BASE_URL = http://www.squid-cache.org/Versions/v3/3.1/

#PATCH_FILE1 = ERR_ACCESS_DENIED.patch
#PATCH_FILE2 = ERR_CACHE_ACCESS_DENIED.patch 
#PATCH_FILE3 = ERR_TOO_BIG.patch

CONFIG_OPTS = --enable-storeio="diskd,ufs" --enable-linux-netfilter \
	      --enable-removal-policies="heap,lru" --enable-delay-pools \
	      --localstatedir=/var --with-dl --with-openssl \
	      --with-large-files --with-libcap

INSTALL_LANGUAGES = English

BUILDTARBALL = yes

SQUID_BITS = ./usr/bin/ ./usr/libexec/ ./usr/sbin/squid \
	     ./usr/etc/mime.conf ./usr/share/

$(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) $(SQUID_BITS)
	@tar -xvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C /

CONFIGURE = yes

configure: $(DIR)/
	@/bin/sh -c "ulimit -n 8192; cd $(DIR); ./configure $(CONFIG_OPTS) --prefix=/$(PKG_DIR)"

include ../Makefile.rules
