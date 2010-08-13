#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# SmoothInfo MOD v. 2.2b by Pascal Touch  (nanouk) on Smoothwall forums (2008).
# SmoothInstall compatible
# Packed using Steve McNeill's Mod Build System

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );
use smoothtype qw(:standard);

include "$swroot/smoothinfo/about.ph";

my (%smoothinfosettings, %checked);
my $version = $ModDetails{'MOD_LONG_NAME'} . " v. " . $ModDetails{'MOD_VERSION'};
my $filename = "$MODDIR/etc/report.txt";
my $settingsfile = "$MODDIR/etc/settings";
my @chains = ('All chains');
my @items = ('MEMORY', 'TOP', 'LOADEDMODULES', 'CPU', 'IRQs', 'DISKSPACE', 'CONNTYPE', 'ADAPTERS', 'NETCONF1', 'NETCONF2', 'ROUTE', 'CONNTRACKS', 'MODLIST', 'DHCPINFO', 'PORTFW', 'CONFIG', 'DMESG', 'APACHE', 'MESSAGES', 'SERVICES', 'MODSERVICES');
my @ASCII_items = ('SWITCH1', 'SWITCH2', 'SWITCH3', 'WAP1', 'WAP2', 'WAP3', 'WAP4', 'WAP5', 'WAP6', 'MODEM', 'ROUTER');
my $errormessage = '';

unless (-e "$settingsfile") { system ("/bin/touch","$settingsfile"); }
unless (-e "$filename") { system ("/bin/touch","$filename"); }

my $success = message('smoothinfogetchains');

if (not defined $success) {
$errormessage = $tr{'smoothd failure'};
}

open (FILE, "<$MODDIR/etc/chains");
my @chains = (@chains,<FILE>);
chomp @chains;

$smoothinfosettings{'ACTION'} = '';

foreach (@items) 
	{
	$smoothinfosettings{$_} = 'off';
	}

foreach (@ASCII_items)
	{
	$smoothinfosettings{$_} = 'off';
	}

$smoothinfosettings{'HEADORTAIL'} = 'TAIL';
$smoothinfosettings{'HEADORTAIL2'} = 'TAIL2';
$smoothinfosettings{'HEADORTAIL3'} ='TAIL3';
$smoothinfosettings{'LINES'} = '';
$smoothinfosettings{'LINES2'} = '';
$smoothinfosettings{'WRAP'} = '100';

&getcgihash(\%smoothinfosettings);

foreach (@chains)
	{
	$checked{$_}{'off'} = '';
	$checked{$_}{'on'} = '';
	$checked{$_}{$smoothinfosettings{$_}} = 'CHECKED';
	}

if ($smoothinfosettings{'ACTION'} eq $tr{'smoothinfo-generate'})
{
# ERROR CHECKING
if ($smoothinfosettings{'LINES2'} eq ''
 && $smoothinfosettings{'STRING2'} eq ''
 && $smoothinfosettings{'APACHE'} eq 'on') {$errormessage = $tr{'smoothinfo-define-number-of-lines'};}

if ($smoothinfosettings{'LINES3'} eq ''
 && $smoothinfosettings{'STRING3'} eq ''
 && $smoothinfosettings{'MESSAGES'} eq 'on') {$errormessage = $tr{'smoothinfo-define-number-of-lines'};}

if ($smoothinfosettings{'SCREENSHOTS'} =~ /a href/i) {$errormessage = $tr{'smoothinfo-bad-link'};}

unlink ("$MODDIR/etc/schematic");

if ($smoothinfosettings{'SWITCH1'} eq 'on'
 || $smoothinfosettings{'SWITCH2'} eq 'on'
 || $smoothinfosettings{'SWITCH3'} eq 'on'
 || $smoothinfosettings{'WAP1'} eq 'on'
 || $smoothinfosettings{'WAP2'} eq 'on'
 || $smoothinfosettings{'WAP3'} eq 'on'
 || $smoothinfosettings{'WAP4'} eq 'on'
 || $smoothinfosettings{'WAP5'} eq 'on'
 || $smoothinfosettings{'WAP6'} eq 'on'
 ||  $smoothinfosettings{'MODEM'} eq 'on'
 || $smoothinfosettings{'ROUTER'} eq 'on') { system("/bin/touch $MODDIR/etc/schematic"); }

unless ($smoothinfosettings{'MEMORY'} eq 'on'
 && $smoothinfosettings{'LOADEDMODULES'} eq 'on'
 && $smoothinfosettings{'TOP'} eq 'on'
 && $smoothinfosettings{'CPU'} eq 'on'
 && $smoothinfosettings{'DISKSPACE'} eq 'on'
 && $smoothinfosettings{'CONNTYPE'} eq 'on'
 && $smoothinfosettings{'ADAPTERS'} eq 'on'
 && $smoothinfosettings{'NETCONF1'} eq 'on'
 && $smoothinfosettings{'NETCONF2'} eq 'on'
 &&  $smoothinfosettings{'DHCPLEASES'} eq 'on'
 && $smoothinfosettings{'PORTFW'} eq 'on'
 && $smoothinfosettings{'ROUTE'} eq 'on'
 && $smoothinfosettings{'MODLIST'} eq 'on'
 && $smoothinfosettings{'CONFIG'} eq 'on'
 && $smoothinfosettings{'DMESG'} eq 'on'
 && $smoothinfosettings{'APACHE'} eq 'on'
 && $smoothinfosettings{'MESSAGES'} eq 'on') {$smoothinfosettings{'CHECKALL'} = 'off'}

if ($smoothinfosettings{'CLIENTIP'} ne '') {
open (TMP,">$MODDIR/etc/clientip") || die 'Unable to open file';
print TMP "$smoothinfosettings{'CLIENTIP'}";
delete $smoothinfosettings{'CLIENTIP'};
close (TMP);
}
else
{
unlink ("$MODDIR/etc/clientip");
}

open (OUT, ">",\$smoothinfosettings{'CHAINS'});
foreach (@chains) {
if ($smoothinfosettings{$_} eq 'on') {
print OUT "$_,";}
}

if ($smoothinfosettings{'OTHER'} ne '') {
if ($smoothinfosettings{'SECTIONTITLE'} eq '') { $errormessage = $tr{'smoothinfo-no-section-title'}; }
open (TMP,">$MODDIR/etc/otherinfo") || die 'Unable to open file';
print TMP "$smoothinfosettings{'SECTIONTITLE'}\n";
print TMP "$smoothinfosettings{'OTHER'}";
delete $smoothinfosettings{'SECTIONTITLE'};
delete $smoothinfosettings{'OTHER'};
close (TMP);
}
else
{
unlink ("$MODDIR/etc/otherinfo");
}

unless ($errormessage) {
delete ($smoothinfosettings{'data'});
delete ($smoothinfosettings{'CHECKALL'});
delete ($smoothinfosettings{'CHECKDEFAULT'});
foreach (@chains) { delete ($smoothinfosettings{$_}) }
&writehash("$settingsfile", \%smoothinfosettings);

my $success = message('smoothinfogenerate');

if (not defined $success) {
$errormessage = $tr{'smoothd failure'};
	}
}
}

