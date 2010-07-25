include ../../../Makefile.conf
include ../DETAILS

PACKAGE = $(MOD_NAME)_smoothd
VERSION = 0.0


compile:
	@make -C $(COMPILE_DIR)

install: compile
	@install -D $(COMPILE_DIR)/sys$(MOD_NAME).so $(PKG_ROOT)/$(MOD_DIR)/$(PKG_DIR)/lib/smoothd/sys$(MOD_NAME).so 

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(MOD_NAME)-$(PACKAGE).tar.gz -C $(PKG_ROOT) .

clean:
	@make -C $(COMPILE_DIR) clean
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

#include ../../Makefile.package
