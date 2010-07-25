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

my (%cgiparams, %checked, %selected);
my $filename = "${swroot}/xtaccess/config";

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

if ($cgiparams{'ACTION'} eq $tr{'add'})
{
	unless($cgiparams{'PROTOCOL'} =~ /^(tcp|udp)$/) { $errormessage = $tr{'invalid input'}; }
	unless(&validipormask($cgiparams{'EXT'}))
	{
		if ($cgiparams{'EXT'} ne '') {
			$errormessage = $tr{'source ip bad'}; }
		else {
			$cgiparams{'EXT'} = '0.0.0.0/0'; }
	}

	unless ( &validcomment( $cgiparams{'COMMENT'} ) ){ $errormessage = $tr{'invalid comment'}; }	
	unless(&validportrange($cgiparams{'DEST_PORT'})) { $errormessage = $tr{'destination port numbers'}; }
	open(FILE, $filename) or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);
	unless ($errormessage)
	{
		open(FILE,">>$filename") or die 'Unable to open config file.';
		flock FILE, 2;
		print FILE "$cgiparams{'PROTOCOL'},$cgiparams{'EXT'},$cgiparams{'DEST_PORT'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
		close(FILE);
		undef %cgiparams;
		$cgiparams{'COLUMN'} = 1;
		$cgiparams{'ORDER'} = $tr{'log ascending'};
		
		&log($tr{'external access rule added'});

		my $success = message('setxtaccess');
	
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
				$cgiparams{'PROTOCOL'} = $temp[0];
				$cgiparams{'EXT'} = $temp[1];
				$cgiparams{'DEST_PORT'} = $temp[2];
				$cgiparams{'ENABLED'} = $temp[3];
				$cgiparams{'COMMENT'} = $temp[4];
			}
		}
		close(FILE);

		&log($tr{'external access rule removed'});

		my $success = message('setxtaccess');
	
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}
if ($cgiparams{'ACTION'} eq '')
{
	$cgiparams{'PROTOCOL'} = 'tcp';
	$cgiparams{'ENABLED'} = 'on';
}

$selected{'PROTOCOL'}{'udp'} = '';
$selected{'PROTOCOL'}{'tcp'} = '';
$selected{'PROTOCOL'}{$cgiparams{'PROTOCOL'}} = 'SELECTED';

$checked{'ENABLED'}{'off'} = '';
$checked{'ENABLED'}{'on'} = '';  
$checked{'ENABLED'}{$cgiparams{'ENABLED'}} = 'CHECKED';

&openpage($tr{'external access configuration'}, 1, '', 'networking');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print <<END
<!-- 
<CENTER>
<TABLE BORDER='0' CELLPADDING='0' CELLSPACING='0'>
<TR><TD VALIGN='top' ALIGN='CENTER'>
<A HREF='http://www.smoothwall.co.uk/'><IMG
 SRC='/ui/img/inlinepromo.smoothhost.png' BORDER='0'
 ALT='Visit smoothwall.co.uk for enhanced commercial SmoothWall products'></A>
</TD></TR>
</TABLE>
</CENTER>
 -->
END
;

print "<FORM METHOD='POST'>\n";

&openbox($tr{'add a new rule'});
print <<END
<TABLE WIDTH='100%'>
<TR>
<TD>
<SELECT NAME='PROTOCOL'>
<OPTION VALUE='udp' $selected{'PROTOCOL'}{'udp'}>UDP
<OPTION VALUE='tcp' $selected{'PROTOCOL'}{'tcp'}>TCP
</SELECT>
</TD>
<TD CLASS='base'><FONT COLOR='$colourred'>$tr{'sourcec'}</FONT></TD>
<TD><INPUT TYPE='TEXT' NAME='EXT' VALUE='$cgiparams{'EXT'}' SIZE='32' id='ext' @{[jsvalidipormask('ext')]}></TD>
<TD CLASS='base'><FONT COLOR='$colourred'>$tr{'destination portc'}</FONT></TD>
<TD><INPUT TYPE='TEXT' NAME='DEST_PORT' VALUE='$cgiparams{'DEST_PORT'}' SIZE='5' id='dest_port' @{[jsvalidport('dest_port')]}></TD>
</TR>
<tr>
	<td class='base'>$tr{'commentc'}</td>
	<td colspan='3'><input type='text' style='width: 80%;' name='COMMENT' value='$cgiparams{'COMMENT'}' id='comment' @{[jsvalidcomment('comment')]}  ></td>
</tr>
</TABLE>
<TABLE WIDTH='100%'>
<TR>
<TD CLASS='base' WIDTH='50%' ALIGN='CENTER'>$tr{'enabled'}<INPUT TYPE='CHECKBOX' NAME='ENABLED' $checked{'ENABLED'}{'on'}></TD>
<TD WIDTH='50%' ALIGN='CENTER'><INPUT TYPE='SUBMIT' NAME='ACTION' VALUE='$tr{'add'}'></TD>
</TR>
</TABLE>
END
;
&closebox();

&openbox($tr{'current rules'});

my %render_settings =
(
	'url'     => "/cgi-bin/xtaccess.cgi?[%COL%],[%ORD%]",
	'columns' => 
	[
		{ 
			column => '1',
			title  => "$tr{'protocol'}",
			size   => 20,
			tr     => { 'udp' => 'UDP', 'tcp' => 'TCP' },
			sort   => 'cmp',
		},
		{
			column => '2',
			title  => "$tr{'source'}",
			size   => 40,
			sort   => 'cmp',
			tr     => { '0.0.0.0/0' => 'ALL' },
		},
		{
			column => '3',
			title  => "$tr{'destination port'}",
			size   => 20,
		},
		{
			column => '4',
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
			title  => "$tr{'comment'}",
			break  => 'line',
			column => '5',
		},
	]
);

&displaytable($filename, \%render_settings, $cgiparams{'ORDER'}, $cgiparams{'COLUMN'} );

print <<END
<table class='blank'>
<tr>
<td style='text-align: center; width: 50%;'><input type='submit' name='ACTION' value='$tr{'remove'}'></td>
<td style='text-align: center; width: 50%;'><input type='submit' name='ACTION' value='$tr{'edit'}'></td>
</tr>
</table>
END
;
&closebox();

&alertbox('add', 'add');

&closebigbox();

&closepage();

