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

my @hour = ( 0 .. 23 );
my @minute = ( '00', '01', '02', '03', '04', '05', '06', '07', '08', '09', 10 .. 59 );

my $errormessage = '';
my $updatebutton = 0;

&showhttpheaders();

my ( %interfaces, %settings, %netsettings, %cgiparams, %selected, %checked );
my ($templine, @configs);

$cgiparams{'ACTION'} = '';

$cgiparams{'COLUMN'} = 1;
$cgiparams{'ORDER'} = $tr{'log ascending'};

$cgiparams{'OLDID'} = 0;
$cgiparams{'ORDER_NUMBER'} = '';
$cgiparams{'RULE_COUNT'} = 0;
$cgiparams{'RULEENABLED'} = 'on';

&getcgihash(\%cgiparams);

&readhash("${swroot}/ethernet/settings", \%netsettings);

if ($ENV{'QUERY_STRING'} && ( not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ))
{
	my @temp = split(',',$ENV{'QUERY_STRING'});
	$cgiparams{'ORDER'}  = $temp[1] if ( defined $temp[ 1 ] and $temp[ 1 ] ne "" );
	$cgiparams{'COLUMN'} = $temp[0] if ( defined $temp[ 0 ] and $temp[ 0 ] ne "" );
}

# Before we do anything else, let's make sure that if there are config file lines for the Purple or Orange
#  interfaces that the interfaces exist. This should fix any issues when a user changes his interfaces
#
open(FILE, $config) or die 'Unable to open config file';
my @tempfile = <FILE>;
close FILE;

open(FILE, ">$config") or die 'Unable to open config file';
foreach $templine (@tempfile) {
	chomp $templine;
	@configs = split /,/, $templine;
	if ($configs[0] eq "GREEN") { 	# GREEN is always configured
		print FILE "$templine\n";
	} elsif ($configs[0] eq "PURPLE") {
		if ($netsettings{'PURPLE_DEV'}) {
			print FILE "$templine\n";
		} else {
			next;
		}
	} elsif ($configs[0] eq "ORANGE") {
		if ($netsettings{'ORANGE_DEV'}) {
			print FILE "$templine\n";
		} else {
			next;
		}
	}
}
close FILE;

$interfaces{'GREEN'} = $netsettings{'GREEN_DEV'};
$interfaces{'ORANGE'} = $netsettings{'ORANGE_DEV'};
$interfaces{'PURPLE'} = $netsettings{'PURPLE_DEV'};

if ($cgiparams{'ACTION'} eq '') {
	$cgiparams{'RULEENABLED'} = 'on';
}

