#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use Net::Netmask;

my %proxysettings;
my %netsettings;
my %mainsettings;

&readhash("${swroot}/proxy/settings", \%proxysettings);
&readhash("${swroot}/ethernet/settings", \%netsettings);
&readhash("${swroot}/main/settings", \%mainsettings);

open(FILE, ">/${swroot}/proxy/squid.conf") or die "Unable to write squid.conf file";
flock(FILE, 2);
print FILE <<END
# ACCESS CONTROLS
# ----------------------------------------------------------------
acl manager proto cache_object
acl localhost src 127.0.0.1/32
acl to_localhost dst 127.0.0.1/8
END
;
my $green_block = new2 Net::Netmask($netsettings{'GREEN_NETADDRESS'}, $netsettings{'GREEN_NETMASK'});
print FILE "acl localnetgreen src ".$green_block->base()."/".$green_block->bits()."\n";
if ($netsettings{'PURPLE_DEV'})
{
	my $purple_block = new2 Net::Netmask($netsettings{'PURPLE_NETADDRESS'}, $netsettings{'PURPLE_NETMASK'});
	print FILE "acl localnetpurple src ".$purple_block->base()."/".$purple_block->bits()."\n";
#acl localnetpurple src $netsettings{'PURPLE_NETADDRESS'}/$netsettings{'PURPLE_NETMASK'}
}
open (ACL, "${swroot}/proxy/acl.1") or die "Unable to open ACL list file";
while (<ACL>) { $_ =~ s/^#ACL- (.*)/$1/; print FILE $_; }
close (ACL);


print FILE <<END
# TAG: http_access
# ----------------------------------------------------------------
END
;

print FILE <<END

http_access allow localhost
http_access deny !Safe_ports
http_access deny CONNECT !SSL_ports
http_access allow localnetgreen
END
;

if ($netsettings{'PURPLE_DEV'})
{
	print FILE "http_access allow localnetpurple\n";
}

print FILE "http_access deny all\n";
print FILE "http_port $netsettings{'GREEN_ADDRESS'}:800";
if ($proxysettings{'TRANSPARENT'} eq 'on') {
	print FILE " intercept"; 
}
print FILE "\n";

open (C, "${swroot}/proxy/cache") or die "Unable to open cache config info file: $! \n";
while (<C>) { print FILE $_; }
close (C);
print FILE <<END;

maximum_object_size $proxysettings{'MAX_SIZE'} KB
minimum_object_size $proxysettings{'MIN_SIZE'} KB

cache_dir diskd /var/spool/squid/cache $proxysettings{'CACHE_SIZE'} 16 256

request_body_max_size $proxysettings{'MAX_OUTGOING_SIZE'} KB
#reply_body_max_size $maxincomingsizebytes allow all

END

print FILE "\n";


open (O, "${swroot}/proxy/other") or die "Unable to open other config info file: $! \n";
while (<O>) { print FILE $_; }
close (O);
if ($netsettings{'PURPLE_DEV'})
{
	print FILE "http_port $netsettings{'PURPLE_ADDRESS'}:800";
	if ($proxysettings{'TRANSPARENT'} eq 'on') {
		print FILE " intercept"; 
	}
	print FILE "\n";
}
print FILE "http_port 127.0.0.1:800 intercept\n";
print FILE <<END
icp_port 7


END
;

print FILE <<END
visible_hostname $mainsettings{'HOSTNAME'}
END
;


$maxincomingsizebytes = $proxysettings{'MAX_INCOMING_SIZE'} * 1024;

print FILE <<END

umask 022

logfile_rotate 0

strip_query_terms off

#acl shoutcast rep_header X-HTTP09-First-Line ^ICY.[0-9]
#upgrade_http0.9 deny shoutcast

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
