# SmoothWall Build system.
#
# Parts (c) SmoothWall Ltd 2005
# Parts (c) Neal P. Murphy 2009
#
# This code is distributed under the terms of the GPL v2.
#
# Top level Makefile for SmoothWall Build system.

SHELL = bash

GLOBALDEBUG = ON

# Default target, display help
#
default:
	@echo "Please use one of:"
	@echo
	@echo "build:            Primary build target (continuing from error).  You must know what you are doing!"
	@echo
	@echo "rebuild:          Primary build target from scratch.  You must know what you are doing!"
	@echo
	@echo "predownload:      Downloads sourcecode packages in advance."
	@echo
	@echo "packageinfos.html:Outputs HTML describing package names and URLs.  Please"
	@echo "                  call with -s switch to make."
	@echo
	@echo "clean:            Clean everything in preparation for a fresh build from scratch."
	@echo
	@echo "Please view the Makefile for other targets."

# Host check
#

crumbs/host.checked:
	@bash host_check.sh
	@echo; echo
	@touch crumbs/host.checked

# build preparation
#
crumbs/tc_prep.built:
	@./make_toolchain PREP
	@touch crumbs/tc_prep.built

# build stage 1
#
crumbs/tc_stage_1.built: crumbs/tc_prep.built
	@env -i HOME=$$HOME TERM=$$TERM PS1='\u:\w\$$ ' /bin/bash ./make_toolchain 1
	@PATH=/tools/bin:$$PATH; echo 'main(){}' > dummy.c && \
	    cc dummy.c && readelf -l a.out | \
	    grep ': /tools/lib/ld-linux.so.2' > /dev/null
	@readelf -l a.out | grep "program interpreter" | \
	  sed -e 's/.*interpreter/ProgInterp/' -e 's/]//' > crumbs/tc_stage_1-ld_linux.out
	@rm -f dummy.c a.out
	@touch crumbs/tc_stage_1.built

# build stage 2
#
crumbs/tc_stage_2.built: crumbs/tc_stage_1.built
	@env -i HOME=$$HOME TERM=$$TERM PS1='\u:\w\$$ ' /bin/bash ./make_toolchain 2
	@PATH=/tools/bin:$$PATH; echo 'main(){}' > dummy.c && \
	    cc dummy.c && readelf -l a.out | \
	    grep ': /tools/lib/ld-linux.so.2' > /dev/null
	@readelf -l a.out | grep "program interpreter" | \
	  sed -e 's/.*interpreter/ProgInterp/' -e 's/]//' > crumbs/tc_stage_2-ld_linux.out
	@rm -f dummy.c a.out
	@touch crumbs/tc_stage_2.built

# build stage 3
#
crumbs/tc_stage_3.built: crumbs/tc_stage_2.built
	@env -i HOME=$$HOME TERM=$$TERM PS1='\u:\w\$$ ' /bin/bash ./make_toolchain 3
	@touch crumbs/tc_stage_3.built


# prepare final build environment
#
crumbs/Final.prep.built: crumbs/tc_stage_3.built
	@tput smso; echo -e "\nPreparing Final Build tree"; tput rmso
	@env -i HOME=$$HOME TERM=$$TERM PS1='\u:\w\$$ ' /bin/bash as_chroot "/tools/bin/bash -l -c 'source /build/functions; prepare_final_tree >/build/crumbs/Final-prep.out 2>&1'"
	@touch crumbs/Final.prep.built

# must have resolv.conf in chroot jail
#
crumbs/resolv.conf:
	@if [ -f /etc/resolv.conf ]; then cp -v /etc/resolv.conf ./crumbs/; fi
	@if [ -f /etc/resolv.conf.dnsmasq ]; then cp -v /etc/resolv.conf.dnsmasq ./crumbs/resolv.conf; fi

# build full product
#
crumbs/Final.built: crumbs/resolv.conf crumbs/Final.prep.built
	@tput smso; echo -e "\nFinal Build"; tput rmso
	@env -i HOME=$$HOME TERM=$$TERM PS1='\u:\w\$$ ' /bin/bash as_chroot "/tools/bin/bash -l -c /build/make_final"
	@touch crumbs/Final.built

build: crumbs/host.checked crumbs/Final.built

rebuild: clean predownload build

redo-final:
	@sudo su -c "make clean-distrib clean-final-crumbs"

chroot:
	env -i HOME=$$HOME TERM=$$TERM PS1='\u:\w\$$ ' /bin/bash ./as_chroot

predownload:
	@(export BUILD_PHASE="PreDownload"; . ./environment;set_build_environment;cd sources; for I in `find -maxdepth 3 -mindepth 1 -type d`; do \
	  if [ "$$I" != "./CVS" ]; then \
	    make -C $$I BUILDENV=1 -n download >/dev/null 2>/dev/null; \
	    if [ $$? -eq 0 ]; then \
	      echo -n " $$I"; \
	      make --no-print-directory -C $$I BUILDENV=1 download; \
	      if [ $$? -ne 0 ]; then \
	        echo "Oops"; \
	        exit; \
	      fi \
	    fi \
	  fi \
	done); echo

packageinfos.html:
	@echo "<ul>" >/tmp/packageinfo.html
	-(cd sources; for I in `find -maxdepth 1 -mindepth 1 -type d`; do \
		if [ "$$I" != "CVS" ]; then \
			make -C $$I packageinfo.html; \
		fi \
	done);
	-make -C sources/modules packageinfos.html
	@echo "</ul>" >>/tmp/packageinfo.html


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
	fi

clean-buildroot:
	@if [ `id -u` -ne 0 ]; then \
          sudo su -c "make --no-print-directory clean-buildroot"; \
        else \
	  tput smso; echo "cleaning buildroot"; tput rmso; \
	  rm -rf buildroot/*; \
	fi

clean-target:
	@if [ `id -u` -ne 0 ]; then \
          sudo su -c "make --no-print-directory clean-target"; \
        else \
	  tput smso; echo "cleaning target"; tput rmso; \
	  rm -rf target/*; \
	fi

clean-sources:
	@if [ `id -u` -ne 0 ]; then \
          sudo su -c "make --no-print-directory clean-sources"; \
        else \
	  tput smso; echo -n "cleaning sources"; tput rmso; \
	  (cd sources; for I in `find -maxdepth 1 -mindepth 1 -type d`; do \
             if [ "$$I" != "CVS" ]; then \
               export BUILDDIR=`pwd`; make --no-print-directory -C $$I clean; \
             fi \
          done); \
	  echo; \
	fi


clean-tools:
	@if [ `id -u` -ne 0 ]; then \
          sudo su -c "make --no-print-directory clean-tools"; \
        else \
	  tput smso; echo "cleaning tools"; tput rmso; \
	  rm -rf ../tools/*; \
	fi

clean-distrib:
	@if [ `id -u` -ne 0 ]; then \
          sudo su -c "make --no-print-directory clean-distrib"; \
        else \
	  tput smso; echo "cleaning distrib"; tput rmso; \
	  cd ..; rm -rf `ls -C1 . | egrep -v "build|tools"` ; \
	fi


clean-crumbs: clean-prep-crumbs clean-stage1-crumbs clean-stage2-crumbs clean-stage3-crumbs clean-final-crumbs

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
