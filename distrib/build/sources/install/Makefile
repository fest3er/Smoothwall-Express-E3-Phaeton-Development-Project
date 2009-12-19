# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = install
VERSION = 0.0

compile:
	@make -C $(COMPILE_DIR)

install: compile
	@install -D $(COMPILE_DIR)/install/install $(TARGET_DIR)/$(PACKAGE)

all: install
	@true

clean:
	@echo -n " $(PACKAGE)"
	@make --no-print-directory -C $(COMPILE_DIR) clean

download:
	@true
