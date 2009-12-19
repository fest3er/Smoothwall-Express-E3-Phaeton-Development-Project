#!/usr/bin/perl
#
# Helper program to write a new IPAC settings file
#
# (c) Lawrence Manning, 2001

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %settings;

readhash("${swroot}/ethernet/settings", \%settings);

if (!open(FILE, '>/etc/ipac-ng/ipac.rules')) {
	die "Unable to create ipac.rules"; }

print FILE "Incoming GREEN Direct|ipac~o|$settings{'GREEN_DEV'}|all||\n";
print FILE "Outgoing GREEN Direct|ipac~i|$settings{'GREEN_DEV'}|all||\n";
print FILE "Incoming GREEN Forward|ipac~fi|$settings{'GREEN_DEV'}|all||\n";
print FILE "Outgoing GREEN Forward|ipac~fo|$settings{'GREEN_DEV'}|all||\n";

if ($settings{'CONFIG_TYPE'} == 1 || $settings{'CONFIG_TYPE'} == 3)
{
        print FILE "Incoming ORANGE Direct|ipac~o|$settings{'ORANGE_DEV'}|all||\n";
	print FILE "Outgoing ORANGE Direct|ipac~i|$settings{'ORANGE_DEV'}|all||\n";
        print FILE "Incoming ORANGE Forward|ipac~fi|$settings{'ORANGE_DEV'}|all||\n";
	print FILE "Outgoing ORANGE Forward|ipac~fo|$settings{'ORANGE_DEV'}|all||\n";

}

if ($settings{'CONFIG_TYPE'} == 0 || $settings{'CONFIG_TYPE'} == 1 ||
	$settings{'RED_TYPE'} eq 'PPPOE')
{
	print FILE "Incoming RED (PPP) Direct|ipac~o|ppp0|all||\n";
	print FILE "Outgoing RED (PPP) Direct|ipac~i|ppp0|all||\n";
	print FILE "Incoming RED (ISDN PPP) Direct|ipac~o|ippp0|all||\n";
	print FILE "Outgoing RED (ISDN PPP) Direct|ipac~i|ippp0|all||\n";
	print FILE "Incoming RED (PPP) Forward|ipac~fi|ppp0|all||\n";
	print FILE "Outgoing RED (PPP) Forward|ipac~fo|ppp0|all||\n";
	print FILE "Incoming RED (ISDN PPP) Forward|ipac~fi|ippp0|all||\n";
	print FILE "Outgoing RED (ISDN PPP) Forward|ipac~fo|ippp0|all||\n";
}
else
{
	print FILE "Incoming RED Direct|ipac~o|$settings{'RED_DEV'}|all||\n";
	print FILE "Outgoing RED Direct|ipac~i|$settings{'RED_DEV'}|all||\n";
	print FILE "Incoming RED Forward|ipac~fi|$settings{'RED_DEV'}|all||\n";
	print FILE "Outgoing RED Forward|ipac~fo|$settings{'RED_DEV'}|all||\n";
}

close FILE;
