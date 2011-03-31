#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# Updated: 4/14/2004 - Drew S. Dupont
#  - Enhanced adding, editing, and deleting of rules
# Updated: 4/20/2004 - Drew S. Dupont v.0.2
#  - Added display of port services
# Updated: 4/21/2004 - Drew S. Dupont v.0.3
#  - Added display of host name resolutuions (if available)
# Updated: 4/21/2004 - Drew S. Dupont v.0.4
#  - Added ability to sort on different fields
# Updated: 6/6/2004 - Drew S. Dupont v.0.5
#  - Added true outbound firewall support
#    + Changed config file format to include target support
# Updated: 8/26/2004 - Drew S. Dupont v.1.0
#  - Added true outbound firewall support
#    + Changed config file format to include target, source 
#	interface, destination interface, and order support
#    + Added targets for rules (Allow, Reject, Drop, Log)
#    + Improved GUI for display and add/update/edit
#    + Added ability to reorder some rules
#    + Added some default rules which can not be changed or reordered
#    + Added some error checking for incorrect and stupid rules
# Updated: 10/01/2004 - Drew S. Dupont v.1.0.1
#  - Fixed display or interfaces
# Updated: 10/01/2004 - Drew S. Dupont v.1.0.2
#  - Fixed error checking
# Updated: 10/10/2004 - Drew S. Dupont v.1.1
#  - Changed from true port forwarding to port mapping
# Updated: 10/10/2004 - Drew S. Dupont v.1.1.1
#  - Enhanced support for port mapping in/out of any interface
# Updated: 10/10/2004 - Drew S. Dupont v.1.1.2
#  - Fixed issue with empty/blank interface when not configured in setup for missing object in comparison
# Updated: 10/12/2004 - Drew S. Dupont v.1.1.3
#  - Fixed issue with MAC addresses and added error check for dest. MAC address
#  - Removed error check for empty/Any destination IP when ports are entered
# Updated: 10/13/2004 - Drew S. Dupont v.1.1.4
#  - Fixed issue of dial-up users not beaing able to view the GUI when not connected
#  - Fixed issue when clicking on check/cross the description would be lost
#  - Fixed issue of renumbering not working correctly on add/update/delete
# Updated: 10/16/2004 - Drew S. Dupont v.1.1.5
#  - Fixed issue with users not being able to set rules correctly in GUI with different interface configurations 
# Updated: 11/04/2004 - Drew S. Dupont v.1.2
#  - Fixed incorrect destination field label
#  - Fixed incorrect mode select box display
#  - Added support for multiple IP's on an interface (which translates into multiple interfaces in the GUI)
#  - Enhanced display of interfaces when having virtual adapters for multiple IP support
#  - Display interface IP in rules when you hover over the interface
# Updated: 02/21/2005 - Drew S. Dupont v.1.2.1
#  - Fixed a few display issues
# Updated 02/24/2008 - Stanford Prescott MD v3.0-0.9 and v3.0-0.9.1 beta versions
#  - Ported Drew S. Dupont's (NetWhiz) Full Firewall Control mod and Toby Long-Leather's (TobyLL) Multiple IPs mod to SmoothWall
#	3.0 and combined the two into one mod
#  - Converted NetWhiz's setportfw setuid wrapper to a SmoothD module. Added code to bring down and bring up new interface aliases
#	to the new sysportfw.so smoothd module and removed the code from the rc.netaddress.down and rc.netaddress.up scripts
#  - Adding or removing aliases no longer requires a network restart. The individual aliases are brought up and down as needed
#  - Revamped how the GUI handles adding, editing and removing interface aliases
#  - When an alias is removed any associated portfw rules with that alias are also removed
#  - Added ability to enable/disable portfw rules without editing or deleting them
#  - Added "smoothtype" javascript checks to address and netmask entries for aliases
#  - Changed the aliases display table to SWE 3.0 displaytable format
#  Updated 06/19/2008
#  - Removed ability to add LAN aliases
#  - Added ability to add a LAN IP to be associated with an alias for outbound SNAT "server mapping"
#  Final release 06/28/2008
#  - sysportfw.so completely rewritten by Steve Pittman (aka MtnLion) for improved bouncing port forwards
#  - and SNAT outbound masking for 1:1 server mapping to an alias
#  First Phaeton/Roadster working 10/08/30


use lib "/usr/lib/smoothwall";

use header qw( :standard );
use smoothd qw( message );
use smoothtype qw( :standard );
use Shell qw(rm touch);
use Socket;
use NetAddr::IP;
use Net::Netmask;

my %netsettings;
&readhash("${swroot}/ethernet/settings", \%netsettings);

my $aliasfile = "${swroot}/portfw/aliases";
my @aliases;
my $settings = "${swroot}/ethernet/settings";
my @colours = ("RED", "GREEN");
if ($netsettings{'ORANGE_DEV'}) {
  push (@colours, "ORANGE");
}
if ($netsettings{'PURPLE_DEV'}) {
  push (@colours, "PURPLE");
}

my $reddev;
# Determine red interface type
if ($netsettings{'RED_TYPE'} eq "PPPOE") {
  $reddev = "ppp0";
} else {
  $reddev = $netsettings{'RED_DEV'};
}

my $redip = '';
if (-e "${swroot}/red/local-ipaddress") {
  open(FILE, "${swroot}/red/local-ipaddress") or die 'Unable to open local-ipaddress file.';
  $redip = <FILE>;
  chomp $redip;
  close(FILE);
}

my (%cgiparams,%selected,%checked);
my $filename = "${swroot}/portfw/config";

&showhttpheaders();

$cgiparams{'COLUMN_ONE'} = 1;
$cgiparams{'ORDER_ONE'} = $tr{'log ascending'};

$cgiparams{'COLUMN_TWO'} = 1;
$cgiparams{'ORDER_TWO'} = $tr{'log ascending'};

if ($ENV{'QUERY_STRING'} && 
    (not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ))
{
  my @temp = split(',',$ENV{'QUERY_STRING'});
  $cgiparams{'ORDER_ONE'}  = $temp[1] if ( defined $temp[ 1 ] and $temp[ 1 ] ne "" );
  $cgiparams{'COLUMN_ONE'} = $temp[0] if ( defined $temp[ 0 ] and $temp[ 0 ] ne "" );
  $cgiparams{'ORDER_TWO'}  = $temp[3] if ( defined $temp[ 3 ] and $temp[ 1 ] ne "" );
  $cgiparams{'COLUMN_TWO'} = $temp[2] if ( defined $temp[ 2 ] and $temp[ 0 ] ne "" );
}

my $edit_id;
my $rule_id;

$cgiparams{'OLDID'} = 0;
$cgiparams{'ENABLED'} = 'off';
$cgiparams{'ADDRESS_ENABLED'} = 'off';

$cgiparams{'RULE_COUNT'} = 0;
$cgiparams{'DESCRIPTION'} = '';
&getcgihash(\%cgiparams);

my $errormessage = '';
my $updatebutton = 0;


