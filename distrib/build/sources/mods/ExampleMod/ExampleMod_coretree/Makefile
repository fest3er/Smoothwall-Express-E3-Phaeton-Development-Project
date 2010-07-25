include ../../../Makefile.conf
include ../DETAILS

PACKAGE = $(MOD_NAME)_coretree
VERSION = 0.0

distrib:
	@mkdir distrib;
	@cp -avR tree/* distrib
	@(cd distrib; rm -rf `find -name .svn`)

all: distrib
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(MOD_NAME)-$(PACKAGE).tar.gz -C distrib .

clean:
	@rm -rf distrib
	@rm -f $(TARGET_DIR)/smoothwall-$(MOD_NAME)-$(PACKAGE).tar.gz

download:
	@true


