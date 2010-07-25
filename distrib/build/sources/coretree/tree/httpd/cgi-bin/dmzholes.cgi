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
use smoothd qw( message );

my (%cgiparams,%checked,%selected);
my $filename = "${swroot}/dmzholes/config";

&showhttpheaders();

$cgiparams{'COLUMN'} = 1;
$cgiparams{'ORDER'} = $tr{'log ascending'};

$cgiparams{'ENABLED'} = 'off';
&getcgihash(\%cgiparams);

if ($ENV{'QUERY_STRING'} && ( not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ))
{
        my @temp = split(',',$ENV{'QUERY_STRING'});
        $cgiparams{'ORDER'}  = $temp[1] if ( defined $temp[ 1 ] and $temp[ 1 ] ne "" );
        $cgiparams{'COLUMN'} = $temp[0] if ( defined $temp[ 0 ] and $temp[ 0 ] ne "" );
}

my $errormessage = '';
my $service = "user";

if ($cgiparams{'ACTION'} eq $tr{'add'})
{
	unless($cgiparams{'PROTOCOL'} =~ /^(tcp|udp)$/) { $errormessage = $tr{'invalid input'}; }
	unless(&validipormask($cgiparams{'SRC_IP'})) { $errormessage = $tr{'source ip bad'}; }

	if ( defined $cgiparams{'SERVICE'} and $cgiparams{'SERVICE'} ne "user" ){
		$cgiparams{'DEST_PORT'} = $cgiparams{'SERVICE'};
	} else {
		unless(&validportrange($cgiparams{'DEST_PORT'})) { $errormessage = $tr{'destination port numbers'}; }
	}
	unless(&validipormask($cgiparams{'DEST_IP'})) { $errormessage = $tr{'destination ip bad'}; }
	unless ( &validcomment( $cgiparams{'COMMENT'} ) ){ $errormessage = $tr{'invalid comment'}; }	
	
	open(FILE, $filename) or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);
	unless ($errormessage)
	{
		open(FILE,">>$filename") or die 'Unable to open config file.';
		flock FILE, 2;
		print FILE "$cgiparams{'PROTOCOL'},$cgiparams{'SRC_IP'},$cgiparams{'DEST_IP'},$cgiparams{'DEST_PORT'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
		close(FILE);
		undef %cgiparams;
		
		&log($tr{'dmz pinhole rule added'});

		my $success = message('setinternal');
		
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
		$id = 0;
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
				$cgiparams{'SRC_IP'} = $temp[1];
				$cgiparams{'DEST_IP'} = $temp[2];
				$cgiparams{'DEST_PORT'} = $temp[3];
				$cgiparams{'ENABLED'} = $temp[4];
				$cgiparams{'COMMENT'} = $temp[5];
				$service = $temp[3];
			}
		}
		close(FILE);

		&log($tr{'dmz pinhole rule removed'});

		my $success = message('setinternal');
		
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

&openpage($tr{'dmz pinhole configuration'}, 1, '', 'networking');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'add a new rule'});
print <<END
<table style='width: 100%;'>
<tr>
	<td class='base'>$tr{'source ip or networkc'}</td>
	<td><input type='text' name='SRC_IP' value='$cgiparams{'SRC_IP'}'id='iaddress' @{[jsvalidipormask('iaddress')]}></td>
	<td class='base'>$tr{'protocolc'}</td>
	<td>
		<SELECT NAME='PROTOCOL'>
			<OPTION VALUE='udp' $selected{'PROTOCOL'}{'udp'}>UDP
			<OPTION VALUE='tcp' $selected{'PROTOCOL'}{'tcp'}>TCP
		</SELECT>
	</td>
</tr>
<tr>
	<td class='base'>$tr{'destination ip or networkc'}</td>
	<td><input type='text' name='DEST_IP' value='$cgiparams{'DEST_IP'}' id='dstiaddress' @{[jsvalidipormask('dstiaddress')]}></td>
</tr>
<tr>
	@{[&portlist('SERVICE', $tr{'application servicec'}, 'DEST_PORT', $tr{'destination portc'}, $service, { blank => 'true'} )]}
</tr>
<tr>
	<td class='base'>$tr{'commentc'}</td>
	<td colspan='3'><input type='text' style='width: 80%;' name='COMMENT' value='$cgiparams{'COMMENT'}' id='comment' @{[jsvalidcomment('comment')]}  ></td>
</tr>
<tr>
	<td class='base'>$tr{'enabled'}</td>
        <td><input type='checkbox' name='ENABLED' $checked{'ENABLED'}{'on'}></td>
	<td colspan=2 style='text-align: left;'><input type='submit' name='ACTION' value='$tr{'add'}'></td>
</tr>
</table>
END
;
&closebox();

&openbox($tr{'current rules'});

my $portmap = &portmap();

my %render_settings =
(
	'url'     => "/cgi-bin/dmzholes.cgi?[%COL%],[%ORD%]",
	'columns' =>
	[
		{ 
			column => '1',
			title  => "$tr{'protocol'}",
			size   => 15,
			tr     => { 'udp' => 'UDP', 'tcp' => 'TCP' },
			sort   => 'cmp',
		},
		{
			column => '2',
			title  => "$tr{'source ip'}",
			size   => 20,
			sort   => &ipcompare,
		},
		{
			column => '3',
			title  => "$tr{'destination ip'}",
			size   => 20,
			sort   => &ipcompare,
		},
		{
			column => '4',
			title  => "$tr{'destination port'}",
			size   => 15,
			sort   => 'cmp',
			tr     => \%{$portmap}
		},
		{
			column => '5',
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
			column => '6',
			title => "$tr{'comment'}",
			break => 'line',
		}
	]
);

&displaytable( $filename, \%render_settings, $cgiparams{'ORDER'}, $cgiparams{'COLUMN'} );

print <<END
<table class='blank'>
<tr>
<td style='width: 50%; text-align: center;'><input type='submit' name='ACTION' value='$tr{'remove'}'></td>
<td style='width: 50%; text-align: center;'><input type='submit' name='ACTION' value='$tr{'edit'}'></td>
</tr>
</table>
END
;
&closebox();

&alertbox('add','add');

&closebigbox();

&closepage();

