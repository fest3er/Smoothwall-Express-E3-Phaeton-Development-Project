#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
# Martin Houston <martin.houston@smoothwall.net>

use lib "/usr/lib/smoothwall";
use header qw(:standard);
use smoothtype qw(:standard);
use smoothd qw(message);

use Socket;

my (%cgiparams, %selected, %checked);
my $errormessage = '';

&showhttpheaders();
&getcgihash(\%cgiparams);

my %settings;
my %netsettings;

# two values are scale factor from full line speed for rate and ceiling
# 1.0 is full speed
# values of rates must add up to no more than 1.0
# as cannot promise more than the avaailable bandwidth!
my %scale = (
	normal => [0.4,0.9],
	high => [0.2, 0.9],
	low => [0.15, 0.4],
	slow => [0.01, 0.02],
	smoothadmin => [0.05, 0.95],
	webcache => [0.05, 0.95],
	smallpkt => [0.05, 0.1],
	localtraffic => [0.05, 0.95]
);

&readhash("${swroot}/ethernet/settings", \%netsettings );
&readhash("${swroot}/traffic/settings", \%settings );

# Action a "Save" request ...

if ( defined $cgiparams{'ACTION'} and $cgiparams{'ACTION'} eq $tr{'save'} )
{
	# Turn our cgi parameters into the more compact settings form
	# the checkboxes
	# force this to on
	$cgiparams{'PERIPSTATS'} = 'on';
	for(qw/ENABLE PERIPSTATS/) 
	{
		$settings{$_} = (defined $cgiparams{$_} ? 'on' : 'off');
	}
	# simple numeric quantities
	for(qw/HEADROOM INTERNAL_SPEED UPLOAD_SPEED DOWNLOAD_SPEED/) 
	{
		$settings{$_} = $1 if defined $cgiparams{$_} && $cgiparams{$_} =~ /^(\d+)$/;
	}
	# prios are hard coded for now
	if(!defined $settings{'PRIO'}) 
	{
		$settings{'PRIO'} = 'high,0,normal,1,low,2,slow,3,smoothadmin,0,webcache,1,localtraffic,0,smallpkt,0';
	}
	# and classids
	if(!defined $settings{'CLASSIDS'}) 
	{
		$settings{'CLASSIDS'} = 'all,100,none,0,normal,109,high,110,low,111,slow,112,smoothadmin,113,webcache,114,localtraffic,115,smallpkt,116';
	}
	# classids are never redefined in the UI
	my %classids = split(',', $settings{'CLASSIDS'});
	if(defined $cgiparams{'DEFAULT_TRAFFIC'} && 
		defined $classids{$cgiparams{'DEFAULT_TRAFFIC'}})
	{
		$settings{'DEFAULT_TRAFFIC'} =  $cgiparams{'DEFAULT_TRAFFIC'};
	}
	
	# Now the rates and ceilings
	@settings{'DRATE','DCEIL','URATE', 'UCEIL'} = ('' x 4);
	for(keys %classids) {
		next if /^(all|none)$/;
		my $mulfactor = ($_ eq 'localtraffic' ? $settings{'INTERNAL_SPEED'} : 
			$settings{'DOWNLOAD_SPEED'} * ((100 - $settings{'HEADROOM'})/100));
		$settings{'DRATE'} .= "$_," . int($scale{$_}->[0] * $mulfactor) . ',';
		$settings{'DCEIL'} .= "$_," . int($scale{$_}->[1] * $mulfactor) . ',';
		# now uploads
		$mulfactor = ($_ eq 'localtraffic' ? $settings{'INTERNAL_SPEED'} : 
			$settings{'UPLOAD_SPEED'} * ((100 - $settings{'HEADROOM'})/100));
		
		$settings{'URATE'} .= "$_," . int($scale{$_}->[0] * $mulfactor) . ',';
		$settings{'UCEIL'} .= "$_," . int($scale{$_}->[1] * $mulfactor) . ',';
	}

	# now the rules - these need to be collected from class choices
	# rest of rule must preexist - no suitable default!
	# class is 5th part of rule def and only one that is changed on this screen.
	for my $cgi (keys %cgiparams)
	{
		next unless $cgi =~ /^R_(\d+)_CLASS$/ && defined $classids{$cgiparams{$cgi}};
		# we have a valid class to assign
		my $rulenum = 'R_' . $1;
		if(defined $settings{$rulenum}) 
		{
			my($name,$tcp,$udp,$dir,$ports,$class,$comment) = split(/,/, $settings{$rulenum});

			$class = $cgiparams{$cgi};
			$settings{$rulenum} = join(',', $name,$tcp,$udp,$dir,$ports,$class,$comment);
		}
	}
		
	unless ($errormessage)
	{
		&writehash("${swroot}/traffic/settings", \%settings);

		my $success = message('trafficrestart');

		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}

&openpage($tr{'traffic configuration'}, 1, '', 'networking');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<form method='post'>";

# deal with the green settings.
&display_speeds( \%settings);

# deal with the green settings.
&display_rules( \%settings);

print qq{
	<div style='text-align: center;'><input type='submit' name='ACTION' value='$tr{'save'}'></div>
	</form>
};

&alertbox('add','add');

&closebigbox();

&closepage();

sub display_speeds
{
	my ( $settings ) = @_;
	my($upload_speed_block,$download_speed_block,$internal_speed_block,$headroom_block,$default_block);
	my %selected = ();
	my %speed_labels = ( 
		4096 => '32kbit',
		8192 => '64kbit',
		16384 => '128kbit',
		32768 => '256kbit',
		49408 => '386kbit', 
		65536 => '512kbit',
		131072 => '1mbit',
		196608 => '1.5mbit',
		262144 => '2mbit',
		393216 => '3mbit',
		524288 => '4mbit',
		655360 => '5mbit',
		786432 => '6mbit',
		1048576 => '8mbit',
		1310720 => '10mbit',
		1572864 => '12mbit',
		2097152 => '16mbit',
		2621440 => '20mbit',
		3145728 => '24mbit',
		3276800 => '25mbit',
		7208960 => '55mbit',
		13107200 => '100mbit',
		52428800 => '400mbit',
		78643200 => '600mbit',
		1342107200 => '1gbit');
	my %headroom_labels = ();
	$headroom_labels{$_} = "$_ %" for(1..35);
	my %default_traffic_labels = ( 
		high => $tr{'traffic high'}, 
		low => $tr{'traffic low'}, 
		slow => $tr{'traffic slow'}, 
		normal => $tr{'traffic normal'});
	my @speeds = sort {$a <=> $b} keys %speed_labels; 
	my @ext_speeds = grep {$_ <= 78643200} @speeds; # up to 600mbit (in your dreams!)
	my @int_speeds = grep {$_ >= 1310720} @speeds; # from 10mbit
		
	%selected = ($settings->{'UPLOAD_SPEED'} => ' selected');
	$upload_speed_block = join('', 
		map { "<option value='$_'" . ($selected{$_} || '') . ">$speed_labels{$_}</option>\n" } 
			@ext_speeds);
	%selected = ($settings->{'DOWNLOAD_SPEED'} => ' selected');
	$download_speed_block = join('', 
		map { "<option value='$_'" . ($selected{$_} || '') . ">$speed_labels{$_}</option>\n" } 
			@ext_speeds);
	%selected = ($settings->{'INTERNAL_SPEED'} => ' selected');
	$internal_speed_block = join('', 
		map { "<option value='$_'" . ($selected{$_} || '') . ">$speed_labels{$_}</option>\n" } 
		@int_speeds);

	%selected = ($settings->{'HEADROOM'} => ' selected');
	$headroom_block = join('', 
		map { "<option value='$_'" . ($selected{$_} || '') . ">$headroom_labels{$_}</option>\n" } 
		(sort { $a <=> $b } keys %headroom_labels ));

	%selected = ($settings->{'DEFAULT_TRAFFIC'} => ' selected');
	$default_block = join('', 
		map { "<option value='$_'" . ($selected{$_} || '') . ">$default_traffic_labels{$_}</option>\n" } 
		(qw/normal high low slow/));

	&openbox($tr{'traffic general options'});
	my $enable_bit = ($settings->{'ENABLE'} eq 'on' ? 'checked' : '');
	print qq{
		<table style='width: 100%;'>
		<tr>
			<td style='width: 25%;'>$tr{'traffic enable'}</td>
			<td style='width: 25%;'><input name='ENABLE' type='checkbox' $enable_bit /></td>
			<td style='width: 25%;'>$tr{'traffic internal speed'}</td>
			<td style='width: 25%;'><select name='INTERNAL_SPEED'>$internal_speed_block</select></td>
		</tr>
		<tr>
			<td>$tr{'traffic external up'}</td>
			<td><select name='UPLOAD_SPEED'>$upload_speed_block</select></td>
			<td>$tr{'traffic external down'}</td>
			<td><select name='DOWNLOAD_SPEED'>$download_speed_block</select></td>
		</tr>
		<tr>
			<td>$tr{'traffic headroom'}</td>
			<td><select name='HEADROOM'>$headroom_block</select></td>
			<td>$tr{'traffic default'}</td>
			<td><select name='DEFAULT_TRAFFIC'>$default_block</select></td>
		</tr>
		</table>
	};

	&closebox();

	return;
}

sub display_rules
{
	my ( $settings ) = @_;
	my %class_labels = ( 
		none => $tr{'traffic none'}, 
		high => $tr{'traffic high'}, 
		low => $tr{'traffic low'}, 
		slow => $tr{'traffic slow'}, 
		normal => $tr{'traffic normal'});

	my @rules = ();
	&openbox($tr{'traffic rules'});

	my %selected = ();

	print qq{
		<table style='width: 100%;'>};

	for my $rule (sort keys %{$settings}) 
	{
		next unless $rule =~ /R_(\d+)$/;
		
		my ($name, $class) = (split(',', $settings->{$rule}))[0,5];
		%selected = ($class => ' selected');
		my $class_block = join('', 
		map { "<option value='$_'" . ($selected{$_} || '') . ">$class_labels{$_}</option>\n" } 
		(qw/none normal high low slow/));
		$name =~ s/_/ /g;
		push @rules, qq[<td style='width: 25%;'>$name:</td><td style='width: 25%;'><select name='${rule}_CLASS'>$class_block</select></td>\n];
	}
	# 2 colums
	for(my $r = 0; $r <= $#rules; $r += 2)
	{
		$rules[$r+1] = '&nbsp;' unless defined $rules[$r+1];
		print "<tr>" . $rules[$r] . $rules[$r+1] . "</tr>\n";
	}

	print qq{</table>};

	&closebox();

	return;
}

sub is_running
{
    my $running = qx{/sbin/tc qdisc list | fgrep htb | wc -l};
    chomp $running; # loose \n or 0\n is considered true!
    return $running;
}

