#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %netsettings;

&readhash("${swroot}/ethernet/settings", \%netsettings);
&showhttpheaders();

&openpage("Realtime bandwidth bars", 1, ' <META HTTP-EQUIV="Refresh" CONTENT="300"> <META HTTP-EQUIV="Cache-Control" content="no-cache"> <META HTTP-EQUIV="Pragma" CONTENT="no-cache"> ', 'about your smoothie');
&openbigbox('100%', 'LEFT');

&alertbox();

my $initial_position = "10";
my @bars;
my @bar_names;
my $oururl = "/cgi-bin/trafficstats.cgi";

my @devices = ( "eth0", "eth1", "eth2", "eth3", "ippp0" , "ppp0" );

&openbox('Bandwidth bars:');
&realtime_graphs();
&closebox();

&closebigbox();
&closepage($errormessage);

sub printableiface 
{
	my $iface = shift;
	my %ifaces = (
			$netsettings{'GREEN_DEV'} => 'Green',
			$netsettings{'ORANGE_DEV'} => 'Orange',
			$netsettings{'PURPLE_DEV'} => 'Purple',
			$netsettings{'RED_DEV'} => 'Red',
			'ppp0' => 'Modem',
			'ippp0' => 'ISDN');
	return $ifaces{$iface} || $iface;
}
			
