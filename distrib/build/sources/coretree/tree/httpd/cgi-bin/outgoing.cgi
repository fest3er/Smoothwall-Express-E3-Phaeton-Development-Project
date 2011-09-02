#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# SmoothWall is (c) The SmoothWall Team
#
# Modifications of this script are (c) Stanford Prescott
#
#################################################################################################
#													     /
# June 10, 2009 changed how exceptions for VPN protocols are written. Selecting		     /
# IPSEC, for example, will write all the rules necessary for IPSEC protocols and ports	     /
#													     /
# June 7, 2011 A number of small changes and fixes up to this date.				     /
#  -Fixed a problem with selecting all ports							     /
#  -Added ability to remove only time frames from an exception and leave the exception in place /
#  -Smoothd module enhanced and updated courtesy of Steve Pittman (MtnLion)			     /
#													     /
# August 2011 More modifications for use of the xtables-addons xt_iprange xt_multiport and      /
#   xt_time. Also ported this mod to Roadster                                                   /
#													     /
#################################################################################################

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );
use smoothtype qw( :standard );
use NetAddr::IP;

my $moddir = '/var/smoothwall/outgoing';
my $config = "$moddir/config";
my $settingsfile = "$moddir/settings";
my $hashfile = "$moddir/confighash";
my $halfopen = "/var/smoothwall-halfopen/outgoing/config";

my %netsettings;
&readhash("${swroot}/ethernet/settings", \%netsettings);

my @hour = ( 0 .. 23 );
my @minute = ( '00', '01', '02', '03', '04', '05', '06', '07', '08', '09', 10 .. 59 );

my $errormessage = '';
my $updatebutton = 0;

&showhttpheaders();

my ( %interfaces, %settings, %netsettings, %cgiparams, %selected, %checked, %confighash );

$cgiparams{'ACTION'} = '';

$cgiparams{'COLUMN'} = 1;
$cgiparams{'ORDER'} = $tr{'log ascending'};

$cgiparams{'OLDID'} = 0;
$cgiparams{'RULE_ORDER'} = '';

$cgiparams{'GREEN_REJECTS'} = 'off';
$cgiparams{'PURPLE_REJECTS'} = 'off';
$cgiparams{'ORANGE_REJECTS'} = 'off';

$cgiparams{'GREEN_RELATED'} = 'off';
$cgiparams{'PURPLE_RELATED'} = 'off';
$cgiparams{'ORANGE_RELATED'} = 'off';
$cgiparams{'RULEENABLED'} = 'on';

&getcgihash(\%cgiparams);

if ($ENV{'QUERY_STRING'} && ( not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ))
{
	my @temp = split(',',$ENV{'QUERY_STRING'});
	$cgiparams{'ORDER'}  = $temp[1] if ( defined $temp[ 1 ] and $temp[ 1 ] ne "" );
	$cgiparams{'COLUMN'} = $temp[0] if ( defined $temp[ 0 ] and $temp[ 0 ] ne "" );
}

# Load inbound interfaces into %interfaces (excluding RED)

&readhash("${swroot}/ethernet/settings", \%netsettings);

$interfaces{'GREEN'} = $netsettings{'GREEN_DEV'};
$interfaces{'ORANGE'} = $netsettings{'ORANGE_DEV'};
$interfaces{'PURPLE'} = $netsettings{'PURPLE_DEV'};

if ($cgiparams{'ACTION'} eq '') {
	$cgiparams{'RULEENABLED'} = 'on';
}

