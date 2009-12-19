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

my (%remotesettings, %checked, $errormessage);

&showhttpheaders();

$remotesettings{'ENABLE_SSH'} = 'off';
$remotesettings{'ENABLE_SECURE_ADMIN'} = 'off';
$remotesettings{'ACTION'} = '';
&getcgihash(\%remotesettings);

$errormessage = '';
if ($remotesettings{'ACTION'} eq $tr{'save'})
{
	&writehash("${swroot}/remote/settings", \%remotesettings);

	if ($remotesettings{'ENABLE_SSH'} eq 'on') {
		&log($tr{'ssh is enabled'}); }
	else {
		&log($tr{'ssh is disabled'}); }

	my $success = message('sshdrestart');
	
	if (not defined $success) {
		$errormessage = $tr{'smoothd failure'}; }
}

$remotesettings{'ENABLE_SECURE_ADMIN'} = 'off';
&readhash("${swroot}/remote/settings", \%remotesettings);

$checked{'ENABLE_SSH'}{'off'} = '';
$checked{'ENABLE_SSH'}{'on'} = '';
$checked{'ENABLE_SSH'}{$remotesettings{'ENABLE_SSH'}} = 'CHECKED';

$checked{'ENABLE_SECURE_ADMIN'}{'off'} = '';
$checked{'ENABLE_SECURE_ADMIN'}{'on'} = '';
$checked{'ENABLE_SECURE_ADMIN'}{$remotesettings{'ENABLE_SECURE_ADMIN'}} = 'CHECKED';

&openpage($tr{'remote access'}, 1, '', 'services');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'remote access2'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='25%' CLASS='base'>SSH:</TD>
	<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLE_SSH' $checked{'ENABLE_SSH'}{'on'}></TD>
	<TD WIDTH='25%' CLASS='base'>$tr{'secure admin'}<IMG SRC='/ui/img/blob.gif'></TD>
	<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLE_SECURE_ADMIN' $checked{'ENABLE_SECURE_ADMIN'}{'on'}></TD>
</TR>
</TABLE>
<BR>
<IMG SRC='/ui/img/blob.gif' VALIGN='top'>&nbsp;
<FONT CLASS='base'>$tr{'secure admin long'}</FONT>
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
