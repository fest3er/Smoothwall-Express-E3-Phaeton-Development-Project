#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) SmoothWall Ltd, 2002

use lib "/usr/lib/smoothwall";
use header qw( :standard );

use Socket;

my %cgiparams;
my @inaddrs, @addrs;
my $addr;
my $address;
my $errormessage = '';

&getcgihash(\%cgiparams);

&showhttpheaders();

$selected{'TOOL'}{'PING'} = '';
$selected{'TOOL'}{'TRACEROUTE'} = '';
$selected{'TOOL'}{$cgiparams{'TOOL'}} = 'SELECTED';

&openpage($tr{'network utilities'}, 1, '', 'tools');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'select tool'});

print <<END
<TABLE WIDTH='100%'>
<TR>
<TD WIDTH='15%' CLASS='base'>$tr{'toolc'}</TD>
<TD WIDTH='20%'>
<SELECT NAME='TOOL'>
<OPTION VALUE='PING' $selected{'TOOL'}{'PING'}>Ping
<OPTION VALUE='TRACEROUTE' $selected{'TOOL'}{'TRACEROUTE'}>Traceroute
</SELECT>
</TD>
<TD WIDTH='20%' CLASS='base'>$tr{'ip addresses or hostnames'}</TD>
<TD WIDTH='30%'><INPUT TYPE='text' SIZE='40' NAME='IP' VALUE='$cgiparams{'IP'}'></TD>
<TD WIDTH='15%' ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'run'}'></TD>
</TR>
</TABLE>
END
;

&closebox();

if ($cgiparams{'ACTION'} eq $tr{'run'})
{
	@inaddrs = split(/,/, $cgiparams{'IP'});

	foreach $addr (@inaddrs)
	{
		if (&validip($addr)) {
			push @addrs, $addr; }
		else
		{
			if ($address = gethostbyname($addr)) {
				push @addrs, inet_ntoa($address); }
			else {
				$errormessage = "$tr{'could not resolve'} $addr"; }
		}		
	}

	unless ($errormessage)
	{
		foreach $addr (@addrs)
		{
			$iaddr = inet_aton($addr);
			$hostname = gethostbyaddr($iaddr, AF_INET);
			if (!$hostname) { $hostname = $tr{'lookup failed'}; }

			&openbox("${addr} (${hostname})");
			print "<PRE>\n";
			if ($cgiparams{'TOOL'} eq 'PING') {
				system('/usr/bin/ping', '-n', '-c', '5', $addr); }
			elsif ($cgiparams{'TOOL'} eq 'TRACEROUTE') {
				system('/bin/traceroute', '-n', $addr); }
			print "</PRE>\n";
			&closebox();
		}
	}
}

print "</FORM>\n";

&alertbox('add','add');

&closebigbox();

&closepage();
