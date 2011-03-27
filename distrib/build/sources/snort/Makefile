# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = snort

# Deal with snort: get the latest version number dynamically. :)
#
VERSION := $(shell ./get-snort-ver.sh -v)
URL := $(shell ./get-snort-ver.sh -u)

BASE_URL = http://dl.snort.org

PATCH_FILE1 = snort-pidfix.patch

# You're looking in here because the build system failed to find
# the snort source package or it failed to build.
#
# This makefile should always get the latest version of snort, whatever it is.
# However, the build might fail if the pidfile fix patch is no longer correct.

DOWNLOAD = yes
download:
	@if [ ! -e ../../downloads/$(TARBALL)$(EXTENSION).done ]; then \
	  wget -O ../../downloads/$(TARBALL)$(EXTENSION) $(BASE_URL)$(URL); \
	  touch ../../downloads/$(TARBALL)$(EXTENSION).done; \
	fi

PREPARE = yes
$(DIR)/: download
	@tar -xvf $(DOWNLOADS_DIR)/$(TARBALL)$(EXTENSION)

INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@mkdir -p $(PKG_ROOT)/var/smoothwall/snort/rules/
	@mkdir -p $(PKG_ROOT)/var/smoothwall/snort/preproc_rules/
	@install -D $(COMPILE_DIR)/etc/unicode.map $(PKG_ROOT)/var/smoothwall/snort/unicode.map
	@install -D $(COMPILE_DIR)/etc/classification.config $(PKG_ROOT)/var/smoothwall/snort/classification.config
	@install -D $(COMPILE_DIR)/etc/reference.config $(PKG_ROOT)/var/smoothwall/snort/reference.config
	@install -D $(COMPILE_DIR)/preproc_rules/preprocessor.rules $(PKG_ROOT)/var/smoothwall/snort/preproc_rules/preprocessor.rules
	@install -D $(COMPILE_DIR)/preproc_rules/decoder.rules $(PKG_ROOT)/var/smoothwall/snort/preproc_rules/decoder.rules
	@chown nobody:nobody -R $(PKG_ROOT)/var/smoothwall/
	@mkdir -p $(PKG_ROOT)/$(MODULE_DIR)/usr/lib/smoothwall
	@echo $(VERSION) > $(PKG_ROOT)/$(MODULE_DIR)/usr/lib/smoothwall/snortversion

include ../Makefile.rules
