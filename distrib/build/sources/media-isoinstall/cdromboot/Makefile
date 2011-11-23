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
	grub-mkimage -O i386-pc -o /tmp/grubcore-img \
	    --prefix=/boot/grub iso9660 biosdisk
	cat ../cdrom/boot/grub/cdboot.img /tmp/grubcore-img \
	    > ../cdrom/boot/grub/eltorito.img

	cp grub.cfg ../cdrom/boot/grub/grub.cfg
	cp -R /build/buildroot/linux-dom0/boot/vmlinuz-$(K_SWE_VERSION) ../cdrom/boot/linux-$(K_SWE_VERSION)-dom0
	cp ../installrootimage.gz ../cdrom/boot/iniramfs-$(K_SWE_VERSION)-dom0.igz
	echo -e "ISO boot prep complete\n\n\n\n\n"

clean:
	rm -rf ../cdrom/boot
	rm -rf tmp
