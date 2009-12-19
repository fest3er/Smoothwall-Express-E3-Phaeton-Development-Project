include ../Makefile.conf

PACKAGE = noip
VERSION = 1.6.0

BASE_URL = http://www.ramereth.net/openwrt/src/distfiles/

EXTENSION = .tbz2

INSTALL = yes
CONFIGURE = yes

$(DIR): download
	@tar xfvj $(DOWNLOADS_DIR)/$(TARBALL)$(EXTENSION)

configure: $(DIR)
	@true

install: compile
	@install -D $(COMPILE_DIR)/noip $(PKG_ROOT)/$(PKG_DIR)/bin/noip

include ../Makefile.rules

