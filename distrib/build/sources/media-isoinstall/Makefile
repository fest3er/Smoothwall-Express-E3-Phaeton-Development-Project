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
	    iptables ipset ipbatch pciutils kernel-phaeton openssl openssh \
	    httpd smoothd squid HTML-Parser HTML-Tagset URI libwww-perl strace \
	    libart_lgpl freetype rrdtool cron tcpdump dnsmasq libcap ntp openntpd \
	    hdparm hddtemp iproute2 libpng gd gmp expat ethtool popt listtables  \
	    libdnet daq libnet libnetfilter_queue libnfnetlink inotify-tools \
	    beep2 pcre trafficstats iftop ppp isdn4k-utils \
	    rp-pppoe ibod dhcpcd eciadsl-synch_bin eciadsl-usermode \
	    cnxadsl dhcp snort oinkmaster openswan wireless_tools imspector  \
	    diffutils traceroute clamav p3scan libosip2 siproxd which miniupnpd \
	    ez-ipupdate noip cdrtools suricata \
	    at dosfstools e2fsprogs grub libpcap linux-atm mtools nano \
	    reiserfsprogs xtables-addons zlib slang cpio \
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
	make -C runtimeroot
	make -C installroot
	make -C cdromboot

cdrom/smoothwall.tgz:
	@mkdir -p distrib distrib/sys distrib-dev distrib-doc/usr/share/

	@install -D runtimerootimage.gz distrib/boot/initrd-$(K_SWE_VERSION).gz

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

	@echo "Copying dev shared libs to distrib:"
	@(cd distrib-dev; \
	        find . -name \*.so\* -print | cpio -pd ../distrib \
	)

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
	@echo "Moving includes to distrib-dev:"
	@if [ -d distrib/usr/include ]; then \
	  mkdir -p distrib-dev/usr/include; \
	  cp -a distrib/usr/include distrib-dev/usr/; \
	  rm -rf distrib/usr/include/*; fi
	if [ -d distrib/usr/ssl/include ]; then \
	  mkdir -p distrib-dev/usr/ssl/include; \
	  cp -a distrib/usr/ssl/include distrib-dev/usr/ssl/; \
	  rm -rf distrib/usr/ssl/include/*; fi

	@echo "Moving static libs to distrib-dev:"
	@cd distrib; find . -name \*.a -o -name \*.la | while read a; do \
	  mkdir -p ../distrib-dev/`dirname $$a`; \
	  mv $$a ../distrib-dev/$$a; \
	done

	@echo "Moving docs to distrib-doc:"
	@for I in man info share/man share/info share/doc examples; do \
	  if [ -d distrib/usr/$$I ]; then \
	    mkdir -p distrib-doc/usr/$$I; \
	    mv distrib/usr/$$I/* distrib-doc/usr/$$I/; fi; \
	  if [ -d distrib/usr/ssl/$$I ]; then \
	    mkdir -p distrib-doc/usr/ssl/$$I; \
	    mv distrib/usr/ssl/$$I/* distrib-doc/usr/ssl/$$I/; fi; \
	done

	@echo -e "\nProduct info:"
	@( \
	echo "  PRODUCT='$(PRODUCT_NAME)'"; \
	echo "  VERSION='$(PRODUCT_VERSION)$(SWETYPE)'"; \
	echo "  REVISION='$(REVISION)'"; \
	echo "  UI_VERSION='$(UI_VERSION)'"; \
	echo "  FIXES='$(FIXES_NUMBER)'"; \
	echo "  ARCH='$(ARCH)'"; \
	) | sed -u -e 's/^/  /'

	mkdir -p distrib/var/smoothwall/main
	( \
	  echo "PRODUCT='$(PRODUCT_NAME)'" ; \
	  echo "VERSION='$(PRODUCT_VERSION)$(SWETYPE)'" ; \
	  echo "REVISION='$(REVISION)'" ; \
	  echo "UI_VERSION='$(UI_VERSION)'" ; \
	  echo "FIXES='$(FIXES_NUMBER)'" ; \
	  echo "ARCH='$(ARCH)'" ; \
	) > distrib/var/smoothwall/main/productdata

	@chown nobody:nobody distrib/var/smoothwall/main/productdata

	@echo -e "\nSecuring tree ..."
	@( \
	  /build/securetree distrib; \
	  /build/securetree distrib-dev; \
	  /build/securetree distrib-doc; \
	  chmod u+s distrib/bin/traceroute; \
	  chmod u+s distrib/usr/bin/ping; \
	  chmod u+s distrib/usr/sbin/listtables; \
	  mkdir -p /distrib/tmp; chmod 1777 distrib/tmp; \
	) | sed -u -e 's/^/  /'
	
	@echo -e "\nConverting to shadow passwords ..."
	@( \
	/usr/bin/chroot distrib /usr/sbin/pwconv; \
	/usr/bin/chroot distrib /usr/sbin/grpconv; \
	) | sed -u -e 's/^/  /'
	
	@echo -e "\nUpdating library cache ..."
	@( \
	/sbin/ldconfig -r distrib; \
	) | sed -u -e 's/^/  /'

	@echo -e "\nTidying ..."
	#@rm -f distrib/etc/resolv.conf
	@find distrib -name .gitignore -exec rm -f {} \;
	@cp ../../make_flash cdrom
	@ln -s /var/smoothwall/vpn/ipsec.conf distrib/etc/ipsec.conf

	@echo -e "\nCopying to OffRoad ..."
	@mkdir -p offroad
	@cd cdrom; cp -av . ../offroad
	@cd offroad/isolinux; sed -ie 's/r0 scroll/r0 scroll explore/' isolinux.cfg
	@cd offroad/isolinux; sed -ie 's/115200/115200 explore/' isolinux.cfg

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
	/sbin/mkisofs -o $(TARGET_DIR)/isos/smoothwall-$(BASENAME)$(PRODUCT_EXTRA)$(TYPE)-$(ARCH)-iniramfs.iso \
	  -b isolinux/isolinux.bin -J -c isolinux/boot.cat \
	  -no-emul-boot -boot-load-size 4 -boot-info-table \
	  -V $(BASENAME) --publisher "Smoothwall Express Phaeton" cdrom
	@echo "smoothwall-$(BASENAME)$(PRODUCT_EXTRA)$(TYPE)-$(ARCH)-iniramfs.iso build complete"
	/sbin/mkisofs -o $(TARGET_DIR)/isos/smoothwall-$(BASENAME)$(PRODUCT_EXTRA)$(TYPE)-$(ARCH)-offroad.iso \
	  -b isolinux/isolinux.bin -J -c isolinux/boot.cat \
	  -no-emul-boot -boot-load-size 4 -boot-info-table \
	  -V $(BASENAME) --publisher "Smoothwall Express Phaeton (OffRoad)" offroad
	@echo "smoothwall-$(BASENAME)$(PRODUCT_EXTRA)$(TYPE)-$(ARCH)-offroad.iso build complete"

clean:
	@echo -n " $(PACKAGE)"
	@rm -f Media.phaetondevISO.built
	@rm -rf cdrom offroad distrib distrib-dev distrib-doc klibc temp
	@make --no-print-directory -C cdromboot clean >/dev/null 2>&1
	@make --no-print-directory -C installroot clean >/dev/null 2>&1
	@make --no-print-directory -C runtimeroot clean >/dev/null 2>&1
	
download:
	@true
