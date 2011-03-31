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

my (%cgiparams, %selected, %checked);

my $config = "${swroot}/outgoing/config";
my $machineconfig = "${swroot}/outgoing/machineconfig";

my $errormessage = '';

&showhttpheaders();

$cgiparams{'COLUMN_ONE'} = 1;
$cgiparams{'ORDER_ONE'} = $tr{'log ascending'};
$cgiparams{'COLUMN_TWO'} = 1;
$cgiparams{'ORDER_TWO'} = $tr{'log ascending'};

$cgiparams{'RULEENABLED'} = 'off';
$cgiparams{'MACHINEENABLED'} = 'off';

&getcgihash(\%cgiparams);

$cgiparams{'RULEENABLED'} = 'on'     if ( not defined $cgiparams{'ACTION'} );
$cgiparams{'MACHINEENABLED'} = 'on'  if ( not defined $cgiparams{'MACHINEACTION'} );

if ($ENV{'QUERY_STRING'} && ( not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ))
{
	my @temp = split(',',$ENV{'QUERY_STRING'});
	$cgiparams{'ORDER_ONE'}  = $temp[1] if ( defined $temp[ 1 ] and $temp[ 1 ] ne "" );
	$cgiparams{'COLUMN_ONE'} = $temp[0] if ( defined $temp[ 0 ] and $temp[ 0 ] ne "" );
	$cgiparams{'ORDER_TWO'}  = $temp[3] if ( defined $temp[ 3 ] and $temp[ 1 ] ne "" );
	$cgiparams{'COLUMN_TWO'} = $temp[2] if ( defined $temp[ 2 ] and $temp[ 0 ] ne "" );
}

# Load inbound interfaces into %interfaces (excluding RED)
my %netsettings;

&readhash("${swroot}/ethernet/settings", \%netsettings);

my %interfaces;

$interfaces{'GREEN'} = $netsettings{'GREEN_DEV'};
$interfaces{'ORANGE'} = $netsettings{'ORANGE_DEV'};
$interfaces{'PURPLE'} = $netsettings{'PURPLE_DEV'};


# Save the settings as is required.

if ( defined $cgiparams{'ACTION'} and $cgiparams{'ACTION'} eq $tr{'save'} )
{
	my %settings;
	
	$settings{'GREEN'} = $cgiparams{'GREEN'};
	$settings{'ORANGE'} = $cgiparams{'ORANGE'};
	$settings{'PURPLE'} = $cgiparams{'PURPLE'};
		
	&writehash("${swroot}/outgoing/settings", \%settings);
	
	my $success = message('setoutgoing');
	
	unless (defined $success) {
		$errormessage = $tr{'smoothd failure'}; }
}

&readhash("${swroot}/outgoing/settings", \%cgiparams);

my %selected;
$selected{'GREEN'}{$cgiparams{'GREEN'}} = " selected ";
$selected{'ORANGE'}{$cgiparams{'ORANGE'}} = " selected ";
$selected{'PURPLE'}{$cgiparams{'PURPLE'}} = " selected ";

my %checked;
$checked{'on'} = " checked ";

my $errormessage = "";

if ( defined $cgiparams{'ACTION'} and $cgiparams{'ACTION'} eq $tr{'add'} )
{
	my $interface = $cgiparams{'INTERFACE'};
	my $enabled   = $cgiparams{'RULEENABLED'};
	my $service   = $cgiparams{'SERVICE'};
	my $port      = $cgiparams{'PORT'};
	my $comment   = $cgiparams{'RULECOMMENT'};

	if ( $service eq "user" )
	{
		if ( not &validportrange( $port ) ){
			$errormessage = $tr{'invalid port or range'};
		} else {
			$service = $port; }
	}

	if ( $errormessage eq "" )
	{	
		open(FILE,">>$config") or die 'Unable to open config file.';
		flock FILE, 2;
		print FILE "$interface,$enabled,$service,$comment\n";
		close(FILE);
		
		my $success = message('setoutgoing');
	
		unless (defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}

	$cgiparams{'INTERFACE'} = "GREEN";
	$cgiparams{'RULEENABLED'} = "on";
	$cgiparams{'RULECOMMENT'} = "";

}

my $service = 'user';

if ( defined $cgiparams{'ACTION'} and $cgiparams{'ACTION'} eq $tr{'edit'} or
	$cgiparams{'ACTION'} eq $tr{'remove'})
{
	open(FILE, "$config") or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);

	foreach $line (@current)
	{
		$id++;
		if ($cgiparams{$id} eq "on") {
			$count++; 
		}
	}

	if ($count == 0) {
		$errormessage = $tr{'nothing selected'};  }
	if ($count > 1 && $cgiparams{'ACTION'} eq $tr{'edit'}) {
		$errormessage = $tr{'you can only select one item to edit'};  }
	
	unless ($errormessage)
	{
		open(FILE, ">$config") or die 'Unable to open config file.';
		flock FILE, 2;
		$id = 0;
		foreach $line (@current)
		{
			$id++;
			unless ($cgiparams{$id} eq "on") {
				print FILE "$line"; 
			} elsif ($cgiparams{'ACTION'} eq $tr{'edit'}) {
				chomp($line);
				my @temp = split(/\,/,$line);
				$cgiparams{'INTERFACE'} = $temp[0];
				$cgiparams{'RULEENABLED'} = $temp[1];
				$cgiparams{'RULECOMMENT'} = $temp[3];
				$service = $temp[2];
			}
		}
		close(FILE);

		my $success = message('setoutgoing');
	
		unless (defined $success) {
			$errormessage = $tr{'smoothd failure'}; }		
	}
}

