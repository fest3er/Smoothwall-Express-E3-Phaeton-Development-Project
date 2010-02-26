#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothnet qw( checkmd5 );

my %pppsettings;
my %modemsettings;
my %netsettings;

my $locks = scalar(glob("/var/run/ppp-*.pid"));

my $cgiparams;

&showhttpheaders();

&getcgihash(\%cgiparams);

$pppsettings{'VALID'} = '';
$pppsettings{'PROFILENAME'} = 'None';
&readhash("${swroot}/ppp/settings", \%pppsettings);
&readhash("${swroot}/modem/settings", \%modemsettings);
&readhash("${swroot}/ethernet/settings", \%netsettings);

if ($pppsettings{'COMPORT'} =~ /^tty/)
{
	if ($locks)
	{
		if (-e "${swroot}/red/active")
		{
			$timestr = &age("${swroot}/red/active");
			$connstate = "$tr{'connected'} (<FONT COLOR='#b04040'>$timestr</FONT>)"; 
		}
		else
		{
			if (-e "${swroot}/red/dial-on-demand")
			{
				$refresh = "<META HTTP-EQUIV='refresh' CONTENT='30;'>";
				$connstate = $tr{'modem dod waiting'};
			}
			else
			{
				$refresh = "<META HTTP-EQUIV='refresh' CONTENT='5;'>";
				$connstate = $tr{'dialing'};
			}
		}
	}
	else {
		$connstate = $tr{'modem idle'};
	 }
}
elsif ($pppsettings{'COMPORT'} =~ /^isdn/) 
{
	$channels = &countisdnchannels();
	if ($channels == 0) {
		$number = 'none!'; }
	elsif ($channels == 1) {
		$number = 'single'; }
	else {
		$number = 'dual'; }
		
	if (-e "${swroot}/red/active")
	{
		$timestr = &age("${swroot}/red/active");
		$connstate = "$tr{'connected'} - $number channel (<FONT COLOR='#b04040'>$timestr</FONT>)";
 	}
	else 
	{
		if ($channels == 0)
		{
			if (-e "${swroot}/red/dial-on-demand")
			{
				$connstate = $tr{'isdn dod waiting'};
				$refresh = "<META HTTP-EQUIV='refresh' CONTENT='30;'>"
			} else {
				$connstate = $tr{'isdn idle'};
			}
		}
		else
		{
			$connstate = $tr{'dialing'};
                 	$refresh = "<META HTTP-EQUIV='refresh' CONTENT='5;'>";
		}
	}
}
elsif ($pppsettings{'COMPORT'} eq 'pppoe')
{
	if (-e "${swroot}/red/active" )
	{
		$timestr = &age("${swroot}/red/active");
		$connstate = "$tr{'connected'} (<FONT COLOR='#b04040'>$timestr</FONT>)";
	}
	else
	{
		if ($locks)
		{
			$connstate = $tr{'dialing'};
			$refresh = "<META HTTP-EQUIV='refresh' CONTENT='5;'>"
		}
		else
		{
			$connstate = $tr{'pppoe idle'};
		}
	}
}
else
{
	if (-e "${swroot}/red/active" )
	{
		$timestr = &age("${swroot}/red/active");
		$connstate = "$tr{'connected'} (<FONT COLOR='#b04040'>$timestr</FONT>)";
	}
	else
	{
		if ($locks)
		{
			$connstate = $tr{'dialing'};
			$refresh = "<META HTTP-EQUIV='refresh' CONTENT='5;'>"
		}
		else
		{
			$connstate = $tr{'adsl idle'};
		}
	}
}

&openpage($tr{'main page'}, 1, $refresh, 'control');

&openbigbox();

&alertbox($errormessage);

my %ownership;
&readhash( "/var/smoothwall/main/ownership", \%ownership );

