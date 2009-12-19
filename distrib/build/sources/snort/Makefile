# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = snort
VERSION = 2.8.4.1

BASE_URL = http://dl.snort.org/snort284

PATCH_FILE1 = snort-pidfix.patch

# You're looking in here because the build system failed to find
# the snort source package.

# Snort screws the world with their every release. They delete the old
# previous release, rename 'current' to its a.b.c number, call the new
# release 'current', and move directories around. Clearly they're
# unfamliar with the concept of automation.

# In short, the build system has found that the release in VERSION,
# above, is out-dated or has been deleted by snort.org. You will need
# to change to a more current version and add/change/delete patches
# as needed. Visit www.linuxfromscratch.org for more clues.

INSTALL = yes

install: compile
	-mkdir $(PKG_ROOT)
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