# Adding or updating multi-addrs
#
if (defined $cgiparams{'ACTION'} and
    ($cgiparams{'ACTION'} eq $tr{'ffc-multi add'} or
     $cgiparams{'ACTION'} eq $tr{'multiip-update'})) {
  unless(&validip($cgiparams{'ADDRESS'})) { 
    $errormessage = "$tr{'multi-ip invalid'} $tr{'ip address'}"; 
  }
  unless(&validipormask($cgiparams{'NETMASK'})) { 
    $errormessage = "$tr{'multi-ip invalid'} $tr{'multi-ip netmask'}"; 
  }
  unless ($cgiparams{'INTADDRESS'} eq '') {
    unless(&validip($cgiparams{'INTADDRESS'})) { 
      $errormessage = "$tr{'multi-ip invalid'} $tr{'ip address'}"; 
    }
  }
  unless ($errormessage) {
    my ($line, $coloralias, $ifalias, $ifreal, $enabled, $octet, $size, $block);
    my (@aliases, @temp);
    my $count = 0;

    # Calculate CIDR notation and network block size
    my $aladd = $cgiparams{'ADDRESS'};
    my $netmask = $cgiparams{'NETMASK'};
    my $intadd = $cgiparams{'INTADDRESS'};
    my $block = new2 Net::Netmask ($aladd, $netmask);
    if (defined $block)
    {
      $size = $block->size();
    }
    else
    {
      $size = 0;
      $errormessage = "$tr{'multi-ip invalid'} $tr{'multi-ip netmask'}  ($Net::Netmask::error)";
    }
    my $counta = 1;

    my @tempa;
    if (open(FILE, "$aliasfile"))
    {
      @tempa = <FILE>;
      close FILE;
    }
    
    foreach my $linea (@tempa) {
      if ($linea =~ /^RED:/) {
        $counta++;
        $size++;
      }
    }

    if ($netmask eq '255.255.255.255') {
      $block = "$cgiparams{'ADDRESS'}\/32";
    }
    my @ipsplit = split(/\./, $cgiparams{'ADDRESS'});
    my $lastnum = $ipsplit[3] - 1;
    my $broadcast = "$ipsplit[0]\.$ipsplit[1]\.$ipsplit[2]\.255";
    if (open(FILE, "$aliasfile"))
    {
      @aliases = <FILE>;
      close FILE;
    }

    # Check for update action first
    my $noupdate = 1;
    if ($cgiparams{'ACTION'} eq $tr{'multiip-update'}) {
      open(FILE, ">$aliasfile") or die 'Unable to open aliases file';
      foreach $line (@aliases) {
        chomp $line;
        @split = split(/\,/, $line);
        unless ($cgiparams{'OLDALIAS'} eq $split[0]) {
          print FILE "$line\n";
        } else {
          print FILE "$split[0],$split[1],$split[2],$aladd,$netmask,";
          print FILE "$split[5],$split[6],$split[7],$cgiparams{'COMMENT'},$intadd\n";
        }
      }
      close FILE;

      $noupdate = 0;
    }

    if ($noupdate) {
      open(FILE, ">>$aliasfile") or die 'Unable to open';
      flock FILE, 2;
      for (my $cnt4 = $counta; $cnt4 <= $size; $cnt4++) {
        $coloralias = "RED:$cnt4" ;
        $ifalias = "$reddev:$cnt4";
        $ifreal = "$reddev";
        $lastnum++;
        unless ($lastnum > 254) {
        $octet = "$ipsplit[0]\.$ipsplit[1]\.$ipsplit[2]\.$lastnum";
        } else {
          last;
        }
        $enabled = "$cgiparams{'ADDRESS_ENABLED'}";
        print FILE "$coloralias,$ifalias,$ifreal,$octet,255\.255\.255\.0,";
        print FILE "$broadcast,yes,$enabled,$cgiparams{'COMMENT'},$intadd\n";
      }
      close FILE;
    }

    my $success = &message('ifaliasup');
  
    unless (defined $success) {
      $errormessage = $tr{'smoothd failure'}; }

    &log($tr{'multi-ip ip address added or updated'});

    my $success = &message('setportfw');
  
    unless (defined $success) {
      $errormessage = $tr{'smoothd failure'};
    }
    unless ($success eq 'Port forwarding rules set') {
      $errormessage = "Error setting portforwarding rules: " . "$success";
    }
  }
EXIT:
  $cgiparams{'ADDRESS'} = '';
  $cgiparams{'NETMASK'} = '';
  $cgiparams{'OLDALIAS'} = '';
  $cgiparams{'SRC_IPMAC'} = '';
  $cgiparams{'DEST_IPMAC'} = '';
  $cgiparams{'SRC_IFACE'} = $reddev;
  $cgiparams{'DEST_IFACE'} = $netsettings{'GREEN_DEV'};
  $cgiparams{'DEST_PORT'} = '';
  $cgiparams{'NEW_DEST_PORT'} = '';
  $cgiparams{'PROTOCOL'} = '6';
  $cgiparams{'TARGET'} = 'ACCEPT';
  $cgiparams{'ENABLED'} = 'on';
  $cgiparams{'DESCRIPTION'} = '';
  $cgiparams{'COMMENT'} = '';
  $cgiparams{'INTADDRESS'} = '';
}


# Removing a multi-addr
#
if (defined $cgiparams{'ACTION'} and 
    ($cgiparams{'ACTION'} eq $tr{'multi-ip remove'} or 
     $cgiparams{'ACTION'} eq $tr{'multi-ip edit'} or
     $cgiparams{'ACTION'} eq $tr{'multi-ip enable alias'})) {
  my ($count, $count2, $id, $line, $line2, @split);

  my @current;
  if (open(FILE, "$aliasfile"))
  {
    @current = <FILE>;
    close FILE;
  }
  
  $count = 0;
  $id = 0;
  foreach $line (@current)
  {
    $id++;
    if ($cgiparams{$id} eq "on") {
      $count++;
      @split = split(/\,/, $line);
      if (($split[0] eq "RED") or ($split[0] eq "GREEN") or
          ($split[0] eq "ORANGE") or ($split[0] eq "PURPLE")) {
        $errormessage = $tr{'multi-ip iface error'};
      }
    }
  }

  if ($count == 0) {
    $errormessage = $tr{'nothing selected'};  }
  if ($count > 1 && $cgiparams{'ACTION'} eq $tr{'edit'}) {
    $errormessage = $tr{'you can only select one item to edit'};  }

  unless ($errormessage)
  {
    # Bring down the aliases first
    my $success = &message('ifaliasdown');
  
    unless (defined $success) {
      $errormessage = 'Unable to bring down interface aliases!'; }

    $id = 0;
    my $count2 = 0;
    my $pfwflag = 0;

    open(FILE, ">$aliasfile") or die 'Unable to open aliases file.';
    flock FILE,2;
    foreach $line (@current)
    {
      $id++;
      chomp($line);
      my @temp = split(/\,/, $line);
      if ($cgiparams{$id} ne "on")
      {
        print FILE "$line\n";
      }
      elsif ($cgiparams{'ACTION'} eq $tr{'multi-ip edit'})
      {
        $cgiparams{'OLDALIAS'} = $temp[0];
        $cgiparams{'ADDRESS'} = $temp[3];
        $cgiparams{'NETMASK'} = '255.255.255.255';
        $cgiparams{'ADDRESS_ENABLED'} = $temp[6];
        $cgiparams{'COMMENT'} = $temp[8];
        $cgiparams{'INTADDRESS'} = $temp[9];

        # Editing support
        $aliasupdbutton = 1;
        print FILE "$line\n";

        # Disable any forwarding rules associated with the edited interface alias
        open(TEMP, "$filename") or die 'Unable to open portfw config file';
        my @temp2 = <TEMP>;
        close TEMP;

        open(TEMP, ">$filename") or die 'Unable to open config file';
        foreach $line2 (@temp2) {
          $count2++;
          chomp $line2;
          @split = split(/\,/, $line2);
          unless (($temp[1] eq $split[1]) or ($temp[1] eq $split[4])) {
            print TEMP "$count2,$split[1],$split[2],$split[3],$split[4],$split[5],";
            print TEMP "$split[6],$split[7],$split[8],$split[9],$split[10]\n";
          } else {
            $pfwflag++;
            print TEMP "$count2,$split[1],$split[2],$split[3],$split[4],$split[5],";
            print TEMP "$split[6],$split[7],$split[8],off,$split[10]\n";
          }
        }
        close TEMP;
      } elsif ($cgiparams{'ACTION'} eq $tr{'multi-ip remove'}) {
        # Remove any forwarding rules associated with the deleted interface alias
        open(TEMP, "$filename") or die 'Unable to open portfw config file';
        my @temp2 = <TEMP>;
        close TEMP;

        open(TEMP, ">$filename") or die 'Unable to open config file';
        foreach $line2 (@temp2) {
          $count2++;
          chomp $line2;
          @split = split(/\,/, $line2);
          unless (($temp[1] eq $split[1]) or ($temp[1] eq $split[4])) {
            print TEMP "$count2,$split[1],$split[2],$split[3],$split[4],$split[5],";
            print TEMP "$split[6],$split[7],$split[8],$split[9],$split[10]\n";
          } else {
            $pfwflag++;
          }
        }
        close TEMP;
      } elsif ($cgiparams{'ACTION'} eq $tr{'multi-ip enable alias'}) {
        if ($temp[7] eq "on") {
          $enabled = "off";
          my $running = "off";
        } else {
          $enabled = "on";
          my $running = "on";
        }
        print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],";
        print FILE "$temp[5],$running,$enabled,$temp[8],$temp[9]\n";
      }
    }
    close FILE;

    # Reorder the aliases
    my @current;
    if (open(FILE, "$aliasfile"))
    {
      flock FILE, 2;
      @current = <FILE>;
      close FILE;
    }

    open(FILE, ">$aliasfile") or die 'Unable to open aliases file.';
    flock FILE, 2;
    $count = 0;
    foreach $line (@current) {
      chomp $line;
      @split = split(/\,/, $line);
      my $alias = $split[1];
      my $reth = $split[2];
      my $raddress = $split[3];
      my $rnet = $split[4];
      my $rbroad = $split[5];
      my $rrunning = $split[6];
      my $renabled = $split[7];
      if ($split[0] =~ /^RED:/) {
        $count++;
        print FILE "RED:$count,$reddev:$count,$reth,$raddress,$rnet,";
        print FILE "$rbroad,$rrunning,$renabled,$split[8],$split[9]\n";

        # Check for alias in portfw config file
        my $pfalias = "$reddev:$count";
        open(TEMP, "$filename") or die 'Unable to open';
        flock TEMP, 2;
        @temp = <TEMP>;
        close TEMP;

        open(TEMP, ">$filename") or die 'Unable to open';
        flock TEMP, 2;
        foreach $line2 (@temp) {
          chomp $line2;
          my @current2 = split(/\,/, $line2);
          my $temp0 = $current2[0];
          my $temp1 = $current2[1];
          my $temp2 = $current2[2];
          my $temp3 = $current2[3];
          my $temp4 = $current2[4];
          my $temp5 = $current2[5];
          my $temp6 = $current2[6];
          my $temp7 = $current2[7];
          my $temp8 = $current2[8];
          my $temp9 = $current2[9];
          my $temp10 = $current2[10];

          if ($alias eq $temp1) {
            $pfwflag++;
            print TEMP "$temp0,$pfalias,$temp2,$temp3,$temp4,$temp5,";
            print TEMP "$temp6,$temp7,$temp8,$temp9,$temp10\n";
          } elsif ($alias eq $temp4) {
            $pfwflag++;
            print TEMP "$temp0,$temp1,$temp2,$temp3,$pfalias,$temp5,";
            print TEMP "$temp6,$temp7,$temp8,$temp9,$temp10\n";
          } else {
            print TEMP "$line2\n";
          }
        }
        close TEMP;
      } else {
        print FILE "$line\n";
      }
    }
    close FILE;

    my $success = &message('ifaliasup');
  
    unless (defined $success) {
      $errormessage = 'Unable to bring up interface alias!'; }

    my $success = &message('setportfw');
  
    unless (defined $success) {
      $errormessage = $tr{'smoothd failure'}; }

    unless ($success eq 'Port forwarding rules set') {
      $errormessage = "Error setting portforwarding rules: " . "$success"; }
  }

  # Cheat to reorder port forwarding rules since I'm too lazy to figure
  # out why it's not ordering them correctly
  my $count1 = 0;
  open (FILE, "$filename") or die 'Unable to open port forward config file';
  @temp = <FILE>;
  close FILE;

  open (FILE, ">$filename") or die 'Unable to open config file';
  flock FILE, 2;
  foreach $line (@temp) {
    $count1++;
    chomp $line;
    @split = split(/\,/, $line);
    print FILE "$count1,$split[1],$split[2],$split[3],$split[4],$split[5],";
    print FILE "$split[6],$split[7],$split[8],$split[9],$split[10]\n";
  }
  close FILE;
}


