include ../../../Makefile.conf

PACKAGE = snmpcoretree
VERSION = 0.0

distrib:
	@mkdir distrib;
	@cp -avR tree/* distrib

	@chown -R nobody:nobody distrib/var/smoothwall

	@(cd distrib/usr/share/snmp; ln -s ../../../var/smoothwall/snmp/snmpd.conf)

	@(cd distrib; rm -rf `find -name .svn`)

all: distrib
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C distrib .

clean:
	@rm -rf distrib
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

download:
	@true