&readhash("$settingsfile", \%smoothinfosettings);

undef $smoothinfosettings{'LINES'};
undef $smoothinfosettings{'LINES2'};
undef $smoothinfosettings{'LINES3'};
undef $smoothinfosettings{'STRING'};
undef $smoothinfosettings{'STRING2'};
undef $smoothinfosettings{'STRING3'};
undef $smoothinfosettings{'SCREENSHOTS'};

foreach (@items,@ASCII_items)
	{
	$checked{$_}{'off'} = '';
	$checked{$_}{'on'} = '';
	$checked{$_}{$smoothinfosettings{$_}} = 'CHECKED';
	}

$checked{'EDIT'}{'off'} = '';
$checked{'EDIT'}{'on'} = '';
$checked{'EDIT'}{$smoothinfosettings{EDIT}} = 'CHECKED';

$selected{'HEADORTAIL'}{'HEAD'} = '';
$selected{'HEADORTAIL'}{'TAIL'} = '';
$selected{'HEADORTAIL'}{$smoothinfosettings{'HEADORTAIL'}} = 'CHECKED';

$selected{'HEADORTAIL2'}{'HEAD2'} = '';
$selected{'HEADORTAIL2'}{'TAIL2'} = '';
$selected{'HEADORTAIL2'}{$smoothinfosettings{'HEADORTAIL2'}} = 'CHECKED';

$selected{'HEADORTAIL3'}{'HEAD3'} = '';
$selected{'HEADORTAIL3'}{'TAIL3'} = '';
$selected{'HEADORTAIL3'}{$smoothinfosettings{'HEADORTAIL3'}} = 'CHECKED';

&showhttpheaders();

&openpage($tr{'smoothinfo-smoothinfo'}, 1, '', 'tools');

&openbigbox('100%', 'LEFT');
print <<END
<script language="javascript" type="text/javascript">

	function toggle(Id)
	{
		var el = document.getElementById(Id);
		if ( el.style.display != 'none' )
		{
		el.style.display = 'none';
		}
		else 
		{
		el.style.display = '';
		}
	}
	
	
// Toggle the state of the arrow on the buttons. Down when settings are hidden. Up when they are visible.
// Ugly I admit :-( ...

