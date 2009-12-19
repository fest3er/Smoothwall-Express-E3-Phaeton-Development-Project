#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothtype qw( :standard );

my %pppsettings;
my %temppppsettings;
my @profilenames;

my $maxprofiles = 5;

my %isdnsettings;
my %netsettings;
my %adslsettings;

# Get ISDN settings so we can see if ISDN is enabled or not.
$isdnsettings{'ENABLED'} = 'off';
&readhash("${swroot}/isdn/settings", \%isdnsettings);
&readhash("${swroot}/adsl/settings", \%adslsettings);

# Get PPPoE settings so we can see if PPPoE is enabled or not.
&readhash("${swroot}/ethernet/settings", \%netsettings);

&showhttpheaders();

$pppsettings{'ACTION'} = '';
$pppsettings{'VALID'} = '';

$pppsettings{'PROFILENAME'} = '';

$pppsettings{'COMPORT'} = '';
$pppsettings{'DTERATE'} = '';
$pppsettings{'TELEPHONE'} = '';
$pppsettings{'SPEAKER'} = 'off';
$pppsettings{'DIALMODE'} = '';
$pppsettings{'MAXRETRIES'} = '';
$pppsettings{'TIMEOUT'} = '';
$pppsettings{'PERSISTENT'} = 'off';
$pppsettings{'DIALONDEMAND'} = 'off';
$pppsettings{'DIALONDEMANDDNS'} = 'off';
$pppsettings{'AUTOCONNECT'} = 'off';
$pppsettings{'AUTOREBOOT'} = 'off';
$pppsettings{'SENDCR'} = 'off';

$pppsettings{'SERVICENAME'} = '';
$pppsettings{'CONCENTRATORNAME'} = '';

$pppsettings{'USERNAME'} = '';
$pppsettings{'PASSWORD'} = '';
$pppsettings{'AUTH'} = '';
$pppsettings{'LOGINSCRIPT'} = '';

$pppsettings{'DNS'} = '';
$pppsettings{'DNS1'} = '';
$pppsettings{'DNS2'} = '';

$pppsettings{'STAYUP'} = 'off';
$pppsettings{'STAYUP_TIME'} = '';

&getcgihash(\%pppsettings);

my $errormessage = '';

