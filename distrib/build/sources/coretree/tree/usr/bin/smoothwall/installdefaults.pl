#!/usr/bin/perl

# Copies default settings.
#
# (c) SmoothWall Ltd, 2005.
#
# Released under the GPL v2.

$arg = shift(@ARGV);

unless ($arg)
{
	print STDERR "Usage: installdefaults.pl <defaultmode>\n";
	exit 1;
}

if ($arg ne 'open' && $arg ne 'halfopen' && $arg ne 'closed')
{
	print STDERR "Default mode should be open, halfopen or closed\n";
	exit 1;
}

system('/bin/cp', '-avR', glob("/var/smoothwall-${arg}/*"), '/var/smoothwall/');

exit 0;