function ToggleImage()
{
	if ( document.getElementById('1').style.display == 'none' )
		{
		url="url('/ui/img/down.jpg')";
		document.myform.SCHEMATIC.style.backgroundImage = url;
		document.myform.SCHEMATIC.style.backgroundPosition = '93% 50%';
		document.myform.SCHEMATIC.style.backgroundRepeat = 'no-repeat';
		document.myform.SCHEMATIC.style.backgroundColor = '#cdcdcd';
		document.myform.SCHEMATIC.style.textAlign = 'left';
		}
		else
		{
		url="url('/ui/img/up.jpg')";
		document.myform.SCHEMATIC.style.backgroundImage = url;
		document.myform.SCHEMATIC.style.backgroundPosition = '93% 50%';
		document.myform.SCHEMATIC.style.backgroundRepeat = 'no-repeat';
		document.myform.SCHEMATIC.style.backgroundColor = '#cdcdcd';
		document.myform.SCHEMATIC.style.textAlign = 'left';
		}
	if ( document.getElementById('2').style.display == 'none' )
		{
		url="url('/ui/img/down.jpg')";
		document.myform.CLIENT.style.backgroundImage = url;
		document.myform.CLIENT.style.backgroundPosition = '93% 50%';
		document.myform.CLIENT.style.backgroundRepeat = 'no-repeat';
		document.myform.CLIENT.style.backgroundColor = '#cdcdcd';
		document.myform.CLIENT.style.textAlign = 'left';
		}
		else
		{
		url="url('/ui/img/up.jpg')";
		document.myform.CLIENT.style.backgroundImage = url;
		document.myform.CLIENT.style.backgroundPosition = '93% 50%';
		document.myform.CLIENT.style.backgroundRepeat = 'no-repeat';
		document.myform.CLIENT.style.backgroundColor = '#cdcdcd';
		document.myform.CLIENT.style.textAlign = 'left';
		}
	if ( document.getElementById('3').style.display == 'none' )
		{
		url="url('/ui/img/down.jpg')";
		document.myform.IPTABLES.style.backgroundImage = url;
		document.myform.IPTABLES.style.backgroundPosition = '93% 50%';
		document.myform.IPTABLES.style.backgroundRepeat = 'no-repeat';
		document.myform.IPTABLES.style.backgroundColor = '#cdcdcd';
		document.myform.IPTABLES.style.textAlign = 'left';
		}
		else
		{
		url="url('/ui/img/up.jpg')";
		document.myform.IPTABLES.style.backgroundImage = url;
		document.myform.IPTABLES.style.backgroundPosition = '93% 50%';
		document.myform.IPTABLES.style.backgroundRepeat = 'no-repeat';
		document.myform.IPTABLES.style.backgroundColor = '#cdcdcd';
		document.myform.IPTABLES.style.textAlign = 'left';
		}
	if ( document.getElementById('4').style.display == 'none' )
		{
		url="url('/ui/img/down.jpg')";
		document.myform.EXTRA.style.backgroundImage = url;
		document.myform.EXTRA.style.backgroundPosition = '93% 50%';
		document.myform.EXTRA.style.backgroundRepeat = 'no-repeat';
		document.myform.EXTRA.style.backgroundColor = '#cdcdcd';
		document.myform.EXTRA.style.textAlign = 'left';
		}
		else
		{
		url="url('/ui/img/up.jpg')";
		document.myform.EXTRA.style.backgroundImage = url;
		document.myform.EXTRA.style.backgroundPosition = '93% 50%';
		document.myform.EXTRA.style.backgroundRepeat = 'no-repeat';
		document.myform.EXTRA.style.backgroundColor = '#cdcdcd';
		document.myform.EXTRA.style.textAlign = 'left';
		}
}

window.onload = function() { ToggleImage(); }
	
	function CheckAll() 
	{
		if(document.myform.CHECKALL.checked)
		{
END
;
foreach (@items) { print "document.myform.$_.checked = true;\n"; }
print <<END
		}
		else
		{
END
;
foreach (@items) { print "document.myform.$_.checked = false;\n"; }
print <<END
		}
	}
	
	function CheckDef() 
	{
		if(document.myform.CHECKDEFAULT.checked)
		{
END
;
for ('MEMORY', 'DISKSPACE', 'CONNTYPE', 'NETCONF1', 'NETCONF2', 'CONFIG', 'SERVICES') { print "document.myform.$_.checked = true;\n"; }
print <<END
		}
		else
		{
END
;
for ('MEMORY', 'DISKSPACE', 'CONNTYPE', 'NETCONF1', 'NETCONF2', 'CONFIG', 'SERVICES') { print "document.myform.$_.checked = false;\n"; }
print <<END
		}
	}
	
		function CheckAllChains() 
	{
		if(document.myform.ALLCHAINS.checked)
		{
END
;
foreach (@chains) { next if /All chains/; print "document.myform.$_.checked = true;\n"; }
print <<END
		}
		else
		{
END
;
foreach (@chains) { next if /All chains/; print "document.myform.$_.checked = false;\n"; }
print <<END
		}
	}
	
	function SwitchOrWAP() 
	{
		if(document.myform.WAP1.checked)
		{
			document.myform.SWITCH1.disabled = true;
			document.myform.WAP4.disabled = true;
		}
		else
		{
			document.myform.SWITCH1.disabled = false;
			document.myform.WAP4.disabled = false;
		}
		if(document.myform.SWITCH1.checked)
		{
			document.myform.WAP1.disabled = true;
		}
		else
		{
			document.myform.WAP1.disabled = false;
		}
		if(document.myform.WAP2.checked)
		{
			document.myform.SWITCH2.disabled = true;
			document.myform.WAP3.disabled = true;
		}
		else
		{
			document.myform.SWITCH2.disabled = false;
			document.myform.WAP3.disabled = false;
		}
		if(document.myform.SWITCH2.checked)
		{
			document.myform.WAP2.disabled = true;
		}
		else
		{
			document.myform.WAP2.disabled = false;
		}
		if(document.myform.WAP6.checked)
		{
			document.myform.SWITCH3.disabled = true;
			document.myform.WAP5.disabled = true;
		}
		else
		{
			document.myform.SWITCH3.disabled = false;
			document.myform.WAP5.disabled = false;
		}
		if(document.myform.SWITCH3.checked)
		{
			document.myform.WAP6.disabled = true;
		}
		else
		{
			document.myform.WAP6.disabled = false;
		}
	}
	
	function selectAll(field)
	{
	var tempval=eval("document."+field)
	tempval.focus()
	tempval.select()
	}
