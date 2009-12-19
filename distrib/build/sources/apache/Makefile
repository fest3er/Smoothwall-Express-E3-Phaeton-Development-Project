# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

APACHE_VER = 1.3.41
MODSSL_VER = 2.8.31

PACKAGE = apache
VERSION = $(APACHE_VER)

DIR = $(PACKAGE)_$(VERSION)

DOWNLOAD = yes
PREPARE = yes
CONFIGURE = yes
INSTALL = yes
BUILDTARBALL = yes
CLEAN = yes
PACKAGEINFO = yes

download:
	@$(DL_CMD) http://www.mirrorservice.org/sites/ftp.apache.org/httpd/$(DIR).tar.gz
	@$(DL_CMD) http://www.modssl.org/source/mod_ssl-$(MODSSL_VER)-$(APACHE_VER).tar.gz

prepare: download
	@tar -zxvf $(DOWNLOADS_DIR)/apache_$(APACHE_VER).tar.gz
	@tar -zxvf $(DOWNLOADS_DIR)/mod_ssl-$(MODSSL_VER)-$(APACHE_VER).tar.gz

configure: prepare
	@(cd mod_ssl-$(MODSSL_VER)-$(APACHE_VER); \
	./configure --with-apache=../$(DIR)/ \
		--with-ssl=/usr/ssl --with-layout=RedHat);

install: compile
	@(cd apache_$(APACHE_VER); \
	install -s -D src/httpd $(PKG_ROOT)/usr/sbin/httpd; \
	install -s -D src/support/htpasswd $(PKG_ROOT)/usr/bin/htpasswd; \
	install -s -D src/support/htdigest $(PKG_ROOT)/usr/bin/htdigest);

$(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz: install
	@(cd $(PKG_ROOT); \
	tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz \
	usr/sbin/httpd usr/bin/htpasswd usr/bin/htdigest);

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf apache_$(APACHE_VER)
	@rm -rf mod_ssl-$(MODSSL_VER)-$(APACHE_VER)

packageinfo.html:
	@echo "<li><span style='font-size:large;'>$(PACKAGE) $(VERSION)</span><br>" >>/tmp/packageinfo.html
	@cp -avR $(DOWNLOADS_DIR)/$(DIR).tar.gz /tmp/downloads
	@cp -avR $(DOWNLOADS_DIR)/mod_ssl-$(MODSSL_VER)-$(APACHE_VER).tar.gz /tmp/downloads
	@echo "Apache Download: <a href='http://www.mirror.ac.uk/mirror/ftp.apache.org/httpd/$(DIR).tar.gz'>http://www.mirror.ac.uk/mirror/ftp.apache.org/httpd/$(DIR).tar.gz</a>" >>/tmp/packageinfo.html
	@echo "(<a href='downloads/$(DIR).tar.gz'>Local mirror</a>)<br>" >>/tmp/packageinfo.html
	@echo "Mod SSL Download: <a href='http://www.modssl.org/source/mod_ssl-$(MODSSL_VER)-$(APACHE_VER).tar.gz'>http://www.modssl.org/source/mod_ssl-$(MODSSL_VER)-$(APACHE_VER).tar.gz</a>" >>/tmp/packageinfo.html
	@echo "(<a href='downloads/mod_ssl-$(MODSSL_VER)-$(APACHE_VER).tar.gz'>Local mirror</a>)<br>" >>/tmp/packageinfo.html
	@echo "Patches:" >>/tmp/packageinfo.html
	@echo "<ul>" >>/tmp/packageinfo.html
	@(for PATCH in $(PATCH_FILE1) $(PATCH_FILE2) $(PATCH_FILE3) $(PATCH_FILE4) $(PATCH_FILE5); do \
		echo "<li><a href='downloads/$$PATCH'>$$PATCH</a>" >>/tmp/packageinfo.html; \
		cp -avR $$PATCH /tmp/downloads; \
	done) 
	@echo "</ul>" >>/tmp/packageinfo.html

include ../Makefile.rules
