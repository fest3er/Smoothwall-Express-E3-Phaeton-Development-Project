# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions
include ../Makefile

PACKAGE = media-isoinstall

# Document these pkgs, don't install
TOOLCHAIN_PKGS = expect
OTHER_PKGS = updates jam sqlite

TARGET_DIR = /build/target/
INST_PKGS = glibc libsmooth+setup libgcc freetype libart_lgpl libpng libdnet \
	    daq libnet libnetfilter_queue libnfnetlink attr libcap-ng \
	    libcap fontconfig glib cairo libffi readline libmnl libpcap \
	    libosip2 libpcap pango zlib slang pixman libxml2 libusb \
	    gmp popt yaml \
	    HTML-Parser HTML-Tagset URI libwww-perl \
	    Net-Netmask NetAddr-IP \
	    newt binutils coreutils iana-etc mktemp file findutils gawk \
	    ncurses vim less sed flex gettext net-tools inetutils iputils \
	    whois perl bash bzip2 kbd e2fsprogs grep gzip module-init-tools \
	    procinfo procps psmisc shadow sysklogd logrotate sysvinit tar \
	    util-linux-ng joe wget iptables ipset ipbatch pciutils \
	    linux-dom0 openssl openssh httpd smoothd squid \
	    strace rrdtool cron tcpdump dnsmasq ntp openntpd hdparm hddtemp \
	    iproute2 gd expat ethtool listtables inotify-tools bridge-utils \
	    beep2 pcre trafficstats iftop setuids ppp isdn4k-utils rp-pppoe \
	    ibod dhcpcd eciadsl-synch_bin eciadsl-usermode cnxadsl dhcp snort \
	    oinkmaster openswan wireless_tools imspector diffutils clamav \
	    p3scan siproxd which miniupnpd ez-ipupdate noip cdrtools \
	    suricata smartmontools at dosfstools e2fsprogs grub linux-atm \
	    mtools nano reiserfsprogs parted xtables-addons cpio Python \
	    udev udev-config lm_sensors xz pcmciautils xen sysfsutils \
	    coretree
# 'coretree' *MUST* be last

DEV_PKGS = kernel-headers patch make autoconf automake bison dejagnu gcc \
	   gdb tcl m4 neon git screen lynx groff man man-pages \
	   klibc pkg-config sudo texinfo dev86 busybox \
	   libtool nasm acpica

cdrom:
	@mkdir cdrom
	@echo "      runtimeroot" >/proc/self/fd/0
	@make -C runtimeroot
	@echo "      installroot" >/proc/self/fd/0
	@make -C installroot
	@echo "      cdromboot" >/proc/self/fd/0
	@make -C cdromboot

