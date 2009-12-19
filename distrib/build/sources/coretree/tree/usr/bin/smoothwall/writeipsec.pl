#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my $filename = "${swroot}/vpn/config";

my %conn;

open(FILE, "$filename") or die 'Unable to open config file.';
my @current = <FILE>;
close(FILE);

open(CONF, ">${swroot}/vpn/ipsec.conf") or die 'unable to open conf file';
open(SECRETS, ">${swroot}/vpn/ipsec.secrets") or die 'unabe to open secrets file.';
flock CONF, 2;
flock SECRETS, 2;
print CONF "version 2\n";
print CONF "\n";
print CONF "config setup\n";
print CONF "\tinterfaces=%defaultroute\n";
print CONF "\tklipsdebug=none\n";
print CONF "\tplutodebug=none\n";
print CONF "\tplutowait=no\n";
print CONF "\tuniqueids=yes\n";
print CONF "\n";

foreach my $conn ('clear', 'clear-or-private', 'private-or-clear',
	'private', 'block', 'packetdefault')
{
	print CONF "conn $conn\n";
	print CONF "\tauto=ignore\n";
	print CONF "\n";
}

my $id = 0;
my $line;
foreach $line (@current)   
{
	chomp($line);
	my @temp = split(/\,/,$line);
	$conn{'NAME'} = $temp[0];
	$conn{'LEFT'} = $temp[1];
	$conn{'LEFT_SUBNET'} = $temp[2];   
	$conn{'RIGHT'} = $temp[3];      
	$conn{'RIGHT_SUBNET'} = $temp[4];   
	$conn{'SECRET'} = $temp[5];
	$conn{'ENABLED'} = $temp[6];
	$conn{'COMPRESSION'} = $temp[7];		
	$conn{'COMMENT'} = $temp[8];
	
	my $connname = $conn{'NAME'};
	$connname =~ s/\s+//g;
	
	if ($conn{'ENABLED'} eq 'on')
	{
		print SECRETS "$conn{'LEFT'} $conn{'RIGHT'} : PSK \"$conn{'SECRET'}\"\n";

		print CONF "conn $connname\n";
		print CONF "\tike=3des-md5\n";
		print CONF "\tesp=3des-md5\n";
		print CONF "\tauthby=secret\n";
		print CONF "\tkeyingtries=0\n";
		print CONF "\tleft=$conn{'LEFT'}\n";
		print CONF "\tleftsubnet=$conn{'LEFT_SUBNET'}\n";
		print CONF "\tleftnexthop=%defaultroute\n";
		print CONF "\tright=$conn{'RIGHT'}\n";
		print CONF "\trightsubnet=$conn{'RIGHT_SUBNET'}\n";
		print CONF "\trightnexthop=%defaultroute\n";
		if ($conn{'COMPRESSION'} eq 'on') {
			print CONF "\tcompress=yes\n"; }
		else {
			print CONF "\tcompress=no\n"; }
		print CONF "\tauto=start\n";
		print CONF "\n";
	}
	$id++;
}

close(CONF);
close(SECRETS);

