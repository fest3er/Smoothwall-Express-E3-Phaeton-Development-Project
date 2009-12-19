#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );

$graphcriticalcolour = "#ff0000";
$graphwarningcolour  = "#ff5d00";
$graphnominalcolour  = "#ffa200";
$graphblankcolour    = "#ffffff";

$graphalertcritical = 90;
$graphalertwarning  = 70;

&showhttpheaders();

&openpage($tr{'advanced status information'}, 1, '', 'about your smoothie');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

&openbox($tr{'memory'});

@echo = `/usr/bin/free -ot`;
shift(@echo);

# these really should be tr strings

print qq|
<br/>
<table style='width: 90%; margin-left: auto; margin-right: auto; border-collapse: collapse; border: solid 1px #c0c0c0;'>
<tr>
	<th>&nbsp;</th>
	<th style='text-align: right; width: 50px'>$tr{ 'adv total' }</th>
	<th style='text-align: right; width: 50px'>$tr{ 'adv used' }</th>
	<th style='text-align: right; width: 50px'>$tr{ 'adv free' }</th>
	<th style='text-align: right;'>&nbsp;</th>
	<th style='text-align: center; width: 150px;'>$tr{ 'adv used%' }</th>
	<th style='text-align: right; width: 50px;' >$tr{ 'adv shared' }</th>
	<th style='text-align: right; width: 50px;' >$tr{ 'adv buffers' }</th>
	<th style='text-align: right; width: 50px;' >$tr{ 'adv cached' }</th>
</tr>
|;

foreach $mline (@echo) {
	chomp($mline);

	my ($mdev, $mtotal, $mused, $mfree, $mshared, $mbuffers, $mcached) = split(/\s+/, $mline);

	$mperc = int((($mused/$mtotal)*100));
	if ($mperc > $graphalertcritical) {
		$graphbgcolour = $graphcriticalcolour;
	} elsif ($mperc > $graphalertwarning) {
		$graphbgcolour = $graphwarningcolour;
	} elsif ($mperc > 0) {
		$graphbgcolour = $graphnominalcolour;
	} else {
		$graphbgcolour = $graphblankcolour;
	}
	if ( $mdev eq "Total:" ) {
		print qq|<tr><td colspan="9"><hr></td></tr>|;
	}
	if ($mdev eq 'Mem:') {
		$graphbgcolour = $graphnominalcolour;
	}
	print qq|
<tr>
<td style='text-align: right;'><tt>$mdev</tt></td>
<td style='text-align: right;'><tt>${mtotal}</tt></td>
<td style='text-align: right;'><tt>${mused}K</tt></td>
<td style='text-align: right;'><tt>${mfree}K</tt></td>
<td style='text-align: right;'><tt>&nbsp;</tt></td>
<td style='text-align: right;' width='160px;' nowrap>
	<table class='blank' style='width: 150px; border: 1px #505050 solid;'><tr>|;
	if ($mperc < 1) {
		print "<td style='background-color: $graphbgcolour; width: 1%; text-align: center;'><tt>$mperc%</tt></td>";}
	else {
		print "<td style='background-color: $graphbgcolour; width: $mperc%; text-align: center;'><tt>$mperc%</tt></td>";
	}
	print qq|
<td style='background-color: $graphblankcolour;'>&nbsp;</td></tr></table></td>
|;
	if ( $mshared ne "" ) {
		print qq|
<td style='text-align: right;'><tt>${mshared}K</tt></td>
<td style='text-align: right;'><tt>${mbuffers}K</tt></td>
<td style='text-align: right;'><tt>${mcached}K</tt></td>
|;
	}
	print qq|
</tr>
|;
}

print qq|
</table><br/>|;
&closebox();

&openbox($tr{'disk usage'});

@echo = `df -h`;
shift(@echo);