if ( defined $cgiparams{'ACTION'} and $cgiparams{'ACTION'} eq $tr{'save'} )
{
	my (@config, @temp);
	my ($line, $line2, $action);
	my $order = 0;
	my @temp2;

	&readhash("$settingsfile", \%settings);

	if ($netsettings{'GREEN_DEV'} and ($settings{'GREEN'} ne $cgiparams{'GREEN'})) {
		# Remove GREEN exceptions and add default exceptions for blocked state
		open (FILE, "$config") or die 'Unable to open TOFC config file';
		@config = <FILE>;
		close FILE;

		open (FILE, ">$config") or die 'Unable to open TOFC config file';
		flock (FILE, 2);
		foreach $line (@config) {
			chomp $line;
			@temp = split(/,/, $line);
			@temp2 = split /\+/, $line;

			if ($temp[0] eq 'GREEN') {
				next;
			} else {
				$order++;
				if ($temp2[1]) {
					print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$order,$temp[8],$temp[9],+$temp[1]\n";
				} else {
					print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$order,$temp[8],$temp[9]\n";
				}
			}
		}
		if (($settings{'GREEN'} eq 'ACCEPT') and ($cgiparams{'GREEN'} ne 'CLOSED')) {
			open (FILE2, "$halfopen") or die 'Unable to open halfopen file';
			my @halfopen = <FILE2>;
			close FILE2;

			foreach $line2 (@halfopen) {
				chomp $line2;
				@temp = split(/,/, $line2);
				if ($temp[0] eq 'GREEN') {
					$order++;
					print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$order,$temp[8],$temp[9]\n";
				}
			}
			&log("$tr{'tofc-log outgoing'} GREEN $tr{'tofc-log changed'} blocked");
		} else {
			&log("$tr{'tofc-log outgoing'} GREEN $tr{'tofc-log changed'} allowed");
		}
		close FILE;
	}
	$order = 0;
	if ($netsettings{'ORANGE_DEV'} and ($settings{'ORANGE'} ne $cgiparams{'ORANGE'})) {
		# Remove ORANGE exceptions and add default exceptions for blocked state
		open (FILE, "$config") or die 'Unable to open TOFC config file';
		@config = <FILE>;
		close FILE;

		open (FILE, ">$config") or die 'Unable to open TOFC config file';
		flock (FILE, 2);
		foreach $line (@config) {
			chomp $line;
			@temp = split(/,/, $line);

			if ($temp[0] eq 'ORANGE') {
				next;
			} else {
				$order++;
				if ($temp[10]) {
					print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$order,$temp[8],$temp[9],$temp[10],$temp[11],$temp[12]\n";
				} else {
					print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$order,$temp[8],$temp[9],$temp[10],$temp[11],$temp[12]\n";
				}
			}
		}

		if (($settings{'ORANGE'} eq 'ACCEPT') and ($cgiparams{'ORANGE'} ne 'CLOSED')) {
			open (FILE2, "$halfopen") or die 'Unable to open halfopen file';
			my @halfopen = <FILE2>;
			close FILE2;

			foreach $line2 (@halfopen) {
				chomp $line2;
				@temp = split(/,/, $line2);
				if ($temp[0] eq 'ORANGE') {
					$order++;
					print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$order,$temp[8],$temp[9],$temp[10],$temp[11],$temp[12]\n";
				}
			}
			&log("$tr{'tofc-log outgoing'} ORANGE $tr{'tofc-log changed'} blocked");
		} else {
			&log("$tr{'tofc-log outgoing'} ORANGE $tr{'tofc-log changed'} allowed");
		}
		close FILE;
	}
	$order = 0;
	if ($netsettings{'PURPLE_DEV'} and ($settings{'PURPLE'} ne $cgiparams{'PURPLE'})) {
		# Remove PURPLE exceptions and add default exceptions for blocked state
		open (FILE, "$config") or die 'Unable to open TOFC config file';
		@config = <FILE>;
		close FILE;

		open (FILE, ">$config") or die 'Unable to open TOFC config file';
		flock (FILE, 2);
		foreach $line (@config) {
			chomp $line;
			@temp = split(/,/, $line);

			if ($temp[0] eq 'PURPLE') {
				next;
			} else {
				$order++;
				if ($temp[10]) {
					print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$order,$temp[8],$temp[9],$temp[10],$temp[11],$temp[12]\n";
				} else {
					print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$order,$temp[8],$temp[9],$temp[10],$temp[11],$temp[12]\n";
				}
			}
		}

		if (($settings{'PURPLE'} eq 'ACCEPT') and ($cgiparams{'PURPLE'} ne 'CLOSED')) {
			open (FILE2, "$halfopen") or die 'Unable to open halfopen file';
			my @halfopen = <FILE2>;
			close FILE2;

			foreach $line2 (@halfopen) {
				chomp $line2;
				@temp = split(/,/, $line2);
				if ($temp[0] eq 'PURPLE') {
					$order++;
					print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$order,$temp[8],$temp[9],$temp[10],$temp[11],$temp[12]\n";
				}
			}
			&log("$tr{'tofc-log outgoing'} PURPLE $tr{'tofc-log changed'} blocked");
		} else {
			&log("$tr{'tofc-log outgoing'} PURPLE $tr{'tofc-log changed'} allowed");
		}
		close FILE;
	}

	$settings{'GREEN'} = $cgiparams{'GREEN'};
	$settings{'ORANGE'} = $cgiparams{'ORANGE'};
	$settings{'PURPLE'} = $cgiparams{'PURPLE'};

	if ($cgiparams{'GREEN_REJECTS'} eq 'on') {
			$settings{'GREEN_REJECTS'} = 'on';
	} else {
		$settings{'GREEN_REJECTS'} = 'off';
	}

	if ($cgiparams{'PURPLE_REJECTS'} eq 'on') {
			$settings{'PURPLE_REJECTS'} = 'on';
	} else {
		$settings{'PURPLE_REJECTS'} = 'off';
	}

	if ($cgiparams{'ORANGE_REJECTS'} eq 'on') {
			$settings{'ORANGE_REJECTS'} = 'on';
	} else {
		$settings{'ORANGE_REJECTS'} = 'off';
	}

	if ($cgiparams{'GREEN_RELATED'} eq 'on') {
			$settings{'GREEN_RELATED'} = 'on';
	} else {
		$settings{'GREEN_RELATED'} = 'off';
	}

	if ($cgiparams{'PURPLE_RELATED'} eq 'on') {
			$settings{'PURPLE_RELATED'} = 'on';
	} else {
		$settings{'PURPLE_RELATED'} = 'off';
	}

	if ($cgiparams{'ORANGE_RELATED'} eq 'on') {
			$settings{'ORANGE_RELATED'} = 'on';
	} else {
		$settings{'ORANGE_RELATED'} = 'off';
	}

	&writehash("$settingsfile", \%settings);
		
	my $success = message('settimedoutgoing');

	unless (defined $success) {
		$errormessage = $tr{'smoothd failure'}; }
}

&readhash("$settingsfile", \%settings);

$selected{'GREEN'}{$settings{'GREEN'}} = " selected ";
$selected{'ORANGE'}{$settings{'ORANGE'}} = " selected ";
$selected{'PURPLE'}{$settings{'PURPLE'}} = " selected ";

my $order;
my @temp;
my $line;
my $configline;
my @splitline;
my $splitline;
my ( $interface, $enabled, $timed, $port, $ipmac, $protocol, $comment );

$errormessage = "";