# Check for add or update action
#
if ($cgiparams{'ACTION'} eq $tr{'add'} or
    $cgiparams{'ACTION'} eq $tr{'ffc-update'}) {
  my $ipmac = $cgiparams{'SRC_IPMAC'};
  if ($ipmac =~ /^\!/) {
    $ipmac =~ s/^\!//;
  }
  unless(&validipormask($ipmac) || &validmac($ipmac)) {
    if ($ipmac ne '') {
      $errormessage .= "$tr{'source ip bad'}<BR />";
    } else {
      $cgiparams{'SRC_IPMAC'} = '0.0.0.0/0';
    }
  }

  if ($cgiparams{'DEST_PORT'}) {
    unless(&validportrange($cgiparams{'DEST_PORT'})) {
      $errormessage .= "$tr{'source port numbers'}<BR />";
    }
  } else {
    $cgiparams{'DEST_PORT'} = 0;
  }

  if ($cgiparams{'NEW_DEST_PORT'}) {
    unless(&validportrange($cgiparams{'NEW_DEST_PORT'})) {
      $errormessage .= "$tr{'destination port numbers'}<BR />";
    }
  } else {
    $cgiparams{'NEW_DEST_PORT'} = 0;
  }

  unless(&validipormask($cgiparams{'DEST_IPMAC'})) {
    if ($cgiparams{'DEST_IPMAC'} ne '') {
      $errormessage .= "$tr{'destination ip bad'}<BR />";
    } else {
      $cgiparams{'DEST_IPMAC'} = '0.0.0.0/0';
    }
  }

  #################
  # Error checking
  #################
  unless ($cgiparams{'PROTOCOL'} eq "6" or $cgiparams{'PROTOCOL'} eq "17" or
          $cgiparams{'PROTOCOL'} eq "TCP & UDP" ) {
    unless ( !($cgiparams{'DEST_PORT'}) and !($cgiparams{'NEW_DEST_PORT'}) ) {
      $errormessage = 'You cannot specify a port with that protocol';
    }
  }

  # Check for source/destination port being defined
  if (($cgiparams{'DEST_PORT'} != 0) || ($cgiparams{'NEW_DEST_PORT'} != 0)) {
    # Check for protocol being Any
    if ($cgiparams{'PROTOCOL'} eq 'Any') {
      $errormessage .= "$tr{'error source-destination port protocol any'}<BR />";
    }

    # Check for dest. IP/MAC being empty
    if ($cgiparams{'DEST_IPMAC'} eq '') {
      $errormessage .= "$tr{'error source-destination port destination any'}<BR />";
    }
  }

  # Check for source and destination ports being equal
  if (($cgiparams{'DEST_PORT'} != 0) and ($cgiparams{'NEW_DEST_PORT'} != 0)) {
    if ($cgiparams{'DEST_PORT'} eq $cgiparams{'NEW_DEST_PORT'}) {
      $cgiparams{'NEW_DEST_PORT'} = 0;
    }
  }

  # Check for source port being defined if destination port is defined-error if not
  if (($cgiparams{'DEST_PORT'} == 0) and ($cgiparams{'NEW_DEST_PORT'} != 0)) {
    $errormessage = 'Source port must be defined if destination port is also defined!';
  }

  open(FILE, $filename) or die 'Unable to open config file.';
  my @current = <FILE>;
  close(FILE);
  my $line;

  #########################################
  # Check for IP's in subnets
  #########################################
  my $defaultipobj = new NetAddr::IP "default";
  my $localhostipobj = new NetAddr::IP "localhost";
  my $greenipobj = new NetAddr::IP "$netsettings{'GREEN_ADDRESS'}/$netsettings{'GREEN_NETMASK'}";
  my $greengwobj = new NetAddr::IP "$netsettings{'GREEN_ADDRESS'}";
  my $redgwobj = $defaultipobj;
  my $orangeipobj = $defaultipobj;
  my $orangegwobj = $defaultipobj;
  my $purpleipobj = $defaultipobj;
  my $purplegwobj = $defaultipobj;
  my $srcipaddrobj = "";
  my $destipaddrobj = "";

  if ($redip ne '') {
    $redgwobj = new NetAddr::IP "$redip";
  } else {
    $redgwobj = $defaultipobj;
  }

  if ($netsettings{'ORANGE_ADDRESS'} ne '') {
    $orangeipobj = new NetAddr::IP "$netsettings{'ORANGE_ADDRESS'}/$netsettings{'ORANGE_NETMASK'}";
    $orangegwobj = new NetAddr::IP "$netsettings{'ORANGE_ADDRESS'}";
  }

  if ($netsettings{'PURPLE_ADDRESS'} ne '') {
    $purpleipobj = new NetAddr::IP "$netsettings{'PURPLE_ADDRESS'}/$netsettings{'PURPLE_NETMASK'}";
    $purplegwobj = new NetAddr::IP "$netsettings{'PURPLE_ADDRESS'}";
  }

  if (&validip($ipmac)) {
    $srcipaddrobj = new NetAddr::IP "$ipmac";

    if ($localhostipobj eq $srcipaddrobj) {
      $errormessage .= "$tr{'source ip bad local'}<BR />";
    }

    if ($greengwobj eq $srcipaddrobj) {
      $errormessage .= "$tr{'source ip bad gateway'}<BR />";
    } elsif (!($srcipaddrobj eq "")) {
      if (($orangegwobj eq $srcipaddrobj) ||
          ($purplegwobj eq $srcipaddrobj) ||
          ($redgwobj eq $srcipaddrobj)) {
        $errormessage .= "$tr{'source ip bad gateway'}<BR />";
      }
    }

    if (!($srcipaddrobj == $defaultipobj)) {
      if ($cgiparams{'SRC_IFACE'} eq $netsettings{'GREEN_DEV'}) {
        if (!$greenipobj->contains($srcipaddrobj)) {
          $errormessage .= "$tr{'source ip bad green'}<BR />";
        }
      } elsif (($netsettings{'ORANGE_ADDRESS'} ne '') &&
               ($cgiparams{'SRC_IFACE'} eq $netsettings{'ORANGE_DEV'})) {
        if (!$orangeipobj->contains($srcipaddrobj)) {
          $errormessage .= "$tr{'source ip bad orange'}<BR />";
        }
      } elsif (($netsettings{'PURPLE_ADDRESS'} ne '') &&
               ($cgiparams{'SRC_IFACE'} eq $netsettings{'PURPLE_DEV'})) {
        if (!$purpleipobj->contains($srcipaddrobj)) {
          $errormessage .= "$tr{'source ip bad purple'}<BR />";
        }
      } elsif (($redip ne '') && ($cgiparams{'SRC_IFACE'} eq $reddev)) {
        if (($greenipobj->contains($srcipaddrobj)) ||
            (($orangeipobj != $defaultipobj) &&
             ($orangeipobj->contains($srcipaddrobj))) ||
            (($purpleipobj != $defaultipobj) &&
             ($purpleipobj->contains($srcipaddrobj)))) {
          $errormessage .= "$tr{'source ip bad red'}<BR />";
        }
      }
    }
  }

  if (&validip($cgiparams{'DEST_IPMAC'})) {
    $destipaddrobj = new NetAddr::IP "$cgiparams{'DEST_IPMAC'}";

    if ($localhostipobj eq $destipaddrobj) {
      $errormessage .= "$tr{'destination ip bad local'}<BR />";
    }

    if ($greengwobj eq $destipaddrobj) {
      $errormessage .= "$tr{'destination ip bad gateway'}<BR />";
    } elsif (!($destipaddrobj eq "")) {
      if (($orangegwobj eq $destipaddrobj) ||
          ($purplegwobj eq $destipaddrobj) ||
          ($redgwobj eq $destipaddrobj)) {
        $errormessage .= "$tr{'destination ip bad gateway'}<BR />";
      }
    }

    if (!($destipaddrobj == $defaultipobj)) {
      if ($cgiparams{'DEST_IFACE'} eq $netsettings{'GREEN_DEV'}) {
        if (!$greenipobj->contains($destipaddrobj)) {
          $errormessage .= "$tr{'destination ip bad green'}<BR />";
        }
      } elsif (($netsettings{'ORANGE_ADDRESS'} ne '') &&
               ($cgiparams{'DEST_IFACE'} eq $netsettings{'ORANGE_DEV'})) {
        if (!$orangeipobj->contains($destipaddrobj)) {
          $errormessage .= "$tr{'destination ip bad orange'}<BR />";
        }
      } elsif (($netsettings{'PURPLE_ADDRESS'} ne '') &&
               ($cgiparams{'DEST_IFACE'} eq $netsettings{'PURPLE_DEV'})) {
        if (!$purpleipobj->contains($destipaddrobj)) {
          $errormessage .= "$tr{'destination ip bad purple'}<BR />";
        }
      } elsif (($redip ne '') && ($cgiparams{'DEST_IFACE'} eq $reddev)) {
        if (($greenipobj->contains($destipaddrobj)) ||
            (($orangeipobj != $defaultipobj) &&
             $orangeipobj->contains($destipaddrobj)) ||
            (($purpleipobj != $defaultipobj) &&
             $purpleipobj->contains($destipaddrobj))) {
          $errormessage .= "$tr{'destination ip bad red'}<BR />";
        }
      }
    }
  }

  unless ($errormessage) {
    ######################################
    # Mod for new edit/update procedure
    ######################################
    if ($cgiparams{'ACTION'} eq $tr{'ffc-update'}) {
      open(FILE, "$filename") or die 'Unable to open config file.';
      my @current = <FILE>;
      close(FILE);

      my $line;
      my $cnt = 0;
      open(FILE, ">$filename") or die 'Unable to open config file.';
      flock FILE, 2;

      foreach $line (@current) {
        $cnt++;

        if ($cnt >= $cgiparams{'OLDID'}) {
          if ($cnt != $cgiparams{'OLDID'}) {
            chomp $line;
            my @temp = split(/\,/, $line);
            $temp[0]--;
            print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],";
            print FILE "$temp[6],$temp[7],$temp[8],$temp[9],$temp[10]\n";
          }
        } else {
          print FILE "$line";
        }
      }

      close(FILE);
    }

    open(FILE, "$filename") or die 'Unable to open config file.';
    my @current = <FILE>;
    close(FILE);

    my $line;
    my $notadded = 1;
    my $cnt = 0;
    open(FILE, ">$filename") or die 'Unable to open config file.';
    flock FILE, 2;

    foreach $line (@current) {
      $cnt++;

      if ($cnt == $cgiparams{'ORDER_NUMBER'}) {
        if ($cgiparams{'PROTOCOL'} eq "TCP & UDP") {
          print FILE "$cgiparams{'ORDER_NUMBER'},$cgiparams{'SRC_IFACE'},";
          print FILE "$cgiparams{'SRC_IPMAC'},$cgiparams{'DEST_PORT'},";
          print FILE "$cgiparams{'DEST_IFACE'},$cgiparams{'DEST_IPMAC'},";
          print FILE "$cgiparams{'NEW_DEST_PORT'},6,$cgiparams{'TARGET'},";
          print FILE "$cgiparams{'ENABLED'},$cgiparams{'DESCRIPTION'}\n";
          $cnt++;
          print FILE "$cnt,$cgiparams{'SRC_IFACE'},$cgiparams{'SRC_IPMAC'},";
          print FILE "$cgiparams{'DEST_PORT'},$cgiparams{'DEST_IFACE'},";
          print FILE "$cgiparams{'DEST_IPMAC'},$cgiparams{'NEW_DEST_PORT'},17,";
          print FILE "$cgiparams{'TARGET'},$cgiparams{'ENABLED'},";
          print FILE "$cgiparams{'DESCRIPTION'}\n";
          $notadded = 0;
          $cnt++;
        } else {
          print FILE "$cgiparams{'ORDER_NUMBER'},$cgiparams{'SRC_IFACE'},";
          print FILE "$cgiparams{'SRC_IPMAC'},$cgiparams{'DEST_PORT'},";
          print FILE "$cgiparams{'DEST_IFACE'},$cgiparams{'DEST_IPMAC'},";
          print FILE "$cgiparams{'NEW_DEST_PORT'},$cgiparams{'PROTOCOL'},";
          print FILE "$cgiparams{'TARGET'},$cgiparams{'ENABLED'},";
          print FILE "$cgiparams{'DESCRIPTION'}\n";
          $notadded = 0;
          $cnt++;
        }
      }

      chomp $line;
      my @temp = split(/\,/, $line);
      print FILE "$cnt,$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],";
      print FILE "$temp[6],$temp[7],$temp[8],$temp[9],$temp[10]\n";
    }

    if ($notadded) {
      if ($cgiparams{'PROTOCOL'} eq "TCP & UDP") {
        print FILE "$cgiparams{'ORDER_NUMBER'},$cgiparams{'SRC_IFACE'},";
        print FILE "$cgiparams{'SRC_IPMAC'},$cgiparams{'DEST_PORT'},";
        print FILE "$cgiparams{'DEST_IFACE'},$cgiparams{'DEST_IPMAC'},";
        print FILE "$cgiparams{'NEW_DEST_PORT'},6,$cgiparams{'TARGET'},";
        print FILE "$cgiparams{'ENABLED'},$cgiparams{'DESCRIPTION'}\n";
        $cgiparams{'ORDER_NUMBER'}++;
        print FILE "$cgiparams{'ORDER_NUMBER'},$cgiparams{'SRC_IFACE'},";
        print FILE "$cgiparams{'SRC_IPMAC'},$cgiparams{'DEST_PORT'},";
        print FILE "$cgiparams{'DEST_IFACE'},$cgiparams{'DEST_IPMAC'},";
        print FILE "$cgiparams{'NEW_DEST_PORT'},17,$cgiparams{'TARGET'},";
        print FILE "$cgiparams{'ENABLED'},$cgiparams{'DESCRIPTION'}\n";
      } else {
        print FILE "$cgiparams{'ORDER_NUMBER'},$cgiparams{'SRC_IFACE'},";
        print FILE "$cgiparams{'SRC_IPMAC'},$cgiparams{'DEST_PORT'},";
        print FILE "$cgiparams{'DEST_IFACE'},$cgiparams{'DEST_IPMAC'},";
        print FILE "$cgiparams{'NEW_DEST_PORT'},$cgiparams{'PROTOCOL'},";
        print FILE "$cgiparams{'TARGET'},$cgiparams{'ENABLED'},";
        print FILE "$cgiparams{'DESCRIPTION'}\n";
      }
    }

    close(FILE);

    if ($cgiparams{'ACTION'} eq $tr{'add'}) {
      &log($tr{'forwarding rule added'});
    } else {
      &log($tr{'forwarding rule updated'});
    }
    ######################################

    my $success = &message('setportfw');
  
    unless (defined $success) { $errormessage = "$tr{'smoothd failure'}"; }

    unless ($success eq 'Port forwarding rules set') {
      $errormessage = "Error setting portforwarding rules: " . "$success";
    }

  }
  $cgiparams{'SRC_IPMAC'} = '';
  $cgiparams{'DEST_IPMAC'} = '';
  $cgiparams{'SRC_IFACE'} = $reddev;
  $cgiparams{'DEST_IFACE'} = $netsettings{'GREEN_DEV'};
  $cgiparams{'DEST_PORT'} = '';
  $cgiparams{'NEW_DEST_PORT'} = '';
  $cgiparams{'PROTOCOL'} = '6';
  $cgiparams{'TARGET'} = 'ACCEPT';
  $cgiparams{'ENABLED'} = 'on';
  $cgiparams{'DESCRIPTION'} = '';
}


