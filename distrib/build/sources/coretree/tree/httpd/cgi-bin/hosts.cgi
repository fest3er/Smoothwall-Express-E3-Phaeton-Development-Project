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
use smoothtype qw(:standard);

my (%cgiparams,%selected,%checked);
my $filename = "${swroot}/hosts/config";

&showhttpheaders();

$cgiparams{'ENABLED'} = 'off';

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
	unless(&validip($cgiparams{'IP'})) { $errormessage = $tr{'ip address not valid'}; }
	unless(&validhostname($cgiparams{'HOSTNAME'})) { $errormessage = $tr{'invalid hostname'}; }

	unless ( &validcomment( $cgiparams{'COMMENT'} ) ){ $errormessage = $tr{'invalid comment'};  }

	unless ($errormessage)
	{
		open(FILE,">>$filename") or die 'Unable to open config file.';
		flock FILE, 2;
		print FILE "$cgiparams{'IP'},$cgiparams{'HOSTNAME'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
		close(FILE);
		undef %cgiparams;
		$cgiparams{'COLUMN'} = 1;
		$cgiparams{'ORDER'} = $tr{'log ascending'};
		&log($tr{'host added to hosts list.'});
		
		system('/usr/bin/smoothwall/writehosts.pl');

		my $success = message('dnsproxyhup');
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }               
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
				$cgiparams{'IP'} = $temp[0];
				$cgiparams{'HOSTNAME'} = $temp[1];
				$cgiparams{'ENABLED'} = $temp[2];
				$cgiparams{'COMMENT'} = $temp[3];
			}
		}
		close(FILE);
		&log($tr{'host removed from host list'});

		system('/usr/bin/smoothwall/writehosts.pl');

		my $success = message('dnsproxyhup');
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}

if ($cgiparams{'ACTION'} eq '')
{
	$cgiparams{'ENABLED'} = 'on';
}

$checked{'ENABLED'}{'off'} = '';
$checked{'ENABLED'}{'on'} = '';  
$checked{'ENABLED'}{$cgiparams{'ENABLED'}} = 'CHECKED';

&openpage($tr{'static dns configuration'}, 1, '', 'services');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'add a host'});

print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD CLASS='base'>$tr{'ip addressc'}</TD>
	<TD><INPUT TYPE='text' NAME='IP' VALUE='$cgiparams{'IP'}' id='ip' @{[jsvalidip('ip')]}></TD>
	<TD CLASS='base'>$tr{'hostnamec'}</TD>
	<TD><INPUT TYPE='text' NAME='HOSTNAME' VALUE='$cgiparams{'HOSTNAME'}' id='hostname' @{[jsvalidregex('hostname','^[a-zA-Z_0-9-\.]+$')]}></TD>
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
	'url'     => "/cgi-bin/hosts.cgi?[%COL%],[%ORD%]",
	'columns' => 
	[
		{ 
                        column => '1',
                        title  => "$tr{'ip address'}",
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
                        column => '4',
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
END
;
&closebox();

&alertbox('add','add');

&closebigbox();

&closepage();
