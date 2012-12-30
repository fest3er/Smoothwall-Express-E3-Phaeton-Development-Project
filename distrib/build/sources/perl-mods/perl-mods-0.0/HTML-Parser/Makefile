# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../../Makefile.conf

PACKAGE = HTML-Parser
VERSION = 3.69

BASE_URL = http://search.cpan.org/CPAN/authors/id/G/GA/GAAS/

CONFIGURE = yes
CLEAN = yes

configure: $(DIR)/
	@+cd $(DIR); echo 'y' | perl Makefile.PL

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(DIR)
	@rm -rf $(COMPILE_DIR)

include ../../../Makefile.rules
