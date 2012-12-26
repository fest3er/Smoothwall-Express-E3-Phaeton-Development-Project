# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../../Makefile.conf

PACKAGE = HTML-Tagset
VERSION = 3.20

BASE_URL = http://search.cpan.org/CPAN/authors/id/P/PE/PETDANCE/

CONFIGURE = yes
CLEAN = yes

configure: $(DIR)/
	@+cd $(DIR); perl Makefile.PL -n

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(DIR)
	@rm -rf $(COMPILE_DIR)
	@rm -rf $(PKG_ROOT); mkdir $(PKG_ROOT)

include ../../../Makefile.rules
