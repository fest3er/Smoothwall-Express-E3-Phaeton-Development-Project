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
#                                                                                               #
# June 10, 2009 changed how exceptions for VPN protocols are written. Selecting                 #
# IPSEC, for example, will write all the rules necessary for IPSEC protocols and ports          #
#                                                                                               #
# June 7, 2011 A number of small changes and fixes up to this date.                             #
#  -Fixed a problem with selecting all ports                                                    #
#  -Added ability to remove only time frames from an exception and leave the exception in place #
#  -Smoothd module enhanced and updated courtesy of Steve Pittman (MtnLion)                     #
#                                                                                               #
# August 2011 More modifications for use of the xtables-addons xt_iprange xt_multiport and      #
#   xt_time. Also ported this mod to Roadster                                                   #
#                                                                                               #
#################################################################################################

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );
use smoothtype qw( :standard );
use NetAddr::IP;

# Subroutine for sorting things numerically instead of as strings
sub numerically { $a <=> $b; }

# 0 - turn debuggerer off
# 1 - show informational debuggerer stuff
my $debuggerer = 0;

my $moddir = '/var/smoothwall/outgoing';
my $config = "$moddir/config";
my $settingsfile = "$moddir/settings";
my $hashfile = "$moddir/confighash";
my $halfopen = "/var/smoothwall-halfopen/outgoing/config";

my @hour = ( 0 .. 23 );
my @minute = ( '00', '01', '02', '03', '04', '05', '06', '07', '08', '09', 10 .. 59 );

my $errormessage = '';
my $infomessage = '';
my $updatebutton = 0;

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

$infomessage .= "<pre>\n";
#$infomessage .= "ENV\n";
#foreach $i (sort keys %ENV) {
#  $infomessage .= "  $i->$ENV{$i}\n";
#  }
#$infomessage .= "\n";
$infomessage .= "cgiparams\n";
foreach $i (sort keys %cgiparams) {
  $infomessage .= "  $i->$cgiparams{$i}\n";
  }