&readhash("$settingsfile", \%settings);

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
	my $interface	 = $cgiparams{'INTERFACE'};
	my $enabled	 = $cgiparams{'RULEENABLED'};
	my $service    = $cgiparams{'SERVICE'};
	my $port       = $cgiparams{'PORT'};
	my $protocol   = $cgiparams{'PROTOCOL'};
	my $ipmac      = $cgiparams{'IPMAC'};
	my $order	 = $cgiparams{'ORDER_NUMBER'};
	my $target	 = $cgiparams{'TARGET'};
	my $comment    = $cgiparams{'COMMENT'};
	my @singleip;
	my $setproxy 	 = 'off';
	my $timeon	 = 'off';
	my $timechk	 = '';
	my $both	 = "TCP\&UDP";

	&writehash("$hashfile", \%cgiparams);

	if ($port =~ /[a-zA-Z]/) {
		$service = $port;
	}

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

	if ( $cgiparams{'SERVICE'} eq "All VPNs" ) {
		$protocol = "All VPNs";
	}
	if ( $cgiparams{'SERVICE'} eq "500:4500" ) {
		$protocol = "IPSec";
	}
	if ( $cgiparams{'SERVICE'} eq "1723" ) {
		$protocol = "PPTP";
	}
	if ( $cgiparams{'SERVICE'} eq "1194:1195" ) {
		$protocol = "OpenVPN";
	}

	if ($protocol eq "PPTP" or $protocol eq "IPSEC") {
		if ($service ne "user" and $port ne "") {
			$errormessage .= "$tr{'tofc-port and protocol'}<BR />\n";
		}
	}

	if ($service eq 'Infrastructure') {
		$protocol = 'Both';
	}

	if ($ipmac eq "") {
		$ipmac = 'N/A';
	} elsif ( (&validipormask ($ipmac)) or (&validmac ($ipmac) or $ipmac =~ /\-/) ) {
		$ipmac = $cgiparams{'IPMAC'};
	} else {
		$errormessage .= "$tr{'tofc-ipmac bad'}<BR />\n";
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

		if ($service eq "80" or $service =~ /^80-/ or $service =~ /-80-/ or $service =~ /-80$/ or $service eq "Web") {
			$setproxy = "on";
		} else {
			$setproxy = "off";
		}
	}

  	#########################################
  	# Check for IP's in subnets
  	#########################################
	if ($ipmac =~ /:/ or $ipmac eq 'N/A') {
		goto EXIT;
	}
	if ($ipmac =~ /-/) {
		@singleip = split /\-/, $ipmac;
	} else {
		$singleip[0] = $ipmac;
		$singleip[1] = $ipmac;
	}
  	my $greenipobj = new NetAddr::IP "$netsettings{'GREEN_ADDRESS'}/$netsettings{'GREEN_NETMASK'}";
  	my $orangeipobj = new NetAddr::IP "$netsettings{'ORANGE_ADDRESS'}/$netsettings{'ORANGE_NETMASK'}";;
  	my $purpleipobj = new NetAddr::IP "$netsettings{'PURPLE_ADDRESS'}/$netsettings{'PURPLE_NETMASK'}";;
  	my $srcipaddrobj1 = "";
  	my $srcipaddrobj2 = "";

	$greenipobj = new NetAddr::IP "$netsettings{'GREEN_ADDRESS'}/$netsettings{'GREEN_NETMASK'}";

	if ($netsettings{'ORANGE_ADDRESS'} ne '') {
		$orangeipobj = new NetAddr::IP "$netsettings{'ORANGE_ADDRESS'}/$netsettings{'ORANGE_NETMASK'}";
  	}

  	if ($netsettings{'PURPLE_ADDRESS'} ne '') {
    		$purpleipobj = new NetAddr::IP "$netsettings{'PURPLE_ADDRESS'}/$netsettings{'PURPLE_NETMASK'}";
  	}

    	$srcipaddrobj1 = new NetAddr::IP "$singleip[0]";
    	$srcipaddrobj2 = new NetAddr::IP "$singleip[1]";

      	if ($cgiparams{'INTERFACE'} eq 'GREEN') {
       	if (!$greenipobj->contains($srcipaddrobj1) or !$greenipobj->contains($srcipaddrobj2)) {
          		$errormessage .= "IP range is not within your Green subnet!<BR />\n";
      		}
      	} elsif (($netsettings{'ORANGE_ADDRESS'} ne '') &&
       	($cgiparams{'INTERFACE'} eq 'ORANGE')) {
       	if (!$orangeipobj->contains($srcipaddrobj1) or !$orangeipobj->contains($srcipaddrobj2)) {
          		$errormessage .= "IP range is not within your Orange subnet!<BR />\n";
      		}
      	} elsif (($netsettings{'PURPLE_ADDRESS'} ne '') &&
              ($cgiparams{'INTERFACE'} eq 'PURPLE')) {
       	if (!$purpleipobj->contains($srcipaddrobj1) or !$purpleipobj->contains($srcipaddrobj2)) {
          		$errormessage .= "IP range is not within your Purple subnet!<BR />\n";
      		}
      	}
