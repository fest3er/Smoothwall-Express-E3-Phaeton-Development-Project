#!/usr/bin/gawk -f test-lib.gawk -f
# the above doesn't work (stupid kernel) but serves as documentation

# Copyright (c) 2006 Seth W. Klein <sk@sethwklein.net>
# Licensed under the Open Software License version 3.0
# See the file COPYING in the distribution tarball or
# http://www.opensource.org/licenses/osl-3.0.txt

normalize() ~ /^[a-zA-Z0-9_+/.-]+ [0-9]+( [a-zA-Z0-9_+/.-]+)*$/ \
    { good() }
{ bad() }