print qq|
<br/>
<table style='width: 90%; margin-left: auto; margin-right: auto; border-collapse: collapse; border: solid 1px #c0c0c0;'>
<tr>
<th style='width: 100px;'>$tr{ 'adv filesystem' }</th>
<th style='width: 75px;'>$tr{ 'adv mount point' }</th>
<th style='width: 40px; text-align: right;'>$tr{ 'adv size'}</th>
<th style='width: 40px; text-align: right;'>$tr{ 'adv used'}</th>
<th style='width: 65px; text-align: right;'>$tr{ 'adv available'}</th>
<th style='width: 5px;' >&nbsp;</th>
<th style='width: 150px; text-align: center;'>$tr{ 'adv used%' }</th>
</tr>
|;
foreach $mount (@echo) {
   chomp($mount);
   ($dev, $size, $size_used, $size_avail, $size_percentage, $mount_point) = split(/\s+/,$mount);
   if (int($size_percentage) > $graphalertcritical) {
      $graphbgcolour = $graphcriticalcolour;
   } elsif (int($size_percentage) > $graphalertwarning) {
      $graphbgcolour = $graphwarningcolour;
   } elsif (int($size_percentage) > 0) {
      $graphbgcolour = $graphnominalcolour;
   } else {
      $graphbgcolour = $graphblankcolour;
   }
   print qq|
<tr>
	<td><tt>$dev</tt></td>
	<td><tt>$mount_point</tt></td>
	<td style='text-align: right;'><tt>$size</tt></td>
	<td style='text-align: right;'><tt>$size_used</tt></td>
	<td style='text-align: right;'><tt>$size_avail</tt></td>
	<td><tt>&nbsp;</tt></td>
	<td><table class='blank' style='width: 150px; border: 1px #505050 solid;'>
<tr>
|;
   if (int($size_percentage) < 1) {
	print "<td style='background-color: $graphbgcolour; width: 1%; text-align: center;'><tt>$size_percentage</tt></td>";}
   else {
	print "<td style='background-color: $graphbgcolour; width: $size_percentage; text-align: center;'><tt>$size_percentage</tt></td>";
   }
   print qq|
<td style='background-color: $graphblankcolour;'>&nbsp;</td></tr></table></td>
</tr>
|;
}

print qq|
</table><br/>|;
&closebox();

&openbox($tr{'inode usage'});
@echo = `df -i`;
shift(@echo);
print qq|
<br/>
<table style='width: 90%; margin-left: auto; margin-right: auto; border-collapse: collapse; border: solid 1px #c0c0c0;'>
<tr>
<th style='width: 100px;'>$tr{ 'adv filesystem' }</th>
<th style='width: 75px;'>$tr{ 'adv mount point' }</th>
<th style='width: 40; text-align: right;'>$tr{ 'adv inodes' }</th>
<th style='width: 40; text-align: right;'>$tr{ 'adv used' }</th>
<th style='width: 65; text-align: right;'>$tr{ 'adv free' }</th>
<th style='width: 5px;'>&nbsp;</th>
<th style='width: 150px; text-align: center;'>$tr{ 'adv used%' }</th>
</tr>
|;
foreach $mount (@echo) {
   chomp($mount);
   ($dev, $size, $size_used, $size_avail, $size_percentage, $mount_point) = split(/\s+/,$mount);
   if (int($size_percentage) > $graphalertcritical) {
      $graphbgcolour = $graphcriticalcolour;
   } elsif (int($size_percentage) > $graphalertwarning) {
      $graphbgcolour = $graphwarningcolour;
   } elsif (int($size_percentage) > 0) {
      $graphbgcolour = $graphnominalcolour;
   } else {
      $graphbgcolour = $graphblankcolour;
   }
   print qq|
<tr>
	<td ><tt>$dev</tt></td>
	<td ><tt>$mount_point</tt></td>
	<td style='text-align: right;'><tt>$size</tt></td>
	<td style='text-align: right;'><tt>$size_used</tt></td>
	<td style='text-align: right;'><tt>$size_avail</tt></td>
	<td><tt>&nbsp;</tt></td>
	<td><table class='blank' style='width: 150px; border: 1px #505050 solid;'>
<tr>
|;
   if (int($size_percentage) < 1) {
	print "<td style='background-color: $graphbgcolour; width: 1%; text-align: center;'><tt>$size_percentage</tt></td>";}
   else {
	print "<td style='background-color: $graphbgcolour; width: $size_percentage; text-align: center;'><tt>$size_percentage</tt></td>";
   }
   print qq|
<td style='background-color: $graphblankcolour;'>&nbsp;</td></tr></table></td>
</tr>
|;
}
print qq|
</table><br/>|;
&closebox();

&openbox($tr{'uptime and users'});

my @who = split /\n/, &pipeopen( '/usr/bin/w' );

