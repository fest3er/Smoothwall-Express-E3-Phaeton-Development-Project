include ../Makefile.conf

PACKAGE = p3scan
VERSION = 2.3.2

BASE_URL = http://heanet.dl.sourceforge.net/sourceforge/p3scan/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

PATCH_FILE1 = p3scan.patch

configure: patch
        @true

compile: configure
        @(cd $(DIR); make $(JOBS) p3scan CFLAGS="$(CFLAGS) -I/usr/ssl/include" LDFLAGS="$(LDFLAGS) -L/usr/ssl/lib -L. -lripmime -lpcre -lssl -lcrypto")

install: compile
        mkdir -p $(PKG_ROOT)/var/avirus/p3scan/children
	mkdir -p $(PKG_ROOT)/var/avirus/p3scan/notify
	chown -R 105:105 $(PKG_ROOT)/var/avirus/
	mkdir -p $(PKG_ROOT)/usr/sbin/
	cp $(DIR)/p3scan $(PKG_ROOT)/usr/sbin

include ../Makefile.rules
