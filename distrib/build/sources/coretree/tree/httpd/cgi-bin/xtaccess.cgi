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
use modlib qw( :standard );
use Socket;

my ( %cgiparams, %netsettings, %checked, %selected );

&readhash( "${swroot}/ethernet/settings", \%netsettings );

my $filename  = "${swroot}/xtaccess/config";
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
my $border = 0;

&showhttpheaders();

$cgiparams{'ENABLED'} = 'off';
$cgiparams{'COLUMN'}  = 1;
$cgiparams{'ORDER'}   = $tr{'log ascending'};

&getcgihash( \%cgiparams );

if ( $ENV{'QUERY_STRING'}
    and ( not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ) )
{
    my @temp = split( ',', $ENV{'QUERY_STRING'} );
    $cgiparams{'ORDER'}  = $temp[1] if ( defined $temp[1] and $temp[1] ne "" );
    $cgiparams{'COLUMN'} = $temp[0] if ( defined $temp[0] and $temp[0] ne "" );
}

my $errormessage = '';

if ( $cgiparams{'ACTION'} eq $tr{'add'} ) {
    unless ( &validipormask( $cgiparams{'EXT'} ) ) {
        if ( $cgiparams{'EXT'} ne '' ) {
            $errormessage = $tr{'source ip bad'};
        } else {
            $cgiparams{'EXT'} = '0.0.0.0/0';
        }
    }

    unless ( &validcomment( $cgiparams{'COMMENT'} ) ) {
        $errormessage = $tr{'invalid comment'};
    }
    if ( $cgiparams{'PROTOCOL'} eq "6" or $cgiparams{'PROTOCOL'} eq "17" ) {
        unless ( &validportrange( $cgiparams{'DEST_PORT'} ) ) {
            $errormessage = $tr{'destination port numbers'};
        }
    }
    unless ( $cgiparams{'PROTOCOL'} eq "6"
        or $cgiparams{'PROTOCOL'} eq "17"
        or $cgiparams{'PROTOCOL'} eq "TCP & UDP" )
    {
        if ( $cgiparams{'DEST_PORT'} ) {
            $errormessage = $tr{'ffc-port and protocol'};
        }
    }
    open( FILE, $filename ) or die 'Unable to open config file.';
    my @current = <FILE>;
    close(FILE);
    unless ($errormessage) {
        if ( $cgiparams{'DEST_PORT'} eq "" ) { $cgiparams{'DEST_PORT'} = '0'; }
        open( FILE, ">>$filename" ) or die 'Unable to open config file.';
        flock FILE, 2;
        if ( $cgiparams{'PROTOCOL'} eq "TCP & UDP" ) {
            print FILE "$cgiparams{'RED_IFACE'},6,$cgiparams{'EXT'},";
            print FILE "$cgiparams{'DEST_PORT'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
            print FILE "$cgiparams{'RED_IFACE'},17,$cgiparams{'EXT'},";
            print FILE "$cgiparams{'DEST_PORT'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
        } else {
            print FILE "$cgiparams{'RED_IFACE'},$cgiparams{'PROTOCOL'},$cgiparams{'EXT'},";
            print FILE "$cgiparams{'DEST_PORT'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
        }
        close(FILE);

        open( FILE, "$filename" );
        my @un_sorted = <FILE>;
        close FILE;

        @sorted = sort { $a cmp $b } @un_sorted;

        open( FILE, ">$filename" ) or die 'Unable to open config file.';
        foreach $line (@sorted) {
            print FILE "$line";
        }
        close FILE;

        undef %cgiparams;
        $cgiparams{'COLUMN'} = 1;
        $cgiparams{'ORDER'}  = $tr{'log ascending'};

        &log( $tr{'external access rule added'} );

        my $success = message('setxtaccess');

        unless ( defined $success ) {
            $errormessage = $tr{'smoothd failure'};
        }
        unless ( $success eq 'External Access Rules set' ) {
            $errormessage =
              "Error setting external access rules: " . "$success";
        }

        if ( not defined $success ) {
            $errormessage = $tr{'smoothd failure'};
        }
    }
}
if (   $cgiparams{'ACTION'} eq $tr{'remove'}
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
        my $id = 0;
        foreach $line (@current) {
            $id++;
            unless ( $cgiparams{$id} eq "on" ) {
                print FILE "$line";
            } elsif ( $cgiparams{'ACTION'} eq $tr{'edit'} ) {
                chomp($line);
                my @temp = split( /\,/, $line );
                $cgiparams{'RED_IFACE'} = $temp[0];
                $cgiparams{'PROTOCOL'}  = $temp[1];
                $cgiparams{'EXT'}       = $temp[2];
                $cgiparams{'DEST_PORT'} = $temp[3];
                $cgiparams{'ENABLED'}   = $temp[4];
                $cgiparams{'COMMENT'}   = $temp[5];
            } elsif ( $cgiparams{'ACTION'} eq $tr{'ffc-enable rule'} ) {
                chomp $line;
                my @temp = split( /\,/, $line );
                if ( $temp[4] eq "on" ) {
                    $enabled = "off";
                } else {
                    $enabled = "on";
                }
                print FILE
                  "$temp[0],$temp[1],$temp[2],$temp[3],$enabled,$temp[5]\n";
            }
        }
        close(FILE);

        &log( $tr{'external access rule removed'} );

        my $success = message('setxtaccess');

        unless ( defined $success ) {
            $errormessage = $tr{'smoothd failure'};
        }
        unless ( $success eq 'External Access Rules set' ) {
            $errormessage =
              "Error setting external access rules: " . "$success";
        }
        if ( not defined $success ) {
            $errormessage = $tr{'smoothd failure'};
        }
    }
}
if ( $cgiparams{'ACTION'} eq '' ) {
    $cgiparams{'PROTOCOL'}  = '6';
    $cgiparams{'ENABLED'}   = 'on';
    $cgiparams{'RED_IFACE'} = $reddev;
}

