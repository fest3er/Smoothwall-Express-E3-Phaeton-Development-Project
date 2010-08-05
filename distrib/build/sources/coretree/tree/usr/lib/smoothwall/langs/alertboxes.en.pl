# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

%baseabouttext = (

'index.cgi' => '
Welcome to <strong>SmoothWall Express</strong> ' . $version . '<br>
This is your gateway to configuring and administering your SmoothWall
firewall.  Further information on your SmoothWall Express is available from <a href=\'http://www.smoothwall.org/\'
 title=\'www.smoothwall.org - external link\'>our website</a>.
',

'credits.cgi' 		=> 'SmoothWall Express' . $tr{'version'} . ' ' . $version . '-' . $revision . ' ' . $webuirevision,
'status.cgi' 		=> 'Active service status of your Smoothie.',
'advstatus.cgi' 	=> 'Pertinent information about your Smoothie, current configuration and resource usage.',
'graphs.cgi' 		=> 'Statistical graphical and numeric data based upon traffic usage across your SmoothWall\'s network interfaces.',
'proxy.cgi' 		=> 'Configure and enable your SmoothWall\'s integrated caching web proxy service.',
'dhcp.cgi' 		=> 'Configure and enable your SmoothWall\'s DHCP service, to automatically allocate LAN IP addresses to your network clients.',
'ddns.cgi' 		=> 'Especially suited when your ISP assigs you a different IP address every time you connect, you can configure your SmoothWall to manage and update your dynamic DNS names from several popular services.',
'ids.cgi' 		=> 'Enable the Snort IDS service to detect potential security breach attempts from outside your network.  Note that Snort <strong>does not</strong> prevent these attempts &mdash; your port forwarding and access rules are used to allow and deny inbound access from the outside.<br>Snort is no longer shipped with any rules. In order to fetch rules you need to visit <a href="http://www.snort.org" target="_new">www.snort.org</a> and register for an Oink code',
'remote.cgi' 		=> 'Enable Secure Shell access to your SmoothWall, and restrict access based upon referral URL to ignore external links to your SmoothWall.',
'portfw.cgi' 		=> 'Forward ports from your external IP address to ports on machines inside your local networks.',
'xtaccess.cgi' 		=> 'Allow access to admin services running on the SmoothWall to external hosts.',
'dmzholes.cgi' 		=> 'Enable access from a host on your ORANGE or PURPLE networks to a port on a host on your GREEN network.',
'pppsetup.cgi' 		=> 'Configure username, password and other details for up to five PPP, PPPoA or PPPoE connections.',
'vpnmain.cgi' 		=> 'Control and manage your VPN connections.',
'vpnconfig.dat' 	=> 'Create connections to other SmoothWalls or IPSec-compliant hosts which have static IP addresses.',
'log.dat' 		=> 'Check activity logs for services operating on your SmoothWall, such as DHCP, IPSec, updates and core kernel activity',
'proxylog.dat' 		=> 'Check logs for the web proxy service.',
'firewalllog.dat'	=> 'Check logs for attempted access to your network from outside hosts.  Connections listed here <strong>have</strong> been blocked.',
'ids.dat' 		=> 'Check logs for potentially malicious attempted access to your network from outside hosts.  Connections listed here <strong>have not necessarily</strong> been blocked &mdash; use the Firewall Log Viewer to confirm blocked access.',
'ipinfo.cgi' 		=> 'Perform a \'whois\' lookup on an ip address or domain name.',
'iptools.cgi' 		=> 'Perform \'ping\' and \'traceroute\' network diagnostics.',
'shell.cgi' 		=> 'Connect to your SmoothWall using a Java SSH applet (requires SSH to be <a href="/cgi-bin/remote.cgi">enabled</a>).',
'updates.cgi' 		=> 'See the latest updates and fixes available for your SmoothWall, and an installation history of updates previously applied.',
'modem.cgi' 		=> 'Apply specific AT string settings for your PSTN modem or ISDN TA.',
'alcateladslfw.cgi' 	=> 'Upload firmware to enable use of an Alcatel/Thomson Speedtouch Home USB ADSL modem, nicknamed the \'frog\' or \'stingray\'.  <a href="http://smoothwall.org/get/" target="_breakFromUI">Download the \'Speedtouch USB Firmware\' tarball</a>, unpack it, and upload the mgmt.o file using this form.',
'changepw.cgi' 		=> 'Change passwords for the \'admin\' and \'dial\' management interface users.  This does not affect access by SSH.',
'shutdown.cgi' 		=> 'Shutdown or restart your SmoothWall &mdash; restarts are sometimes mandated by update installation.',
'time.cgi' 		=> 'Change timezone, manually set the time and date, configure time syncronisation and enable the time server.',
'advnet.cgi' 		=> 'Configure ICMP settings and other advanced features.',
'outbound.cgi'		=> 'Configure interfaces to have outbound traffic blocked, except on specific rules.',
'ipblock.cgi' 		=> 'Add blocking rules to prevent access from specified IP addresses or networks.',
'backup.img' 		=> 'Use this page to create a backup floppy disk or floppy disk image file.',
'im.cgi'		=> 'Configure the IM logging proxy.',
'outgoing.cgi' 		=> 'Add rules to control local machine\'s access to external services.',
'traffic.cgi'		=> 'Set the best speed which your network is capable of achieving. The headroom should be increased if traffic shaping is having no effect.',
'sipproxy.cgi'		=> 'Configure the SIP proxy service. It can be used to transparently or non-transparently proxy SIP calls to and from the GREEN network.',
'interfaces.cgi'	=> 'Configure the network interface IP addresses, as well as DNS and gateway settings.',
'hosts.cgi'		=> 'Add static DNS entries to SmoothWall\'s inbuilt DNS server.',
'timedaccess.cgi'	=> 'Configure timed access rules to prevent or allow internal machines network access at certain times of the day.',
'p3scan.cgi'		=> 'Configure the POP3 anti-virus proxy. This proxy will remove viruses from emails that are retrieved through using the POP3 protocol.',
'pop3log.dat'		=> 'Check log for the POP3 Anti-Virus service.',
'imviewer.cgi'		=> 'View logged IM conversations in realtime.',
'preferences.cgi'	=> 'Configure the SmoothWall Express User Interface.',
'bandwidthbars.cgi'	=> 'Shows realtime network bandwidth usage bars.',
'trafficmonitor.cgi'	=> 'Shows realtime network bandwidth usage graphs.',
'register.cgi'		=> 'Shows information about your SmoothWall Express system.',

);

