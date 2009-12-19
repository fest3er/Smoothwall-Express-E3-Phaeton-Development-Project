#!/usr/bin/perl
#
# coded by Martin Pot 2003
# http://martybugs.net/smoothwall/rrdtool.cgi
#
# SmoothWall scripts
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
# rrdtool.pl

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %netsettings;
my %pppsettings;

&readhash("${swroot}/ppp/settings", \%pppsettings);
&readhash("${swroot}/ethernet/settings", \%netsettings);

# define location of rrdtool binary
my $rrdtool = '/usr/bin/rrdtool';
# define location of rrdtool databases
my $rrd = '/var/lib/rrd';
# define location of images
my $img = '/httpd/html/rrdtool';

# process data for GREEN interface
&ProcessInterface("green", $netsettings{'GREEN_DEV'});

# check for ORANGE interface
if ($netsettings{'ORANGE_DEV'})
{
	&ProcessInterface("orange", $netsettings{'ORANGE_DEV'});
}

# check for PURPLE interface
if ($netsettings{'PURPLE_DEV'})
{
	&ProcessInterface("purple", $netsettings{'PURPLE_DEV'});
}

# process data for RED interface
if ($netsettings{'RED_DEV'} && $pppsettings{'COMPORT'} eq '') 
{
	&ProcessInterface("red", $netsettings{'RED_DEV'});
}
elsif ($pppsettings{'COMPORT'} =~ /^tty/ ||
        $pppsettings{'COMPORT'} =~ /^adsl/ ||
        $pppsettings{'COMPORT'} =~ /^pppoe/)
{
	&ProcessInterface("red", "ppp0");
}
elsif ($pppsettings{'COMPORT'} =~ /^isdn/) 
{
	&ProcessInterface("red", "ippp0");
}

sub ProcessInterface
{
# process interface
# inputs: $_[0]: interface name (ie, green/orange/red)
#         $_[1]: interface (ie, eth0/eth1/eth2/ppp0)

	# get network interface info
	my $in = `ifconfig $_[1] |grep bytes|cut -d":" -f2|cut -d" " -f1`;
	my $out = `ifconfig $_[1] |grep bytes|cut -d":" -f3|cut -d" " -f1`;

	# remove eol chars
	chomp($in);
	chomp($out);

	print "$_[0] ($_[1]) traffic in, out: $in, $out\n";

	# if rrdtool database doesn't exist, create it
	if (! -e "$rrd/$_[0].rrd")
	{
		print "creating rrd database for $_[0] interface ($_[1])...\n";
		system("$rrdtool create $rrd/$_[0].rrd -s 300"
			." DS:in:DERIVE:600:0:12500000"
			." DS:out:DERIVE:600:0:12500000"
			." RRA:AVERAGE:0.5:1:576"
			." RRA:AVERAGE:0.5:6:672"
			." RRA:AVERAGE:0.5:24:732"
			." RRA:AVERAGE:0.5:144:1460");
	}

	# insert values into rrd
	`$rrdtool update $rrd/$_[0].rrd -t in:out N:$in:$out`;

	# create traffic graphs
	&CreatePreviewGraph($_[0], "hour");
	&CreateGraph($_[0], "day");
	&CreatePreviewGraph($_[0], "day");
	&CreateGraph($_[0], "week");
	&CreateGraph($_[0], "month"); 
	&CreateGraph($_[0], "year");
}

sub CreateGraph
{
# creates graph
# inputs: $_[0]: interface name (ie, GREEN, RED, ORANGE)
#	  $_[1]: interval (ie, day, week, month, year)

	system("$rrdtool graph $img/$_[0]-$_[1].png"
		." -s \"-1$_[1]\""
		." -t \"traffic on $_[0] interface over the last $_[1]\""
		." --lazy"
		." -h 100 -w 500"
		." -l 0"
		." -a PNG"
		." -v \"bytes/sec\""
		." DEF:in=$rrd/$_[0].rrd:in:AVERAGE"
		." DEF:out=$rrd/$_[0].rrd:out:AVERAGE"
		." CDEF:out_neg=out,-1,*"
		." AREA:in#FF9900:\"Incoming\""
		." LINE1:in#CC6600"
		." GPRINT:in:MAX:\"  Max\\: %5.1lf %s\""
		." GPRINT:in:AVERAGE:\" Avg\\: %5.1lf %S\""
		." GPRINT:in:LAST:\" Current\\: %5.1lf %Sbytes/sec\\n\""
		." AREA:out_neg#FFCC66:\"Outgoing\""
		." LINE1:out_neg#CC9966"
		." GPRINT:out:MAX:\"  Max\\: %5.1lf %S\""
		." GPRINT:out:AVERAGE:\" Avg\\: %5.1lf %S\""
		." GPRINT:out:LAST:\" Current\\: %5.1lf %Sbytes/sec\""
		." HRULE:0#000000");
}

sub CreatePreviewGraph
{
# creates graph
# inputs: $_[0]: interface name (ie, GREEN, RED, ORANGE)
#	  $_[1]: interval (ie, day, week, month, year)

	system("$rrdtool graph $img/$_[0]-$_[1]_preview.png"
		." -s \"-1$_[1]\""
		." --lazy"
		." -h 40 -w 150"
		." -l 0"
		." -a PNG"
		." -v \"bytes/sec\""
		." DEF:in=$rrd/$_[0].rrd:in:AVERAGE"
		." DEF:out=$rrd/$_[0].rrd:out:AVERAGE"
		." CDEF:out_neg=out,-1,*"
		." AREA:in#FF9900:\"Incoming\""
		." LINE1:in#CC6600"
		." AREA:out_neg#FFCC66:\"Outgoing\""
		." LINE1:out_neg#CC9966"
		." HRULE:0#000000");
}
