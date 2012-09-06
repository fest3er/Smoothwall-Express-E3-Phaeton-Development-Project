#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
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
# Many updates too numerous to list right now. 10/2/2010
#  - Code contributed by Steve McNeil to enable control of subnet check routine from GUI
#
use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );
use smoothtype qw( :standard );
use modlib qw( :standard );

use Socket;
use NetAddr::IP;
use Net::Netmask;

my %netsettings;
&readhash( "${swroot}/ethernet/settings", \%netsettings );
my %subcheck;
if ( -e "${swroot}/portfw/subcheck" ) {
    &readhash( "${swroot}/portfw/subcheck", \%subcheck );
}

# Subroutine for sorting things numerically instead of as strings
sub numerically { $a <=> $b; }

my $aliasfile = "${swroot}/portfw/aliases";
my $settings  = "${swroot}/ethernet/settings";
my @colours   = ( "RED", "GREEN" );
if ( $netsettings{'ORANGE_DEV'} ) {
    push( @colours, "ORANGE" );
}
if ( $netsettings{'PURPLE_DEV'} ) {
    push( @colours, "PURPLE" );
}

my $reddev;

# Determine red interface type
if ( $netsettings{'RED_TYPE'} eq "PPPOE" ) {
    $reddev = "ppp0";
} else {
    $reddev = $netsettings{'RED_DEV'};
}

my $redip = '';
if ( -e "${swroot}/red/local-ipaddress" ) {
    open( FILE, "${swroot}/red/local-ipaddress" )
      or die 'Unable to open config file.';
    $redip = <FILE>;
    chomp $redip;
    close(FILE);
}

my ( %cgiparams, %selected, %checked );
my $filename = "${swroot}/portfw/config";

&showhttpheaders();

$cgiparams{'COLUMN'} = 1;
$cgiparams{'ORDER'}  = $tr{'log ascending'};

#$cgiparams{'COLUMN_TWO'} = 1;
#$cgiparams{'ORDER_TWO'} = $tr{'log ascending'};

if ( $ENV{'QUERY_STRING'} and ( not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ) )
{
    my @temp = split( ',', $ENV{'QUERY_STRING'} );
    $cgiparams{'ORDER'}  = $temp[1] if ( defined $temp[1] and $temp[1] ne "" );
    $cgiparams{'COLUMN'} = $temp[0] if ( defined $temp[0] and $temp[0] ne "" );
}

$cgiparams{'OLDID'}           = 0;
$cgiparams{'ENABLED'}         = 'off';
$cgiparams{'ADDRESS_ENABLED'} = 'off';

$cgiparams{'RULE_COUNT'}  = 0;
$cgiparams{'DESCRIPTION'} = '';
&getcgihash( \%cgiparams );

my $errormessage = '';
my $updatebutton = 0;

