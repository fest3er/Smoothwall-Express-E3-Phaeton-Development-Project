#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %proxysettings;
my %netsettings;
my %mainsettings;

&readhash("${swroot}/proxy/settings", \%proxysettings);
&readhash("${swroot}/ethernet/settings", \%netsettings);
&readhash("${swroot}/main/settings", \%mainsettings);

open(FILE, ">/${swroot}/proxy/squid.conf") or die "Unable to write squid.conf file";
flock(FILE, 2);
print FILE <<END
visible_hostname $mainsettings{'HOSTNAME'}
acl localnetgreen src $netsettings{'GREEN_NETADDRESS'}/$netsettings{'GREEN_NETMASK'}
END
;
if ($netsettings{'PURPLE_DEV'})
{
	print FILE <<END
acl localnetpurple src $netsettings{'PURPLE_NETADDRESS'}/$netsettings{'PURPLE_NETMASK'}
END
	;
}
print FILE "http_port $netsettings{'GREEN_ADDRESS'}:800";
if ($proxysettings{'TRANSPARENT'} eq 'on') {
	print FILE " transparent"; }
print FILE "\n";

if ($netsettings{'PURPLE_DEV'})
{
	print FILE "http_port $netsettings{'PURPLE_ADDRESS'}:800";
	if ($proxysettings{'TRANSPARENT'} eq 'on') {
		print FILE " transparent"; }
	print FILE "\n";
}
open (ACL, "${swroot}/proxy/acl") or die "Unable to open ACL list file";
while (<ACL>) { print FILE $_; }
close (ACL);

print FILE <<END
http_access allow localhost
http_access deny !Safe_ports
http_access deny CONNECT !SSL_ports
http_access allow localnetgreen
END
;

if ($netsettings{'PURPLE_DEV'})
{
	print FILE <<END
http_access allow localnetpurple
END
;
}

		print FILE <<END
http_access deny all
END
;

$maxincomingsizebytes = $proxysettings{'MAX_INCOMING_SIZE'} * 1024;

print FILE <<END

umask 022

maximum_object_size $proxysettings{'MAX_SIZE'} KB
minimum_object_size $proxysettings{'MIN_SIZE'} KB

cache_dir diskd /var/spool/squid/cache $proxysettings{'CACHE_SIZE'} 16 256

request_body_max_size $proxysettings{'MAX_OUTGOING_SIZE'} KB
reply_body_max_size $maxincomingsizebytes allow all

logfile_rotate 0

strip_query_terms off

acl shoutcast rep_header X-HTTP09-First-Line ^ICY.[0-9]
upgrade_http0.9 deny shoutcast

END
;

my ($remotehost, $remoteport) = split(/:/, $proxysettings{'UPSTREAM_PROXY'}); 
if ($remoteport eq '') { 
	$remoteport = 80; }

if ($remotehost ne '')
{
	print FILE "cache_peer $remotehost parent $remoteport 3130 default no-query";
	if ($proxysettings{'PEER_USERNAME'}) {
		print FILE " login=$proxysettings{'PEER_USERNAME'}:$proxysettings{'PEER_PASSWORD'}"; }
	print FILE "\n";
	print FILE "never_direct allow all\n";
}
close FILE;
