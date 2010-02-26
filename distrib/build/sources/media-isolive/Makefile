# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions
include ../Makefile

PACKAGE = media-isolive

TARGET_DIR = /build/target/

INST_PKGS = glibc newt libsmooth+setup newt libgcc binutils coreutils cpio iana-etc \
	    mktemp findutils iana-etc findutils gawk ncurses vim less sed flex \
	    gettext net-tools inetutils iputils whois perl bash bzip2 kbd e2fsprogs \
	    grep gzip module-init-tools procinfo procps psmisc shadow sysklogd \
	    logrotate sysvinit tar util-linux-ng joe wget iptables libipt_ACCOUNT \
	    ipbatch pciutils kernel-phaeton openssl openssh apache \
	    smoothd squid HTML-Parser HTML-Tagset URI libwww-perl strace libart_lgpl \
	    freetype rrdtool cron tcpdump dnsmasq libcap ntp openntpd hdparm \
	    hddtemp iproute2 libpng gd gmp expat ethtool popt listtables beep2 pcre \
	    trafficstats iftop setuids ppp isdn4k-utils rp-pppoe ibod dhcpcd \
	    eciadsl-synch_bin eciadsl-usermode cnxadsl dhcp snort oinkmaster openswan \
	    wireless_tools imspector diffutils traceroute clamav p3scan libosip2 \
	    siproxd which miniupnpd ez-ipupdate noip udev udev-config coretree \
	    reiserfsprogs xtables-addons

DEV_PKGS = kernel-headers patch make autoconf automake binutils bison dejagnu \
	   gcc gdb tcl m4 neon apr apr-util subversion screen lynx groff man man-pages \
	   sudo

ROOTDIRS = dev etc etc/udev proc sys tmp usr

KLIBC_VER = 1.5
klibc-tools:
	@echo -e "\nBuilding klibc..."
	@rm -rf klibc
	@mkdir -p klibc
	@tar xfz klibc-$(KLIBC_VER).tar.gz -C klibc >/dev/null
	@cd klibc/klibc-$(KLIBC_VER); ln -s ../../../kernel-phaeton/linux linux
	@make --no-print-directory -s -j 4 -C klibc/klibc-$(KLIBC_VER) >klibc/klibc-build.out 2>&1
	@INSTALLROOT=/build/buildroot/klibc make --no-print-directory -C klibc/klibc-$(KLIBC_VER) install


cdrom: klibc-tools
	@mkdir cdrom
	@echo -e "\nBuilding boot disk progs..."
	@make --no-print-directory -C installroot/bootdiskprogs 2>&1 | sed -u -e 's/^/  /'
	@echo -e "\nPrep'ing cdromboot..."
	@make --no-print-directory -C cdromboot 2>&1 | sed -u -e 's/^/  /'