# Check for add or update action
if (   $cgiparams{'ACTION'} eq $tr{'add'}
    or $cgiparams{'ACTION'} eq $tr{'ffc-update'} )
{
    my $ipmac = $cgiparams{'SRC_IPMAC'};
    if ( $ipmac =~ /^\!/ ) {
        $ipmac =~ s/^\!//;
    }
    unless ( &validipormask($ipmac) or &validmac($ipmac) ) {
        if ( $ipmac ne '' ) {
            $errormessage .= "$tr{'source ip bad'}<BR />";
        } else {
            $cgiparams{'SRC_IPMAC'} = '0.0.0.0/0';
        }
    }

    if ( $cgiparams{'DEST_PORT'} ) {
        unless ( &validportrange( $cgiparams{'DEST_PORT'} ) ) {
            $errormessage .= "$tr{'source port numbers'}<BR />";
        }
    } else {
        $cgiparams{'DEST_PORT'} = 0;
    }

    if ( $cgiparams{'NEW_DEST_PORT'} ) {
        unless ( &validportrange( $cgiparams{'NEW_DEST_PORT'} ) ) {
            $errormessage .= "$tr{'destination port numbers'}<BR />";
        }
    } else {
        $cgiparams{'NEW_DEST_PORT'} = 0;
    }

    unless ( &validipormask( $cgiparams{'DEST_IPMAC'} ) ) {
        if ( $cgiparams{'DEST_IPMAC'} ne '' ) {
            $errormessage .= "$tr{'destination ip bad'}<BR />";
        } else {
            $cgiparams{'DEST_IPMAC'} = '0.0.0.0/0';
        }
    }

    #################
    # Error checking
    #################
    unless ( $cgiparams{'PROTOCOL'} eq "6"
        or $cgiparams{'PROTOCOL'} eq "17"
        or $cgiparams{'PROTOCOL'} eq "TCP & UDP" )
    {
        unless (!( $cgiparams{'DEST_PORT'} )
            and !( $cgiparams{'NEW_DEST_PORT'} ) )
        {
            $errormessage = 'You cannot specify a port with that protocol';
        }
    }

    # Check for source/destination port being defined
    if (   ( $cgiparams{'DEST_PORT'} != 0 )
        or ( $cgiparams{'NEW_DEST_PORT'} != 0 ) )
    {
        # Check for protocol being Any
        if ( $cgiparams{'PROTOCOL'} eq 'Any' ) {
            $errormessage .=
              "$tr{'error source-destination port protocol any'}<BR />";
        }

        # Check for dest. IP/MAC being empty
        if ( $cgiparams{'DEST_IPMAC'} eq '' ) {
            $errormessage .=
              "$tr{'error source-destination port destination any'}<BR />";
        }
    }

    # Check for source and destination ports being equal
    if (    ( $cgiparams{'DEST_PORT'} != 0 )
        and ( $cgiparams{'NEW_DEST_PORT'} != 0 ) )
    {
        if ( $cgiparams{'DEST_PORT'} eq $cgiparams{'NEW_DEST_PORT'} ) {
            $cgiparams{'NEW_DEST_PORT'} = 0;
        }
    }

# Check for source port being defined if destination port is defined-error if not
    if (    ( $cgiparams{'DEST_PORT'} == 0 )
        and ( $cgiparams{'NEW_DEST_PORT'} != 0 ) )
    {
        $errormessage =
          'Source port must be defined if destination port is also defined!';
    }

    #########################################
    # Check for IP's in subnets
    #########################################
    my $defaultipobj   = new NetAddr::IP "default";
    my $localhostipobj = new NetAddr::IP "localhost";
    my $greenipobj     = new NetAddr::IP "$netsettings{'GREEN_ADDRESS'}/$netsettings{'GREEN_NETMASK'}";
    my $greengwobj    = new NetAddr::IP "$netsettings{'GREEN_ADDRESS'}";
    my $redgwobj      = $defaultipobj;
    my $orangeipobj   = $defaultipobj;
    my $orangegwobj   = $defaultipobj;
    my $purpleipobj   = $defaultipobj;
    my $purplegwobj   = $defaultipobj;
    my $srcipaddrobj  = "";
    my $destipaddrobj = "";

    if ( $redip ne '' ) {
        $redgwobj = new NetAddr::IP "$redip";
    } else {
        $redgwobj = $defaultipobj;
    }

    if ( $netsettings{'ORANGE_ADDRESS'} ne '' ) {
        $orangeipobj = new NetAddr::IP
          "$netsettings{'ORANGE_ADDRESS'}/$netsettings{'ORANGE_NETMASK'}";
        $orangegwobj = new NetAddr::IP "$netsettings{'ORANGE_ADDRESS'}";
    }

    if ( $netsettings{'PURPLE_ADDRESS'} ne '' ) {
        $purpleipobj = new NetAddr::IP
          "$netsettings{'PURPLE_ADDRESS'}/$netsettings{'PURPLE_NETMASK'}";
        $purplegwobj = new NetAddr::IP "$netsettings{'PURPLE_ADDRESS'}";
    }

    if ( &validip($ipmac) ) {
        $srcipaddrobj = new NetAddr::IP "$ipmac";

        if ( $localhostipobj eq $srcipaddrobj ) {
            $errormessage .= "$tr{'source ip bad local'}<BR />";
        }

        if ( $greengwobj eq $srcipaddrobj ) {
            $errormessage .= "$tr{'source ip bad gateway'}<BR />";
        } elsif ( !( $srcipaddrobj eq "" ) ) {
            if (   ( $orangegwobj eq $srcipaddrobj )
                or ( $purplegwobj eq $srcipaddrobj )
                or ( $redgwobj eq $srcipaddrobj ) )
            {
                $errormessage .= "$tr{'source ip bad gateway'}<BR />";
            }
        }

        if ( !( $srcipaddrobj == $defaultipobj ) ) {
            if ( $cgiparams{'SRC_IFACE'} eq $netsettings{'GREEN_DEV'} ) {
                unless ( $subcheck{'GREEN'} eq "off" ) {
                    if ( !$greenipobj->contains($srcipaddrobj) ) {
                        $errormessage .= "$tr{'source ip bad green'}<BR />";
                    }
                }
            } elsif ( ( $netsettings{'ORANGE_ADDRESS'} ne '' )
                and ( $cgiparams{'SRC_IFACE'} eq $netsettings{'ORANGE_DEV'} ) )
            {
                unless ( $subcheck{'ORANGE'} eq "off" ) {
                    if ( !$orangeipobj->contains($srcipaddrobj) ) {
                        $errormessage .= "$tr{'source ip bad orange'}<BR />";
                    }
                }
            } elsif ( ( $netsettings{'PURPLE_ADDRESS'} ne '' )
                and ( $cgiparams{'SRC_IFACE'} eq $netsettings{'PURPLE_DEV'} ) )
            {
                unless ( $subcheck{'PURPLE'} eq "off" ) {
                    if ( !$purpleipobj->contains($srcipaddrobj) ) {
                        $errormessage .= "$tr{'source ip bad purple'}<BR />";
                    }
                }
            } elsif ( ( $redip ne '' )
                and ( $cgiparams{'SRC_IFACE'} eq $reddev ) )
            {
                if (
                    ( $greenipobj->contains($srcipaddrobj) )
                    or ( ( !( $orangeipobj == $defaultipobj ) )
                        and $orangeipobj->contains($srcipaddrobj) )
                    or ( ( !( $purpleipobj == $defaultipobj ) )
                        and $purpleipobj->contains($srcipaddrobj) )
                   )
                {
                    $errormessage .= "$tr{'source ip bad red'}<BR />";
                }
            }
        }
    }

    if ( &validip( $cgiparams{'DEST_IPMAC'} ) ) {
        $destipaddrobj = new NetAddr::IP "$cgiparams{'DEST_IPMAC'}";

        if ( $localhostipobj eq $destipaddrobj ) {
            $errormessage .= "$tr{'destination ip bad local'}<BR />";
        }

        if ( $greengwobj eq $destipaddrobj ) {
            $errormessage .= "$tr{'destination ip bad gateway'}<BR />";
        } elsif ( !( $destipaddrobj eq "" ) ) {
            if (   ( $orangegwobj eq $destipaddrobj )
                or ( $purplegwobj eq $destipaddrobj )
                or ( $redgwobj eq $destipaddrobj ) )
            {
                $errormessage .= "$tr{'destination ip bad gateway'}<BR />";
            }
        }

# This code added by Steve McNeill to generate error messages from subnet checks
        if ( !( $destipaddrobj == $defaultipobj ) ) {
            if ( $cgiparams{'DEST_IFACE'} eq $netsettings{'GREEN_DEV'} ) {
                unless ( $subcheck{'GREEN'} eq "off" ) {
                    if ( !$greenipobj->contains($destipaddrobj) ) {
                        $errormessage .=
                          "$tr{'destination ip bad green'}<BR />";
                    }
                }
            } elsif ( ( $netsettings{'ORANGE_ADDRESS'} ne '' )
                and ( $cgiparams{'DEST_IFACE'} eq $netsettings{'ORANGE_DEV'} ) )
            {
                unless ( $subcheck{'ORANGE'} eq "off" ) {
                    if ( !$orangeipobj->contains($destipaddrobj) ) {
                        $errormessage .=
                          "$tr{'destination ip bad orange'}<BR />";
                    }
                }
            } elsif ( ( $netsettings{'PURPLE_ADDRESS'} ne '' )
                and ( $cgiparams{'DEST_IFACE'} eq $netsettings{'PURPLE_DEV'} ) )
            {
                unless ( $subcheck{'PURPLE'} eq "off" ) {
                    if ( !$purpleipobj->contains($destipaddrobj) ) {
                        $errormessage .=
                          "$tr{'destination ip bad purple'}<BR />";
                    }
                }
            } elsif ( ( $redip ne '' )
                and ( $cgiparams{'DEST_IFACE'} eq $reddev ) )
            {
                if (
                    ( $greenipobj->contains($destipaddrobj) )
                    or ( ( !( $orangeipobj == $defaultipobj ) )
                        and $orangeipobj->contains($destipaddrobj) )
                    or ( ( !( $purpleipobj == $defaultipobj ) )
                        and $purpleipobj->contains($destipaddrobj) )
                  )
                {
                    $errormessage .= "$tr{'destination ip bad red'}<BR />";
                }
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

        if (   ( $cgiparams{'ACTION'} eq $tr{'ffc-update'} )
            or ( $cgiparams{'ACTION'} eq $tr{'add'} ) )
        {
            my $cnt = 0;
            my ( %current, $line, $newIdx, $oldIdx, @temp );

# Read the file into assoc. array 'current', indexed by 10x the file's order numbers.
            open( FILE, "$filename" ) or die 'Unable to read config file.';
            while (<FILE>) {
                my @splt;
                chomp;
                @splt = split /,/, $_;
                $current{ 10 * $splt[0] } = $_;
            }
            close(FILE);

            if ( $cgiparams{'ORDER_NUMBER'} != $cgiparams{'OLDID'} ) {

# If an addition or update and the entry's order number changed, change its index and delete
#   (undef) the original.

                # Prepare an empty entry; it'll be filled in a bit
                $newIdx = ( 10 * $cgiparams{'ORDER_NUMBER'} ) - 1;
                $current{$newIdx} = "";

   # delete the old location only when moving an existing entry
   #   (ObviousMan: "Clearly, new entries don't have corresponding old entries."
                if ( $cgiparams{'OLDID'} != 0 ) {
                    my $oldIdx = ( 10 * $cgiparams{'OLDID'} );
                    undef $current{$oldIdx};
                }
            } else {

                # Same order, but we need $newIdx anyway.
                $newIdx = 10 * $cgiparams{'ORDER_NUMBER'};
            }

            # Now sort the keys and re-write the file in the correct order.

            if ( !open( FILE, ">$filename" ) ) {
                $error_message .= 'Unable to write config file.\n';
            } else {
                flock FILE, 2;

                # Rewrite the file, renumbering the rules from 1 to n, by 1.
                foreach $idx ( sort numerically keys %current ) {
                    next if ( !defined $current{$idx} );
                    $cnt++;

                    # Print the entry with its new order #
                    if ( $idx == $newIdx ) {

                      #$infomessage .= "New/Chg'd entry: $idx==$newIdx<br />\n";
                      # Use cgiparms for new/updated/moved entry
                      # Shovel it out
                        {
                            my $outLine = "";
                            if ( $cgiparams{'PROTOCOL'} eq "TCP & UDP" ) {
                                $outLine .= "$cnt,$cgiparams{'SRC_IFACE'},";
                                $outLine .= "$cgiparams{'SRC_IPMAC'},$cgiparams{'DEST_PORT'},";
                                $outLine .= "$cgiparams{'DEST_IFACE'},$cgiparams{'DEST_IPMAC'},";
                                $outLine .= "$cgiparams{'NEW_DEST_PORT'},6,";
                                $outLine .= "$cgiparams{'TARGET'},$cgiparams{'ENABLED'},";
                                $outLine .= "$cgiparams{'TIMED'},$cgiparams{'DESCRIPTION'},";
                                $outLine .= "$cgiparams{'ORIG_TARGET'},$cgiparams{'NEW_TARGET'},";
                                $outLine .= "$cgiparams{'TIMES_DISP'},$cgiparams{'TIMES'}";
                                print FILE "$outLine\n";

                                $outLine = "";
                                $cnt++;
                                $outLine .= "$cnt,$cgiparams{'SRC_IFACE'},";
                                $outLine .= "$cgiparams{'SRC_IPMAC'},$cgiparams{'DEST_PORT'},";
                                $outLine .= "$cgiparams{'DEST_IFACE'},$cgiparams{'DEST_IPMAC'},";
                                $outLine .= "$cgiparams{'NEW_DEST_PORT'},17,";
                                $outLine .= "$cgiparams{'TARGET'},$cgiparams{'ENABLED'},";
                                $outLine .= "$cgiparams{'TIMED'},$cgiparams{'DESCRIPTION'},";
                                $outLine .= "$cgiparams{'ORIG_TARGET'},$cgiparams{'NEW_TARGET'},";
                                $outLine .= "$cgiparams{'TIMES_DISP'},$cgiparams{'TIMES'}";
                                print FILE "$outLine\n";

                            } else {
                                $outLine .= "$cnt,$cgiparams{'SRC_IFACE'},";
                                $outLine .= "$cgiparams{'SRC_IPMAC'},$cgiparams{'DEST_PORT'},";
                                $outLine .= "$cgiparams{'DEST_IFACE'},$cgiparams{'DEST_IPMAC'},";
                                $outLine .= "$cgiparams{'NEW_DEST_PORT'},$cgiparams{'PROTOCOL'},";
                                $outLine .= "$cgiparams{'TARGET'},$cgiparams{'ENABLED'},";
                                $outLine .= "$cgiparams{'TIMED'},$cgiparams{'DESCRIPTION'},";
                                $outLine .= "$cgiparams{'ORIG_TARGET'},$cgiparams{'NEW_TARGET'},";
                                $outLine .= "$cgiparams{'TIMES_DISP'},$cgiparams{'TIMES'}";
                                print FILE "$outLine\n";
                            }
                        }
                    } else {

                        # Get the untouched entry and fission it
                        my @temp = split /,/, $current{$idx};

                        # Shovel it out
                        {
                            my $outLine = "";
                            $outLine  = "$cnt,$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],";
                            $outLine .= "$temp[6],$temp[7],$temp[8],$temp[9],$temp[10],$temp[11],";
                            $outLine .= "$temp[12],$temp[13],$temp[14],$temp[15]";
                            print FILE "$outLine\n";
                        }
                    }
                }
                close(FILE);
            }
        }

        if ( $cgiparams{'ACTION'} eq $tr{'add'} ) {
            &log( $tr{'forwarding rule added'} );
        } else {
            &log( $tr{'forwarding rule updated'} );
        }

        my $success = message('setportfw');

        unless ( defined $success ) {
            $errormessage .= "$tr{'smoothd failure'}" . "<br />\n";
        }

        unless ( $success eq 'Port forwarding rules set' ) {
            $errormessage .=
              "Error setting portforwarding rules: " . "$success" . "<br />\n";

            $cnt = 1;
            open( FILE, ">$filename" ) or die 'Unable to open config file.';
            flock FILE, 2;

            # Remove the offending line from the config file so it doesn't get
            #  listed in the UI display table
            foreach $line (@current) {
                chomp $line;
                @temp = split /,/, $line;
                unless ( $split[0] == $cgiparams{'ORDER_NUMBER'} ) {
                    if ( $split[0] > $cgiparams{'ORDER_NUMBER'} ) {
                        print FILE "$cnt,$temp[1],$temp[2],$temp[3],$temp[4],";
                        print FILE "$temp[5],$temp[6],$temp[7],$temp[8],$temp[9],";
                        print FILE "$temp[10],$temp[11],$temp[12],$temp[13],$temp[14],$temp[15]\n";
                    } else {
                        print FILE "$line\n";
                    }
                }
                $cnt++;
            }
            close FILE;
        }
    }

    $cgiparams{'ORDER_NUMBER'} = "0";
    open( FILE, "$filename" ) or die 'Unable to open config file';
    while (<FILE>) {
        $cgiparams{'ORDER_NUMBER'}++;
    }
    close FILE;

    $cgiparams{'ORDER_NUMBER'}++;
    $cgiparams{'SRC_IPMAC'}     = '';
    $cgiparams{'DEST_IPMAC'}    = '';
    $cgiparams{'SRC_IFACE'}     = $reddev;
    $cgiparams{'DEST_IFACE'}    = $netsettings{'GREEN_DEV'};
    $cgiparams{'DEST_PORT'}     = '';
    $cgiparams{'NEW_DEST_PORT'} = '';
    $cgiparams{'PROTOCOL'}      = '6';
    $cgiparams{'TARGET'}        = 'ACCEPT';
    $cgiparams{'ENABLED'}       = 'on';
    $cgiparams{'DESCRIPTION'}   = '';
    $cgiparams{'TIMED'}         = 'off';
    $cgiparams{'ORIG_TARGET'}   = '';
    $cgiparams{'NEW_TARGET'}    = '';
    $cgiparams{'TIMES_DISP'}    = '';
    $cgiparams{'TIMES'}         = '';
}

# Check for remove or edit
elsif ($cgiparams{'ACTION'} eq $tr{'remove'}
    or $cgiparams{'ACTION'} eq $tr{'edit'}
    or $cgiparams{'ACTION'} eq $tr{'ffc-enable rule'} )
{
    open( FILE, "$filename" ) or die 'Unable to open config file.';
    my @current = <FILE>;
    close(FILE);

    my $count = 0;
    my $id    = 0;
    my $line;

    foreach $line (@current) {
        $id++;

        if ( $cgiparams{$id} eq "on" ) {
            $count++;
        }
    }

    if ( $count == 0 ) {
        $errormessage = $tr{'nothing selected'};
    }

    if ( $count > 1 and $cgiparams{'ACTION'} eq $tr{'edit'} ) {
        $errormessage = $tr{'you can only select one item to edit'};
    }

    unless ($errormessage) {
        open( FILE, ">$filename" ) or die 'Unable to open config file.';
        flock FILE, 2;
        $id  = 0;
        $cnt = 1;

        foreach $line (@current) {
            $id++;

            unless ( $cgiparams{$id} eq "on" ) {
                chomp $line;
                my @temp = split /,/, $line;
                print FILE "$cnt,$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],$temp[6],$temp[7],";
                print FILE "$temp[8],$temp[9],$temp[10],$temp[11],$temp[12],$temp[13],$temp[14],$temp[15]\n";
                $cnt++;
            } elsif ( $cgiparams{'ACTION'} eq $tr{'edit'} ) {
                chomp($line);
                my @temp = split( /\,/, $line );
                $cgiparams{'ORDER_NUMBER'}  = $temp[0];
                $cgiparams{'SRC_IFACE'}     = $temp[1];
                $cgiparams{'SRC_IPMAC'}     = $temp[2];
                $cgiparams{'DEST_PORT'}     = $temp[3];
                $cgiparams{'DEST_IFACE'}    = $temp[4];
                $cgiparams{'DEST_IPMAC'}    = $temp[5];
                $cgiparams{'NEW_DEST_PORT'} = $temp[6];
                $cgiparams{'PROTOCOL'}      = $temp[7];
                $cgiparams{'TARGET'}        = $temp[8];
                $cgiparams{'ENABLED'}       = $temp[9];
                $cgiparams{'TIMED'}         = $temp[10];
                $cgiparams{'DESCRIPTION'}   = $temp[11];
                $cgiparams{'ORIG_TARGET'}   = $temp[12];
                $cgiparams{'NEW_TARGET'}    = $temp[13];
                $cgiparams{'TIMES_DISP'}    = $temp[14];
                $cgiparams{'TIMES'}         = $temp[15];

                # Editing support
                $cgiparams{'OLDID'} = $id;
                $updatebutton = 1;
                print FILE "$line\n";
                $cnt++;
            } elsif ( $cgiparams{'ACTION'} eq $tr{'ffc-enable rule'} ) {
                chomp $line;
                my @temp = split( /\,/, $line );
                $temp[0] = $cnt++;
                if ( $temp[9] eq "on" ) {
                    $enabled = "off";
                } else {
                    $enabled = "on";
                }
                print FILE "$temp[0],$temp[1],$temp[2],$temp[3],";
                print FILE "$temp[4],$temp[5],$temp[6],$temp[7],";
                print FILE "$temp[8],$enabled,$temp[10],$temp[11],";
                print FILE "$temp[12],$temp[13],$temp[14],$temp[15]\n";
            }
        }

        close(FILE);

        if ( $cgiparams{'ACTION'} eq $tr{'remove'} ) {
            $cgiparams{'ORDER_NUMBER'}  = $id;
            $cgiparams{'SRC_IPMAC'}     = '';
            $cgiparams{'DEST_IPMAC'}    = '';
            $cgiparams{'SRC_IFACE'}     = $reddev;
            $cgiparams{'DEST_IFACE'}    = $netsettings{'GREEN_DEV'};
            $cgiparams{'DEST_PORT'}     = '';
            $cgiparams{'NEW_DEST_PORT'} = '';
            $cgiparams{'PROTOCOL'}      = '6';
            $cgiparams{'TARGET'}        = 'ACCEPT';
            $cgiparams{'ENABLED'}       = 'on';
            $cgiparams{'DESCRIPTION'}   = '';

            # Implied "Don't print the removed rule"
            &log( $tr{'forwarding rule removed'} );
        } else {
            $cgiparams{'RULE_COUNT'} = $id;
        }

        my $success = message('setportfw');

        unless ( defined $success ) {
            $errormessage = "$tr{'smoothd failure'}";
        }
        unless ( $success eq 'Port forwarding rules set' ) {
            $errormessage = "Error setting portforwarding rules: " . "$success";
        }

    }
}

# Check for errormessage
if ( $errormessage ne '' ) {

    # Reset defaults for ip's/ports
    if ( $cgiparams{'SRC_IPMAC'} eq '0.0.0.0/0' ) {
        $cgiparams{'SRC_IPMAC'} = '';
    }

    if ( $cgiparams{'DEST_IPMAC'} eq '0.0.0.0/0' ) {
        $cgiparams{'DEST_IPMAC'} = '';
    }

    if ( $cgiparams{'DEST_PORT'} eq '0' ) {
        $cgiparams{'DEST_PORT'} = '';
    }

    if ( $cgiparams{'NEW_DEST_PORT'} eq '0' ) {
        $cgiparams{'NEW_DEST_PORT'} = '';
    }
}

# Get a rule count to display the next order # in the UI display
open( FILE, "$filename" ) or die 'Unable to open config file.';
while (<FILE>) { $cgiparams{'RULE_COUNT'}++; }
close(FILE);
$cgiparams{'RULE_COUNT'}++;

# Check for normal page load with defaults
if ( $cgiparams{'ACTION'} eq '' ) {
    $cgiparams{'PROTOCOL'}        = '6';
    $cgiparams{'ENABLED'}         = 'on';
    $cgiparams{'ADDRESS_ENABLED'} = 'on';

    # Support for outbound firewall
    $cgiparams{'SRC_IFACE'}    = $reddev;
    $cgiparams{'DEST_IFACE'}   = $netsettings{'GREEN_DEV'};
    $cgiparams{'TARGET'}       = 'ACCEPT';
    $cgiparams{'ORDER_NUMBER'} = $cgiparams{'RULE_COUNT'};
    $cgiparams{'DESCRIPTION'}  = '';
    $cgiparams{'TIMED'}        = 'off';
    $cgiparams{'ORIG_TARGET'}  = '';
    $cgiparams{'NEW_TARGET'}   = '';
    $cgiparams{'TIMES_DISP'}   = '';
    $cgiparams{'TIMES'}        = '';
}

my ( $tmp1, @temp );

# Network device listing
my %availablenetdevices;
my @devices = `ifconfig`;
foreach $dev (@devices) {
    chomp $dev;
    @temp = split( ' ', $dev );
    if ( $temp[1] =~ /^Link/ ) {
        if ( $temp[0] =~ /^ipsec/ or $temp[0] =~ /^lo/ ) {
            next;
        } else {
            my @deviceinfo = `ifconfig $temp[0]`;
            foreach $devinfo (@deviceinfo) {
                chomp $devinfo;
                if ( $devinfo =~ /inet addr\:(\d{1,3}(\.\d{1,3}){3})/ ) {
                    $availablenetdevices{ $temp[0] } = $1;
                }
            }
        }
    }
}

foreach $dev ( keys(%availablenetdevices) ) {
    my $tmp = grep( /^$dev$/, @filelist );
    if ( $tmp > 0 ) {
        delete( $availablenetdevices{$dev} );
    }
}

# Support for outbound firewall
$selected{'SRC_IFACE'}{''}  = '';
$selected{'DEST_IFACE'}{''} = '';

foreach $dev ( sort( keys(%availablenetdevices) ) ) {
    $selected{'SRC_IFACE'}{$dev}  = '';
    $selected{'DEST_IFACE'}{$dev} = '';
}

$selected{'SRC_IFACE'}{ $cgiparams{'SRC_IFACE'} }   = 'selected';
$selected{'DEST_IFACE'}{ $cgiparams{'DEST_IFACE'} } = 'selected';
$selected{'PROTOCOL'}{ $cgiparams{'PROTOCOL'} }     = 'selected';

$selected{'IF_ALIAS'}{'RED'}   = '';
$selected{'IF_ALIAS'}{'GREEN'} = '';
if ( $cgiparams{'ORANGE_DEV'} ) {
    $selected{'IF_ALIAS'}{'ORANGE'} = '';
}
if ( $cgiparams{'PURPLE_DEV'} ) {
    $selected{'IF_ALIAS'}{'PURPLE'} = '';
}
$selected{'IF_ALIAS'}{ $cgiparams{'IF_ALIAS'} } = 'selected';

$selected{'TARGET'}{'ACCEPT'}               = '';
$selected{'TARGET'}{'DROP'}                 = '';
$selected{'TARGET'}{'REJECT'}               = '';
$selected{'TARGET'}{'LOG'}                  = '';
$selected{'TARGET'}{ $cgiparams{'TARGET'} } = 'selected';

$checked{'ENABLED'}{'off'}                   = '';
$checked{'ENABLED'}{'on'}                    = '';
$checked{'ENABLED'}{ $cgiparams{'ENABLED'} } = 'checked';

# Protocol listing
open( TMP, "/etc/protocols" ) or die "Unable to open /etc/protocols\: $!\n";
my @protocols = <TMP>;
close(TMP);

my %availableprotocols;
open( FILE, ">/var/smoothwall/portfw/protolist" )
  or die 'Unable to write protocol list file';
foreach $line (@protocols) {
    chomp $line;

    if ( $line =~ m/^([a-z0-9]|\#\t[0-9]+\t+)/i ) {
        my @protoline = split( /\s+/, $line );
        print FILE "$protoline[1],$protoline[2]\n";
        if ( $#protoline == 3 ) {
            $protoline[3] =~ s/\b(\w)/\u\L$1/g;
            $availableprotocols{ $protoline[1] } =
              "$protoline[2] = $protoline[3]";
        } else {
            $protoline[2] =~ s/\b(\w)/\u\L$1/g;
            $availableprotocols{ $protoline[1] } = "$protoline[2]";
        }
    }
}
close FILE;

delete( $availableprotocols{'0'} );
delete( $availableprotocols{'41'} );
delete( $availableprotocols{'43'} );
delete( $availableprotocols{'44'} );
delete( $availableprotocols{'58'} );
delete( $availableprotocols{'59'} );
delete( $availableprotocols{'60'} );
delete( $availableprotocols{'253'} );
delete( $availableprotocols{'254'} );
delete( $availableprotocols{'255'} );
my @sortedprotocols = sort { $a <=> $b } keys(%availableprotocols);

&openpage( $tr{'full firewall control'}, 1, '', 'Full Firewall Control' );

&openbigbox( '100%', 'left' );

&alertbox($errormessage);

# Javascript support lines
print
"<script type='application/javascript' src='/ui/js/utility.js'></script>";

print <<END
<script>
function ffoxSelectUpdate(elmt)
{
    if(!document.all) elmt.style.cssText = elmt.options[elmt.selectedIndex].style.cssText;
}
</script>
END
  ;

# Border for debug
my $border = 0;

######################################################################
# Mod for source ip, destination ip, and destination port edit display
######################################################################
if ( $cgiparams{'SRC_IPMAC'} eq '0.0.0.0/0' ) {
    $sourceipmac = '';
} else {
    $sourceipmac = $cgiparams{'SRC_IPMAC'};
}

if ( $cgiparams{'DEST_IPMAC'} eq '0.0.0.0/0' ) {
    $destinationipmac = '';
} else {
    $destinationipmac = $cgiparams{'DEST_IPMAC'};
}

if ( $cgiparams{'DEST_PORT'} eq '0' ) {
    $sourceport = '';
} else {
    $sourceport = $cgiparams{'DEST_PORT'};
}

if ( $cgiparams{'NEW_DEST_PORT'} eq '0' ) {
    $destinationport = '';
} else {
    $destinationport = $cgiparams{'NEW_DEST_PORT'};
}

if ($updatebutton) {
    $buttontext = $tr{'ffc-update'};
    $boxtext    = $tr{'update current rule'};
} else {
    $buttontext = $tr{'add'};
    $boxtext    = $tr{'add a new rule'};
}

print <<END
<style type="text/css">
option.red{color:red;}
option.green{color:green;}
option.orange{color:orange;}
option.purple{color:purple;}
</style>
END
  ;

##############################################

print "<form method='POST' name='FIREWALL'>\n";
print "
<table width='100%' class='box' style='margin-top:8pt; margin-bottom:8pt'>
   <tr>
     <td align='center' class='boldbase'>
       <b>$tr{'ffc section'}</b>
     </td>
   </tr>
";

# Check for running interfaces
open( FILE, "$aliasfile" ) or die 'Unable to open aliases file';
my @aliases = <FILE>;
close FILE;

open( FILE, ">$aliasfile" ) or die 'Unable to open config file.';
foreach $line (@aliases) {
    chomp $line;
    @temp = split /,/, $line ;
    my $alias = $temp[1];
    my $running;
    my @ifconfig = `/sbin/ifconfig $alias`;
    foreach my $line2 (@ifconfig) {
        if ( $line2 =~ /inet/ ) {
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
  if ( $selected{'SRC_IFACE'}{ $netsettings{'GREEN_DEV'} } eq 'selected' );

$SRC_COLOR = 'orange'
  if ( $selected{'SRC_IFACE'}{ $netsettings{'ORANGE_DEV'} } eq 'selected' );

$SRC_COLOR = 'purple'
  if ( $selected{'SRC_IFACE'}{ $netsettings{'PURPLE_DEV'} } eq 'selected' );

$SRC_COLOR = 'red'
  if ( $selected{'SRC_IFACE'}{$reddev} eq 'selected' );

# Extract the initial iface color for DEST
my $DEST_COLOR = 'black';
$DEST_COLOR = 'green'
  if ( $selected{'DEST_IFACE'}{ $netsettings{'GREEN_DEV'} } eq 'selected' );

$DEST_COLOR = 'orange'
  if ( $selected{'DEST_IFACE'}{ $netsettings{'ORANGE_DEV'} } eq 'selected' );

$DEST_COLOR = 'purple'
  if ( $selected{'DEST_IFACE'}{ $netsettings{'PURPLE_DEV'} } eq 'selected' );

$DEST_COLOR = 'red'
  if ( $selected{'DEST_IFACE'}{$reddev} eq 'selected' );

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
if ( open( FILE, "$aliasfile" ) ) {
  @temp3 = <FILE>;
  close FILE;
}

foreach $dev ( sort( keys(%availablenetdevices) ) ) {
  $dev =~ /(\:\d{1,3})/;
  my $devifacesub = $1;
  my $redaliasip;

  foreach $line (@temp3) {
    chomp $line;
    @split = split( /\,/, $line );
    if ( $split[0] eq "RED$devifacesub" and $dev =~ /:/ ) {
      $redaliasip = $split[3];
    }
  }

  if ( $netsettings{'GREEN_DEV'} and ( $dev =~ /$netsettings{'GREEN_DEV'}/ ) ) {
    print "         <option style='color: green' value='$dev' $selected{'SRC_IFACE'}{$dev}>
                      GREEN$devifacesub - $dev
                    </option>";
  } elsif ( $netsettings{'ORANGE_DEV'} and ( $dev =~ /$netsettings{'ORANGE_DEV'}/ ) ) {
    print "         <option style='color: orange' value='$dev' $selected{'SRC_IFACE'}{$dev}>
                      ORANGE$devifacesub - $dev
                    </option>";
  } elsif ( $netsettings{'PURPLE_DEV'} and ( $dev =~ /$netsettings{'PURPLE_DEV'}/ ) ) {
    print "         <option style='color: purple' value='$dev' $selected{'SRC_IFACE'}{$dev}>
                      PURPLE$devifacesub - $dev
                    </option>";
  } elsif ( $reddev and ( $dev =~ /$reddev/ ) ) {
    print "         <option style='color: red' value='$dev' $selected{'SRC_IFACE'}{$dev}>\n";
    if ($redaliasip) 
    {
      print "         RED$devifacesub $redaliasip\n";
    } 
    else 
    {
      print "         RED$devifacesub - $dev\n";
    }
      print "        </option>\n";

  } else {
    print "    <option style='color: black' value='$dev' $selected{'SRC_IFACE'}{$dev}>\n";
    print "      $dev\n";
    print "    </option>\n";
  }
}

print "      </select>
    </td>
    <td class='base' width='25%'>
      $tr{'destination ifacec'}
    </td>
    <td width='25%'>
      <select style='color: $DEST_COLOR' onchange='ffoxSelectUpdate(this);' name='DEST_IFACE'>
        <option style='color: black' value='any' $selected{'DEST_IFACE'}{'any'}>
          Any
        </option>
";

foreach $dev ( sort( keys(%availablenetdevices) ) ) {
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
      $tr{'destination ippfc'}
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
      $tr{'destination portc'}
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
          <td class='base'>$tr{'order number'}</td>
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
 	     <input type='hidden' name='TIMES_DISP' value='$cgiparams{'TIMES_DISP'}'>
            <input type='hidden' name='TIMED' value='$cgiparams{'TIMED'}'>
 	     <input type='hidden' name='TIMES' value='$cgiparams{'TIMES'}'>
 	     <input type='hidden' name='ORIG_TARGET' value='$cgiparams{'ORIG_TARGET'}'>
 	     <input type='hidden' name='NEW_TARGET' value='$cgiparams{'NEW_TARGET'}'>
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

&openbox( $tr{'current rules'} );

my $portmap     = &Stans::modlib::portsmap();
my $protocolmap = &Stans::modlib::protocolmap();
my $ifcolorsmap = &Stans::modlib::ifcolormap();

my %render_settings = (
    'url' => "/cgi-bin/portfw.cgi?[%COL%],[%ORD%],$cgiparams{'COLUMN'},$cgiparams{'ORDER'}",
    'columns' => [
        {
            column => '1',
            title  => 'Order',
            size   => 10,
            sort   => '<=>',
        },
        {
            column => '2',
            title  => 'Src Dev',
            size   => 15,
            sort   => 'cmp',
            tr     => \%{$ifcolorsmap},
        },
        {
            column => '3',
            title  => 'Source IP/MAC',
            size   => 15,
            sort   => 'cmp',
            tr     => { '0.0.0.0/0' => 'Any' },
        },
        {
            column => '4',
            title  => 'Dest Port',
            size   => 10,
            sort   => 'cmp',
            tr     => \%{$portmap},
        },
        {
            column => '5',
            title  => 'Dest Dev',
            size   => 15,
            sort   => 'cmp',
            tr     => \%{$ifcolorsmap},
        },
        {
            column => '6',
            title  => 'Destination IP',
            size   => 15,
            sort   => 'cmp',
            tr     => { '0.0.0.0/0' => 'Any' },
        },
        {
            column => '7',
            title  => 'New Dest Port',
            size   => 10,
            sort   => 'cmp',
            tr     => \%{$portmap},
        },
        {
            column => '8',
            title  => 'Protocol',
            size   => 10,
            sort   => 'cmp',
            tr     => \%{$protocolmap},
        },
        {
            column => '9',
            title  => "$tr{'ffc-targetc'}",
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
            column => '11',
            title  => 'Timed',
            size   => 10,
            tr     => 'onoff',
            align  => 'center',
        },
        {
            title => "$tr{'mark'}",
            size  => 5,
            mark  => ' ',
        },
        {
            column => '12',
            title  => "$tr{'comment'}",
            break  => 'line',
        },
        {
            column => '15',
            title  => "Time frames",
            break  => 'line',
        }
    ]
);

&dispaliastab( $filename, \%render_settings, $cgiparams{'ORDER'}, $cgiparams{'COLUMN'} );

print <<END

<table width='100%' margin:6pt 0' border='$border'>
<tr>
	<td width='33%' align='center'><input type='SUBMIT' name='ACTION' value='$tr{'remove'}' onClick="if(confirm('You are about to completely remove port forwarding rules. Are you sure you want to do this?')) {return true;} return false;"></td>
	<td width='34%' align='center'><input type='SUBMIT' name='ACTION' value='$tr{'ffc-enable rule'}'></td>
	<td width='33%' align='center'><input type='SUBMIT' name='ACTION' value='$tr{'edit'}'></td>
</tr>
</table>
END
  ;
&closebox();

&openbox();
print <<END
</form>

END
  ;

&closebox();

&alertbox( 'add', 'add' );

&closebigbox();

&closepage();
