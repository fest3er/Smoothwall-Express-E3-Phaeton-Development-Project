# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions
include ../Makefile

PACKAGE = media-isoinstall

# Two lines above coretree are Phaeton
TARGET_DIR = /build/target/
INST_PKGS = glibc newt libsmooth+setup libgcc binutils  \
	    coreutils iana-etc mktemp file findutils gawk ncurses \
	    vim less sed flex gettext net-tools inetutils iputils whois perl \
	    bash bzip2 kbd e2fsprogs grep gzip module-init-tools procinfo procps \
	    psmisc shadow sysklogd logrotate sysvinit tar util-linux-ng joe wget \
	    iptables ipset ipbatch pciutils linux-dom0 openssl openssh \
	    httpd smoothd squid HTML-Parser HTML-Tagset URI libwww-perl strace \
	    libart_lgpl freetype rrdtool cron tcpdump dnsmasq libcap ntp openntpd \
	    hdparm hddtemp iproute2 libpng gd gmp expat ethtool popt listtables  \
	    libdnet daq libnet libnetfilter_queue libnfnetlink inotify-tools \
	    readline beep2 pcre trafficstats iftop setuids ppp isdn4k-utils \
	    rp-pppoe ibod dhcpcd eciadsl-synch_bin eciadsl-usermode \
	    cnxadsl dhcp snort oinkmaster openswan wireless_tools imspector  \
	    diffutils traceroute clamav p3scan libosip2 siproxd which miniupnpd \
	    ez-ipupdate noip cdrtools suricata yaml smartmontools\
	    at dosfstools e2fsprogs grub libpcap linux-atm mtools nano \
	    reiserfsprogs parted xtables-addons zlib slang cpio \
	    udev udev-config lm_sensors xz pcmciautils \
	    Net-Netmask NetAddr-IP sysfsutils \
	    coretree

OBSOLETE_PKGS = devnodes libstdc++ 
TOOLCHAIN_PKGS = expect
OTHER_PKGS = updates jam sqlite

# Last line is Phaeton
DEV_PKGS = kernel-headers patch make autoconf automake bison dejagnu gcc \
	   gdb tcl m4 neon git screen lynx groff man man-pages \
	   klibc pkg-config sudo syslinux texinfo bin86 busybox \
	   libtool nasm

cdrom:
	mkdir cdrom
	echo "      runtimeroot" > /dev/tty
	make -C runtimeroot
	echo "      installroot" > /dev/tty
	make -C installroot
	echo "      cdromboot" > /dev/tty
	make -C cdromboot