</script>

END
;

&alertbox($errormessage);

print <<END
<FORM METHOD='POST' NAME='myform'>
<DIV ALIGN='CENTER'>
<BR>
<table style='width: 95%; border:solid 1px; border-color:orange; background-color:#f9f0c7; margin:0px; padding:4px;'>
    <TR>
		<TD style='font-size:120%;'>$tr{'smoothinfo-caution'}</TD>
	</TR>
</TABLE>
</DIV>
END
;

&openbox($tr{'smoothinfo-include'});

print <<END
<BR>
<TABLE WIDTH='100%'>
    <TR>
		<TD WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-config-tip'}'>$tr{'smoothinfo-config'}</TD>
		<TD><INPUT TYPE='checkbox' ALIGN='LEFT' NAME='CONFIG' VALUE='on' $checked{'CONFIG'}{'on'}></TD>
		<TD WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-connection-tip'}'>$tr{'smoothinfo-connection'}</TD>
        <TD><INPUT TYPE='checkbox' NAME='CONNTYPE' VALUE='on' $checked{'CONNTYPE'}{'on'}></TD>
	</TR>
	<TR>
		<TD WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-adapters-tip'}'>$tr{'smoothinfo-adapters'}</TD>
		<TD><INPUT TYPE='checkbox' ALIGN='LEFT' NAME='ADAPTERS' $checked{'ADAPTERS'}{'on'}></TD>
		<TD WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-ifconfig-tip'}'>$tr{'smoothinfo-ifconfig'}</TD>
        <TD><INPUT TYPE='checkbox' NAME='NETCONF1' VALUE='on' $checked{'NETCONF1'}{'on'}></TD>
	</TR>
		<TD  WIDTH='30%' CLASS='base' TITLE="$tr{'smoothinfo-net settings-tip'}">$tr{'smoothinfo-net settings'}</TD>
		<TD><INPUT TYPE='checkbox' ALIGN='LEFT' NAME='NETCONF2' $checked{'NETCONF2'}{'on'}></TD>
		<TD  WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-routes-tip'}'>$tr{'smoothinfo-routes'}</TD>
		<TD><INPUT TYPE='checkbox' ALIGN='LEFT' NAME='ROUTE' $checked{'ROUTE'}{'on'}></TD>
    <TR>
		<TD  WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-dhcpinfo-tip'}'>$tr{'smoothinfo-dhcpinfo'}</TD>
		<TD><INPUT TYPE='checkbox' ALIGN='LEFT' NAME='DHCPINFO' $checked{'DHCPINFO'}{'on'}></TD>
		<TD  WIDTH='30%' CLASS='base'>$tr{'smoothinfo-portfw'}</TD>
		<TD><INPUT TYPE='checkbox' ALIGN='LEFT' NAME='PORTFW' $checked{'PORTFW'}{'on'}></TD>
	</TR>
	<TR>
		<TD WIDTH='30%' CLASS='base'>$tr{'smoothinfo-cpu'}</TD>
        <TD><INPUT TYPE='checkbox' NAME='CPU' $checked{'CPU'}{'on'}></TD>
		<TD WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-irqs-tip'}'>$tr{'smoothinfo-irqs'}</TD>
		<TD><INPUT TYPE='checkbox' ALIGN='LEFT' NAME='IRQs' $checked{'IRQs'}{'on'}></TD>
	</TR>
		<TD WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-memory-tip'}'>$tr{'smoothinfo-memory'}</TD>
		<TD><INPUT TYPE='checkbox' ALIGN='LEFT' NAME='MEMORY' $checked{'MEMORY'}{'on'}></TD>
		<TD WIDTH='30%' CLASS='base'>$tr{'smoothinfo-diskspace'}</TD>
		<TD><INPUT TYPE='checkbox' ALIGN='LEFT' NAME='DISKSPACE' $checked{'DISKSPACE'}{'on'}></TD>
	<TR>
	</TR>
	<TR>
		<TD WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-mods-tip'}'>$tr{'smoothinfo-installed-mods'}</TD>
        <TD><INPUT TYPE='checkbox' NAME='MODLIST' $checked{'MODLIST'}{'on'}></TD>
		<TD WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-modules-tip'}'>$tr{'smoothinfo-modules'}</TD>
        <TD><INPUT TYPE='checkbox' NAME='LOADEDMODULES' $checked{'LOADEDMODULES'}{'on'}></TD>
	</TR>
	<TR>
		<TD WIDTH='30%' CLASS='base' TITLE="$tr{'smoothinfo-top-tip'}">$tr{'smoothinfo-top'}</TD>
        <TD><INPUT TYPE='checkbox' NAME='TOP' $checked{'TOP'}{'on'}></TD>
		<TD WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-conntracks-tip'}'>$tr{'smoothinfo-conntracks'}</TD>
		<TD><INPUT TYPE='checkbox' ALIGN='LEFT' NAME='CONNTRACKS' $checked{'CONNTRACKS'}{'on'}></TD>
	</TR>
	<TR>
		<TD WIDTH='30%' CLASS='base' TITLE='$tr{'smoothinfo-services-status-tip'}'>$tr{'smoothinfo-services-status'}</TD>
        <TD><INPUT TYPE='checkbox' NAME='SERVICES' $checked{'SERVICES'}{'on'}></TD>
		<TD WIDTH='30%' CLASS='base' TITLE="$tr{'smoothinfo-mod-services-status-tip'}">$tr{'smoothinfo-mod-services-status'}</TD>
        <TD><INPUT TYPE='checkbox' NAME='MODSERVICES' $checked{'MODSERVICES'}{'on'}></TD>
	</TR>
