include ../../../Makefile.conf
include ../DETAILS

PACKAGE = e1000
VERSION = 8.0.9

LICENCE_FILE = $(DIR)/COPYING

BASE_URL = http://downloadmirror.intel.com/9180/eng


PREPARE = yes
CONFIGURE = yes
COMPILE = yes
INSTALL = yes


prepare: download
	@tar -zxvf $(DOWNLOADS_DIR)/$(PACKAGE)-$(VERSION)$(EXTENSION)


configure: prepare
	@true

compile: configure
	@true

install: compile
	-mkdir $(PKG_ROOT)
	(sed -i 's:gzip -c $$< > $$@:true:' $(DIR)/src/Makefile; sed -i 's:\tman:\t# man:' $(DIR)/src/Makefile; sed -i 's:install -D -m 644 $$(MANFILE):# install -D -m 644 $$(MANFILE):' $(DIR)/src/Makefile;)
	@make -C $(DIR)/src 
	@make -C $(DIR)/src INSTALL_MOD_PATH=$(MOD_DIR) DESTDIR=$(PKG_ROOT) install

include ../../Makefile.package
include ../../../Makefile.rules