if ( not defined $ownership{'ADDED_TO_X3'} or $ownership{'ADDED_TO_X3'} eq "0" ){
	&openbox();

	print "<div style='width: 100%; text-align: center;'><a href='/cgi-bin/register.cgi'><img src='/ui/img/frontpage/frontpage.x3.jpg' alt='SmoothWall Express'/></a></div>";
	&closebox();
} else {

	&openbox();
	if(open(LIST, "<${swroot}/banners/available")) {
		my @images;
		while ( my $input = <LIST> ){
			my ( $url, $md5, $link, $alt ) = ( $input =~/([^\|]*)\|([^\|]*)\|([^\|]*)\|(.*)/ );
	
			if ( -e "/httpd/html/ui/img/frontpage/$md5.jpg" and ( &checkmd5( "/httpd/html/ui/img/frontpage/$md5.jpg", $md5) == 1 )){
				push @images, { md5 => $md5, href => $link, alt => $alt };
			}
		}

		if ( scalar( @images ) >= 1 ){
			my $day = (localtime(time))[6];
			my $r = ( $day % scalar(@images) );
			my $image = $images[$r];
			print "<div style='width: 100%; text-align: center;'><a href='$image->{'href'}'><img src='/ui/img/frontpage/$image->{'md5'}.jpg' alt='$image->{'alt'}'/></a></div>";
		} else {
			print "<div style='width: 100%; text-align: center;'><img src='/ui/img/frontpage/frontpage.jpg' alt='SmoothWall Express'/></div>";
		}
	} else {
		print "<div style='width: 100%; text-align: center;'><img src='/ui/img/frontpage/frontpage.jpg' alt='SmoothWall Express'/></div>";
	}

	&closebox();
}


&openbox('');

$currentconnection = &connectedstate();
print <<END
<table class='centered'>
	<tr>
		<td style='text-align: right; vertical-align: top;'><img src='/ui/img/netstatus.$currentconnection.gif' alt='$connpick' style='float: right;'></td>
		<td>&nbsp;</td>
END
;

if (($pppsettings{'COMPORT'} ne '') && (($netsettings{'RED_DEV'} eq "") || ($netsettings{'RED_TYPE'} eq 'PPPOE')))
{
	if ($pppsettings{'VALID'} eq 'yes')
	{
		my $control = qq {
	<table style='width: 100%;'>
	<form method='post' action='/cgi-bin/dial.cgi'>
	<tr>
		<td style='text-align: center;'><input type='submit' name='ACTION' value="$tr{'dial'}"></td>
		<td>&nbsp;&nbsp;</td>
		<td style='text-align: center;'><input type='submit' name='ACTION' value="$tr{'hangup'}"></td>
		<td>&nbsp;&nbsp;</td>
		</form>
		<form method='post'>
		<td style='text-align: center;'><input type='submit' name='ACTION' value="$tr{'refresh'}"></td>
	</tr>
	</form>
</table>
<br/>
<strong>$tr{'current profile'} $pppsettings{'PROFILENAME'}</strong><br/>
$connstate
		};
		&showstats( $control );
	}
	elsif (-e "${swroot}/red/active" )
	{
		my $control = qq {
	<table style='width: 100%;'>
	<tr>
		<td style='text-align: right;'>
<form method='post' action='/cgi-bin/dial.cgi'>
	<input type='submit' name='ACTION' value="$tr{'hangup'}">
</form>
		</td>
	</tr>
	</table>
<td><strong>$tr{'current profile'} $pppsettings{'PROFILENAME'}</strong><br/>
};
		&showstats( $control );
	} elsif ($modemsettings{'VALID'} eq 'no') {
		print "$tr{'modem settings have errors'}\n"; 
	} else {
		print "$tr{'profile has errors'}\n"; 
	}

	print "</td>";
}
else
{
	my $control = qq {
	<table style='width: 100%;'>
	<tr>
		<td style='text-align: left;'>
			
		</td>
	</tr>
	<tr>
		<td style='text-align: left;'>
<form method='post'>
	<input type='submit' name='ACTION' value='$tr{'refresh'}'>
</form>
		</td>
	</tr>
	</table>
};
	&showstats( $control );
}
	print <<END
	</tr>
</table>
END
;

&closebox();

open(AV, "${swroot}/patches/available") or die "Could not open available patches database ($!)";
@av = <AV>;
close(AV);
open(PF, "${swroot}/patches/installed") or die "Could not open installed patches file. ($!)<br>";
while(<PF>)
{
        next if $_ =~ m/^#/;
        @temp = split(/\|/,$_);
        @av = grep(!/^$temp[0]/, @av);
}
close(PF);