cdrom/smoothwall.tgz:
	@echo "      prepare cdrom tree" >/proc/self/fd/0
	@mkdir -p distrib distrib/sys distrib-dev distrib-doc/usr/share/
	
	@install -D runtimerootimage.gz distrib/boot/initrd-$(K_SWE_VERSION)-dom0.gz
	
	@echo "        unpack dev packages" >/proc/self/fd/0
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
	
	@echo "        copy dev shared libs to distrib" >/proc/self/fd/0
	@echo "Copying dev shared libs to distrib:"
	@(cd distrib-dev; \
	  find . -name \*.so\* -print | cpio -pd ../distrib \
	)
	
	@echo "        unpack main packages" >/proc/self/fd/0
	@echo "Unpacking main packages:"
	@(for I in $(INST_PKGS); do \
	    if [ ! -e /$(TARGET_DIR)/smoothwall-$$I.tar.gz ]; then \
	      make -C ../$$I clean all; \
	      if [ $$? -ne 0 ]; then \
	        echo "$$I FAILED"; \
	        exit 0; \
	      fi; \
	    fi; \
	    echo -n "    $$I"; \
	    tar xf /$(TARGET_DIR)/smoothwall-$$I.tar.gz -C distrib || (echo; exit 68); \
	    echo; \
	  done; \
	);
	@echo "        move includes to distrib-dev" >/proc/self/fd/0
	@echo "Moving includes to distrib-dev:"
	@if [ -d distrib/usr/include ]; then \
	  mkdir -p distrib-dev/usr/include; \
	  cp -a distrib/usr/include distrib-dev/usr/; \
	  rm -rf distrib/usr/include/*; fi
	
	@echo "        move static libs to distrib-dev" >/proc/self/fd/0
	@echo "Moving static libs to distrib-dev:"
	@cd distrib; find . -name \*.a -o -name \*.la | while read a; do \
	  mkdir -p ../distrib-dev/`dirname $$a`; \
	  mv $$a ../distrib-dev/$$a; \
	done
	
	@echo "        move docs to distrib-doc" >/proc/self/fd/0
	@echo "Moving docs to distrib-doc:"
	@for I in man info share/man share/info share/doc examples; do \
	  if [ -d distrib/usr/$$I ]; then \
	    mkdir -p distrib-doc/usr/$$I; \
	    mv distrib/usr/$$I/* distrib-doc/usr/$$I/; fi; \
	done
	
	@echo "        strip symbols" >/proc/self/fd/0
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
	
	@echo "        secure tree ..." >/proc/self/fd/0
	@echo -e "\nSecuring tree ..."
	@( \
	  /build/securetree distrib; \
	  /build/securetree distrib-dev; \
	  bash -x /build/securetree distrib-doc; \
	  chmod u+s distrib/usr/bin/ping; \
	  chmod u+s distrib/usr/sbin/listtables; \
	  chown root:nobody distrib/usr/bin/setuids/*; \
	  chmod 4750 distrib/usr/bin/setuids/*; \
	  mkdir -p /distrib/tmp; chmod 1777 distrib/tmp; \
	) | sed -u -e 's/^/  /'
	
	@echo "        convert to shadow passwords ..." >/proc/self/fd/0
	@echo -e "\nConverting to shadow passwords ..."
	@( \
	/usr/bin/chroot distrib /usr/sbin/pwconv; \
	/usr/bin/chroot distrib /usr/sbin/grpconv; \
	) | sed -u -e 's/^/  /'
	
	@echo "        update library cache ..." >/proc/self/fd/0
	@echo -e "\nUpdating library cache ..."
	@( \
	/sbin/ldconfig -r distrib; \
	) | sed -u -e 's/^/  /'
	
	@echo "        tidy ..." >/proc/self/fd/0
	@echo -e "\nTidying ..."
	#@rm -f distrib/etc/resolv.conf
	@find distrib -name .gitignore -exec rm -f {} \;
	@ln -s /var/smoothwall/vpn/ipsec.conf distrib/etc/ipsec.conf
	
	@echo "      copy to OffRoad ..." >/proc/self/fd/0
	@echo -e "\nCopying to OffRoad ..."
	@mkdir -p offroad
	@cd cdrom; cp -av . ../offroad
	@sed  -e '/Install Roadster/,+4d' -e '/Convert ISO/,+9d' \
	  offroad/boot/grub/grub.conf #| sed  -e '$d'
	
	@echo "      build final tarballs ..." >/proc/self/fd/0
	@echo "        main ..." >/proc/self/fd/0
	@echo -e "\nBuilding smoothwall.tgz, stand by ..."
	@tar -zcf cdrom/smoothwall.tgz -C distrib .
	
	@echo "        dev ..." >/proc/self/fd/0
	@echo -e "\nBuilding smoothdev.tgz, stand by ..."
	@tar -zcf cdrom/smoothdev.tgz -C distrib-dev .
	
	@echo "        doc ..." >/proc/self/fd/0
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
	@echo "      build full ISO ..." >/proc/self/fd/0
	@( \
	  RELEASE=$(RELEASE); \
	  set $${RELEASE/-/ }; \
	  ISONAME=$(TARGET_DIR)/isos/$(RELEASE).iso; \
	  /sbin/mkisofs -o $${ISONAME} \
	    -b boot/grub/stage2_eltorito -J -R \
	    -no-emul-boot -boot-load-size 4 -boot-info-table \
	    -V $(BASENAME) --publisher "AGCL/Roadster" cdrom; \
	  echo "$${ISONAME} build complete"; \
	)
	@echo "      build offRoad ISO ..." >/proc/self/fd/0
	@( \
	  RELEASE=$(RELEASE); \
	  set $${RELEASE/-/ }; \
	  ISONAME=$(TARGET_DIR)/isos/$(RELEASE)-offroad.iso; \
	  /sbin/mkisofs -o $${ISONAME} \
	    -b boot/grub/stage2_eltorito -J -R \
	    -no-emul-boot -boot-load-size 4 -boot-info-table \
	    -V $(BASENAME) --publisher "AGCL/Roadster (OffRoad)" offroad; \
	  echo "$${ISONAME} build complete"; \
	)

clean:
	@echo -n " $(PACKAGE)"
	@rm -f Media.roadsterdevISO.built
	@rm -rf cdrom offroad distrib distrib-dev distrib-doc klibc temp
	@make --no-print-directory -C cdromboot clean >/dev/null 2>&1
	@make --no-print-directory -C installroot clean >/dev/null 2>&1
	@make --no-print-directory -C runtimeroot clean >/dev/null 2>&1
	
download:
	@true