$infomessage .= "</pre>\n";

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
  if ($configs[0] eq "GREEN") {   # GREEN is always configured
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


# Only check actions if it has (a) value.
if (defined $cgiparams{'ACTION'} and $cgiparams{'ACTION'} ne "")
{
  $infomessage .= "action found<br />\n";
  # Start action handling
  #

  # Action: add/update
  if ($cgiparams{'ACTION'} eq $tr{'add'} or $cgiparams{'ACTION'} eq $tr{'tofc-update'})
  {
    $infomessage .= "add/update clicked<br />\n";
                                                     # config field #
    my $interface  = $cgiparams{'INTERFACE'};        #    1
    my $enabled    = $cgiparams{'RULEENABLED'};      #    2
    my $service    = $cgiparams{'SERVICE'};          #    3
    my $port       = $cgiparams{'PORT'};             #    3
    my $protocol   = $cgiparams{'PROTOCOL'};         #    5
    my $ipmac      = $cgiparams{'IPMAC'};            #    6
    my $order      = $cgiparams{'ORDER_NUMBER'};     #    8
    my $target     = $cgiparams{'TARGET'};           #    7
    my $comment    = $cgiparams{'COMMENT'};          #    4
    my @singleip;
    my $setproxy   = 'off';
    my $timeon     = 'off';
    my $timechk    = '';
    my $both       = "TCP\&UDP";

    &writehash("$hashfile", \%cgiparams);

    # If the port begins with a letter, it's a service name
    if ($port =~ /[a-zA-Z]/) {
      $service = $port;
    }

    # Prepare the port number, or show 'not applicable'
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

    # If a VPN service was specified, store the most specific name in 'protocol'
    if ( $cgiparams{'SERVICE'} eq "VPNs" ) {
      $protocol = "VPNs";
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

    # PPTP protocol knows its ports; don't specify them
    if ($protocol eq "PPTP" or $protocol eq "IPSEC") {
      if ($service ne "user" and $port ne "") {
        $errormessage .= "$tr{'tofc-port and protocol'}<BR />\n";
      }
    }

    # Currently only DNS
    if ($service eq 'Infrastructure') {
      $protocol = 'Both';
    }

    # Validate IP addr(s)
    if ($ipmac eq "") {
      $ipmac = 'N/A';
    }
    if ($ipmac ne "N/A" ) {
      if ( (&validipormask ($ipmac)) or (&validmac ($ipmac) or $ipmac =~ /\-/) ) {
        $ipmac = $cgiparams{'IPMAC'};
      } else {
        $errormessage .= "$tr{'tofc-ipmac bad'}<BR />\n";
      }
    }

    # Active or not
    if ($cgiparams{'RULEENABLED'} eq "on") {
      $enabled = "on";
    } else {
      $enabled = "off";
    }

    # Get user-spec'ed port nums/ranges
    if ($cgiparams{'PORT'}) {
      &readhash("$hashfile", \%cgiparams);
      $service = $cgiparams{'PORT'};
      $service =~ s/,/-/g;

      # Note if proxy is needed
      if ($service eq "80" or $service =~ /^80-/ or $service =~ /-80-/ or $service =~ /-80$/ or $service eq "Web") {
        $setproxy = "on";
      } else {
        $setproxy = "off";
      }
    }

    #########################################
    # Check for IP's in subnets
    #########################################
    if ( !($ipmac =~ /:/ or $ipmac eq 'N/A')) {
      if ($ipmac =~ /-/) {
        @singleip = split /\-/, $ipmac;
      } else {
        $singleip[0] = $ipmac;
        $singleip[1] = $ipmac;
      }
      my $greenipobj;
      my $orangeipobj;
      my $purpleipobj;
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
    }

    unless ($errormessage) {
      # Whether adding or updating,
      #   1. Read config file into assoc. array
      #   2. Multiplying the order #s by 10 to give room for the new or moved one
      #   3. Add the new/moved one using index ((10 * ORDER_NUMBER) - 1)
      #   4. If moved, undef index 10 * OLDID
      #   5. Then sort the keys and write the entries with new order numbers.
      # Order numbers will now always be sequential from 1 to n, without gaps.
      # Boundary conditions are much less problematic.

      if (($cgiparams{'ACTION'} eq $tr{'tofc-update'}) or
          ($cgiparams{'ACTION'} eq $tr{'add'})) {
        my $cnt = 0;
        my $notadded = 1;
        my $newIdx;
        my (%current, $line, $newIdx, $oldIdx, @temp, @time, $timeon, $timechk);

        # Read the file into assoc. array 'current', indexed by 10x the file's order numbers.
        open(FILE, "$config") or die 'Unable to read config file.';
        while (<FILE>) {
          my @splt;
          chomp;
          @splt = split /,/, $_;
          $current{10*$splt[7]} = $_;
        }
        close(FILE);

        if ($cgiparams{'ORDER_NUMBER'} != $cgiparams{'OLDID'}) {
          # If an addition or update and the entry's order number changed, change its index and delete
          #   (undef) the original.

          # Prepare an empty entry; it'll be filled in a bit
          $newIdx = (10 * $cgiparams{'ORDER_NUMBER'}) - 1;
          $current{$newIdx} = "";
          # delete the old location only when moving an existing entry
          #   (ObviousMan: "Clearly, new entries don't have corresponding old entries."
          if ($cgiparams{'OLDID'} != 0) {
            my $oldIdx = (10 * $cgiparams{'OLDID'});
            undef $current{$oldIdx};
          }
        } else {
          # Same order, but we need $newIdx anyway.
          $newIdx = 10 * $cgiparams{'ORDER_NUMBER'};
        }

        # Now sort the keys and re-write the file in the correct order.

        if (! open(FILE, ">$config")) {
          $error_message .= 'Unable to write config file.\n';
        } else {
          flock FILE, 2;

          # Rewrite the file, renumbering the rules from 1 to n, by 1.
          foreach $idx (sort numerically keys %current) { 
            next if (!defined $current{$idx});
            $cnt++;

            # Print the entry with its new order #
            if ($idx == $newIdx) {
              #$infomessage .= "New/Chg'd entry: $idx==$newIdx<br />\n";
              # Use cgiparms for new/updated/moved entry

              # Time frame to handle?
              if ($cgiparams{'TIMES'}) {
                $timeon = 'on';
                $timechk = "+$cgiparams{'TIMES'}";
              } else {
                $timeon = "";
                $timechk = "";
              }
              # Shovel it out
              {
                my $outLine = "";
                $outLine .= "$cgiparams{'INTERFACE'},$cgiparams{'RULEENABLED'},";
                $outLine .= "$service,$cgiparams{'COMMENT'},";
                if ($cgiparams{'PROTOCOL'} eq "Both") {
                  $outLine .= "$both,$ipmac,";
                } else {
                  $outLine .= "$protocol,$ipmac,";
                }
                $outLine .= "$cgiparams{'TARGET'},$cnt,$setproxy,";
                $outLine .= "$timeon,$timechk";
                $infomessage .= "Add/Update: $outLine<br />\n";
                print FILE $outLine ."\n";
              }

            } else {

              # Get the untouched entry and fission it
              my @temp = split /,/, $current{$idx};
              my @times = split /\+/, $current{$idx};

              # Time frame to handle?
              if ($times[1]) {
                $timeon = 'on';
                $timechk = "+$times[1]";
              } else {
                $timeon = 'off';
                $timechk = "";
              }
              # Shovel it out
              {
                my $outLine = "";
                $outLine = "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],";
                $outLine .= "$temp[6],$cnt,$temp[8],$timeon,$timechk";
                #$infomessage .= "NoChange: $outLine<br />\n";
                print FILE "$outLine\n";
              }
            }
          }
          close(FILE);
        }
      }


      &writehash("$hashfile", \%cgiparams);

      if ($cgiparams{'ACTION'} eq $tr{'add'}) {
        &log($tr{'Outgoing rule added'});
        $infomessage .= "$tr{'Outgoing rule added'}<br />\n";
      } else {
        &log($tr{'Outgoing rule updated'});
        $infomessage .= "$tr{'Outgoing rule updated'}<br />\n";
      }

      my $success = &message('setoutgoing');
    
      unless (defined $success) { 
        $errormessage .= "$tr{'smoothd failure'}<BR />\n";
      }
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

  # Action: add timeframe
  if ( $cgiparams{'ACTION'} eq $tr{'tofc-schedule'} )
  {
    $infomessage .= "add timeframe clicked<br />\n";

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

  # Action: edit/remove-exception/remove-timeframe
  if ( $cgiparams{'ACTION'} eq $tr{'edit'} or
       $cgiparams{'ACTION'} eq $tr{'tofc-remove-exc'} or
       $cgiparams{'ACTION'} eq $tr{'tofc-remove-tf'})
  {
    $infomessage .= "edit/remove clicked<br />\n";
    open(FILE, "$config") or die 'Unable to open config file.';
    my @current = <FILE>;
    close(FILE);

    my $count = 0;
    my $id = 0;
    my $line;

    # Count the # of entries checked for changing
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

        if ($cgiparams{$id} ne "on") {
          # This is not the 'droid we are looking for.
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
          # Only if changing the 'droid
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
          } elsif ($temp[4] eq "Any") {
            $cgiparams{'PROTOCOL'} = "Any";
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
          # Only if stripping the 'droid's day timer
          chomp $line;
          @temp = split /,/, $line;
          $temp[7] = $count;
          print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$temp[7],$temp[8],off\n";
          $count++;
        }
        # The unspoken else: don't save the 'droid.
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

      my $success = &message('setoutgoing');

      unless (defined $success) {
        $errormessage .= "$tr{'smoothd failure'}<BR />\n"; }
    }
  }

  # Action: toggle enabled/disabled
  if ( $cgiparams{'ACTION'} eq $tr{'tofc-enabledc'} ) 
  {
    $infomessage .= "toggle enabled clicked<br />\n";
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

  # Action: toggle accept/reject
  if ( $cgiparams{'ACTION'} eq $tr{'tofc-change action'} ) 
  {
    $infomessage .= "toggle allow/deny clicked<br />\n";
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
        $temp[10] =~ s/ACCEPT/REJECT/g;
        print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$action,$temp[7],$temp[8],$temp[9],$temp[10]\n";
      } elsif ($temp[6] eq "REJECT") {
        $action = "ACCEPT";
        $temp[10] =~ s/REJECT/ACCEPT/g;
        print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$action,$temp[7],$temp[8],$temp[9],$temp[10]\n";
      } 
    }
    close FILE;

    my $success = message('setoutgoing');

    unless (defined $success) {
      $errormessage .= "$tr{'smoothd failure'}<BR />\n"; }
  }

  #
  # Finished with action handling
}

# Start HTML generation
#
$infomessage .= "generating HTML<br />\n";

&showhttpheaders();

# If it exists, pull the error log into $errormessage and delete the file.
if (-f "$moddir/configErrors.log") {
  open (ceHdl, "<$moddir/configErrors.log");
  while (<ceHdl>) {
    if (! /^  /) {
      if ($errormessage ne "") {
        $errormessage .= "</p>\n";
      }
      $errormessage .= "<p style='margin-left:18pt;text-indent:-18pt;font-size:9pt'>$_";
    } else {
      s/^  //;
      $errormessage .= "<br>$_";
    }
  }
  unlink "$moddir/configErrors.log";
}


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

if ($debuggerer == 1 and $infomessage ne "") {
  print qq!
<div style="margin:4pt; color:black; background-color:#f0fff0; border:1px solid black">
  <p style="margin:4pt"><b>Informational messages</b></p>
  <div style="margin:0pt 12pt 12pt 12pt">
    $infomessage
  </div>
</div>
!;
}

&alertbox("$errormessage");

my $unused = 6;
my $ifcolor;
my $dispcolor;
my $width = 90 / $unused;

&openbox($boxtext);

print qq!
<form method='post'>
<table style='width:100%;' style='margin:6pt 0'>
  <tr>
    <td class='base' style='width:30%;'>$tr{'tofc-interface'}</td>
    <td style='width: 20%;'>
      <select style='color: green' onchange="ffoxSelectUpdate(this);" name='INTERFACE'>
!;

foreach my $interface (keys %interfaces) {
  my $optSel;
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

  print "        <option style=$ifcolor value='$interface' $selected{'INTERFACE'}{$interface}>$interface</option>\n"; 
}

print qq"
      </select>
    </td>
    <td class='base' style='width:30%'><img src='/ui/img/blob.gif'>$tr{'tofc-ipmac blank'}</td>
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
";
 
  $optSel = ($cgiparams{'PROTOCOL'} eq 'TCP') ? " selected" : "";
  print "               <option value='TCP'$optSel>$availableprotocols{6}</option>";

  $optSel = ($cgiparams{'PROTOCOL'} eq 'UDP') ? " selected" : "";
  print "               <option value='UDP'$optSel>$availableprotocols{17}</option>";

  $optSel = ($cgiparams{'PROTOCOL'} eq 'TCP|UDP') ? " selected" : "";
  print "               <option value='Both'$optSel>TCP|UDP</option>";

  $optSel = ($cgiparams{'PROTOCOL'} eq 'PPTP') ? " selected" : "";
  print "               <option value='PPTP'$optSel>PPTP-Passthru</option>";

  $optSel = ($cgiparams{'PROTOCOL'} eq 'IPSEC') ? " selected" : "";
  print "               <option value='IPSEC'$optSel>IPSEC</option>";

  $optSel = ($cgiparams{'PROTOCOL'} eq 'Any') ? " selected" : "";
  print "               <option value='Any'$optSel>Any</option>";



print qq"
            </select>
          </td>
          <td class='base'>$tr{'ffc-targetc'}</td>
          <td>
            <select name='TARGET'>
              <option value='ACCEPT' $selected{'TARGET'}{'ACCEPT'}>$tr{'target accept'}</option>
              <option value='REJECT' $selected{'TARGET'}{'REJECT'}>$tr{'target reject'}</option>
            </select>
          </td>
          <td class='base'>$tr{'order numberc'}</td>
          <td>
            <select name='ORDER_NUMBER'>
";

  for ($cnt = 1; $cnt < $cgiparams{'RULE_COUNT'} + 1; $cnt++) 
  {
    $optSel = ($cnt eq $cgiparams{'ORDER_NUMBER'}) ? " selected" : "";
    print "              <option value='$cnt'$optSel>$cnt</option>\n";
  }

print qq!
            </select>
          </td>
        </tr>
        <tr>
          <td class='base'>$tr{'commentc'}</td>
          <td colspan='5'><input type='text' size='80' name='COMMENT' value='$cgiparams{'COMMENT'}' ></td>
        </tr>
      </table>
    </td>
  </tr>
</table>

<table width='100%'>
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

<p class='base' style='margin-top:8pt'>
  <img src='/ui/img/blob.gif'> $tr{'tofc-blank all'}
</p>
!;

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
      valign => 'top',
      maxrowspan => 3,
      sort   => '<=>',
    },
    { 
      column => '1',
      title  => $tr{'interfacenc'},
      tr     => { 'GREEN' => '<span style="color:green"><b>GREEN</b></span>',
                  'ORANGE' => '<span style="color:orange"><b>ORANGE</b></span>',
                  'PURPLE' => '<span style="color:purple"><b>PURPLE</b></span>'
                },
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
      spanadj => -1,
    },
    {
      column => '11',
      title => "Time frames",
      break => 'line',
      spanadj => -1,
    }
  ]
);

