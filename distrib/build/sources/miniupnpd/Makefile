# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = miniupnpd
VERSION = 1.4.20100511

BASE_URL = http://miniupnp.free.fr/files/

PATCH_FILE1 = umask.patch
PATCH_FILE2 = genconfig.patch

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: patch
	@true

compile: configure
	@make -C $(DIR) -f Makefile.linux \
	  CFLAGS="-DIPTABLES_143 -DUSE_NETFILTER \
	    -I../../iptables/iptables-$(IPTABLES_VER)/include" \
	  LDFLAGS="-L$(IPT_LIBEXEC) -liptc"

install: compile
	@install -D $(DIR)/miniupnpd $(PKG_ROOT)$(PKG_DIR)/sbin/miniupnpd

include ../Makefile.rules
