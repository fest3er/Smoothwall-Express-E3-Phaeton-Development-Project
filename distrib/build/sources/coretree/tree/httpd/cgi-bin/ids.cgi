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
use smoothtype qw( :standard );

use Cwd;

my (%snortsettings, %checked);

&showhttpheaders();

$snortsettings{'ENABLE_SNORT'} = 'off';
$snortsettings{'ACTION'} = '';
&getcgihash(\%snortsettings);

$errormessage = '';
if ($snortsettings{'ACTION'} eq $tr{'save and update rules'})
{
	if ($snortsettings{'OINK'} !~ /^([\da-f]){40}$/i)
	{
		$errormessage = $tr{'oink code must be 40 hex digits'};
		goto EXIT;
	}
	&writehash("${swroot}/snort/settings", \%snortsettings);
	EXIT:
}
if ($snortsettings{'ACTION'} eq $tr{'save'}) {

	&writehash("${swroot}/snort/settings", \%snortsettings);

	if ($snortsettings{'ENABLE_SNORT'} eq 'on') {
		&log($tr{'snort is enabled'}); }
	else {
		&log($tr{'snort is disabled'}); }

	my $success = message('snortrestart');

	if (not defined $success) {
		$errormessage = $tr{'smoothd failure'}; }
}

&readhash("${swroot}/snort/settings", \%snortsettings);

$checked{'ENABLE_SNORT'}{'off'} = '';
$checked{'ENABLE_SNORT'}{'on'} = '';
$checked{'ENABLE_SNORT'}{$snortsettings{'ENABLE_SNORT'}} = 'CHECKED';

my $ruleage = 'N/A';
if (-e "${swroot}/snort/ruleage")
{
	my $days = int(-M "${swroot}/snort/ruleage");
	$ruleage = "$days $tr{'days'}";
}

&openpage($tr{'intrusion detection system'}, 1, '', 'services');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'intrusion detection system2'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='25%' CLASS='base'>Snort:</TD>
	<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLE_SNORT' $checked{'ENABLE_SNORT'}{'on'}></TD>
	<TD WIDTH='25%'>&nbsp;</TD>
	<TD WIDTH='35%'>&nbsp;</TD>
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


&openbox($tr{'rule retreval'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='25%'>$tr{'oink code'}</TD>
	<TD WIDTH='75%'><INPUT TYPE='text' NAME='OINK' SIZE='42' MAXLENGTH='40' VALUE='$snortsettings{OINK}' id='OINK' @{[jsvalidregex('OINK','^([0-9a-fA-F]){40}$')]}></TD>
</TR>
<TR>
	<TD>$tr{'rule age'}</TD><TD>$ruleage</TD>
</TR>
</TABLE>
END
;

&closebox();

#print <<END
#<DIV ALIGN='CENTER'>
#<TABLE WIDTH='60%'>
#<TR>
#	<TD ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'save and update rules'}'></TD> 
#</TR>
#<TR>
#	<TD ALIGN='CENTER'>$tr{'long download'}</TD> 
#</TR>
#</TABLE>
#</DIV>
#END
#;

&alertbox('add', 'add');

&openbox();
print <<END
<table class='blank'>
<tr>

	<td id='progressbar'>
<table class='progressbar' style='width: 380px;'>
	<tr>
		<td id='progress' class='progressbar' style='width: 1px;'>&nbsp;</td>
		<td class='progressend'>&nbsp;</td>
	</tr>
</table>
	<span id='status'></span>
	</td>
	<td>&nbsp;</td>
	<td style='width: 350px;' style='text-align: right;'>
		<INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'save and update rules'}'>
	</td>

</tr>
</table>
END
;
&closebox();

print "</FORM>\n";

&closebigbox();

# close except </body> and </html>
&closepage( "update" );
	
if ($snortsettings{'ACTION'} eq $tr{'save and update rules'} and !$errormessage)
{
	my $snortversion = &readvalue('/usr/lib/smoothwall/snortversion');
	$snortversion =~ /^(\d+\.\d+)/;
	$snortversion = $1;

	&runoinkmaster($snortversion);

	if (!$errormessage) {

		my $success = message('snortrestart');

		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'};
		}
	}
	if ($errormessage) {
		print <<END;
<script>
	document.getElementById('status').innerHTML = "$errormessage";
	document.getElementById('progress').style.width = "1px";
</script>
END
	} else {
		print <<END;
<script>
	document.getElementById('status').innerHTML = "Installation complete";
	document.getElementById('progress').style.width = "${maxwidth}px";
	document.location = "/cgi-bin/ids.cgi";
</script>
END
	}
}
print <<END;
</body>
</html>
END

sub runoinkmaster
{
	my $v = $_[0];
	my $url = 'http://www.snort.org/pub-bin/oinkmaster.cgi/' . $snortsettings{'OINK'} . "/snortrules-snapshot-$v.tar.gz";

	my $curdir = getcwd;
	chdir "${swroot}/snort/";
	
	select STDOUT;
	$| = 1;

	my $pid = open(FD, '-|');
	if (!defined $pid) {
		$errormessage = $tr{'unable to fetch rules'};
	} elsif ($pid) {
		$errormessage = $tr{'rules not available'};

		my $maxwidth = 400;

		print <<END;
<script>
document.getElementById('status').innerHTML = "Downloading, please wait";
document.getElementById('progress').style.background = "#a0a0ff";
</script>
END
		while(<FD>)
		{
			$errormessage = '';
			if (/(\d{1,3})%/) {
				my $percent = $1;
				my $message;
				if ($percent == 100) {
					print <<END;
<script>
	document.getElementById('status').innerHTML = "Installing, please wait";
	document.getElementById('progress').style.width = "${maxwidth}px";
</script>
END
				} else {
#					$message = "Download $percent% complete";
					my $curwidth = $maxwidth * $percent/100;
					print <<END;
<script>
document.getElementById('progress').style.width = "${curwidth}px";
</script>
END
				}
			}
		}
		close(FD);

		if ($?) {
			$errormessage = $tr{'unable to fetch rules'}; } 
		else
		{
			open (FILE, ">${swroot}/snort/ruleage");
			close (FILE);
		}
	} else {
		# so we see wget's output
		close(STDERR);
		open(STDERR, ">&STDOUT");

		exec('/usr/bin/oinkmaster.pl', '-v', '-C',
		'/usr/lib/smoothwall/oinkmaster.conf', '-o', 'rules', '-u', $url);
	}

	chdir $curdir;
}
