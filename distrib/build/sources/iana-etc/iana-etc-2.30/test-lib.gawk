#!/usr/bin/gawk -f

# Copyright (c) 2006 Seth W. Klein <sk@sethwklein.net>
# Licensed under the Open Software License version 3.0
# See the file COPYING in the distribution tarball or
# http://www.opensource.org/licenses/osl-3.0.txt

# This file is used by the test-*.gawk files

function normalize() {
    line = $0
    sub(/#.*/, "", line)
    sub(/[ \t]*$/, "", line)
    if (line ~ /^$/) {
	next }
    gsub(/[ \t]+/, " ", line)
    return line
}
function good() {
    next
}
function bad() {
    were_errors = 1
    print
    next
}
END {
    if (were_errors) {
	print "*** Error: above lines are invalid"
	exit 1
    }
}

