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

my %cgiparams;

&showhttpheaders();

$cgiparams{'ACTION'} = '';
$cgiparams{'VALID'} = '';

&getcgihash(\%cgiparams);

my $errormessage = '';

if ($cgiparams{'ACTION'} eq $tr{'save'})
{
	if (($cgiparams{'START_HOUR'} * 60) + $cgiparams{'START_MIN'} >
		($cgiparams{'END_HOUR'} * 60) + $cgiparams{'END_MIN'})
	{
		$errormessage = $tr{'from time must be before to time'};
	}

	my @machines = split(/\n/, $cgiparams{'MACHINES'});
	my $line = 1;
	foreach (@machines)
	{
		chomp;
		if ($_ eq '') { next; } 
		unless (&validipormask($_))
		{
			$errormessage = $tr{'invalid address'} . " '$_' ($line)";
			goto ERROR;
		}
		$line++;
	}
		
ERROR:
	if ($errormessage) {
		$cgiparams{'VALID'} = 'no'; }
	else {
		$cgiparams{'VALID'} = 'yes'; }
		
	delete $cgiparams{'MACHINES'};
	&writehash("${swroot}/timedaccess/settings", \%cgiparams);
	
	open(FILE, ">${swroot}/timedaccess/machines") or die 'Unable to open machines for writing';
	foreach (@machines)
	{
		chomp;
		if ($_ eq '') { next; } 
		print FILE "$_\n";
	}
	close(FILE);	

	if ($cgiparams{'VALID'} eq 'yes')
	{
		my $success = message('settimedaccess');
		
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}

&readhash("${swroot}/timedaccess/settings", \%cgiparams);

my %checked;

$checked{'ENABLE'}{'off'} = '';
$checked{'ENABLE'}{'on'} = '';
$checked{'ENABLE'}{$cgiparams{'ENABLE'}} = 'checked';

for (my $day = 0; $day < 7; $day++) {
	$checked{"DAY_${day}"}{$cgiparams{"DAY_${day}"}} = 'checked'; }

$selected{'START_HOUR'}{$cgiparams{'START_HOUR'}} = 'selected';
$selected{'START_MIN'}{$cgiparams{'START_MIN'}} = 'selected';
$selected{'END_HOUR'}{$cgiparams{'END_HOUR'}} = 'selected';
$selected{'END_MIN'}{$cgiparams{'END_MIN'}} = 'selected';

$selected{'MODE'}{'ALLOW'} = '';
$selected{'MODE'}{'REJECT'} = '';
$selected{'MODE'}{$cgiparams{'MODE'}} = 'selected';

&openpage($tr{'timed access'}, 1, '', 'networking');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<form method='post'>\n";

&openbox($tr{'global settingsc'});
print <<END
<table width='100%'>
<tr>
<td class='base' width='25%'>$tr{'enabled'}</td>
<td width='25%'><input type='checkbox' name='ENABLE' $checked{'ENABLE'}{'on'}></td>
<td class='base' width='25%'>$tr{'modec'}</td>
<td width='25%'>
<select name='MODE'>
<option value='ALLOW' $selected{'MODE'}{'ALLOW'}>$tr{'allow at specified times'}
<option value='REJECT' $selected{'MODE'}{'REJECT'}>$tr{'reject at specified times'}
</select>
</td>
</tr>
</table>
END
;

&closebox();

&openbox($tr{'timed accessc'});
print <<END
<div align='center'>
<table width='50%'>
<tr>
<td class='base' width='15%'>$tr{'fromc'}</td>
<td width='35%'><select name='START_HOUR'>
END
;
for (my $hour = 0; $hour < 24; $hour++)
{
	print "<option value='$hour' $selected{'START_HOUR'}{$hour}>$hour\n";
}
print <<END
</select>
:
<select name='START_MIN'>
END
;
for (my $min = 0; $min < 60; $min++)
{
	my $printmin = $min;
	if ($min < 10) { $printmin = '0'.$min; }
	print "<option value='$min' $selected{'START_MIN'}{$min}>$printmin\n";
}
print <<END
</select>
</td>
<td class='base' width='15%'>$tr{'toc'}</td>
<td width='35%'><select name='END_HOUR'>
END
;
for (my $hour = 0; $hour < 24; $hour++)
{
	print "<option value='$hour' $selected{'END_HOUR'}{$hour}>$hour\n";
}
print <<END
</select>
:
<select name='END_MIN'>
END
;
for (my $min = 0; $min < 60; $min++)
{
	my $printmin = $min;
	if ($min < 10) { $printmin = '0'.$min; }
	print "<option value='$min' $selected{'END_MIN'}{$min}>$printmin\n";
}
print <<END
</select>
</td>
</tr>
</table>
<table width='60%'>
<tr>
END
;
for (my $day = 0; $day < 7; $day++)
{
	print <<END
<td class='base' style='padding-right:0'>$tr{"day $day"}:</td>
<td style='text-align:left; padding:0 1em 0 0'><input type='checkbox' name='DAY_$day' $checked{"DAY_${day}"}{'on'}></td>
END
	;
}

print <<END
</tr>
</table>
<br>
END
;

print <<END
<table width='100%'>
<tr>
<td align='center'>
$tr{'machinesc'}<br>
<textarea name='MACHINES' cols='35' rows='6'>
END
;

open(FILE, "${swroot}/timedaccess/machines") or die 'Unable to open machines list';
my @machines = <FILE>;
close(FILE);

foreach (@machines)
{
	chomp;
	print "$_\n";
}

print <<END
</textarea><br>
$tr{'enter one ip address or network with netmask address per line'}
</td>
</tr>
</table>
</div>
END
;

&closebox();

print <<END
<DIV ALIGN='CENTER'>
<TABLE WIDTH='60%'>
<TR>
        <TD WIDTH='100%' ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'save'}'></TD>
</TR>
</TABLE>
</DIV>
END
;

print "</form>\n";

&alertbox('add', 'add');

&closebigbox();

&closepage();
