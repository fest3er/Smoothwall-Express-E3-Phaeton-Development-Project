#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) SmoothWall Ltd 2002, 2003

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );
use smoothtype qw( :standard );

my @shortmonths = ( 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug',
	'Sep', 'Oct', 'Nov', 'Dec' );

my (%timesettings, %netsettings, $errormessage, my %timeservers);
my $found;
my @temp;
my $temp; my $tempsettings;

my $tzroot = '/usr/share/zoneinfo/posix';

&showhttpheaders();

$timesettings{'ACTION'} = '';
$timesettings{'VALID'} = '';

$timesettings{'TIMEZONE'} = '';
$timesettings{'ENABLE'} = 'off';
$timesettings{'NTP_RTC'} = 'off';
$timesettings{'NTPD'} = 'off';

open(FILE, "${swroot}/time/timeservers") or die "Unable to open timeservers file.";
while (<FILE>)
{
	chomp;
	@temp = split(/\|/);
	$timeservers{$temp[0]} = $temp[2];
}
close FILE;

&getcgihash(\%timesettings);

&readhash("${swroot}/ethernet/settings", \%netsettings);

open(FILE, "${swroot}/time/timezones");
@timezones = <FILE>;
close(FILE);

$errormessage = '';
if ($timesettings{'ACTION'} eq $tr{'save'})
{
	unless ($timesettings{'HOUR'} =~ /\d+/) {
		$errormessage = $tr{'invalid input'};
		goto ERROR; }
	unless ($timesettings{'MINUTE'} =~ /\d+/) {
		$errormessage = $tr{'invalid input'}; 
		goto ERROR; }
	unless ($timesettings{'SECOND'} =~ /\d+/) {
		$errormessage = $tr{'invalid input'}; 
		goto ERROR; }
	unless ($timesettings{'DAY'} =~ /\d+/) {
		$errormessage = $tr{'invalid input'}; 
		goto ERROR; }
	unless ($timesettings{'MONTH'} =~ /\d+/) {
		$errormessage = $tr{'invalid input'};
		goto ERROR; }
	unless ($timesettings{'YEAR'} =~ /\d+/) {
		$errormessage = $tr{'invalid input'};
		goto ERROR; }
	unless ($timesettings{'NTP_SERVER_TYPE'} =~ /^(RANDOM|SELECTED|USERDEFINED)$/) {
		$errormessage = $tr{'invalid input'};
		goto ERROR; }
	$found = 0;
	foreach (@timezones)
	{
		chomp;
		if ($_ eq $timesettings{'TIMEZONE'}) {
			$found = 1; }
	}
	if ($found == 0) {
		$errormessage = $tr{'invalid input'};
		goto ERROR; }

	if ($timesettings{'NTP_SERVER_TYPE'} eq 'USERDEFINED')
	{
		unless ($timesettings{'NTP_SERVER_USERDEFINED'} and ($timesettings{'NTP_SERVER_USERDEFINED'} =~ /^[\w\d\.\-,\(\)@$!\%\^\&\*=\+_ ]*$/ )) {
			$errormessage = $tr{'bad ntp host'};
			goto ERROR; }
	}	

	if ($timesettings{'TIME_CHANGE'} eq 'on')
	{
		$year = $timesettings{'YEAR'} + 1900;
		$month = $timesettings{'MONTH'} + 1;
		$day = $timesettings{'DAY'};

		$timedate = "$timesettings{'HOUR'}:$timesettings{'MINUTE'}:$timesettings{'SECOND'} $year/$month/$day";
		system('/usr/bin/smoothcom', 'settime', $timedate);
		&log($tr{'setting time'});
	}


	foreach $temp ('TIMEZONE', 'ENABLED', 'NTP_INTERVAL', 'NTP_RTC',
		'NTP_SERVER_TYPE', 'NTP_SERVER_SELECTED', 'NTP_SERVER_USERDEFINED', 'NTPD')
	{
		$tempsettings{$temp} = $timesettings{$temp};
	}

	unlink("${swroot}/time/localtime");
	system('/bin/ln', '-s', "${tzroot}/$timesettings{'TIMEZONE'}", "${swroot}/time/localtime");

ERROR:
	if ($errormessage) {
		$timeettings{'VALID'} = 'no'; }
	else {
		$timesettings{'VALID'} = 'yes'; }

	&writehash("${swroot}/time/settings", \%tempsettings);
	
	if ($timesettings{'VALID'} eq 'yes')
	{	
		system('/usr/bin/smoothwall/writentpd.pl');
	
		my $success = message('ntpdrestart');
		
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; 
		}
	}
}

if ($timesettings{'VALID'} eq '')
{
	$timesettings{'TIMEZONE'} = 'Europe/London';
	$timesettings{'ENABLE'} = 'off';
	$timesettings{'NTP_INTERVAL'} = 24;
	$timesettings{'NTP_RTC'} = 'on';
	$timesettings{'NTP_SERVER_TYPE'} = 'RANDOM';
	$timesettings{'NTPD'} = 'off';
}

