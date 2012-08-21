# SmoothWall Build system.
#
# Parts (c) SmoothWall Ltd 2005
# Parts (c) Neal P. Murphy 2009, 2010
#
# This code is distributed under the terms of the GPL v2.
#
# Top level Makefile for SmoothWall Build system.

# A few notes:
#   - This build system has 'Tidy-Build' built in. And very extended.
#   - The toolchain is build and installed as the starting user. Root is
#     invoked as needed, such as to adjust the host's /tool symlink.
#   - The final stage and the ISO are built as user root in the chroot jail;
#     thus root owns all files created in the final phase.
#   - The flash drive is prepared as user root. You must be *VERY* careful
#     when selecting the /dev name of the flash drive!
#
#   - This build system is much less fragile than the original. If you
#     build as a non-root user, there is almost no chance of damaging
#     your host system.
#   - If a package build fails, a minor tweak will almost always let it
#     proceed without trouble.
#   - If one of the 'adjusting' steps fails, it will back out its
#     adjustments so you can try again.
#
#   - After completing the full build, you can rebuild bits and pieces by
#     deleting the respective bread crumbs (Final.built and Media.*.built
#     in the sources directories, and the various files in build/crumbs)
#     then running 'make build'.

export SHELL = bash
export mARCH = $(shell uname -m)
CUSTOM_ENV = SHELL=BASH mARCH=$(mARCH) AGCL_TGT=$(mARCH)-agcl-linux-gnu \
	     HOME=$$HOME TERM=$$TERM PS1='\u:\w\$$ '

SRC_TGZ = ../../smoothsrc.tar.gz

ifndef ktype
ktype = kvm
CUSTOM_ENV += KERNEL_TYPE=-kvm
else
CUSTOM_ENV += KERNEL_TYPE=-$(ktype)
endif
export finalENV := $(CUSTOM_ENV)

# Default target, display help
#
default:
	@echo
	@echo "If using a host distrib to build, it is safest to build as a non-root user. The"
	@echo "system will become root as needed via 'sudo', so be sure that tool is ready."
	@echo
	@echo "The Phaeton/Roadster build system is much less fragile, and far less likely to"
	@echo "hose your host system. But you must still know what you are doing!"
	@echo
	@echo "Useful commands:"
	@echo "  make build"
	@echo "  make rebuild"
	@echo "  make media"
	@echo "  make package"
	@echo
	@echo "Useful targets:"
	@echo "  build             Build Phaeton/Roadster or continue building from an error."
	@echo "  media             Make ISO and prepare bootable flash drive."
	@echo "  rebuild           Build Phaeton/Roadster from scratch."
	@echo "  package           Clean, archive sources, pre-fetch tarballs, build,"
	@echo "                    and make media with matching source tarball."
	@echo
	@echo "  predownload       Download sourcecode packages in advance."
	@echo "  savesources       Make a tarball of the sources used to build."
	@echo "  reprep-final      Prepare to rebuild final stage *only*."
	@echo "  clean:            Clean everything in preparation for a fresh build from scratch."
	@echo
	@echo "Please view Makefile for other targets."
	@echo
	@echo "Predownload/download helpers"
	@echo "  Options to control downloads:"
	@echo "    CONTINUE_DL=    if you have a slow conection, this will continue"
	@echo "                    a previous, interrupted download instead of creating"
	@echo "                    tarball.[1-n]"
	@echo "    NO_CERT_CHECK=  do not check the veracity of SSL certs"
	@echo "    PASV=           use 'passive' FTP"
	@echo "    MIRROR_UNO=hostA and/or MIRROR_DOS=hostB"
	@echo "                    use hostA and/or hostB as sources for source packages."
	@echo "                    hostA defaults to \"\", and hostB defaults to "roadster.agcl.us/downloads"
	@echo "  This helps when you have a local web site containing the source tarballs"
	@echo "  or want to verify the URLs in the pkg makefiles."

# Host check
#
crumbs/host.checked:
	@bash host_check.sh
	@echo; echo
	@touch crumbs/host.checked

# build preparation
#
crumbs/tc_prep.built:
	@env -i $(CUSTOM_ENV) /bin/bash --noprofile --norc ./make_toolchain PREP
	@touch crumbs/tc_prep.built

# build stage 1
#
crumbs/tc_stage_1.built: crumbs/tc_prep.built
	@env -i $(CUSTOM_ENV) /bin/bash --noprofile --norc ./make_toolchain 1
	@env -i $(CUSTOM_ENV) PATH=/tools/bin:$$PATH /bin/bash tc_check 1
	@rm -f dummy.c a.out
	@touch crumbs/tc_stage_1.built

# build stage 2
#
crumbs/tc_stage_2.built: crumbs/tc_stage_1.built
	@env -i $(CUSTOM_ENV) /bin/bash --noprofile --norc ./make_toolchain 2
	@env -i $(CUSTOM_ENV) PATH=/tools/bin:$$PATH /bin/bash tc_check 2
	@rm -f dummy.c a.out
	@touch crumbs/tc_stage_2.built

