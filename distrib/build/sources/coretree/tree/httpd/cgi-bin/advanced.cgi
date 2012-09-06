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
my %netsettings;
&readhash("${swroot}/ethernet/settings", \%netsettings);

&showhttpheaders();

$cgiparams{'GREEN'} = 'off';
$cgiparams{'ORANGE'} = 'off';
$cgiparams{'PURPLE'} = 'off';

&getcgihash(\%cgiparams);

if ($cgiparams{'ACTION'} eq $tr{'save'})
{
	&writehash("${swroot}/portfw/subcheck", \%cgiparams);
}

if ($cgiparams{'ACTION'} eq '')
{
	$cgiparams{'GREEN'} = 'on';
	$cgiparams{'ORANGE'} = 'on';
	$cgiparams{'PURPLE'} = 'on';
}

&readhash("${swroot}/portfw/subcheck", \%cgiparams);

my %checked;
my $border = 0;

$checked{'GREEN'}{$cgiparams{'GREEN'}} = " checked";
$checked{'ORANGE'}{$cgiparams{'ORANGE'}} = " checked";
$checked{'PURPLE'}{$cgiparams{'PURPLE'}} = " checked";

&openpage( $tr{'full firewall control'}, 1, '', 'Advanced');

&openbigbox('100%', 'left');

&alertbox($errormessage);

print "<form method='post'>\n";

&openbox($tr{'ffc-subnet check'});

print <<END
<table style='width: 50%;'>
<tr>
	<td class='base'>$tr{'ffc-green'}</td>
	<td><input type='checkbox' name='GREEN' $checked{'GREEN'}{'on'}></td>
	<td style='width: 25%;'>&nbsp;</td>
	<td style='width: 25%;'>&nbsp;</td>
</tr>
<tr>
END
;

if ($netsettings{'ORANGE_DEV'}) {
	print "<td class='base'>$tr{'ffc-orange'}</td>\n";
	print "<td><input type='checkbox' name='ORANGE' $checked{'ORANGE'}{'on'}></td>\n";
	print "<td style='width: 25%;'>&nbsp;</td>\n";
	print "<td style='width: 25%;'>&nbsp;</td>\n";
	print "</tr>\n";
	print "<tr>\n";
}

if ($netsettings{'PURPLE_DEV'}) {
	print "<td class='base'>$tr{'ffc-purple'}</td>\n";
	print "<td><input type='checkbox' name='PURPLE' $checked{'PURPLE'}{'on'}></td>\n";
	print "<td style='width: 25%;'>&nbsp;</td>\n";
	print "<td style='width: 25%;'>&nbsp;</td>\n";
	print "</tr>\n";
	print "<tr>\n";
}
print <<END
</tr>
</table>
END
;

&closebox();

print <<END
<table style='width: 100%;' 'margin:6pt 0' border='$border'>
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