# Check for remove or edit
#
elsif ($cgiparams{'ACTION'} eq $tr{'remove'} or
       $cgiparams{'ACTION'} eq $tr{'edit'} or
       $cgiparams{'ACTION'} eq $tr{'ffc-enable rule'}) {
  open(FILE, "$filename") or die 'Unable to open config file.';
  my @current = <FILE>;
  close(FILE);

  my $count = 0;
  my $id = 0;
  my $line;

  foreach $line (@current) {
    $id++;

    if ($cgiparams{$id} eq "on") {
      $count++;
    }
  }

  if ($count == 0) {
    $errormessage = $tr{'nothing selected'};
  }

  if ($count > 1 && $cgiparams{'ACTION'} eq $tr{'edit'}) {
    $errormessage = $tr{'you can only select one item to edit'};
  }

  unless ($errormessage) {
    open(FILE, ">$filename") or die 'Unable to open config file.';
    flock FILE, 2;
    $id = 0;
    $cnt = 1;

    foreach $line (@current) {
      $id++;

      unless ($cgiparams{$id} eq "on") {
        chomp $line;
        my @temp = split(/\,/, $line);
        $temp[0] = $cnt;
        print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],";
        print FILE "$temp[5],$temp[6],$temp[7],$temp[8],$temp[9],$temp[10]\n";
        $cnt++;
      } elsif ($cgiparams{'ACTION'} eq $tr{'edit'}) {
        chomp($line);
        my @temp = split(/\,/,$line);
        $cgiparams{'ORDER_NUMBER'} = $temp[0];
        $cgiparams{'SRC_IFACE'} = $temp[1];
        $cgiparams{'SRC_IPMAC'} = $temp[2];
        $cgiparams{'DEST_PORT'} = $temp[3];
        $cgiparams{'DEST_IFACE'} = $temp[4];
        $cgiparams{'DEST_IPMAC'} = $temp[5];
        $cgiparams{'NEW_DEST_PORT'} = $temp[6];
        $cgiparams{'PROTOCOL'} = $temp[7];
        $cgiparams{'TARGET'} = $temp[8];
        $cgiparams{'ENABLED'} = $temp[9];
        $cgiparams{'DESCRIPTION'} = $temp[10];

        # Editing support
        $cgiparams{'OLDID'} = $id;
        $updatebutton = 1;
        print FILE "$line\n";
      } elsif ($cgiparams{'ACTION'} eq $tr{'ffc-enable rule'}) {
        chomp $line;
        my @temp = split(/\,/,$line);
        $temp[0] = $cnt++;
        if ($temp[9] eq "on") {
          $enabled = "off";
        } else {
          $enabled = "on";
        }
        print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],";
        print FILE "$temp[5],$temp[6],$temp[7],$temp[8],$enabled,$temp[10]\n";
      }
    }

    close(FILE);

    if ($cgiparams{'ACTION'} eq $tr{'remove'}) {
      $cgiparams{'SRC_IPMAC'} = '';
      $cgiparams{'DEST_IPMAC'} = '';
      $cgiparams{'SRC_IFACE'} = $reddev;
      $cgiparams{'DEST_IFACE'} = $netsettings{'GREEN_DEV'};
      $cgiparams{'DEST_PORT'} = '';
      $cgiparams{'NEW_DEST_PORT'} = '';
      $cgiparams{'PROTOCOL'} = '6';
      $cgiparams{'TARGET'} = 'ACCEPT';
      $cgiparams{'ENABLED'} = 'on';
      $cgiparams{'DESCRIPTION'} = '';

      &log($tr{'forwarding rule removed'});
    } else {
      $cgiparams{'RULE_COUNT'} = $id;
    }

    my $success = &message('setportfw');
  
    unless (defined $success) {
      $errormessage = "$tr{'smoothd failure'}";
    }
    unless ($success eq 'Port forwarding rules set') {
      $errormessage = "Error setting portforwarding rules: " . "$success";
    }

  }
}


