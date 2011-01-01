# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = lm_sensors
VERSION = 3.1.2

EXTENSION = .tar.bz2

BASE_URL = http://dl.lm-sensors.org/lm-sensors/releases/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes
CLEAN = yes

configure: patch
        @true

compile: configure
        make $(JOBS) -C $(DIR) PREFIX=$(PKG_DIR) all

install: compile
        @mkdir -p $(PKG_ROOT)
	make -C $(DIR) PREFIX=$(PKG_DIR) DESTDIR=$(PKG_ROOT) install

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(DIR)
	@rm -rf $(COMPILE_DIR)
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz
endif

include ../Makefile.rules
