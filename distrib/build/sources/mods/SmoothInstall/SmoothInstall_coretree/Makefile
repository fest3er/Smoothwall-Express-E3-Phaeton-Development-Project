# $Id: Makefile 336 2009-07-26 15:13:41Z steve@domesticsecurity.com.au $

include ../../../Makefile.conf
include ../DETAILS

PACKAGE = $(MOD_NAME)_coretree
VERSION = 0.0

distrib:
	@(make -C /build/sources/smoothd; cp -p /build/sources/smoothd/smoothd-0.0/smoothd tree/usr/sbin; md5sum tree/usr/sbin/smoothd >tree/var/smoothwall/mods/SmoothInstall/smoothd.md5);
	@(make -C /build/sources/cron; cp -p /build/sources/cron/cron-3.0pl1.orig/cron tree/usr/sbin; md5sum tree/usr/sbin/cron >tree/var/smoothwall/mods/SmoothInstall/cron.md5);
	@sed -i 's/\(VERSION.*= "\)\(.*\)\(";\)/\1$(MOD_VERSION)\3/' tree/usr/lib/smoothwall/SmoothInstall.pm
	@sed -i 's/tree//' tree/var/smoothwall/mods/SmoothInstall/smoothd.md5
	@sed -i 's/tree//' tree/var/smoothwall/mods/SmoothInstall/cron.md5
	@mkdir distrib;
	@cp -avR tree/* distrib
	@(cd distrib; rm -rf `find -name .svn`)

all: distrib
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(MOD_NAME)-$(PACKAGE).tar.gz -C distrib .

clean:
	@rm -rf distrib
	@rm -f $(TARGET_DIR)/smoothwall-$(MOD_NAME)-$(PACKAGE).tar.gz
	@rm -f tree/var/smoothwall/mods/SmoothInstall/*.md5


download:
	@true


