#!/bin/bash

# Script taken from LFS 6.5 and modified to check and report good and
#   bad versions of software.

# Simple script to check and report version numbers of critical development tools
echo
echo "Checking host for required packages. Failures must be addressed"
echo "before proceeding with building Smoothwall".
echo

export LC_ALL=C

# First set the required package versions
CSUDO=010500
VSUDO=1.5.0

CBASH=02050a
VBASH=2.5.a #(/bin/sh should be a symbolic or hard link to bash) 

CBINUTILS=021200
CBINUTILSM=022001
VBINUTILS=2.12
VBINUTILSM=2.20.1

CFLEX=020500
VFLEX=2.5

CBISON=01875
VBISON=1.875 #(/usr/bin/yacc should be a link to bison or small script that executes bison) 

CBZIP2=010002
VBZIP2=1.0.2

CCOREUTILS=0500
VCOREUTILS=5.0

CDIFFUTILS=020800
VDIFFUTILS=2.8

CFINDUTILS=040120 
VFINDUTILS=4.1.20 

CGAWK=0300
VGAWK=3.0 #(/usr/bin/awk should be a link to gawk) 

CGCC=030001
CGCCM=040403 #(Versions greater than 4.4.3 are not recommended as they have not been tested) 
VGCC=3.0.1

CGLIBC=020205
CGLIBCM=021102 #(Versions greater than 2.11.2 are not recommended as they have not been tested) 
VGLIBC=2.2.5
VGLIBCM=2.11.2

CGREP=0205
VGREP=2.5

CGZIP=010204
VGZIP=1.2.4

CKERNEL=020618
VKERNEL=2.6.18
CKERNELGCC=030000
VKERNELGCC=3.0

CM4=0104
VM4=1.4

CMAKE=0379
VMAKE=3.79

CPATCH=020504
VPATCH=2.5.4

CPERL=050600
VPERL=5.6.0

CSED=030002
VSED=3.0.2

CTAR=011400
VTAR=1.14

# TEXINFO is tested with makeinfo
CTEXINFO=0408
VTEXINFO=4.8


# Now verify the system's bonafides.

OK=0

# Check bash
if [ ! -e /bin/bash ]; then
  echo "    FAIL: /bin/bash not found. Need bash version>=$VBASH."
else
  WORK=`bash --version | head -1 | sed -e 's/.*version //' -e 's/(.*//'`
  OIFS=$IFS; IFS="."; set $WORK
  TBASH=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TBASH < $CBASH ]]; then
    echo "    FAIL: bash v$WORK seems too old (<$VBASH)"
    OK=1
  else
    echo "    OK: bash v$WORK seems new enough (>=$VBASH)"
  fi

  # check /bin/sh
  if [ `readlink -f /bin/sh` == "/bin/bash" ]; then
    echo "    OK: /bin/sh -> /bin/bash"
  else
    echo "    FAIL: /bin/sh -> `readlink -f /bin/sh`"
    OK=1
  fi
fi

# check binutils
if [ ! -e /usr/bin/ld ]; then
  echo "    FAIL: /usr/bin/ld not found. Need binutilis $VBINUTILS<=version<=$VBINUTILSM."
  OK=1
else
  WORK=`ld --version | head -1 | sed -e 's/[^0-9.]*\(.*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TBINUTILS=`echo $1 $2 $3|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TBINUTILS < $CBINUTILS || $TBINUTILS > $CBINUTILSM ]]; then
    echo "    FAIL: binutils v$WORK FAILED ($CBINUTILS-$CBINUTILSM)"
    OK=1
  else
    echo "    OK: binutils v$WORK seems OK (>=$VBINUTILS, <=$VBINUTILSM)"
  fi
fi

# Check bison
if [ ! -e /usr/bin/bison ]; then
  echo "    FAIL: /usr/bin/bison not found. Need bison version>=$VBISON."
    OK=1