cdrom/smoothwall.tgz:
	echo "      prepare dir tree" > /dev/tty
	@mkdir -p distrib distrib/sys distrib-dev distrib-doc/usr/share/

	@install -D runtimerootimage.gz distrib/boot/initrd-$(K_SWE_VERSION)-dom0.gz

	echo "        unpack dev packages" > /dev/tty
	@echo "Unpacking dev packages:"
	@(for I in $(DEV_PKGS); do \
	        if [ ! -e /$(TARGET_DIR)/smoothwall-$$I.tar.gz ]; then \
	        make -C ../$$I clean all; \
	                if [ $$? -ne 0 ]; then \
				echo "$$I FAILED"; \
				exit 0; \
			fi; \
		fi; \
		echo -n "    $$I"; \
		tar -zxf /$(TARGET_DIR)/smoothwall-$$I.tar.gz -C distrib-dev || (echo; exit 69); \
		echo; \
	  done; \
	);

	echo "        copy dev shared libs to distrib" > /dev/tty
	@echo "Copying dev shared libs to distrib:"
	@(cd distrib-dev; \
	        find . -name \*.so\* -print | cpio -pd ../distrib \
	)

	echo "        unpack main packages" > /dev/tty
	echo "Unpacking main packages:"
	@(for I in $(INST_PKGS); do \
	        if [ ! -e /$(TARGET_DIR)/smoothwall-$$I.tar.gz ]; then \
	        make -C ../$$I clean all; \
	                if [ $$? -ne 0 ]; then \
				echo "$$I FAILED"; \
				exit 0; \
			fi; \
		fi; \
		echo -n "    $$I"; \
		tar -zxf /$(TARGET_DIR)/smoothwall-$$I.tar.gz -C distrib || (echo; exit 68); \
	        echo; \
	done; \
	);
	echo "        move includes to distrib-dev" > /dev/tty
	@echo "Moving includes to distrib-dev:"
	@if [ -d distrib/usr/include ]; then \
	  mkdir -p distrib-dev/usr/include; \
	  cp -a distrib/usr/include distrib-dev/usr/; \
	  rm -rf distrib/usr/include/*; fi
	if [ -d distrib/usr/ssl/include ]; then \
	  mkdir -p distrib-dev/usr/ssl/include; \
	  cp -a distrib/usr/ssl/include distrib-dev/usr/ssl/; \
	  rm -rf distrib/usr/ssl/include/*; fi

	echo "        move static libs to distrib-dev" > /dev/tty
	@echo "Moving static libs to distrib-dev:"
	@cd distrib; find . -name \*.a -o -name \*.la | while read a; do \
	  mkdir -p ../distrib-dev/`dirname $$a`; \
	  mv $$a ../distrib-dev/$$a; \
	done

	echo "        move docs to distrib-doc" > /dev/tty
	@echo "Moving docs to distrib-doc:"
	@for I in man info share/man share/info share/doc examples; do \
	  if [ -d distrib/usr/$$I ]; then \
	    mkdir -p distrib-doc/usr/$$I; \
	    mv distrib/usr/$$I/* distrib-doc/usr/$$I/; fi; \
	  if [ -d distrib/usr/ssl/$$I ]; then \
	    mkdir -p distrib-doc/usr/ssl/$$I; \
	    mv distrib/usr/ssl/$$I/* distrib-doc/usr/ssl/$$I/; fi; \
	done

	echo "        strip symbols" > /dev/tty
	/build/striptree distrib
	/build/striptree distrib-dev
	/build/striptree distrib-doc

	@echo -e "\nProduct info:"
	@( \
	echo "  PRODUCT='$(PRODUCT_NAME)'"; \
	echo "  VERSION='$(PRODUCT_VERSION)'"; \
	echo "  RELEASE='$(RELEASE)'" ; \
	echo "  REVISION='$(REVISION)'"; \
	echo "  UI_VERSION='$(UI_VERSION)'"; \
	echo "  FIXES='$(FIXES_NUMBER)'"; \
	echo "  ARCH='$(ARCH)'"; \
	) | sed -u -e 's/^/  /'

	mkdir -p distrib/var/smoothwall/main
	( \
	  echo "PRODUCT='$(PRODUCT_NAME)'" ; \
	  echo "VERSION='$(PRODUCT_VERSION)'" ; \
	  echo "RELEASE='$(RELEASE)'" ; \
	  echo "REVISION='$(REVISION)'" ; \
	  echo "UI_VERSION='$(UI_VERSION)'" ; \
	  echo "FIXES='$(FIXES_NUMBER)'" ; \
	  echo "ARCH='$(ARCH)'" ; \
	) > distrib/var/smoothwall/main/productdata

	@chown nobody:nobody distrib/var/smoothwall/main/productdata

	@echo "        Secure tree ..." >/dev/tty
	@echo -e "\nSecuring tree ..."
	@( \
	  /build/securetree distrib; \
	  /build/securetree distrib-dev; \
	  /build/securetree distrib-doc; \
	  chmod u+s distrib/bin/traceroute; \
	  chmod u+s distrib/usr/bin/ping; \
	  chmod u+s distrib/usr/sbin/listtables; \
	  chown root:nobody distrib/usr/bin/setuids/*; \
	  chmod 4750 distrib/usr/bin/setuids/*; \
	  mkdir -p /distrib/tmp; chmod 1777 distrib/tmp; \
	) | sed -u -e 's/^/  /'
	
	@echo "        convert to shadow passwords ..." >/dev/tty
	@echo -e "\nConverting to shadow passwords ..."
	@( \
	/usr/bin/chroot distrib /usr/sbin/pwconv; \
	/usr/bin/chroot distrib /usr/sbin/grpconv; \
	) | sed -u -e 's/^/  /'
	
	@echo "        update library cache ..." >/dev/tty
	@echo -e "\nUpdating library cache ..."
	@( \
	/sbin/ldconfig -r distrib; \
	) | sed -u -e 's/^/  /'

	@echo "        tidy ..." >/dev/tty
	@echo -e "\nTidying ..."
	#@rm -f distrib/etc/resolv.conf
	@find distrib -name .gitignore -exec rm -f {} \;
	@ln -s /var/smoothwall/vpn/ipsec.conf distrib/etc/ipsec.conf

	@echo "        copy to OffRoad ..." >/dev/tty
	@echo -e "\nCopying to OffRoad ..."
	@mkdir -p offroad
	@cd cdrom; cp -av . ../offroad
	@sed -i -e '/Install Roadster/,+3d' -e '/Convert ISO/,+3d' \
	  offroad/boot/grub/menu.lst

	@echo "        build final tarballs ..." >/dev/tty
	@echo -e "\nBuilding smoothwall.tgz, stand by ..."
	@tar -zcf cdrom/smoothwall.tgz -C distrib .

	@echo -e "\nBuilding smoothdev.tgz, stand by ..."
	@tar -zcf cdrom/smoothdev.tgz -C distrib-dev .

	@echo -e "\nBuilding smoothdoc.tgz, stand by ..."
	@tar -zcf cdrom/smoothdoc.tgz -C distrib-doc .

	@( \
	  src_tgz=../../../../smoothsrc.tar.gz; \
	  if [ -e "$src_tgz" ]; then \
	    echo -e "\nCopying smoothsrc.tgz, stand by ..."; \
	    cp $src_tgz  cdrom/smoothsrc.tgz; \
	  fi \
	)

extracontent:
	cp extracontent/* cdrom -avR
	(for I in README.txt README.html; do \
		cat templates/$$I | \
			sed "s@\%PRODUCT_NAME\%@$(PRODUCT_NAME)@g" | \
			sed "s@\%BASENAME\%@$(BASENAME)@g" \
		> cdrom/$$I; \
	done);
	find cdrom -name CVS -exec rm -rf {} \;
	find cdrom -name .svn -exec rm -rf {} \;

all: cdrom cdrom/smoothwall.tgz extracontent
	@( \
	  RELEASE=$(RELEASE); \
	  set $${RELEASE/-/ }; \
	  ISONAME=$(TARGET_DIR)/isos/smoothwall$(PRODUCT_EXTRA)-$${2}.iso; \
	  /sbin/mkisofs -o $${ISONAME} \
	    -b boot/grub/stage2_eltorito -J -R \
	    -no-emul-boot -boot-load-size 4 -boot-info-table \
	    -V $(BASENAME) --publisher "Silver Cloud Test" cdrom; \
	  echo "$${ISONAME} build complete"; \
	)
	@( \
	  RELEASE=$(RELEASE); \
	  set $${RELEASE/-/ }; \
	  ISONAME=$(TARGET_DIR)/isos/smoothwall$(PRODUCT_EXTRA)-$${2}-offroad.iso; \
	  /sbin/mkisofs -o $${ISONAME} \
	    -b boot/grub/stage2_eltorito -J -R \
	    -no-emul-boot -boot-load-size 4 -boot-info-table \
	    -V $(BASENAME) --publisher "Silver Cloud Test (OffRoad)" offroad; \
	  echo "$${ISONAME} build complete"; \
	)

clean:
	echo -n " $(PACKAGE)"
	rm -f Media.*.built
	rm -rf cdrom offroad distrib distrib-dev distrib-doc klibc temp
	make --no-print-directory -C cdromboot clean >/dev/null 2>&1
	make --no-print-directory -C installroot clean >/dev/null 2>&1
	make --no-print-directory -C runtimeroot clean >/dev/null 2>&1
	
download:
	@true