</TABLE>
<BR>
<FIELDSET style='border-style: solid; border-width : 1px; border-color: grey;'><LEGEND><B>&nbsp LOGS &nbsp<A href='#'><IMG SRC='/ui/img/help.gif' ALT='?' TITLE='$tr{'smoothinfo-log-help'}' VALIGN='top' onClick="javascript:toggle('help'); return false;"></A></B></LEGEND>
<TABLE WIDTH='100%'>
	<TR>
		<TD WIDTH='12%' TITLE='$tr{'smoothinfo-dmesg-tip'}'>$tr{'smoothinfo-dmesg'}</TD>
		<TD WIDTH='2%'><INPUT TYPE='checkbox' NAME='DMESG' $checked{'DMESG'}{'on'}></TD>
		<TD WIDTH='2%'>$tr{'smoothinfo-head'}</TD>
		<TD WIDTH='2%'><INPUT TYPE='radio' NAME='HEADORTAIL' VALUE='HEAD' $selected{'HEADORTAIL'}{'HEAD'}></TD
		<TD WIDTH='2%'>$tr{'smoothinfo-tail'}</TD>
		<TD WIDTH='2%'><INPUT TYPE='radio' NAME='HEADORTAIL' VALUE='TAIL' $selected{'HEADORTAIL'}{'TAIL'}></TD>
		<TD WIDTH='14%'>$tr{'smoothinfo-lines'}&nbsp <IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'></TD>
		<TD WIDTH='3%'><INPUT TYPE='text' NAME='LINES' VALUE='$smoothinfosettings{'LINES'}' size='2' TITLE='$tr{'smoothinfo-apache-lines-tip'}'></TD>
		<TD WIDTH='14%'>$tr{'smoothinfo-search'}&nbsp <IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'></TD>
		<TD WIDTH='10%'><INPUT TYPE='text' NAME='STRING' VALUE='$smoothinfosettings{'STRING'}' size='10'></TD>
		<TD WIDTH='8%'>$tr{'smoothinfo-ignore-case'}</TD>
		<TD WIDTH='2%'><INPUT TYPE='checkbox' NAME='IGNORECASE' $checked{'IGNORECASE'}{'on'}></TD>
	</TR>
	<TR>
		<TD WIDTH='12%' TITLE='$tr{'smoothinfo-apache-error-tip'}'>$tr{'smoothinfo-apache-error'}</TD>
        <TD WIDTH='2%'><INPUT TYPE='checkbox' NAME='APACHE' $checked{'APACHE'}{'on'}></TD>
		<TD WIDTH='2%'>$tr{'smoothinfo-head'}</TD>
		<TD WIDTH='2%'><INPUT TYPE='radio' NAME='HEADORTAIL2' VALUE='HEAD2' $selected{'HEADORTAIL2'}{'HEAD2'}></TD>
        <TD WIDTH='2%'>$tr{'smoothinfo-tail'}</TD>
		<TD WIDTH='2%'><INPUT TYPE='radio' NAME='HEADORTAIL2' VALUE='TAIL2' $selected{'HEADORTAIL2'}{'TAIL2'}></TD>
		<TD WIDTH='14%'>$tr{'smoothinfo-lines'}&nbsp <IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'><IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'></TD>
		<TD WIDTH='3%'><INPUT TYPE='text' NAME='LINES2' VALUE='$smoothinfosettings{'LINES2'}' size='2' TITLE='$tr{'smoothinfo-apache-lines-tip'}'></TD>
		<TD WIDTH='14%'>$tr{'smoothinfo-search'}&nbsp <IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'><IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'></TD>
		<TD WIDTH='10%'><INPUT TYPE='text' NAME='STRING2' VALUE='$smoothinfosettings{'STRING2'}' size='10'></TD>
		<TD WIDTH='8%'>$tr{'smoothinfo-ignore-case'}</TD>
		<TD WIDTH='2%'><INPUT TYPE='checkbox' NAME='IGNORECASE2' $checked{'IGNORECASE2'}{'on'}></TD>
	</TR>
	<TR>
		<TD WIDTH='12%' TITLE='$tr{'smoothinfo-system-tip'}'>$tr{'smoothinfo-system'}</TD>
        <TD WIDTH='2%'><INPUT TYPE='checkbox' NAME='MESSAGES' $checked{'MESSAGES'}{'on'}></TD>
		<TD WIDTH='2%'>$tr{'smoothinfo-head'}</TD>
		<TD WIDTH='2%'><INPUT TYPE='radio' NAME='HEADORTAIL3' VALUE='HEAD3' $selected{'HEADORTAIL3'}{'HEAD3'}></TD>
        <TD WIDTH='2%'>$tr{'smoothinfo-tail'}</TD>
		<TD WIDTH='2%'><INPUT TYPE='radio' NAME='HEADORTAIL3' VALUE='TAIL3' $selected{'HEADORTAIL3'}{'TAIL3'}></TD>
		<TD WIDTH='14%'>$tr{'smoothinfo-lines'}&nbsp <IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'><IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'></TD>
		<TD WIDTH='3%'><INPUT TYPE='text' NAME='LINES3' VALUE='$smoothinfosettings{'LINES3'}' size='2' TITLE='$tr{'smoothinfo-apache-lines-tip'}'></TD>
		<TD WIDTH='14%'>$tr{'smoothinfo-search'}&nbsp <IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'><IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'></TD>
		<TD WIDTH='10%'><INPUT TYPE='text' NAME='STRING3' VALUE='$smoothinfosettings{'STRING3'}' size='10'></TD>
		<TD WIDTH='8%'>$tr{'smoothinfo-ignore-case'}</TD>
		<TD WIDTH='2%'><INPUT TYPE='checkbox' NAME='IGNORECASE3' $checked{'IGNORECASE3'}{'on'}></TD>
	</TR>