if ((defined $cgiparams{'ACTION'}) and ($cgiparams{'ACTION'} eq $tr{'add'} or $cgiparams{'ACTION'} eq $tr{'tofc-update'}))
{
	my $interface = $cgiparams{'INTERFACE'};
	my $service    = $cgiparams{'SERVICE'};
	my $port       = $cgiparams{'PORT'};
	my $protocol   = $cgiparams{'PROTOCOL'};
	my $ipmac      = $cgiparams{'IPMAC'};
	my $order	 = $cgiparams{'RULE_ORDER'};
	my $comment    = $cgiparams{'COMMENT'};
	my @singleip;

	&writehash("$hashfile", \%cgiparams);

	#Temporary Bandaid to fix a problem when selecting a service instead of a port
	chomp $service;

	if ( $service eq "user" )
	{
		if ( $protocol eq "TCP" or $protocol eq "UDP" or $protocol eq "Both" ) {
			if ($port eq "") {
				$service = 'N/A';
			} else {
				$service = $port;
			}
		} else {
			$service = 'N/A';
		}
	}

	unless ( $protocol eq "TCP" or $protocol eq "UDP" or $protocol eq "Both" ) {
		unless ( $service eq "N/A" ) {
			$errormessage = $tr{'tofc-port and protocol'};
		}
	}

	if ($service eq 'Infrastructure') {
		$protocol = 'Both';
	}

	if ($cgiparams{'IPMAC'} eq "") {
		$ipmac = 'N/A';
	} elsif ( (&validipormask ($cgiparams{'IPMAC'})) or (&validmac ($cgiparams{'IPMAC'}) or $cgiparams{'IPMAC'} =~ /\-/) ) {
		$ipmac = $cgiparams{'IPMAC'};
	} else {
		$errormessage = $tr{'tofc-ipmac bad'};
	}

	if (($settings{'GREEN'} eq "REJECT" or $settings{'GREEN'} eq "CLOSED") and $cgiparams{'INTERFACE'} eq "GREEN") {
		$action = "ACCEPT";
	} elsif ($settings{'GREEN'} eq "ACCEPT" and $cgiparams{'INTERFACE'} eq "GREEN") {
		$action = "REJECT";
	}

	if (($settings{'ORANGE'} eq "REJECT" or $settings{'ORANGE'} eq "CLOSED") and $cgiparams{'INTERFACE'} eq "ORANGE") {
		$action = "ACCEPT";
	} elsif ($settings{'ORANGE'} eq "ACCEPT" and $cgiparams{'INTERFACE'} eq "ORANGE") {
		$action = "REJECT";
	}

	if (($settings{'PURPLE'} eq "REJECT" or $settings{'PURPLE'} eq "CLOSED") and $cgiparams{'INTERFACE'} eq "PURPLE") {
		$action = "ACCEPT";
	} elsif ($settings{'PURPLE'} eq "ACCEPT" and $cgiparams{'INTERFACE'} eq "PURPLE") {
		$action = "REJECT";
	}

	if ($cgiparams{'RULEENABLED'} eq "on") {
		$enabled = "on";
	} else {
		$enabled = "off";
	}

	if ($cgiparams{'PORT'}) {
		&readhash("$hashfile", \%cgiparams);
		$service = $cgiparams{'PORT'};
		$service =~ s/,/-/g;
	}

  	#########################################
  	# Check for IP's in subnets
  	#########################################
	if ($ipmac =~ /:/) {
		goto EXIT;
	}
	if ($ipmac =~ /-/) {
		@singleip = split /\-/, $ipmac;
	} else {
		$singleip[0] = $ipmac;
	}
  	my $greenipobj = new NetAddr::IP "$netsettings{'GREEN_ADDRESS'}/$netsettings{'GREEN_NETMASK'}";
  	my $orangeipobj = new NetAddr::IP "$netsettings{'ORANGE_ADDRESS'}/$netsettings{'ORANGE_NETMASK'}";;
  	my $purpleipobj = new NetAddr::IP "$netsettings{'PURPLE_ADDRESS'}/$netsettings{'PURPLE_NETMASK'}";;
  	my $srcipaddrobj = "";

	$greenipobj = new NetAddr::IP "$netsettings{'GREEN_ADDRESS'}/$netsettings{'GREEN_NETMASK'}";

	if ($netsettings{'ORANGE_ADDRESS'} ne '') {
		$orangeipobj = new NetAddr::IP "$netsettings{'ORANGE_ADDRESS'}/$netsettings{'ORANGE_NETMASK'}";
  	}

  	if ($netsettings{'PURPLE_ADDRESS'} ne '') {
    		$purpleipobj = new NetAddr::IP "$netsettings{'PURPLE_ADDRESS'}/$netsettings{'PURPLE_NETMASK'}";
  	}

    	$srcipaddrobj = new NetAddr::IP "$singleip[0]";

      	if ($cgiparams{'INTERFACE'} eq 'GREEN') {
       	if (!$greenipobj->contains($srcipaddrobj)) {
          		$errormessage .= "$tr{'tofc-source ip bad green'}<BR />";
      		}
      	} elsif (($netsettings{'ORANGE_ADDRESS'} ne '') &&
       	($cgiparams{'INTERFACE'} eq 'ORANGE')) {
        	if (!$orangeipobj->contains($srcipaddrobj)) {
          		$errormessage .= "$tr{'tofc-source ip bad orange'}<BR />";
        	}
      	} elsif (($netsettings{'PURPLE_ADDRESS'} ne '') &&
              ($cgiparams{'INTERFACE'} eq 'PURPLE')) {
        	if (!$purpleipobj->contains($srcipaddrobj)) {
          		$errormessage .= "$tr{'tofc-source ip bad purple'}<BR />";
        	}
      	}
EXIT:
	if ( $errormessage eq "" ) {
		my $notadded = 1;
		my $cnt = 0;

		if ($cgiparams{'ACTION'} eq $tr{'tofc-update'})
		{
			$notadded = 0;

			open(FILE, "$config") or die 'Unable to open config file.';
			my @current = <FILE>;
			close(FILE);

			my $line;
			open(FILE, ">$config") or die 'Unable to open config file.';
			flock FILE, 2;

			foreach $line (@current) {
				$cnt++;
				chomp $line;
				my @temp = split(/\,/, $line);
				my @times = split /\+/, $line;
				if ($temp[7] eq $order) {
					print FILE "$interface,$enabled,$service,$comment,$protocol,$ipmac,$temp[6],$cnt,$cgiparams{'TIMED'},+$cgiparams{'TIMES'}\n";
					$order = "0";
				} else {
					print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$cnt,$temp[8],+$times[1]\n";
				}
			}
			close FILE;
		}

		# If adding new exception
		if ($notadded) {
			# Get a line count of config file
			open(FILE, "$config") or die 'Unable to open config file';
			@temp = <FILE>;
			close FILE;

			my $linecnt = 1;
			foreach $line (@temp) {
				$linecnt++;
			}
				
			open(FILE, ">>$config") or die 'Unable to open config file';
			flock FILE, 2;
			print FILE "$interface,$enabled,$service,$comment,$protocol,$ipmac,$action,$linecnt,off,,\n";
			close(FILE);
		}

		if ($cgiparams{'ACTION'} eq $tr{'add'}) {
			&log($tr{'tofc-outgoing added'});
		} else {
			&log($tr{'tofc-outgoing updated'});
		}
		
		my $success = message('settimedoutgoing');

		unless (defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}

	undef $cgiparams{'INTERFACE'};
	undef $cgiparams{'PROTOCOL'};
	undef $cgiparams{'IPMAC'}; 
	undef $cgiparams{'PORT'};
	undef $cgiparams{'COMMENT'};
	undef $cgiparams{'TIMES'};
}
my $service = 'user';

if ( defined $cgiparams{'ACTION'} and $cgiparams{'ACTION'} eq $tr{'tofc-schedule'} ) {

	my $sminute = $cgiparams{'START_MINUTE'};
	my $shour   = $cgiparams{'START_HOUR'};
	my $eminute = $cgiparams{'END_MINUTE'};
	my $ehour   = $cgiparams{'END_HOUR'};
	my $timestart = "$shour:$sminute";
	my $timestop = "$ehour:$eminute";

	if ($shour eq $ehour and $sminute eq $eminute) {
		$errormessage = 'Start time cannot equal End time!';
	}
    unless ($errormessage) {
	my ($Sun, $Mon, $Tue, $Wed, $Thu, $Fri, $Sat, $days);
	my $flag = '';
	# Convert days of week to numerical values and build days of week argument
	if ($cgiparams{'DAY_6'} eq "on") {
		$sun = 'Sun ';
		$flag = 1;
	}
	if ($cgiparams{'DAY_0'} eq "on") {
		$mon = 'Mon ';
		$flag = 1;
	}
	if ($cgiparams{'DAY_1'} eq "on") {
		$tue = 'Tue ';
		$flag = 1;
	}
	if ($cgiparams{'DAY_2'} eq "on") {
		$wed = 'Wed ';
		$flag = 1;
	}
	if ($cgiparams{'DAY_3'} eq "on") {
		$thu = 'Thu ';
		$flag = 1;
	}
	if ($cgiparams{'DAY_4'} eq "on") {
		$fri = 'Fri ';
		$flag = 1;
	}
	if ($cgiparams{'DAY_5'} eq "on") {
		$sat = 'Sat ';
		$flag = 1;
	}
	if (defined $flag) {
		$days = "$mon$tue$wed$thu$fri$sat$sun";
		chop $days;
	}

	if ($cgiparams{'DAY_9'} eq "on" and $flag eq '') {
		$days = '';
	}

	unless ($days eq '') {
		$timedays = $days;
	} else {
		$timedays = 'Every day';
	}

	my $timedisp = "$timedays $timestart to $timestop";

	if (open(FILE, "$config")) {
		@temp = <FILE>;
		close FILE;
	}
	open(FILE, ">$config") or die 'unable to open config file';
	flock FILE, 2;
	$id = 0;
	$count = 1;

	foreach $line (@temp)
	{
		$id++;
		chomp($line);
		my @cfg_ln = split /,/, $line;
		unless ($cgiparams{$id} eq "on") {
			print FILE "$line\n";
	# Let's handle a stupid mistake like if a user enters a time frame that spans MN
		} elsif ($shour > $ehour) {
			my $timestop2 = "23:59";
			$timedisp = "$cfg_ln[6] on $timedays $timestart to $timestop2";
			if ($cfg_ln[8] eq 'off') {
				$cfg_ln[8] = 'on';
				$cfg_ln[9] = "+$timedisp";
			} else {
				$cfg_ln[9] = "$cfg_ln[9] | $timedisp";
			}
			push (@cfg_ln, $days, $timestart, $timestop2);

			$timestart2 = "00:01";
			$cfg_ln[9] = "$cfg_ln[9] | $cfg_ln[6] on $timedays $timestart2 to $timestop";
			push (@cfg_ln, $days, $timestart2, $timestop);
			my $cfg_ln_cnt = @cfg_ln;
			for (my $i = 0; $i < $cfg_ln_cnt; $i++) {
				print FILE "$cfg_ln[$i],";
			}
			print FILE "\n";
	# This bit for the user who is careful and enters appropriate time frames
		} else { 
			$timedisp = "$cfg_ln[6] on $timedays $timestart to $timestop";
			if ($cfg_ln[8] eq 'off') {
				$cfg_ln[8] = 'on';
				$cfg_ln[9] = "+$timedisp";
			} else {
				$cfg_ln[9] = "$cfg_ln[9] | $timedisp";
			}
			push (@cfg_ln, $days, $timestart, $timestop);
			my $cfg_ln_cnt = @cfg_ln;
			for (my $i = 0; $i < $cfg_ln_cnt; $i++) {
				print FILE "$cfg_ln[$i],";
			}
			print FILE "\n";
		}
	}
	close FILE;

	$success = message('settimedoutgoing');

	unless (defined $success) {
		$errormessage = 'Unable to set outgoing exceptions';
	}
   }
	undef $cgiparams{'START_MINUTE'};
	undef $cgiparams{'START_HOUR'};
	undef $cgiparams{'END_MINUTE'};
	undef $cgiparams{'END_HOUR'};
}

if ( defined $cgiparams{'ACTION'} and $cgiparams{'ACTION'} eq $tr{'edit'} or
	$cgiparams{'ACTION'} eq $tr{'tofc-remove-exc'} or $cgiparams{'ACTION'} eq $tr{'tofc-remove-tf'})
{
	open(FILE, "$config") or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);

	my $count = 0;
	my $id = 0;
	my $line;

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
		$count = 1;
		foreach $line (@current)
		{
			$id++;
			chomp($line);
			my @temp = split(/\,/,$line);
			my @times = split /\+/, $line;
			unless ($cgiparams{$id} eq "on") {
				print FILE "$line\n";
				$count++; 
			} elsif ($cgiparams{'ACTION'} eq $tr{'edit'}) {
				$cgiparams{'INTERFACE'} = $temp[0];
				if ( &validportrange( $temp[2] ) ) {
					$service = $temp[2];
				}
				if ($temp[4] eq "TCP") {
					$cgiparams{'PROTOCOL'} = "6";
				} elsif ($temp[4] eq "UDP") {
					$cgiparams{'PROTOCOL'} = "17";
				} elsif ($temp[4] eq "ICMP") {
					$cgiparams{'PROTOCOL'} = "1";
				} elsif ($temp[4] eq "PPTP") {
					$cgiparams{'PROTOCOL'} = "PPTP";
				} elsif ($temp[4] eq "IPSEC") {
					$cgiparams{'PROTOCOL'} = "IPSEC";
				} else {
					$cgiparams{'PROTOCOL'} = $temp[4];
				}
				unless ($temp[5] eq "N/A") {
					$cgiparams{'IPMAC'} = $temp[5];
				}
				$cgiparams{'RULEENABLED'} = $temp[1];
				$cgiparams{'RULE_ORDER'} = $temp[7];
				$cgiparams{'COMMENT'} = $temp[3];
				$cgiparams{'TIMED'} = $temp[8];
				$cgiparams{'TIMES'} = $times[1];

				# Editing support
				$updatebutton = 1;
				print FILE "$line\n";
			} elsif ($cgiparams{'ACTION'} eq $tr{'tofc-remove-tf'}) {
				print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$temp[7],off,,\n";
			}
		}
		close(FILE);

		my $success = message('settimedoutgoing');

		unless (defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}

if ( defined $cgiparams{'ACTION'} and $cgiparams{'ACTION'} eq $tr{'tofc-enabledc'} ) 
{
	unless ( $errormessage ) {
		open(FILE, "$config") or die 'Unable to open config file.';
		@current = <FILE>;
		close FILE;

		open(FILE, ">$config") or die 'Unable to open config file.';
		flock FILE, 2;
		$id = 0;
		foreach $line (@current)
		{
			$id++;
			chomp($line);
			my @temp = split(/\,/,$line);
			unless ($cgiparams{$id} eq "on") {
				print FILE "$line\n";
			} elsif ( $temp[1] eq "off" ) {
				print FILE "$temp[0],on,$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$temp[7],$temp[8],$temp[9],$temp[10],$temp[11],$temp[12]\n";
			} elsif ( $temp[1] eq "on" ) {
				print FILE "$temp[0],off,$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$temp[7],$temp[8],$temp[9],$temp[10],$temp[11],$temp[12]\n";
			}
		}
		close FILE;
		my $success = message('settimedoutgoing');
	
		unless (defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}

if ( defined $cgiparams{'ACTION'} and $cgiparams{'ACTION'} eq $tr{'tofc-change action'} ) 
{
	open(FILE, "$config") or die 'Unable to open config file.';
	@current = <FILE>;
	close FILE;

	open(FILE, ">$config") or die 'Unable to open config file.';
	flock FILE, 2;
	$id = 0;
	foreach $line (@current)
	{
		$id++;
		chomp($line);
		my @temp = split(/\,/,$line);
		unless ($cgiparams{$id} eq "on") {
			print FILE "$line\n";
		} elsif ($temp[6] eq "ACCEPT" ) {
			$action = "REJECT";
			print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$action,$temp[7],$temp[8],$temp[9],$temp[10],$temp[11],$temp[12]\n";
		} elsif ($temp[6] eq "REJECT") {
			$action = "ACCEPT";
			print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$action,$temp[7],$temp[8],$temp[9],$temp[10],$temp[11],$temp[12]\n";
		} 
	}
	close FILE;

	my $success = message('settimedoutgoing');

	unless (defined $success) {
		$errormessage = $tr{'smoothd failure'}; }
}

# Protocol listing
open(TMP, "/etc/protocols") or die "Unable to open /etc/protocols\: $!\n";
my @protocols = <TMP>;
close(TMP);

my %availableprotocols;
foreach $line (@protocols) { 
	chomp $line;

	if ($line =~ m/^([a-z0-9]|\#\s[0-9]+\s+)/i) {
		my @protoline = split(/\s+/, $line);

		if ($#protoline == 3) {
			$protoline[3] =~ s/\b(\w)/\u\L$1/g;
			$availableprotocols{$protoline[1]} = "$protoline[2] = $protoline[3]";
		} else {
			$protoline[2] =~ s/\b(\w)/\u\L$1/g;
			$availableprotocols{$protoline[1]} = "$protoline[2]";
		}
	}
}

$selected{'INTERFACE'}{$cgiparams{'INTERFACE'}} = " selected";
$selected{'SERVICE'}{$cgiparams{'SERVICE'}} = " selected";
$selected{'START_HOUR'}{$cgiparams{'START_HOUR'}} = " selected";
$selected{'START_MINUTE'}{$cgiparams{'START_MINUTE'}} = " selected";
$selected{'END_HOUR'}{$cgiparams{'END_HOUR'}} = " selected";
$selected{'END_MINUTE'}{$cgiparams{'END_MINUTE'}} = " selected";
$selected{'RULE_ACTION'}{$cgiparams{'RULE_ACTION'}} = " selected";

&readhash("$settingsfile", \%cgiparams);

$checked{'GREEN_REJECTS'}{'off'} = '';
$checked{'GREEN_REJECTS'}{'on'} = '';  
$checked{'GREEN_REJECTS'}{$cgiparams{'GREEN_REJECTS'}} = 'CHECKED';

$checked{'PURPLE_REJECTS'}{'off'} = '';
$checked{'PURPLE_REJECTS'}{'on'} = '';  
$checked{'PURPLE_REJECTS'}{$cgiparams{'PURPLE_REJECTS'}} = 'CHECKED';

$checked{'ORANGE_REJECTS'}{'off'} = '';
$checked{'ORANGE_REJECTS'}{'on'} = '';  
$checked{'ORANGE_REJECTS'}{$cgiparams{'ORANGE_REJECTS'}} = 'CHECKED';

$checked{'GREEN_RELATED'}{'off'} = '';
$checked{'GREEN_RELATED'}{'on'} = '';  
$checked{'GREEN_RELATED'}{$cgiparams{'GREEN_RELATED'}} = 'CHECKED';

$checked{'PURPLE_RELATED'}{'off'} = '';
$checked{'PURPLE_RELATED'}{'on'} = '';  
$checked{'PURPLE_RELATED'}{$cgiparams{'PURPLE_RELATED'}} = 'CHECKED';

$checked{'ORANGE_RELATED'}{'off'} = '';
$checked{'ORANGE_RELATED'}{'on'} = '';  
$checked{'ORANGE_RELATED'}{$cgiparams{'ORANGE_RELATED'}} = 'CHECKED';

$checked{'RULEENABLED'}{'off'} = '';
$checked{'RULEENABLED'}{'on'} = '';  
$checked{'RULEENABLED'}{$cgiparams{'RULEENABLED'}} = 'CHECKED';

if ($updatebutton) {
	$buttontext = $tr{'tofc-update'};
	$boxtext = $tr{'tofc-update box'};
} else {
	$buttontext = $tr{'add'};
	$boxtext = $tr{'tofc-add'};
}

&openpage($tr{'tofc-timedoutgoing filtering'}, 1, '', 'networking');

print <<END

<script>
function ffoxSelectUpdate(elmt)
{
        if(!document.all) elmt.style.cssText =
        elmt.options[elmt.selectedIndex].style.cssText;
}
</script>
<script language="javascript" type="text/javascript">
	function checkWkends(form)
	{
		if (form.DAY_8.checked)
		{
			form.DAY_5.checked = 'checked';
			form.DAY_6.checked = 'checked';
		}
		else
		{
			form.DAY_5.checked = '';
			form.DAY_6.checked = '';
		}
	}
	function checkWkdays(form)
	{
		if (form.DAY_7.checked)
		{
			form.DAY_0.checked = 'checked';
			form.DAY_1.checked = 'checked';
			form.DAY_2.checked = 'checked';
			form.DAY_3.checked = 'checked';
			form.DAY_4.checked = 'checked';
		}
		else
		{
			form.DAY_0.checked = '';
			form.DAY_1.checked = '';
			form.DAY_2.checked = '';
			form.DAY_3.checked = '';
			form.DAY_4.checked = '';
		}
	}
	function checkAlldays(form) 
	{
		if (form.DAY_9.checked)
		{
			form.DAY_0.checked = 'checked';
			form.DAY_1.checked = 'checked';
			form.DAY_2.checked = 'checked';
			form.DAY_3.checked = 'checked';
			form.DAY_4.checked = 'checked';
			form.DAY_5.checked = 'checked';
			form.DAY_6.checked = 'checked';
		}
		else
		{
			form.DAY_0.checked = '';
			form.DAY_1.checked = '';
			form.DAY_2.checked = '';
			form.DAY_3.checked = '';
			form.DAY_4.checked = '';
			form.DAY_5.checked = '';
			form.DAY_6.checked = '';
		}
	}
</script>

END
;

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

&openbox($tr{'filtered interfaces'} . ':');
print "<form method='post'>\n";
print '<table style=\'width: 100%;\'>' . "\n";

my $unused = 6;
my $ifcolor;
my $dispcolor;
my $width = 90 / $unused;
foreach $interface (keys(%interfaces))
{
	if ($interfaces{$interface} eq '') { next; }

	if ($interface eq 'GREEN') {
		print qq{
		<tr>
		<td class='base' style='width: 30%;'>$tr{'traffic is 1'}$tr{'tofc-green'}$tr{'traffic is 2'}</td>
		<td style='width: 20%;'>
			<select name=\"$interface\">
				<option $selected{"$interface"}{'ACCEPT'} value='ACCEPT'>$tr{'allowed'}</option>
				<option $selected{"$interface"}{'REJECT'} value='REJECT'>$tr{'blocked'}</option>
				<option $selected{"$interface"}{'CLOSED'} value='CLOSED'>$tr{'tofc-closed'}</option>
			</select>
		</td>
		</tr>
		<tr>
		<td class='base' width='30%'>Log rejected packets on $tr{'tofc-green'}:</td>
		<td width='20%'><input type='checkbox' name='GREEN_REJECTS' $checked{'GREEN_REJECTS'}{'on'}></td>
		<td class='base' width='30%'>Allow related packets on $tr{'tofc-green'}:</td>
		<td width='20%'><input type='checkbox' name='GREEN_RELATED' $checked{'GREEN_RELATED'}{'on'}></td>
		</tr>
		};
	} elsif ($interface eq 'PURPLE') {
		print qq{
		<td>&nbsp;</td>
		<tr>
		<td class='base' style='width: 30%;'>$tr{'traffic is 1'}$tr{'tofc-purple'}$tr{'traffic is 2'}</td>
		<td style='width: 20%;'>
			<select name=\"$interface\">
				<option $selected{"$interface"}{'ACCEPT'} value='ACCEPT'>$tr{'allowed'}</option>
				<option $selected{"$interface"}{'REJECT'} value='REJECT'>$tr{'blocked'}</option>
				<option $selected{"$interface"}{'CLOSED'} value='CLOSED'>$tr{'tofc-closed'}</option>
			</select>
		</td>
		</tr>
		<tr>
		<td class='base'>Log rejected packets on $tr{'tofc-purple'}:</td>
		<td><input type='checkbox' name='PURPLE_REJECTS' $checked{'PURPLE_REJECTS'}{'on'}></td>
		<td class='base'>Allow related packets on $tr{'tofc-purple'}:</td>
		<td><input type='checkbox' name='PURPLE_RELATED' $checked{'PURPLE_RELATED'}{'on'}></td>
		</tr>
		};
	} elsif ($interface eq 'ORANGE') {
		print qq{
		<td>&nbsp;</td>
		<tr>
		<td class='base' style='width: 30%;'>$tr{'traffic is 1'}$tr{'tofc-orange'}$tr{'traffic is 2'}</td>
		<td style='width: 20%;'>
			<select name=\"$interface\">
				<option $selected{"$interface"}{'ACCEPT'} value='ACCEPT'>$tr{'allowed'}</option>
				<option $selected{"$interface"}{'REJECT'} value='REJECT'>$tr{'blocked'}</option>
				<option $selected{"$interface"}{'CLOSED'} value='CLOSED'>$tr{'tofc-closed'}</option>
			</select>
		</td>
		</tr>
		<tr>
		<td class='base'>Log rejected packets on $tr{'tofc-orange'}:</td>
		<td><input type='checkbox' name='ORANGE_REJECTS' $checked{'ORANGE_REJECTS'}{'on'}></td>
		<td class='base'>Allow related packets on $tr{'tofc-orange'}:</td>
		<td><input type='checkbox' name='ORANGE_RELATED' $checked{'ORANGE_RELATED'}{'on'}></td>
		</tr>
		};
	}
}
print "</table>" . "\n";
print qq{
<br>
<table width='100%'>
	<tr>
	<td colspan='3' style='text-align: center;'>
		<input type="submit" name="ACTION" value="$tr{'save'}" onClick="if(confirm('Changing outgoing states will remove all existing exceptions and time frames. Changing to Blocked with exceptions will add ONLY basic exceptions but NOT email, for example. Changing to the Closed state will block ALL outgoing connections. Are you sure you want to do this?')) {return true;} return false;">
	</td>
	</tr>
</table>
</form>
};

&closebox();

&openbox($boxtext);

print qq{
<form method='post'>
<table style='width: 100%;'>
<tr>
	<td class='base' style='width: 30%;'>$tr{'interface'}</td>
	<td style='width: 20%;'>
	<select style='color: green' onchange="ffoxSelectUpdate(this);" name='INTERFACE'>
};

foreach my $interface (keys %interfaces) {
	if ($interfaces{$interface} eq '') { next; }

	if ($interface eq 'GREEN') {
		$ifcolor = "\'color\: green\'";
		$dispcolor = "$tr{'tofc-green'}";
	} elsif ($interface eq 'PURPLE') {
		$ifcolor = "\'color\: purple\'";
		$dispcolor = "$tr{'tofc-purple'}";
	} elsif ($interface eq 'ORANGE') {
		$ifcolor = "\'color\: orange\'";
		$dispcolor = "$tr{'tofc-orange'}";
	}

	print "<option style=$ifcolor value='$interface' $selected{'INTERFACE'}{$interface}>$interface</option>\n"; 
}

print qq{
	</select>
	</td>
	<td class='base' width='30%'>$tr{'tofc-ipmac blank'} <IMG SRC='/ui/img/blob.gif'></td>
	<td width='20%'><input type='text' name='IPMAC' value='$cgiparams{'IPMAC'}'></td>
</tr>
<tr>
	@{[&portlist('SERVICE', $tr{'application servicec'}, 'PORT', $tr{'tofc-portc'}, $service)]}
</tr>
<tr>
	<td class='base' width='30%'>$tr{'tofc-protocol'}</td>
	<td width='20%'>
	<select name='PROTOCOL'>
};
	if ($cgiparams{'PROTOCOL'} eq '6') {
		print "<option value='TCP' selected>$availableprotocols{6}</option>";
	} else {
		print "<option value='TCP'>$availableprotocols{6}</option>";
	}

	if ($cgiparams{'PROTOCOL'} eq '17') {
		print "<option value='UDP' selected>$availableprotocols{17}</option>";
	} else {
		print "<option value='UDP'>$availableprotocols{17}</option>";
	}

	if ($cgiparams{'PROTOCOL'} eq 'TCP|UDP') {
		print "<option value='Both' selected>TCP|UDP</option>";
	} else {
		print "<option value='Both'>TCP|UDP</option>";
	}

	if ($cgiparams{'PROTOCOL'} eq '1') {
		print "<option value='ICMP' selected>$availableprotocols{1}</option>";
	} else {
		print "<option value='ICMP'>$availableprotocols{1}</option>";
	}

	if ($cgiparams{'PROTOCOL'} eq 'PPTP') {
		print "<option value='PPTP' selected>PPTP-Passthru</option>";
	} else {
		print "<option value='PPTP'>PPTP-Passthru</option>";
	}

	if ($cgiparams{'PROTOCOL'} eq 'IPSEC') {
		print "<option value='IPSEC' selected>IPSEC</option>";
	} else {
		print "<option value='IPSEC'>IPSEC</option>";

	}
print qq{
		</select></td>
	<td class='base' width='30%'>$tr{'enabled'}</td>
	<td width='20%'><input type='checkbox' name='RULEENABLED' $checked{'RULEENABLED'}{'on'}></td>
	<td>&nbsp;</td>
	<td>&nbsp;</td>

</tr>
<tr>
	<td class='base' width='25%'>$tr{'commentc'}</td>
	<td width='75%' colspan='3'><input type='text' style='width: 75%;' name='COMMENT' value='$cgiparams{'COMMENT'}' ></td>
</tr>
</table>
<br/>
<img src='/ui/img/blob.gif'> $tr{'tofc-blank all'}
<table width='100%'>
<tr>
	<td colspan='3' style='text-align: center;'>
		<input type="submit" name="ACTION" value='$buttontext' onclick='return validate();'>
		<INPUT TYPE='HIDDEN' name='RULE_ORDER' value='$cgiparams{'RULE_ORDER'}'>
		<INPUT TYPE='HIDDEN' name='TIMED' value='$cgiparams{'TIMED'}'>
		<INPUT TYPE='HIDDEN' name='TIME_DISP' value='$cgiparams{'TIME_DISP'}'>
		<INPUT TYPE='HIDDEN' name='TIMES' value='$cgiparams{'TIMES'}'>
	</td>
</tr>
</table>
};

&closebox();

&openbox($tr{'current exceptions'});

my $ports = &portmap();

my %render_settings =
(
	'url'     => "/cgi-bin/outgoing.cgi?[%COL%],[%ORD%],$cgiparams{'COLUMN'},$cgiparams{'ORDER'}",
	'columns' => 
	[
		{ 
			column => '1',
			title  => "$tr{'interfacenc'}",
			tr     => { 'GREEN' => '<b><font color=#00ff00>GREEN</font></b>', 'ORANGE' => '<b><font color=#ff8c00>ORANGE</font></b>', 'PURPLE' => '<b><font color=#9400d3>PURPLE</font></b>' },
			size   => 15,
			sort   => 'cmp',
		},
		{
			column => '3',
			title  => "$tr{'application service'}",
			size   => 20,
			sort   => 'cmp',
			tr     => \%{$ports},
		},
		{ 
			column => '5',
			title  => 'Protocol',
			size   => 10,
			sort   => 'cmp',
		},
		{ 
			column => '6',
			title  => 'IP or MAC address',
			size   => 15,
			sort   => 'cmp',
		},
		{ 
			column => '7',
			title  => 'Action',
			size   => 10,
			sort   => 'cmp',
		},
		{
			column => '9',
			title  => 'Timed',
			size   => 10,
			tr     => 'onoff',
			align  => 'center',
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
			size   => 5,
			mark   => ' ',
		},
		{ 
			column => '4',
			title => "$tr{'comment'}",
			break => 'line',
		},
		{
			column => '10',
			title => "Time frames",
			break => 'line',
		}
	]
);

&dispaliastab($config, \%render_settings, $cgiparams{'ORDER'}, $cgiparams{'COLUMN'} );

print <<END
<br>
<table class='blank'>
<tr>
	<td style='width: 15%; text-align: center;'><input type='submit' name='ACTION' value='$tr{'tofc-enabledc'}'></td>
	<td style='width: 20%; text-align: center;'><input type='submit' name='ACTION' value='$tr{'tofc-change action'}'></td>
	<td style='width: 25%; text-align: center;'><input type='submit' name='ACTION' value='$tr{'tofc-remove-exc'}' onClick="if(confirm('You are about to completely remove exceptions. Are you sure you want to do this?')) {return true;} return false;"></td>
	<td style='width: 25%; text-align: center;'><input type='submit' name='ACTION' value='$tr{'tofc-remove-tf'}'></td>
	<td style='width: 15%; text-align: center;'><input type='submit' name='ACTION' value='$tr{'edit'}'></td>
</tr>
</table>
END
;
&closebox();
&openbox($tr{'timed accessc'});

print qq{
<div align='center'>
<table width='100%'>
};

if ($cgiparams{'GREEN'} eq "REJECT" or $cgiparams{'GREEN'} eq "CLOSED") {
	print "<tr><td align='center'>$tr{'tofc-traffic allowed'} $tr{'tofc-green'} $tr{'tofc-during time'}</td></tr>\n";
} elsif ($cgiparams{'GREEN'} eq "ACCEPT") {
	print "<tr><td align='center'>$tr{'tofc-traffic blocked'} $tr{'tofc-green'} $tr{'tofc-during time'}</td></tr>\n";
}
if ($netsettings{'ORANGE_DEV'}) {
	if ($cgiparams{'ORANGE'} eq "REJECT" or $cgiparams{'ORANGE'} eq "CLOSED") {
		print "<tr><td align='center'>$tr{'tofc-traffic allowed'} $tr{'tofc-orange'} $tr{'tofc-during time'}</td></tr>\n";
	} elsif ($cgiparams{'ORANGE'} eq "ACCEPT") {
		print "<tr><td align='center'>$tr{'tofc-traffic blocked'} $tr{'tofc-orange'} $tr{'tofc-during time'}</td></tr>\n";
	}
}
if ($netsettings{'PURPLE_DEV'}) {
	if ($cgiparams{'PURPLE'} eq "REJECT" or $cgiparams{'PURPLE'} eq "CLOSED") {
		print "<tr><td align='center'>$tr{'tofc-traffic allowed'} $tr{'tofc-purple'} $tr{'tofc-during time'}</td></tr>\n";
	} elsif ($cgiparams{'PURPLE'} eq "ACCEPT") {
		print "<tr><td align='center'>$tr{'tofc-traffic blocked'} $tr{'tofc-purple'} $tr{'tofc-during time'}</td></tr>\n";
	}
}
print qq{
</table>
<br>
<table width='50%' align='center'>
<tr>
<td class='base' width='15%'>$tr{'fromc'}</td>
<td width='35%'><select name='START_HOUR'>
};
		foreach (@hour) {
   			print "\t<option value='$_' $selected{'START_HOUR'}{$_}>$_</option>\n";
		}
		print <<END
   	</select>
:
	<select name='START_MINUTE'>
END
;
		foreach (@minute) {
   			print "\t<option value='$_' $selected{'START_MINUTE'}{$_}>$_</option>\n";
		}
		print <<END
   	</select>
   </td>
<td class='base' width='15%'>$tr{'toc'}</td>
<td width='35%'><select name='END_HOUR'>
END
;
		foreach (@hour) {
   			print "\t<option value='$_' $selected{'END_HOUR'}{$_}>$_</option>\n";
		}
		print <<END
   	</select>
:
	<select name='END_MINUTE'>
END
;
		foreach (@minute) {
   			print "\t<option value='$_' $selected{'END_MINUTE'}{$_}>$_</option>\n";
		}
		print <<END
   	</select>
   </td>
</tr>
</table>
</div>
<div align='center' id='checkBoxes'>
<table width='50%'>
<tr>
END
;
for (my $day = 0; $day < 10; $day++)
{
	if ($day == 7)
	{
		print "</tr></table>\n";
		print "<table width='40%' align='center'><tr>";
		print "<td class='base' width='33%'>$tr{\"tofc-day $day\"}:</td>";
		print "<td><input type='checkbox' name='DAY_$day' $checked{\"DAY_${day}\"}{'on'} onClick='javaScript:checkWkdays(this.form);' ></td>\n";
	}
	elsif ($day == 8)
	{
		print "<td class='base' width='33%'>$tr{\"tofc-day $day\"}:</td>";
		print "<td><input type='checkbox' name='DAY_$day' $checked{\"DAY_${day}\"}{'on'} onClick='javaScript:checkWkends(this.form);'></td>\n";
	}
	elsif ($day == 9)
	{
		print "<td class='base' width='34%'>$tr{\"tofc-day $day\"}:</td>";
		print "<td><input type='checkbox' name='DAY_$day' $checked{\"DAY_${day}\"}{'on'} onClick='javaScript:checkAlldays(this.form);'></td>\n";
		print "</tr>";
	}
	else
	{
		print "<td class='base'>$tr{\"tofc-day $day\"}:</td>";
		print "<td><input type='checkbox' name='DAY_$day' $checked{\"DAY_${day}\"}{'on'}></td>\n";
	}
}
print <<END
</tr>
</table>
</div>
<div align='center'>
<table width='100%'>
<tr>
	<td style='text-align: center;'><input type='submit' name='ACTION' value='$tr{'tofc-schedule'}'></td>
</tr>
</table>
</div>

END
;

&closebox();

&alertbox('add','add');

&closebigbox();

&closepage();