my ( $tmp1, @temp );

# Network device listing
my %availablenetdevices;
my @devices = `/sbin/ifconfig`;
foreach $dev (@devices) {
    chomp $dev;
    @temp = split( ' ', $dev );
    if ( $temp[1] =~ /^Link/ ) {
        if ( $temp[0] =~ /^ipsec/ or $temp[0] =~ /^lo/ ) {
            next;
        } elsif ( $temp[0] =~ /^$reddev/ ) {
            my @deviceinfo = `ifconfig $temp[0]`;
            foreach $devinfo (@deviceinfo) {
                chomp $devinfo;
                if ( $devinfo =~ /inet addr\:(\d{1,3}(\.\d{1,3}){3})/ ) {
                    $availablenetdevices{ $temp[0] } = $1;
                }
            }
        } else {
            next;
        }
    }
}

foreach $dev ( keys(%availablenetdevices) ) {
    my $tmp = grep( /^$dev$/, @filelist );
    if ( $tmp > 0 ) {
        delete( $availablenetdevices{$dev} );
    }
}

# Protocol listing
open( TMP, "/etc/protocols" ) or die "Unable to open /etc/protocols\: $!\n";
my @protocols = <TMP>;
close(TMP);

my $line;
my %availableprotocols;
foreach $line (@protocols) {
    chomp $line;

    if ( $line =~ m/^([a-z0-9]|\#\t[0-9]+\t+)/i ) {
        my @protoline = split( /\s+/, $line );

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

$selected{'PROTOCOL'}{'udp'}                    = '';
$selected{'PROTOCOL'}{'tcp'}                    = '';
$selected{'PROTOCOL'}{ $cgiparams{'PROTOCOL'} } = 'selected';

$selected{'RED_IFACE'}{$reddev} = 'selected';

foreach $dev ( sort( keys(%availablenetdevices) ) ) {
    $selected{'RED_IFACE'}{$dev} = '';
}
$selected{'RED_IFACE'}{ $cgiparams{'RED_IFACE'} } = 'selected';

$selected{'IF_ALIAS'}{'RED'} = '';
$selected{'IF_ALIAS'}{ $cgiparams{'IF_ALIAS'} } = 'selected';

$checked{'ENABLED'}{'off'}                   = '';
$checked{'ENABLED'}{'on'}                    = '';
$checked{'ENABLED'}{ $cgiparams{'ENABLED'} } = 'checked';

&openpage( $tr{'external access configuration'}, 1, '', 'networking' );

&openbigbox( '100%', 'LEFT' );

&alertbox($errormessage);

print <<END

<script>
function ffoxSelectUpdate(elmt)
{
        if(!document.all) elmt.style.cssText =
        elmt.options[elmt.selectedIndex].style.cssText;
}
</script>

<style type="text/css">
option.red{color:red;}
option.green{color:green;}
option.orange{color:orange;}
option.purple{color:purple;}
</style>

END
  ;

print "<form method='POST'>\n";

&openbox( $tr{'add a new rule'} );
print <<END
<table width='100%' border='$border'>
		<td class='base'>Red interface and aliases:</td>
              <td><select style='color: red' onchange="ffoxSelectUpdate(this);" name='RED_IFACE'>
END
  ;

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

    if ( !($reddev) ) {
        next;
    } elsif ( $reddev and ( $dev =~ /$reddev/ ) ) {
        print
"<option style='color: red' value='$dev' $selected{'RED_IFACE'}{$dev}>\n";
        if ($redaliasip) {
            print "RED$devifacesub $redaliasip\n";
        } else {
            print "RED$devifacesub - $dev\n";
        }
        print "</option>\n";
    } else {
        print
"<option style='color: black' value='$dev' $selected{'RED_IFACE'}{$dev}>$dev</option>";
    }
}

print <<END
</tr>
	<tr>
				<td width='25%' class='base' align='right'>$tr{'protocol long'}</td>
				<td width='25%'><select name='PROTOCOL'>
END
  ;
if ( $cgiparams{'PROTOCOL'} eq '6' ) {
    print "<option value='6' selected>$availableprotocols{6}</option>";
} else {
    print "<option value='6'>$availableprotocols{6}</option>";
}

if ( $cgiparams{'PROTOCOL'} eq '17' ) {
    print "<option value='17' selected>$availableprotocols{17}</option>";
} else {
    print "<option value='17'>$availableprotocols{17}</option>";
}

if ( $cgiparams{'PROTOCOL'} eq 'TCP & UDP' ) {
    print "<option value='TCP & UDP' selected>TCP & UDP</option>";
} else {
    print "<option value='TCP & UDP'>TCP & UDP</option>";
}

if ( $cgiparams{'PROTOCOL'} eq '1' ) {
    print "<option value='1' selected>$availableprotocols{1}</option>";
} else {
    print "<option value='1'>$availableprotocols{1}</option>";
}
print <<END
		</select></td>
<td width='25%' align='right' class='base'><font color='$colourred'>$tr{'sourcec'}</font></td>
<td width='25%'><input type='TEXT' name='EXT' value='$cgiparams{'EXT'}' SIZE='32' id='ext' @{[jsvalidipormask('ext')]}></td>
</tr>
<tr>
<td align='right' class='base'><font color='$colourred'>Original Destination Port:</font></td>
<td><input type='TEXT' name='DEST_PORT' value='$cgiparams{'DEST_PORT'}' SIZE='5' id='dest_port' @{[jsvalidport('dest_port')]}></td>
<td class='base'>$tr{'commentc'}</td>
<td><input type='text' name='COMMENT' value='$cgiparams{'COMMENT'}' size='32' id='comment' @{[jsvalidcomment('comment')]}  ></td>
</tr>
</table>
<table width='100%' border='$border'>
<tr>
<td class='base' width='50%' align='center'>$tr{'enabled'}<input type='checkbox' name='ENABLED' $checked{'ENABLED'}{'on'}></td>
<td width='50%' align='center'><input type='submit' name='ACTION' value='$tr{'add'}'></td>
</tr>
</table>
END
  ;
&closebox();

&openbox( $tr{'current rules'} );

my $ifcolorsmap = &Stans::modlib::ifcolormap();

my %render_settings = (
    'url'     => "/cgi-bin/xtaccess.cgi?[%COL%],[%ORD%]",
    'columns' => [
        {
            column => '1',
            title  => 'Red interface',
            size   => 15,
            sort   => 'cmp',
            tr     => \%{$ifcolorsmap},
        },
        {
            column => '3',
            title  => "$tr{'source'}",
            size   => 40,
            sort   => 'cmp',
            tr     => { '0.0.0.0/0' => 'ALL' },
        },
        {
            column => '2',
            title  => "$tr{'protocol'}",
            size   => 20,
            tr     => { '17' => 'UDP', '6' => 'TCP', '47' => 'GRE' },
            sort   => 'cmp',
        },
        {
            column => '4',
            title  => "$tr{'destination port'}",
            size   => 20,
            tr     => { '0' => 'N/A' },
        },
        {
            column => '5',
            title  => "$tr{'enabledtitle'}",
            size   => 10,
            tr     => 'onoff',
            align  => 'center',
        },
        {
            title => "$tr{'mark'}",
            size  => 10,
            mark  => ' ',
        },
        {
            title  => "$tr{'comment'}",
            break  => 'line',
            column => '6',
        },
    ]
);

&dispaliastab( $filename, \%render_settings, $cgiparams{'ORDER'}, $cgiparams{'COLUMN'} );

print <<END
<table class='blank' style='margin:6pt 0' border='$border'>
<tr>
<td style='text-align: center; width: 33%;'><input type='submit' name='ACTION' value='$tr{'remove'}'></td>
<td width='34%' align='center'><input type='submit' name='ACTION' value='$tr{'ffc-enable rule'}'></td>
<td style='text-align: center; width: 33%;'><input type='submit' name='ACTION' value='$tr{'edit'}'></td>
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

sub dispaliastab {
    my ( $filename, $settings, $order, $selected_column, $id ) = @_;

    # 'id' can be used to give us a different name, *iff* we are repeating the
    # widget.

    print qq{
		<table class='list'>
			<tr>
	};

    # display the header information, that is the list of columns etc.
    # whilst we're doing this, we can generate a mapping of which column to
    # display where.

    my @columns;
    my @styles;
    my @translations;
    my @breaks;
    my $i = 0;
    my $sort;
    my $colourtranslations;
    my $colourcolumn = 0;

    my ( $table1colour, $table2colour ) = ( '#f0f0f0', '#e0e0e0' );

    foreach my $column ( @{ $settings->{'columns'} } ) {
        my $span  = "";
        my $style = "";
        my $class = "list";

        if ( defined $column->{'break'} ) {
            print "</tr><tr>";
            $span  = " colspan='" . scalar(@columns) . "'";
            $class = "listcomment";
        }

        if ( defined $column->{'size'} ) {
            $style .= "width: $column->{'size'}%;";
        }

        my $arrow;
        my $url = $settings->{'url'};
        $i++;
        $url =~ s/\[%COL%\]/$i/;

        if ( $i == $selected_column ) {
            if ( $order eq $tr{'log ascending'} ) {
                $url =~ s/\[%ORD%\]/$tr{'log descending'}/;
                $arrow =
                  "&nbsp;<img src='/ui/img/down.jpg' alt='change direction'>";
            } else {
                $url =~ s/\[%ORD%\]/$tr{'log ascending'}/;
                $arrow =
                  "&nbsp;<img src='/ui/img/up.jpg' alt='change direction'>";
            }

            # choose a sorting algorithm
            if ( defined $column->{'sort'} ) {
                $sort = $column->{'sort'};
            }

        } else {
            $url =~ s/\[%ORD%\]/$tr{'log ascending'}/;
        }

        if ( not defined $column->{'colour'} ) {
            print qq {
				<th$span class='$class' style='$style'>
					<a href="$url">$column->{'title'}$arrow</a>
				</th>
			};
        }

        $style = "";

        if ( defined $column->{'align'} ) {
            $style = "text-align: $column->{'align'};";
        }

        if ( defined $column->{'break'} ) {
            push @breaks, $column->{'column'};
        } elsif ( defined $column->{'colour'} ) {
            $colourcolumn       = $column->{'column'};
            $colourtranslations = $column->{'tr'};
        } else {

            # Incase no column has been specified, assume column 0.
            if ( not defined $column->{'column'} ) {
                push @columns, "0,$column->{'mark'}";
            } elsif ( defined $column->{'mark'} ) {
                push @columns, "$column->{'column'},$column->{'mark'}";
            } else {
                push @columns, $column->{'column'};
            }
        }

        push @styles,       $style;
        push @translations, $column->{'tr'};
    }

    print qq{
		</tr>
	};

    # now we can render the content

    my $colour = "background-color: $table1colour;";
    $i = 0;
    my @lines;

    if ( ref $filename ) {
        @lines = (@$filename);
    } elsif ( open my $input, "$filename" ) {
        my $position = 1;
        while ( my $line = <$input> ) {
            chomp $line;
            my @cols = ( $position++, ( split /,/, $line ) );
            push @lines, \@cols;
        }
        close $input;
    } else {
        print "</table>\n";
        return;
    }

    # sort the lines according to the relevant selected row.
    my @sorted_lines;

    if ( ref $sort ) {

        # an overridden sort function.
        if ( $order eq $tr{'log ascending'} ) {
            @sorted_lines = sort {
                &{$sort}(
                    $a->[ $columns[ $selected_column - 1 ] ],
                    $b->[ $columns[ $selected_column - 1 ] ]
                  )
            } @lines;
        } else {
            @sorted_lines = sort {
                &{$sort}(
                    $b->[ $columns[ $selected_column - 1 ] ],
                    $a->[ $columns[ $selected_column - 1 ] ]
                  )
            } @lines;
        }
    } elsif ( $sort eq "<=>" ) {
        if ( $order eq $tr{'log ascending'} ) {
            @sorted_lines = sort {
                $a->[ $columns[ $selected_column - 1 ] ]
                  <=> $b->[ $columns[ $selected_column - 1 ] ]
            } @lines;
        } else {
            @sorted_lines = sort {
                $b->[ $columns[ $selected_column - 1 ] ]
                  <=> $a->[ $columns[ $selected_column - 1 ] ]
            } @lines;
        }
    } else {
        if ( $order eq $tr{'log ascending'} ) {
            @sorted_lines = sort {
                $a->[ $columns[ $selected_column - 1 ] ]
                  cmp $b->[ $columns[ $selected_column - 1 ] ]
            } @lines;
        } else {
            @sorted_lines = sort {
                $b->[ $columns[ $selected_column - 1 ] ]
                  cmp $a->[ $columns[ $selected_column - 1 ] ]
            } @lines;
        }
    }

    # slice the information up at all ?
    my $sliced = 0;

    if ( defined $settings->{'slice'} and ref $settings->{'slice'} ) {
        my $start = $settings->{'slice'}->{'start'};
        my $end   = $settings->{'slice'}->{'end'};
        $sliced = scalar @sorted_lines;
        my @slice = splice( @sorted_lines, $start, $end );
        @sorted_lines = @slice;
    }

    foreach my $line (@sorted_lines) {
        my @cols = @{$line};
        print "<tr class='list'>\n";
        my $entry = 0;
        foreach my $reference (@columns) {
            unless ( $reference =~ /,/ ) {

                # are we supposed to tranlate this at all ?
                my $text = $cols[$reference];
                if ( $cols[$reference] =~ /^\!/ ) {
                    $text = "<font color=#2B60DE>$text</font>";
                }

                if ( defined $translations[$entry] ) {
                    my $type = ref $translations[$entry];
                    if ( not $type ) {
                        if ( $translations[$entry] eq "onoff" ) {
                            if ( $cols[$reference] eq "on" ) {
                                $text = "<img alt='on' src='/ui/img/on.gif'>";
                            } else {
                                $text = "<img alt='off' src='/ui/img/off.gif'>";
                            }
                        }
                    } elsif ( $type eq "HASH"
                        and
                        defined $translations[$entry]->{ $cols[$reference] } )
                    {
                        $text = $translations[$entry]->{ $cols[$reference] };
                    } elsif ( $type eq "ARRAY"
                        and
                        defined $translations[$entry]->[ $cols[$reference] ] )
                    {
                        $text = $translations[$entry]->[ $cols[$reference] ];
                    }
                }
                if ( $colourcolumn != 0 ) {
                    $text =
"<span class='$colourtranslations->{$cols[$colourcolumn]}'>$text</span>";
                }
                print
"<td class='list' style='$colour$styles[$entry]' onclick=\"toggle_row('$id_$cols[0]');\" >$text</td>\n";
            } else {

                # this is a "mark" field, i.e. a checkbox
                my $text;
                my ( $column, $mark ) = split( /,/, $reference );
                my $newmark = "";
                if ( $mark ne " " ) {
                    $newmark = $mark;
                }
                if (   $cols[1] =~ /:/
                    or !( $cols[3] =~ /^eth/ )
                    or !( $cols[3] =~ /^ppp/ ) )
                {
                    $text = '';
                } else {
                    $text = 'DISABLED';
                }
                print
"<td class='list' style='$colour$styles[$entry]'><input id ='$id_$cols[$column]' type='checkbox' name='$newmark$cols[$column]' $text></td>";
            }
            $entry++;
        }

        # do we need to render any comments etc ?
        foreach my $reference (@breaks) {
            if ( defined $cols[$reference] and $cols[$reference] ne "" ) {
                print
"</tr><tr class='list'><td style='$colour' class='listcomment' colspan='"
                  . scalar(@cols)
                  . "'$styles[$entry]>$cols[$reference]</td>\n";
            }
        }

        $i++;
        print "</tr>\n";
        if ( $colour eq "background-color: $table1colour;" ) {
            $colour = "background-color: $table2colour;";
        } else {
            $colour = "background-color: $table1colour;";
        }
    }

    # and end the table
    print qq{
		</table>
	};

}
