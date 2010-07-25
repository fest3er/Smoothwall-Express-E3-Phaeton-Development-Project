#! /bin/bash

# Deal with snort's eccentricity. Look up the latest version and return it.
ANCHOR=`wget -q -O- "http://www.snort.org/snort-downloads/" \
       | egrep "snort-[0-9.]*tar\.gz" \
       | head -1`

case $1 in
  -v) echo -n $ANCHOR | sed -e 's#.*snort-##' -e 's/\.tar.gz.*//';;
  -u) echo -n $ANCHOR | sed -e 's#.*="##' -e 's/".*//';;
esac
