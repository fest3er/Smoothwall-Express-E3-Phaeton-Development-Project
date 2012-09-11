#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# Multiple IPs on Red mod for SWE 2.0 originally done by TobyLL
#  updated for SWE 3.0 and integrated with the Full Firewall Control mod
#  by Stanford T. Prescott MD
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
      or die 'Unable to open local-ipaddress file.';
    $redip = <FILE>;
    chomp $redip;
    close(FILE);
}

my ( %cgiparams, %selected, %checked );
my $filename = "${swroot}/portfw/config";

$cgiparams{'GREEN'}  = 'off';
$cgiparams{'ORANGE'} = 'off';
$cgiparams{'PURPLE'} = 'off';

&showhttpheaders();

$cgiparams{'COLUMN_ONE'} = 1;
$cgiparams{'ORDER_ONE'}  = $tr{'log ascending'};

$cgiparams{'COLUMN_TWO'} = 1;
$cgiparams{'ORDER_TWO'}  = $tr{'log ascending'};

if ( $ENV{'QUERY_STRING'} and ( not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ) )
{
    my @temp = split( ',', $ENV{'QUERY_STRING'} ); 
    $cgiparams{'ORDER_ONE'} = $temp[1] if ( defined $temp[1] and $temp[1] ne "" );
    $cgiparams{'COLUMN_ONE'} = $temp[0] if ( defined $temp[0] and $temp[0] ne "" );
    $cgiparams{'ORDER_TWO'} = $temp[3] if ( defined $temp[3] and $temp[1] ne "" );
    $cgiparams{'COLUMN_TWO'} = $temp[2] if ( defined $temp[2] and $temp[0] ne "" );
}

$cgiparams{'OLDID'}           = 0;
$cgiparams{'ENABLED'}         = 'off';
$cgiparams{'ADDRESS_ENABLED'} = 'off';

$cgiparams{'RULE_COUNT'}  = 0;
$cgiparams{'DESCRIPTION'} = '';
$cgiparams{'NETMASK'}     = '255.255.255.255';

&getcgihash( \%cgiparams );

my $errormessage = '';
my $updatebutton = 0;

if ( $cgiparams{'ACTION'} eq '' ) {
    $cgiparams{'GREEN'}           = 'on';
    $cgiparams{'ORANGE'}          = 'on';
    $cgiparams{'PURPLE'}          = 'on';
    $cgiparams{'ADDRESS_ENABLED'} = "on";
}