cdrom/isolinux/iniramfs.igz:
	@mkdir -p distrib
	@(cd distrib; mkdir -p $(ROOTDIRS))

	@echo "Unpacking main packages:"
	@(for I in $(INST_PKGS) $(DEV_PKGS); do \
	    if [ ! -e /$(TARGET_DIR)/smoothwall-$$I.tar.gz ]; then \
	      make ---no-print-directory C ../$$I clean all; \
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

	@echo -e "\nCopying loader&iowrap to distrib..."
	@(cp -aR installroot/bootdiskprogs/loader installroot/bootdiskprogs/iowrap distrib/bin; \
	) | sed -u -e 's/^/  /'
	  
	@echo -e "\nCopying live.init to distrib..."
	@(cp installroot/etc/live-init distrib/etc/rc.d/rc.sysinit-setup; \
	  chmod 555 distrib/etc/rc.d/rc.sysinit-setup; \
	  echo -e "#! /bin/bash\nexec /sbin/init" >distrib/init; \
	  chmod 555 distrib/init; \
	) | sed -u -e 's/^/  /'

	@echo -e "\nMaking initial device nodes..."
	@(cd distrib; \
	  mknod -m0600 dev/console c 5 1; \
	    chown root:root dev/console; \
	  mknod -m0600 dev/kmem c 1 2; \
	    chown root:root dev/kmem; \
	  mknod -m0600 dev/mem c 1 1; \
	    chown root:root dev/mem; \
	  mknod -m0666 dev/null c 1 3; \
	    chown root:root dev/null; \
	  mkdir dev/ida dev/rd; \
	  install -d lib/{firmware,udev/devices/{pts,shm}}; \
	  test -e lib/udev/devices/null || mknod -m0666 lib/udev/devices/null c 1 3; \
	  test -e lib/udev/devices/kmsg || mknod -m0600 lib/udev/devices/kmsg c 1 11; \
	  typeset -i i; i=0; \
	  while [ $$i -lt 10 ]; do \
	    mknod -m0600 dev/tty$$i c 4 $$i; \
	    chown root:root dev/tty$$i; \
	    i=i+1; \
	  done; \
	  ln -sf /proc/self/fd lib/udev/devices/fd; \
	  ln -sf /proc/self/fd/0 lib/udev/devices/stdin; \
	  ln -sf /proc/self/fd/1 lib/udev/devices/stdout; \
	  ln -sf /proc/self/fd/2 lib/udev/devices/stderr; \
	  ln -sf /proc/kcore lib/udev/devices/core; \
	) | sed -u -e 's/^/  /'

	@echo -e "\nCopying udev.rc to distrib..."
	@(cp installroot/etc/udev.rc distrib/etc/udev.rc; \
	  chmod 555 distrib/etc/udev.rc) | sed -u -e 's/^/  /'

	@echo -e "\nSetting /etc/fstab..."
	@(echo "/	/	rootfs	defaults	0 0" > distrib/etc/fstab; \
	  echo "proc	/proc	proc	defaults	0 0" > distrib/etc/fstab; \
	  echo "sys	/sys	sysfs	defaults	0 0" >> distrib/etc/fstab; \
	) | sed -u -e 's/^/  /'

	@echo -e "\nPatching /etc/rc.sysinit to be live..."
	@patch -p0 < rc.sysinit.live.patch | sed -u -e 's/^/  /'

	@echo -e "\nMaking tweaked pci.ids..."
	@(gawk -f installroot/etc/fill-pci-ids.awk < distrib/usr/share/pci.ids > distrib/usr/share/filled.pci.ids; \
	) > /dev/null 2>&1

	@echo -e "\nSetting product info..."
	@mkdir -p distrib/var/smoothwall/main
	@(echo "  PRODUCT='$(PRODUCT_NAME)'"; \
	  echo "  VERSION='$(PRODUCT_VERSION)$(SWETYPE)'"; \
	  echo "  REVISION='$(REVISION)'"; \
	  echo "  UI_VERSION='$(UI_VERSION)'"; \
	  echo "  FIXES='$(FIXES_NUMBER)'"; \
	  echo "  ARCH='$(ARCH)'"; \
	) > distrib/var/smoothwall/main/productdata
	@chown nobody:nobody distrib/var/smoothwall/main/productdata
	@sed -u -e 's/^/  /' distrib/var/smoothwall/main/productdata

ifdef BITS_64
	@(cd cw../distrib; ln -s lib lib64)
endif

	@echo -e "\nSecuring tree ..."
	@( \
	/build/securetree distrib; \
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

	@echo -e "\nTidying ..."
	@rm -f distrib/etc/resolv.conf
	@rm -rf distrib/boot

	@echo -e "\nBuilding live initramfs stand by ..."
	@(cd distrib; find . | \
	  cpio -o -H newc | \
	  gzip -c > ../cdrom/isolinux/iniramfs.igz)


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

all: cdrom cdrom/isolinux/iniramfs.igz extracontent
	/sbin/mkisofs -o $(TARGET_DIR)/isos/smoothwall-$(BASENAME)$(PRODUCT_EXTRA)$(TYPE)-$(ARCH)-live.iso \
	  -b isolinux/isolinux.bin -J -c isolinux/boot.cat \
	  -no-emul-boot -boot-load-size 4 -boot-info-table \
	  -V $(BASENAME) --publisher "Smoothwall Express Phaeton" cdrom
	@echo "\nsmoothwall-$(BASENAME)$(PRODUCT_EXTRA)$(TYPE)-$(ARCH)-live.iso build complete"

clean:
	@echo -n " $(PACKAGE)"
	@(rm -rf cdrom distrib distrib-dev distrib-doc klibc; \
	  make --no-print-directory -C cdromboot clean; \
	  make --no-print-directory -C installroot clean; \
	  make --no-print-directory -C runtimeroot clean; \
	) > /dev/null 2>&1

	
download:
	@true