EXIT:
  unless ($errormessage) {
    if ($cgiparams{'ACTION'} eq $tr{'tofc-update'}) {
      open(FILE, "$config") or die 'Unable to open config file.';
      my @current = <FILE>;
      close(FILE);

      my $line;
      my $cnt = 0;
      open(FILE, ">$config") or die 'Unable to open config file.';
      flock FILE, 2;

      foreach $line (@current) {
        $cnt++;

        if ($cnt >= $cgiparams{'OLDID'}) {
          if ($cnt != $cgiparams{'OLDID'}) {
            chomp $line;
            my @temp = split /,/, $line;
	     my @times = split /\+/, $line;
	     if ($times[1]) {
		 $timeon = 'on';
		 $timechk = "+$times[1]";
	     }
            $temp[7]--;
            print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],";
            print FILE "$temp[6],$temp[7],$temp[8],$timeon,$timechk\n";
          }
        } else {
            print FILE "$line";
        }
      }
      close(FILE);
    }

    open(FILE, "$config") or die 'Unable to open config file.';
    my @current = <FILE>;
    close(FILE);

    my $line;
    my $notadded = 1;
    my $cnt = 0;
    open(FILE, ">$config") or die 'Unable to open config file.';
    flock FILE, 2;

    foreach $line (@current) {
      $cnt++;
      $timeon = 'off';
      $timechk = '';
      @times = split /\+/, $line;
      if ($times[1]) {
	   $timeon = 'on';
	   $timechk = "+$times[1]";
      }
      if ($cgiparams{'TIMES'}) {
	   $timeon = 'on';
	   $timechk = "+$cgiparams{'TIMES'}";
      }
      if ($cnt == $cgiparams{'ORDER_NUMBER'}) {
        if ($cgiparams{'PROTOCOL'} eq "Both") {
          print FILE "$cgiparams{'INTERFACE'},$cgiparams{'RULEENABLED'},";
          print FILE "$service,$cgiparams{'COMMENT'},";
          print FILE "$both,$ipmac,";
          print FILE "$cgiparams{'TARGET'},$cnt,$setproxy,";
          print FILE "$timeon,$timechk\n";
          $notadded = 0;
          $cnt++;
        } else {
          print FILE "$cgiparams{'INTERFACE'},$cgiparams{'RULEENABLED'},";
          print FILE "$service,$cgiparams{'COMMENT'},";
          print FILE "$protocol,$ipmac,";
          print FILE "$cgiparams{'TARGET'},$cnt,$setproxy,";
          print FILE "$timeon,$timechk\n";
          $notadded = 0;
          $cnt++;
        }
      }

      chomp $line;
      my @temp = split /,/, $line;
      my @times = split /\+/, $line;
      if ($times[1]) {
	  $temp[10] = "+$times[1]";
      }
      print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],";
      print FILE "$temp[6],$cnt,$temp[8],$temp[9],$temp[10]\n";
    }

    if ($notadded) {
      my $timeon = 'off';
      my $timechk = '';
      if($cgiparams{'TIMES'}) {
	 $timeon = 'on';
	 $timechk = "+$cgiparams{'TIMES'}";
      }
      if ($protocol eq "Both") {
        print FILE "$cgiparams{'INTERFACE'},$cgiparams{'RULEENABLED'},";
        print FILE "$service,$cgiparams{'COMMENT'},$both,$ipmac,";
        print FILE "$cgiparams{'TARGET'},$cgiparams{'ORDER_NUMBER'},$setproxy,";
        print FILE "$timeon,$timechk\n";
      } else {
        print FILE "$cgiparams{'INTERFACE'},$cgiparams{'RULEENABLED'},";
        print FILE "$service,$cgiparams{'COMMENT'},$protocol,$ipmac,";
        print FILE "$cgiparams{'TARGET'},$cgiparams{'ORDER_NUMBER'},$setproxy,";
        print FILE "$timeon,$timechk\n";
      }
    }
    close(FILE);

    &writehash("$hashfile", \%cgiparams);

    if ($cgiparams{'ACTION'} eq $tr{'add'}) {
      &log($tr{'Outgoing rule added'});
    } else {
      &log($tr{'Outgoing rule updated'});
    }

    my $success = message('setoutgoing');
  
    unless (defined $success) { 
	$errormessage .= "$tr{'smoothd failure'}<BR />\n"; }
  }

	$cgiparams{'ORDER_NUMBER'} = "0";
	open(FILE, "$config") or die 'Unable to open config file';
	while (<FILE>) {
		$cgiparams{'ORDER_NUMBER'}++;
	}
	close FILE;

	$cgiparams{'ORDER_NUMBER'}++;
	undef $cgiparams{'INTERFACE'};
	undef $cgiparams{'PROTOCOL'};
	undef $cgiparams{'IPMAC'}; 
	undef $cgiparams{'PORT'};
	undef $cgiparams{'COMMENT'};
	undef $cgiparams{'TIMES'};
	$cgiparams{'TARGET'} = "ACCEPT";
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
		$errormessage .= "Start time cannot equal End time!<BR />\n";
	}
    unless ($errormessage) {
	my ($Sun, $Mon, $Tue, $Wed, $Thu, $Fri, $Sat, $days);
	my $flag = '';
	# Convert days of week to numerical values and build days of week argument
	my $cnt = 0;
	if ($cgiparams{'DAY_6'} eq "on") {
		$sun = 'Sun ';
		$flag = 1;
		$cnt++;
	}
	if ($cgiparams{'DAY_0'} eq "on") {
		$mon = 'Mon ';
		$flag = 1;
		$cnt++;
	}
	if ($cgiparams{'DAY_1'} eq "on") {
		$tue = 'Tue ';
		$flag = 1;
		$cnt++;
	}
	if ($cgiparams{'DAY_2'} eq "on") {
		$wed = 'Wed ';
		$flag = 1;
		$cnt++;
	}
	if ($cgiparams{'DAY_3'} eq "on") {
		$thu = 'Thu ';
		$flag = 1;
		$cnt++;
	}
	if ($cgiparams{'DAY_4'} eq "on") {
		$fri = 'Fri ';
		$flag = 1;
		$cnt++;
	}
	if ($cgiparams{'DAY_5'} eq "on") {
		$sat = 'Sat ';
		$flag = 1;
		$cnt++;
	}
	if (defined $flag) {
		$days = "$mon$tue$wed$thu$fri$sat$sun";
		chop $days;
	}

	if ($cgiparams{'DAY_9'} eq "on" and $flag eq '') {
		$days = '';
	}

	if ($cnt == 7) {
		$timedays = 'Every day';
	} else {
		$timedays = $days;
	}

	my $timedisp = "$timedays $timestart to $timestop";

	if (open(FILE, "$config")) {
		@temp = <FILE>;
		close FILE;
	}
	open(FILE, ">$config") or die 'unable to open config file';
	flock FILE, 2;
	$id = 0;

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
			$timedisp = "$cfg_ln[6] from $timestart to $timestop2 on $timedays";
			if ($cfg_ln[9] eq 'off') {
				$cfg_ln[9] = 'on';
				$cfg_ln[10] = "+$timedisp";
			} else {
				$cfg_ln[10] = "$cfg_ln[10] | $timedisp";
			}
			push (@cfg_ln, $days, $timestart, $timestop2);

			$timestart2 = "00:00";
			$cfg_ln[10] = "$cfg_ln[10] | $cfg_ln[6] from $timestart2 to $timestop on $timedays";
			push (@cfg_ln, $days, $timestart2, $timestop);
			my $cfg_ln_cnt = @cfg_ln;
			for (my $i = 0; $i < $cfg_ln_cnt; $i++) {
				print FILE "$cfg_ln[$i],";
			}
			print FILE "\n";
	# This bit for the user who is careful and enters appropriate time frames
		} else { 
			$timedisp = "$cfg_ln[6] from $timestart to $timestop on $timedays";
			if ($cfg_ln[9] eq 'off') {
				$cfg_ln[9] = 'on';
				$cfg_ln[10] = "+$timedisp";
			} else {
				$cfg_ln[10] = "$cfg_ln[10] | $timedisp";
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

	$success = message('setoutgoing');

	unless (defined $success) {
		$errormessage .= "Unable to set outgoing rules<BR />\n"; 
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
		$errormessage .= "$tr{'nothing selected'}<BR />\n";  }

	if ($count > 1 && $cgiparams{'ACTION'} eq $tr{'edit'}) {
		$errormessage .= "$tr{'you can only select one item to edit'}<BR />\n";  }

	unless ($errormessage)
	{
		open(FILE, ">$config") or die 'Unable to open config file.';
		flock FILE, 2;
		$id = 0;
		$count = 1;
		foreach $line (@current)
		{
			$id++;
			unless ($cgiparams{$id} eq "on") {
				chomp $line;
				@temp = split /,/, $line;
				@times = split /\+/, $line;
				if ($times[1]) {
					$temp[10] = "+$times[1]";
				}
       			$temp[7] = $count;
        			print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],";
        			print FILE "$temp[5],$temp[6],$temp[7],$temp[8],$temp[9],$temp[10]\n";
				$count++; 
			} elsif ($cgiparams{'ACTION'} eq $tr{'edit'}) {
				chomp $line;
				@temp = split /,/, $line;
				@times = split /\+/, $line;
				$cgiparams{'INTERFACE'} = $temp[0];
				if ($temp[2] =~ /-/) {
					$temp[2] =~ s/-/,/g;
				}
				$service = $temp[2];
				if ($temp[4] eq "TCP") {
					$cgiparams{'PROTOCOL'} = "6";
				} elsif ($temp[4] eq "UDP") {
					$cgiparams{'PROTOCOL'} = "17";
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
				$cgiparams{'TARGET'} = $temp[6];
				$cgiparams{'ORDER_NUMBER'} = $temp[7];
				$cgiparams{'COMMENT'} = $temp[3];
				$cgiparams{'TIMED'} = $temp[9];
				$cgiparams{'TIMES'} = $times[1];

				# Editing support
        			$cgiparams{'OLDID'} = $id;
				$updatebutton = 1;
				print FILE "$line\n";
				$count++;
			} elsif ($cgiparams{'ACTION'} eq $tr{'tofc-remove-tf'}) {
				chomp $line;
				@temp = split /,/, $line;
        			$temp[7] = $count;
				print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$temp[7],$temp[8],off\n";
				$count++;
			}
		}
		close(FILE);

    		if ($cgiparams{'ACTION'} eq $tr{'tofc-remove-exc'}) {
      			$cgiparams{'IPMAC'} = '';
      			$cgiparams{'PORT'} = '';
      			$cgiparams{'TARGET'} = 'ACCEPT';
      			$cgiparams{'RULEENABLED'} = 'on';
      			$cgiparams{'COMMENT'} = '';
      			$cgiparams{'ORDER_NUMBER'} = $id;

      			&log('Outgoing rule removed');
    		} else {
      			$cgiparams{'RULE_COUNT'} = $id;
    		}

		my $success = message('setoutgoing');

		unless (defined $success) {
			$errormessage .= "$tr{'smoothd failure'}<BR />\n"; }
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
				print FILE "$temp[0],on,$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$temp[7],$temp[8],$temp[9],$temp[10]\n";
			} elsif ( $temp[1] eq "on" ) {
				print FILE "$temp[0],off,$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$temp[7],$temp[8],$temp[9],$temp[10]\n";
			}
		}
		close FILE;
		my $success = message('setoutgoing');
	
		unless (defined $success) {
			$errormessage .= "$tr{'smoothd failure'}<BR />\n"; }
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
			print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$action,$temp[7],$temp[8],$temp[9],$temp[10]\n";
		} elsif ($temp[6] eq "REJECT") {
			$action = "ACCEPT";
			print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$action,$temp[7],$temp[8],$temp[9],$temp[10]\n";
		} 
	}
	close FILE;

	my $success = message('setoutgoing');

	unless (defined $success) {
		$errormessage .= "$tr{'smoothd failure'}<BR />\n"; }
}

