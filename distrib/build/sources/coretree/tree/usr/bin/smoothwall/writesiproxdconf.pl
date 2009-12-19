#!/usr/bin/perl
#
# SmoothWall CGIs
#
# (c) Smoothwall Ltd 2007

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %sipsettings;
my %netsettings;

&readhash("${swroot}/sipproxy/settings", \%sipsettings);
&readhash("${swroot}/ethernet/settings", \%netsettings);

open(FILE, ">${swroot}/sipproxy/siproxd.conf") or die "Unable to write sipproxd.conf file";
flock(FILE, 2);

print FILE <<END
user = sip
daemonize = 1
registration_file = /var/lib/siproxd/registrations/registrations
pid_file = /var/run/siproxd.pid
rtp_proxy_enable = 1
rtp_timeout = 300
rtp_dscp = 46
default_expires = 600
hosts_allow_reg = 10.0.0.0/8,172.16.0.0/12,192.168.0.0/16,169.254.0.0/16
debug_level = -1
plugindir=/usr/lib/siproxd/
END
;

my $red = &readvalue('/var/smoothwall/red/iface');

print FILE <<END
if_inbound = $netsettings{'GREEN_DEV'}
END
;

if ($red) {
	print FILE "if_outbound = $red\n"; }


if ($sipsettings{'LOG_CALLS'} eq 'on')
{
	print FILE "load_plugin = plugin_logcall.so\n";
	if ($sipsettings{'LOGGING'} < 1) { $sipsettings{'LOGGING'} = 1; }
}

if ($sipsettings{'LOGGING'} eq '0') {
	print FILE "silence_log = 2\n"; }
elsif ($sipsettings{'LOGGING'} eq '1') {
	print FILE "silence_log = 1\n"; }
else {
	print FILE "silence_log = 0\n"; }

my $highport = 7070 + $sipsettings{'CLIENTS'};

print FILE "rtp_port_low = 7070\n";
print FILE "rtp_port_high = $highport\n";

print FILE "rtp_dscp = 46\n";

close FILE;
