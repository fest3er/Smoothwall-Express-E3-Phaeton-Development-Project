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

my (%cgiparams,%selected,%checked);
my $filename = "${swroot}/portfw/config";

&showhttpheaders();

$cgiparams{'COLUMN'} = 1;
$cgiparams{'ORDER'} = $tr{'log ascending'};
$cgiparams{'ENABLED'} = 'off';

my $service = "user";
my $dst_service = "user";

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

	if (defined $cgiparams{'SRC_PORT_SEL'} and $cgiparams{'SRC_PORT_SEL'} ne "user") {
		$cgiparams{'SRC_PORT'} = $cgiparams{'SRC_PORT_SEL'};
	} else {
		unless(&validportrange($cgiparams{'SRC_PORT'})) { $errormessage = $tr{'source port numbers'}; } }

	if (defined $cgiparams{'DEST_PORT_SEL'} and $cgiparams{'DEST_PORT_SEL'} ne "user" ){
		$cgiparams{'DEST_PORT'} = $cgiparams{'DEST_PORT_SEL'}; } 
	else 
	{
		if ($cgiparams{'DEST_PORT'}) {
			unless(&validport($cgiparams{'DEST_PORT'})) { $errormessage = $tr{'destination port numbers'}; } }
		else {
			$cgiparams{'DEST_PORT'} = 0; }
	}

	unless ( &validcomment( $cgiparams{'COMMENT'} ) ){ $errormessage = $tr{'invalid comment'}; }	

	unless (&validip($cgiparams{'DEST_IP'})) { $errormessage = $tr{'destination ip bad'}; }
	open(FILE, $filename) or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);
	my $line;
	foreach $line (@current)
	{
		my @temp = split(/\,/,$line);
		if ($cgiparams{'SRC_PORT'} eq $temp[2] && 
			$cgiparams{'PROTOCOL'} eq $temp[0] &&
			$cgiparams{'EXT'} eq $temp[1])
		{
			$errormessage = "$tr{'source port in use'} $cgiparams{'SRC_PORT'}";
		}
	}

	$service = $cgiparams{'SRC_PORT'};
	$dst_service = $cgiparams{'DEST_PORT'};

	unless ($errormessage)
	{
		open(FILE,">>$filename") or die 'Unable to open config file.';
		flock FILE, 2;
		print FILE "$cgiparams{'PROTOCOL'},$cgiparams{'EXT'},$cgiparams{'SRC_PORT'},$cgiparams{'DEST_IP'},$cgiparams{'DEST_PORT'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
		close(FILE);
		undef %cgiparams;
		$service = '';
		$dst_service = '';

		&log($tr{'forwarding rule added'});
		
		my $success = message('setincoming');
		
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
				$cgiparams{'EXT'} = $temp[1];
				$cgiparams{'SRC_PORT'} = $temp[2];
				$cgiparams{'DEST_IP'} = $temp[3];
				$cgiparams{'DEST_PORT'} = $temp[4];
				$cgiparams{'ENABLED'} = $temp[5];
				$cgiparams{'COMMENT'} = $temp[6];
				$service = $temp[2];
				$dst_service = $temp[4];

			}
		}
		close(FILE);

		&log($tr{'forwarding rule removed'});

		my $success = message('setincoming');
		
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

&openpage($tr{'port forwarding configuration'}, 1, '', 'networking');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'add a new rule'});
print <<END
<table style='width: 100%;'>
<tr>
	<td style='width: 25%;'>$tr{'protocolc'}</td>
	<td style='width: 25%;'>
		<select name='PROTOCOL'>
			<option value='udp' $selected{'PROTOCOL'}{'udp'}>UDP
			<option value='tcp' $selected{'PROTOCOL'}{'tcp'}>TCP
		</select>
	</td>
	<td style='width: 25%;'>$tr{'sourcec'}</td>
	<td style='width: 25%;'><input type='text' name='EXT' value='$cgiparams{'EXT'}' size=18' title='$tr{'sourcec hint'}' id='extaddress' @{[jsvalidipormask('extaddress','true')]}></td>
</tr>
<tr>
	@{[&portlist('SRC_PORT_SEL', $tr{'source port or rangec'}, 'SRC_PORT', $tr{'portc'}, $service, { 'ungrouped' => 'true' })]}
</tr>
<tr>
	<td>$tr{'destination ipc'}</td>
	<td><input type='text' name='DEST_IP' value='$cgiparams{'DEST_IP'}' size='18' id='iaddress' @{[jsvalidip('iaddress')]}></td>
	<td></td>
	<td></td>
</tr>
<tr>
	@{[&portlist('DEST_PORT_SEL', $tr{'destination portc'}, 'DEST_PORT', $tr{'portc'}, $dst_service, { 'ungrouped' => 'true', 'blank' => 'true', 'blob' => 'true' } )]}
</tr>
<tr>
	<td>$tr{'commentc'}</td>
	<td colspan='3'><input type='text' style='width: 80%;' name='COMMENT' value='$cgiparams{'COMMENT'}' id='comment' @{[jsvalidcomment('comment')]}  ></td>
</tr>
<tr>
	<td>$tr{'enabled'}</td>
	<td><input type='checkbox' name='ENABLED' $checked{'ENABLED'}{'on'}></td>
	<td colspan='2' style='text-align: center;'><input type='submit' name='ACTION' value='$tr{'add'}'></td>
</tr>
</table>
<br/>
<img src='/ui/img/blob.gif'>&nbsp;$tr{'portfw destination port'}
END
;
&closebox();

&openbox($tr{'current rules'});

my $portmap = &portmap();

my %render_settings =
(
	'url'     => "/cgi-bin/portfw.cgi?[%COL%],[%ORD%]",
	'columns' =>
	[ 
		{ 
			column => '1',
			title  => "$tr{'protocol'}",
			size   => 10,
			sort   => 'cmp',
			tr     =>  { 'tcp' => 'TCP', 'udp' => 'UDP' },
		},
		{ 
			column => '2',
			title  => "External source IP",
			size   => 15,
			sort   => &ipcompare,
			tr     => { '0.0.0.0/0' => 'ALL' },
		},
		{ 
			column => '3',
			title  => "$tr{'source port'}",
			size   => 20,
			sort   => 'cmp',
			tr     => \%{$portmap}
		},
		{ 
			column => '4',
			title  => "$tr{'destination ip'}",
			size   => 20,
			sort   => &ipcompare,
		},
		{ 
			column => '5',
			title  => "$tr{'destination port'}",
			size   => 15,
			sort   => 'cmp',
			tr     => { %{$portmap}, '0' => 'N/A' },
		},
		{
			column => '6',
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
			column => '7',
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

&alertbox('add', 'add');

&closebigbox();

&closepage();

