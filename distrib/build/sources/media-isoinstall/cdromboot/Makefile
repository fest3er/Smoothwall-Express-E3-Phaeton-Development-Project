# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../Makefile.versions
include ../../Makefile

default:
	mkdir -p ../cdrom/boot/grub
	cp -a /build/buildroot/grub/usr/share/grub/i386-pc/* ../cdrom/boot/grub
	chown root:root grub.conf
	cp -a grub.conf ../cdrom/boot/grub/grub.conf
	cp -R /build/buildroot/linux$(KERNEL_TYPE)/boot/vmlinuz-$(K_AGCL_VERSION) ../cdrom/boot/vmlinuz
	cp ../installrootimage.gz ../cdrom/boot/iniramfs.igz
	echo -e "ISO boot prep complete\n\n\n\n\n"

clean:
	@rm -rf ../cdrom/boot
	@rm -rf tmp
