# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../../Makefile.conf

PACKAGE = Net-Netmask
VERSION = 1.9016

BASE_URL = http://search.cpan.org/CPAN/authors/id/M/MU/MUIR/modules/

CONFIGURE = yes
CLEAN = yes

configure: $(DIR)/
	@+cd $(DIR); echo 'y' | perl Makefile.PL

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(DIR)
	@rm -rf $(COMPILE_DIR)
	@rm -rf $(PKG_ROOT); mkdir $(PKG_ROOT)

include ../../../Makefile.rules
