#!/usr/bin/perl
#
# SmoothWall CGIs
#
# (c) SmoothWall Ltd, 2005

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );

my %sipsettings;
my %checked; my %selected;

&showhttpheaders();

$sipsettings{'ACTION'} = '';
$sipsettings{'ENABLE'} = 'off';
$sipsettings{'LOG_CALLS'} = 'off';
$sipsettings{'TRANSPARENT'} = 'off';

&getcgihash(\%sipsettings);

my $errormessage = '';
if ($sipsettings{'ACTION'} eq $tr{'save'})
{
ERROR:

	unless ($errormessage)
	{
		&log("SIP service restarted.");
	
		&writehash("${swroot}/sipproxy/settings", \%sipsettings);
		system("/usr/bin/smoothwall/writesiproxdconf.pl");
		my $success = message("siprestart");
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}

if ($sipsettings{'ACTION'} eq '')
{
	$sipsettings{'LOGGING'} = '0';
	$sipsettings{'LOG_CALLS'} = 'on';
	$sipsettings{'CLIENTS'} = '50';
	
	&readhash("${swroot}/sipproxy/settings", \%sipsettings);
}

$checked{'ENABLE'}{'off'} = '';
$checked{'ENABLE'}{'on'} = '';
$checked{'ENABLE'}{$sipsettings{'ENABLE'}} = 'CHECKED';

$checked{'TRANSPARENT'}{'off'} = '';
$checked{'TRANSPARENT'}{'on'} = '';
$checked{'TRANSPARENT'}{$sipsettings{'TRANSPARENT'}} = 'CHECKED';

$checked{'LOG_CALLS'}{'off'} = '';
$checked{'LOG_CALLS'}{'on'} = '';
$checked{'LOG_CALLS'}{$sipsettings{'LOG_CALLS'}} = 'CHECKED';

$selected{'LOGGING'}{'0'} = '';
$selected{'LOGGING'}{'1'} = '';
$selected{'LOGGING'}{'2'} = '';
$selected{'LOGGING'}{$sipsettings{'LOGGING'}} = 'SELECTED';

$selected{'CLIENTS'}{'5'} = '';
$selected{'CLIENTS'}{'10'} = '';
$selected{'CLIENTS'}{'50'} = '';
$selected{'CLIENTS'}{'100'} = '';
$selected{'CLIENTS'}{'200'} = '';
$selected{'CLIENTS'}{$sipsettings{'CLIENTS'}} = 'SELECTED';

&openpage($tr{'sip'}, 1, '', 'services');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'session initiation protocol'});

print <<END
<TABLE WIDTH='100%'>
<TR>
<TD WIDTH='25%' CLASS='base'>$tr{'enabled'}</TD>
<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLE' $checked{'ENABLE'}{'on'}></TD>
<TD CLASS='base'>$tr{'sip logging level'}</TD>
<TD>
<SELECT NAME='LOGGING'>
<OPTION VALUE='0' $selected{'LOGGING'}{'0'}>$tr{'sip normal'}
<OPTION VALUE='1' $selected{'LOGGING'}{'1'}>$tr{'sip detailed'}
<OPTION VALUE='2' $selected{'LOGGING'}{'2'}>$tr{'sip very detailed'}
</SELECT>
</TD>
</TR>
<TR>
<TD CLASS='base'>$tr{'log calls'}</TD>
<TD><INPUT TYPE='checkbox' NAME='LOG_CALLS' $checked{'LOG_CALLS'}{'on'}></TD>
<TD CLASS='base'>$tr{'maximum number of clients'}</TD>
<TD>
<SELECT NAME='CLIENTS'>
<OPTION VALUE='5' $selected{'CLIENTS'}{'5'}>5
<OPTION VALUE='10' $selected{'CLIENTS'}{'10'}>10
<OPTION VALUE='50' $selected{'CLIENTS'}{'50'}>50
<OPTION VALUE='100' $selected{'CLIENTS'}{'100'}>100
<OPTION VALUE='200' $selected{'CLIENTS'}{'200'}>200
</SELECT>
</TD>
</TR>
<TR>
<TD CLASS='base'>$tr{'sip transparent'}</TD>
<TD><INPUT TYPE='checkbox' NAME='TRANSPARENT' $checked{'TRANSPARENT'}{'on'}></TD>
<TD>&nbsp;</TD>
<TD>&nbsp;</TD>
</TR>
</TABLE>
END
;

&closebox();

print <<END
<DIV ALIGN='CENTER'>
<TABLE WIDTH='60%'>
<TR>
	<TD ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'save'}'></TD>
</TR>
</TABLE>
</DIV>
END
;

print "</FORM>\n";

&alertbox('add', 'add');

&closebigbox();

&closepage();
