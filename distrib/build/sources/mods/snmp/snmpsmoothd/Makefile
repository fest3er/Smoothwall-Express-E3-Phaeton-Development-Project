include ../../../Makefile.conf

PACKAGE = snmpsmoothd
VERSION = 0.0

compile:
	@make -C $(COMPILE_DIR)

install: compile
	@install -D $(COMPILE_DIR)/syssnmp.so $(PKG_ROOT)/$(PKG_DIR)/lib/smoothd/syssnmp.so 

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .

clean:
	@make -C $(COMPILE_DIR) clean
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz
