#!/usr/bin/perl
#
# SmoothWall CGIs
#
# (c) 2005 SmoothWall Ltd

use lib ('/usr/lib/smoothwall');
use header qw ( :standard );

my %settings, %netsettings;

readhash("${swroot}/snmp/settings", \%settings);
readhash("${swroot}/ethernet/settings", \%netsettings);

open (FILE, ">${swroot}/snmp/snmpd.conf");

if ($netsettings{'PURPLE_ADDRESS'}) {
	print FILE "agentaddress $netsettings{'PURPLE_ADDRESS'}:161\n"; }

print FILE <<END
agentaddress $netsettings{'GREEN_ADDRESS'}:161
rocommunity $settings{'COMMUNITY'}
disk /
disk /var/log
disk /boot
END
;

close FILE;
