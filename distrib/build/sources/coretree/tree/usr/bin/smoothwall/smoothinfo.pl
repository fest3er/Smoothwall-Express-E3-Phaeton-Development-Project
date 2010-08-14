#!/usr/bin/perl -w
# Perl script to gather the info requested  in the UI.
# smoothinfo.pl v. 2.2b (c) Pascal Touch (nanouk) on Smoothwall forums
# Improved mods sorting routine and the list is now numbered.
# New smoothd module to make SmoothInfo more "compliant".
# Packed using Steve McNeill's Mod Build System.
# Detects mods packed with the Mod Build System.
# Added detection of several additional "non-standard" mods.
# Added IRQ's and Conntracks sections.
# Corrected "double webproxy section" bug.
# Corrected "missing opening info tag square bracket" bug in the screenshot section.
# Added verbosity to the display of firewall rules.

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use File::Find;
use File::Basename;

$SIdir="${swroot}/smoothinfo";
require "${SIdir}/about.ph";

my (%productdata, %pppsettings, %modemsettings, %netsettings, %smoothinfosettings, %defseclevelsettings, %green_dhcpsettings, %purple_dhcpsettings, %imsettings, %p3scansettings, %sipproxysettings, %proxysettings, %advproxysettings, %filteringsettings, %SSHsettings);

&readhash("${swroot}/main/productdata", \%productdata);
&readhash("${swroot}/ppp/settings", \%pppsettings);
&readhash("${swroot}/modem/settings", \%modemsettings);
&readhash("${swroot}/ethernet/settings", \%netsettings);
&readhash("${SIdir}/etc/settings", \%smoothinfosettings);
&readhash("${swroot}/main/settings", \%defseclevelsettings);
# stock services
&readhash("${swroot}/im/settings", \%imsettings);
&readhash("${swroot}/p3scan/settings", \%p3scansettings);
&readhash("${swroot}/sipproxy/settings", \%sipproxysettings);
&readhash("${swroot}/proxy/settings", \%proxysettings);
&readhash("${swroot}/snort/settings", \%snortsettings);
&readhash("${swroot}/remote/settings", \%SSHsettings);
# mod services
if (-e "${swroot}/proxy/advanced/settings") {
  &readhash("${swroot}/proxy/advanced/settings", \%advproxysettings);
}
if (-e "${swroot}/filtering/settings") {
  &readhash("${swroot}/filtering/settings", \%filteringsettings);
}

unless (-z "${swroot}/dhcp/settings-green") {
  &readhash("${swroot}/dhcp/settings-green", \%green_dhcpsettings); }
unless (-z "${swroot}/dhcp/settings-purple") {
  &readhash("${swroot}/dhcp/settings-purple", \%purple_dhcpsettings);
}

my $modversion = $modinfo{'MOD_LONG_NAME'} . " v. " . $modinfo{'MOD_VERSION'};
my $outputfile = "${SIdir}/etc/report.txt";

# checking for installed updates
#if (! -z "${swroot}/patches/installed") {
#open (INSTALLED,"<${swroot}/patches/installed") || die "Unable to open $!";
#my @installed = (<INSTALLED>);
#my $patch = pop (@installed);
#my @update = split (/\|/, $patch);
#my $updatenumber = $update[1];
#$updatenumber =~ s/-i386//;
#$swe_version = "$productdata{'PRODUCT'} $productdata{'VERSION'}-$productdata{'REVISION'}-$productdata{'ARCH'}-$updatenumber";
#} else {
#$swe_version = "$productdata{'PRODUCT'} $productdata{'VERSION'}-$productdata{'REVISION'}-$productdata{'ARCH'}";
#}

# MEMORY
my $memory = &pipeopen( '/usr/bin/free', '-ot' );
chomp ($memory);

# CPU
open (CPU,"</proc/cpuinfo") || die "Unable to open $!";
my $cpu = (grep /model\sname/, <CPU>)[0];
$cpu =~ s/model name(\t+|s+): //;
chomp ($cpu);
open (CPU,"</proc/cpuinfo") || die "Unable to open $!";
my $frequency = (grep /cpu\sMHz/, <CPU>)[0];
$frequency =~ s/cpu MHz(\t+|s+): //;
chomp ($frequency);
open (CPU,"</proc/cpuinfo") || die "Unable to open $!";
my $cache = (grep /cache\ssize/, <CPU>)[0];
$cache =~ s/cache size(\t+|s+): //;
chomp ($cache);

#IRQ's
opendir (DIR, "/proc/irq");
my @IRQs = '';
my $warning = '';
my @files = sort { lc($a) > lc($b) } (grep { /^\d+$/ } readdir DIR);
foreach (@files) {
chdir ("/proc/irq/$_");
$device = `ls -m`;
chomp $device;
if ($device) { if ($device =~ /,/) { push (@IRQs, "IRQ $_ used by $device\t<==\n"); $warning = "There seems to be at least one shared IRQ in your system!\n"; } else { push (@IRQs, "IRQ $_ used by $device\n"); } }
}

#CONNTRACKS
my $conntracks = `cat /proc/net/ip_conntrack|wc -l`;
chomp ($conntracks);

# DISKSPACE
my $diskspace = &pipeopen( '/bin/df', '-h' );
chomp $diskspace;

# ETHERNET ADAPTERS
open (LSPCI, "-|") or exec ("/usr/sbin/lspci");
my @ethernet_adapters = '';
foreach (<LSPCI>){
  if (/Ethernet/){
    $_ =~ s/^(.*)Ethernet controller: //;
    push (@ethernet_adapters, $_);
  }
}
close (LSPCI);

# Get the 'real' red iface when connected
if (-e "${swroot}/red/active") {
  open (IFACE, "/var/smoothwall/red/iface")
    or warn "Could not open /var/smoothwall/red/iface: $!";
  chomp ($RED = <IFACE>);
  $netsettings{'RED_DEV'} = $RED;
  close (IFACE);
}

# IFCONFIG
my @netconf;
my @netconf_red = &pipeopen( "/sbin/ifconfig", "$netsettings{'RED_DEV'}" )
  if ($netsettings{'RED_DEV'});
