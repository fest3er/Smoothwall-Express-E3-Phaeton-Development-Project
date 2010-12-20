include ../Makefile.conf

PACKAGE = cgilib
VERSION = 0.5

BASE_URL = http://www.infodrom.org/projects/cgilib/download/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@true

compile: configure
	@cd $(DIR) && make $(JOBS)

install: compile
	@mkdir -p $(PKG_ROOT)
	@install -D $(COMPILE_DIR)/libcgi.a $(PKG_ROOT)/usr/lib/libcgi.a
	@install -D $(COMPILE_DIR)/cgi.h $(PKG_ROOT)/usr/include/cgi.h

include ../Makefile.rules
