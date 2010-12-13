#!/usr/bin/gawk -f

# Copyright (c) 2003-2004, 2006, 2008 Seth W. Klein <sk@sethwklein.net>
# Licensed under the Open Software License version 3.0
# See the file COPYING in the distribution tarball or
# http://www.opensource.org/licenses/osl-3.0.txt

BEGIN {
    if (strip == "yes") {
	strip = 1
    } else {
	strip = 0
	print "# See also: services(5)" \
	    ", http://www.sethwklein.net/projects/iana-etc/\n#"
    }
    while (getline <"port-aliases") {
	sub(/#.*/, "")
	if (/^[ \t]*$/) { continue }
	#                  1:name         2:protocol     3:aliases
	match($0, /^[ \t]*([^ \t]+)[ \t]+([^ \t]+)[ \t]+(.*)$/, f)
	aliases[f[1] " " f[2]] = " " f[3]
    }
}
{ sub(/\r/, "") }
#           1:name               2:ws    3:port  4:range     5:proto  6:comment
match($0, /(^[[:alnum:]][^ \t]+)([ \t]+)([0-9]+)(-[0-9]+)?\/([^ \t]+)(.*)/, f) \
&& f[3] != "0" {
# port 0 means unallocated, per port-numbers
    name = f[1]
    whitespace = f[2]
    port = f[3]
    protocol = f[5]
    comment = f[6]
    if (strip) {
	whitespace = "\t"
	comment = ""
    } else if (length(comment) > 0)
	sub(/^[ \t]*/, "&# ", comment)
    print name whitespace port "/" protocol aliases[name " " protocol] comment
    next
}
# comment out, prettily
!/^#/ { sub(/^ /, "#") || sub(/^/, "# ") }
!strip { print }