foreach (@netconf_red) {
  $_ =~ s/(\d+)\.(\d+)\.(\d+)\.(\d+)/\[\/b]\[\/color][color=#000000][b]$1\.$2\.$3\.$4\[\/b][\/color\][color=#FF0000\][b\]/g;
}
@netconf_red = ("[b][color=#FF0000]", @netconf_red, "[/color][/b]");
my @netconf_green = &pipeopen( "/sbin/ifconfig", "$netsettings{'GREEN_DEV'}" ) if ($netsettings{'GREEN_DEV'});
foreach (@netconf_green) {
  $_ =~ s/(\d+)\.(\d+)\.(\d+)\.(\d+)/\[\/b]\[\/color][color=#000000][b]$1\.$2\.$3\.$4\[\/b][\/color\][color=#00BF00\][b\]/g;
}
@netconf_green = ("[b][color=#00BF00]", @netconf_green, "[/color][/b]");
my @netconf_purple = &pipeopen( "/sbin/ifconfig", "$netsettings{'PURPLE_DEV'}" ) if ($netsettings{'PURPLE_DEV'});
foreach (@netconf_purple) {
  $_ =~ s/(\d+)\.(\d+)\.(\d+)\.(\d+)/\[\/b]\[\/color][color=#000000][b]$1\.$2\.$3\.$4\[\/b][\/color\][color=#BF00FF\][b\]/g;
}
@netconf_purple = ("[b][color=#BF00FF]", @netconf_purple, "[/color][/b]");
my @netconf_orange = &pipeopen( "/sbin/ifconfig", "$netsettings{'ORANGE_DEV'}" ) if ($netsettings{'ORANGE_DEV'});
foreach (@netconf_orange) {
  $_ =~ s/(\d+)\.(\d+)\.(\d+)\.(\d+)/\[\/b]\[\/color][color=#000000][b]$1\.$2\.$3\.$4\[\/b][\/color\][color=#FFBF00\][b\]/g;
}
@netconf_orange = ("[b][color=#FFBF00]", @netconf_orange, "[/color][/b]");

# Get the active ipsec connections
open (DEV, "/proc/net/dev");
@dev = <DEV>;
chomp @dev;
shift @dev;
shift @dev;
foreach (@dev) {
  next if /eth/;
  next if /lo/;
  if (/ipsec/) {
    ($if,$traffic) = split /:/;
    ($rx,$tx) = (split /\s+/, $traffic)[1,9];
    if ($rx > 0 || $tx > 0) { push (@other_interfaces, $if);}
  }
}
my @netconf_lo = &pipeopen( "/sbin/ifconfig", "lo" );
foreach (@netconf_lo) { $_ =~ s/(\d+)\.(\d+)\.(\d+)\.(\d+)/[b][color=#000000]$1\.$2\.$3\.$4\[\/color\][\/b]/g; }
@netconf = (@netconf, @netconf_lo);
if (@other_interfaces){
  foreach (@other_interfaces) {
  @ifconfig = &pipeopen( "/sbin/ifconfig", "$_" );
  foreach (@ifconfig) { $_ =~ s/(\d+)\.(\d+)\.(\d+)\.(\d+)/[b][color=#000000]$1\.$2\.$3\.$4\[\/color\][\/b]/g; }
  push (@netconf, @ifconfig);
  }
}

# 'LIVE' NET SETTINGS
# Define the tag that shows in the 'Network settings 2' report when there is a discrepancy
# between the settings and the actual values on a live connection
my $string = " [/b][/size][size=85][i]<not applicable>[/i][/color][color=#FF0000][b]";
# Define the actual values on a pppoX connected system
my ($dns1_tag, $dns2_tag, $redIP_tag, $remoteIP_tag, $bcast_tag, $netmask_tag);
if (($netsettings{'RED_TYPE'} eq 'DHCP' || 
     $netsettings{'RED_TYPE'} eq 'PPPOE') && 
    (-e "${swroot}/red/active")) {
  open (DNS1, "</var/smoothwall/red/dns1")
    or warn "Could not open /var/smoothwall/red/dns1: $!";
  chomp($redDNS1 = <DNS1>);
  if ($netsettings{'DNS1'} ne $redDNS1) { $dns1_tag = "$string"; }
  $netsettings{'DNS1'} = $redDNS1;
  open (DNS2, "</var/smoothwall/red/dns2")
    or warn "Could not open /var/smoothwall/red/dns2: $!";
  chomp($redDNS2 = <DNS2>);
  if ($netsettings{'DNS2'} ne $redDNS2) { $dns2_tag = "$string"; }
  $netsettings{'DNS2'} = $redDNS2;
  close (DNS1); close (DNS2);
  open (LOCALIP, "/var/smoothwall/red/local-ipaddress")
    or warn "Could not open /var/smoothwall/red/local-ipaddress: $!";
  chomp($redIP = <LOCALIP>);
  close (LOCALIP);
  if ($netsettings{'RED_ADDRESS'} ne $redIP) { $redIP_tag = "$string"; }
  $netsettings{'RED_ADDRESS'} = $redIP;
  open (REMOTEIP, "/var/smoothwall/red/remote-ipaddress")
    or warn "Could not open /var/smoothwall/red/local-ipaddress: $!";
  chomp($remoteIP = <REMOTEIP>);
  close (REMOTEIP);
  if ($netsettings{'DEFAULT_GATEWAY'} ne $remoteIP) { $remoteIP_tag = "$string"; }
  $netsettings{'DEFAULT_GATEWAY'} = $remoteIP;

  # Let's get the broadcast and netmask of the red iface when up
  open (IFCONFIG_RED, "-|") or exec ("ifconfig $netsettings{'RED_DEV'}");
  @temp = <IFCONFIG_RED>;
  close (IFCONFIG_RED);
  shift (@temp);
  chomp ($line = shift (@temp));
  @newarray = split /\s+/, $line;
  (undef, $bcast) = split /:/, $newarray[3];
  (undef, $netmask) = split /:/, $newarray[4];
  if ($netsettings{'RED_BROADCAST'} ne $bcast) { $bcast_tag = "$string"; }
  $netsettings{'RED_BROADCAST'} = $bcast;
  if ($netsettings{'RED_NETMASK'} ne $netmask) { $netmask_tag = "$string"; }
  $netsettings{'RED_NETMASK'} = $netmask;
  &writehash("/tmp/livesettings", \%netsettings);
  open (LIVESETTINGS,"</tmp/livesettings");
  while (<LIVESETTINGS>) {
    if (/RED|DNS|GATEWAY/) { push (@livered, $_); }
  }
  @live_red = sort @livered;
  @live_red = ("[color=#FF0000][b]", @live_red, "[/b][/color]\n");
  close (LIVESETTINGS);
}

# Opening /var/smoothwall/ethernet/settings regardless of the connection state.
# This file is not updated when on pppoe and/or dhcp and when you are disconnected/reconnected,
# and possibly when you subsequently run setup.
open (NETSETTINGS,"<${swroot}/ethernet/settings") || die "Unable to open $!";
while (<NETSETTINGS>) {
  chomp;
  if (/DNS1/) { push (@red, "$_" . $dns1_tag . "\n"); }
  elsif (/DNS2/) { push (@red, "$_" . $dns2_tag . "\n"); }
  elsif (/GATEWAY/) { push (@red, "$_" . $remoteIP_tag . "\n"); }
  elsif (/RED_ADDRESS/) { push (@red, "$_" . $redIP_tag . "\n"); }
  elsif (/RED_BROADCAST/) { push (@red, "$_" . $bcast_tag . "\n"); }
  elsif (/RED_NETMASK/) { push (@red, "$_" . $netmask_tag . "\n"); }
  elsif (/RED_D.*/) { push (@red, "$_\n"); }
  elsif (/RED_N.*/) { push (@red, "$_\n"); }
  elsif (/RED_T.*/) { push (@red, "$_\n"); }
  elsif (/GREEN/) { push (@green, "$_\n"); }
  elsif (/PURPLE/) { push (@purple, "$_\n"); }
  elsif (/ORANGE/) { push (@orange, "$_\n"); }
  else { push (@other, "$_\n\n"); }
}
close (NETSETTINGS);
@green = sort @green;
@green = ("[color=#00BF00][b]", @green, "[/b][/color]\n");
@red = sort @red;
@red = ("[color=#FF0000][b]", @red, "[/b][/color]\n");
@purple = sort @purple;
@purple = ("[color=#BF00FF][b]", @purple, "[/b][/color]\n");
@orange = sort @orange;
@orange = ("[color=#FFBF00][b]", @orange, "[/b][/color]");
@other = sort @other;
my $note = '';
if ($netsettings{'RED_TYPE'} eq 'DHCP' || $netsettings{'RED_TYPE'} eq 'PPPOE') { $note = "$tr{'smoothinfo-note'}\n\n"; }
my @ethernet_settings = ("[size=90]", @other,@red,@green,@purple,@orange, "[/size]");
my @live_settings = ("[color=\#400000][i]$note\[/i]\[/color]", "[size=90]", @other,@live_red,@green,@purple,@orange, "[/size]");

# ROUTING
my $route = &pipeopen( '/sbin/route', '-n' );

# IPTABLES CHAINS
my @chains = split (/,/,$smoothinfosettings{'CHAINS'});

# MODS
my %modlist = ();
my $dir = "${swroot}";
find(\&list, $dir);
# Deal with some "non-standard" mods
open (BASE, "</usr/lib/smoothwall/langs/en.pl") || die "Couldn't open $base: $!";
my @base = <BASE>;
close (BASE);
open (CRONTAB, "</etc/crontab") || die "Couldn't open /etc/crontab: $!";
my @crontab = <CRONTAB>;
close (CRONTAB);
open (SYSINIT,"/etc/rc.d/rc.sysinit") || die "Couldn't open /etc/crontab: $!";
my @sysinit = <SYSINIT>;
close (SYSINIT);

# ACTIVE IP BLOCK
if (grep /\[3\.0\] Active IP Block V1.0/, @base) {
  chomp($aipversion = (grep /\[3\.0\] Active IP Block V1.0/, @base)[0]);
  $aipversion =~ s/# \[3\.0\]\s//;
  $modlist{'activeblock'} = $aipversion;
}
# ENHANCED FIREWALL LOGS
if (grep /Enhanced Firewall Logs MOD V1.3/, @base) {
  chomp($efwlversion = (grep /Enhanced Firewall Logs MOD V1.3/, @base)[0]);
  $efwlversion =~ s/#\s//;
  $modlist{'enhanced-fw-logs'} = $efwlversion;
}
# CONNVIEW
if (grep /connview/, @base) {
  chomp($connviewversion = (grep /connview/, @base)[0]);
  $connviewversion =~ s/#\s//;
  $connviewversion = ucfirst $connviewversion;
  $modlist{'connview'} = $connviewversion;
}
# NET SCANNERS
if (grep /Net Scanner/, @base) {
  chomp($netscanversion = (grep /Net Scanner/, @base)[0]);
  $netscanversion =~ s/#\s//;
  $modlist{'netscan'} = $netscanversion;
}
# FILE UPLOAD
if (grep /File Upload/, @base) {
  chomp($fileuploadversion = (grep /File Upload/, @base)[0]);
  $fileuploadversion =~ s/#\s//;
  $modlist{'fileupload'} = $fileuploadversion;
}
# ZERINA
if (-d "$modpath/zerina" && -f "/httpd/cgi-bin/ovpnmain.cgi") {
  &readhash("$dir/zerina/settings", \%zsettings);
  $zerinaversion = "ZERINA-$zsettings{'VERSION'} / OpenVPN v$zsettings{'OVPNVER'}";
  $modlist{'zerina'} = $zerinaversion;
}
# PRIVOXY
if (-e "$dir/privoxy/settings") {
  &readhash("$dir/privoxy/settings", \%privoxyinfo);
  $privoxyinfo{'VERSION'} =~ s/\"//g;
  $privoxyversion = ucfirst $privoxyinfo{'MODNAME'} . " v. " . $privoxyinfo{'VERSION'};
  $modlist{'privoxy'} = $privoxyversion;
}
# ANONIMA
if (-e "$modpath/anonima/settings") {
  &readhash("$modpath/anonima/settings",\%anonimainfo);
  $anonimainfo{'VERSION'} =~ s/\"//g;
  $anonimaversion = ucfirst $anonimainfo{'MODNAME'} . " v. " . $anonimainfo{'VERSION'};
  $modlist{'anonima'} = $anonimaversion;
}
# CUSTOM ISO 3
if (-e "/root/CustomIso3/package") {
  open (VER, "/root/CustomIso3/package");
  while (<VER>) {
  if ($_ =~ /^version/i) {
     @tmp = split /"/, $_; $customiso3version = $tmp[1];}
  }
  $modlist{'customiso3'} = $customiso3version;
}
# WHO DIALLED
if (grep /WhoDialled/, @base) {
  chomp($whodialledversion = (grep /WhoDialled/, @base)[0]);
  $whodialledversion =~ s/#\s//;
  $modlist{'whodialled'} = $whodialledversion;
}
# CLAMAV BLOCKLISTS
if (-e "$dir/filtering/blocklists/version") {
  open (VERSION,"<$modpath/filtering/blocklists/version");
  $blocklistsversion = <VERSION>;
  $modlist{'blocklists'} = "Clamav Blocklists v. $blocklistsversion";
}
# WIFIDOG
if (grep /wifidog v/, @base) {
  chomp($wifidogversion = (grep /wifidog v/, @base)[0]);
  $wifidogversion =~ s/#\s//;
  $wifidogversion = ucfirst $wifidogversion;
  $modlist{'wifidog'} = $wifidogversion;
}
# NMAP
if (-e "/usr/bin/nmap") {
  @nmapversion = &pipeopen( '/usr/bin/nmap -V' );
  chomp @nmapversion;
  $tmp = pop (@nmapversion);
  @tmparray = split /\s+/,$tmp;
  $nmapversion = $tmparray[3];
  chomp ($nmapversion);
  $modlist{'nmap'} = "Nmap v. $nmapversion";
}
# ADVANCED WEB PROXY
if (-e "${swroot}/proxy/advanced/version") {
  open (AWPVERSION, "${swroot}/proxy/advanced/version");
  my $awpversion = <AWPVERSION>;
  close (AWPVERSION);
  $modlist{'awp'} = "Advanced Web Proxy v. $awpversion";
}
# URL FILTER
if (-e "${swroot}/urlfilter/version") {
  open (URLFILTER, "${swroot}/urlfilter/version");
  my $urlfilterversion = <URLFILTER>;
  close (URLFILTER);
  $modlist{'urlfilter'} = "URL Filter v. $urlfilterversion";
}
# COMPACT FLASH MOD
if (grep /Compact Flash/,@sysinit) {
  $modlist{'compactflash'} = "Compact Flash based Smoothwall V3.0";
}
# BANVIEWX
if (-e "$dir/bandview/installedX") {
  open (BANDVIEWX, "$dir/bandview/installedX");
  my $bandviewxversion = <BANDVIEWX>;
  close (BANDVIEWX);
  $modlist{'bandview'} = "BandviewX v. $bandviewxversion";
}
# DGLOG
if (-e "/httpd/cgi-bin/logs.cgi/dglog.cgi") {
  $modlist{'dglog'} = "Dansguardian Log Parser";
}

# MODULES
my $modules = &pipeopen( '/bin/lsmod' );
open (TOP, "-|") or exec ("/usr/bin/top -b -n 1");
my @top = (<TOP>);
close (TOP);
pop (@top);

# CONFIG
my ($RED, $ORANGE, $PURPLE);
open (ETHERSETTINGS,"<${swroot}/ethernet/settings") || die "Unable to open $!";
my @ethersettings = <ETHERSETTINGS>;
close (ETHERSETTINGS);
my $reddev = (grep /RED_DEV=eth/, @ethersettings)[0];
my $orangedev = (grep /ORANGE_DEV=eth/, @ethersettings)[0];
my $purpledev = (grep /PURPLE_DEV=eth/, @ethersettings)[0];

# WRITING OUTPUT
open (FILE,">$outputfile") || die 'Unable to open file';
print FILE "[color=purple][u][b][i]$tr{'smoothinfo-generated'} $modversion:[/i][/b][/u][/color]\n\n";
print FILE "[info=\"$tr{'smoothinfo-smoothwall-version'}\"]\[code\]$swe_version\[/code\]\[/info\]";

if ($smoothinfosettings{'CONFIG'} eq 'on') {
  if ($reddev) {$RED = 'RED'} else { $RED = 'RED(modem)'}
  if ($orangedev) {$ORANGE = '-ORANGE'}
  if ($purpledev) {$PURPLE = '-PURPLE'}
  print FILE "[info=\"$tr{'smoothinfo-firewall-config-type'}\"]\[code\]$RED-GREEN$ORANGE$PURPLE\[/code\]\[/info\]";
}

# Generate the ASCII schematic (ugly but works)
my $purple;
if ($purpledev) {$purple = '(purple)';} else {$purple = '        ';}
if (-e "${SIdir}/etc/schematic") {
  print FILE "[info=\"$tr{'smoothinfo-ascii-schematic'}\"][code\]";
  print FILE "                                  Internet\n";
  print FILE "                                     |\n";
  if ($smoothinfosettings{'MODEM'} eq 'on') {
    print FILE "                                   Modem\n";
    print FILE "                                     |\n";
  }
  if ($smoothinfosettings{'ROUTER'} eq 'on') {
    print FILE "                                   Router\n";
    print FILE "                                     |\n";
  }
  print FILE "                                   (red)\n";
  if ($smoothinfosettings{'SWITCH2'} eq 'on') {
    print FILE "W/Lan <=== Switch <=== (orange)";
    print FILE "[SMOOTHWALL](green)";
  } elsif ($smoothinfosettings{'WAP2'} eq 'on') {
    print FILE "    WLan <=== WAP <=== (orange)";
    print FILE "[SMOOTHWALL](green)";
  } else {
    print FILE "                               [SMOOTHWALL](green)";}
    if ($smoothinfosettings{'SWITCH1'} eq 'on') {
      print FILE " ===> Switch ===> W/Lan";
    }
    elsif ($smoothinfosettings{'WAP1'} eq 'on') {
      print FILE " ===> WAP ===> WLan";
    }

  if ($smoothinfosettings{'WAP3'} eq 'on' && 
      $smoothinfosettings{'SWITCH2'} eq 'on' &&
      $smoothinfosettings{'WAP4'} ne 'on' &&
      $smoothinfosettings{'WAP5'} ne 'on' &&
      $smoothinfosettings{'WAP6'} ne 'on' &&
      $smoothinfosettings{'SWITCH3'} ne 'on') {
    print FILE "\n              |                  $purple\n";
    print FILE "             WAP\n";
    print FILE "              |\n";
    print FILE "            WLan";
  }

  if ($smoothinfosettings{'WAP3'} ne 'on' &&
      $smoothinfosettings{'SWITCH2'} ne 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on' &&
      $smoothinfosettings{'WAP5'} ne 'on' &&
      $smoothinfosettings{'SWITCH3'} ne 'on' &&
      $smoothinfosettings{'WAP6'} ne 'on') {
    print FILE "\n                                 $purple                 |\n";
    print FILE "                                                         WAP\n";
    print FILE "                                                          |\n";
    print FILE "                                                        WLan";
  }

  if ($smoothinfosettings{'WAP3'} ne 'on' &&
      $smoothinfosettings{'SWITCH2'} eq 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on' &&
      $smoothinfosettings{'WAP5'} ne 'on' &&
      $smoothinfosettings{'SWITCH3'} ne 'on' &&
      $smoothinfosettings{'WAP6'} ne 'on') {
    print FILE "\n                                 $purple                 |\n";
    print FILE "                                                         WAP\n";
    print FILE "                                                          |\n";
    print FILE "                                                        WLan";
  }


  if ($smoothinfosettings{'WAP3'} eq 'on' &&
      $smoothinfosettings{'SWITCH2'} eq 'on' &&
      $smoothinfosettings{'WAP4'} ne 'on' &&
      $smoothinfosettings{'WAP6'} eq 'on') {
    print FILE "\n              |                  (purple)\n";
    print FILE "             WAP                     |\n";
    print FILE "              |                     WAP\n";
    print FILE "             WLan                    |\n";
    print FILE "                                    WLan";
  }

  if ($smoothinfosettings{'WAP3'} ne 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on' &&
      $smoothinfosettings{'WAP6'} eq 'on') {
    print FILE "\n                                 (purple)                  |\n";
    print FILE "                                     |                    WAP\n";
    print FILE "                                    WAP                    |\n";
    print FILE "                                     |                    WLan\n";
    print FILE "                                    WLan";
  }

  if ($smoothinfosettings{'WAP3'} eq 'on' &&
      $smoothinfosettings{'SWITCH2'} eq 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on' &&
      $smoothinfosettings{'SWITCH3'} ne 'on' &&
      $smoothinfosettings{'WAP6'} ne 'on') {
    print FILE "\n              |                  $purple                 |\n";
    print FILE "             WAP                                         WAP\n";
    print FILE "              |                                           |\n";
    print FILE "             WLan                                        WLan";
    print FILE "";
  }

  if ($smoothinfosettings{'WAP6'} eq 'on' &&
      $smoothinfosettings{'WAP3'} ne 'on' &&
      $smoothinfosettings{'WAP4'} ne 'on') {
    print FILE "\n                                 (purple)\n";
    print FILE "                                     |\n";
    print FILE "                                    WAP\n";
    print FILE "                                     |\n";
    print FILE "                                    WLan";
  }

  if ($smoothinfosettings{'SWITCH3'} eq 'on' &&
      $smoothinfosettings{'WAP3'} ne 'on' &&
      $smoothinfosettings{'WAP4'} ne 'on' &&
      $smoothinfosettings{'WAP5'} ne 'on') {
    print FILE "\n                                 (purple)\n";
    print FILE "                                     |\n";
    print FILE "                                  Switch\n";
    print FILE "                                     |\n";
    print FILE "                                   W/Lan";
  }

  if ($smoothinfosettings{'WAP5'} ne 'on' &&
      $smoothinfosettings{'SWITCH3'} eq 'on' &&
      $smoothinfosettings{'WAP3'} eq 'on' &&
      $smoothinfosettings{'SWITCH2'} eq 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on') {
    print FILE "\n              |                  (purple)                |\n";
    print FILE "             WAP                     |                   WAP\n";
    print FILE "              |                   Switch                  |\n";
    print FILE "             WLan                    |                   WLan\n";
    print FILE "                                   W/Lan";
    print FILE "";
  }

  if ($smoothinfosettings{'WAP6'} eq 'on' &&
      $smoothinfosettings{'SWITCH3'} ne 'on' &&
      $smoothinfosettings{'WAP3'} eq 'on' &&
      $smoothinfosettings{'SWITCH2'} eq 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on') {
    print FILE "\n              |                  (purple)                 |\n";
    print FILE "             WAP                     |                   WAP\n";
    print FILE "              |                     WAP                   |\n";
    print FILE "             WLan                    |                   WLan\n";
    print FILE "                                   WLan";
    print FILE "";
  }

  if ($smoothinfosettings{'SWITCH3'} eq 'on' &&
      $smoothinfosettings{'WAP3'} ne 'on' &&
      $smoothinfosettings{'WAP4'} ne 'on' &&
      $smoothinfosettings{'WAP5'} eq 'on') {
    print FILE "\n                                 (purple)\n";
    print FILE "                                     |\n";
    print FILE "                          WAP <== Switch\n";
    print FILE "                           |         |\n";
    print FILE "                         WLan      W/Lan";
  }

  if ($smoothinfosettings{'WAP5'} eq 'on' &&
      $smoothinfosettings{'SWITCH3'} eq 'on' &&
      $smoothinfosettings{'WAP3'} eq 'on' &&
      $smoothinfosettings{'SWITCH2'} eq 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on') {
    print FILE "\n              |                  (purple)                 |\n";
    print FILE "             WAP                     |                   WAP\n";
    print FILE "              |           WAP <== Switch                  |\n";
    print FILE "             WLan          |         |                   WLan\n";
    print FILE "                         WLan      W/Lan";
    print FILE "";
  }

  if ($smoothinfosettings{'SWITCH2'} eq 'on' &&
      $smoothinfosettings{'WAP3'} eq 'on' &&
      $smoothinfosettings{'SWITCH3'} eq 'on' &&
      $smoothinfosettings{'WAP3'} eq 'on' &&
      $smoothinfosettings{'WAP4'} ne 'on' &&
      $smoothinfosettings{'WAP5'} eq 'on') {
    print FILE "\n              |                  (purple)\n";
    print FILE "             WAP                     |\n";
    print FILE "              |           WAP <== Switch\n";
    print FILE "             WLan          |         |\n";
    print FILE "                         WLan      W/Lan";
  }

  if ($smoothinfosettings{'SWITCH2'} ne 'on' &&
      $smoothinfosettings{'WAP3'} ne 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on' &&
      $smoothinfosettings{'SWITCH3'} eq 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'WAP5'} ne 'on') {
    print FILE "\n                                 (purple)                 |\n";
    print FILE "                                     |                   WAP\n";
    print FILE "                                  Switch                  |\n";
    print FILE "                                     |                   WLan\n";
    print FILE "                                   W/Lan";
  }

  if ($smoothinfosettings{'SWITCH2'} eq 'on' &&
      $smoothinfosettings{'WAP3'} ne 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on' &&
      $smoothinfosettings{'SWITCH3'} eq 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'WAP5'} ne 'on') {
    print FILE "\n                                 (purple)                 |\n";
    print FILE "                                     |                   WAP\n";
    print FILE "                                  Switch                  |\n";
    print FILE "                                     |                   WLan\n";
    print FILE "                                   W/Lan";
  }

  if ($smoothinfosettings{'SWITCH2'} ne 'on' &&
      $smoothinfosettings{'WAP3'} ne 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on' &&
      $smoothinfosettings{'SWITCH3'} eq 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'WAP5'} eq 'on') {
    print FILE "\n                                 (purple)                 |\n";
    print FILE "                                     |                   WAP\n";
    print FILE "                          WAP <== Switch                  |\n";
    print FILE "                           |         |                   WLan\n";
    print FILE "                         WLan      W/Lan";
  }

  if ($smoothinfosettings{'SWITCH2'} eq 'on' &&
      $smoothinfosettings{'WAP3'} ne 'on' &&
      $smoothinfosettings{'SWITCH1'} eq 'on' &&
      $smoothinfosettings{'SWITCH3'} eq 'on' &&
      $smoothinfosettings{'WAP4'} eq 'on' &&
      $smoothinfosettings{'WAP5'} eq 'on') {
    print FILE "\n                                 (purple)                 |\n";
    print FILE "                                     |                   WAP\n";
    print FILE "                          WAP <== Switch                  |\n";
    print FILE "                           |         |                   WLan\n";
    print FILE "                         WLan      W/Lan";
  }

  print FILE "\[/code\]\[/info\]";
}

if ($defseclevelsettings{'OPENNESS'} eq 'halfopen') {
  $smoothinfosettings{'SECPOLICY'} = 'Half-open';
}
if ($defseclevelsettings{'OPENNESS'} eq 'open') {
  $smoothinfosettings{'SECPOLICY'} = 'Open';
}
if ($defseclevelsettings{'OPENNESS'} eq 'closed') {
  $smoothinfosettings{'SECPOLICY'} = 'Closed'
 }
print FILE "[info=\"$tr{'smoothinfo-default-secpol'}\"]\[code\]$smoothinfosettings{'SECPOLICY'}\[/code\]\[/info\]";

open (OUTGOING, "<${swroot}/outgoing/settings") || die "Unable to open ${swroot}/outgoing/settings: $!";
print FILE "[info=\"$tr{'smoothinfo-outgoing'}\"]\[code\]";
foreach (<OUTGOING>) {
  if (grep /GREEN=REJECT/, $_) {
    $rule_green = "$tr{'smoothinfo-traffic-originating'} GREEN is: $tr{'smoothinfo-allowed'}"; print FILE "$rule_green\n"
  }
  if (grep /GREEN=ACCEPT/, $_) {
    $rule_green = "$tr{'smoothinfo-traffic-originating'} GREEN is: $tr{'smoothinfo-blocked'}"; print FILE "$rule_green\n"
  }
  if (grep /ORANGE=REJECT/, $_) {
    $rule_orange = "$tr{'smoothinfo-traffic-originating'} ORANGE is: $tr{'smoothinfo-allowed'}"; print FILE "$rule_orange\n"
  }
  if (grep /ORANGE=ACCEPT/, $_) {
    $rule_orange = "$tr{'smoothinfo-traffic-originating'} ORANGE is: $tr{'smoothinfo-blocked'}"; print FILE "$rule_orange\n"
  }
  if (grep /PURPLE=REJECT/, $_) {
    $rule_purple = "$tr{'smoothinfo-traffic-originating'} PURPLE is: $tr{'smoothinfo-allowed'}"; print FILE "$rule_purple\n"
  }
  if (grep /PURPLE=ACCEPT/, $_) {
    $rule_purple = "$tr{'smoothinfo-traffic-originating'} PURPLE is: $tr{'smoothinfo-blocked'}"; print FILE "$rule_purple\n"
  }
}
print FILE "\[/code\]\[/info\]";
unless (-z "${swroot}/outgoing/config") {
  open (CONFIG, "${swroot}/outgoing/config") || die "Unable to open config file $!";
  print FILE "[info=\"Current exceptions\"]\[code\]";
  foreach (<CONFIG>) {
    chomp;
    $_ =~ s/,/\t\t/g;
    $_ =~ s/on/Enabled/g;
    print FILE "$_\n";
  }
  print FILE "\[/code\]\[/info\]";
}

unless (-z "${swroot}/outgoing/machineconfig") {
  open (MACHINECONFIG, "<${swroot}/outgoing/machineconfig") || die "Unable to open machineconfig file $!";
  print FILE "[info=\"Current always allowed machines\"]\[code\]";
  foreach (<MACHINECONFIG>) {
    chomp;
    $_ =~ s/,/\t\t/g;
    $_ =~ s/on/Enabled/g;
    print FILE "$_\n";
  }
  print FILE "\[/code\]\[/info\]";
}

# Get the DNS info for Red, Green, Purple
print FILE "[info=\"$tr{'smoothinfo-dns'}\"]\[code\]";
open (DNS1, "</var/smoothwall/red/dns1");
chomp($redDNS1 = (<DNS1>)[0]);
open (DNS2, "</var/smoothwall/red/dns2");
chomp($redDNS2 = (<DNS2>)[0]);
print FILE "DNS servers for RED:\nDNS1: $redDNS1\nDNS2: $redDNS2\n";
close (DNS1);
close (DNS2);

unless (-z "${swroot}/dhcp/settings-green") {
  print FILE "DNS servers for GREEN:\nDNS1: $green_dhcpsettings{'DNS1'}\nDNS2: $green_dhcpsettings{'DNS2'}\n";
}
unless (-z "${swroot}/dhcp/settings-purple") {
  print FILE "DNS servers for PURPLE:\nDNS1: $purple_dhcpsettings{'DNS1'}\nDNS2: $purple_dhcpsettings{'DNS2'}\n";
}
print FILE "\[/code\]\[/info\]";

if ($smoothinfosettings{'CONNTYPE'} eq 'on') {
  my $conntype = '';
  if ($pppsettings{'COMPORT'} =~ /^tty/) {$conntype = 'Dial-Up';}
  elsif ($pppsettings{'COMPORT'} =~ /^isdn/) {$conntype = 'ISDN';}
  elsif ($pppsettings{'COMPORT'} eq 'pppoe') {$conntype = 'PPPOE';}
  elsif ($reddev) {$conntype = 'LAN';}
  else {$conntype = 'Cable or DSL';}
  print FILE "[info=\"$tr{'smoothinfo-connection'}\"]\[code\]$conntype\[/code\]\[/info\]";
}

if ($smoothinfosettings{'MEMORY'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-memory-specs'}\"]\[code\]$memory\[/code\]\[/info\]";
}

if ($smoothinfosettings{'CONNTRACKS'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-conntracks'}\"]\[code\]$conntracks\[/code\]\[/info\]";
}

if ($smoothinfosettings{'LOADEDMODULES'} eq 'on') {
  my $data = do {local $/; $modules};
  # Will wrap lines longer then n characters
  $data =~ s{(.{$smoothinfosettings{'WRAP'}})(?=.)}{$1\n}g;
  print FILE "[info=\"$tr{'smoothinfo-modules'}\"]\[code\]$data\[/code\]\[/info\]";
}

if ($smoothinfosettings{'TOP'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-top'}\"]\[code\]@top\[/code\]\[/info\]";
}

if ($smoothinfosettings{'CPU'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-cpu'}\"]\[code\]$cpu (Freq.: $frequency MHz - Cache: $cache)\[/code\]\[/info\]";
}

if ($smoothinfosettings{'IRQs'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-irq'}\"]\[code\]$warning@IRQs\[/code\]\[/info\]";
}

if ($smoothinfosettings{'DISKSPACE'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-diskspace'}\"]\[code\]$diskspace\[/code\]\[/info\]";
}

if ($smoothinfosettings{'ADAPTERS'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-ethernet-reported'}\"]\[code\]@ethernet_adapters\[/code\]\[/info\]";
}

if ($smoothinfosettings{'NETCONF1'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-netsettings1'}\"]\[quote\][size=90\]@netconf_red@netconf_green@netconf_purple@netconf_orange@netconf\[/size\]\[/quote\]\[/info\]";
}

if ( -e "$MODDIR/etc/clientip"){
  open (CLIENTIP,"<$MODDIR/etc/clientip") || die "Unable to open $!";
  my @clientIP = (<CLIENTIP>);
  close (CLIENTIP);
  print FILE "[info=\"$tr{'smoothinfo-client-IP'}\"]";
  print FILE "\[code\]@clientIP\[/code\]\[/info\]";
  unlink ("/tmp/clientip");
}

if ($smoothinfosettings{'NETCONF2'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-netsettings2'}\"]\[quote\]@ethernet_settings\[/quote\]\[/info\]";
  if (($netsettings{'RED_TYPE'} eq 'DHCP' || $netsettings{'RED_TYPE'} eq 'PPPOE') && 
      (-e "${swroot}/red/active")) {
       print FILE "[info=\"$tr{'smoothinfo-livesettings'}\"]\[quote\]@live_settings\[/quote\]\[/info\]";
  }
}

# Status of core services
my $process_status;
print FILE "[info=\"$tr{'smoothinfo-core-services'}\"]\[code\]";
my @coreservices = ('cron', 'dnsmasq', 'httpd', 'klogd', 'smoothd');
foreach my $service (@coreservices) {
  if (open(PID, "/var/run/$service.pid")) {
    $pid = <PID>; chomp $pid;
    close PID;
    if ($pid) {
      if (open(PID, "/proc/$pid/status")) {
         while (<PID>) {
           if (/^State:\W+(.*)/) {
             $status = $1;
           }
         }
         close PID;
         if ($status =~ /s|sleeping|r|running/i) {
	   $process_status = 'running';
         } else {
           $process_status = 'stopped';
         }
         if ($service =~ /cron/) { $name = 'CRON server'; }
         elsif ($service =~ /dnsmasq/) { $name = 'DNS proxy server'; }
         elsif ($service =~ /httpd/) { $name = 'Web server'; }
         elsif ($service =~ /klogd/) { $name = 'Logging server'; }
         elsif ($service =~ /smoothd/) { $name = 'SetUID Daemon'; }
         print FILE "$name ($service): $process_status\n";
      }
    }
  }
}
print FILE "\[/code\]\[/info\]";

if ($smoothinfosettings{'SERVICES'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-services-status'}\"]\[code\]";
  if ($SSHsettings{'ENABLE_SSH'}) {
    print FILE "Remote access (SSH server): $SSHsettings{'ENABLE_SSH'}\n";
  }  
  if ($green_dhcpsettings{'ENABLE'}) {
    print FILE "DHCP server on green: $green_dhcpsettings{'ENABLE'}\n";
  }
  if ($purple_dhcpsettings{'ENABLE'}) {
    print FILE "DHCP server on purple: $purple_dhcpsettings{'ENABLE'}\n";
  }
  if ($imsettings{'ENABLE'}) {
    print FILE "IM Proxy: $imsettings{'ENABLE'}\n";
  }
  if ($p3scansettings{'ENABLE'}) {
    print FILE "Pop3 Proxy: $p3scansettings{'ENABLE'}\n";
  }
  if ($sipproxysettings{'ENABLE'}) {
    print FILE "SIP Proxy: $sipproxysettings{'ENABLE'}\n";
  }
  unless (-e "${swroot}/proxy/advanced/version") {
    if ($proxysettings{'ENABLE'}) {
      print FILE "Web Proxy: $proxysettings{'ENABLE'}";
      if ($proxysettings{'TRANSPARENT'} eq 'on') {
        print FILE ", in transparent mode\n";
      } else {
        print FILE ", not in transparent mode\n";
      }
    }
  }
  if ($snortsettings{'ENABLE_SNORT'}) {
    print FILE "IDS (Snort): $snortsettings{'ENABLE_SNORT'}\n";
  }
  print FILE "\[/code\]\[/info\]";
}

if ($smoothinfosettings{'MODSERVICES'} eq 'on') {
  my @modservices;
  push (@modservices, "[info=\"$tr{'smoothinfo-mod-services-status'}\"]\[code\]");
  if ($advproxysettings{'ENABLE'}) {
    push (@modservices, "Advanced Web Proxy: $advproxysettings{'ENABLE'}");
    if ($advproxysettings{'TRANSPARENT'} eq 'on') {
      push (@modservices, ", in transparent mode\n");
    } else {
     push (@modservices, ", not in transparent mode\n"); 
    }
  }
  if ($filteringsettings{'DGAV'}) {
    push (@modservices, "Dansguardian Web Content Filter (DGAV): $filteringsettings{'DGAV'}\n");
  }
  if ($filteringsettings{'SEMF'}) {
    push (@modservices,  "Smoothwall Express Mail Filter (SEMF): $filteringsettings{'SEMF'}\n");
  }
  if ($snortsettings{'ENABLE_GUARD'}) {
    push (@modservices, "Guardian Active Response (GAR): $snortsettings{'ENABLE_GUARD'}\n");
  }
  push (@modservices, "\[/code\]\[/info\]");
  $test = @modservices;
  # If at least one mod is found create the section
  if ($test > 2)  { print FILE "@modservices"; }
}

if ($advproxysettings{'ENABLE'} eq 'on' || $advproxysettings{'ENABLE_PURPLE'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-proxy'}\"]\[code\]";
  print FILE "Advanced Web proxy\n===========================\n";
  print FILE "Enabled on green: $advproxysettings{'ENABLE'}\n";
  print FILE "Transparent on green: $advproxysettings{'TRANSPARENT'}\n";
  print FILE "Enabled on purple: $advproxysettings{'ENABLE_PURPLE'}\n";
  print FILE "Transparent on purple: $advproxysettings{'TRANSPARENT_PURPLE'}\n";
  print FILE "Cache size (MB): $advproxysettings{'CACHE_SIZE'}\n";
  print FILE "Remote proxy: $advproxysettings{'UPSTREAM_PROXY'}\n";
  print FILE "Max object size (KB): $advproxysettings{'MAX_SIZE'}\n";
  print FILE "Min object size (KB): $advproxysettings{'MIN_SIZE'}\n";
  print FILE "Max outgoing size (KB): $advproxysettings{'MAX_OUTGOING_SIZE'}\n";
  print FILE "Max incoming size (KB): $advproxysettings{'MAX_INCOMING_SIZE'}\n";
  print FILE "\[/code\]\[/info\]";
} elsif ($proxysettings{'ENABLE'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-proxy'}\"]\[code\]";
  print FILE "Standard Web proxy\n===========================\n";
  print FILE "Transparent: $proxysettings{'TRANSPARENT'}\n";  
  print FILE "Cache size (MB): $proxysettings{'CACHE_SIZE'}\n";
  print FILE "Remote proxy: $proxysettings{'UPSTREAM_PROXY'}\n";
  print FILE "Max object size (KB): $proxysettings{'MAX_SIZE'}\n";
  print FILE "Min object size (KB): $proxysettings{'MIN_SIZE'}\n";
  print FILE "Max outgoing size (KB): $proxysettings{'MAX_OUTGOING_SIZE'}\n";
  print FILE "Max incoming size (KB): $proxysettings{'MAX_INCOMING_SIZE'}\n";
  print FILE "\[/code\]\[/info\]";
}

if ($smoothinfosettings{'DHCPINFO'} eq 'on' && -e "${swroot}/dhcp/enable") {
  
  unless (-z "${swroot}/dhcp/green") {
    print FILE "[info=\"$tr{'smoothinfo-dhcpsettings'} green\"]\[code\]";
    print FILE "Range of addresses: $green_dhcpsettings{'START_ADDR'} - $green_dhcpsettings{'END_ADDR'}\n";
    print FILE "Default lease time (mins): $green_dhcpsettings{'DEFAULT_LEASE_TIME'}\n";
    print FILE "Max lease time (mins): $green_dhcpsettings{'MAX_LEASE_TIME'}\n";
    print FILE "Primary DNS: $green_dhcpsettings{'DNS1'}\n";
    print FILE "Secondary DNS: $green_dhcpsettings{'DNS2'}\n";
    print FILE "Primary NTP: $green_dhcpsettings{'NTP1'}\n";
    print FILE "Secondary NTP: $green_dhcpsettings{'NTP2'}\n";
    print FILE "Primary WINS: $green_dhcpsettings{'WINS1'}\n";
    print FILE "Secondary WINS: $green_dhcpsettings{'WINS2'}\n";
    print FILE "Domain name suffix: $green_dhcpsettings{'DOMAIN_NAME'}\n";
    print FILE "NIS domain: $green_dhcpsettings{'NIS_DOMAIN'}\n";
    print FILE "Primary NIS: $green_dhcpsettings{'NIS1'}\n";
    print FILE "Secondary NIS: $green_dhcpsettings{'NIS2'}\n";
    print FILE "\[/code\]";
    
    unless (-z "${swroot}/dhcp/staticconfig-green") {
      open (STATIC, "${swroot}/dhcp/staticconfig-green");
      @statics = <STATIC>;
      close (STATIC);
      print FILE "[b]$tr{'smoothinfo-statics'}\[/b]\[code\]";
      print FILE "@statics";
      print FILE "\[/code\]\[/info\]";
    } else {
      print FILE "\[/info\]";
    }
  }

  unless (-z "${swroot}/dhcp/purple") {
    print FILE "[info=\"$tr{'smoothinfo-dhcpsettings'} purple\"]\[code\]";
    print FILE "Range of addresses: $purple_dhcpsettings{'START_ADDR'} - $purple_dhcpsettings{'END_ADDR'}\n";
    print FILE "Default lease time (mins): $purple_dhcpsettings{'DEFAULT_LEASE_TIME'}\n";
    print FILE "Max lease time (mins): $purple_dhcpsettings{'MAX_LEASE_TIME'}\n";
    print FILE "Primary DNS: $purple_dhcpsettings{'DNS1'}\n";
    print FILE "Secondary DNS: $purple_dhcpsettings{'DNS2'}\n";
    print FILE "Primary NTP: $purple_dhcpsettings{'NTP1'}\n";
    print FILE "Secondary NTP: $purple_dhcpsettings{'NTP2'}\n";
    print FILE "Primary WINS: $purple_dhcpsettings{'WINS1'}\n";
    print FILE "Secondary WINS: $purple_dhcpsettings{'WINS2'}\n";
    print FILE "Domain name suffix: $purple_dhcpsettings{'DOMAIN_NAME'}\n";
    print FILE "NIS domain: $purple_dhcpsettings{'NIS_DOMAIN'}\n";
    print FILE "Primary NIS: $purple_dhcpsettings{'NIS1'}\n";
    print FILE "Secondary NIS: $purple_dhcpsettings{'NIS2'}\n";
    print FILE "\[/code\]";
    
    unless (-z "${swroot}/dhcp/staticconfig-purple") {
      open (STATIC, "${swroot}/dhcp/staticconfig-purple");
      @statics = <STATIC>;
      close (STATIC);
      print FILE "[b]$tr{'smoothinfo-statics'}\[/b]\[code\]";
      print FILE "@statics";
      print FILE "\[/code\]\[/info\]";
    } else {
      print FILE "\[/info\]";
    }
  }

  if (-e "/usr/etc/dhcpd.leases") {
    print FILE "[info=\"$tr{'smoothinfo-dhcpleases'}\"]\[code\]";
    # block of code borrowed from dhcp.cgi

    ### Simple DHCP Lease Viewer (2007-0905) put together by catastrophe
    # - Borrowed "dhcpLeaseData" subroutine from dhcplease.pl v0.2.5 (DHCP Pack v1.3) for SWE2.0
    # by Dane Robert Jones and Tiago Freitas Leal
    # - Borrowed parts of "displaytable" subroutine from smoothtype.pm
    # (SmoothWall Express "Types" Module) from SWE3.0 by the SmoothWall Team
    # - Josh DeLong - 09/15/07 - Added unique filter
    # - Josh DeLong - 09/16/07 - Fixed sort bug and added ability to sort columns
    # - Josh DeLong - 10/1/07 - Rewrote complete dhcp.cgi to use this code
    ###

    my $leaseCount = -1;
    my $dhcptmpfile = "${SIdir}/tempfile";

    # Location of DHCP Lease File
    my $datfile = "/usr/etc/dhcpd.leases";
    open (LEASES,"<$datfile") || die "Unable to open $!";
    @catleasesFILENAME = (<LEASES>);
    close (LEASES);
    chomp (@catleasesFILENAME);
    for ($i=1; $i <= $#catleasesFILENAME; $i++){
      $datLine = $catleasesFILENAME[$i];

      if ($datLine =~ /^#/) {
        # Ignores comments
      } else {
        for ($datLine) {
          # Filter out leading & training spaces, double quotes, and remove end ';'
          s/^\s+//;
          s/\s+$//;
          s/\;//;
          s/\"//g;
        }
        if ($datLine =~ /^lease/) {

          $leaseCount++;      # Found start of lease
          @lineSplit = split(/ /,$datLine);       # Extract IP Address
          $dhcplIPAddy[$leaseCount] = $lineSplit[1];

        } elsif ($datLine =~ /^starts/) {

          @lineSplit = split(/ /,$datLine);     # Extract Lease Start Date
          $dhcplStart[$leaseCount] = "$lineSplit[2] $lineSplit[3]";

        } elsif ($datLine =~ /^ends/) {

          @lineSplit = split(/ /,$datLine);     # Extract Lease End Date
          $dhcplEnd[$leaseCount] = "$lineSplit[2] $lineSplit[3]";

        } elsif ($datLine =~ /^hardware ethernet/) {

          @lineSplit = split(/ /,$datLine);     # Extract MAC Address
          $dhcplMACAddy[$leaseCount] = uc($lineSplit[2]); # Make MAC Address All Upper Case for page consistancy.

        } elsif ($datLine =~ /^client-hostname/ || $datLine =~ /^hostname/) {

          @lineSplit = split(/ /,$datLine);     # Extract Host Name
          $dhcplHostName[$leaseCount] = $lineSplit[1];
        }
      }
    }

    for ($i = $#dhcplIPAddy; $i >= 0; $i--) {
      $catLINEnumber = $i+1;
      $dhcpprintvar = "True";

      if ($i == $#dhcplIPAddy){
        push(@dhcptemparray, $dhcplIPAddy[$i]);
      } else {
        foreach $IP (@dhcptemparray) {
          if ($IP =~ $dhcplIPAddy[$i]) {
            $dhcpprintvar = "False";
          }
        }
      }

      if (index($dhcplIPAddy[$i], $dhcpstart) == -1 ) {
        $dhcpprintvar = "False"
      }

      # Printing values to temp file
      if ($dhcpprintvar =~ "True"){
        push(@dhcptemparray, $dhcplIPAddy[$i]);
        print FILE "IP: $dhcplIPAddy[$i] Lease started: $dhcplStart[$i] Ends: $dhcplEnd[$i] Mac: $dhcplMACAddy[$i] Host name: $dhcplHostName[$i]\n";
      }
    }
  
    if (!@dhcptemparray) {
      print FILE "No leases.";
    }
  
    print FILE "\[/code\]\[/info\]";
  }

}

if ($smoothinfosettings{'ROUTE'} eq 'on') {
  print FILE "[info=\"$tr{'smoothinfo-routes'}\"]\[code\]$route\[/code\]\[/info\]";
}

if (($smoothinfosettings{'PORTFW'} eq 'on') && (! -z "${swroot}/portfw/config")) {
  open (PORTFW, "<${swroot}/portfw/config") || die "Unable to open $!";
  while (<PORTFW>) {
    chomp;
    $_ =~ s/,/\t\t/g;
    $_ =~ s/\t0\t/\tN.A.\t/g;
    $_ =~ s/on/Enabled/g;
    $_ =~ s/off/Disabled/g;
    push (@portfw, "$_\n");
  }
  close (PORTFW);
  print FILE "[info=\"$tr{'smoothinfo-portfw'}\"]\[code\]@portfw\[/code\]\[/info\]";
}

if ($smoothinfosettings{'CHAINS'} ne '') {
  foreach (@chains) {
    if (/All chains/) {
      open (FIREWALL,"-|", '/usr/sbin/iptables', '-L', '-n', '-v');  last;
    } else {
      open (FIREWALL,"-|", '/usr/sbin/iptables', '-L', $_, '-n', '-v')
    }
    @firewall = <FIREWALL>;
    push (@filtering, "\n");
    @filtering = (@filtering,@firewall);
  }
  shift (@filtering);
  print FILE "[info=\"$tr{'smoothinfo-firewall'}\"]\[code\]@filtering\[/code\]\[/info\]";
}

if ($smoothinfosettings{'MODLIST'} eq 'on') {
  if (%modlist) { $number = scalar (keys(%modlist)); }
  if ($number eq 1) { $suffix = ''; } else { $suffix = 's'; }
  my $id = 0;
  if (%modlist) {
    print FILE "[info=\"$number $tr{'smoothinfo-mods2'}$suffix\"]\[code\]";
    my @sorted = sort { lc($modlist{$a}) cmp lc($modlist{$b}) } keys %modlist;
    foreach(@sorted) { $id++; chomp ($modlist{$_}); print FILE "$id - $modlist{$_}\n"; }
  } else {
    print FILE "[info=\"$tr{'smoothinfo-mods'}\"]\[code\]";
    print FILE "$tr{'smoothinfo-no-mods'}. $tr{'smoothinfo-mods-tip'}";
  }
  print FILE "\[/code\]\[/info\]";
}

if ($smoothinfosettings{'DMESG'} eq 'on') {
  my $file = "/var/log/dmesg";
  my $dmesg;
  if ($smoothinfosettings{'LINES'} ne '' && $smoothinfosettings{'STRING'} eq '') {
    if ($smoothinfosettings{'HEADORTAIL'} eq 'HEAD') {
      open (DMESG,"<$file") || die "Unable to open $!";
      my @dmesg = (<DMESG>);
      @tmp = splice (@dmesg,0,$smoothinfosettings{'LINES'});
      open (TMP,">",\$dmesg);
      foreach (@tmp) {chomp; print TMP "$_\n";}
    } elsif ($smoothinfosettings{'HEADORTAIL'} eq 'TAIL') {
      open (DMESG,"<$file") || die "Unable to open $!";
      my @dmesg = (<DMESG>);
      $end = @dmesg;
      $start = $end - $smoothinfosettings{'LINES'};
      open (TMP,">",\$dmesg);
      $count = 0;
      foreach (@dmesg) {
        $count++;
        chomp;
        if ($count > $start && $count <= $end) {print TMP "$_\n";}
      }
    }
  } elsif ($smoothinfosettings{'LINES'} eq '' && $smoothinfosettings{'STRING'} ne '') {
    if ($smoothinfosettings{'IGNORECASE'} eq 'on') {
      open (DMESG,"<$file") || die "Unable to open $!";
      my @dmesg = (grep /$smoothinfosettings{'STRING'}/i, <DMESG>);
      open (TMP,">",\$dmesg);
      foreach (@dmesg) {chomp; print TMP "$_\n";}
    } else {
      open (DMESG,"<$file") || die "Unable to open $!";
      my @dmesg = (grep /$smoothinfosettings{'STRING'}/, <DMESG>);
      open (TMP,">",\$dmesg);
      foreach (@dmesg) {chomp; print TMP "$_\n";}
    }
  } elsif ($smoothinfosettings{'LINES'} eq '' && $smoothinfosettings{'STRING'} eq '') {
    open (DMESG,"<$file") || die "Unable to open $!";
    open (TMP,">",\$dmesg);
    foreach (<DMESG>) {chomp; print TMP "$_\n";}
  }
  
  if (!$dmesg) {
    print FILE "[info=\"$tr{'smoothinfo-dmesg2'}\"]\[code\]No search results for string '$smoothinfosettings{'STRING'}'.\[/code\]\[/info\]";
  } else {
    my $data = do {local $/; $dmesg};
    # Will wrap lines longer then n characters
    $data =~ s{(.{$smoothinfosettings{'WRAP'}})(?=.)}{$1\n}g;
    print FILE "[info=\"$tr{'smoothinfo-dmesg2'}\"]\[code\]$data\[/code\]\[/info\]";
  }
}

if ($smoothinfosettings{'APACHE'} eq 'on') {
  my $file = "/var/log/httpd/error_log";
  my $apache_error_log;
  if ($smoothinfosettings{'LINES2'} ne '' && $smoothinfosettings{'STRING2'} eq '') {
    if ($smoothinfosettings{'HEADORTAIL2'} eq 'HEAD2') {
      open (ERRORLOG,"<$file") || die "Unable to open $!";
      my @errorlog = (<ERRORLOG>);
      @tmp = splice (@errorlog,0,$smoothinfosettings{'LINES2'});
      open (TMP,">",\$apache_error_log);
      foreach (@tmp) {chomp; print TMP "$_\n";}
    } elsif ($smoothinfosettings{'HEADORTAIL2'} eq 'TAIL2') {
      open (ERRORLOG,"<$file") || die "Unable to open $!";
      my @errorlog = (<ERRORLOG>);
      $end = @errorlog;
      $start = $end - $smoothinfosettings{'LINES2'};
      open (TMP,">",\$apache_error_log);
      $count = 0;
      foreach (@errorlog) {
        $count++;
        chomp;
        if ($count > $start && $count <= $end) {print TMP "$_\n";}
      }
    }
  } elsif ($smoothinfosettings{'LINES2'} eq '' && $smoothinfosettings{'STRING2'} ne '') {
    if ($smoothinfosettings{'IGNORECASE2'} eq 'on') {
      open (ERRORLOG,"<$file") || die "Unable to open $!";
      my @errorlog = (grep /$smoothinfosettings{'STRING2'}/i, <ERRORLOG>);
      open (TMP,">",\$apache_error_log);
      foreach (@errorlog) {chomp; print TMP "$_\n";}
    } else {
      open (ERRORLOG,"<$file") || die "Unable to open $!";
      my @errorlog = (grep /$smoothinfosettings{'STRING2'}/, <ERRORLOG>);
      open (TMP,">",\$apache_error_log);
      foreach (@errorlog) {chomp; print TMP "$_\n";}
    }
    close (ERRORLOG);
  } elsif ($smoothinfosettings{'LINES2'} ne '' && $smoothinfosettings{'STRING2'} ne '') {
    if ($smoothinfosettings{'IGNORECASE2'} eq 'on') {
      open (ERRORLOG,"<$file") || die "Unable to open /var/log/httpd/error_log";
      my @errorlog = (grep /$smoothinfosettings{'STRING2'}/i, <ERRORLOG>);
      open (TMP,">",\$temporary);
      foreach (@errorlog) {chomp; print TMP "$_\n";}
      close (TMP);
      if ($smoothinfosettings{'HEADORTAIL2'} eq 'HEAD2') {
        open (HEAD,"<",\$temporary) || die "Unable to open $temporary";
        my @head = <HEAD>;
        @tmp = splice (@head,0,$smoothinfosettings{'LINES2'});
        open (TMP,">",\$apache_error_log);
        foreach (@tmp) {chomp; print TMP "$_\n";}
        close (TMP);
      } elsif ($smoothinfosettings{'HEADORTAIL2'} eq 'TAIL2') {
        open (TAIL,"<",\$temporary) || die "Unable to open $var";
        my @tail = <TAIL>;
        $end = @tail;
        $start = $end - $smoothinfosettings{'LINES2'};
        open (TMP,">",\$apache_error_log);
        $count = 0;
        foreach (@tail) {
          $count++;
          chomp;
          if ($count > $start && $count <= $end) {print TMP "$_\n";}
        }
      }
    } else {
      open (ERRORLOG,"<$file") || die "Unable to open $file: $!";
      my @errorlog = (grep /$smoothinfosettings{'STRING2'}/, <ERRORLOG>);
      open (TMP,">",\$temporary);
      foreach (@errorlog) {chomp; print TMP "$_\n";}
      close (TMP);
      if ($smoothinfosettings{'HEADORTAIL2'} eq 'HEAD2') {
        open (HEAD,"<",\$temporary) || die "Unable to open $temporary";
        my @head = <HEAD>;
        @tmp = splice (@head,0,$smoothinfosettings{'LINES2'});
        open (TMP,">",\$apache_error_log);
        foreach (@tmp) {chomp; print TMP "$_\n";}
        close (TMP);
      } elsif ($smoothinfosettings{'HEADORTAIL2'} eq 'TAIL2') {
        open (TAIL,"<",\$temporary) || die "Unable to open $temporary";
        my @tail = <TAIL>;
        $end = @tail;
        $start = $end - $smoothinfosettings{'LINES2'};
        open (TMP,">",\$apache_error_log);
        $count = 0;
        foreach (@tail) {
          $count++;
          chomp;
          if ($count > $start && $count <= $end) {print TMP "$_\n";}
        }
      }
    }
  }

  if (!$apache_error_log) {
    print FILE "[info=\"$tr{'smoothinfo-apache-error2'}\"]\[code\]No search results for string '$smoothinfosettings{'STRING2'}'.\[/code\]\[/info\]";
  } else {
    my $data = do {local $/; $apache_error_log};
    # Will wrap lines longer then n characters
    $data =~ s{(.{$smoothinfosettings{'WRAP'}})(?=.)}{$1\n}g;
    print FILE "[info=\"$tr{'smoothinfo-apache-error2'}\"]\[code\]$data\[/code\]\[/info\]";
  }
}

if ($smoothinfosettings{'MESSAGES'} eq 'on') {
  my $file = "/var/log/messages";
  my $messages_log;
  if ($smoothinfosettings{'LINES3'} ne '' && $smoothinfosettings{'STRING3'} eq '') {
    if ($smoothinfosettings{'HEADORTAIL3'} eq 'HEAD3') {
      open (MESSAGES,"<$file") || die "Unable to open $!";
      my @messages = (<MESSAGES>);
      @tmp = splice (@messages,0,$smoothinfosettings{'LINES3'});
      open (TMP,">",\$messages_log);
      foreach (@tmp) {chomp; print TMP "$_\n";}
    } elsif ($smoothinfosettings{'HEADORTAIL3'} eq 'TAIL3') {
      open (MESSAGES,"<$file") || die "Unable to open $!";
      my @messages = (<MESSAGES>);
      $end = @messages;
      $start = $end - $smoothinfosettings{'LINES3'};
      open (TMP,">",\$messages_log);
      $count = 0;
      foreach (@messages) {
        $count++;
        chomp;
        if ($count > $start && $count <= $end) {print TMP "$_\n";}
      }
    }
  } elsif ($smoothinfosettings{'LINES3'} eq '' && $smoothinfosettings{'STRING3'} ne '') {
    if ($smoothinfosettings{'IGNORECASE3'} eq 'on') {
      open (MESSAGES,"<$file") || die "Unable to open $!";
      my @messages = (grep /$smoothinfosettings{'STRING3'}/i, <MESSAGES>);
      open (TMP,">",\$messages_log);
      foreach (@messages) {chomp; print TMP "$_\n";}
    } else {
      open (MESSAGES,"<$file") || die "Unable to open $!";
      my @messages = (grep /$smoothinfosettings{'STRING3'}/, <MESSAGES>);
      open (TMP,">",\$messages_log);
      foreach (@messages) {chomp; print TMP "$_\n";}
    }
    close (MESSAGES);
  } elsif ($smoothinfosettings{'LINES3'} ne '' && $smoothinfosettings{'STRING3'} ne '') {
    if ($smoothinfosettings{'IGNORECASE3'} eq 'on') {
      open (MESSAGES,"<$file") || die "Unable to open $file: $!";
      my @messages = (grep /$smoothinfosettings{'STRING3'}/i, <MESSAGES>);
      open (TMP,">",\$temporary2);
      foreach (@messages) {chomp; print TMP "$_\n";}
      close (TMP);
      if ($smoothinfosettings{'HEADORTAIL3'} eq 'HEAD3') {
        open (HEAD,"<",\$temporary2) || die "Unable to open $temporary2";
        my @head = <HEAD>;
        @tmp = splice (@head,0,$smoothinfosettings{'LINES3'});
        open (TMP,">",\$messages_log);
        foreach (@tmp) {chomp; print TMP "$_\n";}
        close (TMP);
      } elsif ($smoothinfosettings{'HEADORTAIL3'} eq 'TAIL3') {
        open (TAIL,"<",\$temporary2) || die "Unable to open $temporary2: $!";
        my @tail = <TAIL>;
        $end = @tail;
        $start = $end - $smoothinfosettings{'LINES3'};
        open (TMP,">",\$messages_log);
        $count = 0;
        foreach (@tail) {
          $count++;
          chomp;
          if ($count > $start && $count <= $end) {print TMP "$_\n";}
        }
      }
    } else {
      open (MESSAGES,"<$file") || die "Unable to open $file: $!";
      my @messages = (grep /$smoothinfosettings{'STRING3'}/, <MESSAGES>);
      open (TMP,">",\$temporary2);
      foreach (@messages) {chomp; print TMP "$_\n";}
      close (TMP);
      if ($smoothinfosettings{'HEADORTAIL3'} eq 'HEAD3') {
        open (HEAD,"<",\$temporary2) || die "Unable to open $temporary2";
        my @head = <HEAD>;
        @tmp = splice (@head,0,$smoothinfosettings{'LINES3'});
        open (TMP,">",\$messages_log);
        foreach (@tmp) {chomp; print TMP "$_\n";}
        close (TMP);
      } elsif ($smoothinfosettings{'HEADORTAIL3'} eq 'TAIL3') {
        open (TAIL,"<",\$temporary2) || die "Unable to open $var";
        my @tail = <TAIL>;
        $end = @tail;
        $start = $end - $smoothinfosettings{'LINES3'};
        open (TMP,">",\$messages_log);
        $count = 0;
        foreach (@tail) {
          $count++;
          chomp;
          if ($count > $start && $count <= $end) {print TMP "$_\n";}
        }
      }
    }
  }


  if (!$messages_log) {
    print FILE "[info=\"$tr{'smoothinfo-system2'}\"]\[code\]No search results for string '$smoothinfosettings{'STRING3'}'.\[/code\]\[/info\]";
  } else {
    my $data = do {local $/; $messages_log};
    # Will wrap lines longer then n characters
    $data =~ s{(.{$smoothinfosettings{'WRAP'}})(?=.)}{$1\n}g;
    print FILE "[info=\"$tr{'smoothinfo-system2'}\"]\[code\]$data\[/code\]\[/info\]";
  }
}

if ($smoothinfosettings{'SCREENSHOTS'} ne '') {
  if ($smoothinfosettings{'SCREENSHOTS'} =~ /^(http|ftp)/) {
    $smoothinfosettings{'SCREENSHOTS'} = "\[img\]$smoothinfosettings{'SCREENSHOTS'}\[/img\]";
  }
  print FILE "[info=\"$tr{'smoothinfo-screenshots'}\"]$smoothinfosettings{'SCREENSHOTS'}\[/info\]";
}

if ( -e "$MODDIR/etc/otherinfo"){
  open (EXTRA,"<$MODDIR/etc/otherinfo") || die "Unable to open $!";
  my @extrainfo = (<EXTRA>);
  close (EXTRA);
  $section_title = shift @extrainfo;
  chomp $section_title;
  $section_title =~ s/([:;)(!'"]*)//g;
  $section_title = ucfirst $section_title;
  print FILE "[color=purple]====================================================[/color]\n";
  print FILE "[info=\"$section_title\"]";
  print FILE "\[code\]@extrainfo\[/code\]\[/info\]";
}

unlink ("/tmp/livesettings");

close (FILE);

sub list {
  my $id = 0;
  next unless -d;
  next if /modfiles/;
  next if /updates/;
  next if /smoothinfo/;
  if (-s "$File::Find::name/installed") {
    open (FILE, "$File::Find::name/installed");
    $ver = <FILE>;
    close (FILE);
    $ver =~ s/#//;
    $ver =~ s/^\s+//;
    $modlist{$_} = $ver;
  }
  if (-s "$File::Find::name/DETAILS") {
    &readhash("$File::Find::name/DETAILS", \%modinfo);
    if ($modinfo{'MOD_LONG_NAME'}){
      $modlist{basename($File::Find::name)} = ucfirst $modinfo{'MOD_LONG_NAME'} . " v. " . $modinfo{'MOD_VERSION'};
    } else {
      $modlist{basename($File::Find::name)} = ucfirst $modinfo{'MOD_NAME'} . " v. " . $modinfo{'MOD_VERSION'};
    }
  }
}