if ($pppsettings{'ACTION'} ne '' &&
	(-e '/var/run/ppp-smooth.pid' || -e "${swroot}/red/active"))
{
	$errormessage = $tr{'unable to alter profiles while red is active'};
	# read in the current vars
	%pppsettings = ();
	$pppsettings{'VALID'} = '';
	&readhash("${swroot}/ppp/settings", \%pppsettings);
}
elsif ($pppsettings{'ACTION'} eq $tr{'save'})
{
	unless ($pppsettings{'COMPORT'} =~ /^(ttyS0|ttyS1|ttyS2|ttyS3|isdn1|isdn2|pppoe|adsl)$/) {
		$errormessage = $tr{'invalid input'};
		goto ERROR; }
	unless ($pppsettings{'DTERATE'} =~ /^(9600|19200|38400|57600|115200|230400)$/) {
		$errormessage = $tr{'invalid input'};
		goto ERROR; }
	unless ($pppsettings{'DIALMODE'} =~ /^(T|P)$/) {
		$errormessage = $tr{'invalid input'};
		goto ERROR; }
	unless ($pppsettings{'AUTH'} =~
		/^(pap-or-chap|standard-login-script|demon-login-script|other-login-script)$/)
	{
		$errormessage = $tr{'invalid input'};
		goto ERROR;
	}
	unless ($pppsettings{'USERNAME'} and ($pppsettings{'USERNAME'} =~ /^[\w\d\.\-,\(\)\@\$\!\%\^\&\*=\+\/_ ]*$/ )) {
		$errormessage = $tr{'invalid username'};
		goto ERROR; 
	}
	unless ($pppsettings{'PASSWORD'} and ($pppsettings{'PASSWORD'} =~ /^[\w\d\.\-,\(\)\@\$\!\%\^\&\*=\+\/_ ]*$/ )) {
		$errormessage = $tr{'invalid password'};
		goto ERROR; 
	}


	if ($pppsettings{'PROFILENAME'} eq '') {
		$errormessage = $tr{'profile name not given'};
		$pppsettings{'PROFILENAME'} = '';
		goto ERROR; }

	unless($pppsettings{'COMPORT'} eq 'pppoe' || $pppsettings{'COMPORT'} eq 'adsl') {
		if ($pppsettings{'TELEPHONE'} eq '') {
			$errormessage = $tr{'telephone not set'}; 
			goto ERROR; }
		if (!($pppsettings{'TELEPHONE'} =~ /^[\d\*\#\,]+$/)) {
			$errormessage = $tr{'bad characters in the telephone number field'};
			goto ERROR; }
	}
	if ($pppsettings{'DIALONDEMAND'} eq 'on' &&
		($pppsettings{'COMPORT'} eq 'pppoe' || $pppsettings{'COMPORT'} eq 'adsl'))
	{
		$errormessage = $tr{'dial on demand for this interface is not supported'};
		goto ERROR;
	}
	if ($pppsettings{'USERNAME'} eq '') {
		$errormessage = $tr{'username not set'}; 
		goto ERROR; }
	if ($pppsettings{'PASSWORD'} eq '') {
		$errormessage = $tr{'password not set'};
		goto ERROR; }

	if ($pppsettings{'TIMEOUT'} eq '') {
		$errormessage = $tr{'idle timeout not set'};
		goto ERROR; }
	if (!($pppsettings{'TIMEOUT'} =~ /^\d+$/)) {
                $errormessage = $tr{'only digits allowed in the idle timeout'};
		goto ERROR; }

	if ($pppsettings{'LOGINSCRIPT'} =~ /[.\/ ]/ ) {
		$errormessage = $tr{'bad characters in script field'}; 
		goto ERROR; }

        if ($pppsettings{'DNS1'})
        {
                if (!(&validip($pppsettings{'DNS1'}))) {
                        $errormessage = $tr{'invalid primary dns'};
			goto ERROR;  }
        }
        if ($pppsettings{'DNS2'})
        {
                if (!(&validip($pppsettings{'DNS2'}))) {
                        $errormessage = $tr{'invalid secondary dns'};
			goto ERROR; }
        }

	if ($pppsettings{'MAXRETRIES'} eq '') {
		$errormessage = $tr{'max retries not set'};
		goto ERROR; }
	if (!($pppsettings{'MAXRETRIES'} =~ /^\d+$/)) {
		$errormessage = $tr{'only digits allowed in max retries field'};
		goto ERROR; }

	if ($adslsettings{'ENABLED'} eq 'on' &&
		$pppsettings{'COMPORT'} eq 'adsl')
	{
		if ($adslsettings{'DEVICE'} eq 'ALCATEL' &&
			!-e "${swroot}/adsl/mgmt.o")
		{
			$errormessage = $tr{'no usb adsl firmware'};
			goto ERROR;
		}
	}

	if ($isdnsettings{'ENABLED'} eq 'on' &&
		$pppsettings{'COMPORT'} =~ /^isdn/)
	{
		unless ($pppsettings{'STAYUP_TIME'} =~ /^\d+$/)
		{
			$errormessage = 'Minimum time to keep second channel up is not a number.';
			goto ERROR;
		}
	}
ERROR:
	if ($errormessage) {
		$pppsettings{'VALID'} = 'no'; }
	else {
		$pppsettings{'VALID'} = 'yes'; }

	# write cgi vars to the file.
	&writehash("${swroot}/ppp/settings-$pppsettings{'PROFILE'}",
		\%pppsettings);

	# make link and write secret file.
	&updatesettings();
	&writesecrets();

	&log("$tr{'profile saved'} $pppsettings{'PROFILENAME'}");
}
elsif ($pppsettings{'ACTION'} eq $tr{'select'})
{
	%temppppsettings = ();
	$temppppsettings{'PROFILE'} = '';
	&readhash("${swroot}/ppp/settings-$pppsettings{'PROFILE'}",
		\%temppppsettings);

	if ($temppppsettings{'PROFILE'} ne '')
	{
		# make link.
		&updatesettings(); 

		# read in the new params "early" so we can write secrets.
		%pppsettings = ();
		&readhash("${swroot}/ppp/settings", \%pppsettings);

		&writesecrets();

		&log("$tr{'profile made current'} $pppsettings{'PROFILENAME'}"); 
	}
	else
	{
		$errormessage = $tr{'the selected profile is empty'};
		%pppsettings = ();		 
		$pppsettings{'VALID'} = '';	 
		&readhash("${swroot}/ppp/settings", \%pppsettings);
	}		
}
elsif ($pppsettings{'ACTION'} eq $tr{'delete'})
{
	&log("$tr{'profile deleted'} $pppsettings{'PROFILENAME'}");

	truncate ("${swroot}/ppp/settings-$pppsettings{'PROFILE'}", 0);
	%pppsettings = ();
	$pppsettings{'VALID'} = '';
	&readhash("${swroot}/ppp/settings", \%pppsettings);			
}
else
{
	# read in the current vars
	%pppsettings = ();
	$pppsettings{'VALID'} = '';
	&readhash("${swroot}/ppp/settings", \%pppsettings);
}

# read in the profile names into @profilenames.
my $c;
for ($c = 1; $c <= $maxprofiles; $c++)
{
 	%temppppsettings = ();
	$temppppsettings{'PROFILENAME'} = $tr{'empty'};
	&readhash("${swroot}/ppp/settings-$c", \%temppppsettings);
	$profilenames[$c] = $temppppsettings{'PROFILENAME'};
}

if ($pppsettings{'VALID'} eq '')
{
	$pppsettings{'PROFILE'} = 1;
	$pppsettings{'PROFILENAME'} = $tr{'unnamed'};
	$pppsettings{'COMPORT'} = 'ttyS0';
	$pppsettings{'DTERATE'} = 115200;
	$pppsettings{'SPEAKER'} = 'on';
	$pppsettings{'PERSISTENT'} = 'off';
	$pppsettings{'DIALONDEMAND'} = 'off';
	$pppsettings{'DIALONDEMANDDNS'} = 'off';
	$pppsettings{'AUTOCONNECT'} = 'off';
	$pppsettings{'AUTOREBOOT'} = 'off';
	$pppsettings{'SERVICENAME'} = '';
	$pppsettings{'CONCENTRATORNAME'} = '';
	$pppsettings{'DIALMODE'} = 'T';
	$pppsettings{'MAXRETRIES'} = 10;
	$pppsettings{'TIMEOUT'} = 15;
	$pppsettings{'AUTH'} = 'pap-or-chap';
	$pppsettings{'DNS'} = 'Automatic';
	$pppsettings{'STAYUP'} = 'off';
	$pppsettings{'STAYUP_TIME'} = '30';
}
my (%selected,%checked);
for ($c = 1; $c <= $maxprofiles; $c++) {
	$selected{'PROFILE'}{$c} = ''; }
$selected{'PROFILE'}{$pppsettings{'PROFILE'}} = 'SELECTED';

$selected{'COMPORT'}{'ttyS0'} = '';
$selected{'COMPORT'}{'ttyS1'} = '';
$selected{'COMPORT'}{'ttyS2'} = '';
$selected{'COMPORT'}{'ttyS3'} = '';
$selected{'COMPORT'}{'isdn1'} = '';
$selected{'COMPORT'}{'isdn2'} = '';
$selected{'COMPORT'}{'pppoe'} = '';
$selected{'COMPORT'}{'adsl'} = '';
$selected{'COMPORT'}{$pppsettings{'COMPORT'}} = 'SELECTED';

$selected{'DTERATE'}{'9600'} = '';
$selected{'DTERATE'}{'19200'} = '';
$selected{'DTERATE'}{'38400'} = '';
$selected{'DTERATE'}{'57600'} = '';
$selected{'DTERATE'}{'115200'} = '';
$selected{'DTERATE'}{'230400'} = '';
$selected{'DTERATE'}{$pppsettings{'DTERATE'}} = 'SELECTED';

$checked{'SPEAKER'}{'off'} = '';
$checked{'SPEAKER'}{'on'} = '';
$checked{'SPEAKER'}{$pppsettings{'SPEAKER'}} = 'CHECKED';

$selected{'DIALMODE'}{'T'} = '';
$selected{'DIALMODE'}{'P'} = '';
$selected{'DIALMODE'}{$pppsettings{'DIALMODE'}} = 'SELECTED';

$checked{'PERSISTENT'}{'off'} = '';
$checked{'PERSISTENT'}{'on'} = '';
$checked{'PERSISTENT'}{$pppsettings{'PERSISTENT'}} = 'CHECKED';

$checked{'DIALONDEMAND'}{'off'} = '';
$checked{'DIALONDEMAND'}{'on'} = '';
$checked{'DIALONDEMAND'}{$pppsettings{'DIALONDEMAND'}} = 'CHECKED';

$checked{'DIALONDEMANDDNS'}{'off'} = '';
$checked{'DIALONDEMANDDNS'}{'on'} = '';
$checked{'DIALONDEMANDDNS'}{$pppsettings{'DIALONDEMANDDNS'}} = 'CHECKED';

$checked{'AUTOCONNECT'}{'off'} = '';
$checked{'AUTOCONNECT'}{'on'} = ''; 
$checked{'AUTOCONNECT'}{$pppsettings{'AUTOCONNECT'}} = 'CHECKED';

$checked{'AUTOREBOOT'}{'off'} = '';
$checked{'AUTOREBOOT'}{'on'} = ''; 
$checked{'AUTOREBOOT'}{$pppsettings{'AUTOREBOOT'}} = 'CHECKED';

$checked{'SENDCR'}{'off'} = '';
$checked{'SENDCR'}{'on'} = '';
$checked{'SENDCR'}{$pppsettings{'SENDCR'}} = 'CHECKED';

$selected{'AUTH'}{'pap-or-chap'} = '';
$selected{'AUTH'}{'standard-login-script'} = '';
$selected{'AUTH'}{'demon-login-script'} = '';
$selected{'AUTH'}{'other-login-script'} = '';
$selected{'AUTH'}{$pppsettings{'AUTH'}} = 'SELECTED';

$checked{'DNS'}{'Automatic'} = '';
$checked{'DNS'}{'Manual'} = '';
$checked{'DNS'}{$pppsettings{'DNS'}} = 'CHECKED';

$checked{'STAYUP'}{'off'} = '';
$checked{'STAYUP'}{'on'} = '';
$checked{'STAYUP'}{$pppsettings{'STAYUP'}} = 'CHECKED';

&openpage($tr{'ppp setup'}, 1, '', 'networking');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'profiles'});