$selected{'EDIT'}{$cgiparams{'INTERFACE'}} = " selected";


if ( defined $cgiparams{'MACHINEACTION'} and $cgiparams{'MACHINEACTION'} eq $tr{'add'} )
{
	my $machine = $cgiparams{'MACHINE'};
	my $enabled = $cgiparams{'MACHINEENABLED'};
	my $comment = $cgiparams{'MACHINECOMMENT'};

	unless ( &validip( $machine ) ){
		$errormessage = "invalid ip"; }

	if ( $errormessage eq "" )
	{
		open(FILE,">>$machineconfig") or die 'Unable to open config file.';
		flock FILE, 2;
		print FILE "$machine,$enabled,$comment\n";
		close(FILE);
		$cgiparams{'MACHINE'} = "";
		$cgiparams{'MACHINEENABLED'} = "on";
		$cgiparams{'MACHINECOMMENT'} = "";

		my $success = message('setoutgoing');
	
		unless (defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}

if ( defined $cgiparams{'MACHINEACTION'} and $cgiparams{'MACHINEACTION'} eq $tr{'edit'} or
	$cgiparams{'MACHINEACTION'} eq $tr{'remove'})
{
	open(FILE, "$machineconfig") or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);

	foreach $line (@current)
	{
		$id++;
		if ($cgiparams{$id} eq "on") {
			$count++; }
	}

	if ($count == 0) {
		$errormessage = $tr{'nothing selected'}; 
	}
	if ($count > 1 && $cgiparams{'ACTION'} eq $tr{'edit'}) {
		$errormessage = $tr{'you can only select one item to edit'}; 
	}
	
	unless ($errormessage)
	{
		open(FILE, ">$machineconfig") or die 'Unable to open config file.';
		flock FILE, 2;
		$id = 0;
		foreach $line (@current)
		{
			$id++;
			unless ($cgiparams{$id} eq "on") {
				print FILE "$line"; }
			elsif ($cgiparams{'MACHINEACTION'} eq $tr{'edit'})
			{
				chomp($line);
				my @temp = split(/\,/,$line);
				$cgiparams{'MACHINE'} = $temp[0];
				$cgiparams{'MACHINEENABLED'} = $temp[1];
				$cgiparams{'MACHINECOMMENT'} = $temp[2];
			}
		}
		close(FILE);

		my $success = message('setoutgoing');
	
		unless (defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}

&openpage($tr{'outgoing filtering'}, 1, '', 'networking');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

&openbox($tr{'filtered interfaces'} . ':');
print "<form method='post'>\n";
print '<table style=\'width: 100%;\'>' . "\n";
print '<tr>' . "\n";

my $unused = 6;
my $width = 90 / $unused;
foreach $interface (keys(%interfaces))
{
	if ($interfaces{$interface} eq '') { next; }
	
	print qq{
	<tr>
	<td class='base' style='width: 25%;'>$tr{'traffic is 1'}$interface$tr{'traffic is 2'}</td>
	<td style='width: 25%;'>
		<select name=\"$interface\">
			<option $selected{"$interface"}{'REJECT'} value='REJECT'>$tr{'allowed'}</option>
			<option $selected{"$interface"}{'ACCEPT'} value='ACCEPT'>$tr{'blocked'}</option>
		</select>
	</td>
	<td style='width: 50%;'></td>
	</tr>
	};
}

print qq{
	<tr>
	<td colspan='3' style='text-align: center;'>
		<input type="submit" name="ACTION" value="$tr{'save'}">
	</td></tr>
</table>
</form>
};
print "</form>\n";

&closebox();

&openbox($tr{'add exception'});

print qq{
<form method='post'>
<table style='width: 100%;'>
<tr>
	<td class='base' style='width: 25%;'>$tr{'interfacec'}</td>
	<td style='width: 25%;'>
	<select name='INTERFACE'>
};

foreach my $interface (keys %interfaces) {
	if ($interfaces{$interface} eq '') { next; }

	print "<option value='$interface' $selected{'EDIT'}{$interface}>$interface</option>\n"; 
}

print qq{
	</select>
	</td>
	<td style='width: 25%;'></td>
	<td style='width: 25%;'></td>
</tr>
<tr>
	@{[&portlist('SERVICE', $tr{'application servicec'}, 'PORT', $tr{'portc'}, $service)]}
</tr>
<tr>
	<td class='base'>$tr{'commentc'}</td>
	<td colspan='3'><input type='text' style='width: 80%;' name='RULECOMMENT' value='$cgiparams{'RULECOMMENT'}' id='rulecomment' @{[jsvalidcomment('rulecomment')]}  ></td>
</tr>
<tr>
	<td class='base' style='width: 25%;'>$tr{'enabled'}</td>
	<td style='width: 25%;'><input type='checkbox' name='RULEENABLED' $checked{$cgiparams{'RULEENABLED'}}></td>
	<td colspan='2' style='text-align: center;'>
		<input type="submit" name="ACTION" value="$tr{'add'}">
	</td>
</tr>
</table>
</form>
};

&closebox();

&openbox($tr{'current exceptions'});
print "<form method='post'>\n";

my $portmap = &portmap();

my %render_settings =
(
	'url'     => "/cgi-bin/outgoing.cgi?[%COL%],[%ORD%],$cgiparams{'COLUMN_TWO'},$cgiparams{'ORDER_TWO'}",
	'columns' => 
	[
		{ 
			column => '1',
			title  => "$tr{'interfacenc'}",
			size   => 30,
			sort   => 'cmp',
		},
		{
			column => '3',
			title  => "$tr{'application service'}",
			size   => 50,
			sort   => 'cmp',
			tr     => \%{$portmap}
		},
		{
			column => '2',
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
			column => '1',
			colour => 'colour',
			tr     => { 'GREEN' => 'green', 'ORANGE' => 'orange', 'PURPLE' => 'purple' },
		},
		{ 
			column => '4',
			title => "$tr{'comment'}",
			break => 'line',
		}
	]
);

&displaytable($config, \%render_settings, $cgiparams{'ORDER_ONE'}, $cgiparams{'COLUMN_ONE'} );

print <<END
<table class='blank'>
<tr>
	<td style='width: 50%; text-align: center;'><input type='submit' name='ACTION' value='$tr{'remove'}'></td>
	<td style='width: 50%; text-align: center;'><input type='submit' name='ACTION' value='$tr{'edit'}'></td>
</tr>
</table>
</form>
END
;
&closebox();

&openbox($tr{'add allowed machine'});

print qq{
<form method='post'>
<table style='width: 100%;'>
<tr>
	<td class='base' style='width: 25%;'>$tr{'ip addressc'}</td>
	<td style='width: 25%;'><input type='text' name='MACHINE' id='address' @{[jsvalidip('address')]} value='$cgiparams{'MACHINE'}'/></td>
	<td style='width: 25%;'></td>
	<td style='width: 25%;'></td>	
</tr>
<tr>
	<td class='base'>$tr{'commentc'}</td>
	<td colspan='3'><input type='text' style='width: 80%;' name='MACHINECOMMENT' value='$cgiparams{'MACHINECOMMENT'}' id='machinecomment' @{[jsvalidcomment('machinecomment')]}  ></td>
</tr>
<tr>
	<td class='base'>$tr{'enabled'}</td>
	<td><input type='checkbox' name='MACHINEENABLED' $checked{$cgiparams{'MACHINEENABLED'}}></td>
	<td colspan='2' style='text-align: center;'><input type='submit' name='MACHINEACTION' value='$tr{'add'}'></td>
</tr>
</table>
</form>
};

&closebox();


&openbox($tr{'current allowed machines'});
print "<form method='post'>\n";

my %render_settings =
(
	'url'     => "/cgi-bin/outgoing.cgi?$cgiparams{'COLUMN_ONE'},$cgiparams{'ORDER_ONE'},[%COL%],[%ORD%]",
	'columns' => 
	[
		{ 
			column => '1',
			title  => "$tr{'ip address'}",
			size   => 30,
			sort   => 'cmp',
		},
		{
			column => '2',
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
			column => '3',
			title => "$tr{'comment'}",
			break => 'line',
		}
	]
);

&displaytable($machineconfig, \%render_settings, $cgiparams{'ORDER_TWO'}, $cgiparams{'COLUMN_TWO'} );

print <<END
<table class='blank'>
<tr>
	<td style='width: 50%; text-align: center;'><input type='submit' name='MACHINEACTION' value='$tr{'remove'}'></td>
	<td style='width: 50%; text-align: center;'><input type='submit' name='MACHINEACTION' value='$tr{'edit'}'></td>
</tr>
</table>
</form>
END
;

&closebox();

&alertbox('add','add');

&closebigbox();

&closepage();