sub realtime_graphs 
{
	print "<div id='dbg'></div>";

	# construct the bar graphs accordingly.

	my %interfaces;
	my %addresses;

	open INPUT, "</var/log/quicktrafficstats";
	while ( my $line = <INPUT> ){
		next if ( not $line =~ /^cur_(inc|out)_rate/ );
		my $rule = $&;
		$line = $';
		my ($interface, $value ) = ( $line =~ /_([^=]+)=([\d\.]+)$/i );
		$interfaces{ $interface }{ $rule } = $value;
		if($interface =~ /^\d+\.\d+\.\d+\.\d+/ && $rule eq 'cur_out_rate') {
			$addresses{$interface} = $value;
		}
	}

	push @devices, sort { $addresses{$b} <=> $addresses{$a}; } keys %addresses;


	print	"<div style='width: 100%; text-align: right;'><span id='status' style='background-color: #fef1b5; display: none;'>Updating</span>&nbsp;</div>\n";
	print qq { 
	<div id='main_content'>
	<style>  
		.s0{ display: none; } 
		.s1{ opacity: 0.10 ; -moz-opacity: 0.10 ; -khtml-opacity:0.10 ; filter:alpha(opacity=10)} 
		.s2{ opacity: 0.15 ; -moz-opacity: 0.15 ; -khtml-opacity:0.15 ; filter:alpha(opacity=15)} 
		.s3{ opacity: 0.20 ; -moz-opacity: 0.20 ; -khtml-opacity:0.20 ; filter:alpha(opacity=20)} 
		.s4{ opacity: 0.25 ; -moz-opacity: 0.25 ; -khtml-opacity:0.25 ; filter:alpha(opacity=25)} 
		.s5{ opacity: 0.30 ; -moz-opacity: 0.30 ; -khtml-opacity:0.30 ; filter:alpha(opacity=30)} 
		.s6{ opacity: 0.35 ; -moz-opacity: 0.35 ; -khtml-opacity:0.35 ; filter:alpha(opacity=35)} 
		.s7{ opacity: 0.40 ; -moz-opacity: 0.40 ; -khtml-opacity:0.40 ; filter:alpha(opacity=40)} 
		.s8{ opacity: 0.45 ; -moz-opacity: 0.45 ; -khtml-opacity:0.45 ; filter:alpha(opacity=45)} 
		.s9{ opacity: 0.50 ; -moz-opacity: 0.50 ; -khtml-opacity:0.50 ; filter:alpha(opacity=50)} 
		.s10{ opacity: 0.55 ; -moz-opacity: 0.55 ; -khtml-opacity:0.55 ; filter:alpha(opacity=55)} 
		.s11{ opacity: 0.60 ; -moz-opacity: 0.60 ; -khtml-opacity:0.60 ; filter:alpha(opacity=60)} 
		.s12{ opacity: 0.65 ; -moz-opacity: 0.65 ; -khtml-opacity:0.65 ; filter:alpha(opacity=65)} 
		.s13{ opacity: 0.70 ; -moz-opacity: 0.70 ; -khtml-opacity:0.70 ; filter:alpha(opacity=70)} 
		.s14{ opacity: 0.75 ; -moz-opacity: 0.75 ; -khtml-opacity:0.75 ; filter:alpha(opacity=75)} 
		.s15{ opacity: 0.80 ; -moz-opacity: 0.80 ; -khtml-opacity:0.80 ; filter:alpha(opacity=80)} 
		.s16{ opacity: 0.85 ; -moz-opacity: 0.85 ; -khtml-opacity:0.85 ; filter:alpha(opacity=85)} 
		.s17{ opacity: 0.90 ; -moz-opacity: 0.90 ; -khtml-opacity:0.90 ; filter:alpha(opacity=90)} 
		.s18{ opacity: 0.95 ; -moz-opacity: 0.95 ; -khtml-opacity:0.95 ; filter:alpha(opacity=95)} 
		.s19{ } 
	</style>
		<div style='width: 90%; margin-left: auto; margin-right: auto;'>
	};
	my @rules;

	foreach my $interface ( @devices ){
		my $iftitle = $interface;
		$iftitle =~ s/_/ /g;
		$iftitle =~ s/(GREEN|RED|ORANGE|PURPLE)//;
		$iftitle = printableiface($iftitle);
		if ($iftitle eq 'Red') { $bgcolor = '#ffaaaa'; }
		elsif ($iftitle eq "Green") {$bgcolor = "#bbffbb";}
		elsif ($iftitle eq "Purple") {$bgcolor = "#ddaaff";}
		elsif ($iftitle eq "Orange") {$bgcolor = "#ffaa77";}
		else { $bgcolor = ""; }
		print qq{
		<table id='${interface}_container' style='width: 90%; border-collapse: collapse; border: 0px; margin-left: auto; margin-right: auto; background-color:$bgcolor'>
		<tr style='background-color: #C3D1E5;'>
			<td style="background-position: top left; background-repeat: no-repeat; text-align: left; vertical-align: middle;">&nbsp;<strong>$iftitle</strong></td>
			<td style="width: 40%; background-position: top right; background-repeat: no-repeat; text-align: left; vertical-align: middle;">&nbsp;</td>
		</tr>
		<tr>
			<td colspan='2' style='background-position: top left; background-repeat: no-repeat; vertical-align: top;' ><br/><table style='width: 95%; margin-left: auto; margin-right: auto; border: 0px; border-collapse: collapse;'>
		};

		foreach my $section ( keys %{$interfaces{$interface}} ){
			my $colour = $table1colour;
			my $title  = $section;
		
			if ( $section eq "cur_inc_rate" ){
				$title  = "Incoming";
				$colour = "#4D71A3";
			} elsif ( $section = "cur_out_rate" ){
				$title  = "Outgoing";
				$colour = "#4F8E83";
			}

			print qq{
			<tr>
				<td style='width: 15%;' rowspan='2' style='vertical-align: top;'>$title</td>
				<td style='width: 400px; background-color: $table2colour; font-size: 6pt; padding: 0px;'  id='${section}_${interface}_scale' >
				<table style='width: 100%; border: 0px; border-collapse: collapse;'>
				<tr>
				<td style='height: 8px; overlow: hiddent; font-size: 6pt; color: #303030; width: 25%; background-colour: #efefef; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_1'>25%</td>
				<td style='height: 8px; overlow: hiddent; font-size: 6pt; color: #303030; width: 25%; background-colour: #dfdfdf; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_2'>50%</td>
				<td style='height: 8px; overlow: hiddent; font-size: 6pt; color: #303030; width: 25%; background-colour: #cfcfcf; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_3'>75%</td>
				<td style='height: 8px; overlow: hiddent; font-size: 6pt; color: #303030; width: 25%; background-colour: #bfbfbf; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_4'>100%<td>
				</tr>
				</table>
				</td>
				<td style='width: 2%;' rowspan='2' style='vertical-align: top;'>&nbsp;</td>
				<td rowspan='2' id='${section}_${interface}_rate'></td>
			</tr>
			<tr>
				<td style='width: 400px; background-color: $table2colour; padding: 0px;'>
				<div style='width: 0px; border: 0px; background-color: $colour;' id='${section}_$interface'>&nbsp;</div>	
				</td>
			</tr>
			<tr style='height: 5px;'><td colspan='3'></td></tr>
			};
			push @rules, "${section}_${interface}";
		}

		print qq{
			</table>
		</td>
	</tr>
	<tr><td colspan='2'></td></tr>
	</table>
		};
	}

	print "</div><script>";
	show_script( \@rules );
	print "</script>";
	print "</div>";
	print "<script> monitor(); fader(); </script>\n";
}