print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='30%'>
	<SELECT NAME='PROFILE'>
END
;
for ($c = 1; $c <= $maxprofiles; $c++)
{
	print "\t<OPTION VALUE='$c' $selected{'PROFILE'}{$c}>$profilenames[$c]\n";
}
print <<END
	</SELECT>
	</TD>
	<TD WIDTH='10%'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'select'}'></TD>
	<TD WIDTH='10%'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'delete'}'></TD>
	<TD WIDTH='25%' CLASS='base'>$tr{'profile name'}</TD>
        <TD WIDTH='25%'><INPUT TYPE='text' NAME='PROFILENAME' VALUE='$pppsettings{'PROFILENAME'}'></TD>
</TR>
</TABLE>
END
;

&closebox();

&openbox($tr{'telephony'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD CLASS='base' WIDTH='25%'>$tr{'interface'}</TD>
	<TD WIDTH='25%'>
	<SELECT NAME='COMPORT'>
	<OPTION VALUE='ttyS0' $selected{'COMPORT'}{'ttyS0'}>$tr{'modem on com1'}
	<OPTION VALUE='ttyS1' $selected{'COMPORT'}{'ttyS1'}>$tr{'modem on com2'}
	<OPTION VALUE='ttyS2' $selected{'COMPORT'}{'ttyS2'}>$tr{'modem on com3'}
	<OPTION VALUE='ttyS3' $selected{'COMPORT'}{'ttyS3'}>$tr{'modem on com4'}
END
;
if ($isdnsettings{'ENABLED'} eq 'on')
{
	print <<END
	<OPTION VALUE='isdn1' $selected{'COMPORT'}{'isdn1'}>$tr{'isdn1'}
	<OPTION VALUE='isdn2' $selected{'COMPORT'}{'isdn2'}>$tr{'isdn2'}
END
;
}
if ($netsettings{'RED_TYPE'} eq 'PPPOE')
{
	print <<END
	<OPTION VALUE='pppoe' $selected{'COMPORT'}{'pppoe'}>PPPoE
END
;
}
if ($adslsettings{'ENABLED'} eq 'on')
{
	print <<END
	<OPTION VALUE='adsl' $selected{'COMPORT'}{'adsl'}>ADSL
END
;
}

print <<END
	</SELECT>
	</TD>
	<TD WIDTH='25%' CLASS='base'>$tr{'computer to modem rate'}</TD>
	<TD WIDTH='25%'>
	<SELECT NAME='DTERATE'>
	<OPTION VALUE='9600' $selected{'DTERATE'}{'9600'}>9600
	<OPTION VALUE='19200' $selected{'DTERATE'}{'19200'}>19200
	<OPTION VALUE='38400' $selected{'DTERATE'}{'38400'}>38400
	<OPTION VALUE='57600' $selected{'DTERATE'}{'57600'}>57600
	<OPTION VALUE='115200' $selected{'DTERATE'}{'115200'}>115200
	<OPTION VALUE='230400' $selected{'DTERATE'}{'230400'}>230400
	</SELECT>
	</TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'number'}</TD>
	<TD><INPUT TYPE='text' NAME='TELEPHONE' VALUE='$pppsettings{'TELEPHONE'}' id='telephone' @{[jsvalidregex('telephone', '^[0-9\*\#\,]+$', 'true')]}></TD>
	<TD CLASS='base'>$tr{'modem speaker on'}</TD>
	<TD><INPUT TYPE='checkbox' NAME='SPEAKER' VALUE='on' $checked{'SPEAKER'}{'on'}></TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'dialing mode'}</TD>
	<TD>
	<SELECT NAME='DIALMODE'>
	<OPTION VALUE='T' $selected{'DIALMODE'}{'T'}>$tr{'tone'}
	<OPTION VALUE='P' $selected{'DIALMODE'}{'P'}>$tr{'pulse'}
	</SELECT>
	</TD>
	<TD CLASS='base'>$tr{'maximum retries'}</TD>
	<TD><INPUT TYPE='text' NAME='MAXRETRIES' VALUE='$pppsettings{'MAXRETRIES'}' id='maxretries' @{[jsvalidnumber('maxretries','0','10000')]}></TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'idle timeout'}</TD>
	<TD><INPUT TYPE='text' NAME='TIMEOUT' VALUE='$pppsettings{'TIMEOUT'}' id='timeout' @{[jsvalidnumber('timeout','0','10000')]}  ></TD>
	<TD CLASS='base'>$tr{'persistent connection'}</TD>
	<TD><INPUT TYPE='checkbox' NAME='PERSISTENT' VALUE='on' $checked{'PERSISTENT'}{'on'}></TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'dod'}</TD>
	<TD><INPUT TYPE='checkbox' NAME='DIALONDEMAND' VALUE='on' $checked{'DIALONDEMAND'}{'on'}></TD>
	<TD CLASS='base'>$tr{'dod for dns'}</TD>
	<TD><INPUT TYPE='checkbox' NAME='DIALONDEMANDDNS' VALUE='on' $checked{'DIALONDEMANDDNS'}{'on'}></TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'connect on smoothwall restart'}</TD>
	<TD><INPUT TYPE='checkbox' NAME='AUTOCONNECT' VALUE='on'$checked{'AUTOCONNECT'}{'on'}></TD>
	<TD CLASS='base'>$tr{'automatic reboot'}</TD>
	<TD><INPUT TYPE='checkbox' NAME='AUTOREBOOT' VALUE='on' $checked{'AUTOREBOOT'}{'on'}></TD>
