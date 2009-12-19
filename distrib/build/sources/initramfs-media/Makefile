# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions
include ../Makefile

TARGET_DIR = /build/target/
INST_PKGS = glibc newt libsmooth+setup newt libgcc binutils coreutils iana-etc \
	    mktemp findutils iana-etc findutils gawk ncurses vim less sed flex \
	    gettext net-tools inetutils iputils whois perl bash bzip2 kbd e2fsprogs \
	    grep grub gzip module-init-tools procinfo procps psmisc shadow sysklogd \
	    logrotate sysvinit tar util-linux-ng joe wget iptables libipt_ACCOUNT \
	    ipp2p ipbatch pciutils kernel-phaeton lilo openssl openssh apache \
	    smoothd squid HTML-Parser HTML-Tagset URI libwww-perl strace libart_lgpl \
	    freetype rrdtool cron tcpdump dnsmasq libcap ntp openntpd hdparm \
	    hddtemp iproute2 libpng gd gmp expat ethtool popt listtables beep2 pcre \
	    trafficstats iftop setuids ppp isdn4k-utils rp-pppoe ibod dhcpcd \
	    eciadsl-synch_bin eciadsl-usermode cnxadsl dhcp snort oinkmaster openswan \
	    wireless_tools imspector diffutils traceroute clamav p3scan libosip2 \
	    siproxd which miniupnpd ez-ipupdate noip udev udev-config coretree \
	    reiserfsprogs

DEV_PKGS = kernel-headers patch make autoconf automake binutils bison dejagnu \
	   gcc gdb tcl m4 neon apr apr-util subversion screen lynx groff man man-pages

KLIBC_VER = 1.5
klibc-tools:
	echo "  Building klibc"
	@rm -rf klibc
	@mkdir -p klibc
	@tar xfz klibc-$(KLIBC_VER).tar.gz -C klibc >/dev/null
	@cd klibc/klibc-$(KLIBC_VER); ln -s ../../../kernel-phaeton/linux linux
	@make -s -j 4 -C klibc/klibc-$(KLIBC_VER) >klibc/klibc-build.out 2>&1
	@INSTALLROOT=/build/buildroot/klibc make -e -C klibc/klibc-$(KLIBC_VER) install


cdrom: klibc-tools
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
	echo "Copying udev.rc to distrib:"
	@cp runtimeroot/etc/udev.rc distrib/etc/udev/

	@echo "Moving docs to distrib-doc:"
	@for I in include man info share/man share/info share/doc examples; do \
	  if [ -d distrib/usr/$$I ]; then \
	    mv distrib/usr/$$I distrib-doc/usr/$$I; fi; \
	  if [ -d distrib/usr/ssl/$$I ]; then \
	    mv distrib/usr/ssl/$$I distrib-doc/usr/ssl/$$I; fi; \
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

	@mkdir -p distrib/var/smoothwall/main
	@echo "  PRODUCT='$(PRODUCT_NAME)'" > distrib/var/smoothwall/main/productdata
	@echo "  VERSION='$(PRODUCT_VERSION)$(SWETYPE)'" >> distrib/var/smoothwall/main/productdata
	@echo "  REVISION='$(REVISION)'" >> distrib/var/smoothwall/main/productdata
	@echo "  UI_VERSION='$(UI_VERSION)'" >> distrib/var/smoothwall/main/productdata
	@echo "  FIXES='$(FIXES_NUMBER)'" >> distrib/var/smoothwall/main/productdata
	@echo "  ARCH='$(ARCH)'" >> distrib/var/smoothwall/main/productdata

	@chown nobody:nobody distrib/var/smoothwall/main/productdata

	@echo -e "\nMake some initial device nodes:"
	@( \
	mknod distrib/dev/console c 5 1; \
	  chown root:root distrib/dev/console; \
	  chmod 600 distrib/dev/console; \
	mknod distrib/dev/kmem c 1 2; \
	  chown root:root distrib/dev/kmem; \
	  chmod 600 distrib/dev/kmem; \
	mknod distrib/dev/mem c 1 1; \
	  chown root:root distrib/dev/mem; \
	  chmod 600 distrib/dev/mem; \
	mknod distrib/dev/null c 1 3; \
	  chown root:root distrib/dev/null; \
	  chmod 666 distrib/dev/null; \
	) | sed -u -e 's/^/  /'

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

	@echo -e "\nBuilding smoothwall.tgz, stand by ..."
	@tar -zcf cdrom/smoothwall.tgz -C distrib .

	@echo -e "\nBuilding smoothdev.tgz, stand by ..."
	@tar -zcf cdrom/smoothdev.tgz -C distrib-dev .

	@echo -e "\nBuilding smoothdoc.tgz, stand by ..."
	@tar -zcf cdrom/smoothdoc.tgz -C distrib-doc .

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
	@/sbin/mkisofs -o $(TARGET_DIR)/isos/smoothwall-$(BASENAME)$(PRODUCT_EXTRA)$(TYPE)-$(ARCH)-iniramfs.iso \
          -b isolinux/isolinux.bin -J -c isolinux/boot.cat \
	  -no-emul-boot -boot-load-size 4 -boot-info-table \
	  -V $(BASENAME) --publisher "Smoothwall Express Phaeton" cdrom
	@echo "smoothwall-$(BASENAME)$(PRODUCT_EXTRA)$(TYPE)-$(ARCH)-iniramfs.iso build complete"

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf cdrom distrib distrib-dev distrib-doc klibc
	@make --no-print-directory -C cdromboot clean
	@make --no-print-directory -C installroot clean
	@make --no-print-directory -C runtimeroot clean
	
download:
	true
