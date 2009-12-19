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

my $history = 60;
my $oururl = "/cgi-bin/trafficstats.cgi";
my $ubermax = 40;
my $maxgraphs = 5;   # Maximum number of graphs to show on the page;

# calculate some name translations 

my %ethersettings;
&readhash(  "/var/smoothwall/ethernet/settings", \%ethersettings );

my $etherline = "var translations = new Array(); ";

if ( defined $ethersettings{'GREEN_DEV'} and $ethersettings{'GREEN_DEV'} ne "" ){
	$etherline .= "translations[\"$ethersettings{'GREEN_DEV'}\"] = \"Green\"; ";
}
if ( defined $ethersettings{'ORANGE_DEV'} and $ethersettings{'ORANGE_DEV'} ne "" ){
	$etherline .= "translations[\"$ethersettings{'ORANGE_DEV'}\"] = \"Orange (DMZ)\"; ";
}
if ( defined $ethersettings{'PURPLE_DEV'} and $ethersettings{'PURPLE_DEV'} ne "" ){
	$etherline .= "translations[\"$ethersettings{'PURPLE_DEV'}\"] = \"Purple\"; ";
}
if ( defined $ethersettings{'RED_DEV'} and $ethersettings{'RED_DEV'} ne "" ){
	$etherline .= "translations[\"$ethersettings{'RED_DEV'}\"] = \"Red (External)\"; ";
}