sub show_script
{
	my ( $rules ) = @_;

	print qq {
var interfaces 	= new Array();
var old 	= new Array();
var cur 	= new Array();
var iftotals 	= new Array();
var ifclass 	= new Array();
var ofclass 	= new Array();
var ifnames 	= new Array();
var scale       = new Array();
var oscale      = new Array();
var scalef      = new Array();
	};

	for ( my $i = 0 ; $i < scalar( @$rules ) ; $i++ ){
		print "interfaces[$i] = '$rules->[$i]';\n";
		print "cur['$rules->[$i]'] = 0;\n";
		print "old['$rules->[$i]'] = 0;\n";
		print "scale['$rules->[$i]'] = 0;\n";
		print "oscale['$rules->[$i]'] = 0;\n";
		print "scalef['$rules->[$i]'] = 0;\n";
	}

	my $i = 0;
	foreach my $interface ( @devices ){
		print "iftotals['$interface'] = 0\n";
		print "ifclass['$interface'] = 19\n";
		print "ofclass['$interface'] = 19\n";
		print "ifnames[ $i ] = '$interface'\n;";
		$i++;
	}

	print qq {
var dbg = document.getElementById('dbg');

function xmlhttpPost()
{
	var xmlHttpReq = false;
    	var self = this;


    	if (window.XMLHttpRequest) {
		// Mozilla/Safari
        	self.xmlHttpReq = new XMLHttpRequest();
    	} else if (window.ActiveXObject) {
    		// IE
        	self.xmlHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
    	}

    	self.xmlHttpReq.open('GET', "$oururl", true);
	self.xmlHttpReq.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

	self.xmlHttpReq.onreadystatechange = function() {
	        if ( self.xmlHttpReq && self.xmlHttpReq.readyState == 4) {
        		updatepage(self.xmlHttpReq.responseText);
		}
    	}

	document.getElementById('status').style.display = "inline";

    	self.xmlHttpReq.send( null );
}

var splitter = /^cur_(inc|out)_rate_([^=]+)=([\\d\\.]+)\$/i;

function updatepage(str){
	document.getElementById('status').style.display = "none";
	var rows = str.split( '\\n' );
	
	for ( var i = 0; i < ifnames.length ; i++ ){
		iftotals[ ifnames[i] ] = 0;
	}

	dbg.innerHTML = "";

	for ( var i = 0; i < rows.length ; i++ ){
		if ( rows[ i ] != "" ){
			var results = splitter.exec(rows[i]);	
			var id = 'cur_' + results[ 1 ] + '_rate_' + results[2];
			if ( !document.getElementById( id + '_rate' ) ){
				continue;
			}
			var divider = 0;;
			var rate = 0;
			var s1; var s2; var s3; var s4;
			if ( results[ 3 ] < (1024) ) {
				s1 = '&nbsp;'; s2 = '&nbsp;'; s3 = '&nbsp;'; s4 = '&nbsp;';
			} else if ( results[ 3 ] < (1024*64) ) {
				/* 64 KBps */
				divider = (400 / ( 1024*64 )) * results[ 3 ];
				rate = parseInt( results[3] / 1024 );
				rate += " Kbps";
				s1 = '16K'; s2 = '32K'; s3 = '48K'; s4 = '64K';
			} else if ( results[ 3 ] < (1024*512) ) {
				/* 512 KBps */
				divider = (400 / ( 1024*512 )) * results[ 3 ];
				rate = parseInt( results[3] / 1024 );
				rate += " Kbps";
				s1 = '128K'; s2 = '256K'; s3 = '384K'; s4 = '512K';
			} else if ( results[ 3 ] < (1024 * 1024) ){
				/* 1 MBps */
				divider = (400 / ( 1024 * 1024 )) * results[ 3 ];
				rate = parseInt( results[3] / 1024 );
				rate += " Kbps";
				s1 = '256K'; s2 = '512K'; s3 = '768K'; s4 = '1M';
			} else if ( results[ 3 ] < (1024 * 1024 *2) ){
				/* 2 MBps */
				divider = (400 / ( 1024 * 1024 *2 )) * results[ 3 ];
				rate = parseInt( results[3] / (1024 * 1024) * 10) / 10;
				rate += " Mbps";
				s1 = '512K'; s2 = '1M'; s3 = '1.5M'; s4 = '2M';
			} else if ( results[ 3 ] < (1024 * 1024 *4) ){
				/* 4 MBps */
				divider = (400 / ( 1024 * 1024 *4 )) * results[ 3 ];
				rate = parseInt( results[3] / (1024 * 1024) * 10) / 10;
				rate += " Mbps";
				s1 = '1M'; s2 = '2M'; s3 = '3M'; s4 = '4M';
			} else if ( results[ 3 ] < (1024 * 1024 *8) ){
				/* 8 MBps */
				divider = (400 / ( 1024 * 1024 *8 )) * results[ 3 ];
				rate = parseInt( results[3] / (1024 * 1024) * 10) / 10;
				rate += " Mbps";
				s1 = '2M'; s2 = '4M'; s3 = '6M'; s4 = '8M';
			} else if ( results[ 3 ] < (1024*1024*10) ){
				/* 10 MBps */
				divider = (400 / ( 1024 * 1024 * 10 )) * results[ 3 ];
				rate = parseInt( results[3] / (1024*1024) );
				rate += " Mbps";
				s1 = '2.5M'; s2 = '5M'; s3 = '7.5M'; s4 = '10M';
			} else if ( results[ 3 ] < (1024*1024*100) ){
				/* 100 MBps */
				divider = (400 / ( 1024 * 1024 * 100 )) * results[ 3 ];
				rate = parseInt( results[3] / (1024*1024) );
				rate += " Mbps";
				s1 = '25M'; s2 = '50M'; s3 = '75M'; s4 = '100M';
			} else {
				/* GBps Scale */
				divider = (400 / ( 1024 * 1024 * 1024 )) * results[ 3 ];
				rate = parseInt( results[3] / (1024*1024) );
				rate += " Mbps";
				s1 = '250M'; s2 = '500M'; s3 = '750M'; s4 = '1G';
			}


			scale[ id ] = s4;
			old[ id ] = cur[ id ];
			cur[ id ] = parseInt( divider );
			document.getElementById( id ).style.width = cur[ id ] + 'px';
			document.getElementById( id + '_rate' ).innerHTML = rate;
			document.getElementById( id + '_scale_1' ).innerHTML = s1;
			document.getElementById( id + '_scale_2' ).innerHTML = s2;
			document.getElementById( id + '_scale_3' ).innerHTML = s3;
			document.getElementById( id + '_scale_4' ).innerHTML = s4;
		
			iftotals[ results[ 2 ] ] += parseInt( results[ 3 ] );

			if ( scale[ id ] != oscale[ id ] ){
				oscale[ id ] = scale[ id ];
				scalef[ id ] = 20;
			}
		}

	}
			

	setTimeout( "xmlhttpPost()", 1000 );
}

function monitor()
{
	xmlhttpPost();
}

function fader()
{
	var jump = 3;

	for ( var i = 0; i < interfaces.length ; i++ ){
		var element  = document.getElementById( interfaces[i] );

		var id = interfaces[i];
		if ( element ){
                        var direction =  ( old[ id ] - cur[ id ] );

			var jdis = parseInt( direction / jump );
			if ( jdis < 0 ){ jdis *= -1; }

                        if ( direction < -jump ){
                                if ( old[ id ] < cur[ id ] ){
                                        old[ id ] = ( old[ id ] + jdis );
                                        if ( element ){
                                                element.style.width = old[ id ] + "px";
                                        }
                                }
                        } else if ( direction > jump ){
                                if ( old[ id ] > cur[ id ] ){
                                        old[ id ] = ( old[ id ] - jdis );
                                        if ( element ){
                                                element.style.width = old[ id ] + "px";
                                        }
                                }
                        }
		}
	}


	for ( var i = 0; i < ifnames.length ; i++ ){
		if ( iftotals[ ifnames[i] ] == 0 ){
			ifclass[ ifnames[i] ] --; 
		} else {
			ifclass[ ifnames[i] ] ++; 
		}

		if ( ifclass[ ifnames[i] ] > 19 ){
			ifclass[ifnames[i]] = 19;
		} else if ( ifclass[ifnames[i]] < 0 ){
			ifclass[ifnames[i]] = 0;
		}

		if ( ofclass[ ifnames[ i ] ] != ifclass[ ifnames[ i ] ] ){
			document.getElementById( ifnames[i] + '_container' ).className = "s" + ifclass[ifnames[i]];
			ofclass[ ifnames[ i ] ] = ifclass[ ifnames[ i ] ];
		}
	}

	for ( var i = 0; i < interfaces.length ; i++ ){
		var id = interfaces[i];
		var v = scalef[id];
		if ( scalef[ id ] > 0 ){
			scalef[id]--;
			document.getElementById( id + '_scale').style.background = 'rgb(' + (195+(v*2)) + ',' + (209+(v*2)) + ',' + (229+(v*2)) +')';
		} else if ( scalef[ id ] == 0 ){
			scalef[id]--;
			document.getElementById( id + '_scale').style.background = 'rgb(195,209,229)';
		}
			
	}



	setTimeout( fader, 60 );



}




	};
}