else
  WORK=`bison --version | head -1 | sed -e 's/[^0-9.]*\(.*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TBISON=`echo $*|awk '{printf("%2.2d%2.3d\n", $1, $2)}'`
  IFS=$OIFS
  if [[ $TBISON < $CBISON ]]; then
    echo "    FAIL: bison v$WORK seems too old (<$VBISON)"
    OK=1
  else
    echo "    OK: bison v$WORK seems new enough (>=$VBISON)"
  fi

  # check /usr/bin/yacc
  readlink -ef /usr/bin/yacc | grep -i bison >/dev/null 2>&1
  if [ $? -eq 0 ]; then
    echo "    OK: /usr/bin/yacc starts bison"
  else
    egrep "exec /usr/bin/bison|exec '/usr/bin/bison'" /usr/bin/yacc >/dev/null 2>&1
    ret=$?
    if [ $ret -eq 0 ]; then
      echo "    OK: yacc starts bison"
    else
      test -h /usr/bin/yacc && echo "    FAIL: yacc -> `readlink -f /usr/bin/yacc`"
      test -h /usr/bin/yacc || echo "    FAIL: yacc doesn't start bison"
      OK=1
    fi
  fi
fi

# Check bzip2
if [ ! -e /bin/bzip2 -a ! -e /usr/bin/bzip2 ]; then
  echo "    FAIL: bzip2 not found in /bin or /usr/bin. Need bzip2 version>=$VBZIP2."
  OK=1
else
  WORK=`bzip2 --version </dev/null 2>&1 | head -1 | sed -e 's/bzip2.*Version \([0-9.]*\).*/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TBZIP2=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TBZIP2 < $CBZIP2 ]]; then
    echo "    FAIL: bzip2 v$WORK seems too old (<$VBZIP2)"
    OK=1
  else
    echo "    OK: bzip2 v$WORK seems new enough (>=$VBZIP2)"
  fi
fi

# Check coreutils
if [ ! -e /bin/chown ]; then
  echo "    FAIL: /usr/bin/chown not found. Need coreutils version>=$VCOREUTILS."
    OK=1
else
  WORK=`chown --version 2>&1| head -1 | sed -e 's/[^0-9.]*\(.*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TCOREUTILS=`echo $*|awk '{printf("%2.2d%2.2d\n", $1, $2)}'`
  IFS=$OIFS
  if [[ $TCOREUTILS < $CCOREUTILS ]]; then
    echo "    FAIL: coreutils v$WORK seems too old (<$VCOREUTILS)"
    OK=1
  else
    echo "    OK: coreutils v$WORK seems new enough (>=$VCOREUTILS)"
  fi
fi

# Check diffutils
if [ ! -e /usr/bin/diff ]; then
  echo "    FAIL: /usr/bin/diff not found. Need diffutils version>=$VDIFFUTILS."
    OK=1
else
  WORK=`diff --version 2>&1| head -1 | sed -e 's/[^0-9.]*\(.*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TDIFFUTILS=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TDIFFUTILS < $CDIFFUTILS ]]; then
    echo "    FAIL: diffutils v$WORK seems too old (<$VDIFFUTILS)"
    OK=1
  else
    echo "    OK: diffutils v$WORK seems new enough (>=$VDIFFUTILS)"
  fi
fi

# Check findutils
if [ ! -e /usr/bin/find ]; then
  echo "    FAIL: /usr/bin/find not found. Need findutils version>=$VFINDUTILS."
    OK=1
else
  WORK=`find --version 2>&1| head -1 | sed -e 's/[^0-9.]*\(.*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TFINDUTILS=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TFINDUTILS < $CFINDUTILS ]]; then
    echo "    FAIL: findutils v$WORK seems too old (<$VFINDUTILS)"
    OK=1
  else
    echo "    OK: findutils v$WORK seems new enough (>=$VFINDUTILS)"
  fi
fi

# Check flex
if [ ! -e /usr/bin/flex ]; then
  echo "    FAIL: /usr/bin/flex not found. Need flex version>=$VFLEX."
    OK=1
else
  WORK=`flex --version | head -1 | sed -e 's/[^0-9.]*\(.*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TFLEX=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TFLEX < $CFLEX ]]; then
    echo "    FAIL: flex v$WORK seems too old (<$VFLEX)"
    OK=1
  else
    echo "    OK: flex v$WORK seems new enough (>=$VFLEX)"
  fi
fi

# Check gawk
if [ ! -e /usr/bin/gawk ]; then
  echo "    FAIL: /usr/bin/gawk not found. Need gawk version>=$VGAWK."
    OK=1
