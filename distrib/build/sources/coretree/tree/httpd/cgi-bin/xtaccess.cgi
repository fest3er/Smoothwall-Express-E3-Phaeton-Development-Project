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
#use SmoothInstall qw( :standard );
use Shell qw(rm touch);

#Init("fullfirewall");

my (%cgiparams, %netsettings, %checked, %selected);
my $filename = "${swroot}/xtaccess/config";

&showhttpheaders();

&readhash("${swroot}/ethernet/settings", \%netsettings);

$cgiparams{'ENABLED'} = 'off';
$cgiparams{'COLUMN'} = 1;
$cgiparams{'ORDER'} = $tr{'log ascending'};

&getcgihash(\%cgiparams);

if ($ENV{'QUERY_STRING'} && ( not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ))
{
	my @temp = split(',',$ENV{'QUERY_STRING'});
	$cgiparams{'ORDER'}  = $temp[1] if ( defined $temp[ 1 ] and $temp[ 1 ] ne "" );
	$cgiparams{'COLUMN'} = $temp[0] if ( defined $temp[ 0 ] and $temp[ 0 ] ne "" );
}


my $errormessage = '';

if ($cgiparams{'ACTION'} eq $tr{'add'})
{
	unless( &validipormask($cgiparams{'EXT'}) )
	{
		if ($cgiparams{'EXT'} ne '') {
			$errormessage = $tr{'source ip bad'}; }
		else {
			$cgiparams{'EXT'} = '0.0.0.0/0'; }
	}

	unless ( &validcomment( $cgiparams{'COMMENT'} ) ){ $errormessage = $tr{'invalid comment'}; }
	if ($cgiparams{'PROTOCOL'} eq "6" || $cgiparams{'PROTOCOL'} eq "17") {	
		unless(&validportrange($cgiparams{'DEST_PORT'})) { 
			$errormessage = $tr{'destination port numbers'}; 
		}
	}
	unless ( $cgiparams{'PROTOCOL'} eq "6" or $cgiparams{'PROTOCOL'} eq "17" or $cgiparams{'PROTOCOL'} eq "TCP & UDP" ) {
		if ( $cgiparams{'DEST_PORT'} ) {
			$errormessage = $tr{'ffc-port and protocol'};
		}
	}
	open(FILE, $filename) or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);
	unless ($errormessage)
	{
		if ($cgiparams{'DEST_PORT'} eq "") { $cgiparams{'DEST_PORT'} = '0'; }
		open(FILE,">>$filename") or die 'Unable to open config file.';
		flock FILE, 2;
		if ($cgiparams{'PROTOCOL'} eq "TCP & UDP") {
			print FILE "$cgiparams{'RED_IFACE'},6,$cgiparams{'EXT'},$cgiparams{'DEST_PORT'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
			print FILE "$cgiparams{'RED_IFACE'},17,$cgiparams{'EXT'},$cgiparams{'DEST_PORT'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
		} else {
			print FILE "$cgiparams{'RED_IFACE'},$cgiparams{'PROTOCOL'},$cgiparams{'EXT'},$cgiparams{'DEST_PORT'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
		}
		close(FILE);

		open(FILE, "$filename");
		my @un_sorted = <FILE>;
		close FILE;

		@sorted = sort { $a cmp $b } @un_sorted;

		open(FILE, ">$filename") or die 'Unable to open config file.';
		foreach $line (@sorted) {
			print FILE "$line";
		}
		close FILE;

		undef %cgiparams;
		$cgiparams{'COLUMN'} = 1;
		$cgiparams{'ORDER'} = $tr{'log ascending'};
		
		&log($tr{'external access rule added'});

		my $success = message('setxtaccess');
	
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}
if ($cgiparams{'ACTION'} eq $tr{'remove'} or $cgiparams{'ACTION'} eq $tr{'edit'} or $cgiparams{'ACTION'} eq $tr{'ffc-enable rule'})
{
	open(FILE, "$filename") or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);

	my $count = 0;
	my $id = 0;
	my $line;
	foreach $line (@current)
	{
		$id++;
		if ($cgiparams{$id} eq "on") {
			$count++; }
	}
	if ($count == 0) {
		$errormessage = $tr{'nothing selected'}; }
	if ($count > 1 && $cgiparams{'ACTION'} eq $tr{'edit'}) {
		$errormessage = $tr{'you can only select one item to edit'}; }

	unless ($errormessage)
	{
		open(FILE, ">$filename") or die 'Unable to open config file.';
		flock FILE, 2;
		my $id = 0;
		foreach $line (@current)
		{
			$id++;
			unless ($cgiparams{$id} eq "on") {
				print FILE "$line"; }
			elsif ($cgiparams{'ACTION'} eq $tr{'edit'}) {
				chomp($line);
				my @temp = split(/\,/,$line);
				$cgiparams{'RED_IFACE'} = $temp[0];
				$cgiparams{'PROTOCOL'} = $temp[1];
				$cgiparams{'EXT'} = $temp[2];
				$cgiparams{'DEST_PORT'} = $temp[3];
				$cgiparams{'ENABLED'} = $temp[4];
				$cgiparams{'COMMENT'} = $temp[5];
			} elsif ($cgiparams{'ACTION'} eq $tr{'ffc-enable rule'}) {
				chomp $line;
				my @temp = split(/\,/,$line);
				if ($temp[4] eq "on") {
					$enabled = "off";
				} else {
					$enabled = "on";
				}
				print FILE "$temp[0],$temp[1],$temp[2],$temp[3],$enabled,$temp[5]\n";
			}
		}
		close(FILE);

		&log($tr{'external access rule removed'});

		my $success = message('setxtaccess');
	
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}
if ($cgiparams{'ACTION'} eq '')
{
	$cgiparams{'PROTOCOL'} = '6';
	$cgiparams{'ENABLED'} = 'on';
	$cgiparams{'RED_IFACE'} = $reddev;
}
#
# Red interface and aliases list
#
my $aliasfile = "${swroot}/portfw/aliases";

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
my ($tmp1, @temp);
# Network device listing
my %availablenetdevices;
my @devices = `/sbin/ifconfig`;
foreach $dev (@devices) {
	chomp $dev;
	@temp = split(' ', $dev);
	if ($temp[1] =~ /^Link/) {
		if ($temp[0] =~ /^ipsec/ or $temp[0] =~ /^lo/) {
			next;
		} elsif ($temp[0] =~ /^$reddev/) {
			my @deviceinfo = `ifconfig $temp[0]`;
			foreach $devinfo (@deviceinfo) {
				chomp $devinfo;
				if ($devinfo =~ /inet addr\:(\d{1,3}(\.\d{1,3}){3})/) {
					$availablenetdevices{$temp[0]} = $1;
				}
			}
		} else {
			next;
		}
	}
}