&dispaliastab($config, \%render_settings, $cgiparams{'ORDER'}, $cgiparams{'COLUMN'} );

print qq!
<p class='base' style='margin:0'>
  <br>
</p>

<table class='blank'>
  <tr>
    <td style='width: 15%; text-align: center;'>
      <input type='submit' name='ACTION' value='$tr{'tofc-enabledc'}'>
    </td>
    <td style='width: 20%; text-align: center;'>
      <input type='submit' name='ACTION' value='$tr{'tofc-change action'}'>
    </td>
    <td style='width: 25%; text-align: center;'>
      <input type='submit' name='ACTION' value='$tr{'tofc-remove-exc'}'
             onClick="if(confirm('You are about to completely remove exceptions.
                                  Are you sure you want to do this?')) {return true;} return false;">
    </td>
    <td style='width: 25%; text-align: center;'>
      <input type='submit' name='ACTION' value='$tr{'tofc-remove-tf'}'>
    </td>
    <td style='width: 15%; text-align: center;'>
      <input type='submit' name='ACTION' value='$tr{'edit'}'>
    </td>
  </tr>
</table>
!;
&closebox();
&openbox($tr{'timed accessc'});

print qq!
<div align='center'>
  <p class='base' style='margin:0 0 8pt 0'>
    <img src='/ui/img/blob.gif'>
    <i>Select the rule(s) you want to add<br>
    &nbsp;&nbsp;time frames to before setting the time frames.</i>
  </p>
  <table width='50%' align='center'>
    <tr>
      <td class='base' width='15%'>$tr{'fromc'}</td>
      <td width='35%'>
        <select name='START_HOUR'>