else
  WORK=`gawk --version 2>&1| head -1 | sed -e 's/[^0-9.]*\(.*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TGAWK=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TGAWK < $CGAWK ]]; then
    echo "    FAIL: gawk v$WORK seems too old (<$VGAWK)"
    OK=1
  else
    echo "    OK: gawk v$WORK seems new enough (>=$VGAWK)"
  fi

  # check /usr/bin/awk
  if [ `readlink -ef /usr/bin/awk` == "/usr/bin/gawk" ]; then
    echo "    OK: /usr/bin/awk -> /usr/bin/gawk"
  else
    echo "    FAIL: /usr/bin/awk -> `readlink -f /bin/sh`"
    OK=1
  fi
fi

# Check gcc
if [ ! -e /usr/bin/gcc ]; then
  echo "    FAIL: /usr/bin/gcc not found. Need gcc version>=$VGCC."
    OK=1
else
  WORK=`gcc --version 2>&1| head -1 | sed -e 's/.* \([0-9.][0-9.]*\)$/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TGCC=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TGCC < $CGCC || $TGCC > $CGCCM ]]; then
    echo "    FAIL: gcc v$WORK seems too old (<$VGCC)"
    OK=1
  else
    echo "    OK: gcc v$WORK seems new enough (>=$VGCC)"
  fi
fi

# Check glibc
if [ ! -e /lib/libc.so.6 ]; then
  echo "    FAIL: /lib/libc.so.6 not found. Need glibc $VGLIBC<=version<=$VGLIBCM."
    OK=1
else
  WORK=`/lib/libc.so.6 --version 2>&1| head -1 | sed -e 's/.* version \([0-9.][0-9.]*\),.*/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TGLIBC=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TGLIBC < $CGLIBC || $TGLIBC > $CGLIBCM ]]; then
    echo "    FAIL: glibc v$WORK FAILED ($VGLIBC-$VGLIBCM)"
    OK=1
  else
    echo "    OK: glibc v$WORK seems OK (>=$VGLIBC, <=$VGLIBCM)"
  fi
fi

# Check grep
if [ ! -e /bin/grep -a ! -e /usr/bin/grep ]; then
  echo "    FAIL: not found in /bin or /usr/bin. Need grep version>=$VGREP."
    OK=1
else
  WORK=`grep --version 2>&1| head -1 | sed -e 's/[^0-9.]*\(.*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TGREP=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TGREP < $CGREP ]]; then
    echo "    FAIL: grep v$WORK seems too old (<$VGREP)"
    OK=1
  else
    echo "    OK: grep v$WORK seems new enough (>=$VGREP)"
  fi
fi

# Check gzip
if [ ! -e /bin/gzip -a ! -e /usr/bin/gzip ]; then
  echo "    FAIL: not found in /bin or /usr/bin. Need gzip version>=$VGZIP."
    OK=1
else
  WORK=`gzip --version 2>&1| head -1 | sed -e 's/[^0-9.]*\(.*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TGZIP=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TGZIP < $CGZIP ]]; then
    echo "    FAIL: gzip v$WORK seems too old (<$VGZIP)"
    OK=1
  else
    echo "    OK: gzip v$WORK seems new enough (>=$VGZIP)"
  fi
fi

# Check kernel
WORK=`cat /proc/version 2>&1| head -1 | sed -e 's/Linux version \([^-]*\).*/\1/'`
WORKGCC=`cat /proc/version 2>&1| head -1 | sed -e 's/.*gcc version \([^ ]*\).*/\1/'`
OIFS=$IFS; IFS="."; set $WORK
TKERNEL=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
set $WORKGCC
TKERNELGCC=`echo $*|awk '{printf("%2.2d%2.2d00\n", $1, $2)}'`
IFS=$OIFS
if [[ $TKERNEL < $CKERNEL || $TKERNELGCC < $CKERNELGCC ]]; then
  echo "    FAIL: kernel v$WORK (<$VKERNEL?) and/or its compiler v$WORKGCC (<$VKERNELGCC) seem too old"
  OK=1
else
  echo "    OK: kernel v$WORK built by GCC v$WORKGCC seems new enough (>=$VKERNEL, GCC>=$VKERNELGCC)"
fi

# Check m4
if [ ! -e /usr/bin/m4 ]; then
  echo "    FAIL: /usr/bin/m4 not found. Need m4 version>=$VM4."
  OK=1
else
  WORK=`m4 --version 2>&1| head -1 | sed -e 's/.* \([0-9.]*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TM4=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TM4 < $CM4 ]]; then
    echo "    FAIL: m4 v$WORK seems too old (<$VM4)"
    OK=1
  else
    echo "    OK: m4 v$WORK seems new enough (>=$VM4)"
  fi
