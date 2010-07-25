#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %cgiparams; 

&showhttpheaders();

$cgiparams{'ACTION'} = '';
$cgiparams{'MENU'} = 'off';

&getcgihash(\%cgiparams);

if ($cgiparams{'ACTION'} eq $tr{'save'})
{
	&writehash("${swroot}/main/uisettings", \%cgiparams);
}

if ($cgiparams{'ACTION'} eq '')
{
	$cgiparams{'MENU'} = 'on';
}

&readhash("${swroot}/main/uisettings", \%cgiparams);

my %checked;

$checked{'MENU'}{$cgiparams{'MENU'}} = " checked";

&openpage( $tr{'preferences'}, 1, '', 'maintenance');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<form method='post'>\n";

&openbox($tr{'user interface'});

print <<END
<table style='width: 100%;'>
<tr>
	<td class='base' style='width: 25%;'>$tr{'drop down menus'}</td>
	<td style='width: 25%;'><input type='checkbox' name='MENU' $checked{'MENU'}{'on'}></td>
	<td style='width: 25%;'>&nbsp;</td>
	<td style='width: 25%;'>&nbsp;</td>
</tr>
</table>
END
;

&closebox();

print <<END
<table style='width: 100%;'>
<tr>
	<td style='width: 100%; text-align: center;'>
	<input type='submit' name='ACTION' value='$tr{'save'}'>
	</td>
</tr>
</table>
END
;

print "</form>\n";

&closebigbox();

&closepage();
