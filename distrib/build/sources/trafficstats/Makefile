# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions # for IPTABLES version to use


PACKAGE = trafficstats
VERSION = 0.0

MODSWROOT = /var/smoothwall/traffic

compile:
	@make -C $(COMPILE_DIR) CFLAGS="$(CFLAGS)" MODSWROOT=$(MODSWROOT) KERNEL_VER=$(KERNEL_VER) \
	  IPTABLES_VERSION=$(IPTABLES_VER) LIB_IPT_DIR=$(IPT_LIBEXEC)

install: compile
	@install -D $(COMPILE_DIR)/trafficlogger $(PKG_ROOT)/$(PKG_DIR)/sbin/trafficlogger
	@install -D $(COMPILE_DIR)/trafficmon $(PKG_ROOT)/$(PKG_DIR)/sbin/trafficmon

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .
	@tar -zxvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C /

clean:
	@echo -n " $(PACKAGE)"
	@make --no-print-directory -C $(COMPILE_DIR) CFLAGS="$(CFLAGS)" clean
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT); mkdir -p $(PKG_ROOT)
endif
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

packageinfo.html: clean
	@echo "<li><span style='font-size:large;'>$(PACKAGE) $(VERSION)</span><br>" >>/tmp/packageinfo.html
	@tar -zcvf /tmp/downloads/$(TARBALL)-$(VERSION).tar.gz $(DIR)
	@echo "Download: <a href='downloads/$(TARBALL)$(EXTENSION)'>$(TARBALL)$(EXTENSION)</a><br>" >>/tmp/packageinfo.html