!;

foreach (@hour) {
  print "        <option value='$_' $selected{'START_HOUR'}{$_}>$_</option>\n";
}

print qq!
        </select>
        :
        <select name='START_MINUTE'>
!;

foreach (@minute) {
  print "        <option value='$_' $selected{'START_MINUTE'}{$_}>$_</option>\n";
}

print qq!
        </select>
      </td>
      <td class='base' width='15%'>$tr{'toc'}</td>
      <td width='35%'>
        <select name='END_HOUR'>
!;

foreach (@hour) {
  print "        <option value='$_' $selected{'END_HOUR'}{$_}>$_</option>\n";
}

print qq!
        </select>
        :
        <select name='END_MINUTE'>
!;

foreach (@minute) {
  print "        <option value='$_' $selected{'END_MINUTE'}{$_}>$_</option>\n";
}

print qq!
        </select>
      </td>
    </tr>
  </table>
</div>

<div align='center' id='checkBoxes'>
  <table width='50%'>
    <tr>
!;

for (my $day = 0; $day < 10; $day++)
{
  if ($day == 7)
  {
    print qq!
    </tr>
  </table>
  <table width='40%' align='center'>
    <tr>
      <td class='base' width='33%'>$tr{"tofc-day $day"}:</td>
      <td>
        <input type='radio' name='DAY_$day' $checked{"DAY_${day}"}{'on'}
               onClick='javaScript:checkWkdays(this.form);' >
      </td>
!;
  }
  elsif ($day == 8)
  {
    print qq!
      <td class='base' width='33%'>$tr{"tofc-day $day"}:</td>
      <td>
        <input type='radio' name='DAY_$day' $checked{"DAY_${day}"}{'on'}
               onClick='javaScript:checkWkends(this.form);'>
      </td>
!;
  }
  elsif ($day == 9)
  {
    print qq!
      <td class='base' width='34%'>$tr{"tofc-day $day"}:</td>
      <td>
        <input type='radio' name='DAY_$day' $checked{"DAY_${day}"}{'on'}
               onClick='javaScript:checkAlldays(this.form);'>
      </td>
!;
  }
  else
  {
    print qq!
      <td class='base'>$tr{"tofc-day $day"}:</td>
      <td>
        <input type='checkbox' name='DAY_$day' $checked{"DAY_${day}"}{'on'}>
      </td>
!;

  }
}

print qq!
    </tr>
  </table>
</div>
<div align='center'>
  <table width='100%'>
    <tr>
      <td style='text-align: center;'>
        <input type='submit' name='ACTION' value='$tr{'tofc-schedule'}'>
      </td>
    </tr>
  </table>
</div>
!;

&closebox();

&alertbox('add','add');

&closebigbox();

&closepage();
