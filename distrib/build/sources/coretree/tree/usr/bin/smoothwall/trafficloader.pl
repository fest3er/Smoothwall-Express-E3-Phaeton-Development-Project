#!/usr/bin/perl
# QoS for Express
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team


use strict;
use warnings;
use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothtype qw( :standard );
use Socket;

my (%netsettings, %trafficsettings);

readhash("${swroot}/ethernet/settings", \%netsettings);
readhash("${swroot}/traffic/settings", \%trafficsettings);


my @internal_interface = ();
my %internal_netaddress = ();
my %internal_netmask = ();
for(qw/GREEN_DEV ORANGE_DEV PURPLE_DEV/) {
	if(defined $netsettings{$_} &&  $netsettings{$_} ne '') {
		
		push @internal_interface, $netsettings{$_};
		if(defined $trafficsettings{'PERIPSTATS'} && $trafficsettings{'PERIPSTATS'} eq 'on') {
			my $ad = $_;
			my $colour = $_;
			$ad =~ s/_DEV/_NETADDRESS/;
			$colour =~ s/_DEV//;
			$internal_netaddress{$colour} = $netsettings{$ad} if defined $netsettings{$ad} && $netsettings{$ad} ne '';
			$ad =~ s/_NETADDRESS/_NETMASK/;
			$internal_netmask{$colour} = $netsettings{$ad} if defined $netsettings{$ad} && $netsettings{$ad} ne '';
		}
	}
}

my $external_interface = &readvalue('/var/smoothwall/red/iface');
my $internal_speed = $trafficsettings{'INTERNAL_SPEED'} || 13107200; # 100mbit sanity default

my $upload_speed = $trafficsettings{'UPLOAD_SPEED'} || 32768; # 256 kbit sanity default
my $download_speed =  $trafficsettings{'DOWNLOAD_SPEED'} || 65536; # 512 kbit

# which class to use for things not otherwise classified
my $default_traffic = $trafficsettings{'DEFAULT_TRAFFIC'} || 'normal';
# tc classids are numeric, so need to map our names to these numbers

# this script is designed to produce a sane default even if there are no traffic settings.
my %classids = (
	'all' => 100, 'normal' => 109, 'high' => 110,	'low' => 111, 'slow' => 112,
	'smoothadmin' => 113, 'webcache' => 114, 'localtraffic' => 115,	'smallpkt' => 116,
);
# optional override - may want extra classes
if(defined $trafficsettings{CLASSIDS} && $trafficsettings{CLASSIDS} ne '') {
	%classids = split(',', $trafficsettings{CLASSIDS});
}
my %prio = (
	'high' => 0, 'normal' => 1, 'low' => 2, 'slow' => 3,
	'smoothadmin' => 0,	'webcache' => 1, 'localtraffic' => 0, 'smallpkt' => 0,
);
# optional override - needed if want extra classes or to change the prorities of those already here
if(defined $trafficsettings{PRIO} && $trafficsettings{PRIO} ne '') {
	%prio = split(',', $trafficsettings{PRIO});
}
# rates for data comming into the smoothie
# read these in from UI sometime
# initial data all appropriate for 512/256 ADSL
my %drate = (
	'normal' => 26214, 'high' => 13107, 'low' => 9830, 'slow' => 655,
	'smoothadmin' => 3276, 'webcache' =>  3276, 'localtraffic' => 655360, 'smallpkt' => 3276,
);


my %dceil = (
	'normal' => 58982, 'high' => 58982, 'low' => 26214, 'slow' => 1310,
	'smoothadmin' => 62259, 'webcache' => 62259, 'localtraffic' => 12451840, 'smallpkt' => 6553,
);

# optional override - (well not that optional unless you are happy with 512k adsl :)
if(defined $trafficsettings{DRATE} && $trafficsettings{DRATE} ne '') {
	%drate = split(',', $trafficsettings{DRATE});
}
if(defined $trafficsettings{DCEIL} && $trafficsettings{DCEIL} ne '') {
	%dceil = split(',', $trafficsettings{DCEIL});
}