# build stage 3
#
crumbs/tc_stage_3.built: crumbs/tc_stage_2.built
	@env -i $(CUSTOM_ENV) /bin/bash --noprofile --norc ./make_toolchain 3
	@touch crumbs/tc_stage_3.built


# prepare final build environment
#
crumbs/Final.prep.built: crumbs/tc_stage_3.built
	@tput smso; echo -e "\nPreparing Final Build tree"; tput rmso
	@env -i $(CUSTOM_ENV) /bin/bash --noprofile --norc as_chroot "/tools/bin/bash -l -c 'source /build/functions; prepare_final_tree >/build/crumbs/Final-prep.out 2>&1'"
	@touch crumbs/Final.prep.built

# must have resolv.conf in chroot jail
#
crumbs/resolv.conf:
	@if [ -f /etc/resolv.conf ]; then cp -v /etc/resolv.conf ./crumbs/; fi
	@if [ -f /etc/resolv.conf.dnsmasq ]; then cp -v /etc/resolv.conf.dnsmasq ./crumbs/resolv.conf; fi

# build all software
#
crumbs/Final.built: crumbs/resolv.conf crumbs/Final.prep.built
	@tput smso; echo -e "\nFinal Build"; tput rmso
	@env -i $(CUSTOM_ENV) /bin/bash --noprofile --norc as_chroot "$(CUSTOM_ENV) /tools/bin/bash -l -c /build/make_final"
	@touch crumbs/Final.built

build: crumbs/host.checked crumbs/Final.built

rebuild: clean predownload build

# prepare the installation media
#
media:
	@echo -e "\nMaking ISO image with linux-$(ktype)..."; sleep 1
	@env -i $(CUSTOM_ENV) /bin/bash --noprofile --norc as_chroot "$(CUSTOM_ENV) /bin/bash -l -c /build/make_media"
	@echo -e "\nMaking FLASH content..."; sleep 1
	@sudo su -c "./make_flash"

# after a successful build, this will do everything
#
package: clean savesources predownload build media
	@if [ `id -u` -eq 0 ]; then CMD="bash"; else CMD="sudo su"; fi \
	   $$CMD -c "rm -f $(SRC_TGZ)"

# Get tossed in the pokey with the proper environment
#
chroot:
	@env -i HOME=$$HOME TERM=$$TERM PS1='\u:\w\$$ ' /bin/bash --noprofile --norc ./as_chroot

# Pre-fetch source tarballs from internet
#
predownload:
	@echo -e "\nPre-downloading source tarballs..."; sleep 1
	@( \
	  export BUILD_PHASE="PreDownload"; \
	  . ./environment; \
	  set_build_environment; \
	  cd sources; \
	  for I in `find -maxdepth 3 -mindepth 1 -type d | sort`; do \
	    if [ "$$I" != "\./\.git" ]; then \
	      if echo $$I | egrep "^./mtools/mtools-4.0.15/scripts" >/dev/null 2>&1; then \
	        continue; \
	      elif echo $$I | egrep "^./xen$$" >/dev/null 2>&1; then \
	        make --no-print-directory -C $$I download_ipxe; \
	        if [ $$? -ne 0 ]; then \
	          echo "Xen ipxe DL oops!"; \
	          exit; \
	        fi; \
	      elif echo $$I | egrep "^./cdrtools" >/dev/null 2>&1; then \
	        NW="GMAKE_NOWARN=true "; \
	      else \
	        unset NW; \
	      fi; \
	      make $$NW -C $$I -n download >/dev/null 2>/dev/null; \
	      if [ $$? -eq 0 ]; then \
	        echo -n " `basename $$I`"; \
	        make --no-print-directory -C $$I download; \
	        if [ $$? -ne 0 ]; then \
	          echo "Oops"; \
	          exit; \
	        fi \
	      fi \
	    fi; \
	  done \
	); \
	echo

# Save a copy of the build directory, sans downloads, for future replication
#
savesources:
	@echo -e "\nArchiving sources..."; sleep 1
	@tar cfz $(SRC_TGZ) -C .. --exclude build/downloads .

packageinfos.html:
	@if [ `id -u` -ne 0 ]; then \
	  sudo su -c "make packageinfos.html"; \
	else \
	  rm -rf /tmp/downloads; mkdir /tmp/downloads; \
	  echo "<ul>" >/tmp/packageinfo.html; \
	  (cd sources; for I in `find -maxdepth 1 -mindepth 1 -type d`; do \
		if [ "$$I" != "CVS" ]; then \
			ROOT=$$PWD/../.. make --no-print-directory -C $$I packageinfo.html; \
		fi \
	  done); \
	fi
	-make -C sources/modules packageinfos.html
	@echo "</ul>" >>/tmp/packageinfo.html

