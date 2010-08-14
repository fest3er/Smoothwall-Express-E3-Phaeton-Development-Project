include ../Makefile.conf
include ../Makefile.versions

export GLOBALDEBUG = ON
export CFLAGS = -g -I /usr/include

PACKAGE = smoothd
VERSION = 0.0

compile:
	@(IPTABLES_VER=$(IPTABLES_VER) make -C $(COMPILE_DIR))

install: compile
	@install -D $(COMPILE_DIR)/libsmoothd.so $(PKG_ROOT)/usr/lib/libsmoothd.so
	@install -D $(COMPILE_DIR)/smoothd $(PKG_ROOT)/usr/sbin/smoothd 
	@install -D $(COMPILE_DIR)/ipbatchtest $(PKG_ROOT)/usr/sbin/ipbatchtest
	@install -D $(COMPILE_DIR)/default $(PKG_ROOT)/usr/lib/smoothd/sockets/default 

	@install -D $(COMPILE_DIR)/module.h $(PKG_ROOT)/usr/include/smoothwall/module.h
	@install -D $(COMPILE_DIR)/setuid.h $(PKG_ROOT)/usr/include/smoothwall/setuid.h
	@install -D $(COMPILE_DIR)/config.h $(PKG_ROOT)/usr/include/smoothwall/config.h
	@install -D $(COMPILE_DIR)/smoothcom $(PKG_ROOT)/usr/bin/smoothcom 
	@install -D $(COMPILE_DIR)/smoothd.pm $(PKG_ROOT)/usr/lib/smoothwall/smoothd.pm

	@install -D $(COMPILE_DIR)/sysshutdown.so $(PKG_ROOT)/usr/lib/smoothd/sysshutdown.so 
	@install -D $(COMPILE_DIR)/sysinstall.so $(PKG_ROOT)/usr/lib/smoothd/sysinstall.so 
	@install -D $(COMPILE_DIR)/sysiptables.so $(PKG_ROOT)/usr/lib/smoothd/sysiptables.so 
	@install -D $(COMPILE_DIR)/sysim.so $(PKG_ROOT)/usr/lib/smoothd/sysim.so 
	@install -D $(COMPILE_DIR)/sysp3scan.so $(PKG_ROOT)/usr/lib/smoothd/sysp3scan.so 
	@install -D $(COMPILE_DIR)/sysalcatel.so $(PKG_ROOT)/usr/lib/smoothd/sysalcatel.so 
	@install -D $(COMPILE_DIR)/sysdhcpd.so $(PKG_ROOT)/usr/lib/smoothd/sysdhcpd.so 
	@install -D $(COMPILE_DIR)/sysdnsproxy.so $(PKG_ROOT)/usr/lib/smoothd/sysdnsproxy.so 
	@install -D $(COMPILE_DIR)/sysntpd.so $(PKG_ROOT)/usr/lib/smoothd/sysntpd.so 
	@install -D $(COMPILE_DIR)/syssnort.so $(PKG_ROOT)/usr/lib/smoothd/syssnort.so 
	@install -D $(COMPILE_DIR)/syssquid.so $(PKG_ROOT)/usr/lib/smoothd/syssquid.so 
	@install -D $(COMPILE_DIR)/syssshd.so $(PKG_ROOT)/usr/lib/smoothd/syssshd.so 
	@install -D $(COMPILE_DIR)/sysupnpd.so $(PKG_ROOT)/usr/lib/smoothd/sysupnpd.so 
	@install -D $(COMPILE_DIR)/sysadvnet.so $(PKG_ROOT)/usr/lib/smoothd/sysadvnet.so 
	@install -D $(COMPILE_DIR)/sysipblock.so $(PKG_ROOT)/usr/lib/smoothd/sysipblock.so 
	@install -D $(COMPILE_DIR)/sysfullfirewall.so $(PKG_ROOT)/usr/lib/smoothd/sysfullfirewall.so 
	@install -D $(COMPILE_DIR)/syssmoothinfo.so $(PKG_ROOT)/usr/lib/smoothd/syssmoothinfo.so 
	@install -D $(COMPILE_DIR)/sysupdown.so $(PKG_ROOT)/usr/lib/smoothd/sysupdown.so 
	@install -D $(COMPILE_DIR)/sysipsec.so $(PKG_ROOT)/usr/lib/smoothd/sysipsec.so 
	@install -D $(COMPILE_DIR)/systimeset.so $(PKG_ROOT)/usr/lib/smoothd/systimeset.so 
	@install -D $(COMPILE_DIR)/sysiptables.so $(PKG_ROOT)/usr/lib/smoothd/sysiptables.so 
	@install -D $(COMPILE_DIR)/systraffic.so $(PKG_ROOT)/usr/lib/smoothd/systraffic.so 
	@install -D $(COMPILE_DIR)/syssip.so $(PKG_ROOT)/usr/lib/smoothd/syssip.so 
	@install -D $(COMPILE_DIR)/sysclamav.so $(PKG_ROOT)/usr/lib/smoothd/sysclamav.so 
	@install -D $(COMPILE_DIR)/systimedaccess.so $(PKG_ROOT)/usr/lib/smoothd/systimedaccess.so 

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .
	@tar -zxvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C /

clean:
	@echo -n " $(PACKAGE)"
	@make --no-print-directory -C $(COMPILE_DIR) clean >/dev/null 2>&1
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT); mkdir -p $(PKG_ROOT)
endif
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

download:
	@true
