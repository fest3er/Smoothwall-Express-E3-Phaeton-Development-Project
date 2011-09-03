#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothtype qw(:standard);

&showhttpheaders();

&openpage('VPN configuration - Connections', 1, '', 'vpn');

&openbigbox();

my %cgiparams;
my $filename = "${swroot}/vpn/config";

$cgiparams{'ENABLED'} = 'off';
$cgiparams{'COMPRESSION'} = 'off';
&getcgihash(\%cgiparams);

my $errormessage = '';

if ($cgiparams{'ACTION'} eq $tr{'add'})
{
        unless ($cgiparams{'NAME'} =~ /^[a-zA-Z]+$/) {
                $errormessage .= $tr{'name must only contain characters'}; }
        unless (&validip($cgiparams{'LEFT'}) or $cgiparams{'LEFT'} eq '%any') { 
                $errormessage .= $tr{'left ip is invalid'}; }
        unless (&validip($cgiparams{'LEFT_PRIVATE'})) { 
                $errormessage .= $tr{'left private ip is invalid'}; }
        unless (&validipandmask($cgiparams{'LEFT_SUBNET'})) {
                $errormessage .= $tr{'left subnet is invalid'}; }
        unless (&validip($cgiparams{'RIGHT'}) or $cgiparams{'RIGHT'} eq '%any') { 
                $errormessage .= $tr{'right ip is invalid'}; }
        unless (&validip($cgiparams{'RIGHT_PRIVATE'})) { 
                $errormessage .= $tr{'right private ip is invalid'}; }
        unless (&validipandmask($cgiparams{'RIGHT_SUBNET'})) {
                $errormessage .= $tr{'right subnet is invalid'}; }

        unless ( &validcomment( $cgiparams{'COMMENT'} ) ){ $errormessage .= $tr{'invalid comment'};  }

        unless ($timesettings{'SECRET1'} and ($timesettings{'SECRET1'} =~ /^[\w\d\.\-,\(\)@$!\%\^\&\*=\+_ ]*$/ )) {
                $errormessage .= $tr{'bad password'};
        }

        unless ($timesettings{'SECRET2'} and ($timesettings{'SECRET2'} =~ /^[\w\d\.\-,\(\)@$!\%\^\&\*=\+_ ]*$/ )) {
                $errormessage .= $tr{'bad password'};
        }

        if ($cgiparams{'SECRET1'} ne $cgiparams{'SECRET2'}) {
                $errormessage .= $tr{'passwords do not match'}; }
        unless ($cgiparams{'SECRET1'}) {
                $errormessage .= $tr{'password not set'}; } 
        open(FILE, $filename) or die 'Unable to open config file.';
        my @current = <FILE>;
        close(FILE);
	unless ($errormessage)
        {
                open(FILE,">>$filename") or die 'Unable to open config file.';
                flock FILE, 2;
                print FILE "$cgiparams{'NAME'},$cgiparams{'LEFT'},$cgiparams{'LEFT_SUBNET'},$cgiparams{'RIGHT'},$cgiparams{'RIGHT_SUBNET'},$cgiparams{'SECRET1'},$cgiparams{'ENABLED'},$cgiparams{'COMPRESSION'},$cgiparams{'COMMENT'},$cgiparams{'LEFT_PRIVATE'},$cgiparams{'RIGHT_PRIVATE'}\n";
                close(FILE);
                undef %cgiparams;

		system('/usr/bin/smoothwall/writeipsec.pl');
	}
}
if ($cgiparams{'ACTION'} eq $tr{'remove'} || $cgiparams{'ACTION'} eq $tr{'edit'})
{
	open(FILE, "$filename") or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);

	my $count = 0;
	my $id = 0;
	my $line;
	foreach $line (@current)
	{
		$id++;
		if ($cgiparams{$id} eq "on") {
                        $count++; }
        }
        if ($count == 0) {
                $errormessage .= $tr{'nothing selected'}; }
        if ($count > 1 && $cgiparams{'ACTION'} eq $tr{'edit'}) {
                $errormessage .= $tr{'you can only select one item to edit'}; }
        unless ($errormessage)
        {
                open(FILE, ">$filename") or die 'Unable to open config file.';
		flock FILE, 2;
		my $id = 0;
		foreach $line (@current)
		{
			$id++;
			unless ($cgiparams{$id} eq "on") {
				print FILE "$line"; }
			elsif ($cgiparams{'ACTION'} eq $tr{'edit'})
			{
				chomp($line);
				my @temp = split(/\,/,$line);
				$cgiparams{'NAME'} = $temp[0];
				$cgiparams{'LEFT'} = $temp[1];
				$cgiparams{'LEFT_SUBNET'} = $temp[2];
				$cgiparams{'RIGHT'} = $temp[3];
				$cgiparams{'RIGHT_SUBNET'} = $temp[4];
				$cgiparams{'SECRET1'} = $temp[5];
				$cgiparams{'SECRET2'} = $temp[5];
                                $cgiparams{'ENABLED'} = $temp[6];
                                $cgiparams{'COMPRESSION'} = $temp[7];
                                $cgiparams{'COMMENT'} = $temp[8];
                                $cgiparams{'LEFT_PRIVATE'} = $temp[9];
                                $cgiparams{'RIGHT_PRIVATE'} = $temp[10];
                        }
                }
                close(FILE);

		system('/usr/bin/smoothwall/writeipsec.pl');
	}
}
if ($cgiparams{'ACTION'} eq $tr{'export'})
{
	print "Content-type: unknown/unknown\n\n";
	open (FILE, "$filename");
	my @current = <FILE>;
	close (FILE);
	print @current;
	exit;
}
if ($cgiparams{'ACTION'} eq $tr{'import'})
{
        if (length($cgiparams{'FH'}) > 1)
        {
                open(FILE, ">$filename") or $errormessage .= 
                        'Could not open config file for writing';
                flock FILE, 2;
                binmode(FILE);
                print FILE $cgiparams{'FH'};
		close (FILE);
		
		system('/usr/bin/smoothwall/writeipsec.pl');
	}
}
if ($cgiparams{'ACTION'} eq '')
{
        $cgiparams{'ENABLED'} = 'on';
	$cgiparams{'COMPRESSION'} = 'off';
}

