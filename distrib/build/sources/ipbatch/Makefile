include ../Makefile.conf
include ../Makefile.versions

PACKAGE = ipbatch
VERSION = 0.0

compile:
	@make -C $(COMPILE_DIR) IPTABLES_VER=$(IPTABLES_VER) IPT_LIB_DIR=/usr/lib/iptables

install: compile
	@install -D $(COMPILE_DIR)/ipbatch $(PKG_ROOT)/usr/sbin/ipbatch

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .

clean:
	echo -n " $(PACKAGE)"
	rm -rf $(PKG_ROOT)
	rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

download:
	@true
