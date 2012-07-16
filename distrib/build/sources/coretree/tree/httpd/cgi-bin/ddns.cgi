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

my (%cgiparams,%selected,%checked);
my $filename = "${swroot}/ddns/config";

&showhttpheaders();

$cgiparams{'ENABLED'} = 'off';
$cgiparams{'PROXY'} = 'off';
$cgiparams{'WILDCARDS'} = 'off';

$cgiparams{'COLUMN'} = 1;
$cgiparams{'ORDER'} = $tr{'log ascending'};

&getcgihash(\%cgiparams);

if ($ENV{'QUERY_STRING'} && ( not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ))
{
	my @temp = split(',',$ENV{'QUERY_STRING'});
	$cgiparams{'ORDER'}  = $temp[1] if ( defined $temp[ 1 ] and $temp[ 1 ] ne "" );
	$cgiparams{'COLUMN'} = $temp[0] if ( defined $temp[ 0 ] and $temp[ 0 ] ne "" );
}

my $errormessage = '';
my @service = ();

if ($cgiparams{'ACTION'} eq $tr{'add'})
{
	unless ($cgiparams{'SERVICE'} =~ /^(dhs|dyndns-custom|dyndns|dyns|hn|no-ip|zoneedit|easydns|ods)$/) { $errormessage = $tr{'invalid input'}; }
	unless ($cgiparams{'LOGIN'} =~ /^[^\"\']*$/) { $errormessage = $tr{'invalid username'}; }
	unless ($cgiparams{'LOGIN'} ne '') { $errormessage = $tr{'username not set'}; }
	unless ($cgiparams{'PASSWORD'} ne '') { $errormessage = $tr{'password not set'}; }
	unless ($cgiparams{'PASSWORD'} =~ /^[^\s\"\']*$/) { $errormessage = $tr{'invalid username'}; }
	unless ($cgiparams{'DOMAIN'} ne '') { $errormessage = $tr{'domain not set'}; }
	unless ($cgiparams{'DOMAIN'} =~ /^[a-zA-Z_0-9.-]+$/) { $errormessage = $tr{'invalid domain name'}; }
	unless ($cgiparams{'DOMAIN'} =~ /[.]/) { $errormessage = $tr{'invalid domain name'}; }
	unless ( &validcomment( $cgiparams{'COMMENT'} ) ){ $errormessage = $tr{'invalid comment'};  }
	open(FILE, $filename) or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);
	my $line;
	foreach $line (@current)
	{
		my @temp = split(/\,/,$line);
		if($cgiparams{'HOSTNAME'} eq $temp[1] &&
			$cgiparams{'DOMAIN'} eq $temp[2])
		{
			 $errormessage = $tr{'hostname and domain already in use'};
		}
	}
	unless ($errormessage)
	{
		open(FILE,">>$filename") or die 'Unable to open config file.';
		flock FILE, 2;
		print FILE "$cgiparams{'SERVICE'},$cgiparams{'HOSTNAME'},$cgiparams{'DOMAIN'},$cgiparams{'PROXY'},$cgiparams{'WILDCARDS'},$cgiparams{'LOGIN'},$cgiparams{'PASSWORD'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
		close(FILE);
		undef %cgiparams;
		$cgiparams{'COLUMN'} = 1;
		$cgiparams{'ORDER'} = $tr{'log ascending'};
		&log($tr{'ddns hostname added'});
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
		$errormessage = $tr{'nothing selected'}; }
	if ($count > 1 && $cgiparams{'ACTION'} eq $tr{'edit'}) {
		$errormessage = $tr{'you can only select one item to edit'}; }
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
				$cgiparams{'SERVICE'} = $temp[0];
				$cgiparams{'HOSTNAME'} = $temp[1];
				$cgiparams{'DOMAIN'} = $temp[2];
				$cgiparams{'PROXY'} = $temp[3];
				$cgiparams{'WILDCARDS'} = $temp[4];
				$cgiparams{'LOGIN'} = $temp[5];
				$cgiparams{'PASSWORD'} = $temp[6];
				$cgiparams{'ENABLED'} = $temp[7];
				$cgiparams{'COMMENT'} = $temp[8];
			}
		}
		close(FILE);
		&log($tr{'ddns hostname removed'});
	}
}

