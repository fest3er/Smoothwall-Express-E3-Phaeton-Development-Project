# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = MAKEDEV
VERSION = 3.3.8

BASE_URL = http://www.aslak.net/smoothmirror/

TARBALL = $(PACKAGE)-$(VERSION)-2

DIR = $(PACKAGE)-$(VERSION)

CONFIGURE = yes
PACKAGEINFO = yes

configure: patch
	@true

devnodes:
	@(cd /dev; mkdir pts shm; \
	@tar -zcvf /$(TARGET_DIR)/smoothwall-devnodes.tar.gz -C / --exclude ./dev/MAKEDEV ./dev;

packageinfo.html: 
	@echo "<li><span style='font-size:large;'>$(PACKAGE) $(VERSION)</span><br>" >>/tmp/packageinfo.html
	@cp -avR $(DOWNLOADS_DIR)/$(TARBALL)$(EXTENSION) /tmp/downloads/
	@echo "Download: <a href='downloads/$(TARBALL)$(EXTENSION)'>$(TARBALL)$(EXTENSION)</a><br>" >>/tmp/packageinfo.html

include ../Makefile.rules
