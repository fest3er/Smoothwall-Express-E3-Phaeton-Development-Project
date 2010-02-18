# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = libsmooth+setup
VERSION = 0.0

compile:
	@make -C $(COMPILE_DIR)

install: compile
	@install -D $(COMPILE_DIR)/setup/setup $(PKG_ROOT)/usr/sbin/setup
	@install -D $(COMPILE_DIR)/libsmooth/libsmooth.so $(PKG_ROOT)/usr/lib/libsmooth.so
	@install -D $(COMPILE_DIR)/libsmooth/libsmooth.h /usr/include/libsmooth.h
	@install -D $(COMPILE_DIR)/libsmooth/langs.h /usr/include/langs.h
	@install -D $(COMPILE_DIR)/libsmooth/libsmooth.o /usr/lib/libsmooth.o

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .

clean:
	@echo -n " $(PACKAGE)"
	@make --no-print-directory -C $(COMPILE_DIR) clean
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT); mkdir -p $(PKG_ROOT)
endif
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

download:
	@true