if ($cgiparams{'ACTION'} eq $tr{'force update'})
{
	system('/usr/bin/smoothwall/setddns.pl', '-f');
}

if ($cgiparams{'ACTION'} eq '')
{
	$cgiparams{'ENABLED'} = 'on';
}

$selected{'SERVICE'}{'dhs'} = '';
$selected{'SERVICE'}{'dyndns'} = '';
$selected{'SERVICE'}{'dyndns-custom'} = '';
$selected{'SERVICE'}{'dyns'} = '';
$selected{'SERVICE'}{'hn'} = '';
$selected{'SERVICE'}{'no-ip'} = '';
$selected{'SERVICE'}{'zoneedit'} = '';
$selected{'SERVICE'}{'easydns'} = '';
$selected{'SERIVCE'}{'ods'} = '';
$selected{'SERVICE'}{$cgiparams{'SERVICE'}} = 'SELECTED';

$checked{'PROXY'}{'off'} = '';
$checked{'PROXY'}{'on'} = '';
$checked{'PROXY'}{$cgiparams{'PROXY'}} = 'CHECKED';

$checked{'WILDCARDS'}{'off'} = '';
$checked{'WILDCARDS'}{'on'} = '';
$checked{'WILDCARDS'}{$cgiparams{'WILDCARDS'}} = 'CHECKED';

$checked{'ENABLED'}{'off'} = '';
$checked{'ENABLED'}{'on'} = '';  
$checked{'ENABLED'}{$cgiparams{'ENABLED'}} = 'CHECKED';

&openpage($tr{'dynamic dns'}, 1, '', 'services');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'add a host'});

print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='25%' CLASS='base'>$tr{'servicec'}</TD>
	<TD WIDTH='25%'>
	<SELECT SIZE='1' NAME='SERVICE'>
	<OPTION VALUE='dhs' $selected{'SERVICE'}{'dhs'}>dhs.org
	<OPTION VALUE='dyndns' $selected{'SERVICE'}{'dyndns'}>dyndns.org
	<OPTION VALUE='dyndns-custom' $selected{'SERVICE'}{'dyndns-custom'}>dyndns.org (Custom)
	<OPTION VALUE='dyns' $selected{'SERVICE'}{'dyns'}>dyns.cx
	<OPTION VALUE='hn' $selected{'SERVICE'}{'hn'}>hn.org
	<OPTION VALUE='no-ip' $selected{'SERVICE'}{'no-ip'}>no-ip.com
	<OPTION VALUE='zoneedit' $selected{'SERVICE'}{'zoneedit'}>zonedit.com
	<OPTION VALUE='easydns' $selected{'SERVICE'}{'easydns'}>easydns.com
	<OPTION VALUE='ods' $selected{'SERVICE'}{'ods'}>ods.org
	</SELECT>
	</TD>
	<TD WIDTH='25%' CLASS='base'>$tr{'behind a proxy'} <INPUT TYPE='checkbox' NAME='PROXY' VALUE='on' $checked{'PROXY'}{'on'}></TD>
	<TD WIDTH='25%' CLASS='base'>$tr{'enable wildcards'} <INPUT TYPE='checkbox' NAME='WILDCARDS' VALUE='on' $checked{'WILDCARDS'}{'on'}></TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'hostnamec'}</TD>
	<TD><INPUT TYPE='text' NAME='HOSTNAME' VALUE='$cgiparams{'HOSTNAME'}' id='hostname' @{[jsvalidregex('hostname','^[a-zA-Z_0-9-]+$','true')]}></TD>
	<TD CLASS='base'>$tr{'domainc'}</TD>
	<TD><INPUT TYPE='text' NAME='DOMAIN' VALUE='$cgiparams{'DOMAIN'}' id='domain' @{[jsvalidregex('domain','^[a-zA-Z_0-9-\.]+$')]}></TD>
