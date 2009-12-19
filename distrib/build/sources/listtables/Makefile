# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = listtables
VERSION = 0.0

compile:
	@make -C $(COMPILE_DIR)

install: compile
	@install -o root -g nobody -m 4750 -D $(COMPILE_DIR)/listtables $(PKG_ROOT)/usr/sbin/listtables

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .

clean:
	@echo -n " $(PACKAGE)"
	@make --no-print-directory -C $(COMPILE_DIR) clean
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

download:
	@true