fi

# Check make
if [ ! -e /usr/bin/make ]; then
  echo "    FAIL: /usr/bin/make not found. Need make version>=$VMAKE."
  OK=1
else
  WORK=`make --version 2>&1| head -1 | sed -e 's/.* \([0-9.]*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TMAKE=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TMAKE < $CMAKE ]]; then
    echo "    FAIL: make v$WORK seems too old (<$VMAKE)"
    OK=1
  else
    echo "    OK: make v$WORK seems new enough (>=$VMAKE)"
  fi
fi

# Check patch
if [ ! -e /usr/bin/patch ]; then
  echo "    FAIL: /usr/bin/patch not found. Need patch version>=$VPATCH."
  OK=1
else
  WORK=`patch --version 2>&1| head -1 | sed -e 's/.* \([0-9.]*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TPATCH=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TPATCH < $CPATCH ]]; then
    echo "    FAIL: patch v$WORK seems too old (<$VPATCH)"
    OK=1
  else
    echo "    OK: patch v$WORK seems new enough (>=$VPATCH)"
  fi
fi

# Check perl
if [ ! -e /usr/bin/perl ]; then
  echo "    FAIL: /usr/bin/perl not found. Need perl version>=$VPERL."
  OK=1
else
  WORK=`perl -V:version 2>&1| head -1 | sed -e 's/.*'"'"'\([0-9.]*\)'"'"'.*/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TPERL=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TPERL < $CPERL ]]; then
    echo "    FAIL: perl v$WORK seems too old (<$VPERL)"
    OK=1
  else
    echo "    OK: perl v$WORK seems new enough (>=$VPERL)"
  fi
fi

# Check sed
if [ ! -e /bin/sed -a ! -e /usr/bin/sed ]; then
  echo "    FAIL: sed not found in /bin or /usr/bin. Need sed version>=$VSED."
  OK=1
else
  WORK=`sed --version 2>&1| head -1 | sed -e 's/.* \([0-9.]*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TSED=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TSED < $CSED ]]; then
    echo "    FAIL: sed v$WORK seems too old (<$VSED)"
    OK=1
  else
    echo "    OK: sed v$WORK seems new enough (>=$VSED)"
  fi
fi

# Check sudo
if [ ! -e /usr/bin/sudo ]; then
  echo "    FAIL: /usr/bin/sudo not found. Need sudo version>=$VSUDO."
else
  WORK=`sudo -V | head -1 | sed -e 's/.*version //'`
  OIFS=$IFS; IFS=".p"; set $WORK
  TSUDO=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TSUDO < $CSUDO ]]; then
    echo "    FAIL: sudo v$WORK seems too old (<$VSUDO)"
    OK=1
  else
    echo "    OK: sudo v$WORK seems new enough (>=$VSUDO)"
  fi
fi

# Check tar
if [ ! -e /bin/tar -a ! -e /usr/bin/tar ]; then
  echo "    FAIL: tar not found in /bin or /usr/bin. Need tar version>=$VTAR."
  OK=1
else
  WORK=`tar --version 2>&1| head -1 | sed -e 's/.* \([0-9.]*\)/\1/'`
  OIFS=$IFS; IFS="."; set $WORK
  TTAR=`echo $*|awk '{printf("%2.2d%2.2d%2.2d\n", $1, $2, $3)}'`
  IFS=$OIFS
  if [[ $TTAR < $CTAR ]]; then
    echo "    FAIL: tar v$WORK seems too old (<$VTAR)"
    OK=1
  else
    echo "    OK: tar v$WORK seems new enough (>=$VTAR)"
  fi
fi

# Check compiling
echo 'main(){}' > dummy.c && gcc -o dummy dummy.c && echo "    OK: can compile" \
  && rm -f dummy.c dummy && if [ $OK -ne 0 ]; then
    echo -e "\nFix the failures before continuing.\n"
    exit 1
  else
    exit 0
  fi
echo "    FAIL: can't compile." && rm -f dummy.c dummy && echo -e "\nFix the failures before continuing.\n"
exit 255