# add these even after import
$drate{$_} = $internal_speed for @internal_interface;
$dceil{$_} = $internal_speed for @internal_interface;

# max rate we can send data
my %urate = (
	'normal' => 13107, 'high' => 6553, 'low' => 4915, 'slow' => 327,
	'smoothadmin' => 1638, 'webcache' => 1638, 'localtraffic' => 1638, 'smallpkt' => 1638,
);

my %uceil = ( 
	'normal' => 29491, 'high' => 29491, 'low' => 13107, 'slow' => 655,
	'smoothadmin' => 31129, 'webcache' => 31129, 'localtraffic' => 31129, 'smallpkt' => 3276,
);

# optional override
if(defined $trafficsettings{URATE} && $trafficsettings{URATE} ne '') {
	%urate = split(',', $trafficsettings{URATE});
}
if(defined $trafficsettings{UCEIL} && $trafficsettings{UCEIL} ne '') {
	%uceil = split(',', $trafficsettings{UCEIL});
}

$urate{$external_interface} = $upload_speed;
$uceil{$external_interface} = $upload_speed;

# connection marks associated with 'special' classes
my %connmarks = (
	'smoothadmin' => 103, 'webcache' => 104, 'localtraffic' => 105,
);
# first connmark useable by user defined rule is 200
# we start with total, other classes get added
my @classsortorder = ('total');

# the mangle tables we work with

my $POSTR = 'trafficpostrouting';
my $OUTPUT = 'trafficoutput';
my $FORWARD = 'trafficforward';
# list of rules to implement
# protocol is alway an array as may need to iterate TCP and UDP
# each rule needs a distinct connmark
# they are in trafficsettings in the form
# R_{connmark}=name,enabled,tcp?udp?,(in|out),portrange,class,comment


my @rules = ();

for(sort keys %trafficsettings) {
	next unless /^R_(\d+)/; # just looking for rules
	my $cm = $1;
	my($name, $tcp, $udp, $dir, $port, $class, $comment) = split(',', $trafficsettings{$_});
	next if $class eq 'none'; # this one is being ignorred
	my $protocol = [];
	push @{$protocol}, 'TCP' if $tcp eq 'on';
	push @{$protocol}, 'UDP' if $udp eq 'on';

	push @rules, {
		'name' => $name,
		'protocol' => $protocol,
		'direction' => $dir,
		'connmark' => $cm,
		'port' => $port,
		'class' => $class,
		'comment' => $comment,
	};
}


# start with a clean slate always
removetraffic();

# maybe thats all we do...
exit(0) unless defined $trafficsettings{'ENABLE'} && $trafficsettings{'ENABLE'} eq 'on' && -e '/var/smoothwall/red/active';
	
# setting the root qdiscs
# adding root qdisc for external - specifying where default traffic goes
tcqdisc("$external_interface root handle 1: htb default $classids{$default_traffic}");

# 'all' is the parent class of all for outgoing data, set at the speed of the external interface
tcclass("$external_interface parent 1: classid 1:$classids{'all'} htb rate $urate{$external_interface}bps quantum 15000");

# adding root qdisc for internal - specifying where default traffic goes
# note that as download queueing is done at the internal interface
# all internal interfaces have to have the same speed. i.e. have to
# choose the speed of the lowest. This isnt so bad if we can assume
# that all internals are at least as fast as the external.
tcqdisc("$_ root handle 1: htb default $classids{$default_traffic}") for @internal_interface;

# note that the download rate of the internal interface is its full speed (100mbit for example)
tcclass("$_ parent 1: classid 1:$classids{'all'} htb rate $drate{$_}bps quantum 15000") for @internal_interface;

# default extra options is just 'quantum 1500'
my %extras = (
			  'low' => 'quantum 1500 burst 20k',
			  'smoothadmin' => 'quantum 1500 burst 100k',
			  'webcache' => 'quantum 1500 burst 100k',
			  'localtraffic' => 'quantum 15000',
			  'smallpkt' => 'quantum 1500 burst 20k cburst 100k',
			  );