</TABLE>
</FIELDSET>

<DIV ALIGN='CENTER' Id='help' style='display: none'>
<BR>
<table style='width: 99%; border:dotted 1px; background-color:#ffee88; margin:0px; padding:4px;'>
<TR>
	<TD width='15%' style='font-size:100%; font-style:italic;'>$tr{'smoothinfo-dmesg'}</TD>
	<TD style='font-size:100%;'>$tr{'smoothinfo-dmesg-help'}</TD>
</TR>
<TR>
	<TD width='15%' style='font-size:100%; font-style:italic;'>$tr{'smoothinfo-apache-error'}</TD>
	<TD style='font-size:100%;'>$tr{'smoothinfo-apache-error-help1'}</TD>
</TR>
<TR>
	<TD width='15%'>&nbsp</TD>
	<TD style='font-size:100%;'>$tr{'smoothinfo-apache-error-help2'}</TD>
</TR>
<TR>
	<TD width='15%' style='font-size:100%; font-style:italic;'>$tr{'smoothinfo-system'}</TD>
	<TD style='font-size:100%;'>$tr{'smoothinfo-system-log-help'}</TD>
</TR>
</TABLE>
</DIV
END
;

print <<END
<TABLE WIDTH='100%'>
	<TR>
	<TD>&nbsp</TD>
	</TR>
</TABLE>
<TABLE style='width: 100%; border-spacing:0px 0px; border:none; text-indent:5px; margin:0px; padding:4px;'>
	<TR>
		<TD WIDTH='12%' CLASS='base' BGCOLOR='lightgrey'>$tr{'smoothinfo-checkdefault'}</TD>
        <TD WIDTH='3%' ALIGN='LEFT' BGCOLOR='lightgrey'><INPUT TYPE='checkbox' NAME='CHECKDEFAULT' $checked{'CHECKDEFAULT'}{'on'} onClick='javaScript:CheckDef();'></TD>
		<TD WIDTH='10%' CLASS='base' BGCOLOR='lightgrey'>$tr{'smoothinfo-checkall'}</TD>
        <TD WIDTH='3%' ALIGN='LEFT' BGCOLOR='lightgrey'><INPUT TYPE='checkbox' NAME='CHECKALL' $checked{'CHECKALL'}{'on'} onClick='javaScript:CheckAll();'></TD>
		<TD WIDTH='50%' ALIGN='RIGHT' TITLE='$tr{'smoothinfo-wrap-tip'}'>$tr{'smoothinfo-wrap-prefix'}</TD>
		<TD WIDTH='15%' ALIGN='CENTER' TITLE='$tr{'smoothinfo-wrap-tip'}'><INPUT TYPE='text' NAME='WRAP' VALUE='$smoothinfosettings{'WRAP'}' size='1'>$tr{'smoothinfo-wrap-suffix'}</TD>
	</TR>
</TABLE>
<BR>
<TABLE>
	<TR>
	<TD WIDTH='20%' CLASS='base'>Link(s) to screenshot(s):</TD>
	<TD WIDTH='25%'><INPUT TYPE='text' NAME='SCREENSHOTS' VALUE='$smoothinfosettings{'SCREENSHOTS'}' size='80' TITLE='$tr{'smoothinfo-screenshots-tip'}'></TD>
	<TD></TD>
	<TD></TD>
	</TR>