if (
    defined $cgiparams{'ACTION'}
    and (  $cgiparams{'ACTION'} eq $tr{'ffc-multi add'}
        or $cgiparams{'ACTION'} eq $tr{'multiip-update'} )
   )
{
    unless ( &validip( $cgiparams{'ADDRESS'} ) ) {
        $errormessage .= "$tr{'multi-ip invalid'} $tr{'ip address'}<b />";
    }
    unless ( &validipormask( $cgiparams{'NETMASK'} ) ) {
        $errormessage .= "$tr{'multi-ip invalid'} $tr{'multi-ip netmask'}<b />";
    }
    unless ( $cgiparams{'INTADDRESS'} eq '' ) {
        unless ( &validip( $cgiparams{'INTADDRESS'} ) ) {
            $errormessage .= "$tr{'multi-ip invalid'} $tr{'ip address'}<b />";
        }
    }
    unless ($errormessage) {
        my ( $line, $coloralias, $ifalias, $ifreal, $enabled );
        my (@aliases);
        my $count = 0;

        # Calculate CIDR notation and network block size
        my $aladd   = $cgiparams{'ADDRESS'};
        my $netmask = $cgiparams{'NETMASK'};
        my $intadd  = $cgiparams{'INTADDRESS'};

        if ( open( FILE, "$aliasfile" ) ) {
            @aliases = <FILE>;
            close FILE;
        }

        # count the number of aliases in the file
        foreach $line (@aliases) {
            if ( $line =~ /^RED:/ ) {
                $count++;
            }
        }

        my $broadcast = "$aladd";
        if ( open( FILE, "$aliasfile" ) ) {
            @aliases = <FILE>;
            close FILE;
        }

        # Check for update action first (editing an alias)
        my $noupdate = 1;
        if ( $cgiparams{'ACTION'} eq $tr{'multiip-update'} ) {
            if ( open( FILE, ">$aliasfile" ) ) {
                flock FILE, 2;
                foreach $line (@aliases) {
                    chomp $line;
                    @split = split( /\,/, $line );
                    unless ( $cgiparams{'OLDALIAS'} eq $split[0] ) {
                        print FILE "$line\n";
                    } else {
                        print FILE "$split[0],$split[1],$split[2],$aladd,";
                        print FILE "$netmask,$broadcast,$split[6],$split[7],";
                        print FILE "$cgiparams{'COMMENT'},$intadd\n";
                    }
                }
                close FILE;
            }
            $noupdate = 0;
        }

        # Adding an alias ---
        if ($noupdate) {
            open( FILE, ">>$aliasfile" ) or die 'Unable to open';
            flock FILE, 2;
            $count++;
            $coloralias = "RED:$count";
            $ifalias    = "$reddev:$count";
            $ifreal     = "$reddev";
            $enabled    = "$cgiparams{'ADDRESS_ENABLED'}";
            print FILE "$coloralias,$ifalias,$ifreal,$aladd,";
            print FILE "$netmask,$broadcast,yes,$enabled,";
            print FILE "$cgiparams{'COMMENT'},$intadd\n";
            close FILE;
        }

        my $success = message('ifaliasup');

        unless ( defined $success ) {
            $errormessage .= "$tr{'smoothd failure'}<b />";
        }
        unless ( $success eq 'Successfully brought up alias interfaces.' ) {
            $errormessage .= "Error bringing up interfaces: $success<b />";
        }

        &log( $tr{'multi-ip ip address added or updated'} );

        my $success = message('setportfw');

        unless ( defined $success ) {
            $errormessage .= "$tr{'smoothd failure'}<b />";
        }
        unless ( $success eq 'Port forwarding rules set' ) {
            $errormessage .= "Error setting portforwarding rules: $success<b />";
        }
    }
  EXIT:
    $cgiparams{'ADDRESS'}         = '';
    $cgiparams{'OLDALIAS'}        = '';
    $cgiparams{'INTADDRESS'}      = '';
    $cgiparams{'ADDRESS_ENABLED'} = 'on';
}

