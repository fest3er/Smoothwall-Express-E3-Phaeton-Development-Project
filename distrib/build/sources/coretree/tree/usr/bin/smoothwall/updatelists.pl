#!/usr/bin/perl

use IO::Socket;

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use update qw( :standard );

my @this;
my $return = &downloadlist();
if ($return =~ m/^HTTP\/\d+\.\d+ 200/) {
	unless(open(LIST, ">${swroot}/patches/available")) {
		die "Could not open available updates file."; }
	flock LIST, 2;
	@this = split(/----START LIST----\n/,$return);
	print LIST $this[1];
	close(LIST);
} else {
	die "Could not download updates list.";
}