my %checked;
$checked{'ENABLED'}{'off'} = '';
$checked{'ENABLED'}{'on'} = '';
$checked{'ENABLED'}{$cgiparams{'ENABLED'}} = 'CHECKED';

$checked{'COMPRESSION'}{'off'} = '';
$checked{'COMPRESSION'}{'on'} = '';
$checked{'COMPRESSION'}{$cgiparams{'COMPRESSION'}} = 'CHECKED';

&alertbox($errormessage);

print <<END
<!--
<CENTER>
<TABLE BORDER='0' CELLPADDING='0' CELLSPACING='0'>
<TR><TD VALIGN='top' ALIGN='CENTER'>
<A HREF='http://www.smoothwall.co.uk/'><IMG
 SRC='/ui/img/inlinepromo.smoothtunnel.png' BORDER='0'
 ALT='Visit smoothwall.co.uk for enhanced commercial SmoothWall products'></A>
</TD></TR>
</TABLE>
</CENTER>
 -->
END
;


print "<FORM METHOD='POST'>\n";

&openbox($tr{'add a new connection'});
print <<END
<div style='margin:1em 2em .5em 1em; font-size:8pt'>
  <div style='width:5em; margin:.5em .5em 0 1em;
              font-size:8pt; text-align:right; display:inline-block'>
    $tr{'enabled'}
  </div>
  <input type='checkbox' name='ENABLED' $checked{'ENABLED'}{'on'}
         style='margin:.5em 2em 0 0'>
  <div style='margin:.5em 0 0 2em;
              font-size:8pt; text-align:right; display:inline-block'>
    Compression:
    <input type='checkbox' name='COMPRESSION' $checked{'COMPRESSION'}{'on'}>
  </div>
</div>
<div style='margin:0 2em 0 1em; font-size:8pt'>
  <div style='width:5em; margin:.5em .5em 0 1em;
              font-size:8pt; text-align:right; display:inline-block'>
    $tr{'namec'}
  </div>
  <input type='text' name='NAME' value='$cgiparams{'NAME'}'
         style='margin:.5em 0 0 0'
         id='name' @{[jsvalidregex('name','^[a-zA-Z]+$')]}>
