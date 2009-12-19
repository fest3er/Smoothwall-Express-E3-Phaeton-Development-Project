# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = ibod
VERSION = 0.0

compile:
	@make -C $(COMPILE_DIR)

install: compile
	@install -D $(COMPILE_DIR)/ibod $(PKG_ROOT)/$(PKG_DIR)/sbin/ibod

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .

clean:
	@echo -n " $(PACKAGE)"
	@rm -f $(COMPILE_DIR)/ibod
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz
