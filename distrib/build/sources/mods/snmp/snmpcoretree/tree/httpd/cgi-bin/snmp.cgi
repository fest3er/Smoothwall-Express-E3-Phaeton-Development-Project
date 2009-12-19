#!/usr/bin/perl
#
# SmoothWall CGIs
#
# (c) SmoothWall Ltd, 2005

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );

my %snmpsettings;
my %checked; my %selected;

&showhttpheaders();

$snmpsettings{'ACTION'} = '';
$snmpsettings{'ENABLE'} = 'off';
$snmpsettings{'COMMUNITY'} = '';

&getcgihash(\%snmpsettings);

my $errormessage = '';
if ($snmpsettings{'ACTION'} eq $tr{'save'})
{
ERROR:

	unless ($errormessage)
	{
		&log("SNMP service restarted.");
	
		&writehash("${swroot}/snmp/settings", \%snmpsettings);
		system("/usr/bin/smoothwall/writesnmpconf.pl");
		my $success = message("snmprestart");
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}

if ($snmpsettings{'ACTION'} eq '')
{
	$snmpsettings{'ENABLE'} = 'off';
	$snmpsettings{'COMMUNITY'} = 'public';
	
	&readhash("${swroot}/snmp/settings", \%snmpsettings);
}

$checked{'ENABLE'}{'off'} = '';
$checked{'ENABLE'}{'on'} = '';
$checked{'ENABLE'}{$snmpsettings{'ENABLE'}} = 'CHECKED';

&openpage('snmp', 1, '', 'services');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox('SNMP:');

print <<END
<TABLE WIDTH='100%'>
<TR>
<TD WIDTH='25%' CLASS='base'>$tr{'enabled'}</TD>
<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLE' $checked{'ENABLE'}{'on'}></TD>
<TD CLASS='base'>Community:</TD>
<TD WIDTH='25%'><INPUT TYPE='text' NAME='COMMUNITY' VALUE='$snmpsettings{'COMMUNITY'}'></TD>
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
