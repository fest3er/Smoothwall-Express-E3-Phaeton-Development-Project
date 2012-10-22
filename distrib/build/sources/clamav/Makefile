include ../Makefile.conf

PACKAGE = clamav
VERSION = 0.97.5

#BASE_URL = http://softlayer.dl.sourceforge.net/project/$(PACKAGE)/$(PACKAGE)/$(VERSION)
BASE_URL = http://superb-sea2.dl.sourceforge.net/project/$(PACKAGE)/$(PACKAGE)/$(VERSION)


CONFIG_OPTS = --sysconfdir=/usr/lib/smoothwall --with-dbdir=/var/clamav --disable-clamav --enable-ltdl-install

PATCH_FILE1 = clamav-pidperms.patch

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)
	@mkdir -p $(PKG_ROOT)/var/clamav
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@chown -R 105:105 $(PKG_ROOT)/var/clamav
	@rm -f $(PKG_ROOT)/usr/lib/smoothwall/clamd.conf
	@rm -f $(PKG_ROOT)/usr/lib/smoothwall/freshclam.conf

include ../Makefile.rules