my $script = qq {
<script language="Javascript">

var interfaces = new Array();
var rates = new Array();

var graphs = new Array();
var visibility = new Array();

var totals = new Array();

rates[ 10      ] = "10 bps";
rates[ 100     ] = "100 bps";
rates[ 1000    ] = "1 kbps";
rates[ 10000   ] = "10 kbps";
rates[ 100000  ] = "100 kbps";
rates[ 1000000    ] = "1 Mbps";
rates[ 10000000   ] = "10 Mbps";
rates[ 100000000  ] = "100 Mbps";
rates[ 1000000000 ] = "1 Gbps";

var titles = new Array();
titles[ 'inc' ] = "I<br/>n<br/>c<br/>o<br/>m<br/>i<br/>n<br/>g";
titles[ 'out' ] = "O<br/>u<br/>t<br/>g<br/>o<br/>i<br/>n<br/>g";

$etherline

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

function updatepage(str){
	document.getElementById('status').style.display = "none";

	/* determine the interfaces we're supposed to be dealing with, wherever they may be */

	var rows = str.split( '\\n' );

	var ugraph = new Array();
	ugraph[ 'inc' ] = new Array();
	ugraph[ 'out' ] = new Array();
	
	for ( var i = 0; i < graphs.length ; i++ ){
		ugraph[ 'inc' ][ graphs[ i ] ] = 0;
		ugraph[ 'out' ][ graphs[ i ] ] = 0;
		totals[ graphs[i] ] = 0;
	}
	
	
	var detail_finder = new RegExp( /cur_(.{3})_rate_([^=]*)=(.*)/ );
	for ( var i = 0; (i < rows.length) && ( i <= $maxgraphs ) ; i++ ){
		if ( !rows[i] ){ continue; }
		var matches = detail_finder.exec( rows[i] );
		var tinterface = matches[2];
		var direction = matches[1];	
		var value     = matches[3];

		if (!document.getElementById('graph_' + tinterface ) ){
			create_graph( tinterface );
		}

		update_graph( tinterface, direction, value );
		
		ugraph[ direction ][ tinterface ] = 1;
	}

	for ( var i = 0; i < graphs.length ; i++ ){
		if ( ugraph['inc'][graphs[i]] == 0 ){
			update_graph( graphs[i], 'inc', 0 );
		}
		if ( ugraph['out'][graphs[i]] == 0 ){
			update_graph( graphs[i], 'out', 0 );
		}
	}

	the_timeout = setTimeout( "xmlhttpPost()", 1000 );
}

function update_graph( tinterface, direction, value)
{
	var max_height = $ubermax;

	if ( value >= max_height ){
		max_height = value;
	}

	var total_height = 0;

	for ( var i = 0 ; i < ($history-1) ; i++ ){
		if ( parseInt(interfaces[ tinterface ][ direction ][ i ]) > max_height ){
			max_height = interfaces[ tinterface ][ direction ][ i ];
			total_height += interfaces[tinterface][direction][i];
		}
	}
	
	max_height++;

	max_height = rationalise( max_height );

	var height = Math.floor( ( value / max_height ) * $ubermax );
	total_height += height;

	for ( var i = 0 ; i < ($history-1) ; i++ ){
		interfaces[ tinterface ][ direction ][ i ] = interfaces[ tinterface ][ direction ][ i + 1 ];
		var nheight = Math.floor((interfaces[ tinterface ][ direction ][ i ]/max_height)*60);
		document.getElementById('u_' + tinterface + '_' + direction + '_' + i ).style.height = nheight + 'px';
		document.getElementById('l_' + tinterface + '_' + direction + '_' + i ).style.height = Math.floor(nheight/2) + 'px';
	}
	document.getElementById('u_' + tinterface + '_' + direction + '_' + i ).style.height = height + 'px';
	document.getElementById('l_' + tinterface + '_' + direction + '_' + i ).style.height = Math.floor(height/2) + 'px';

	document.getElementById('title_' + tinterface + '_' + direction ).innerHTML = rates[ max_height ]  + " ";
	interfaces[tinterface][direction][($history-1)] = value;

	// work out the actual rate ...
	
	var value2 = value;

	var re = new RegExp( /(\\d+\\.?\\d?)/ );

	if ( value2 > 1048576 ){
		value2 = ( value2 / 1048576 );
		var m2 = re.exec( value2 ); 
		value2 = m2[1] + "Mbps";
	} else if ( value2 > 1024 ){
		value2 = ( value2 / 1024 );
		var m2 = re.exec( value2 ); 
		value2 = m2[1] + "Kbps";
	} else {
		var m2 = re.exec( value2 ); 
		value2 = m2[1] + "bps";
	}

	document.getElementById('rate_' + tinterface + '_' + direction ).innerHTML = value2 + " ";

	totals[ tinterface ] += total_height;
}

function fader()
{
	for ( var i = 0; i < graphs.length ; i++ ){
		var total_height = totals[ graphs[i] ];
		var tinterface = graphs[i];

		if ( total_height == 0 ){
			visibility[ tinterface ] = visibility[tinterface] - 1;
		} else {
			visibility[ tinterface ] ++;
		}
	
		if ( visibility[ tinterface ] >= 20 ){
			visibility[tinterface] = 20;
		} else if ( visibility[ tinterface ] < 0 ){
			visibility[tinterface] = 0;
			document.getElementById('graph_' + tinterface).style.display = 'none';
		} else {
			document.getElementById('graph_' + tinterface).style.display = 'inline';
		}
	
		document.getElementById('outer_' + tinterface).className = "s" + visibility[tinterface];
	}

	the_timeout = setTimeout( "fader()", 60 );
}

function maxit( v1, v2 )
{
	if ( v1 > v2 ){
		return v1;
	} else {
		return v2;
	}
}

function rationalise( value )
{
	for ( var v = 10; v < 1000000000 ; v *= 10 ){
		if ( maxit( value, v ) == v ){
			return v;
		}
	}
}

function printableinterface(tinterface)
{

	if (tinterface == '$netsettings{'GREEN_DEV'}') {
		return 'Green'; 
	} else if (tinterface == '$netsettings{'ORANGE_DEV'}') {
		return 'Orange'; 
	} else if (tinterface == '$netsettings{'PURPLE_DEV'}') {
		return 'Purple'; 
	} else if (tinterface == '$netsettings{'RED_DEV'}') {
		return 'Red'; 
	} else if (tinterface == 'ppp0') {
		return 'Modem'; 
	} else if (tinterface == 'ippp0') {
		return 'ISDN'; 
	} else {
		return tinterface;
	}
}

function create_graph(tinterface)
{
	var directions = new Array( "inc", "out" );
	var colours    = new Array( "#ff9900", "#ffcc66" );
	var graph_html = "";	

	for ( var j = 0; j < directions.length ; j++ ){
		var direction = directions[j];
		graph_html += "<div><table style='width: 100%; height: 100px; border: 1px solid #c0c0c0; border-collapse: collapse; background-color: #f0f0ff; margin-bottom: 3px;'><tr style='height: 70px;'><td style='vertical-align: top; font-size: 8px; width: 10px; text-align: center; vertical-align: middle; background-color: #e0e0ef;' rowspan='2'>" + titles[ direction ] + "</td><td style='vertical-align: top; font-size: 8px; width: 50px; text-align: right; border-right: 1px dashed #909090;' id='title_" + tinterface + "_" + direction + "' rowspan='2'></td><td style='vertical-align: top;'><div class='rateb'></div><div class='ratet' id='rate_" + tinterface + "_" + direction + "'>11</div></td>";
	
		for ( var i = 0; i < $history ; i++ ){
			graph_html += "<td style='width: 5px; margin: 0px;padding: 0px; vertical-align: bottom;  background-image: url(/ui/img/dasher.png); background-repeat: repeat;'><div id='u_" + tinterface + "_" + direction + "_" + i + "' style='background-color: " + colours[j] + "; height: 33px; margin: 0px; width: 6px; padding: 0px; font-size: 1px;'></div></td>";
		}

		graph_html += "<td></td></tr><tr style='height: 30px;'><td></td>";

		for ( var i = 0; i < $history ; i++ ){
			graph_html += "<td style='border-top: 1px solid #909090;  width: 5px; margin: 0px;padding: 0px; padding-top: 1px; vertical-align: top;'><div id='l_" + tinterface + "_" + direction + "_" + i + "' style=\\\"background-image: url('/ui/img/fader" + direction + ".jpg'); background-repeat: repeat-x; background-position: top left; height: 11px; margin: 0px; width: 6px; padding: 0px; font-size: 1px;\\\"></div></td>";
		}

		graph_html += "<td></td></tr></table></div>";
	}

	var tinterfacetitle = tinterface;
	tinterfacetitle = tinterfacetitle.replace(/_/g," ");
	tinterfacetitle = printableinterface(tinterfacetitle.replace(/(GREEN|ORANGE|PURPLE|RED)/g," "));

	if ( translations[tinterface] && translations[ tinterface ] != "" ){
		tinterfacetitle = translations[tinterface];
	}

	document.getElementById('content').innerHTML += "<div style='width: 60%; margin-left: auto; margin-right: auto;' id='outer_" + tinterface + "'><div id='graph_" + tinterface + "'><div style='border: 1px solid #c0c0c0; margin-bottom: 3px;'><div style='background-color: #d0d0d0; text-align: center; width: 100%;'>" + tinterfacetitle + "</div><div style='padding: 5px;'>" + graph_html + "</div></div></div></div>";
	interfaces[ tinterface ] = new Array();
	interfaces[ tinterface ]["inc"] = new Array();
	interfaces[ tinterface ]["out"] = new Array();
	for ( var i = 0 ; i < $history ; i++ ){
		interfaces[tinterface]["inc"][ i ] = 0;
		interfaces[tinterface]["out"][ i ] = 0;
	}

	graphs[ graphs.length ] = tinterface;
	visibility[ tinterface ] = 0;
}


</script>
};

# display the page ...

&openpage($tr{'network traffic graphs'}, 1, $script, 'about your smoothie');
&openbigbox('100%', 'LEFT');

&alertbox();

print qq{
	<style>  
		.ratet{ display: inline; float: right; position: absolute; border: 1px solid #c0c0c0; margin-left: 320px; width: 60px; font-size: 9px; text-align: right; }
		.rateb{ display: inline; float: right; position: absolute; background-color: #f0f0ff; margin-left: 320px; width: 60px; opacity: 0.50; -moz-opacity: 0.50; -khtm-opacity: 0.50; filter:alpha(opacity=50); height: 10px;}
		.s0{ opacity: 0.10 ; -moz-opacity: 0.10 ; -khtml-opacity:0.10 ; filter:alpha(opacity=10)} 
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

	<div style='width: 100%; text-align: right;' id='dbg'></div>
	<div style='width: 100%; text-align: right;'><span id='status' style='background-color: #fef1b5; display: none;'>Updating</span>&nbsp;</div>
	<div id='content' style='width: 100%; overflow: auto; vertical-align: bottom; border-bottom: solid 1px #c0c0c0; overflow-x: hidden;'></div>
	<script>xmlhttpPost(); fader();</script>
};

&closebigbox();
&closepage();
exit;

