include ../Makefile.conf
include ../Makefile.versions

PACKAGE = freefont
VERSION = 0.0

install:
	@mkdir -p $(PKG_ROOT)
	( \
	  cd $(PACKAGE)-$(VERSION); \
	  install -dv $(PKG_ROOT)/usr/share/fonts/truetype/freefont; \
	  install -v * $(PKG_ROOT)/usr/share/fonts/truetype/freefont; \
	 )
	( \
	  cd $(PKG_ROOT)/usr/share; \
	  find fonts -type f -exec chown root:root {} \; -exec chmod 644 {} \; ; \
	  find fonts -type d -exec chown root:root {} \; -exec chmod 755 {} \; ; \
	 )

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

download:
	@true
