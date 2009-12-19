include ../../../Makefile.conf

PACKAGE = bridge-utils
VERSION = 1.2

BASE_URL = http://kent.dl.sourceforge.net/sourceforge/bridge/

PREPARE = yes

$(DIR)/: download
	@tar -zxvf $(DOWNLOADS_DIR)/$(PACKAGE)-$(VERSION)$(EXTENSION)
	@(cd $(DIR); autoconf)

include ../../../Makefile.rules