# as we are doing a 'flat' scheme - all qdiscs are direct children of whole interface
# only extra options differ so can create things with a simple loop.			  
for my $tag (sort { $classids{$a} <=> $classids{$b} } keys %classids) {
	# note $extras{$tag} is usually undef
	next if $tag =~ /^(all|none)$/;
	stdclass($external_interface,$tag, $extras{$tag},\%urate,\%uceil); 
	# note can only extablish QOS for an interface that is up at the time.
	stdclass($_,$tag, $extras{$tag}, \%drate, \%dceil) for up_interfaces(@internal_interface); 
	push @classsortorder, $tag;
}


for my $dir (qw/up dn/) {
	iptables("-N ${external_interface}-${dir}-traf-tot");
}


# as we make rule keep a running tally of which class each connmark refers to as finally have to turn connmarks into classes
my %connmark_to_class = ();
for(qw/smoothadmin webcache localtraffic/) {
	$connmark_to_class{$connmarks{$_}} = (/localtraffic/ ? $_ : 'high') if defined $connmarks{$_};
}

# so can remember the order in which connmarks are pushed onto the traf-tot tables

my @rulenumbers = ();

# special smoothadmin rule - so can admin externally even when busy
if(defined $classids{'smoothadmin'}) {
	iptables("-A $OUTPUT -m connmark --mark 0 -j CONNMARK --set-mark $connmarks{'smoothadmin'} " .
			 "--out-interface $external_interface --protocol TCP --match multiport --source-ports 81,441,222");
}
# Webcache special rule - give squid something.
iptables("-A $OUTPUT -m connmark --mark 0 -j CONNMARK --set-mark $connmarks{'webcache'}  " .
		 "--out-interface $external_interface  --protocol TCP --match multiport --destination-ports 80,8080,443,8443") if defined $classids{'webcache'};


# Avoid mark localtraffic going output to external - because if its leaving to the outside it isnt local!
if(defined $classids{'localtraffic'}) {
	iptables("-A $OUTPUT -j RETURN -o $external_interface");

# Marking all other output destinations as localtraffic - 
# because if the traffic starts here e.g. from the web proxy and is going inwards
# it should run at full line speed.

	iptables("-A $OUTPUT  -j CONNMARK --set-mark $connmarks{'localtraffic'}");

# Avoid mark localtraffic going forward from external - as if it has come from the outside we want to shape it. 
	iptables("-A $FORWARD  -j RETURN -i $external_interface");

# Avoid mark localtraffic going forward to external - as if going to the outside want to shape it
	iptables("-A $FORWARD  -j RETURN -o $external_interface");

# Marking everything else being forwarded as localtraffic - as is going internal - internal
	iptables("-A $FORWARD -j CONNMARK --set-mark $connmarks{'localtraffic'}");
}