foreach $dev (keys(%availablenetdevices)) {
	my $tmp = grep(/^$dev$/, @filelist);
	if ($tmp > 0) {
		delete($availablenetdevices{$dev});
	}
}

# Protocol listing
open(TMP, "/etc/protocols") or die "Unable to open /etc/protocols\: $!\n";
my @protocols = <TMP>;
close(TMP);

my $line;
my %availableprotocols;
foreach $line (@protocols) { 
	chomp $line;

	if ($line =~ m/^([a-z0-9]|\#\t[0-9]+\t+)/i) {
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

$selected{'PROTOCOL'}{'udp'} = '';
$selected{'PROTOCOL'}{'tcp'} = '';
$selected{'PROTOCOL'}{$cgiparams{'PROTOCOL'}} = 'SELECTED';

$selected{'RED_IFACE'}{''} = '';
foreach $dev (sort(keys(%availablenetdevices))) {
	$selected{'RED_IFACE'}{$dev} = '';
}
$selected{'RED_IFACE'}{$cgiparams{'RED_IFACE'}} = 'SELECTED';

$selected{'IF_ALIAS'}{'RED'} = '';
$selected{'IF_ALIAS'}{$cgiparams{'IF_ALIAS'}} = 'SELECTED';

$checked{'ENABLED'}{'off'} = '';
$checked{'ENABLED'}{'on'} = '';  
$checked{'ENABLED'}{$cgiparams{'ENABLED'}} = 'CHECKED';

&openpage($tr{'external access configuration'}, 1, '', 'networking');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print <<END

<script>
function ffoxSelectUpdate(elmt)
{
        if(!document.all) elmt.style.cssText =
        elmt.options[elmt.selectedIndex].style.cssText;
}
</script>

<STYLE type="text/css">
OPTION.red{color:red;}
OPTION.green{color:green;}
OPTION.orange{color:orange;}
OPTION.purple{color:purple;}
</STYLE>

<!-- 
<CENTER>
<TABLE BORDER='0' CELLPADDING='0' CELLSPACING='0'>
<TR><TD VALIGN='top' ALIGN='CENTER'>
<A HREF='http://www.smoothwall.co.uk/'><IMG
 SRC='/ui/img/inlinepromo.smoothhost.png' BORDER='0'
 ALT='Visit smoothwall.co.uk for enhanced commercial SmoothWall products'></A>
</TD></TR>
</TABLE>
</CENTER>
 -->
END
;

print "<FORM METHOD='POST'>\n";

&openbox($tr{'add a new rule'});
print <<END
<TABLE WIDTH='100%'>
		<TD CLASS='base' WIDTH='25%'>Red interface and aliases:</TD>
              <TD WIDTH='25%'><SELECT style='color: red' onchange="ffoxSelectUpdate(this);" NAME='RED_IFACE'>
END
;

open(FILE, "$aliasfile") or die 'Unable to open alias file';
my @temp3 = <FILE>;
close FILE;

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
	if ($reddev && ($dev =~ /$reddev/)) {
		if ($redaliasip) {
			print "<OPTION style='color: red' VALUE='$dev' $selected{'RED_IFACE'}{$dev}>RED$devifacesub $redaliasip</OPTION>";
		} else {
			print "<OPTION style='color: red' VALUE='$dev' $selected{'RED_IFACE'}{$dev}>RED$devifacesub - $dev</OPTION>";
		}
	} else {
		print "<OPTION style='color: black' VALUE='$dev' $selected{'RED_IFACE'}{$dev}>$dev</OPTION>";
	}
}

print <<END
		</SELECT></TD>
		<TD WIDTH='25%'>&nbsp;</TD>
		<TD WIDTH='25%'>&nbsp;</TD>
</TR>
<TR>
	<TD WIDTH='25%' CLASS='base'>$tr{'protocol long'}</TD>
	<TD WIDTH='25%'><SELECT NAME='PROTOCOL'>
END
;
	if ($cgiparams{'PROTOCOL'} eq 'all') {
		print "<OPTION VALUE='all' SELECTED>All</OPTION>";
	} else {
		print "<OPTION VALUE='all'>All</OPTION>";
	}

	if ($cgiparams{'PROTOCOL'} eq '6') {
		print "<OPTION VALUE='6' SELECTED>$availableprotocols{6}</OPTION>";
	} else {
		print "<OPTION VALUE='6'>$availableprotocols{6}</OPTION>";
	}

	if ($cgiparams{'PROTOCOL'} eq '17') {
		print "<OPTION VALUE='17' SELECTED>$availableprotocols{17}</OPTION>";
	} else {
		print "<OPTION VALUE='17'>$availableprotocols{17}</OPTION>";
	}

	if ($cgiparams{'PROTOCOL'} eq 'TCP & UDP') {
		print "<OPTION VALUE='TCP & UDP' SELECTED>TCP & UDP</OPTION>";
	} else {
		print "<OPTION VALUE='TCP & UDP'>TCP & UDP</OPTION>";
	}

	if ($cgiparams{'PROTOCOL'} eq '1') {
		print "<OPTION VALUE='1' SELECTED>$availableprotocols{1}</OPTION>";
	} else {
		print "<OPTION VALUE='1'>$availableprotocols{1}</OPTION>";
	}

	if ($cgiparams{'PROTOCOL'} eq '47') {
		print "<OPTION VALUE='47' SELECTED>$availableprotocols{47}</OPTION>";
	} else {
		print "<OPTION VALUE='47'>$availableprotocols{47}</OPTION>";
	}

	foreach $protocolnumber (@sortedprotocols) {
		if (($protocolnumber != 1) && ($protocolnumber != 6) && ($protocolnumber != 17) && ($protocolnumber != 47)) {
			if ($cgiparams{'PROTOCOL'} == $protocolnumber) {
				print "<OPTION VALUE='$protocolnumber' SELECTED>$availableprotocols{$protocolnumber}</OPTION>";
			} else {
				print "<OPTION VALUE='$protocolnumber'>$availableprotocols{$protocolnumber}</OPTION>";
			}
		}
	}

	print <<END
	</SELECT></TD>
<TD WIDTH='25%' CLASS='base'><FONT COLOR='$colourred'>$tr{'sourcec'}</FONT></TD>
<TD WIDTH='25%'><INPUT TYPE='TEXT' NAME='EXT' VALUE='$cgiparams{'EXT'}' SIZE='32' id='ext' @{[jsvalidipormask('ext')]}></TD>
</TR>
<TR>
<TD CLASS='base'><FONT COLOR='$colourred'>$tr{'destination portc'}</FONT></TD>
<TD><INPUT TYPE='TEXT' NAME='DEST_PORT' VALUE='$cgiparams{'DEST_PORT'}' SIZE='5' id='dest_port' @{[jsvalidport('dest_port')]}></TD>
<td CLASS='base'>$tr{'commentc'}</td>
<td><input type='text' name='COMMENT' value='$cgiparams{'COMMENT'}' size='32' id='comment' @{[jsvalidcomment('comment')]}  ></td>
</tr>
</TABLE>
<TABLE WIDTH='100%'>
<TR>
<TD CLASS='base' WIDTH='50%' ALIGN='CENTER'>$tr{'enabled'}<INPUT TYPE='CHECKBOX' NAME='ENABLED' $checked{'ENABLED'}{'on'}></TD>
<TD WIDTH='50%' ALIGN='CENTER'><INPUT TYPE='SUBMIT' NAME='ACTION' VALUE='$tr{'add'}'></TD>
</TR>
</TABLE>
END
;
&closebox();

&openbox($tr{'current rules'});

my $ifcolorsmap = &ifcolormap();

my %render_settings =
(
	'url'     => "/cgi-bin/xtaccess.cgi?[%COL%],[%ORD%]",
	'columns' => 
	[
		{ 
			column => '1',
			title  => 'Red interface',
			size   => 15,
			sort   => 'cmp',
			tr	=> \%{$ifcolorsmap},
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
			tr	=> { '0' => 'N/A' },
		},
		{
			column => '5',
			title  => "$tr{'enabledtitle'}",
			size   => 10,
			tr     => 'onoff',
			align  => 'center',
		},
		{
			title  => "$tr{'mark'}", 
			size   => 10,
			mark   => ' ',
		},
		{
			title  => "$tr{'comment'}",
			break  => 'line',
			column => '6',
		},
	]
);

&dispaliastab(6, $filename, \%render_settings, $cgiparams{'ORDER'}, $cgiparams{'COLUMN'} );

print <<END
<table class='blank' style='margin:6pt 0'>
<tr>
<td style='text-align: center; width: 33%;'><input type='submit' name='ACTION' value='$tr{'remove'}'></td>
<TD WIDTH='34%' ALIGN='CENTER'><INPUT TYPE='SUBMIT' NAME='ACTION' VALUE='$tr{'ffc-enable rule'}'></TD>
<td style='text-align: center; width: 33%;'><input type='submit' name='ACTION' value='$tr{'edit'}'></td>
</tr>
</table>
END
;
&closebox();
#&openbox();
#print <<END
#<br>
#<table width='100%'>
#<tr>
	#<td width='70%' align='right'>For help please visit the <a href=$ModDetails{'MOD_FORUM'}>SmoothWall Community Forums</a></td>
	#<td width='30%' align='right'>Mod version: $ModDetails{'MOD_VERSION'}</td>
#</tr>
#</table>
#</FORM>
#
#END
#;
#
#&closebox();

&alertbox('add', 'add');

&closebigbox();

&closepage();

sub ifcolormap
{
	my %ifcolors;
	
	open(FILE, $aliasfile) or die 'Unable to open aliases file';
	while ( my $line = <FILE> ){
		chomp $line;
		next if ( $line eq "" );
	
		my ( $ifcolor, $ethdev ) = split /,/, $line;
		if ($ifcolor =~ /^RED/) {
			$ethcolor = "<font color=red>$ifcolor</font>";
		}
		$ifcolors{$ethdev} = "$ethcolor ($ethdev)";
	}
	close FILE;
	return \%ifcolors;
}

