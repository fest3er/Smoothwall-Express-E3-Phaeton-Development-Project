# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../Makefile.versions
include ../../Makefile

default:
	mkdir -p ../cdrom/boot/grub
	cp -a /usr/lib/grub/i386-pc/* ../cdrom/boot/grub
	cp -a menu.lst ../cdrom/boot/grub/menu.lst
	cp -R /build/buildroot/kernel$(SWETYPE)/boot/vmlinuz-$(K_SWE_VERSION) ../cdrom/boot/vmlinuz
	cp ../installroot/installrootimage.gz ../cdrom/boot/iniramfs.igz
	echo -e "ISO boot prep complete\n\n\n\n\n"

clean:
	rm -rf ../cdrom/boot
	rm -rf tmp
