# Smoothwall Build System
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.
#
# Written by: Charlie Heselton
#
# Makefile for all needed perl modules in P/R/AGCL
#

include ../Makefile.conf

PACKAGE = perl-mods
VERSION = 0.0

# List of all mods to be built. Add here, then add dir and makefile in
#   version dir below.
export PERL_MODS = Authen-NTLM \
	    Authen-SASL \
	    Config-Patch \
	    Date-Calc \
	    GD \
	    HTML-Parser \
	    HTML-Tagset \
	    IO-Socket-SSL \
	    Log-Log4perl \
	    Net-CIDR \
	    Net-Netmask \
	    Net-SSLeay \
	    NetAddr-IP \
	    RRD-Simple \
	    Set-IntSpan \
	    URI \
	    libnet \
	    libwww-perl

DOWNLOAD = yes
download:
	@+$(MAKE) -C $(PACKAGE)-$(VERSION) download

COMPILE = yes
compile: 
	@+$(MAKE) $(JOBS) -C $(COMPILE_DIR)

INSTALL = yes
install: compile
	@true

CLEAN = yes
clean:
	@echo -n " $(PACKAGE)"
	@+$(MAKE) --no-print-directory -C $(COMPILE_DIR) clean >/dev/null 2>&1
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

include ../Makefile.rules