# Check for errormessage
if ($errormessage ne '') {
  # Reset defaults for ip's/ports
  if ($cgiparams{'SRC_IPMAC'} eq '0.0.0.0/0') {
    $cgiparams{'SRC_IPMAC'} = '';
  }

  if ($cgiparams{'DEST_IPMAC'} eq '0.0.0.0/0') {
    $cgiparams{'DEST_IPMAC'} = '';
  }

  if ($cgiparams{'DEST_PORT'} eq '0') {
    $cgiparams{'DEST_PORT'} = '';
  }

  if ($cgiparams{'NEW_DEST_PORT'} eq '0') {
    $cgiparams{'NEW_DEST_PORT'} = '';
  }
}


# Finished with action handling
#
open(FILE, "$filename") or die 'Unable to open config file.';
while (<FILE>) { $cgiparams{'RULE_COUNT'}++; }
close(FILE);
$cgiparams{'RULE_COUNT'}++;

# Check for normal page load
if ($cgiparams{'ACTION'} eq '') {
  $cgiparams{'PROTOCOL'} = '6';
  $cgiparams{'ENABLED'} = 'on';
  $cgiparams{'ADDRESS_ENABLED'} = 'on';

  # Support for outbound firewall
  $cgiparams{'SRC_IFACE'} = $reddev;
  $cgiparams{'DEST_IFACE'} = $netsettings{'GREEN_DEV'};
  $cgiparams{'TARGET'} = 'ACCEPT';
  $cgiparams{'ORDER_NUMBER'} = $cgiparams{'RULE_COUNT'};
  $cgiparams{'DESCRIPTION'} = '';
}

my ($tmp1, @temp);
# Network device listing
my %availablenetdevices;
my @devices = `ifconfig`;
foreach $dev (@devices) {
  chomp $dev;
  @temp = split(' ', $dev);
  if ($temp[1] =~ /^Link/) {
    if ($temp[0] =~ /^ipsec/ or $temp[0] =~ /^lo/) {
      next;
    } else {
      my @deviceinfo = `ifconfig $temp[0]`;
      foreach $devinfo (@deviceinfo) {
        chomp $devinfo;
        if ($devinfo =~ /inet addr\:(\d{1,3}(\.\d{1,3}){3})/) {
          $availablenetdevices{$temp[0]} = $1;
        }
      }
    }
  }
}

foreach $dev (keys(%availablenetdevices)) {
  my $tmp = grep(/^$dev$/, @filelist);
  if ($tmp > 0) {
    delete($availablenetdevices{$dev});
  }
}

# Support for outbound firewall
$selected{'SRC_IFACE'}{''} = '';
$selected{'DEST_IFACE'}{''} = '';

foreach $dev (sort(keys(%availablenetdevices))) {
  $selected{'SRC_IFACE'}{$dev} = '';
  $selected{'DEST_IFACE'}{$dev} = '';
}

$selected{'SRC_IFACE'}{$cgiparams{'SRC_IFACE'}} = 'SELECTED';
$selected{'DEST_IFACE'}{$cgiparams{'DEST_IFACE'}} = 'SELECTED';
$selected{'PROTO'}{$cgiparams{'PROTO'}} = 'SELECTED';

$selected{'IF_ALIAS'}{'RED'} = '';
$selected{'IF_ALIAS'}{'GREEN'} = '';
if ($cgiparams{'ORANGE_DEV'}) {
  $selected{'IF_ALIAS'}{'ORANGE'} = '';
}
if ($cgiparams{'PURPLE_DEV'}) {
  $selected{'IF_ALIAS'}{'PURPLE'} = '';
}
$selected{'IF_ALIAS'}{$cgiparams{'IF_ALIAS'}} = 'SELECTED';

$selected{'TARGET'}{'ACCEPT'} = '';
$selected{'TARGET'}{'DROP'} = '';  
$selected{'TARGET'}{'REJECT'} = '';  
$selected{'TARGET'}{'LOG'} = '';  
$selected{'TARGET'}{$cgiparams{'TARGET'}} = 'SELECTED';

$checked{'ENABLED'}{'off'} = '';
$checked{'ENABLED'}{'on'} = '';  
$checked{'ENABLED'}{$cgiparams{'ENABLED'}} = 'CHECKED';

$checked{'ADDRESS_ENABLED'}{'off'} = '';
$checked{'ADDRESS_ENABLED'}{'on'} = '';  
$checked{'ADDRESS_ENABLED'}{$cgiparams{'ADDRESS_ENABLED'}} = 'CHECKED';

# Protocol listing
open(TMP, "/etc/protocols") or die "Unable to open /etc/protocols\: $!\n";
my @protocols = <TMP>;
close(TMP);