# Finished with action handling
#
open(FILE, "$config") or die 'Unable to open config file.';
while (<FILE>) { $cgiparams{'RULE_COUNT'}++; }
close(FILE);
$cgiparams{'RULE_COUNT'}++;
#$cgiparams{'ORDER_NUMBER'} = $cgiparams{'RULE_COUNT'};

# Check for normal page load
if ($cgiparams{'ACTION'} eq '') {
  $cgiparams{'RULEENABLED'} = 'on';
  $cgiparams{'TARGET'} = 'ACCEPT';
  $cgiparams{'ORDER_NUMBER'} = $cgiparams{'RULE_COUNT'};
  $cgiparams{'COMMENT'} = '';
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

$selected{'TARGET'}{'ACCEPT'} = '';
$selected{'TARGET'}{'DROP'} = '';  
$selected{'TARGET'}{'REJECT'} = '';  
$selected{'TARGET'}{'LOG'} = '';  
$selected{'TARGET'}{$cgiparams{'TARGET'}} = 'selected';

&readhash("$settingsfile", \%cgiparams);

$checked{'RULEENABLED'}{'off'} = '';
$checked{'RULEENABLED'}{'on'} = '';  
$checked{'RULEENABLED'}{$cgiparams{'RULEENABLED'}} = 'checked';

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
			form.DAY_0.checked = '';
			form.DAY_1.checked = '';
			form.DAY_2.checked = '';
			form.DAY_3.checked = '';
			form.DAY_4.checked = '';
			form.DAY_5.checked = 'checked';
			form.DAY_6.checked = 'checked';
			form.DAY_7.checked = '';
			form.DAY_9.checked = '';
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
			form.DAY_5.checked = '';
			form.DAY_6.checked = '';
			form.DAY_8.checked = '';
			form.DAY_9.checked = '';
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
			form.DAY_7.checked = '';
			form.DAY_8.checked = '';
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

my $unused = 6;
my $ifcolor;
my $dispcolor;
my $width = 90 / $unused;

&openbox($boxtext);

print qq{
<form method='post'>
<table style='width: 100%;' style='margin:6pt 0'>
<tr>
	<td class='base' style='width: 30%;'>$tr{'tofc-interface'}:</td>
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
	<td class='base' width='30%'><IMG SRC='/ui/img/blob.gif'> $tr{'tofc-ipmac blank'}</td>
	<td width='20%'><input type='text' name='IPMAC' value='$cgiparams{'IPMAC'}'></td>
</tr>
<tr>
	@{[&portlist('SERVICE', $tr{'application servicec'}, 'PORT', $tr{'portc'}, $service)]}
</tr>
  <tr>
    <td colspan='6'>

      <table width='100%' style='margin:6pt 0'>
	<tr>
	<td class='base'>$tr{'tofc-protocol'}</td>
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
          <td class='base'>$tr{'ffc-targetc'}</td>
          <td>
            <select name='TARGET'>
              <option value='ACCEPT' $selected{'TARGET'}{'ACCEPT'}>$tr{'target accept'}</option>
              <option value='REJECT' $selected{'TARGET'}{'REJECT'}>$tr{'target reject'}</option>
            </select></td>
          <td class='base'>$tr{'order numberc'}</td>
          <td>
            <select name='ORDER_NUMBER'>
};

        for ($cnt = 1; $cnt < $cgiparams{'RULE_COUNT'} + 1; $cnt++) 
	 {
          if ($cnt eq $cgiparams{'ORDER_NUMBER'}) {
            print "<option value='$cnt' selected>$cnt</option>\n";
          } else {
            print "<option value='$cnt'>$cnt</option>\n";
          }
        }

print qq{ </select>
          </td>
	</tr>
	<tr>
		<td class='base'>$tr{'commentc'}</td>
		<td colspan='5'><input type='text' size='80' name='COMMENT' value='$cgiparams{'COMMENT'}' ></td>
	</tr>
</table>
</td></tr>
<table width='100%'>
<tr>
	<tr>
	<td class='base' width='30%'>$tr{'enabled'}</td>
	<td width='20%'><input type='checkbox' name='RULEENABLED' $checked{'RULEENABLED'}{'on'}></td>

	<td colspan='3' style='text-align: center;'>
	<input type="submit" name="ACTION" value='$buttontext' onclick='return validate();'>
       <input type='hidden' name='OLDID' value='$cgiparams{'OLDID'}'>
	<input type='hidden' name='TIMED' value='$cgiparams{'TIMED'}'>
	<input type='hidden' name='TIMES' value='$cgiparams{'TIMES'}'>
	</td>
</tr>
</table>
<br/>
<img src='/ui/img/blob.gif'> $tr{'tofc-blank all'}
};

&closebox();

&openbox($tr{'current exceptions'});

my $portmap = &portmap();

my %render_settings =
(
	'url'     => "/cgi-bin/outgoing.cgi?[%COL%],[%ORD%],$cgiparams{'COLUMN'},$cgiparams{'ORDER'}",
	'columns' => 
	[
		{ 
			column => '8',
      			title  => $tr{'order'},
			size   => 5,
			sort   => '<=>',
		},
		{ 
			column => '1',
			title  => $tr{'interfacenc'},
			tr     => { 'GREEN' => '<b><font color=#00ff00>GREEN</font></b>', 'ORANGE' => '<b><font color=#ff8c00>ORANGE</font></b>', 'PURPLE' => '<b><font color=#9400d3>PURPLE</font></b>' },
			size   => 15,
			sort   => 'cmp',
		},
		{
			column => '3',
			title  => $tr{'application service'},
			size   => 20,
			sort   => 'cmp',
			tr     => \%{$portmap},
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
			column => '10',
			title  => 'Timed',
			size   => 10,
			tr     => 'onoff',
			align  => 'center',
		},
		{
			column => '2',
			title  => $tr{'enabledtitle'},
			size   => 10,
			tr     => 'onoff',
			align  => 'center',
		},
		{
			title  => $tr{'mark'}, 
			size   => 5,
			mark   => ' ',
		},
		{ 
			column => '4',
			title => $tr{'comment'},
			break => 'line',
		},
		{
			column => '11',
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
		print "<td><input type='radio' name='DAY_$day' $checked{\"DAY_${day}\"}{'on'} onClick='javaScript:checkWkdays(this.form);' ></td>\n";
	}
	elsif ($day == 8)
	{
		print "<td class='base' width='33%'>$tr{\"tofc-day $day\"}:</td>";
		print "<td><input type='radio' name='DAY_$day' $checked{\"DAY_${day}\"}{'on'} onClick='javaScript:checkWkends(this.form);'></td>\n";
	}
	elsif ($day == 9)
	{
		print "<td class='base' width='34%'>$tr{\"tofc-day $day\"}:</td>";
		print "<td><input type='radio' name='DAY_$day' $checked{\"DAY_${day}\"}{'on'} onClick='javaScript:checkAlldays(this.form);'></td>\n";
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
<img src='/ui/img/blob.gif'> Remember to select the rule you want to add<br>&nbsp;&nbsp;time frames to before selecting the time frames.
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
