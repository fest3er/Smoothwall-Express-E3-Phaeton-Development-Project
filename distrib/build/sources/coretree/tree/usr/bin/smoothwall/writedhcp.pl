#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %dhcpsettings;
my %netsettings;

&readhash("${swroot}/ethernet/settings", \%netsettings);

my %dhcpsettings;

unlink "${swroot}/dhcp/enable";

open(FILE, ">/${swroot}/dhcp/dhcpd.conf") or die "Unable to write dhcpd.conf file";
flock(FILE, 2);

print FILE "authoritative;\n";

&readhash("${swroot}/dhcp/global", \%dhcpsettings);

if ($dhcpsettings{'BOOT_ENABLE'} eq 'on' && $dhcpsettings{'BOOT_SERVER'} &&
	$dhcpsettings{'BOOT_FILE'}  && $dhcpsettings{'BOOT_ROOT'})
{
	if ($dhcpsettings{'BOOT_SERVER'}) {
		print FILE "allow booting;\n"; }
	if ($dhcpsettings{'BOOT_SERVER'}) {
		print FILE "allow bootp;\n"; }
	if ($dhcpsettings{'BOOT_SERVER'}) {
		print FILE "next-server $dhcpsettings{'BOOT_SERVER'};\n" if ($dhcpsettings{'BOOT_SERVER'} ne ''); }
	if ($dhcpsettings{'BOOT_FILE'}) {
		print FILE "filename \"$dhcpsettings{'BOOT_FILE'}\";\n" if ($dhcpsettings{'BOOT_FILE'} ne ''); }
	if ($dhcpsettings{'BOOT_ROOT'}) {
		print FILE "option root-path \"$dhcpsettings{'BOOT_ROOT'}\";\n" if ($dhcpsettings{'BOOT_ROOT'} ne ''); }
}
print FILE "ddns-update-style ad-hoc;\n\n";

my $subnet;
my $id = 0;

foreach $subnet ('green', 'purple')
{
	%dhcpsettings = ();
	
	&readhash("${swroot}/dhcp/settings-$subnet", \%dhcpsettings);
	if ($subnet eq 'green' && $netsettings{'GREEN_DEV'} eq '') { next; }
	if ($subnet eq 'purple' && $netsettings{'PURPLE_DEV'} eq '') { next; }
	
	if ($dhcpsettings{'VALID'} eq 'yes')
	{
		if ($subnet eq 'green')
		{
			print FILE "subnet $netsettings{'GREEN_NETADDRESS'} netmask $netsettings{'GREEN_NETMASK'}\n";
			print FILE "{\n";
			print FILE "\toption subnet-mask $netsettings{'GREEN_NETMASK'};\n";
			print FILE "\toption domain-name \"$dhcpsettings{'DOMAIN_NAME'}\";\n";
			print FILE "\toption routers $netsettings{'GREEN_ADDRESS'};\n";
		}
		else
		{
			print FILE "subnet $netsettings{'PURPLE_NETADDRESS'} netmask $netsettings{'PURPLE_NETMASK'}\n";
			print FILE "{\n";
			print FILE "\toption subnet-mask $netsettings{'PURPLE_NETMASK'};\n";
			print FILE "\toption domain-name \"$dhcpsettings{'DOMAIN_NAME'}\";\n";
			print FILE "\toption routers $netsettings{'PURPLE_ADDRESS'};\n";
		}
					
		if ($dhcpsettings{'DNS1'})
		{
			print FILE "\toption domain-name-servers ";
			print FILE "$dhcpsettings{'DNS1'}";
			if ($dhcpsettings{'DNS2'}) {
				print FILE ", $dhcpsettings{'DNS2'}"; }
			print FILE ";\n";
		}
		if ($dhcpsettings{'NTP1'})
		{
			print FILE "\toption ntp-servers ";
			print FILE "$dhcpsettings{'NTP1'}";
			if ($dhcpsettings{'NTP2'}) {
				print FILE ", $dhcpsettings{'NTP2'}"; }
			print FILE ";\n";
		}
		if ($dhcpsettings{'WINS1'})
		{
			print FILE "\toption netbios-name-servers ";
			print FILE "$dhcpsettings{'WINS1'}";
			if ($dhcpsettings{'WINS2'}) {
				print FILE ", $dhcpsettings{'WINS2'}"; }
			print FILE ";\n";
		}
		if ($dhcpsettings{'NIS1'} && $dhcpsettings{'NIS_DOMAIN'}) {
			if ($dhcpsettings{'NIS1'}) {
				print FILE "\toption nis-servers ";
				print FILE "$dhcpsettings{'NIS1'}";
				if ($dhcpsettings{'NIS2'}) {
					print FILE ", $dhcpsettings{'NIS2'}"; }
				print FILE ";\n";
			}
			if ($dhcpsettings{'NIS_DOMAIN'}) {
				print FILE "\toption nis-domain \"$dhcpsettings{'NIS_DOMAIN'}\";\n";}
		}
		my $defaultleasetime = $dhcpsettings{'DEFAULT_LEASE_TIME'} * 60;
		my $maxleasetime = $dhcpsettings{'MAX_LEASE_TIME'} * 60;
		print FILE "\trange dynamic-bootp $dhcpsettings{'START_ADDR'} $dhcpsettings{'END_ADDR'};\n";
		print FILE "\tdefault-lease-time $defaultleasetime;\n";
		print FILE "\tmax-lease-time $maxleasetime;\n";
		open(RULES, "${swroot}/dhcp/staticconfig-$subnet") or die 'Unable to open config file.';
		while (<RULES>)
		{
			$id++;
			chomp($_);
			my @temp = split(/\,/,$_);
			if ($temp[4] eq 'on') {			
				print FILE "\thost $id { hardware ethernet $temp[1]; fixed-address $temp[2]; option host-name \"$temp[0]\"; }\n"; }
		}
		close(RULES);
		print FILE "}\n";
		open(DEV, ">${swroot}/dhcp/${subnet}") or die "Unable to write to device file";	
		if ($dhcpsettings{'ENABLE'} eq 'on' && $dhcpsettings{'VALID'} eq 'yes')
		{
			system ('/bin/touch', "${swroot}/dhcp/enable");
			if ($subnet eq 'green') {
				print DEV $netsettings{'GREEN_DEV'}; }
			else {
				print DEV $netsettings{'PURPLE_DEV'}; }
		}
		close(DEV);
	}
}

close FILE;

unlink "${swroot}/dhcp/uptodate";
