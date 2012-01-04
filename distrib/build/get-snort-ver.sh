#! /bin/bash

# Must say what to look for
if [ $# -ne 2 ]; then
  echo "Usage: $0 { -v | -u } [ snort | daq ]"
  echo "    -v: fetch the latest snort version"
  echo "    -u: fetch the download URL"
  echo "    snort: fetch the info for snort"
  echo "    daq: fetch the info for the daq library"
  exit 1
fi

# Deal with snort's eccentricity. Look up the latest version and return it.
ANCHOR=`wget -q -O- "http://www.snort.org/snort-downloads/" \
       | egrep "${2}-[0-9.]*tar\.gz" \
       | head -1`

case $1 in
  -v) echo -n $ANCHOR | sed -e 's#.*'${2}'-##' -e 's/\.tar.gz.*//';;
  -u) echo -n $ANCHOR | sed -e 's#.*="##' -e 's/".*//';;
esac
