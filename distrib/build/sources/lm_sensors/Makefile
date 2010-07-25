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
	make -C $(DIR) PREFIX=$(PKG_DIR) all

install: compile
	make -C $(DIR) PREFIX=$(PKG_DIR) DESTDIR=$(PKG_ROOT) install

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(DIR)
	@rm -rf $(COMPILE_DIR)
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(BUILDDIR)/buildroot/$(PACKAGE)
endif

include ../Makefile.rules