</TABLE>
<BR>
<DIV ALIGN='CENTER'>
<TABLE WIDTH='95%' CELLSPACING='0'>
	<TR>
		<TD WIDTH='25%' ALIGN='CENTER' CLASS='base'><INPUT TYPE='button' NAME='SCHEMATIC' Id='schematic' value='&nbsp $tr{'smoothinfo-schematic'}' &nbsp" style="width: 160px;"  onClick="javascript:toggle('1');javascript:ToggleImage();" /></TD>
		<TD WIDTH='25%' ALIGN='CENTER'><INPUT TYPE='button' NAME='CLIENT' Id='client' VALUE='$tr{'smoothinfo-clientinfo'}' &nbsp" style="width: 160px;"  onClick="javascript:toggle('2');javascript:ToggleImage();" /></TD>
		<TD WIDTH='25%' ALIGN='CENTER'><INPUT TYPE='button' NAME='IPTABLES' Id='iptables' VALUE='&nbsp &nbsp $tr{'smoothinfo-iptables'}' &nbsp" style="width: 160px;"  onClick="javascript:toggle('3');javascript:ToggleImage();" /></TD>
        <TD WIDTH='25%' ALIGN='CENTER'><INPUT TYPE='button' NAME='EXTRA' Id='other' VALUE='&nbsp &nbsp $tr{'smoothinfo-other'}' style="width: 160px;"  onClick="javascript:toggle('4');javascript:ToggleImage();" /></TD>
	</TR>
</TABLE>
</DIV>
END
;
&closebox();

print <<END
<DIV Id='1' style="display:none;">
END
;
&openbox();
print <<END
<b>$tr{'smoothinfo-shematic-items'}</b>
<CENTER>
<BR><b>Internet</b>
<BR>|<BR>
Modem <INPUT TYPE='checkbox' NAME='MODEM' $checked{'MODEM'}{'on'}>
<BR>|<BR>
Router <INPUT TYPE='checkbox' NAME='ROUTER' $checked{'ROUTER'}{'on'}>
<BR>|<BR>
(<font color='red'><b>red</b></font>)
<BR>|<BR><b>W<code>/</code>Lan</b> 
<=== Switch<INPUT TYPE='checkbox' NAME='SWITCH2' $checked{'SWITCH2'}{'on'} onClick='javaScript:SwitchOrWAP();' />WAP<INPUT TYPE='checkbox' NAME='WAP2' $checked{'WAP2'}{'on'} onClick='javaScript:SwitchOrWAP();' /<=== (<font color='orange'><b>Orange</b></font>)-<big><b>[SMOOTHWALL]</b></big>-(<font color='green'><b>green</b></font>) ===> WAP<INPUT TYPE='checkbox' NAME='WAP1' $checked{'WAP1'}{'on'} onClick='javaScript:SwitchOrWAP();' />Switch<INPUT TYPE='checkbox' NAME='SWITCH1' $checked{'SWITCH1'}{'on'} onClick='javaScript:SwitchOrWAP();' /> ===> <b>W<code>/</code>Lan</b><BR>|&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp|&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp|<BR><INPUT TYPE='checkbox' NAME='WAP3' $checked{'WAP3'}{'on'} onClick='javaScript:SwitchOrWAP();' />&nbsp WAP &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp  &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp(<font color='purple'><b>Purple</b></font>)&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp WAP <INPUT TYPE='checkbox' NAME='WAP4' $checked{'WAP4'}{'on'} onClick='javaScript:SwitchOrWAP();' /><BR>|&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp|&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp|<BR>
<b>WLan</b>&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp WAP <INPUT TYPE='checkbox' NAME='WAP5' $checked{'WAP5'}{'on'}><=== Switch<INPUT TYPE='checkbox' NAME='SWITCH3' $checked{'SWITCH3'}{'on'} onClick='javaScript:SwitchOrWAP();' />WAP<INPUT TYPE='checkbox' NAME='WAP6' $checked{'WAP6'}{'on'} onClick='javaScript:SwitchOrWAP();' />&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp<b>WLan</b><BR>|&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp|&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp <BR><b>&nbsp WLan</b>&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp <b>W<code>/</code>Lan</b>&nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp <BR>
</CENTER>
END
;
&closebox();
print <<END
</DIV>
END
;

print <<END
<DIV Id='2' style="display:none;">
END
;
&openbox();
print <<END
<b>$tr{'smoothinfo-clientIP'}</b>
<BR><BR><FONT COLOR='red'><CENTER>$tr{'smoothinfo-warn'}</CENTER></FONT>
<TABLE WIDTH='100%'>
<TR>
 <TD ALIGN='CENTER' WIDTH='50%'><TEXTAREA NAME='CLIENTIP' ROWS='10' COLS='70' WRAP='off' TITLE='$tr{'smoothinfo-clientIP-tip'}'>
</TEXTAREA></TD>
</TR>
</TABLE>
END
;
&closebox();
print <<END
</DIV>
END
;

print <<END
<DIV Id='4' style="display:none;">
END
;
&openbox("$tr{'smoothinfo-other'}:");
print <<END
<BR>
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='9%'><b>$tr{'smoothinfo-other-title'}</b></TD><TD WIDTH='91%'><INPUT TYPE='text' NAME='SECTIONTITLE' Id='sectiontitle' VALUE='$smoothinfosettings{'SECTIONTITLE'}'  @{[jsvalidregex('sectiontitle','^[a-zA-Z0-9-_., ]+$')]} size='20'>&nbsp <i>(required)</i></TD>
</TR>
</TABLE>
<TABLE WIDTH='100%'>
<TR>
	<TD ALIGN='CENTER' WIDTH='50%'><TEXTAREA NAME='OTHER' ROWS='10' COLS='70' WRAP='off' TITLE='$tr{'smoothinfo-other-tip'}'></TEXTAREA></TD>
