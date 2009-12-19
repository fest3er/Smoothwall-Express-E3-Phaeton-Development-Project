#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );

use Socket;

my %cgiparams;
my @addrs; my @vars;
my $addr;
my $var;
my $hostname;

&getcgihash(\%cgiparams);

&showhttpheaders();

if ($ENV{'QUERY_STRING'} && $cgiparams{'ACTION'} eq '')
{
	@vars = split(/\&/, $ENV{'QUERY_STRING'});
	$cgiparams{'IP'} = '';
	foreach $_ (@vars)
	{
		($var, $addr) = split(/\=/);
		if ($var eq 'ip')
		{
			$cgiparams{'IP'} .= "$addr,";
			push(@addrs, $addr);
		} elsif ( $var eq "MODE" ){
			$cgiparams{'MODE'} = $addr;
		}
	}
	$cgiparams{'ACTION'} = 'Run';
}
else {
	@addrs = split(/,/, $cgiparams{'IP'});  }

if ( $cgiparams{'MODE'} ne "quick" ){
	&openpage($tr{'ip info'}, 1, '', 'tools');

	&openbigbox('100%', 'LEFT');

	&alertbox($errormessage);

	print "<FORM METHOD='POST'>\n";

	&openbox($tr{'whois lookupc'});

	print <<END
<TABLE WIDTH='100%'>
<TR>
<TD WIDTH='20%' CLASS='base'>$tr{'ip addresses or domain names'}</TD>
<TD WIDTH='65%'><INPUT TYPE='text' SIZE='60' NAME='IP' VALUE='$cgiparams{'IP'}'></TD>
<TD WIDTH='15%' ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'run'}'></TD>
</TR>
</TABLE>
END
;

	&closebox();

	if ($cgiparams{'ACTION'} eq $tr{'run'})
	{
		unless ($errormessage)
		{
			foreach $addr (@addrs)
			{
	        		$hostname = gethostbyaddr(inet_aton($addr), AF_INET);
        			if (!$hostname) { $hostname = $tr{'lookup failed'}; }
				&openbox("$addr ($hostname)");
				print "<PRE>\n";
				system('/usr/bin/whois', '--nocgi', $addr);
				print "</PRE>\n";
				&closebox();
			}
		}	
	}

	print "</FORM>\n";

	&alertbox('add','add');

	&closebigbox();

	&closepage();
}
else
{
	unless ($errormessage)
	{
		foreach $addr (@addrs)
		{
			$hostname = gethostbyaddr(inet_aton($addr), AF_INET);
			if (!$hostname) { $hostname = $tr{'lookup failed'}; }
			&openbox("$addr ($hostname)");
			print "<div style='height: 140px; width: 400px; overflow: auto;'><pre style='font-size: 9px;'>";
			system('/usr/bin/whois', '--nocgi', $addr);
			print "</pre></div>";
			&closebox();
		}
	}	
}
