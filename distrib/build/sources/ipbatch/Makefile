include ../Makefile.conf
include ../Makefile.versions

PACKAGE = ipbatch
VERSION = 0.0

compile:
	@make -C $(COMPILE_DIR) IPTABLES_VER=$(IPTABLES_VER) IPT_LIB_DIR=$(IPT_LIBEXEC)

install: compile
	@mkdir -p $(PKG_ROOT)
	@install -D $(COMPILE_DIR)/ipbatch $(PKG_ROOT)/usr/sbin/ipbatch

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .

clean:
	@echo -n " $(PACKAGE)"
	@make --no-print-directory -C $(COMPILE_DIR) clean >/dev/null 2>&1
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz
endif

download:
	@true