</TR>
</TABLE>
END
;
&closebox();
print <<END
</DIV>
END
;

print <<END
<DIV Id='3' style="display:none;">
END
;
&openbox($tr{'smoothinfo-known-chains'});
print "$tr{'smoothinfo-chains'}";
my @rows = ();
	print "<table style='width: 100%;'>";
	$id = -1;
	foreach (@chains) 
	{
		$id++;
		if (/All chains/)
			{ push @rows, qq[<td style='width: 15%; color: red; font-weight: bold;'>$_</td><td style='width: 20%;'><INPUT TYPE='checkbox' NAME='ALLCHAINS' \$checked{'ALLCHAINS'}{'on'} onClick='javaScript:CheckAllChains();' /></td>\n];
			}
			else
			{
			push @rows, qq[<td style='width: 15%;'>$_</td><td style='width: 20%;'><INPUT TYPE='checkbox' NAME='$_' \$checked{'$_'}{'on'}></td>\n];
			}
	}
	# 3colums
	for(my $id = 0; $id <= $#rows; $id += 3)
	{
		$rows[$id+1] = '&nbsp;' unless defined $rows[$id+1];
		$rows[$id+2] = '&nbsp;' unless defined $rows[$id+2];
		print "<tr>" . $rows[$id] . $rows[$id+1] . $rows[$id+2] . "</tr>\n";

	}
	print "</table>";
&closebox();
print <<END
</DIV>
END
;

print <<END
<DIV ALIGN='CENTER'>
<TABLE WIDTH='60%'>
 <TR>
 <TD ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'smoothinfo-generate'}' style="height:25px;background-color:#fdb445;font: bold 12px Arial;" onClick="if(confirm('$tr{'smoothinfo-confirm'}')) {return true;} return false;"></TD>
 </TR>
</TABLE>
</DIV>
END
;

if ($smoothinfosettings{'EDIT'} eq 'on')
{
$textarea = "<TD ALIGN='CENTER' WIDTH='50%'><TEXTAREA NAME='data' ROWS='30' COLS='70' WRAP='off'>";
$bbcodehelp = "<TD ALIGN='RIGHT'><sup><small><i><A HREF='http://community.smoothwall.org/forum/faq.php?mode=bbcode' onclick=\"window.open(this.href,'popup','height=600 ,width=800, scrollbars=yes, left=150,top=150,screenX=150,screenY=150');return false;\" TITLE='$tr{'smoothinfo-connected'}'>$tr{'smoothinfo-bbcode'}&nbsp</a></i></small></sup></TD>";
}
else
{
$textarea = "<TD ALIGN='CENTER' WIDTH='50%'><TEXTAREA NAME='data' ROWS='30' COLS='70' WRAP='off' READONLY='yes' TITLE='$tr{'smoothinfo-report-tip'}' onclick='this.select();' style='background:#ecece8;'>";
$bbcodehelp = '';
}

&openbox($tr{'smoothinfo-report'});
print <<END
<BR>
<DIV ALIGN='CENTER'>
<TABLE WIDTH='60%'>
<TR>
	<TD CLASS='base' ALIGN='LEFT' TITLE='$tr{'smoothinfo-edit-tip'}'>$tr{'smoothinfo-edit'}&nbsp<INPUT TYPE='checkbox' NAME='EDIT' $checked{'EDIT'}{'on'}></TD>
$bbcodehelp
</TR>
</TABLE>
</DIV>
<DIV ALIGN='CENTER'>
<TABLE WIDTH='60%'>
<TR>
	<TD ALIGN='CENTER'><a href="javascript:selectAll('myform.data')" style='font-size:120%; color:red; font-weight:bold;' TITLE='$tr{'smoothinfo-selectall-tip'}'>Select All</a></TD>
</TR>
</TABLE>
</DIV>
<TABLE WIDTH='100%'>
<TR>
$textarea
END
;
open (REPORT, "<$filename") or die "unable to open report";
while (<REPORT>) {
chomp;
print "$_\n";
}
close REPORT;
print <<END
</TEXTAREA></TD>
</TR>
</TABLE>
<BR>
END
;
&closebox();

print "</FORM>\n";

&closebigbox();

print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD><IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'><FONT CLASS='base'>&nbsp $tr{'this field may be blank'}</FONT></TD>
</TR>
</TABLE>
<TABLE WIDTH='100%'>
<TR>
	<TD><IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'><IMG SRC='/ui/img/blob.gif' ALT='*' VALIGN='top'><FONT CLASS='base'>&nbsp $tr{'smoothinfo-both-fields-cannot-be-blank'}</FONT></TD>
	<TD ALIGN='right'><sup><small><a href='http://community.smoothwall.org/forum/viewforum.php?f=54' title='$tr{'smoothinfo-link-tip'}'  target='_blank'>$version &nbsp</a></small></sup></TD>
</TR>
</TABLE>
END
;
&closepage();