</div>
<div style='margin:0 2em .5em 1em; font-size:8pt'>
  <div style='width:5em; margin:.5em .5em 0 1em; font-size:8pt; text-align:right; display:inline-block'>
    $tr{'commentc'}
  </div>
  <input type='text' name='COMMENT' value='$cgiparams{'COMMENT'}'
         style='margin:.25em 0 0 0; width:25em'
         id='comment' @{[jsvalidcomment('comment')]}>
</div>
<div style='width:48%; margin:.5em 0 0 2em; spacing:0 0 0 5em; display:inline-block'>
  <div>
    <div style='width:10em; margin:.5em .5em 0 0; text-align:right; display:inline-block'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'leftc'}
    </div>
    <input type='text' name='LEFT' value='$cgiparams{'LEFT'}'
           style='margin:.5em 2em 0 0'
           id='left' @{[jsvalidip('left')]}>
  </div>
  <div style='margin:.25em 0 0 0'>
    <div style='width:10em; margin:.5em .5em 0 0; text-align:right; display:inline-block'>
      <img src='/ui/img/blob.gif' valign='top'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'leftprivc'}
    </div>
    <input type=text name='LEFT_PRIVATE' value='$cgiparams{'LEFT_PRIVATE'}'
           id='left_private' @{[jsvalidip('left_private')]}>
  </div>
  <div style='margin:.25em 0 0 0'>
    <div style='width:10em; margin:.5em .5em 0 0; text-align:right; display:inline-block'>
      $tr{'left subnetc'}
    </div>
    <input type='text' name='LEFT_SUBNET' value='$cgiparams{'LEFT_SUBNET'}'
           id='left_subnet' @{[jsvalidipandmask('left_subnet')]}>
  </div>
</div>
<div style='width:48%; margin:.5em 0 .25em 0; spacing:0 5em 1em 0; display:inline-block'>
  <div>
    <div style='width:10em; margin:0 .5em 0 0; text-align:right; display:inline-block'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'rightc'}
    </div>
    <input type='text' name='RIGHT' value='$cgiparams{'RIGHT'}'
           style='margin:.5em 2em 0 0'
           id='right' @{[jsvalidip('right')]}>
  </div>
  <div style='margin:.25em 0 0 0'>
    <div style='width:10em; margin:.5em .5em 0 0; text-align:right; display:inline-block'>
      <img src='/ui/img/blob.gif' valign='top'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'rightprivc'}
    </div>
    <input type=text name='RIGHT_PRIVATE' value='$cgiparams{'RIGHT_PRIVATE'}'
           id='right-private' @{[jsvalidip('right-private')]}>
  </div>
  <div style='margin:.25em 0 0 0'>
    <div style='width:10em; margin:.5em .5em 0 0; text-align:right; display:inline-block'>
      $tr{'right subnetc'}
    </div>
    <input type='text' name='RIGHT_SUBNET' value='$cgiparams{'RIGHT_SUBNET'}'
           id='right_subnet' @{[jsvalidipandmask('right_subnet')]}>
  </div>
</div>
<div style='margin:.25em 0 0 20em; display:inline-block'>
  <p style='margin:0'>
    <div style='width:3em; margin:0 .25em 0 0; text-align:right; display:inline-block'>
      <img src='/ui/img/blob.gif' valign='top' style='margin:0'>
    </div>
    Enter
    <span style='font-family:courier-new,courier,fixed; font-size:10pt'>
      <b>%any</b>
    </span>
    if NATted; enter the public IP address otherwise.
  <br />
    <div style='width:3em; margin:0 .25em 0 0; text-align:right; display:inline-block'>
      <img src='/ui/img/blob.gif' valign='top' style='margin:0'><img src='/ui/img/blob.gif' valign='top' style='margin:0'>
    </div>
    Enter the private IP address if NATted; leave blank otherwise.
  </p>
</div>