my ( $time, $up, $users, $load ) = ( $who[0] =~/\s+([^\s]*)\s+up\s+([^,]*),\s+([^,]*),\s+(.*)/ );

print "<div style='text-align: center;'>$time,  up $up,  $users,  $load</div>";

print qq{
	<br/>
	<table style='width: 90%; margin-left: auto; margin-right: auto; border-collapse: collapse; border: solid 1px #c0c0c0;'>
	<tr>
		<th>$tr{'adv user'}</th>
		<th>$tr{'adv tty'}</th>
		<th>$tr{'adv login'}</th>
		<th>$tr{'adv idle'}</th>
		<th>$tr{'adv jcpu'}</th>
		<th>$tr{'adv pcpu'}</th>
		<th>$tr{'adv what'}</th>
	</tr>
};

shift @who;  # remove the header information
shift @who;  # remove the header information

foreach my $whol (@who){
	my ( $user, $tty, $login, $idle, $jcpu, $pcpu, $what ) = split /\s+/, $whol;
	print qq{
		<tr>
			<td>$user</td>
			<td>$tty</td>
			<td>$login</td>
			<td>$idle</td>
			<td>$jcpu</td>
			<td>$pcpu</td>
			<td>$what</td>
		</tr>
	};
}
print "</table>\n";

&closebox();

my %ethersettings;
&readhash(  "/var/smoothwall/ethernet/settings", \%ethersettings );
my %devices;
$devices{$ethersettings{'GREEN_DEV'}} = $tr{'green'};
$devices{$ethersettings{'ORANGE_DEV'}} = $tr{'orange'};
$devices{$ethersettings{'RED_DEV'}} = $tr{'red'};
$devices{$ethersettings{'PURPLE_DEV'}} = $tr{'purple'};

&openbox($tr{'interfaces'});

my @interfaces = split /\n\n/, &pipeopen( '/sbin/ifconfig', '-a' );

foreach my $interface ( @interfaces ){
	my ( $devicename ) = ( $interface =~ /([^\s]+)/ );
        my ( $macaddress ) = ( $interface =~ /HWaddr\s+([a-fA-F\d]+:[a-fA-F\d]+:[a-fA-F\d]+:[a-fA-F\d]+:[a-fA-F\d]+:[a-fA-F\d]+)/ );
        my ( $ipaddress  ) = ( $interface =~ /inet addr:(\d+\.\d+\.\d+\.\d+)/ );
        my ( $mtu )        = ( $interface =~ /MTU:(\d+)/ );
        my ( $broadcast  ) = ( $interface =~ /Bcast:(\d+\.\d+\.\d+\.\d+)/ );
        my ( $netmask    ) = ( $interface =~ /Mask:(\d+\.\d+\.\d+\.\d+)/ );
        my ( $status     ) = ( $interface =~ /\s+(UP)\s+/ );
        my ( $rx, $rxk   ) = ( $interface =~ /RX bytes:(\d+) \((\d+\.\d+ [KMG]*b)\)/ );
        my ( $tx, $txk   ) = ( $interface =~ /TX bytes:(\d+) \((\d+\.\d+ [KMG]*b)\)/ );
        my ( $rxp, $rxe, $rxd, $rxo, $rxf) =
			     ( $interface =~ /RX packets:(\d+)\s+errors:(\d+)\s+dropped:(\d+)\s+overruns:(\d+)\s+frame:(\d+)/ );
        my ( $txp, $txe, $txd, $txo, $txc) =
			     ( $interface =~ /TX packets:(\d+)\s+errors:(\d+)\s+dropped:(\d+)\s+overruns:(\d+)\s+carrier:(\d+)/ );

	if ( defined $devices{$devicename} ){
		$devicename = "$devicename ($devices{$devicename})";
	}

	print qq{
		<br/>
		<table style='width: 90%; margin-left: auto; margin-right: auto; border-collapse: collapse; border: solid 1px #c0c0c0;'>
		<tr>
			<th colspan='5'>$devicename</th>
		</tr>
		<tr>
			<th style='width: 4%;' rowspan='8'>&nbsp;</th>
			<td style='width: 23%;'>IP Address:</td>
			<td style='width: 25%;'>$ipaddress</td>
			<td style='width: 23%;'>Broadcast</td>
			<td style='width: 25%;'>$broadcast</td>
		</tr>
		<tr>
			<td>Netmask</td>
			<td>$netmask</td>
			<td>MTU</td>
			<td>$mtu</td>
		</tr>
		<tr>
			<td>MAC Address</td>
			<td>$macaddress</td>
			<td>Status</td>
			<td>$status</td>
		</tr>
		<tr>
			<td>Sent packets</td>
			<td>$txp ($txk)</td>
			<td>Received packets</td>
			<td>$rxp ($rxk)</td>
		</tr>
		<tr>
			<td>Errors (sent)</td>
			<td>$txe</td>
			<td>Errors (received)</td>
			<td>$rxe</td>
		</tr>
		<tr>
			<td>Dropped (sent)</td>
			<td>$txd</td>
			<td>Dropped (received)</td>
			<td>$rxd</td>
		</tr>
		<tr>
			<td>Overruns (sent)</td>
			<td>$txo</td>
			<td>Overruns (received)</td>
			<td>$rxo</td>
		</tr>
		<tr>
			<td>Carrier (sent)</td>
			<td>$txc</td>
			<td>Frame (received)</td>
			<td>$rxf</td>
		</tr>
		</table>
	};
}

