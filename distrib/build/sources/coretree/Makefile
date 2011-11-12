# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = coretree
VERSION = 0.0

distrib:
	@mkdir distrib;
	@cp -avR tree/* distrib

	@chown -R root:root distrib/
	@chmod -v 700 distrib/root
	@chmod -v 1777 distrib/tmp

	@chown -R nobody:nobody distrib/var/smoothwall
	@chown -R nobody:nobody distrib/var/patches
	@chown -R 100:100 distrib/var/spool/squid
	@chown -R 100:100 distrib/var/log/squid
	@chown -R 101:101 distrib/var/log/snort
	@chown -R 104:104 distrib/var/log/imspector
	
	@chown -R 106:106 distrib/var/lib/siproxd/registrations
	@chown -R 104:104 distrib/var/lib/imspector
	
	@(cd distrib/etc/httpd/conf; ln -s /var/smoothwall/main/hostname.conf)
	@(cd distrib/usr/etc ln -s /var/smoothwall/dhcp/dhcpd.conf)
	@(cd distrib/usr/etc; ln -s /var/smoothwall/vpn/ipsec.conf)
	@(cd distrib/usr/etc; ln -s /var/smoothwall/vpn/ipsec.secrets)
	@(cd distrib/etc; ln -s /var/smoothwall/time/localtime)
	@(cd distrib/etc; ln -s /var/smoothwall/vpn/ibod.cf)
	@(cd distrib/etc; ln -s /var/smoothwall/hosts/hosts)
	@(cd distrib/usr/etc; ln -s /var/smoothwall/dhcp/dhcpd.conf)
	@(cd distrib/etc/ppp; ln -s /var/smoothwall/ppp/secrets chap-secrets)
	@(cd distrib/etc/ppp; ln -s /var/smoothwall/ppp/secrets pap-secrets)
	@(cd distrib/etc/ppp; ln -s /var/smoothwall/ppp/ibod.cf)
	@(cd distrib/usr/etc; ln -s /var/smoothwall/time/ntpd.conf)
	@(cd distrib/usr/etc; ln -s /var/smoothwall/sipproxy/siproxd.conf)
	@(cd distrib/var/run; ln -s /var/clamav/clamd.pid)
	
	@(cd distrib/var/smoothwall/time; ln -s /usr/share/zoneinfo/posix/Europe/London localtime)

	@(cd distrib; echo "CURRENT=$(KERNEL_VER)" > var/smoothwall/main/kernel)

	@(cd distrib; rm -rf `find -name CVS`)
	@(cd distrib; rm -rf `find -name .svn`)

	@(cd distrib/var/smoothwall/patches; perl -i -pe "s/ARCH/$(ARCH)/g;" installed)

all: distrib
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C distrib .

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf distrib
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

download:
	@true
