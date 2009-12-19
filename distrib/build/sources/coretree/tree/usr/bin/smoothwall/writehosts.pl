#!/usr/bin/perl
#
# (c) SmoothWall Ltd, 2002
#
# This script rewrites the hosts file

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my (%netsettings, %dhcpsettings, %mainsettings);
my @temp;
my $filename = "${swroot}/hosts/config";

&readhash("${swroot}/ethernet/settings", \%netsettings);
&readhash("${swroot}/dhcp/settings-green", \%dhcpsettings);
&readhash("${swroot}/main/settings", \%mainsettings);

open(FILE, ">${swroot}/hosts/hosts") or die 'Unable to write hosts.';
print FILE "127.0.0.1\tlocalhost\n";
&writeentry($netsettings{'GREEN_ADDRESS'}, $mainsettings{'HOSTNAME'});
open(RULES, "$filename") or die 'Unable to open config file.';
while (<RULES>)
{
	chomp($_);
	@temp = split(/\,/,$_);
	if ($temp[2] eq 'on') {
		&writeentry($temp[0], $temp[1]); }
}
close RULES;
close FILE;

sub writeentry
{
	$ip  = $_[0];
	$name = $_[1];

	print FILE "$ip\t$name";

	if (!($name =~ /\./) && $dhcpsettings{'DOMAIN_NAME'}) {
		print FILE " ${name}.${dhcpsettings{'DOMAIN_NAME'}}"; }
	print FILE "\n";
}