</TR>
<TR>
	<TD CLASS='base'>$tr{'username'}</TD>
	<TD><INPUT TYPE='text' NAME='LOGIN' VALUE='$cgiparams{'LOGIN'}' id='login' @{[jsvalidregex('login','^[a-zA-Z0-9\@\s~#!\(\)&^\%\$£\*]+$')]}></TD>
	<TD CLASS='base'>$tr{'password'}</TD>
	<TD><INPUT TYPE='PASSWORD' NAME='PASSWORD' VALUE='$cgiparams{'PASSWORD'}' id='password' @{[jsvalidregex('password','^[a-zA-Z0-9\@\s~#!\(\)&^\%\$£\*]+$')]}></TD>
</TR>
<TR>
	<td class='base'>$tr{'commentc'}</td>
	<td colspan='3'><input type='text' style='width: 80%;' name='COMMENT' id='comment' @{[jsvalidcomment('comment')]} value='$cgiparams{'COMMENT'}'></td>
</tr>
</TABLE>
<TABLE WIDTH='100%'>
<TR>
	<TD CLASS='base' WIDTH='50%' ALIGN='CENTER'>$tr{'enabled'}<INPUT TYPE='checkbox' NAME='ENABLED' VALUE='on' $checked{'ENABLED'}{'on'}></TD>
	<TD WIDTH='50%' ALIGN='CENTER'><INPUT TYPE='SUBMIT' NAME='ACTION' VALUE='$tr{'add'}'></TD>
</TR>
</TABLE>
END
;
&closebox();

&openbox($tr{'current hosts'});

my %render_settings =
(
	'url'     => "/cgi-bin/ddns.cgi?[%COL%],[%ORD%]",
	'columns' => 
	[
		{ 
                        column => '1',
                        title  => "$tr{'service'}",
                        size   => 15,
                        valign => 'top',
                        maxrowspan => 2,
                        sort   => 'cmp',
                },
                {
			column => '2',
			title  => "$tr{'hostname'}",
			size   => 20,
			sort   => 'cmp'
		},
		{
			column => '3',
			title  => "$tr{'domain'}",
			size   => 25,
			sort   => 'cmp'
		},
		{
			column => '4',
			title  => "$tr{'proxy'}",
			tr     => 'onoff',
			size   => 10,
			align  => 'center',
		},
		{
			column => '5',
			title  => "$tr{'wildcards'}",
			tr     => 'onoff',
			size   => 10,
			align  => 'center',
		},
		{
			column => '8',
			title  => "$tr{'enabledtitle'}",
			size   => 10,
			tr     => 'onoff',
			align  => 'center',
		},
		{
			title  => "$tr{'mark'}", 
			size   => 10,
			mark   => ' ',
		},
		{ 
                        column => '9',
                        title => "$tr{'comment'}",
                        break => 'line',
                        spanadj => -1,
                },

        ]
);

&dispaliastab($filename, \%render_settings, $cgiparams{'ORDER'}, $cgiparams{'COLUMN'} );

print <<END
<table class='blank'>
<tr>
<td style='width: 50%; text-align:center;'><input type='submit' name='ACTION' value='$tr{'remove'}'></td>
<td style='width: 50%; text-align:center;'><input type='submit' name='ACTION' value='$tr{'edit'}'></td>
</tr>
</table>
<table class='blank'>
<tr>
<td style='text-align: center;'><input type='submit' name='ACTION' value='$tr{'force update'}'></td>
</tr>
</table>
END
;
&closebox();

&alertbox('add','add');

&closebigbox();

&closepage();