# now the defined rules - simple port based rules only unless 'special'.
for my $rule (@rules) {
	# we are doing this on
	my ($name,$mark, $port, $class, $direction, $proto) = @{$rule}{qw/name connmark port class direction protocol/};
	next if $port eq 'special'; # may do something with special rules later
	my ($sport, $dport) = '' x 2;
	
	if($port =~ /;/) {
		# multi port rule - semicolons as was in CSV file
		$port =~ s/;/,/g;
		$sport = " --match multiport --source-ports $port";
		$dport = " --match multiport --destination-ports $port";
	}
	else {
		$sport = "--source-port $port";
		$dport = "--destination-port $port";
	}
	$connmarks{$name} = $mark;
	$connmark_to_class{$mark} = $class;
	if($direction eq 'in' || $direction eq 'both') {
		# for a service here i.e. HOSTING a website
		for my $p (@{$proto}) {
			# so things from e.g. port 80 leave by the external
			iptables("-A $POSTR -m connmark --mark 0 -j CONNMARK --set-mark $mark --protocol $p $sport -o $external_interface");
			# and traffic comming in from external are for e.g port 80
			iptables("-A $POSTR -m connmark --mark 0 -j CONNMARK --set-mark $mark --protocol $p $dport -i $external_interface");
		}
	}	
	if($direction eq 'out' || $direction eq 'both') {
		# service is out on the net so reverse logic applies i.e. browsing a website
		for my $p (@{$proto}) {
			# things comming from e.g. port 80 are going internal
			iptables("-A $POSTR -m connmark --mark 0 -j CONNMARK --set-mark $mark --protocol $p $sport -i $external_interface");
			# and requests to e.g. port 80 go out on external
			iptables("-A $POSTR -m connmark --mark 0 -j CONNMARK --set-mark $mark --protocol $p $dport -o $external_interface");
		}
	}
	# collect per rule stats if chosen - so create named accounting tables for each directly connected
	# internal network.
	if(defined $trafficsettings{'PERIPSTATS'} && $trafficsettings{'PERIPSTATS'} eq 'on') {
		for(keys %internal_netaddress) {
			
			iptables("-A $POSTR -m connmark --mark $mark -j ACCOUNT --addr $internal_netaddress{$_}/$internal_netmask{$_} --tname ${_}_$name");
		}
	}
}
# going around again
# could put in code to handle special rules - handling based on rule name
for my $rule (@rules) {
	my ($name,$mark, $port, $class) = @{$rule}{qw/name connmark port class/};
	# we are doing this on
	$connmarks{$name} = $mark;
	$connmark_to_class{$mark} = $class;
	if($name eq 'Peer_to_Peer') {
		# leverage the ip2p iptables module, matches all p2p protocols known to ipp2p
		iptables("-A $POSTR -m ipp2p --ipp2p -j CONNMARK --set-mark $mark -i $external_interface");
		iptables("-A $POSTR -m ipp2p --ipp2p -j CONNMARK --set-mark $mark -o $external_interface");
	}
	if($name eq 'Voice_Over_IP') {
		# also assume EF diffserv mark set wants to
		# be treated as if it were VOIP
		iptables("-A $POSTR -m dscp --dscp-class EF -j CONNMARK --set-mark $mark");
		
	}
	if($name eq 'VPN') {
		# no ports involved - just shape protocols 50 and 51 (ipsec)
		iptables("-A $POSTR -j CONNMARK --set-mark $mark --protocol $_") for 50..51;
	}
	# other fancy rules can go here
}
	

# MUST BE LAST! special rule for smallpkt - nothing to do with connections
# will attach to any small packet that has not been associated with a connection already...
iptables("-A $POSTR -m connmark --mark 0 -j CLASSIFY --set-class 1:$classids{'smallpkt'} --match length --length 1:110") if defined $classids{'smallpkt'};

# now jump to traf-tot table for connmark to class processing (and statistics gathering)
# uploads qued on external interface
iptables("-A $POSTR -j ${external_interface}-up-traf-tot -o $external_interface");
# downloads queued on each internal one
for my $if (@internal_interface) {
	iptables("-A $POSTR -j ${external_interface}-dn-traf-tot -o $if");
}

# now get to assign connmarks into classes, and do connmark for default as last
for my $cm (sort keys %connmark_to_class, 0) {
	next if $cm == 0;
	my $class = $classids{$connmark_to_class{$cm}} || $classids{$default_traffic} ;
	push @rulenumbers, $cm;
	iptables("-A ${external_interface}-up-traf-tot -m connmark --mark $cm -j CLASSIFY --set-class 1:$class -o $external_interface");
	
	for my $if (@internal_interface) {
		iptables("-A ${external_interface}-dn-traf-tot -m connmark --mark $cm -j CLASSIFY --set-class 1:$class -o $if");
	}
}

# so can use trafficmon etc. - SmoothTraffic compatible descriptions of what is in iptables
writesettings();
exit(0);
# end of main program....

# wrappers to eliminate repeated typing
sub tcqdisc {
	my $args = shift;
	system(split(/\s+/,'/usr/sbin/tc qdisc add dev ' . $args));
	print "/usr/sbin/tc qdisc add dev $args\n" if $? != 0;
}

sub tcclass {
	my $args = shift;
	system(split(/\s+/,'/usr/sbin/tc class add dev ' . $args));
	print "/usr/sbin/tc class add dev $args\n" if $? != 0;
}