my %availableprotocols;
open(FILE, ">${swroot}/portfw/protolist") or die 'Unable to write protocol list file';
foreach $line (@protocols) { 
  chomp $line;

  if ($line =~ m/^([a-z0-9]|\#\t[0-9]+\t+)/i) {
    my @protoline = split(/\s+/, $line);
    print FILE "$protoline[1],$protoline[2]\n";
    if ($#protoline == 3) {
      $protoline[3] =~ s/\b(\w)/\u\L$1/g;
      $availableprotocols{$protoline[1]} = "$protoline[2] = $protoline[3]";
    } else {
      $protoline[2] =~ s/\b(\w)/\u\L$1/g;
      $availableprotocols{$protoline[1]} = "$protoline[2]";
    }
  }
}
close FILE;

delete($availableprotocols{'0'});
delete($availableprotocols{'41'});
delete($availableprotocols{'43'});
delete($availableprotocols{'44'});
delete($availableprotocols{'58'});
delete($availableprotocols{'59'});
delete($availableprotocols{'60'});
delete($availableprotocols{'253'});
delete($availableprotocols{'254'});
delete($availableprotocols{'255'});
my @sortedprotocols = sort { $a <=> $b } keys(%availableprotocols);


######
# Start generating HTML
######

&openpage($tr{'full firewall control'}, 1, '', 'networking');

&openbigbox('100%', 'left');

&alertbox($errormessage);

# Javascript support lines
print "
<script type='application/javascript' src='/ui/js/utility.js'></script>
<script>
  function ffoxSelectUpdate(elmt)
  {
    if(!document.all) elmt.style.cssText = elmt.options[elmt.selectedIndex].style.cssText;
  }
</script>
";

# Border for debug
my $border = 0;

######################################################################
# Mod for source ip, destination ip, and destination port edit display
######################################################################
if ($cgiparams{'SRC_IPMAC'} eq '0.0.0.0/0') {
  $sourceipmac = '';
} else {
  $sourceipmac = $cgiparams{'SRC_IPMAC'};
}

if ($cgiparams{'DEST_IPMAC'} eq '0.0.0.0/0') {
  $destinationipmac = '';
} else {
  $destinationipmac = $cgiparams{'DEST_IPMAC'};
}

if ($cgiparams{'DEST_PORT'} eq '0') {
  $sourceport = '';
} else {
  $sourceport = $cgiparams{'DEST_PORT'};
}

if ($cgiparams{'NEW_DEST_PORT'} eq '0') {
  $destinationport = '';
} else {
  $destinationport = $cgiparams{'NEW_DEST_PORT'};
}

if ($updatebutton) {
  $buttontext = $tr{'ffc-update'};
  $boxtext = $tr{'update current rule'};
} else {
  $buttontext = $tr{'add'};
  $boxtext = $tr{'add a new rule'};
}

if ($aliasupdbutton) {
  $aliasbtntext = $tr{'multiip-update'};
  $aliasboxtext = $tr{'multiip-update current alias'};
} else {
  $aliasbtntext = $tr{'ffc-multi add'};
  $aliasboxtext = $tr{'add new alias'};
}

print "
<style type='text/css'>
  option.red{color:red;}
  option.green{color:green;}
  option.orange{color:orange;}
  option.purple{color:purple;}
</style>
";

##############################################

print "<form method='post' name='FIREWALL'>\n";
print "
<table width='100%' class='box' style='margin-top:8pt; margin-bottom:8pt'>
  <tr>
    <td align='center' class='boldbase'>
      <b>$tr{'ffc section'}</b>
    </td>
  </tr>
  <tr>
    <td>

";

# Update interfaces
if (open(FILE, "$aliasfile"))
{
  @aliases = <FILE>;
  close FILE;
}

open(ALIASES, ">$aliasfile") or die 'Unable to open';
flock ALIASES, 2;
foreach $line (@aliases) {
  chomp $line;
  @temp = split(/\,/, $line);
  unless (($temp[0] eq "RED") or ($temp[0] eq "GREEN") or 
          ($temp[0] eq "ORANGE") or ($temp[0] eq "PURPLE")) {
    print ALIASES "$line\n";
  } elsif ($temp[0] eq "RED") {
    unless ($netsettings{'RED_TYPE'} eq "PPPOE") {
      my $block = new Net::Netmask ($redip, $netsettings{'RED_NETMASK'});
      print ALIASES "RED,$reddev,$reddev,$redip,$netsettings{'RED_NETMASK'},";
      print ALIASES "$netsettings{'RED_BROADCAST'},off,on,,$intadd\n";
    } else {
      print ALIASES "RED,$reddev,$reddev,$redip,N/A,N/A,off,on,,N/A\n";
    }
  } elsif ($temp[0] eq "GREEN") {
    $block = new Net::Netmask ($netsettings{'GREEN_ADDRESS'}, $netsettings{'GREEN_NETMASK'});
    print ALIASES "GREEN,$netsettings{'GREEN_DEV'},$netsettings{'GREEN_DEV'},";
    print ALIASES "$netsettings{'GREEN_ADDRESS'},$netsettings{'GREEN_NETMASK'},";
    print ALIASES "$netsettings{'GREEN_BROADCAST'},off,on,,$intadd\n";
  } elsif ($temp[0] eq "ORANGE") {
    $block = new Net::Netmask ($netsettings{'ORANGE_ADDRESS'}, $netsettings{'ORANGE_NETMASK'});
    print ALIASES "ORANGE,$netsettings{'ORANGE_DEV'},$netsettings{'ORANGE_DEV'},";
    print ALIASES "$netsettings{'ORANGE_ADDRESS'},$netsettings{'ORANGE_NETMASK'},";
    print ALIASES "$netsettings{'ORANGE_BROADCAST'},off,on,,$intadd\n";
  } elsif ($temp[0] eq "PURPLE") {
    $block = new Net::Netmask ($netsettings{'PURPLE_ADDRESS'}, $netsettings{'PURPLE_NETMASK'});
    print ALIASES "PURPLE,$netsettings{'PURPLE_DEV'},$netsettings{'PURPLE_DEV'},";
    print ALIASES "$netsettings{'PURPLE_ADDRESS'},$netsettings{'PURPLE_NETMASK'},";
    print ALIASES "$netsettings{'PURPLE_BROADCAST'},off,on,,$intadd\n";
  }
}
close ALIASES;

# Check for running interfaces
if (open(FILE, "$aliasfile"))
{
  @aliases = <FILE>;
foreach $line (@aliases) {
}
  close FILE;
}

open(FILE, ">$aliasfile") or die 'Unable to open';
foreach $line (@aliases) {
  chomp $line;
  @temp = split (/\,/, $line);
  my $alias = $temp[1];
  my $running;
  my @ifconfig = `/sbin/ifconfig $alias`;
  foreach my $line2 (@ifconfig) {
    if ($line2 =~ /inet/) {
      $running = "on";
      last;
    } else {
      $running = "off";
    }
  }
  print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],";
  print FILE "$temp[5],$running,$temp[7],$temp[8],$temp[9]\n";
}
close FILE;
 
&openbox($boxtext);

# Extract the initial iface color for SRC
my $SRC_COLOR = 'black';
$SRC_COLOR = 'green'
    if ($selected{'SRC_IFACE'}{$netsettings{'GREEN_DEV'}} eq 'SELECTED');

$SRC_COLOR = 'orange'
    if ($selected{'SRC_IFACE'}{$netsettings{'ORANGE_DEV'}} eq 'SELECTED');

$SRC_COLOR = 'purple'
    if ($selected{'SRC_IFACE'}{$netsettings{'PURPLE_DEV'}} eq 'SELECTED');

$SRC_COLOR = 'red'
    if ($selected{'SRC_IFACE'}{$reddev} eq 'SELECTED');

# Extract the initial iface color for DEST
my $DEST_COLOR = 'black';
$DEST_COLOR = 'green'
    if ($selected{'DEST_IFACE'}{$netsettings{'GREEN_DEV'}} eq 'SELECTED');

$DEST_COLOR = 'orange'
    if ($selected{'DEST_IFACE'}{$netsettings{'ORANGE_DEV'}} eq 'SELECTED');

$DEST_COLOR = 'purple'
    if ($selected{'DEST_IFACE'}{$netsettings{'PURPLE_DEV'}} eq 'SELECTED');

$DEST_COLOR = 'red'
    if ($selected{'DEST_IFACE'}{$reddev} eq 'SELECTED');

print "
<table width='100%' border='$border'>
  <tr>
    <td class='base' width='25%'>
      $tr{'source ifacec'}
    </td>
    <td width='25%'>
      <select style='color: $SRC_COLOR' onchange='ffoxSelectUpdate(this);' name='SRC_IFACE'>
        <option style='color: black' value='any' $selected{'SRC_IFACE'}{'any'}>
          Any
        </option>
";

my @temp3;
if (open(FILE, "$aliasfile"))
{
  @temp3 = <FILE>;
  close FILE;
}

foreach $dev (sort(keys(%availablenetdevices))) {
  $dev =~ /(\:\d{1,3})/;
  my $devifacesub = $1;
  my $redaliasip;

  foreach $line (@temp3) {
    chomp $line;
    @split = split(/\,/, $line);
    if ($split[0] eq "RED$devifacesub" && $dev =~ /:/) {
      $redaliasip = $split[3];
    }
  }	

  if ($netsettings{'GREEN_DEV'} && ($dev =~ /$netsettings{'GREEN_DEV'}/)) {
    print "        <option style='color: green' value='$dev' $selected{'SRC_IFACE'}{$dev}>\n";
    print "          GREEN$devifacesub - $dev\n";
    print "        </option>\n";

  } elsif ($netsettings{'ORANGE_DEV'} && ($dev =~ /$netsettings{'ORANGE_DEV'}/)) {
    print "        <option style='color: orange' value='$dev' $selected{'SRC_IFACE'}{$dev}>\n";
    print "          ORANGE$devifacesub - $dev\n";
    print "        </option>\n";

  } elsif ($netsettings{'PURPLE_DEV'} && ($dev =~ /$netsettings{'PURPLE_DEV'}/)) {
    print "        <option style='color: purple' value='$dev' $selected{'SRC_IFACE'}{$dev}>\n";
    print "          PURPLE$devifacesub - $dev\n";
    print "        </option>\n";

  } elsif ($reddev && ($dev =~ /$reddev/)) {
    print "        <option style='color: red' value='$dev' $selected{'SRC_IFACE'}{$dev}>\n";
    if ($redaliasip)
    {
      print "          RED$devifacesub $redaliasip\n";
    }
    else
    {
      print "          RED$devifacesub - $dev\n";
    }
      print "        </option>\n";

  } else {
    print "        <option style='color: black' value='$dev' $selected{'SRC_IFACE'}{$dev}>\n";
    print "          $dev\n";
    print "        </option>\n";
  }
}

print "      </select>
    </td>
    <td class='base' width='25%'>
      $tr{'new destination ifacec'}
    </td>
    <td width='25%'>
      <select style='color: $DEST_COLOR' onchange='ffoxSelectUpdate(this);' name='DEST_IFACE'>
        <option style='color: black' value='any' $selected{'DEST_IFACE'}{'any'}>
          Any
        </option>
";

