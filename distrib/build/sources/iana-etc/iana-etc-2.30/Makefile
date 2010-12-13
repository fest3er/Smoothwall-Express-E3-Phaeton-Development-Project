# Copyright (c) 2003-2006 Seth W. Klein <sk@sethwklein.net>
# Licensed under the Open Software License version 3.0
# See the file COPYING in the distribution tarball or
# http://www.opensource.org/licenses/osl-3.0.txt

# Makefile: See README for usage

DESTDIR=
PREFIX=
ETC_DIR=/etc

AWK=gawk
STRIP=no
# STRIP=yes

# sed -n 's/^\([^#:]*\):.*/\1/p' < Makefile | grep -v '^\(.PHONY\|.*-numbers.*\|protocols\|services\)$' | tr '\n' ' ' | sed 's/ $/\n/' | xclip
.PHONY: all files get test test-services test-protocols install clean \
	protocol-numbers.iana port-numbers.iana dist

all: files
files: protocols services

get: protocol-numbers.iana port-numbers.iana

test: test-protocols test-services

test-services: services test-lib.gawk test-services.gawk
	$(AWK) --re-interval -f test-lib.gawk -f test-services.gawk <services

test-protocols: protocols test-lib.gawk test-protocols.gawk
	$(AWK) -f test-lib.gawk -f test-protocols.gawk <protocols

install: files
	install -d $(DESTDIR)$(PREFIX)$(ETC_DIR)
	install -m 644 protocols $(DESTDIR)$(PREFIX)$(ETC_DIR)
	install -m 644 services $(DESTDIR)$(PREFIX)$(ETC_DIR)

clean:
	rm -vf \
	    protocols services \
	    protocol-numbers port-numbers \
	    protocol-numbers.iana port-numbers.iana

protocol-numbers.iana:
	$(AWK) -f get.gawk -v file=protocol-numbers >protocol-numbers.iana
	rm -f protocol-numbers

port-numbers.iana:
	$(AWK) -f get.gawk -v file=port-numbers >port-numbers.iana
	rm -f port-numbers

protocol-numbers:
ifeq (protocol-numbers.iana, $(wildcard protocol-numbers.iana))
	ln -f -s protocol-numbers.iana protocol-numbers
else
	ln -f -s protocol-numbers.dist protocol-numbers
endif

port-numbers:
ifeq (port-numbers.iana, $(wildcard port-numbers.iana))
	ln -f -s port-numbers.iana port-numbers
else
	ln -f -s port-numbers.dist port-numbers
endif

protocols: protocol-numbers protocols.gawk
	$(AWK) --re-interval -f protocols.gawk -v strip=$(STRIP) \
	    protocol-numbers > protocols

services: port-numbers services.gawk
	$(AWK) -f services.gawk -v strip=$(STRIP) port-numbers > services

dist: clean
	rm -vrf ../iana-etc-`cat VERSION`
	cp -a . ../iana-etc-`cat VERSION`
	tar --owner=root --group=root  -vjf ../iana-etc-`cat VERSION`.tar.bz2 \
	    -C .. -c iana-etc-`cat VERSION`