sub stdclass {
	my($iface, $tag, $extra, $ratehash, $ceilhash) = @_;
	$extra = 'quantum 1500' unless defined $extra;
	next if $tag eq 'none';

    tcclass("$iface parent 1:$classids{'all'} classid 1:$classids{$tag} htb rate " . $ratehash->{$tag} ."bps ceil " . $ceilhash->{$tag} ."bps prio $prio{$tag} $extra");
	tcqdisc("$iface parent 1:$classids{$tag} handle $classids{$tag}: sfq perturb 10");
}
	

sub iptables {
	my $args = shift;
	# print "iptables -t mangle $args\n";
	system(split(/\s+/,'/usr/sbin/iptables -t mangle ' . $args));
	print "iptables -t mangle $args\n" if $? != 0;
}

# clearing out traffic
sub removetraffic {
	for(qw/postrouting forward output/) {
		iptables("-F traffic$_");
	}
	for my $if ($external_interface) {
		for my $dir (qw/up dn/) { 
			iptables("-F ${if}-${dir}-traf-tot");
			iptables("-X ${if}-${dir}-traf-tot");
		}
		# and axe the qdiscs
		system(split(/\s+/,"/usr/sbin/tc qdisc del root dev $if"));
	}
	for my $if (@internal_interface) {
		system(split(/\s+/,"/usr/sbin/tc qdisc del root dev $if"));
	}
}

# this is needed to make trafficmon and trafficlogger pick up per rule info
sub writesettings {
	my $settingsdir = '/var/smoothwall/traffic';
    # chosen_speeds
	if(open(FD, ">$settingsdir/chosen_speeds")) {
		print FD "red_download=${download_speed}bps\n";
		print FD "red_upload=${upload_speed}bps\n";
		for(@internal_interface) {
			print FD "$_=${internal_speed}bps\n";
		}
		close(FD);
	}
	# classid to classname lookup
	if(open(FD, ">$settingsdir/classnames")) {
		for(sort keys %classids) {
			next if $_ eq 'none';
			print FD "1:$classids{$_}=$_\n";
		}
		close(FD);
	}
	# connmark to rulename lookup
	if(open(FD, ">$settingsdir/rulenames")) {
		print FD "0=DEFAULT\n";
		for(sort keys %connmarks) {
			print FD "$connmarks{$_}=$_\n";
		}
		close(FD);
	}
	# position in traf-tot to connmark map
	if(open(FD, ">$settingsdir/rulenumbers")) {
		for(my $i = 0; $i < scalar(@rulenumbers); $i++) {
			print FD "$i=$rulenumbers[$i]\n";
		}
		close(FD);
	}
	# order classes should be presented
	if(open(FD, ">$settingsdir/classsortorder")) {
		for(my $i = 0; $i < scalar(@classsortorder); $i++) {
			print FD "$i=$classsortorder[$i]\n";
		}
		close(FD);
	}
	# connmark to class lookup
	if(open(FD, ">$settingsdir/rule2class")) {
		print FD "0=DEFAULT\n";
		for(sort keys %connmark_to_class) {
			my $class = $classids{$connmark_to_class{$_}};
			print FD "$_=1:$class\n";
		}
		close(FD);
	}
	system(split(/\s+/,"/bin/chown -R nobody:nobody $settingsdir"));

}
# Tests that the given parameter is up by using the SIOCGIFFLAGS ioctl on a socket.
# Yucky bit of hard coding here but these things are not likley to change.

sub isup {
	my $dev = shift;
	my $format = "a16S"; # 16 char interface name and short flags
	my $IFF_UP = 1;
	my $SIOCGIFFLAGS = 0x8913;
	my $ioctlarg = pack($format, $dev, 0);
	local (*SOCK);
	
	socket(SOCK, AF_UNIX, SOCK_STREAM, 0) or die "no socket";
	ioctl(SOCK, 0x8913, $ioctlarg);
	close(SOCK);
	
	my ($devname, $flag) = unpack("a16S", $ioctlarg);
	return $flag & $IFF_UP;
}

# returns list of interfaces which are up
sub up_interfaces {
	return grep(isup($_), @_);
}