<div style='margin:.5em 0 0 1em'>
  <div>
    <div style='width:5em; margin:.5em .5em 0 1em;
                text-align:right; display:inline-block'>
      $tr{'secretc'}
    </div>
    <input type='PASSWORD' name='SECRET1' value='$cgiparams{'SECRET1'}'
           style='width:35em; margin:.25em 0 0 0'
           id='secret1' @{[jsvalidpassword('secret1','secret2','^[a-zA-Z0-9\.,\(\)@$!\%\^\&\*=\+_ ]*$')]}>
  </div>
  <div style='margin:.25em 0 0 0'>
    <div style='width:5em; margin:.5em .5em 0 1em;
                text-align:right; display:inline-block'>
      $tr{'again'}
    </div>
    <input type='PASSWORD' name='SECRET2' value='$cgiparams{'SECRET2'}'
           style='width:35em'
           id='secret2' @{[jsvalidpassword('secret2','secret1','^[a-zA-Z0-9\.,\(\)@$!\%\^\&\*=\+_ ]*$')]}>
  </div>
</div>
    
<div style='margin:2em 0 1em 0; text-align:center; width:100%'>
  <input type='submit' name='ACTION' value='$tr{'add'}'>
  </div>
END
;
&closebox();

&openbox($tr{'current connections'});

print "<table class='centered'>\n";

my $id = 0;
open(RULES, "$filename") or die 'Unable to open config file.';
while (<RULES>)
{
	my $egif; my $cgif;
	$id++;
	chomp($_);
	my @temp = split(/\,/,$_);
	my $class;
	if ($id % 2) {
		$class = 'light'; }
	else {
		$class = 'dark'; }
 	if ($temp[6] eq 'on') { $egif = 'on.gif'; }
                else { $egif = 'off.gif'; }
 	if ($temp[7] eq 'on') { $cgif = 'on.gif'; }
                else { $cgif = 'off.gif'; }
print <<END
<tr class='$class'>
<td style='width: 25%;'>$tr{'namec'} <strong>$temp[0]</strong></td>
<td style='width: 25%;'>$tr{'enabled'} <img src='/ui/img/$egif'></td>
<td style='width: 25%;'>Compression: <img src='/ui/img/$cgif'></td>
<td style='width: 25%;'>&nbsp;</td>
</tr>
<tr class='$class'>
<td>$tr{'leftc'} <strong>$temp[1]</strong></td>
<td>$tr{'left subnetc'} <strong> $temp[2]</strong></td>
<td>$tr{'rightc'} <strong>$temp[3]</strong></td>
<td>$tr{'right subnetc'} <strong>$temp[4]</strong></td>
</tr>
<tr class='$class'>
<td>$tr{'leftprivc'} <strong>$temp[9]</strong></td>
<td>&nbsp;</td>
<td>$tr{'rightprivc'} <strong>$temp[10]</strong></td>
<td>&nbsp;</td>
</tr>
<tr class='$class'>
<td colspan='3'>$tr{'commentc'} <strong>$temp[8]</strong></td>
<td><strong>$tr{'markc'} </strong><input type='checkbox' name='$id'></td>
</tr>
END
	;
}
close(RULES);

print <<END
</table>
<table class='blank'>
<tr>
<td style='text-align: center;'><input type='submit' name='ACTION' value='$tr{'remove'}'></td>
<td style='text-align: center;'><input type='submit' name='ACTION' value='$tr{'edit'}'></td>
</tr>
</table>
END
;
&closebox();

&openbox($tr{'import and export'});
print <<END
<DIV ALIGN='CENTER'>
<TABLE WIDTH='80%'>
<TR>
<TD ALIGN='LEFT'><INPUT TYPE='SUBMIT' NAME='ACTION' VALUE='$tr{'export'}'></TD>
</FORM>
<FORM METHOD='POST' ENCTYPE='multipart/form-data'>
<TD ALIGN='RIGHT'><INPUT TYPE='FILE' NAME='FH' SIZE='30'>
<INPUT TYPE='SUBMIT' NAME='ACTION' VALUE='$tr{'import'}'></TD>
</FORM>
</TR>
</TABLE>
</DIV>
END
;
&closebox();

&alertbox('add','add');

&closebigbox();

&closepage();
