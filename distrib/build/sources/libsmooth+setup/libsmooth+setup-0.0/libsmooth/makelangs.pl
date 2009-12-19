#!/usr/bin/perl
#
# Used to process english.c and build the enum type from comments embeded 
# within said source file.

while (<>)
{
	if (/\/\* (TR_[A-Z0-9_]*)/) {
		print "\t$1,\n"; }
} 
print "};\n";
