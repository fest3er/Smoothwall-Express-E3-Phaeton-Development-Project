# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.useKernel
include ../../Makefile
include ../../Makefile.conf
include ../../Makefile.versions

RUNTIME_ROOT = runtimeroot
BB = /opt/busybox
BBPROGS = [ [[ ar ash awk basename bbconfig blkid bunzip2 bzcat bzip2 cat \
	  chattr chgrp chmod chown chroot chvt clear cmp cp cpio cut date dd \
	  df diff dirname dmesg du echo ed egrep eject env expr false fdisk find \
	  findfs fsck grep gunzip gzip halt hdparm head ifconfig init insmod \
	  ipcrm ipcs kill killall less ln ls lsattr lsmod mdev microcom \
	  mkdir mkdosfs mkfifo mkfs.vfat mknod mkswap mktemp modprobe \
	  more mount mv nohup od patch pipe_progress poweroff ps pwd \
	  raidautorun rdev reboot reset resize rm rmdir rmmod run-parts \
	  runlevel sed sh sleep sort start-stop-daemon strings stty su \
	  swapoff swapon switch_root sync tail tar test time top touch \
	  true tty umount uname uniq unzip usleep vi wc wget which yes zcat

SBINPROGS = /sbin/mke2fs /sbin/sfdisk /sbin/depmod
AGCL_PKGS = udev kmod grub pciutils bash reiserfsprogs
ROOTDIRS = cdrom dev etc harddisk proc sys tmp usr run

bin:
	@(mkdir -p tmp/bin tmp/lib tmp/usr/lib/klibc/bin; \
	cp -aR /sbin/* tmp/bin)
	#cp -aRv /build/target/install tmp/bin/install
	#strip tmp/bin/*

lib:
	@(mkdir -p tmp/lib tmp/usr/lib/klibc/lib; \
	cp -aR /lib/* tmp/lib; \
	cp -aR /lib/lib* tmp/lib; \
	cp -aR /usr/lib/libncurses.so* tmp/lib/; \
	cp -aR /usr/lib/libtinfo.so* tmp/lib/; \
	tar xfz /build/target/smoothwall-linux$(KERNEL_TYPE).tar.gz -C tmp ./lib/modules > /dev/null 2>&1)

klibc:
	@cp -aR /build/buildroot/klibc/usr/lib/klibc/lib/klibc-*.so tmp/lib
	@cp -aR /build/buildroot/klibc/usr/lib/klibc/bin tmp/usr/lib/klibc/
	@cp -aR /build/buildroot/klibc/usr/lib/klibc/lib tmp/usr/lib/klibc/


all: clean bin lib klibc
	@mkdir -p tmp/sys
	@for i in $(ROOTDIRS); do mkdir -p tmp/$$i; done
	@ln -s bin tmp/sbin
	@cp -aR $(BB)/bin/busybox tmp/bin
	@cd tmp/bin; for FILE in $(BBPROGS); do ln -s busybox $$FILE; done
	#@cd tmp; ln -sv /bin/busybox init
	@cd tmp; cp ../etc/runt-init init; chmod 555 init

	@(cd tmp; \
	  cp -aR ../../../coretree/tree/etc/passwd etc; \
	  cp -aR ../../../coretree/tree/etc/group etc; \
	  install -D /usr/share/terminfo/l/linux usr/share/terminfo/l/linux; \
	  cp -aR /lib/ld-*.so /lib/ld-*.so.* /lib/libc-*.so /lib/libc.so.* lib; \
	  cp -aR ../etc/fstab etc; \
	  cp -aR ../etc/mtab etc; \
	  cp -aR ../etc/shells etc; \
	  cp -aR ../etc/termcap etc; \
	  cp -aR ../etc/udev-trigger.rc etc; chmod 755 etc/udev-trigger.rc; \
	  cp -aR ../etc/halt etc; chmod 755 etc/halt; \
	  cp -aR ../etc/modules.conf etc; \
	  cp -aR ../etc/storagedevices etc; \
	) > /dev/null 2>&1
	@(cd tmp/etc; \
	  gawk -f ../../etc/fill-pci-ids.awk < /usr/share/pci.ids > filled.pci.ids;\
	) > /dev/null 2>&1
	@(cd tmp; \
	  for i in $(AGCL_PKGS); do \
	    tar xfz /build/target/smoothwall-$$i.tar.gz; \
	  done; \
	  touch etc/ld.so.conf; \
	  /sbin/ldconfig -r .; \
	  mkdir dev/ida dev/rd; \
	  install -d lib/{firmware,udev/devices/{pts,shm}}; \
	  ln -sv /lib/firmware usr/lib/firmware; \
	  mknod -m0666 lib/udev/devices/null c 1 3; \
	  ln -sf proc/kcore /lib/udev/devices/core; \
	  ln -sf proc/self/fd /lib/udev/devices/fd; \
	  ln -sf proc/self/fd/0 /lib/udev/devices/stdin; \
	  ln -sf proc/self/fd/1 /lib/udev/devices/stdout; \
	  ln -sf proc/self/fd/2 /lib/udev/devices/stderr; \
	) > /dev/null 2>&1

ifdef BITS_64
	@(cd tmp; ln -s lib lib64)
endif

	@touch tmp/cdrominstall

	@/build/striptree tmp

	@# Build the runtime initramfs CPIO archive
	@cd tmp; find . | cpio -o -H newc | gzip > ../../$(RUNTIME_ROOT)image.gz

	@echo -e "runtimeroot build for $(MEDIA) complete\n\n"

clean:
	@rm -rf $(RUNTIME_ROOT) tmp/
	@rm -f ../$(RUNTIME_ROOT)image.gz
	@rm -f $(RUNTIME_ROOT)image.gz
	@rm -rf runtimeroot/
