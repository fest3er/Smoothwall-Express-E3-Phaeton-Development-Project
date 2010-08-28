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

print "<form method='post'>\n";

&openbox($tr{'timezonec'});
print <<END
  <table width='100%'>
    <tr>
      <td width='25%' class='base'>
        $tr{'timezonec'}
      </td>
      <td width='75%'>
        <select name='TIMEZONE'>
END
;
foreach (@timezones)
{
	chomp;
	$file = $_;
	s/_/ /g;
	print "          <option value='$file' $selected{'TIMEZONE'}{$file}>$_\n";
}
print <<END
        </select>
      </td>
      <td width='10%'>&nbsp;</td>
      <td width='35%'>&nbsp;</td>
    </tr>
  </table>
END
;
&closebox();

&openbox($tr{'time and datec'});
print <<END
  <table width='100%'>
    <tr>
      <td width='10%' class='base'>
        <span style='margin:0 .5em 0 0'>$tr{'setc'}</span>
        <input type='checkbox' name='TIME_CHANGE'>
      </td>
      <td width='10%' class='base'>$tr{'timec'}</td>
      <td width='35%'>
        <select name='HOUR'>
END
;
	for ($hour = 0; $hour < 24; $hour++)
	{
		if ($hour == $now[2]) { $sel = 'selected'; }
		else { $sel = ''; }
		print "        <option valuE='$hour' $sel>$hour\n";
	}
print <<END
        </select>
	:
	<select name='MINUTE'>
END
;
	for ($minute = 0; $minute < 60; $minute++)
	{
		if ($minute < 10) { $minutestr = "0$minute"; }
		else { $minutestr = $minute; }
		if ($minute == $now[1]) { $sel = 'selected'; }
		else { $sel = ''; }
		print "          <option value='$minute' $sel>$minutestr\n";
	}
print <<END
        </select>
	:
        <select name='SECOND'>
END
;
	for ($second = 0; $second < 60; $second++)
	{
		if ($second < 10) { $secondstr = "0$second"; }
		else { $secondstr = $second; }
		if ($second == $now[0]) { $sel = 'selected'; }
		else { $sel = ''; }
		print "          <option value='$second' $sel>$secondstr\n";
	}
print <<END
	</select>
      </td>
      <td width='10%' class='base'>$tr{'datec'}</td>
      <td width='35%'>
        <select name='DAY'>
END
;
	for ($day = 1; $day <= 31; $day++)
	{
		if ($day == $now[3]) { $sel = 'selected'; }
		else { $sel = ''; }
		print "          <option value='$day' $sel>$day\n";
	}
print <<END
        </select>
	<select name='MONTH'>
END
;
	for ($month = 0; $month < 12; $month++)
	{
		if ($month == $now[4]) { $sel = 'selected'; }
		else { $sel = ''; }
		print "          <option value='$month' $sel>$shortmonths[$month]\n";
	}
	print <<END
        </select>
        <select name='YEAR'>
END
	;
	for ($year = 101; $year < 130; $year++)
	{
		if ($year == $now[5]) { $sel = 'selected'; }
		else { $sel = ''; }
		$yearstr = $year + 1900;
		print "          <option value='$year' $sel>$yearstr\n";
	}
	print <<END
        </select>
      </td>
    </tr>
  </table>
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
  <table width='100%'>
    <tr>
      <td width='25%' class='base'>$tr{'enabled'}</td>
      <td width='25%'>
        <input type='checkbox' name='ENABLED' $checked{'ENABLED'}{'on'}>
      </td>
      <td width='25%' class='base'>$tr{'interval'}</td>
      <td width='25%'>
        <select name='NTP_INTERVAL'>
          <option value='1' $selected{'NTP_INTERVAL'}{'1'}>$tr{'one hour'}
          <option value='2' $selected{'NTP_INTERVAL'}{'2'}>$tr{'two hours'}
          <option value='3' $selected{'NTP_INTERVAL'}{'3'}>$tr{'three hours'}
          <option value='6' $selected{'NTP_INTERVAL'}{'6'}>$tr{'six hours'}
          <option value='12' $selected{'NTP_INTERVAL'}{'12'}>$tr{'twelve hours'}
          <option value='24' $selected{'NTP_INTERVAL'}{'24'}>$tr{'one day'}
          <option value='48' $selected{'NTP_INTERVAL'}{'48'}>$tr{'two days'}
          <option value='72' $selected{'NTP_INTERVAL'}{'72'}>$tr{'three days'}
        </select>
      </td>
    </tr>
    <tr>
      <td class='base'>$tr{'save time to rtc'}</td>
      <td>
        <input type='checkbox' name='NTP_RTC' $checked{'NTP_RTC'}{'on'}>
      </td>
      <td class='base'>$tr{'next update in'}</td>
      <td>$nextupdate</td>
    </tr>
  </table>
END
;

&closebox();

&openbox($tr{'network time servers'});

print <<END
  <table>
    <tr>
      <td class='base' style='text-align:left; padding-left:5em'>
        <input type='radio' name='NTP_SERVER_TYPE'
               value='RANDOM' $selected{'NTP_SERVER_TYPE'}{'RANDOM'}>
$tr{'multiple random public servers'}
      </td>
    </tr>
    <tr>
      <td class='base' style='text-align:left; padding-left:5em'>
        <input type='radio' name='NTP_SERVER_TYPE'
               value='selected' $selected{'NTP_SERVER_TYPE'}{'SELECTED'}>
        $tr{'selected single public server'}
      </td>
      <td>
        <select name='NTP_SERVER_SELECTED'>
END
;
foreach $desc (sort keys %timeservers) {
	print "<option value='$timeservers{$desc}' $selected{'NTP_SERVER_SELECTED'}{$timeservers{$desc}}>$desc\n"; }
print <<END
        </selected>
      </td>
    </tr>
    <tr>
      <td class='base' style='text-align:left; padding-left:5em'>
        <input type='radio' name='NTP_SERVER_TYPE' value='USERDEFINED'
               $selected{'NTP_SERVER_TYPE'}{'USERDEFINED'}>
          $tr{'user defined single public or local server'}
      </td>
      <td>
        <input type='text' SIZE='35' name='NTP_SERVER_USERDEFINED'
               value='$timesettings{'NTP_SERVER_USERDEFINED'}'
               id='ntp_server_userdefined'
               @{[jsvalidregex('ntp_server_userdefined','^[a-zA-Z0-9\.,\(\)@$!\%\^\&\*=\+_ ]*$')]}>
      </td>
    </tr>
  </table>
END
;

&closebox();

&openbox('Time server:');
print <<END
  <table width='100%'>
    <tr>
      <td width='25%' class='base'>$tr{'enabled'}</td>
      <td width='25%'><input type='checkbox' name='NTPD' $checked{'NTPD'}{'on'}></td>
      <td width='25%'>&nbsp;</td>
      <td width='25%'>&nbsp;</td>
    </tr>
  </table>
END
;
&closebox();

print <<END
  <div style='text-align:center'>
    <table width='60%'>
      <tr>
        <td style='text-align:center'>
          <input type='submit' name='ACTION' value='$tr{'save'}'>
        </td>
      </tr>
    </table>
  </div>
END
;

print "</form>\n";

&alertbox('add', 'add');

&closebigbox();

&closepage();