</TR>
<TR>
        <TD CLASS='base'>$tr{'send cr'}</TD>
        <TD><INPUT TYPE='checkbox' NAME='SENDCR' $checked{'SENDCR'}{'on'}></TD>
	<TD>&nbsp;</TD>
	<TD>&nbsp;</TD>
</TR>
</TABLE>
END
;

&closebox();

if ($netsettings{'RED_TYPE'} eq 'PPPOE')
{
	&openbox($tr{'pppoe settings'});

print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='25%' CLASS='base'>$tr{'service name'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='text' NAME='SERVICENAME' VALUE='$pppsettings{'SERVICENAME'}'></TD>
        <TD WIDTH='25%' CLASS='base'>$tr{'concentrator name'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='text' NAME='CONCENTRATORNAME' VALUE='$pppsettings{'CONCENTRATORNAME'}'></TD>
</TR>
</TABLE>
END
;
	&closebox();
}

if ($adslsettings{'ENABLED'} eq 'on')
{
	if ($adslsettings{'DEVICE'} eq 'ALCATEL')
	{
		&openbox('Alcatel USB ADSL settings:');

		print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD CLASS='base'>$tr{'firmwarec'}</TD>
END
;
		if (-e "${swroot}/adsl/mgmt.o") {
			print "<TD WIDTH='50%' ALIGN='LEFT' CLASS='base'>$tr{'firmware present'}</TD>\n"; }
		else {
			print "<TD WIDTH='50%' ALIGN='LEFT' CLASS='base'>$tr{'firmware not present'}</TD>\n"; }
		print <<END
	<TD WIDTH='50%' ALIGN='CENTER'><A HREF='/cgi-bin/alcateladslfw.cgi'>$tr{'upload usb adsl firmware'}</A></TD>
</TR>
</TABLE>
END
;
		&closebox();
	}
}

if ($isdnsettings{'ENABLED'} eq 'on')
{
	openbox('ISDN settings:');
	print <<END
<TABLE WIDTH='100%'>
<TR>
<TD WIDTH='25%' CLASS='base'>Keep second channel up:</TD>
<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='STAYUP' $checked{'STAYUP'}{'on'}></TD>
<TD WIDTH='25%' CLASS='base'>Minimum time to keep second channel up (sec):</TD>
<TD WIDTH='25%'><INPUT TYPE='text' NAME='STAYUP_TIME' VALUE='$pppsettings{'STAYUP_TIME'}'></TD>
</TR>
</TABLE>
END
	;
	&closebox();
}

&openbox($tr{'authentication'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='25%' CLASS='base'>$tr{'username'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='text' NAME='USERNAME' VALUE='$pppsettings{'USERNAME'}' id='username' @{[jsvalidregex('username','^[a-zA-Z0-9\.,\(\)@$!\%\^\&\*=\+\/_ ]*$')]}></TD>
	<TD WIDTH='25%' CLASS='base'>$tr{'password'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='password' NAME='PASSWORD' VALUE='$pppsettings{'PASSWORD'}' id='password' @{[jsvalidregex('password','^[a-zA-Z0-9\.,\(\)@$!\%\^\&\*=\+\/_ ]*$')]}></TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'method'}</TD>
	<TD>
	<SELECT NAME='AUTH'>
	<OPTION VALUE='pap-or-chap' $selected{'AUTH'}{'pap-or-chap'}>$tr{'pap or chap'}
	<OPTION VALUE='standard-login-script' $selected{'AUTH'}{'standard-login-script'}>$tr{'standard login script'}
	<OPTION VALUE='demon-login-script' $selected{'AUTH'}{'demon-login-script'}>$tr{'demon login script'}
	<OPTION VALUE='other-login-script' $selected{'AUTH'}{'other-login-script'}>$tr{'other login script'}
	</SELECT>
	</TD>
	<TD CLASS='base'>$tr{'script name'}</TD>
	<TD><INPUT TYPE='text' NAME='LOGINSCRIPT' VALUE='$pppsettings{'LOGINSCRIPT'}'></TD>
</TR>
</TABLE>
END
;
&closebox();

&openbox('DNS:');
print <<END
<script>
function checkdns(option,field1, field2)
{
	var val = document.getElementById(option).value;
	if ( val == 'Automatic' ){
		_ok(field1);
		_ok(field2);
		document.getElementById(field1).disabled = true;
		document.getElementById(field2).disabled = true;
	} else {
		document.getElementById(field1).disabled = false;
		document.getElementById(field2).disabled = false;
		validip(field1,'false');
		validip(field2,'true');
	}
}
</script>
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='25%' CLASS='base'>$tr{'type'}</TD>
	<TD WIDTH='25%' CLASS='base'>
	<INPUT TYPE='radio' NAME='DNS' VALUE='Manual' $checked{'DNS'}{'Manual'} id='r1' onClick="checkdns('r1','dns1', 'dns2')">$tr{'manual'}
	<INPUT TYPE='radio' NAME='DNS' VALUE='Automatic' $checked{'DNS'}{'Automatic'} id='r2' onClick="checkdns('r2','dns1', 'dns2')" >$tr{'automatic'}
	</TD>
	<TD WIDTH='25%'>&nbsp;</TD>
	<TD WIDTH='25%'>&nbsp;</TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'primary dns'}</TD>
	<TD><INPUT TYPE='text' NAME='DNS1' VALUE='$pppsettings{'DNS1'}' id='dns1' @{[jsvalidip('dns1')]}></TD>
	<TD CLASS='base'>$tr{'secondary dns'}</TD>
	<TD><INPUT TYPE='text' NAME='DNS2' VALUE='$pppsettings{'DNS2'}' id='dns2' @{[jsvalidip('dns2','true')]}></TD>
</TR>
</TABLE>
END
;
&closebox();

push @_validation_items, "checkdns('r2','dns1','dns2')";
	
print <<END
<DIV ALIGN='CENTER'>
<TABLE WIDTH='80%'>
<TR>
	<TD ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'save'}'></TD>
</TR>
</TABLE>
</DIV>
END
;

print "</FORM>\n";

&alertbox('add','add');

&closebigbox();

&closepage();

sub updatesettings
{
	# make a link from the selected profile to the "default" one.
 	unlink("${swroot}/ppp/settings");
	link("${swroot}/ppp/settings-$pppsettings{'PROFILE'}",
		"${swroot}/ppp/settings");
}

sub writesecrets
{
	my $stayup;

	# write secrets file.
	open(FILE, ">/${swroot}/ppp/secrets") or die "Unable to write secrets file.";
	flock(FILE, 2);
	my $username = $pppsettings{'USERNAME'};
	my $password = $pppsettings{'PASSWORD'};
        print FILE "'$username' * '$password'\n";
	chmod 0600, "${swroot}/ppp/secrets";
        close FILE;

	# write ibod.cf
	open (FILE, ">${swroot}/ppp/ibod.cf") or die 'Unable to create ibod.cf.';
	flock(FILE, 2);
	if ($pppsettings{'STAYUP'} eq 'on') { $stayup = 1; }
	else { $stayup = 0; }
	print FILE "STAYUP $stayup\n";
	print FILE "STAYUP_TIME $pppsettings{'STAYUP_TIME'}\n";
	close FILE;
}
