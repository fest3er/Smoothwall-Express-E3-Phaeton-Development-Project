#!/usr/bin/perl
#
# (c) SmoothWall Ltd, 2006
#
# This script rewrites the p3scan config

use lib "/usr/lib/smoothwall/";
use header qw( :standard );

my %settings;

&readhash("${swroot}/p3scan/settings", \%p3scansettings);

open(FILE, "/usr/lib/smoothwall/p3scan.conf.in");
my @conf = <FILE>;
close(FILE);
open(FILE, ">${swroot}/p3scan/p3scan.conf");
print FILE @conf;

close(FILE);