foreach $dev (sort(keys(%availablenetdevices))) {
  $dev =~ /(\:\d{1,3})/;
  my $devifacesub = $1;
  my $redaliasip;

  foreach $line (@temp3) {
    chomp $line;
    @split = split(/\,/, $line);
    if ($split[0] eq "RED$devifacesub" && $dev =~ /:/) {
      $redaliasip = $split[3];
    }
  }	

  if ($netsettings{'GREEN_DEV'} && ($dev =~ /$netsettings{'GREEN_DEV'}/)) {
    print "        <option style='color: green' value='$dev' $selected{'DEST_IFACE'}{$dev}>\n";
    print "          GREEN$devifacesub - $dev\n";
    print "        </option>\n";

  } elsif ($netsettings{'ORANGE_DEV'} && ($dev =~ /$netsettings{'ORANGE_DEV'}/)) {
    print "        <option style='color: orange' value='$dev' $selected{'DEST_IFACE'}{$dev}>\n";
    print "          ORANGE$devifacesub - $dev\n";
    print "        </option>\n";

  } elsif ($netsettings{'PURPLE_DEV'} && ($dev =~ /$netsettings{'PURPLE_DEV'}/)) {
    print "        <option style='color: purple' value='$dev' $selected{'DEST_IFACE'}{$dev}>\n";
    print "          PURPLE$devifacesub - $dev\n";
    print "        </option>\n";

  } elsif ($reddev && ($dev =~ /$reddev/)) {
    print "        <option style='color: red' value='$dev' $selected{'DEST_IFACE'}{$dev}>\n";
    if ($redaliasip)
    {
      print "          RED$devifacesub $redaliasip\n";
    }
    else
    {
      print "          RED$devifacesub - $dev\n";
    }
      print "        </option>\n";

  } else {
    print "        <option style='color: black' value='$dev' $selected{'DEST_IFACE'}{$dev}>\n";
    print "          $dev\n";
    print "        </option>\n";
  }
}

print "      </select>
    </td>
  </tr>
  <tr>
    <td class='base' nowrap='nowrap'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'source ippfc'}
    </td>
    <td>
      <input type='text' name='SRC_IPMAC' value='$sourceipmac' size='18'>
    </td>
    <td class='base' nowrap='nowrap'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'new destination ippfc'}
    </td>
    <td>
      <input type='text' name='DEST_IPMAC' value='$destinationipmac' size='18'>
    </td>
  </tr>
  <tr>
    <td class='base' width='12%' nowrap='nowrap'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'source port or rangec'}
    </td>
    <td>
      <input type='text' name='DEST_PORT' value='$sourceport' SIZE='11'>
    </td>
    <td class='base' nowrap='nowrap'>
      <img src='/ui/img/blob.gif' valign='top'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'new destination port or rangec'}
    </td>
    <td>
      <input type='text' name='NEW_DEST_PORT' value='$destinationport' size='11'>
    </td>
  </tr>
  <tr>
    <td colspan='6'>

      <table width='100%' border=$border style='margin:6pt 0'>
        <tr>
          <td class='base'>$tr{'protocol longc'}</td>
          <td>
            <select name='PROTOCOL'>
";
        my $protocol = $cgiparams{'PROTOCOL'};
        if ($protocol eq 'all') {
          print "              <option value='all' selected>All</option>\n";
        } else {
          print "              <option value='all'>All</OPTION>\n";
        }

        if ($protocol eq '6') {
          print "              <option value='6' selected>$availableprotocols{6}</option>\n";
        } else {
          print "              <option value='6'>$availableprotocols{6}</option>\n";
        }

        if ($protocol eq '17') {
          print "              <option value='17' selected>$availableprotocols{17}</option>\n";
        } else {
          print "              <option value='17'>$availableprotocols{17}</option>\n";
        }

        if ($protocol eq 'TCP & UDP') {
          print "              <option value='TCP & UDP' selected>TCP & UDP</option>\n";
        } else {
          print "              <option value='TCP & UDP'>TCP & UDP</option>\n";
        }

        if ($protocol eq '1') {
          print "              <option value='1' selected>$availableprotocols{1}</option>\n";
        } else {
          print "              <option value='1'>$availableprotocols{1}</option>\n";
        }

        if ($protocol eq '47') {
          print "              <option value='47' selected>$availableprotocols{47}</option>\n";
        } else {
          print "              <option value='47'>$availableprotocols{47}</option>\n";
        }

        if ($protocol eq '50') {
          print "              <option value='50' selected>$availableprotocols{50}</option>\n";
        } else {
          print "              <option value='50'>$availableprotocols{50}</option>\n";
        }

        if ($protocol eq '51') {
          print "              <option value='51' selected>$availableprotocols{51}</option>\n";
        } else {
          print "              <option value='51'>$availableprotocols{51}</option>\n";
        }

print "            </select>
          </td>
          <td class='base'>$tr{'ffc-targetc'}</td>
          <td>
            <select name='TARGET'>
              <option value='ACCEPT' $selected{'TARGET'}{'ACCEPT'}>$tr{'target accept'}</option>
              <option value='REJECT' $selected{'TARGET'}{'REJECT'}>$tr{'target reject'}</option>
              <option value='DROP' $selected{'TARGET'}{'DROP'}>$tr{'target drop'}</option>
              <option value='LOG' $selected{'TARGET'}{'LOG'}>$tr{'target log'}</option>
            </select></td>
          <td class='base'>$tr{'order numberc'}</td>
          <td>
            <select name='ORDER_NUMBER'>
";

        for ($cnt = 1; $cnt < $cgiparams{'RULE_COUNT'} + 1; $cnt++) {
          if ($cnt eq $cgiparams{'ORDER_NUMBER'}) {
            print "              <option value='$cnt' selected>$cnt</option>\n";
          } else {
            print "              <option value='$cnt'>$cnt</option>\n";
          }
        }

print "            </select>
          </td>
        </tr>
        <tr>
          <td class='base'>$tr{'descriptionc'}</td>
          <td colspan='5'>
            <input type='text' name='DESCRIPTION' size='80' value='$cgiparams{'DESCRIPTION'}'
                   id='description' @{[jsvalidcomment('description')]}>
          </td>
        </tr>
      </table>

      <table style='width:100%; margin:6pt 0' border='$border'>
        <tr>
          <td style='width:50%; text-align:center'>
            $tr{'enabled'}
            <input type='checkbox' name='ENABLED' $checked{'ENABLED'}{'on'}>
          </td>
          <td style='width:50%; text-align:center'>
            <input type='submit' name='ACTION' value='$buttontext' onclick='return validate();'>
            <input type='hidden' name='OLDID' value='$cgiparams{'OLDID'}'>
          </td>
        </tr>
      </table>

    </td>
  </tr>
  <tr>
    <td colspan='6' style='text-align:left'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'portfw source destination ip'}
    </td>
  </tr>
  <tr>
    <td colspan='5' style='text-align:left'>
      <img src='/ui/img/blob.gif' valign='top'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'portfw destination port'}
    </td>
  </tr>
</table>

