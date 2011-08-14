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

&readhash("${swroot}/main/productdata", \%productdata);
&readhash("${swroot}/ethernet/settings", \%netsettings);
&showhttpheaders();

&openpage("Realtime bandwidth bars", 1, ' <META HTTP-EQUIV="Refresh" CONTENT="300"> <META HTTP-EQUIV="Cache-Control" content="no-cache"> <META HTTP-EQUIV="Pragma" CONTENT="no-cache"> ', 'about your smoothie');
&openbigbox('100%', 'LEFT');

&alertbox();

my $initial_position = "10";
my @bars;
my @bar_names;
my $oururl = "/cgi-bin/trafficstats.cgi?BARS=1";

if ($productdata{'RELEASE'} ne "")
{
  # Is P/R
  open (HDL, "/sbin/ip link | grep -v lo: | grep 'state UP' | sed -e 's/^[0-9]*: //' -e 's/:.*//'|");
}
else
{
  # Is SWE3
  open (HDL, "/usr/sbin/ip link | grep -v lo: | grep ',UP' | sed -e 's/^[0-9]*: //' -e 's/:.*//'|");
}
my @devices = <HDL>;
close (HDL);
chomp @devices;

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
		<div style='width: 81%; border:1px solid #7f7f7f; margin-left: auto; margin-right: auto;'>
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
<table id='${interface}_container' style='width: 100%; border-collapse: collapse; border:none; margin-left: auto; margin-right: auto; background-color:$bgcolor' cellspacing='0' cellpadding='0'>
  <tr>
    <td colspan='2' style='background-position: top left; background-repeat: no-repeat; vertical-align: top;' >
      <table style='width: 100%; margin-left: auto; margin-right: auto; border: none; border-collapse: collapse;' cellspacing='0' cellpadding='0'>
        <tr>
        <tr>
          <td colspan='6' style='background-color:#c3d1e5; height:2px'></td>
        </tr>
          <td colspan='2' style='width: 85px; text-align: left; background-color:#c3d1e5;'>&nbsp;<strong>$iftitle</strong></td>
          <td style='width:400px; background-color:#c3d1e5'>
            <table style='width: 100% border: 0; border-collapse: collapse;' cellpadding='0' cellspacing='0'>
              <tr>
                <td style='height: 8px; overflow: hidden; font-size: 6pt; color: #303030; background-color:#c3d1e5; width: 39px; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_10'>10&nbsp;</td>
                <td style='height: 8px; overflow: hidden; font-size: 6pt; color: #303030; background-color:#c3d1e5; width: 39px; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_100'>100&nbsp;</td>
                <td style='height: 8px; overflow: hidden; font-size: 6pt; color: #303030; background-color:#c3d1e5; width: 39px; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_1'>1k&nbsp;</td>
                <td style='height: 8px; overflow: hidden; font-size: 6pt; color: #303030; background-color:#c3d1e5; width: 39px; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_10k'>10&nbsp;</td>
                <td style='height: 8px; overflow: hidden; font-size: 6pt; color: #303030; background-color:#c3d1e5; width: 39px; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_100k'>100&nbsp;</td>
                <td style='height: 8px; overflow: hidden; font-size: 6pt; color: #303030; background-color:#c3d1e5; width: 39px; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_2'>1M&nbsp;</td>
                <td style='height: 8px; overflow: hidden; font-size: 6pt; color: #303030; background-color:#c3d1e5; width: 39px; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_10m'>10&nbsp;</td>
                <td style='height: 8px; overflow: hidden; font-size: 6pt; color: #303030; background-color:#c3d1e5; width: 39px; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_100m'>100&nbsp;</td>
                <td style='height: 8px; overflow: hidden; font-size: 6pt; color: #303030; background-color:#c3d1e5; width: 39px; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_3'>1G&nbsp;</td>
                <td style='height: 8px; font-size: 6pt; color: #303030; background-color:#c3d1e5; width: 39px; border-right: 1px solid #505050; text-align: right;' id='${section}_${interface}_scale_10g'>10&nbsp;</td>
              </tr>
            </table>
          </td>
          <td style='height:10px; width: 4px; margin:0; background-color:#c3d1e5'>&nbsp;</td>
          <td style='height:10px; width:55px; margin:0; background-color:#c3d1e5' id='${section}_${interface}_rate'></td>
          <td style='width: 2.5%; margin:0; background-color:#c3d1e5'>&nbsp;</td>
        </tr>
        <tr>
          <td colspan='6' style='background-color:none; height:5px'></td>
        </tr>
		};

		foreach my $section ( keys %{$interfaces{$interface}} ){
			my $colour = $table1colour;
			my $title  = $section;
		
			if ( $section eq "cur_inc_rate" ){
				$title  = "Incoming";
				$colour = "#5f5f9f";
			} elsif ( $section = "cur_out_rate" ){
				$title  = "Outgoing";
				$colour = "#9f5f5f";
			}

			print qq{
			<tr>
                                <td style='width: 2.5%; margin:0; background-color:none'>&nbsp;</td>
				<td style='height:10px;'>$title</td>
				<td style='height:10px; background-color: #efefef; font-size:0; margin:0; padding: 0px'>
                                  <div style='height:10pt; width: 0px; font-size:0; margin:0; padding:0; border: 0px; background-color: $colour;' id='${section}_$interface'>&nbsp;</div>
                                </td>
				<td style='height:10px; vertical-align: top;'>&nbsp;</td>
				<td style='height:10px' id='${section}_${interface}_rate'></td>
                                <td style='width: 2.5%; margin:0; background-color:none'>&nbsp;</td>
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
	print "<script> monitor(); </script>\n";
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

	//document.getElementById('status').style.display = "inline";

    	self.xmlHttpReq.send( null );
}

var splitter = /^cur_(inc|out)_rate_([^=]+)=([\\d\\.]+)\$/i;

function updatepage(str){
	//document.getElementById('status').style.display = "none";
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
			
			// The divider is based on the full bit rate. It
			//   is crowbarred to stay within the lines.
			divider = (40 * Math.log(results[3])/Math.log(10));
			if (divider == -Infinity) {
				divider = 0;
			}
			if (divider < 0) {
				divider = 0;
			}
			if (divider > 400) {
				divider = 400;
			}

			// The displayed rate is adjusted to use multipliers.
			if ( results[ 3 ] < 1000 ) {
				rate = parseFloat( results[3] )+'     ';
				rate = String(rate).substring(0,5);
				rate += " b/s";
			} else if ( results[ 3 ] < (1000*1000) ) {
				results[3] /= 1000;
				rate = parseFloat( results[3] )+'     ';
				rate = String(rate).substring(0,5);
				rate += " kb/s";
			} else if ( results[ 3 ] < (1000*1000*1000) ) {
				results[3] /= 1000*1000;
				rate = parseFloat( results[3] )+'     ';
				rate = String(rate).substring(0,5);
				rate += " Mb/s";
			} else if ( results[ 3 ] < (1000*1000*1000*1000) ){
				results[3] /= 1000*1000*1000;
				rate = parseFloat( results[3] )+'     ';
				rate = String(rate).substring(0,5);
				rate += " Gb/s";
			}


			scale[ id ] = "10G";
			old[ id ] = cur[ id ];
			cur[ id ] = parseInt( divider );
			document.getElementById( id ).style.width = cur[ id ] + 'px';
			document.getElementById( id + '_rate' ).innerHTML = rate;
		
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
