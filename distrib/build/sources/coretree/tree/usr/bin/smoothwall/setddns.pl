#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

close(STDIN);
close(STDOUT);
close(STDERR);

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %settings;
my %noipsettings;
my $filename = "${swroot}/ddns/config";
my $cachefile = "${swroot}/ddns/ipcache";
my $ipcache = 0;

open(FILE, "$filename") or die 'Unable to open config file.';
my @current = <FILE>;
close(FILE);
my $lines = $#current + 1;
unless($lines) { exit 0; }

open(IP, "${swroot}/red/local-ipaddress") or die 'Unable to open local-ipaddress file.';
my $ip = <IP>;
close(IP);
chomp $ip;


if ($ARGV[0] ne '-f')
{
	open(IPCACHE, "$cachefile");
	$ipcache = <IPCACHE>;
	close(IPCACHE);
	chomp $ipcache;
}

if ($ip ne $ipcache)
{
	my $id = 0;
	my $success = 0;
	my $line;

	foreach $line (@current)
	{
		$id++;
		chomp($line);
		my @temp = split(/\,/,$line);
		unless ($temp[7] eq "off")
		{
			$settings{'SERVICE'} = $temp[0];
			$settings{'HOSTNAME'} = $temp[1];
			$settings{'DOMAIN'} = $temp[2];
			$settings{'PROXY'} = $temp[3];
			$settings{'WILDCARDS'} = $temp[4];
			$settings{'LOGIN'} = $temp[5];
			$settings{'PASSWORD'} = $temp[6];
			$settings{'ENABLED'} = $temp[7];
			my @service = split(/\./, "$settings{'SERVICE'}");
			$settings{'SERVICE'} = "$service[0]";
			if ($settings{'SERVICE'} eq 'no-ip')
			{
				$noipsettings{'LOGIN'} = $settings{'LOGIN'};
				$noipsettings{'PASSWORD'} = $settings{'PASSWORD'};
				$noipsettings{'HOSTNAME'} = $settings{'HOSTNAME'};
				$noipsettings{'DOMAIN'} = $settings{'DOMAIN'};
				$noipsettings{'DAEMON'} = 'N';
				$noipsettings{'DEVICE'} = '';
				$noipsettings{'INTERVAL'} = '1';
				$noipsettings{'NAT'} = 'N';
				$noipsettings{'GROUP'} = ';';
				if ($settings{'PROXY'} eq 'on') { $noipsettings{'PROXY'} = 'Y'; }
				else { $noipsettings{'PROXY'} = 'N'; }

				&writehash("${swroot}/ddns/noipsettings", \%noipsettings);
				open(F, "${swroot}/ddns/noipsettings");
				my @unsorted = <F>;
				close(F);
				my @sorted = sort { $b cmp $a } @unsorted;
				open(F, ">${swroot}/ddns/noipsettings");
				flock F, 2;
				print F @sorted;
				close(F);

				my @ddnscommand = ('/usr/bin/noip','-c',"${swroot}/ddns/noipsettings",'-i',"$ip");
				&log( "ddnscommand: @ddnscommand " );

				my $result = system(@ddnscommand);

				if ( $result != 0) {
					&log("Dynamic DNS ip-update for $settings{'HOSTNAME'}.$settings{'DOMAIN'}: failure"); }
				else 
				{
					&log("Dynamic DNS ip-update for $settings{'HOSTNAME'}.$settings{'DOMAIN'}: success");
					$success++;
				}
			}
			else
			{
				my $host = $settings{'DOMAIN'};
				if ($settings{'HOSTNAME'}) {
					$host = "$settings{'HOSTNAME'}.$settings{'DOMAIN'}"; }
				
				my @ddnscommand = ('/usr/bin/ez-ipupdate', '-a', "$ip", '-S', "$settings{'SERVICE'}", '-u', "$settings{'LOGIN'}:$settings{'PASSWORD'}", '-h', $host,  '-q');
				if ($settings{'SERVICE'} eq 'zoneedit') {
					push @ddnscommand, ('-s', 'dynamic.zoneedit.com'); }
				if ($settings{'WILDCARDS'} eq 'on') { push(@ddnscommand, '-w'); } 
				my $result = system(@ddnscommand);
				if ( $result != 0) {
					&log("Dynamic DNS ip-update for $host: failure"); }
				else
				{
					&log("Dynamic DNS ip-update for $host: success");
					$success++;
				}
			}
		}
		else {
			$success++; }

	}

	if ($lines == $success)
	{
		open(IPCACHE, ">$cachefile");
		flock IPCACHE, 2;
		print IPCACHE $ip;
		close(IPCACHE);
	}
}

else { &log('Dynamic DNS ip-update: your IP is already up-to-date'); }