<script language='JavaScript' type='text/javascript'>
<!--
  // validate function
  function validate() {
    // Vars
    var errorMessage = \"\";

    // Get form
    var form = document.forms['FIREWALL'];

    // Setup fields
    form.DESCRIPTION.humanname = 'Description';
    form.DESCRIPTION.checkspecialchars = true;
    form.DESCRIPTION.specialChars = /(,)/;

    // Check for source and/or destination ports being filled in
    if ((form.DEST_PORT.value != '') || (form.NEW_DEST_PORT.value != '')) {
      // Check for a protocol of Any
      if (form.PROTOCOL.selectedIndex == 0) {
        // Add to error message
        errorMessage += \"\\n  - $tr{'error source-destination port protocol any'}\";
      }
//
//			// Check for an empty/Any destination
//			if (form.DEST_IPMAC.value == '') {
//				// Add to error message
//				errorMessage += \"\\n  - $tr{'error source-destination port destination any'}\";
//			}
    }

    // Check for errormessage
    if (errorMessage != '') {
      // Alert and return false
      alert(\"The following errors have occured:\" + errorMessage);
      return false;
    }

    // Check for source and destination both being blank, warn if so
    if ((form.SRC_IPMAC.value == '') && (form.DEST_IPMAC.value == '')) {
      // Confirm you really want to do this
      if (!confirm(\"Are you sure you want to \" + form.TARGET.options[form.TARGET.selectedIndex].text + \" \" + form.PROTOCOL.options[form.PROTOCOL.selectedIndex].text + \" traffic from \" + form.SRC_IFACE.options[form.SRC_IFACE.selectedIndex].text + \" to \" + form.DEST_IFACE.options[form.DEST_IFACE.selectedIndex].text + \"?\")) {
        // Return false if not wanted
        return false;
      }
    }

    // Check form
    if (checkForm(form)) {
      // Return true
      return true;
    }

    // Return false
    return false;
  }
-->
</script>
";

&closebox();

&openbox($tr{'current rules'});

my $portmap = &portsmap();
my $protocolmap = &protocolmap();
my $ifcolorsmap = &ifcolormap();

my $border = 0;
my %render_settings =
(
  'url'     => "/cgi-bin/portfw.cgi?$cgiparams{'COLUMN_ONE'},$cgiparams{'ORDER_ONE'},[%COL%],[%ORD%]",
  'columns' => 
  [
    { 
      column => '1',
      title  => $tr{'order'},
      size   => 10,
      sort   => '<=>',
    },
    { 
      column => '2',
      title  => $tr{'src dev'},
      size   => 15,
      sort   => 'cmp',
      tr	=> \%{$ifcolorsmap},
    },
    {
      column => '3',
      title  => $tr{'original destination ipmac'},
      size   => 15,
      sort   => 'cmp',
      tr	=> { '0.0.0.0/0' => 'Any' },
    },
    { 
      column => '4',
      title  => $tr{'original destination port'},
      size   => 10,
      sort   => 'cmp',
      tr	=> \%{$portmap},
    },
    { 
      column => '5',
      title  => $tr{'new destination dev'},
      size   => 15,
      sort   => 'cmp',
      tr	=> \%{$ifcolorsmap},
    },
    { 
      column => '6',
      title  => $tr{'new destination ip'},
      size   => 15,
      sort   => 'cmp',
      tr	=> { '0.0.0.0/0' => 'Any' },		
    },
    { 
      column => '7',
      title  => $tr{'new destination port or range'},
      size   => 10,
      sort   => 'cmp',
      tr	=> \%{$portmap},
    },
    {
      column => '8',
      title  => $tr{'protocol long'},
      size   => 10,
      sort   => 'cmp',
      tr     => \%{$protocolmap},
    },
    {
      column => '9',
      title  => "$tr{'ffc-target'}",
      size   => 10,
      sort   => 'cmp',
    },
    {
      column => '10',
      title  => "$tr{'enabledtitle'}",
      size   => 5,
      tr     => 'onoff',
      align  => 'center',
    },
    {
      title  => "$tr{'mark'}", 
      size   => 5,
      mark   => ' ',
    },
    { 
      column => '11',
      title => "$tr{'comment'}",
      break => 'line',
    }
  ]
);

&dispaliastab($filename, \%render_settings, $cgiparams{'ORDER_TWO'}, $cgiparams{'COLUMN_TWO'} );

print "
<table style='width:100%; margin:6pt 0' border='$border'>
  <tr>
    <td width='33%' style='text-align:center'>
      <input type='submit' name='ACTION' value='$tr{'remove'}'
             onClick='if(confirm(\"$tr{'del port fwd rules'}\")) {return true;} return false;'>
    </td>
    <td width='34%' style='text-align:center'>
      <input type='submit' name='ACTION' value='$tr{'ffc-enable rule'}'>
    </td>
    <td width='33%' style='text-align:center'>
      <input type='submit' name='ACTION' value='$tr{'edit'}'>
    </td>
  </tr>
</table>
";

&closebox();

# End the P&P Fwd Sect.
print "
    </td>
  </tr>
</table>
";

print "

<table width='100%' class='box'>
  <tr>
    <td align='center' class='bolbdbase'>
      <b>$tr{'multi-ip section'}</b>
    </td>
  </tr>
  <tr>
    <td>
";

&openbox($aliasboxtext);

my $border = 0;
print "

<table style='width:100%; margin:6pt 0' border='$border'>
  <tr>
    <td class='base'>
      $tr{'red alias ipc'}
    </td>
    <td>
      <input type='text' name='ADDRESS' value='$cgiparams{'ADDRESS'}'
             size=15' id='ifaddress' @{[jsvalidipormask('ifaddress','true')]}>
    </td>
    <td class='base'>
      $tr{'multi-ip netmask'}:
    </td>
    <td>
      <input type='text' name='NETMASK' value='$cgiparams{'NETMASK'}'
             size='15' id='ifnetmask' @{[jsvalidipormask('ifnetmask','true')]}>
    </td>
    <td class='base'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'internal ipc'}
    </td>
    <td>
      <input type='text' name='INTADDRESS' value='$cgiparams{'INTADDRESS'}'
             size='15' id='ifintaddress' @{[jsvalidip('ifintaddress','true')]}>
    </td>
  </tr>
  <tr>
    <td class='base'>
      $tr{'descriptionc'}
    </td>
    <td colspan='5'>
      <input type='text' name='COMMENT' size='80' value='$cgiparams{'COMMENT'}'
             id='comment' @{[jsvalidcomment('comment')]}>
    </td>
  </tr>
</table>

<table style='width:100%; margin:6pt 0' border='$border'>
  <tr>
    <td width='50%' align='center'>
      $tr{'enabled'}
      <input type='checkbox' name='ADDRESS_ENABLED' $checked{'ADDRESS_ENABLED'}{'on'}>
    </td>
    <td width='50%' align='center'>
      <input type='submit' name='ACTION' value='$aliasbtntext'>
      <input type='hidden' name='OLDALIAS' value='$cgiparams{'OLDALIAS'}'>
    </td>
  </tr>
</table>

<table style='width:100%; margin:8pt 0 0 0'>
  <tr>
    <td colspan='6' align='left'>
      <img src='/ui/img/blob.gif' valign='top'>
      $tr{'enter internal lan ip'}
    </td>
  </tr>
</table>
";

&closebox();

&openbox($tr{'multi-ip current ip addresses'});

my $ifcolorsmap = &ifcolormap();

my %render_settings =
(
  'url'     => "/cgi-bin/portfw.cgi?[%COL%],[%ORD%],$cgiparams{'COLUMN_TWO'},$cgiparams{'ORDER_TWO'}",
  'columns' => 
  [
    { 
      column => '2',
      title  => $tr{'interface'},
      size   => 15,
      sort   => 'cmp',
      tr	=> \%{$ifcolorsmap},
    },
    {
      column => '4',
      title  => $tr{'alias ip addr'},
      size   => 20,
      sort   => 'cmp',
    },
    {
      column => '10',
      title  => $tr{'internal ip addr'},
      size   => 20,
      sort   => 'cmp',
      tr	=> { '' => 'N/A' },
    },
    { 
      column => '7',
      title  => $tr{'active'},
      size   => 5,
      tr     => 'onoff',
      align  => 'center',
    },
    { 
      column => '8',
      title  => $tr{'multi-ip enabled'},
      size   => 5,
      tr     => 'onoff',
      align  => 'center',
    },
    {
      title  => $tr{'mark'}, 
      size   => 5,
      mark   => ' ',
    },
    { 
      column => '9',
      title  => $tr{'comment'},
      break  => 'line',
    }
  ]
);

&dispaliastab($aliasfile, \%render_settings, $cgiparams{'ORDER_ONE'}, $cgiparams{'COLUMN_ONE'});

print "

<table width='100%'>
  <tr>
    <td width='33%' align='center'>
      <input type='submit' name='ACTION' value='$tr{'multi-ip remove'}'
             onClick=\"if(confirm('$tr{'multiip-validate'}')) {return true;} return false;\" >
    </td>
    <td width='34%' align='center'>
      <input type='submit' name='ACTION' value='$tr{'multi-ip enable alias'}'>
    </td>
    <td width='33%' align='centeR'>
      <input type='submit' name='ACTION' value='$tr{'multi-ip edit'}'>
    </td>
  </tr>
</table>
<table style='width:100%; margin:8pt 0 0 0'>
  <tr>
    <td colspan='6'>
      $tr{'multi-ip device note'}
    </td>
  </tr>
</table>
";

&closebox();

# close the Mult. Stat. IP Sect.
print "
    <td>
  </tr>
</table>
";

print "</form>\n";


&alertbox('add','add');

&closebigbox();

&closepage();

sub validmac {
  $_ = $_[0];

  # Check for valid MAC format
  if (/^[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}$/) {
    return 1;
  }

  return 0;
}

sub portsmap
{
  my %ports;
  my $portfiles = "/var/smoothwall/knownports/*";
  
  foreach my $filenames ( glob $portfiles ){
    unless (open(FILE, $filenames))
    {
      next;
    }

    while ( my $line = <FILE> ){
      chomp $line;
      next if ( $line eq "" );
  
      my ( $name, $value ) = split /,/, $line;

      if ($value eq "0") {
        $ports{$value} = '*';
      } else {
        $ports{$value} = "$name ($value)";
      }
    }
  }
  return \%ports;
}

sub ifcolormap
{
  my %ifcolors;
  
  open(FILE, $aliasfile) or return %ifcolors;

  while ( my $line = <FILE> ){
    chomp $line;
    next if ( $line eq "" );
  
    my ( $ifcolor, $ethdev ) = split /,/, $line;
    if ($ifcolor =~ /^RED/) {
      $ethcolor = "<span style='color:red'>$ifcolor</span>";
    } elsif ($ifcolor =~ /^GREEN/) {
      $ethcolor = "<span style='color:green'>$ifcolor</span>";
    } elsif ($ifcolor =~ /^ORANGE/) {
      $ethcolor = "<span style='color:orange'>$ifcolor</span>";
    } elsif ($ifcolor =~ /^PURPLE/) {
      $ethcolor = "<span style='color:purple'>$ifcolor</span>";
    } else {
      $ethcolor = $ifcolor;
    }
    $ifcolors{$ethdev} = "$ethcolor ($ethdev)";
  }
  close FILE;
  return \%ifcolors;
}

sub protocolmap
{
  my %protocollist;
  
  open(FILE, "${swroot}/portfw/protolist") or die 'Unable to read protocol list file';
  while ( my $line = <FILE> ){
    chomp $line;
    next if ( $line eq "" );

    my ($protocoln, $proto) = split /,/, $line;	
    $protocollist{$protocoln} = $proto;
  }
  close FILE;
  return \%protocollist;
}