&readhash("${swroot}/time/settings", \%timesettings);

$checked{'ENABLED'}{'on'} = '';
$checked{'ENABLED'}{'off'} = '';
$checked{'ENABLED'}{$timesettings{'ENABLED'}} = 'CHECKED';

$checked{'NTP_RTC'}{'on'} = '';
$checked{'NTP_RTC'}{'off'} = '';
$checked{'NTP_RTC'}{$timesettings{'NTP_RTC'}} = 'CHECKED';

$checked{'NTPD'}{'on'} = '';
$checked{'NTPD'}{'off'} = '';
$checked{'NTPD'}{$timesettings{'NTPD'}} = 'CHECKED';

$selected{'TIMEZONE'}{$timesettings{'TIMEZONE'}} = 'SELECTED';

$selected{'NTP_INTERVAL'}{'1'} = '';
$selected{'NTP_INTERVAL'}{'2'} = '';
$selected{'NTP_INTERVAL'}{'3'} = '';
$selected{'NTP_INTERVAL'}{'6'} = '';
$selected{'NTP_INTERVAL'}{'12'} = '';
$selected{'NTP_INTERVAL'}{'24'} = '';
$selected{'NTP_INTERVAL'}{'48'} = '';
$selected{'NTP_INTERVAL'}{'72'} = '';
$selected{'NTP_INTERVAL'}{$timesettings{'NTP_INTERVAL'}} = 'SELECTED';

$selected{'NTP_SERVER_TYPE'}{'RANDOM'} = '';
$selected{'NTP_SERVER_TYPE'}{'SELECTED'} = '';
$selected{'NTP_SERVER_TYPE'}{'USERDEFINED'} = '';
$selected{'NTP_SERVER_TYPE'}{$timesettings{'NTP_SERVER_TYPE'}} = 'CHECKED';

$selected{'NTP_SERVER_SELECTED'}{$timesettings{'NTP_SERVER_SELECTED'}} = 'SELECTED';

my @now = localtime(time);

&openpage($tr{'time settings'}, 1, '', 'services');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'timezonec'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='25%' CLASS='base'>$tr{'timezonec'}</TD>
	<TD WIDTH='75%'>
	<SELECT NAME='TIMEZONE'>
END
;
foreach (@timezones)
{
	chomp;
	$file = $_;
	s/_/ /g;
	print "<OPTION VALUE='$file' $selected{'TIMEZONE'}{$file}>$_\n";
}
print <<END
	</SELECT>
	</TD>
	<TD WIDTH='10%'>&nbsp;</TD>
	<TD WIDTH='35%'>&nbsp;</TD>
</TR>
</TABLE>
END
;
&closebox();

