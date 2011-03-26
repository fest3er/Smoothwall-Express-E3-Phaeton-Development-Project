# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../Makefile.versions
include ../../Makefile

default:
	mkdir -p ../cdrom/isolinux/
	cp /usr/share/syslinux/isolinux.bin ../cdrom/isolinux/
	cp isolinux.cfg ../cdrom/isolinux/isolinux.cfg
	cp dosfiles/phaeton.txt dosfiles/prhelpf2.txt dosfiles/phaeton.lss ../cdrom/isolinux/
	cp -R /build/buildroot/kernel$(SWETYPE)/boot/vmlinuz-$(K_SWE_VERSION) ../cdrom/isolinux/vmlinuz
	cp ../installroot/installrootimage.gz ../cdrom/isolinux/iniramfs.igz
	echo -e "ISOLinux boot prep complete\n\n\n\n\n"

clean:
	@rm -rf ../cdrom/isolinux
	@rm -rf tmp
