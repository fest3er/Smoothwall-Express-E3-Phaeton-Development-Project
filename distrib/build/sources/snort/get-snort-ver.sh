#! /bin/bash

# Deal with snort's eccentricity. Look up the latest version and return it.
echo -n `wget -q -O- "http://www.snort.org/downloads/" \
       | grep "snort-current" \
       | head -1 \
       | sed -e 's#.*snort-current/snort-##' -e 's/\.tar.gz.*//'`
