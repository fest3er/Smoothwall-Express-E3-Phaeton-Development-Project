#!/usr/bin/perl
# Smoothinfo CLI reporting script v. 2.2
# Pascal Touch (nanouk) on Smoothwall forums - 2009

use lib "/usr/lib/smoothwall";
use header qw( :standard );
my %smoothinfosettings;

my $ColourRed = "\033[31;1m";
my $ColourGreen = "\033[32;1m";
my $ColourCyan = "\033[36;1m";
my $ColourPurp   = "\033[35;1m";
my $ColourYellow = "\033[33;1m";
my $ColourNorm = "\033[m";

$smoothinfosettings{'CONFIG'} = 'on';
$smoothinfosettings{'CONNTYPE'} = 'on';
$smoothinfosettings{'NETCONF1'} = 'on';
$smoothinfosettings{'NETCONF2'} = 'on';
$smoothinfosettings{'DISKSPACE'} = 'on';
$smoothinfosettings{'MEMORY'} = 'on';
$smoothinfosettings{'WRAP'} = 100;

system ("${swroot}/smoothinfo/bin/getchains.pl");
open (FILE, "<${swroot}/smoothinfo/etc/chains");
my @chains = (@chains,<FILE>);
chomp @chains;

open (OUT, ">",\$smoothinfosettings{'CHAINS'});
foreach (@chains) {
print OUT "$_,";
}
close (OUT);

print $ColourNorm . "\nBy default, the script will provide info on the" . $ColourYellow . " network configuration type" . $ColourNorm . ", the" .$ColourYellow . " connection type" . $ColourNorm . ",\nthe" . $ColourYellow . " network settings (as saved in /var/smoothwall/ethernet settings)" . $ColourNorm . ", the" . $ColourYellow . " disk space" . $ColourNorm . ",\nthe" . $ColourYellow . " state of all interfaces" . $ColourNorm . ", and the" . $ColourYellow . " current state of memory usage" . $ColourNorm . ".\n
Now, before you can generate a basic report (with no logs), would you like to include additional info on:\n";

print $ColourPurp . "\nthe network cards detected by the system? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'ADAPTERS'} = 'on'; } else { $smoothinfosettings{'ADAPTERS'} = 'off'; }

print $ColourPurp . "\nthe CPU? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'CPU'} = 'on'; } else { $smoothinfosettings{'CPU'} = 'off'; }

print $ColourPurp . "\nthe used IRQ's? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'IRQs'} = 'on'; } else { $smoothinfosettings{'IRQs'} = 'off'; }

print $ColourPurp . "\nthe status of stock services (DHCP, Web proxy, IM proxy, Pop3 proxy, Sip proxy, IDS)? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'SERVICES'} = 'on'; } else { $smoothinfosettings{'SERVICES'} = 'off'; }

print $ColourPurp . "\nthe status of some mod services (AWP, SEMF, DGAV, GAR)? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'MODSERVICES'} = 'on'; } else { $smoothinfosettings{'MODSERVICES'} = 'off'; }

if (-e "${swroot}/dhcp/enable") {
print $ColourPurp . "\nthe current DHCP settings? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'DHCPINFO'} = 'on'; } else { $smoothinfosettings{'DHCPINFO'} = 'off'; }
}

print $ColourPurp . "\nthe current IP routing table? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'ROUTE'} = 'on'; } else { $smoothinfosettings{'ROUTE'} = 'off'; }

print $ColourPurp . "\nthe current port forwarding rules? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'PORTFW'} = 'on'; } else { $smoothinfosettings{'PORTFW'} = 'off'; }

print $ColourPurp . "\nthe current firewall rules? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
unless ($response =~ /y/i) { $smoothinfosettings{'CHAINS'} = ''; }

print $ColourPurp . "\nthe loaded modules? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'LOADEDMODULES'} = 'on'; } else { $smoothinfosettings{'LOADEDMODULES'} = 'off'; }

print $ColourPurp . "\nthe resource usage snapshot? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'TOP'} = 'on'; } else { $smoothinfosettings{'TOP'} = 'off'; }

print $ColourPurp . "\nthe installed mods? (Y,N) " . $ColourNorm;
$response = "";
$response = <STDIN>;
if ($response =~ /y/i) { $smoothinfosettings{'MODLIST'} = 'on'; } else { $smoothinfosettings{'MODLIST'} = 'off'; }

print $ColourCyan . "\nGenerating report...\n\n";

&writehash("${swroot}/smoothinfo/etc/settings", \%smoothinfosettings);
wait();
system '/var/smoothwall/smoothinfo/bin/smoothinfo.pl';

print $ColourGreen . "Done!\n\n";

print $ColourNorm . "Now copy/paste the contents of " . $ColourYellow . "${swroot}/smoothinfo/etc/report.txt " . $ColourNorm . "\"AS IS\"\ninto your post edit box.\n\n";

unlink ("${swroot}/smoothinfo/etc/chains");
