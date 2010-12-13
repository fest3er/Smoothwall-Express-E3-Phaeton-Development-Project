#!/usr/bin/gawk --re-interval -f
# the above doesn't work (stupid kernel) but serves as documentation

# Copyright (c) 2003-2004, 2006 Seth W. Klein <sk@sethwklein.net>
# Licensed under the Open Software License version 3.0
# See the file COPYING in the distribution tarball or
# http://www.opensource.org/licenses/osl-3.0.txt

BEGIN {
    if (strip == "yes") {
	strip = 1
	format = "%s\t%s\t%s\n"
	header_printed = 1
    } else {
	strip = 0
	print "# See also: protocols(5)" \
	    ", http://www.sethwklein.net/projects/iana-etc/\n#"
	format = "%-12s %3s %-12s # %s\n"
	header_printed = 0
    }
}
{ sub(/\r/, "") }
match($0, /^[ \t]+([0-9]+)[ \t]{1,5}([^ \t]+)(.*)/, f) {
    if ( ! header_printed) {
	printf format, "# protocol", "num", "aliases", "comments"
	header_printed = 1;
    }
    sub(/^[ \t]*/, "", f[3])
    printf format, tolower(f[2]), f[1], f[2], f[3]
    next
}
!strip { print "# " $0 }