if (
    defined $cgiparams{'ACTION'}
    and (  $cgiparams{'ACTION'} eq $tr{'multi-ip remove'}
        or $cgiparams{'ACTION'} eq $tr{'multi-ip edit'}
        or $cgiparams{'ACTION'} eq $tr{'multi-ip enable alias'} )
   )
{
    my ( $count, $count2, $id, $line, $line2, @split );

    open( FILE, "$aliasfile" ) or die 'Unable to open aliases file.';
    my @current = <FILE>;
    close(FILE);

    $count = 0;
    $id    = 0;
    foreach $line (@current) {
        $id++;
        if ( $cgiparams{$id} eq "on" ) {
            $count++;
            @split = split( /\,/, $line );
            if (   ( $split[0] eq "RED" )
                or ( $split[0] eq "GREEN" )
                or ( $split[0] eq "ORANGE" )
                or ( $split[0] eq "PURPLE" ) )
            {
                $errormessage .= "$tr{'multi-ip iface error'}<b />";
            }
        }
    }

    if ( $count == 0 ) {
        $errormessage .= "$tr{'nothing selected'}<b />";
    }
    if ( $count > 1 && $cgiparams{'ACTION'} eq $tr{'edit'} ) {
        $errormessage .= "$tr{'you can only select one item to edit'}<b />";
    }

    unless ($errormessage) {

        # Bring down the aliases first
        my $success = message('ifaliasdown');

        unless ( defined $success ) {
            $errormessage .= "Unable to bring down interface aliases!<b />";
        }
        unless ( $success eq 'Successfully brought down alias interfaces.' ) {
            $errormessage .= "Error bringing interfaces down: $success<b />";
        }

        $id = 0;
        my $count2  = 0;
        my $pfwflag = 0;

        open( FILE, ">$aliasfile" ) or die 'Unable to open aliases file.';
        flock FILE, 2;
        foreach $line (@current) {
            $id++;
            chomp($line);
            my @temp = split( /\,/, $line );
            unless ( $cgiparams{$id} eq "on" ) {
                print FILE "$line\n";
            } elsif ( $cgiparams{'ACTION'} eq $tr{'multi-ip edit'} ) {
                $cgiparams{'OLDALIAS'}        = $temp[0];
                $cgiparams{'ADDRESS'}         = $temp[3];
                $cgiparams{'NETMASK'}         = '255.255.255.255';
                $cgiparams{'ADDRESS_ENABLED'} = $temp[6];
                $cgiparams{'COMMENT'}         = $temp[8];
                $cgiparams{'INTADDRESS'}      = $temp[9];

                # Editing support
                $aliasupdbutton = 1;
                print FILE "$line\n";

       # Disable any forwarding rules associated with the edited interface alias
                open( TEMP, "$filename" )
                  or die 'Unable to open portfw config file';
                my @temp2 = <TEMP>;
                close TEMP;

                open( TEMP, ">$filename" ) or die 'Unable to open config file';
                foreach $line2 (@temp2) {
                    $count2++;
                    chomp $line2;
                    @split = split( /\,/, $line2 );
                    unless ( ( $temp[1] eq $split[1] )
                        or ( $temp[1] eq $split[4] ) )
                    {
                        print TEMP "$count2,$split[1],$split[2],$split[3],$split[4],";
                        print TEMP "$split[5],$split[6],$split[7],$split[8],$split[9],";
                        print TEMP "$split[10],$split[11],$split[12],$split[13],$split[14],$split[15]\n";
                    } else {
                        $pfwflag++;
                        print TEMP "$count2,$split[1],$split[2],$split[3],$split[4],";
                        print TEMP "$split[5],$split[6],$split[7],$split[8],$split[9],";
                        print TEMP "$split[10],$split[11],$split[12],$split[13],$split[14],$split[15]\n";
                    }
                }
                close TEMP;
            } elsif ( $cgiparams{'ACTION'} eq $tr{'multi-ip remove'} ) {

       # Remove any forwarding rules associated with the deleted interface alias
                open( TEMP, "$filename" )
                  or die 'Unable to open portfw config file';
                my @temp2 = <TEMP>;
                close TEMP;

                open( TEMP, ">$filename" ) or die 'Unable to open config file';
                foreach $line2 (@temp2) {
                    $count2++;
                    chomp $line2;
                    @split = split( /\,/, $line2 );
                    unless ( ( $temp[1] eq $split[1] )
                        or ( $temp[1] eq $split[4] ) )
                    {
                        print TEMP "$count2,$split[1],$split[2],$split[3],$split[4],";
                        print TEMP "$split[5],$split[6],$split[7],$split[8],$split[9],";
                        print TEMP "$split[10],$split[11],$split[12],$split[13],$split[14],$split[15]\n";
                    } else {
                        $pfwflag++;
                    }
                }
                close TEMP;
            } elsif ( $cgiparams{'ACTION'} eq $tr{'multi-ip enable alias'} ) {
                if ( $temp[7] eq "on" ) {
                    $enabled = "off";
                    my $running = "off";
                } else {
                    $enabled = "on";
                    my $running = "on";
                }
                print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],";
                print FILE "$running,$enabled,$temp[8],$temp[9],$temp[10],$temp[11],";
                print FILE "$temp[12],$temp[13],$temp[14],$temp[15]\n";
            }
        }
        close FILE;

        # Reorder the aliases
        open( FILE, "$aliasfile" ) or die 'Unable to open aliases file.';
        flock FILE, 2;
        @current = <FILE>;
        close FILE;

        open( FILE, ">$aliasfile" ) or die 'Unable to open aliases file.';
        flock FILE, 2;
        $count = 0;
        foreach $line (@current) {
            chomp $line;
            @split = split( /\,/, $line );
            my $alias    = $split[1];
            my $reth     = $split[2];
            my $raddress = $split[3];
            my $rnet     = $split[4];
            my $rbroad   = $split[5];
            my $rrunning = $split[6];
            my $renabled = $split[7];
            if ( $split[0] =~ /^RED:/ ) {
                $count++;
                print FILE "RED:$count,$reddev:$count,$reth,$raddress,$rnet,";
                print FILE "$rbroad,$rrunning,$renabled,$split[8],$split[9],$temp[10]\n";

                # Check for alias in portfw config file
                my $pfalias = "$reddev:$count";
                open( TEMP, "$filename" ) or die 'Unable to open';
                flock TEMP, 2;
                @temp = <TEMP>;
                close TEMP;

                open( TEMP, ">$filename" ) or die 'Unable to open';
                flock TEMP, 2;
                foreach $line2 (@temp) {
                    chomp $line2;
                    my @current2 = split( /\,/, $line2 );
                    my $temp0    = $current2[0];
                    my $temp1    = $current2[1];
                    my $temp2    = $current2[2];
                    my $temp3    = $current2[3];
                    my $temp4    = $current2[4];
                    my $temp5    = $current2[5];
                    my $temp6    = $current2[6];
                    my $temp7    = $current2[7];
                    my $temp8    = $current2[8];
                    my $temp9    = $current2[9];
                    my $temp10   = $current2[10];
                    my $temp11   = $current2[11];
                    my $temp12   = $current2[12];
                    my $temp13   = $current2[13];
                    my $temp14   = $current2[14];
                    my $temp15   = $current2[15];

                    if ( $alias eq $temp1 ) {
                        $pfwflag++;
                        print TEMP "$temp0,$pfalias,$temp2,$temp3,$temp4,";
                        print TEMP "$temp5,$temp6,$temp7,$temp8,$temp9,$temp10,";
                        print TEMP "$temp11,$temp12,$temp13,$temp14,$temp15\n";
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

        my $success = message('ifaliasup');

        unless ( defined $success ) {
            $errormessage .= "Unable to bring up interface alias!<b />";
        }
        unless ( $success eq 'Successfully brought up alias interfaces.' ) {
            $errormessage .= "Error bringing up interfaces: $success<b />";
        }

        my $success = message('setportfw');

        unless ( defined $success ) {
            $errormessage .= "$tr{'smoothd failure'}<b />";
        }

        unless ( $success eq 'Port forwarding rules set' ) {
            $errormessage .= "Error setting portforwarding rules: $success<b />";
        }
    }

# Cheat to reorder port forwarding rules since I'm too lazy to figure out why it's not ordering them correctly
    my $count1 = 0;
    open( FILE, "$filename" ) or die 'Unable to open port forward config file';
    @temp = <FILE>;
    close FILE;

    open( FILE, ">$filename" ) or die 'Unable to open config file';
    flock FILE, 2;
    foreach $line (@temp) {
        $count1++;
        chomp $line;
        @split = split( /\,/, $line );
        print FILE "$count1,$split[1],$split[2],$split[3],$split[4],$split[5],";
        print FILE "$split[6],$split[7],$split[8],$split[9],$split[10],$split[11],";
        print FILE "$split[12],$split[13],$split[14],$split[15]\n";
    }
    close FILE;
}

open( FILE, "$filename" ) or die 'Unable to open config file.';
while (<FILE>) { $cgiparams{'RULE_COUNT'}++; }
close(FILE);
$cgiparams{'RULE_COUNT'}++;

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

$checked{'ADDRESS_ENABLED'}{'off'}                           = '';
$checked{'ADDRESS_ENABLED'}{'on'}                            = '';
$checked{'ADDRESS_ENABLED'}{ $cgiparams{'ADDRESS_ENABLED'} } = 'checked';

&openpage( $tr{'full firewall control'}, 1, '', 'multiple IPs' );

&openbigbox( '100%', 'left' );

&alertbox($errormessage);

# Javascript support lines
print "<script language='javascript' src='/ui/js/utility.js'></script>";

print <<END
<script>
function ffoxSelectUpdate(elmt)
{
        if(!document.all) elmt.style.cssText =
        elmt.options[elmt.selectedIndex].style.cssText;
}
</script>
END
  ;

# Border for debug
my $border = 0;

if ($aliasupdbutton) {
    $aliasbtntext = $tr{'multiip-update'};
    $aliasboxtext = $tr{'multiip-update current alias'};
} else {
    $aliasbtntext = $tr{'ffc-multi add'};
    $aliasboxtext = $tr{'add new alias'};
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

# If there are no interfaces in the aliasfile then add them now

# Update interfaces with normal page load
open(FILE, "$aliasfile") or die 'Unable to open alias file';
@aliases = <FILE>;
close FILE;

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
open( FILE, "$aliasfile" ) or die 'Unable to open';
my @aliases = <FILE>;
close FILE;

open( FILE, ">$aliasfile" ) or die 'Unable to open';
foreach $line (@aliases) {
    chomp $line;
    @temp = split( /\,/, $line );
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
    print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$temp[4],$temp[5],";
    print FILE "$running,$temp[7],$temp[8],$temp[9]\n";
}
close FILE;

print "<form method='POST'>\n";

print <<END

<table width='100%' class='box' style='margin-top:8pt; margin-bottom:8pt'>
  <tr>
	<td align='center' class='bolbdbase'><B>$tr{'multi-ip section'}</B></td>
  </tr>
</table>
END
  ;

&openbox($aliasboxtext);
print <<END

<table style='width:100%; margin:6pt 0' border='$border'>
  <tr>
	<td class='base'>Red Alias IP:</td>
	<td><input type='text' name='ADDRESS' value='$cgiparams{'ADDRESS'}' size=15' id='ifaddress' @{[jsvalidipormask('ifaddress','true')]}></td>
	<td><input type='hidden' name='NETMASK' value='$cgiparams{'NETMASK'}' size='15' id='ifnetmask' @{[jsvalidipormask('ifnetmask','true')]}></td>
	<td class='base'><img src='/ui/img/blob.gif' valign='top'>Internal IP:</td>
	<td><input type='text' name='INTADDRESS' value='$cgiparams{'INTADDRESS'}' size='15' id='ifintaddress' @{[jsvalidip('ifintaddress','true')]}></td>
  </tr>
</table>

<table width='100%'>
  <tr>
	<td class='base' colspan='3' nowrap='nowrap'>$tr{'descriptionc'} <input type='text' name='COMMENT' size='80' value='$cgiparams{'COMMENT'}' id='comment' @{[jsvalidcomment('comment')]}></td>
  </tr>
  <tr>
	<td class='base'>$tr{'enabled'}<input type='checkbox' name='ADDRESS_ENABLED' $checked{'ADDRESS_ENABLED'}{'on'}></td>
	<td align='center'><input type='SUBMIT' name='ACTION' value='$aliasbtntext'><input type='hidden' name='OLDALIAS' value='$cgiparams{'OLDALIAS'}'></td>
  </tr>
  </table>

<table style='width:100%; margin:6pt 0' border='$border'>
  <tr>
	<td align='left'><img src='/ui/img/blob.gif' valign='top'>Enter an internal LAN IP address to associate with a specific red alias address, if needed. Otherwise leave blank.</td>
  </tr>
</table>
END
  ;

&closebox();

&openbox( $tr{'multi-ip current ip addresses'} );
print "<font class='base'>$tr{'multi-ip device note'}</font><BR>";

my $ifcolorsmap = &Stans::modlib::ifcolormap();

my %render_settings = (
    'url' => "/cgi-bin/multiips.cgi?[%COL%],[%ORD%],$cgiparams{'COLUMN_TWO'},$cgiparams{'ORDER_TWO'}",
    'columns' => [
        {
            column => '2',
            title  => 'Interface',
            size   => 15,
            sort   => 'cmp',
            tr     => \%{$ifcolorsmap},
        },
        {
            column => '4',
            title  => 'Alias IP Address',
            size   => 20,
            sort   => 'cmp',
        },
        {
            column => '10',
            title  => 'Internal IP Address',
            size   => 20,
            sort   => 'cmp',
            tr     => { '' => 'N/A' },
        },
        {
            column => '7',
            title  => 'Running',
            size   => 5,
            tr     => 'onoff',
            align  => 'center',
        },
        {
            column => '8',
            title  => "$tr{'multi-ip enabled'}",
            size   => 5,
            tr     => 'onoff',
            align  => 'center',
        },
        {
            title => "$tr{'mark'}",
            size  => 5,
            mark  => ' ',
        },
        {
            column => '9',
            title  => "$tr{'comment'}",
            break  => 'line',
        }
    ]
);

&dispaliastab( $aliasfile, \%render_settings, $cgiparams{'ORDER_ONE'}, $cgiparams{'COLUMN_ONE'} );

print <<END

<table width='100%'>
<tr>
	<td width='33%' align='center'><input type='SUBMIT' name='ACTION' value='$tr{'multi-ip remove'}' onClick="if(confirm('$tr{'multiip-validate'}')) {return true;} return false;" ></td>
	<td width='34%' align='center'><input type='SUBMIT' name='ACTION' value='$tr{'multi-ip enable alias'}'></td>
	<td width='33%' align='center'><input type='SUBMIT' name='ACTION' value='$tr{'multi-ip edit'}'></td>
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
