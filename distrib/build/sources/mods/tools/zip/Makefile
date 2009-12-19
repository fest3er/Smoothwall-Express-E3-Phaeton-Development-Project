include ../../../Makefile.conf

PACKAGE = zip
VERSION = 30

BASE_URL = http://kent.dl.sourceforge.net/sourceforge/infozip

TARBALL = $(PACKAGE)$(VERSION)

DIR = $(PACKAGE)$(VERSION)

EXTENSION = .tar.gz

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: patch
	@true

compile: configure
	@make -C $(DIR) -f unix/Makefile CC="gcc $(CFLAGS)" generic

install: compile
	@make -C $(DIR) -f unix/Makefile prefix=$(PKG_ROOT)/$(PKG_DIR) install

include ../../../Makefile.rules
