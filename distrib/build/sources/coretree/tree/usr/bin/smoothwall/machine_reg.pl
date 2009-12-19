#!/usr/bin/perl

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my (%eth,%isdn,%pppsettings);

## url length limit is 8192
## subtract the chars required for the URI, what we have left is
my $url_limit = 8124;

# detail connection details, this is how the smoothie connects to the
# outside world.  We have no interest in passwords or anything of that
# ilk of course, but knowing modem types and drivers is always good
# for support purposes.

# determine the Ethernet Settings

&readhash("${swroot}/ethernet/settings", \%eth);

# And the ISDN Settings (if enabled)

&readhash("${swroot}/isdn/settings", \%isdn);

# PPP Settings

&readhash("${swroot}/ppp/settings", \%pppsettings);

# ADSL Settings

&readhash("${swroot}/adsl/settings", \%adslsettings);

# Retrieve details about the CPU, again this is for general purpose
# information about what to support (can we finally drop i386 support
# and turn on some optimisations ? )

open(CPU, "/proc/cpuinfo") or die "Could not open /proc/cpuinfo";
my ($junk,$mhz,$model,$vid);
while(<CPU>)
{
	if($_ =~ m/^cpu MH/) { ($junk,$mhz) = split(/\:/,$_,-1); }
	if($_ =~ m/^vendor_id/) { ($junk,$vid) = split(/\:/,$_,-1); }
	if($_ =~ m/^model\sna/) { ($junk,$model) = split(/\:/,$_,-1); }
}
close(CPU);

$mhz =~ s/^\s+//;
$mhz =~ s/\s+$//;
$vid =~ s/^\s+//;
$vid =~ s/\s+$//;
$model =~ s/^\s+//;
$model =~ s/\s+$//;

# Detail memory use and amounts on the machine.

open(MEM, "/proc/meminfo") or die "Could not open /proc/meminfo";
my ($lp);
while(<MEM>)
{
	if($_ =~ m/^MemTotal/) { ($junk,$lp) = split(/\:/,$_,-1); }
}
close(MEM);
$lp =~ s/^\s+//;
$lp =~ s/\s+$//;
my $mem;
($mem,$junk) = split(/\s/,$lp);

# Partitioning information, should we start worrying about logging
# and come up with new methods of storing it etc..

open(DISK, "/proc/partitions") or die "Could not open /proc/partitions";
my (@this,$disk);
while(<DISK>)
{
	my @test = split(/\s/,$_,-1);
	undef @this;
	my $item;	
	foreach $item (@test)
	{
		unless($item eq "") { push(@this,$item); }
		if($this[3] eq "hda") { $disk = $this[2]; }
	}
}
close(DISK);

# discover some LSPCI information, this will give us an indication of the sort
# of hardware we're coming up against and the sort of drivers we need to 
# support.
my $lspci;

open(PIPE, '-|') || exec( '/usr/sbin/lspci' );
while ( my $line = <PIPE>) { 
	chomp $line;
	my ( $busid, $type, $name ) = ( $line =~ /([^\s]+)\s+([^:]+):\s+(.*)/ );
	$lspci .= "$busid|$type|$name||";
}
close(PIPE);

# discover various interesting things about the USB Bus, this is in a 
# perpetual state of flux and a widish range of details can only serve
# to make sense of some of it.

my $usbbus;

if (open(USB, "/proc/bus/usb/devices"))
{
	while( my $line = <USB>)
	{
		chomp $line;
		$line =~s/#//g;
		$usbbus .= "$line|";
	}
	close(USB);
}

# construct the regular information
my $nonextra = join('&', 
	'cpu_vid=' . &_urlencode($vid),
	'cpu_model=' . &_urlencode($model),
	'cpu_mhz=' . &_urlencode($mhz),
	'mem=' . &_urlencode($mem),
	'hdd=' . &_urlencode($disk),
	'inst_type=' . &_urlencode($eth{'CONFIG_TYPE'}),
	'isdn=' . &_urlencode($pppsettings{'COMPORT'}),
	'version=' . &_urlencode("$version"));

# construct the additional information.
my $extra = join('&', 
	'ADSL_DEVICE=' . &_urlencode($adslsettings{'DEVICE'}),
	'ADSL_ECITYPE=' . &_urlencode($adslsettings{'ECITYPE'}),
	'ISDN_TYPE=' . &_urlencode($isdnsettings{'TYPE'}),
	'LSPCI=' . &_urlencode($lspci),
	'USBBUS=' . &_urlencode($usbbus));

my $info = join('&', $nonextra, $extra);
my $length = length($info);

## truncate the info if needed
if($length > $url_limit) { $info = substr($info, 0, $url_limit); }

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

use IO::Socket;
$sock = new IO::Socket::INET ( PeerAddr => $host, PeerPort => $port, Proto => 'tcp', Timeout => 5 ) or die "Could not connect to host\n\n";
print $sock "GET http://$xhost/cgi-bin/system_register.cgi?$info HTTP/1.1\n";
print $sock "Host: $xhost\n\n";
undef $/;
$retsrt = <$sock>;
close $sock;

@page = split(/\n/,$retsrt,-1);
$found = 0;

my $id;

foreach(@page)
{
	if($_ =~ m/^status/)
	{
		@temp = split(/\=/,$_,2);
		$found = 1;
	}
	if($_ =~ m/^id/)
	{
		( $junk, $id ) = split(/\=/,$_,2);
	}
}


&readhash("/var/smoothwall/main/ownership", \%ownership);
$ownership{'ID'} = $id;
$ownership{'REGISTERED'} = time();
&writehash("/var/smoothwall/main/ownership", \%ownership);


if ($found == 1)
{
	if ($temp[1] =~ /^success/) {
		print STDERR "success with id $id\n";
		exit 0; 
	} else {
		exit 1; 
	}
} else {
	exit 2; 
}

sub _urlencode
{
	my ($string) = @_;
	$string =~ s/\%/%25/g;
	$string =~ s/\s/%20/g;
	$string =~ s/\!/%21/g;
	$string =~ s/\*/%2a/g;
	$string =~ s/\'/%27/g;
	$string =~ s/\(/%28/g;
	$string =~ s/\)/%29/g;
	$string =~ s/\;/%3b/g;
	$string =~ s/\:/%3a/g;
	$string =~ s/\@/%40/g;
	$string =~ s/\&/%26/g;
	$string =~ s/\=/%3d/g;
	$string =~ s/\+/%2b/g;
	$string =~ s/\$/%24/g;
	$string =~ s/\,/%2c/g;
	$string =~ s/\//%2f/g;
	$string =~ s/\?/%3f/g;
	$string =~ s/\#/%23/g;
	$string =~ s/\[/%5b/g;
	$string =~ s/\]/%5d/g;
	return $string;
}