&openbox($tr{'time and datec'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='10%' CLASS='base'>
	$tr{'setc'}<INPUT TYPE='checkbox' NAME='TIME_CHANGE'>
	</TD>
	<TD WIDTH='10%' CLASS='base'>$tr{'timec'}</TD>
	<TD WIDTH='35%'>
	<SELECT NAME='HOUR'>
END
;
	for ($hour = 0; $hour < 24; $hour++)
	{
		if ($hour == $now[2]) { $sel = 'SELECTED'; }
		else { $sel = ''; }
		print "<OPTION VALUE='$hour' $sel>$hour\n";
	}
print <<END
	</SELECT>
	:
	<SELECT NAME='MINUTE'>
END
;
	for ($minute = 0; $minute < 60; $minute++)
	{
		if ($minute < 10) { $minutestr = "0$minute"; }
		else { $minutestr = $minute; }
		if ($minute == $now[1]) { $sel = 'SELECTED'; }
		else { $sel = ''; }
		print "<OPTION VALUE='$minute' $sel>$minutestr\n";
	}
print <<END
	</SELECT>
	:
	<SELECT NAME='SECOND'>
END
;
	for ($second = 0; $second < 60; $second++)
	{
		if ($second < 10) { $secondstr = "0$second"; }
		else { $secondstr = $second; }
		if ($second == $now[0]) { $sel = 'SELECTED'; }
		else { $sel = ''; }
		print "<OPTION VALUE='$second' $sel>$secondstr\n";
	}
print <<END
	</SELECT>
	<TD WIDTH='10%' CLASS='base'>$tr{'datec'}</TD>
	<TD WIDTH='35%'>
	<SELECT NAME='DAY'>
END
;
	for ($day = 1; $day <= 31; $day++)
	{
		if ($day == $now[3]) { $sel = 'SELECTED'; }
		else { $sel = ''; }
		print "<OPTION VALUE='$day' $sel>$day\n";
	}
print <<END
	</SELECT>
	<SELECT NAME='MONTH'>
END
;
	for ($month = 0; $month < 12; $month++)
	{
		if ($month == $now[4]) { $sel = 'SELECTED'; }
		else { $sel = ''; }
		print "<OPTION VALUE='$month' $sel>$shortmonths[$month]\n";
	}
	print <<END
	</SELECT>
	<SELECT NAME='YEAR'>
END
	;
	for ($year = 101; $year < 130; $year++)
	{
		if ($year == $now[5]) { $sel = 'SELECTED'; }
		else { $sel = ''; }
		$yearstr = $year + 1900;
		print "<OPTION VALUE='$year' $sel>$yearstr\n";
	}
	print <<END
	</SELECT>
	</TD>
</TR>
</TABLE>
END
;

&closebox();

&openbox($tr{'network time retrieval'});

my $timecount;
my $nextupdate;

open(FILE, "${swroot}/time/timecount");
$timecount = <FILE>; chomp $timecount;
close(FILE);

if (($timesettings{'NTP_INTERVAL'} - $timecount) > 1) {
	$nextupdate = $timesettings{'NTP_INTERVAL'} - $timecount . " $tr{'hours'}"; }
else {
	$nextupdate = $tr{'less than one hour'}; }

print <<END
<TABLE WIDTH='100%'>
<TR>
<TD WIDTH='25%' CLASS='base'>$tr{'enabled'}</TD>
<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLED' $checked{'ENABLED'}{'on'}></TD>
<TD WIDTH='25%' CLASS='base'>$tr{'interval'}</TD>
<TD WIDTH='25%'>
<SELECT NAME='NTP_INTERVAL'>
<OPTION VALUE='1' $selected{'NTP_INTERVAL'}{'1'}>$tr{'one hour'}
<OPTION VALUE='2' $selected{'NTP_INTERVAL'}{'2'}>$tr{'two hours'}
<OPTION VALUE='3' $selected{'NTP_INTERVAL'}{'3'}>$tr{'three hours'}
<OPTION VALUE='6' $selected{'NTP_INTERVAL'}{'6'}>$tr{'six hours'}
<OPTION VALUE='12' $selected{'NTP_INTERVAL'}{'12'}>$tr{'twelve hours'}
<OPTION VALUE='24' $selected{'NTP_INTERVAL'}{'24'}>$tr{'one day'}
<OPTION VALUE='48' $selected{'NTP_INTERVAL'}{'48'}>$tr{'two days'}
<OPTION VALUE='72' $selected{'NTP_INTERVAL'}{'72'}>$tr{'three days'}
</SELECT>
</TD>
</TR>
<TR>
<TD CLASS='base'>$tr{'save time to rtc'}</TD>
<TD><INPUT TYPE='checkbox' NAME='NTP_RTC' $checked{'NTP_RTC'}{'on'}></TD>
<TD CLASS='base'>$tr{'next update in'}</TD>
<TD>$nextupdate</TD>
</TR>
</TABLE>
END
;

&closebox();

&openbox($tr{'network time servers'});

print <<END
<TABLE>
<TR>
<TD CLASS='base' style='text-align:left; padding-left:5em'>
<INPUT TYPE='radio' NAME='NTP_SERVER_TYPE' VALUE='RANDOM' $selected{'NTP_SERVER_TYPE'}{'RANDOM'}>
$tr{'multiple random public servers'}
</TD>
<TD 
</TR>
<TR>
<TD CLASS='base' style='text-align:left; padding-left:5em'>
<INPUT TYPE='radio' NAME='NTP_SERVER_TYPE' VALUE='SELECTED' $selected{'NTP_SERVER_TYPE'}{'SELECTED'}>
$tr{'selected single public server'}
</TD>
<TD>
<SELECT NAME='NTP_SERVER_SELECTED'>
END
;
foreach $desc (sort keys %timeservers) {
	print "<OPTION VALUE='$timeservers{$desc}' $selected{'NTP_SERVER_SELECTED'}{$timeservers{$desc}}>$desc\n"; }
print <<END
</SELECTED>
</TD>
</TR>
<TD CLASS='base' style='text-align:left; padding-left:5em'>
<INPUT TYPE='radio' NAME='NTP_SERVER_TYPE' VALUE='USERDEFINED' $selected{'NTP_SERVER_TYPE'}{'USERDEFINED'}>
$tr{'user defined single public or local server'}
</TD>
<TD><INPUT TYPE='text' SIZE='35' NAME='NTP_SERVER_USERDEFINED' VALUE='$timesettings{'NTP_SERVER_USERDEFINED'}' id='ntp_server_userdefined' @{[jsvalidregex('ntp_server_userdefined','^[a-zA-Z0-9\.,\(\)@$!\%\^\&\*=\+_ ]*$')]}></TD>
</TABLE>
END
;

&closebox();

&openbox('Time server:');
print <<END
<TABLE WIDTH='100%'>
<TR>
<TD WIDTH='25%' CLASS='base'>$tr{'enabled'}</TD>
<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='NTPD' $checked{'NTPD'}{'on'}></TD>
<TD WIDTH='25%'>&nbsp;</TD>
<TD WIDTH='25%'>&nbsp;</TD>
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
