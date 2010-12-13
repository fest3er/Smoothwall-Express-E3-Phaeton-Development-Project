include ../Makefile.conf

PACKAGE = noip
#VERSION = 1.6.0
VERSION = 2.1.9-1

#BASE_URL = http://www.ramereth.net/openwrt/src/distfiles/
BASE_URL = http://www.no-ip.com/client/linux/

EXTENSION = .tar.gz

INSTALL = yes
CONFIGURE = yes

$(DIR): download
	@tar xfvj $(DOWNLOADS_DIR)/$(TARBALL)$(EXTENSION)

configure: $(DIR)
	@true

install: compile
	@mkdir -p $(PKG_ROOT)
	@install -D $(COMPILE_DIR)/binaries/noip2-i686 $(PKG_ROOT)$(PKG_DIR)/bin/noip

include ../Makefile.rules

