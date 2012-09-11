#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );

my %proxysettings;
my %netsettings;
my %mainsettings;

&readhash("${swroot}/ethernet/settings", \%netsettings);
&readhash("${swroot}/main/settings", \%mainsettings);

&showhttpheaders();

$proxysettings{'ACTION'} = '';
$proxysettings{'VALID'} = '';

$proxysettings{'UPSTREAM_PROXY'} = '';
$proxysettings{'ENABLE'} = 'off';
$proxysettings{'CACHE_SIZE'} = '500';
$proxysettings{'TRANSPARENT'} = 'off';
$proxysettings{'MAX_SIZE'} = '4096';
$proxysettings{'MIN_SIZE'} = '0';
$proxysettings{'MAX_OUTGOING_SIZE'} = '0';
$proxysettings{'MAX_INCOMING_SIZE'} = '0';

&getcgihash(\%proxysettings);

my $needhup = 0;
my $errormessage = '';
if ($proxysettings{'ACTION'} eq $tr{'save'} ||
	$proxysettings{'ACTION'} eq $tr{'save and restart with cleared cache'})
{ 
	if (!($proxysettings{'CACHE_SIZE'} =~ /^\d+/) ||
		($proxysettings{'CACHE_SIZE'} < 10))
	{
		$errormessage = $tr{'invalid cache size'};
		goto ERROR;
	}		
	if (!($proxysettings{'MAX_SIZE'} =~ /^\d+/))
	{
		$errormessage = $tr{'invalid maximum object size'};
		goto ERROR;
	}
	if (!($proxysettings{'MIN_SIZE'} =~ /^\d+/))
	{
		$errormessage = $tr{'invalid minimum object size'};
		goto ERROR;
	}
	if (!($proxysettings{'MAX_OUTGOING_SIZE'} =~ /^\d+/))
	{
		$errormessage = $tr{'invalid maximum outgoing size'};
		goto ERROR;
	}
	if (!($proxysettings{'MAX_INCOMING_SIZE'} =~ /^\d+/))
	{
		$errormessage = $tr{'invalid maximum incoming size'};
		goto ERROR;
	}
	if ($proxysettings{'PEER_USERNAME'})
	{
		unless ($proxysettings{'PEER_PASSWORD'})
		{
			$errormessage = $tr{'password cant be blank'};
			goto ERROR;
		}
	}

ERROR:
	if ($errormessage) {
		$proxysettings{'VALID'} = 'no'; }
	else {
              	$proxysettings{'VALID'} = 'yes'; }

	&writehash("${swroot}/proxy/settings", \%proxysettings);
	if ($proxysettings{'VALID'} eq 'yes')
	{
		system('/usr/bin/smoothwall/writeproxy.pl');
        	
        	my @args = ('squidrestart');
        	if ($proxysettings{'ACTION'} eq $tr{'save and restart with cleared cache'}) {
			push(@args, '--clearcache'); }
        	
		my $success = message(@args);
		
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }

		message('setoutgoing');
	}
}

&readhash("${swroot}/proxy/settings", \%proxysettings);

my %checked;

$checked{'ENABLE'}{'off'} = '';
$checked{'ENABLE'}{'on'} = '';
$checked{'ENABLE'}{$proxysettings{'ENABLE'}} = 'CHECKED';

$checked{'TRANSPARENT'}{'off'} = '';
$checked{'TRANSPARENT'}{'on'} = '';
$checked{'TRANSPARENT'}{$proxysettings{'TRANSPARENT'}} = 'CHECKED';

&openpage($tr{'web proxy configuration'}, 1, '', 'services');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'web proxyc'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='25%' CLASS='base'>$tr{'cache size'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='text' NAME='CACHE_SIZE' VALUE='$proxysettings{'CACHE_SIZE'}' SIZE='5'></TD>
	<TD WIDTH='25%' CLASS='base'><IMG SRC='/ui/img/blob.gif'>&nbsp;$tr{'remote proxy'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='text' NAME='UPSTREAM_PROXY' VALUE='$proxysettings{'UPSTREAM_PROXY'}'></TD>
</TR>
<TR>
	<TD CLASS='base'><IMG SRC='/ui/img/blob.gif'>&nbsp;$tr{'remote proxy username'}</TD>
	<TD><INPUT TYPE='text' NAME='PEER_USERNAME' VALUE='$proxysettings{'PEER_USERNAME'}'></TD>
	<TD CLASS='base'>$tr{'remote proxy password'}</TD>
	<TD><INPUT TYPE='password' NAME='PEER_PASSWORD' VALUE='$proxysettings{'PEER_PASSWORD'}'></TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'max size'}</TD>
	<TD><INPUT TYPE='text' NAME='MAX_SIZE' VALUE='$proxysettings{'MAX_SIZE'}' SIZE='5'></TD>
	<TD CLASS='base'>$tr{'min size'}</TD>
	<TD><INPUT TYPE='text' NAME='MIN_SIZE' VALUE='$proxysettings{'MIN_SIZE'}' SIZE='5'></TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'max outgoing size'}</TD>
	<TD><INPUT TYPE='text' NAME='MAX_OUTGOING_SIZE' VALUE='$proxysettings{'MAX_OUTGOING_SIZE'}' SIZE='5'></TD>
	<TD CLASS='base'>$tr{'max incoming size'}</TD>
	<TD><INPUT TYPE='text' NAME='MAX_INCOMING_SIZE' VALUE='$proxysettings{'MAX_INCOMING_SIZE'}' SIZE='5'></TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'transparent'}</TD>
	<TD><INPUT TYPE='checkbox' NAME='TRANSPARENT' $checked{'TRANSPARENT'}{'on'}></TD>
	<TD CLASS='base'>$tr{'enabled'}</TD>
	<TD><INPUT TYPE='checkbox' NAME='ENABLE' $checked{'ENABLE'}{'on'}></TD>
</TR>
</TABLE>
<BR>
<IMG SRC='/ui/img/blob.gif' VALIGN='top'>&nbsp;
<FONT CLASS='base'>$tr{'these fields may be blank'}</FONT>
END
;
&closebox();


print <<END
<DIV ALIGN='CENTER'>
<TABLE WIDTH='60%'>
<TR>
        <TD WIDTH='50%' ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE=
'$tr{'save'}'></TD>
        <TD WIDTH='50%' ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE=
'$tr{'save and restart with cleared cache'}'></TD>
</TR>
</TABLE>
</DIV>
END
;

print "</FORM>\n";

&alertbox('add', 'add');

&closebigbox();

&closepage();
