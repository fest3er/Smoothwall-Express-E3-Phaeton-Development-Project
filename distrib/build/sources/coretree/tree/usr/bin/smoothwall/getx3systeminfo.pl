#!/usr/bin/perl

use lib "/usr/lib/smoothwall";
use header qw( :standard );

# fetch the system id
my $sysid = &getsystemid();

my %proxy;
&readhash("${swroot}/main/proxy", \%proxy);

my $xhost = 'x3.smoothwall.org';

my $host; my $port;
unless ($proxy{'SERVER'})
{
	$host = $xhost;
	$port = 80;
} else {
	$host = $proxy{'SERVER'};
	$port = $proxy{'PORT'};
}

my %ownership;
&readhash("${swroot}/main/ownership", \%ownership);
my $sysid = $ownership{'ID'};
use IO::Socket;
$sock = new IO::Socket::INET ( PeerAddr => $host, PeerPort => $port, Proto => 'tcp', Timeout => 5 ) or die "Could not connect to host\n\n";
print $sock "GET http://$xhost/cgi-bin/system_info.cgi?id=$sysid HTTP/1.1\n";
print $sock "Host: $xhost\n\n";
undef $/;
$retsrt = <$sock>;
close $sock;

@page = split(/\n/,$retsrt,-1);
$found = 0;

foreach(@page)
{
	if($_ =~ m/^status/)
	{
		@temp = split(/\=/,$_,2);
		$found = 1;
	}
	elsif($_ =~ m/^name/)
	{
		( $junk, $name ) = split(/\=/,$_,2);
	}
	elsif($_ =~ m/^added/)
	{
		( $junk, $added ) = split(/\=/,$_,2);
	}
	elsif($_ =~ m/^timestamp/)
	{
		( $junk, $timestamp ) = split(/\=/,$_,2);
	}
}

if($added)
{
	$ownership{'NAME'} = $name;
	$ownership{'ADDED_TO_X3'} = $added;
	$ownership{'ADDED_TO_X3_ON'} = $timestamp;
	&writehash("${swroot}/main/ownership", \%ownership);
}

if ($found == 1)
{
	if ($temp[1] =~ /^success/) {
		print STDERR "got my.SmoothWall system info with id [$sysid]\n";
		exit 0; 
	} else {
		exit 1; 
	}
} else {
	exit 2; 
}