if ($#av != -1)
{
	&pageinfo($alertbox{"texterror"}, "$tr{'there are updates'}");
}
$age = &age("/${swroot}/patches/available");
if ($age =~ m/(\d{1,3})d/)
{
	if ($1 >= 7)
	{
		&pageinfo($alertbox{"texterror"}, "$tr{'updates is old1'} $age $tr{'updates is old2'}");
	}
}

print "<br/><table class='blank'><tr><td class='note'>";

system('/usr/bin/uptime');

print "</td></tr></table>\n";

&closebigbox();

&closepage();

sub countisdnchannels
{
	my ($idmap, $chmap, $drmap, $usage, $flags, $phone);
	my @phonenumbers;
	my $count;

	open (FILE, "/dev/isdninfo");

	$idmap = <FILE>; chomp $idmap;
	$chmap = <FILE>; chomp $chmap;
	$drmap = <FILE>; chomp $drmap;
	$usage = <FILE>; chomp $usage;
	$flags = <FILE>; chomp $flags;
	$phone = <FILE>; chomp $phone;

	$phone =~ s/^phone(\s*):(\s*)//;

	@phonenumbers = split / /, $phone;

	$count = 0;
	foreach (@phonenumbers)
	{
 		if ($_ ne '???') {
			$count++; }
	}
	
	return $count;
}

# if we are connected, show some connection details...
sub showstats
{
	my $control = $_[0];
	# determine the name of our red interface.
	my $iface_file;
	
	my( $daystatsin, $daystatsout, $monthstatsin, $monthstatsout, $ratein, $rateout );

	print "<td style='vertical-align: top;'>\n";
	print "<table style='width: 100%; border-collapse: collapse;'>";

	if ( open ( $iface_file, "</var/smoothwall/red/local-ipaddress" )){
		my $ip = <$iface_file>;
		chomp $ip;
		print "<tr><td><strong>Local:</strong></td><td>$ip</td></tr>";
		close $iface_file;
	}

	if ( open ( $iface_file, "</var/smoothwall/red/remote-ipaddress" )){
		my $ip = <$iface_file>;
		chomp $ip;
		print "<tr><td><strong>Remote:</strong></td><td>$ip</td></tr>";
		close $iface_file;
	}


	if ( open ( $iface_file, "</var/smoothwall/red/iface" )){
		my $iface = <$iface_file>;
		chomp $iface;
		close $iface_file;
		
		# interogate the traffic stats
		my %stats;
		&readhash( "/var/log/trafficstats", \%stats );

		$ratein  = $stats{"cur_inc_rate_$iface"};
		$rateout = $stats{"cur_out_rate_$iface"};

		$daystatsin  = $stats{"this_day_inc_total_$iface"};
		$daystatsout = $stats{"this_day_out_total_$iface"};

		$monthstatsin  = $stats{"this_month_inc_total_$iface"};
		$monthstatsout = $stats{"this_month_out_total_$iface"};
	
		# convert the text into kbps / mbps etc
		sub rerange {
			my $number = $_[0];
			my $ret;
			
			if ( $number > (1024*1024*1024) ){
				$ret = sprintf( "%0.1f TB", $number/(1024*1024*1024) );	
			} elsif ( $number > (1024*1024) ){
				$ret = sprintf( "%0.1f GB", $number/(1024*1024) );	
			} elsif ( $number > (1024) ){
				$ret = sprintf( "%0.1f MB", $number/(1024) );	
			} else {
				$ret = sprintf( "%0.1f KB", $number );	
			}
			return $ret;
		}

		sub rerangeb {
			my $number = $_[0];
			my $ret;
			
			if ( $number > (1000*1000*1000) ){
				$ret = sprintf( "%0.1f Gbit/s", $number/(1024*1024*1024) );	
			} elsif ( $number > (1000*1000) ){
				$ret = sprintf( "%0.1f Mbit/s", $number/(1024*1024) );	
			} elsif ( $number > (1000) ){
				$ret = sprintf( "%0.1f Kbit/s", $number/(1024) );	
			} else {
				$ret = sprintf( "%0.1f bit/s", $number );	
			}
			return $ret;
		}

		$ratein        = &rerangeb( $ratein );
		$rateout       = &rerangeb( $rateout );
		$daystatsin    = &rerange( $daystatsin    );
		$daystatsout   = &rerange( $daystatsout   );
		$monthstatsin  = &rerange( $monthstatsin  );
		$monthstatsout = &rerange( $monthstatsout );

		print <<END
		<tr><td>Current:</td><td>$rateout / $ratein (Out / In)</td></tr> 
		<tr><td>Today:</td><td>$daystatsout / $daystatsin (Out / In)</td></tr>
		<tr><td>Month:</td><td>$monthstatsout / $monthstatsin (Out / In)</td></tr>
END
;
	}

	print "</table>\n";
	print "$control</td>\n";

	# we even have a preview graph thingy

	if (-e '/var/smoothwall/red/active' && -e '/httpd/html/rrdtool/red-hour_preview.png' ){
		print "<td>&nbsp;</td><td style='vertical-align: top;'><img src='/rrdtool/red-day_preview.png' alt='traffic'></td>\n";
	}
}
