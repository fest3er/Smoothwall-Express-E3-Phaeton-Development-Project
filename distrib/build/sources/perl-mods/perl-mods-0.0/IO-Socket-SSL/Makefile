# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../../Makefile.conf

PACKAGE = IO-Socket-SSL
VERSION = 1.76

BASE_URL = http://search.cpan.org/CPAN/authors/id/S/SU/SULLR/

CONFIGURE = yes
CLEAN = yes

configure: $(DIR)/
	@+cd $(DIR); echo 'y' | perl Makefile.PL

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(DIR)
	@rm -rf $(COMPILE_DIR)

include ../../../Makefile.rules