&closebox();

&openbox($tr{'routing'});

my @routes = split /\n/, &pipeopen( '/sbin/route', '-n' );

print qq{
	<br/>
	<table style='width: 90%; margin-left: auto; margin-right: auto; border-collapse: collapse; border: solid 1px #c0c0c0;'>
	<tr>
		<th>$tr{'adv destination'}</th>
		<th>$tr{'adv gateway'}</th>
		<th>$tr{'adv genmask'}</th>
		<th>$tr{'adv flags'}</th>
		<th>$tr{'adv metric'}</th>
		<th>$tr{'adv ref'}</th>
		<th>$tr{'adv use'}</th>
		<th>$tr{'adv iface'}</th>
	</tr>
};

shift @routes;  # remove the header information
shift @routes;  # remove the header information

foreach my $routel (@routes){
	my ( $destination, $gateway, $genmask, $flags, $metric, $ref, $use, $iface ) = split /\s+/, $routel;
	print qq{
		<tr>
			<td>$destination</td>
			<td>$gateway</td>
			<td>$genmask</td>
			<td>$flags</td>
			<td>$metric</td>
			<td>$ref</td>
			<td>$use</td>
			<td>$devices{$iface}</td>
		</tr>
	};
}
print "</table>\n";

&closebox();
&openbox($tr{'adv hardware'});

my @lspci = split /\n/, &pipeopen( '/usr/sbin/lspci' );

print qq{
	<br/>
	<table style='width: 90%; margin-left: auto; margin-right: auto; border-collapse: collapse; border: solid 1px #c0c0c0;'>
	<tr>
		<th>$tr{'adv address'}</th>
		<th>$tr{'adv type'}</th>
		<th>$tr{'adv device'}</th>
	</tr>
};

foreach my $devl (@lspci){
	my ( $address, $type, $device ) = ( $devl =~/([^\s]*)\s+([^:]*):\s+(.*)/ );
	print qq{
		<tr>
			<td>$address</td>
			<td>$type</td>
			<td>$device</td>
		</tr>
	};
}
print "</table>\n";

&closebox();

&openbox($tr{'loaded modules'});

my @lsmod = split /\n/, &pipeopen( '/bin/lsmod' );

print qq{
	<br/>
	<table style='width: 90%; margin-left: auto; margin-right: auto; border-collapse: collapse; border: solid 1px #c0c0c0;'>
	<tr>
		<th>$tr{'adv module'}</th>
		<th>$tr{'adv size'}</th>
		<th>$tr{'adv used by'}</th>
	</tr>
};

shift @lsmod;  # remove the header information

foreach my $modl (@lsmod){
	my ( $module, $size, $usedby ) = split /\s+/, $modl;
	print qq{
		<tr>
			<td>$module</td>
			<td>$size</td>
			<td>$usedby</td>
		</tr>
	};
}
print "</table>\n";

&closebox();

&openbox($tr{'kernel version'});
print "<PRE>";
system ('/bin/uname', '-a');
print "</PRE>\n";
&closebox();

&alertbox('add', 'add');

&closebigbox();

&closepage();