# Prepare to rebuild the final stage
reprep-final:
	@env -i HOME=$$HOME TERM=$$TERM PS1='\u:\w\$$ ' /bin/bash --noprofile --norc as_chroot "/tools/bin/bash -l -c 'cd /build; bash make_final --unwind'"
	@if [ `id -u` -eq 0 ]; then CMD="bash"; else CMD="sudo su"; fi; \
	   $$CMD -c "make clean-distrib clean-final-crumbs clean-buildroot clean-target"
	@echo "You must run this twice in a row to complete the 'unwind' back"
	@echo "  to the toolchain. If this is the first time through, please"
	@echo "  run it again now!"

# Clean the entire directory back to pristine plus downloaded tarballs
clean:
	@if [ `id -u` -ne 0 ]; then \
	  sudo su -c "make clean"; \
	else \
	  make --no-print-directory clean-distrib; \
	  make --no-print-directory clean-buildroot; \
	  make --no-print-directory clean-target; \
	  make --no-print-directory clean-sources; \
	  make --no-print-directory clean-tools; \
	  make --no-print-directory clean-crumbs; \
	  rm -f $(SRC_TGZ); \
	fi

# Empty the buildroot directory
#
clean-buildroot:
	@if [ `id -u` -ne 0 ]; then \
	  sudo su -c "make --no-print-directory clean-buildroot"; \
	else \
	  tput smso; echo "cleaning buildroot"; tput rmso; \
	  rm -rf buildroot/*; \
	fi

# Delete the ISOs and the package tarballs
#
clean-target:
	@if [ `id -u` -ne 0 ]; then \
	  sudo su -c "make --no-print-directory clean-target"; \
	else \
	  tput smso; echo "cleaning target"; tput rmso; \
	  rm -rf target/*; \
	fi

# Clean the sources dir to 'pristine'
#
clean-sources:
	@if [ `id -u` -ne 0 ]; then \
	  sudo su -c "make --no-print-directory clean-sources"; \
	else \
	  tput smso; echo -n "cleaning sources"; tput rmso; \
	  (cd sources; for I in `find -maxdepth 1 -mindepth 1 -type d | sort`; do \
	     if [ "$$I" != "CVS" ]; then \
	       export BUILDDIR=`pwd`; make --no-print-directory -C $$I clean; \
	     fi \
	  done); \
	  echo; \
	fi

# Delete the toolchain
#
clean-tools:
	@if [ `id -u` -ne 0 ]; then \
	  sudo su -c "make --no-print-directory clean-tools"; \
	else \
	  tput smso; echo "cleaning tools"; tput rmso; \
	  rm -rf ../tools/*; \
	fi

# Delete the packages installed for the chroot environment
#
clean-distrib:
	@if [ `id -u` -ne 0 ]; then \
	  sudo su -c "make --no-print-directory clean-distrib"; \
	else \
	  tput smso; echo "cleaning distrib"; tput rmso; \
	  cd ..; rm -rf `ls -C1 . | egrep -v "build|tools"` ; \
	  echo "Only build and tools should print:"; \
	  ls -dC * | sed -e 's/^/  /'; \
	fi

# Sweep up the trail of bread crumbs
#
clean-crumbs: clean-prep-crumbs clean-stage1-crumbs clean-stage2-crumbs clean-stage3-crumbs clean-final-crumbs clean-media-crumbs

clean-prep-crumbs:
	@tput smso; echo "cleaning prep crumbs"; tput rmso
	@rm -f crumbs/host.checked crumbs/tc_prep.built crumbs/tc-Stage_PREP.debug

clean-stage1-crumbs:
	@tput smso; echo "cleaning stage1 crumbs"; tput rmso
	@rm -f sources/*/Stage_1*.{out,built} crumbs/*tage_1*

clean-stage2-crumbs:
	@tput smso; echo "cleaning stage2 crumbs"; tput rmso
	@rm -f sources/*/Stage_2*.{out,built} crumbs/*tage_2*

clean-stage3-crumbs:
	@tput smso; echo "cleaning stage3 crumbs"; tput rmso
	@rm -f sources/*/Stage_3*.{out,built} crumbs/*tage_3*

clean-final-crumbs:
	@if [ `id -u` -ne 0 ]; then \
	  sudo su -c "make clean-final-crumbs"; \
	else \
	  tput smso; echo "cleaning final crumbs"; tput rmso; \
	  rm -f sources/*/Final*.{out,built} crumbs/Final* crumbs/resolv.conf; \
	fi

clean-media-crumbs:
	@if [ `id -u` -ne 0 ]; then \
	  sudo su -c "make clean-media-crumbs"; \
	else \
	  tput smso; echo "cleaning media crumbs"; tput rmso; \
	  rm -f sources/*/Media*.{out,built} crumbs/Media*; \
	fi
