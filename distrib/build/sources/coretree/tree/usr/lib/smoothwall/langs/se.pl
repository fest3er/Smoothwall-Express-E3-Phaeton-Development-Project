# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# (c) Swedish Translation Team:
# Fredrik Johansson	<frejo@home.se>
# Francis j. Morris	<franman@visto.com>
# Thomas Persson	<thomas.persson@mbox2.swipnet.se> 
# Torulf Wiberg		<torulf@dcab.se> 
# Oden Eriksson		<oden.eriksson@kvikkjokk.net> 
# Hans Laakso		<Hans.Laakso@evoxrifa.com>

%basetr = (

%basetr,

# common
'invalid input' => 'Ogiltig inmatning.',
'save' => 'Spara', # button
'refresh' => 'Uppdatera', # button
'restore' => 'ATERSTALL', # button
'error messages' => 'Felmeddelanden:',
'back' => 'Bak�t',
'help' => 'Hj�lp',
'primary dns' => 'Prim�r DNS:',
'secondary dns' => 'Sekund�r DNS:',
'invalid primary dns' => 'Ogiltig prim�r DNS.',
'invalid secondary dns' => 'Ogiltig sekund�r DNS.',
'dhcp server' => 'DHCP-server',
'username' => 'Anv�ndarnamn:',
'password' => 'L�senord:',
'enabled' => 'Aktiverad:',
'this field may be blank' => 'Detta f�lt kan vara tomt.',
'these fields may be blank' => 'Dessa f�lt kan vara tomma.',

# header.pl
'sshome' => 'hem',
'ssstatus' => 'status',
'sstraffic graphs' => 'trafikgrafer',
'ssppp settings' => 'ppp inst�llningar',
'ssmodem' => 'modem',
'ssusb adsl firmware upload' => 'usb adsl firmware uppladdning',
'ssssh' => 'ssh',
'sspasswords' => 'l�senord',
'ssweb proxy' => 'webproxy',
'ssdhcp' => 'dhcp',
'ssport forwarding' => 'vidarebefodring av portar',
'ssexternal service access' => 'extern �tkomst till tj�nster',
'ssdmz pinholes' => 'dmz pinholes',
'ssdynamic dns' => 'dynamisk dns',
'ssids' => 'system f�r att uppt�cka intr�ng (IDS)',
'sscontrol' => 'kontrol',
'ssconnections' => 'anslutningar',
'ssother' => 'annat',
'ssfirewall' => 'brandv�gg',
'ssshutdown' => 'st�ng av',
'ssshell' => 'skal (shell)',
'ssupdates' => 'uppdateringar',
'sshelp' => 'hj�lp',
'sscredits' => 'tack till',
'ssip info' => 'IP-Information',
'powered by' => 'powered by',
'alt home' => 'Hem', # alt
'alt information' => 'Information', # alt
'alt dialup' => 'Uppringning', # alt
'alt remote access' => 'Extern Access', # alt
'alt services' => 'Tj�nster', # alt
'alt ids' => 'System f�r att Uppt�cka Intr�ng (IDS)', # alt
'alt vpn' => 'Virtuella Privata N�tverk', # alt
'alt logs' => 'Loggar', #alt 
'alt shutdown' => 'St�ng av', # alt
'alt shell' => 'Skal (Shell)', # alt
'alt updates' => 'Uppdateringar', # alt

# changepw.cgi
'admin user password has been changed' => 'L�senord f�r admin har �ndrats.',
'dial user password has been changed' => 'L�senord f�r uppringd anv�ndare har �ndrats.',
'password cant be blank' => 'L�senord kan inte vara tomt.',
'passwords do not match' => 'L�senorden �r olika.',
'change passwords' => '�ndra l�senord',
'administrator user password' => 'L�senord f�r administrat�r:',
'dial user password' => 'L�senord f�r uppringd anv�ndare:',
'again' => 'Repetera:',
'passwords must be at least 6 characters in length' => 'L�senordet m�ste vara minst 6 tecken l�ngt',
'password contains illegal characters' => 'L�senordet inneh�ller otill�tna tecken',

# credits.cgi
'credits' => 'Tack till',
'version' => 'Version: ',
'sponsors' => 'Sponsorer',
'links' => 'L�nkar',
'smoothwall homepage' => 'Hemsida f�r SmoothWall',
'translation teams' => '�vers�ttningsgrupper',

# dhcp.cgi
'invalid start address' => 'Ogiltig startadress.',
'invalid end address' => 'Ogiltig slutadress.',
'cannot specify secondary dns without specifying primary' => 'Kan inte specificera sekund�r DNS utan att specificera prim�r.',
'invalid default lease time' => 'Ogiltig standardleasetid.',
'invalid max lease time' => 'Ogiltig max leasetid.',
'dhcp server enabled' => 'DHCP server aktiverad.  Startar om.',
'dhcp server disabled' => 'DHCP server avaktiverad.  Stoppad.',
'dhcp configuration' => 'DHCP-konfiguration',
'start address' => 'Startadress:',
'end address' => 'Slutadress:',
'default lease time' => 'Standardl�netid (min):',	
'max lease time' => 'Max l�netid (min):',
'domain name suffix' => 'Dom�n (dom�n suffix):',	

# proxy.cgi
'web proxy configuration' => 'Konfiguration av webproxy',
'web proxyc' => 'Webproxy:',
'cache size' => 'Cachestorlek (MB):',
'invalid cache size' => 'Ogiltig cachestorlek.',
'remote proxy' => 'Fj�rrproxy:',
'invalid maximum object size' => 'Ogiltig maximal objektstorlek.',
'invalid minimum object size' => 'Ogiltig minimal objektstorlek.',
'invalid maximum outgoing size' => 'Ogiltig maximal utg�ende storlek.',
'invalid maximum incoming size' => 'Ogiltig maximal inkommande storlek.',
'transparent' => 'Transparent:',
'max size' => 'Max objektstorlek (KB):',
'min size' => 'Min objektstorlek (KB):',
'max outgoing size' => 'Max utg�ende storlek (KB):',
'max incoming size' => 'Max inkommande storlek (KB):', 

# common to logs cgis
'january' => 'Januari',
'february' => 'Februari',
'march' => 'Mars',
'april' => 'April',
'may' => 'Maj',
'june' => 'Juni',
'july' => 'Juli',
'august' => 'Augusti',
'september' => 'September',
'october' => 'Oktober',
'november' => 'November',
'december' => 'December',
'month' => 'M�nad:',
'day' => 'Dag:',
'update' => 'Uppdatera', # button
'export' => 'Exportera', # button
'older' => '�ldre',
'newer' => 'Nyare',
'settingsc' => 'Inst�llningar:',

# logs.cgi/firewalllog.dat
'firewall log' => 'Brandv�ggslogg',
'firewall log2' => 'Brandv�ggslogg:',
'date' => 'Datum:',
'time' => 'Tid',
'action' => '�tg�rd',
'chain' => 'Kedja',
'iface' => 'GrSnitt',
'protocol' => 'Proto',
'source' => 'K�lla',
'src port' => 'K�llport',
'destination' => 'Destination',
'dst port' => 'Dst port',
'unknown' => 'OK�ND',
'lookup' => 'Sl� upp',

# logs.cgi/log.dat
'log viewer' => 'Visa logg',
'section' => 'Sektion:',
'kernel' => 'Kernel',
'loginlogout' => 'Logga in/Logga ut',
'update transcript' => 'Uppdatera transcript',
'log' => 'Logg:',

# logs.cgi/proxylog.dat
'proxy log viewer' => 'Proxylogg visare',
'bad ignore filter' => 'Ignorerad�ligt-filter:',
'caps all' => 'ALLA',
'ignore filterc' => 'Ignorerafilter:',
'enable ignore filterc' => 'Aktivera ignorerafilter:',
'source ip' => 'K�ll-IP',
'website' => 'Websida',

# logs.cgi/ids.dat
'ids log viewer' => 'IDS-loggvisare',
'datec' => 'Datum:',
'namec' => 'Namn:',
'priorityc' => 'Prioritet:',
'typec' => 'Typ:',
'ipinfoc' => 'IP-info:',
'referencesc' => 'Referenser:',
'none found' => 'ingen hittad',

# index.cgi
'main page' => 'Huvudsida',
'dial' => 'Anslut', # button
'hangup' => 'LAGG PA', # button
'current profile' => 'Aktuell profil:',
'connected' => 'Ansluten',
'dialing' => 'Ansluter...',
'modem idle' => 'Modem inaktivt',
'isdn idle' => 'ISDN inaktivt',
'profile has errors' => 'Profilen inneh�ller fel',
'modem settings have errors' => 'Modeminst�llningar felaktiga',
'user pages' => 'Anv�ndarsidor',
'mstatus information' => 'Statusinformation',
'mnetwork traffic graphs' => 'Trafikgrafer&nbsp;n�tverk',
'administrator pages' => 'Administrat�rssidor',
'mppp setup' => 'Inst�llningar&nbsp;f�r&nbsp;PPP',
'mmodem configuration' => 'Inst�llningar&nbsp;f�r&nbsp;modem',
'mchange passwords' => '�ndra&nbsp;l�senord',
'mremote access' => 'Fj�rr�tkomst',
'mdhcp configuration' => 'Inst�llningar&nbsp;f�r&nbsp;DHCP',
'mproxy configuration' => 'Inst�llningar&nbsp;f�r&nbsp;Proxy',
'mport forwarding configuration' => 'Inst�llningar&nbsp;f�r&nbsp;vidarebefordran',
'mshutdown control' => 'Avsluta...',
'mlog viewer' => 'Visa&nbsp;logg',
'mfirewall log viewer' => 'Visa&nbsp;brandv�ggslogg',
'msecure shell' => 'S�kert&nbsp;skal&nbsp;(SSH)',
'modem dod waiting' => 'Modem Uppringning vid F�rfr�gan v�ntar',
'isdn dod waiting' => 'ISDN Uppringning vid F�rfr�gan v�ntar',
'pppoe idle' => 'PPPOE inaktivt',
'usbadsl idle' => 'USB ADSL inaktivt',
'pppoe dod waiting' => 'PPPOE Uppringning vid F�rfr�gan v�ntar',
'there are updates' => 'There are updates available for your system. Please go to the \"Updates\" section for more information.',
'updates is old1' => 'Your update file is ', 'updates is old2' => 'days old. We recommend you update it on the \"Updates\" page.',

# pppsetup.cgi
'profile name not given' => 'Profilnamn saknas.',
'telephone not set' => 'Telefonnummer saknas.',
'bad characters in the telephone number field' => 'Ogiltiga tecken i telefonnummer.',
'username not set' => 'Anv�ndarnamn saknas.',
'spaces not allowed in the username field' => 'Blanksteg ej till�tet i anv�ndarnamn.',
'password not set' => 'L�senord saknas.',
'spaces not allowed in the password field' => 'Blanksteg ej till�tet i l�senord.',
'idle timeout not set' => 'Inaktivitetstidsgr�ns ej inst�llt.',
'only digits allowed in the idle timeout' => 'Enbart siffror till�tna f�r inaktivitetstidsgr�nsen.',
'bad characters in script field' => 'Ogiltiga tecken i skriptf�lt',
'max retries not set' => 'Max antal f�rs�k ej angivet.',
'only digits allowed in max retries field' => 'Enbart siffror till�tna i max antal f�rs�k.',
'profile saved' => 'Profil sparad: ',
'select' => 'VALJ', # button
'profile made current' => 'Aktuell profil: ',
'the selected profile is empty' => 'Den valda profilen �r tom.',
'delete' => 'Radera', # button
'profile deleted' => 'Profil raderad: ',
'empty' => 'Tom',
'unnamed' => 'Inget namn',
'ppp setup' => 'Inst�llningar f�r PPP',
'profiles' => 'Profiler:',
'profile name' => 'Profilnamn:',
'telephony' => 'Telefon:',
'interface' => 'Gr�nssnitt:',
'modem on com1' => 'Modem p� COM1',
'modem on com2' => 'Modem p� COM2',
'modem on com3' => 'Modem p� COM3',
'modem on com4' => 'Modem p� COM4',
'isdn tty' => 'ISDN p� TTY',
'isdn1' => 'Enkanals ISDN',
'isdn2' => 'Tv�kanals ISDN',
'computer to modem rate' => 'Hastighet dator till modem:',
'number' => 'Nummer:',
'modem speaker on' => 'Modemh�gtalare p�:',
'dialing mode' => 'Uppringning:',
'tone' => 'Ton',
'pulse' => 'Puls',
'maximum retries' => 'Max antal f�rs�k:',
'idle timeout' => 'Inaktivitetstidsgr�ns (min; 0=inaktiverad):',
'persistent connection' => 'Best�ndig uppkoppling:',
'authentication' => 'Identifiering',
'method' => 'Metod:',
'pap or chap' => 'PAP eller CHAP',
'standard login script' => 'Standard loginskript',
'demon login script' => 'Loginskript f�r demon',
'other login script' => 'Loginskript �vrigt',
'script name' => 'Skriptnamn:',
'type' => 'Typ:',
'manual' => 'Manuell',
'automatic' => 'Automatisk',
'dod' => 'Uppringning vid F�rfr�gan:',
'dod for dns' => 'Uppringning vid F�rfr�gan f�r DNS:',
'connect on smoothwall restart' => 'Anslut vid �terstart av SmoothWall:',
'pppoe settings' => 'Ytterligare inst�llningar f�r PPPoE:',
'usb adsl settings' => 'Ytterligare inst�llningar f�r USB ADSL:',
'service name' => 'Servicenamn:',
'concentrator name' => 'Namn p� koncentrator:',
'vpi number' => 'VPI-nummer:',
'vci number' => 'VCI-nummer:',
'firmwarec' => 'Firmware:',
'firmware present' => 'N�rvarande',
'firmware not present' => '<B>Inte</B> n�rvarande',
'upload usb adsl firmware' => 'Uppladdning USB ADSL firmware',
'no usb adsl firmware' => 'Ingen USB ADSL firmware. Var sn�ll och ladda upp.',
'dial on demand for this interface is not supported' => 'Uppringning vid f�rfr�gan st�ds inte av detta gr�nssnitt.',
'unable to alter profiles while red is active' => 'Kan inte �ndra profiler n�r R�D �r aktiv.',

# remote.cgi
'ssh is enabled' => 'SSH �r aktiverad.  Startar om.',
'ssh is disabled' => 'SSH �r inaktiverad.  Stoppar.',
'remote access' => 'Fj�rr�tkomst',
'remote access2' => 'Fj�rr�tkomst:',

# shutdown.cgi
'shutting down smoothwall' => 'Avslutar SmoothWall',
'shutdown control' => 'Avsluta...',
'shutdown' => 'Avsluta', # button
'shutdown2' => 'Avsluta:',
'shutting down' => 'Avslutar',
'smoothwall has now shutdown' => 'SmoothWall har nu avslutats.',
'rebooting smoothwall' => 'Startar om SmoothWall',
'reboot' => 'Starta om', # button
'rebooting' => 'Startar om',
'smoothwall has now rebooted' => 'SmoothWall har nu startats om.',

# status.cgi
'web server' => 'Webserver',
'cron server' => 'CRON-server',
'dns proxy server' => 'DNS-proxyserver',
'logging server' => 'Loggserver',
'kernel logging server' => 'Kernel loggserver',
'secure shell server' => 'S�kert skal server (SSH)',
'vpn' => 'VPN',
'web proxy' => 'Webproxy',
'intrusion detection system' => 'System f�r att Uppt�cka Intr�ng (IDS)',
'status information' => 'Statusinformation',
'services' => 'Tj�nster:',
'memory' => 'Minne:',
'uptime and users' => 'Drifttid och anv�ndare:',
'interfaces' => 'Gr�nssnitt:',
'disk usage' => 'Diskutnyttjande:',
'loaded modules' => 'Laddade moduler:',
'kernel version' => 'Kernelversion:',
'stopped' => 'STOPPAD',
'running' => 'AKTIV',
'swapped' => 'SWAPPAD',

# portfw.cgi and dmzhole.cgi and xtaccess.cgi
'source port numbers' => 'K�llport m�ste vara ett nummer.',
'source is bad' => 'Inte en giltlig IP- eller n�tverksadress.',
'destination ip bad' => 'Ogiltig destinations-IP',
'destination port numbers' => 'Destinationsport m�ste vara ett nummer.',
'unable to open file' => 'Kan inte �ppna fil',
'source port in use' => 'K�llport anv�nds:',
'forwarding rule added' => 'Lagt till regel f�r vidarebefodring; startar om vidarebefodrare',
'forwarding rule removed' => 'Tagit bort regel f�r vidarebefodring; startar om vidarebefodrare',
'external access rule added' => 'Lagt till regel f�r extern access; startar om access kontrolant',
'external access rule removed' =>' Tagit bort regel f�r extern access; startar om access kontrolant',
'dmz pinhole rule added' => 'Lagt till DMZ pinhole regel; startar om DMZ pinhole',
'dmz pinhole rule removed' => 'Tagit bort DMZ pinhole regel; startar om DMZ pinhole',
'port forwarding configuration' => 'Konfiguration f�r vidarebefordring av portar',
'dmz pinhole configuration' => 'DMZ pinhole konfiguration',
'external access configuration' => 'Konfiguration av extern access',
'add a new rule' => 'L�gg till en ny regel:',
'sourcec' => 'K�ll-IP, eller n�tverk (blank f�r \"ALLA\"):',
'source ipc' => 'K�ll-IP:',
'source portc' => 'K�llport:',
'destination ipc' => 'Destinations-IP:',
'destination portc' => 'Destinationsport:',
'current rules' => 'Nuvarande regler:',
'source ip' => 'K�ll-IP',
'source port' => 'K�llport',
'destination ip' => 'Destinations-IP',
'destination port' => 'Destinationsport',
'add' => 'LAGG TILL', # button
'remove' => 'Ta bort', # button
'edit' => 'Redigera', # button
'enabledtitle' => 'Aktiverad',
'nothing selected' => 'Ingenting markerat',
'you can only select one item to edit' => 'Du kan bara markera ett f�rem�l f�r redigering',
'mark' => 'Markera',
'all' => 'ALLA',

# ddns.cgi
'dynamic dns' => 'Dynamisk DNS',
'add a host' => 'L�gg till en v�rd:',
'servicec' => 'Tj�nst:',
'behind a proxy' => 'Bakom en proxy:',
'enable wildcards' => 'Aktivera wildcards:',
'hostnamec' => 'V�rdnamn:',
'domainc' => 'Dom�n:',
'current hosts' => 'Nuvarande v�rdar:',
'service' => 'Tj�nst',
'hostname' => 'V�rd',
'domain' => 'Dom�n',
'proxy' => 'Proxy',
'wildcards' => 'Wildcards',
'hostname not set' => 'V�rdnamn inte inst�llt.',
'domain not set' => 'Dom�n inte inst�lld.',
'invalid hostname' => 'Ogiltigt v�rdnamn.',
'invalid domain name' => 'Ogiltigt dom�nnamn.',
'hostname and domain already in use' => 'V�rdnamn och dom�n anv�nds redan.',
'ddns hostname added' => 'Lagt till Dynamiskt DNS v�rdnamn',
'ddns hostname removed' => 'Tagit bort DNS v�rdnamn',
'force update' => 'Tvinga uppdatering',

# ipinfo.cgi
'ip info' => 'IP-information',
'lookup failed' => 'Omv�nd uppslagning misslyckades',

# shell.cgi
'secure shellc' => 'S�kert Skal (SSH):',

# modem.cgi
'restore defaults' => 'ATERSTALL GRUNDINSTALLNINGAR', # button
'timeout must be a number' => 'Timeout m�ste vara numeriskt.',
'modem configuration' => 'Konfiguration av modem',
'modem configurationc' => 'Konfiguration av modem:',
'init string' => 'Initiera:',
'hangup string' => 'L�gg p�:',
'speaker on' => 'H�gtalare till:',
'speaker off' => 'H�gtalare fr�n:',
'tone dial' => 'Tonringning:',
'pulse dial' => 'Pulsringning:',
'connect timeout' => 'Anslutningstimeout:',
'send cr' => 'ISP kr�ver vagnretur (CR):',

# vpnmain.cgi
'restart' => 'ATERSTARTA', # button
'stop' => 'Stoppa', # button
'vpn configuration main' => 'VPN-konfiguration - Allm�nt',
'main' => 'Allm�nt',
'connections' => 'Anslutningar',
'global settingsc' => 'Globala inst�llningar:',
'local vpn ip' => 'Lokal VPN-IP:',
'if blank the currently configured ethernet red address will be used' => 'Om blank, kommer aktuell R�D Ethernetadress att anv�ndas.',
'manual control and status' => 'Manuell hantering och status:',
'connection name' => 'Namn',
'connection status' => 'Status',
'capsclosed' => 'ST�NGD',
'capsdisabled' => 'INAKTIVERAD',
'capsopen' => '�PPEN',

# vpn.cgi/vpnconfig.dat
'name must only contain characters' => 'Namn f�r bara inneh�lla tecken.',
'left ip is invalid' => 'V�nster IP �r ogiltigt.',
'left next hop ip is invalid' => 'V�nster n�sta hopp IP �r ogiltigt.',
'left subnet is invalid' => 'V�nster subn�t �r ogiltigt.',
'right ip is invalid' => 'H�ger IP �r ogiltigt.',
'right next hop ip is invalid' => 'H�ger n�sta hopp IP �r ogiltigt.',
'right subnet is invalid' => 'H�ger subn�t �r ogiltigt.',
'vpn configuration connections' => 'VPN-konfiguration - Anslutningar',
'add a new connection' => 'L�gg till ny anslutning:',
'namec' => 'Namn:',
'leftc' => 'V�nster:',
'left next hopc' => 'V�nster n�sta hopp:',
'left subnetc' => 'V�nster subn�t:',
'rightc' => 'H�ger:',
'right next hopc' => 'H�ger n�sta hopp:',
'right subnetc' => 'H�ger subn�t:',
'secretc' => 'Hemlig:',
'current connections' => 'Aktuella anslutningar:',
'markc' => 'M�rk:',
'import and export' => 'Importera och exportera:',
'import' => 'Importera', # button

# graphs.cgi
'network traffic graphs' => 'N�tverkstrafikgrafer',
'network traffic graphsc' => 'N�tverkstrafikgrafer:',
'no graphs available' => 'Inga grafer tillg�ngliga.',
'no information available' => 'Ingen information tillg�nglig.',

# usbadsl.cgi
'usb adsl setup' => 'USB-ADSL-inst�llningar',
'usb adsl help' => 'F�r att anv�nda USB modem m�ste du ladda upp \"firmware\" till din SmoothWall burk. Ladda ner \"tarball\" fr�n Alcatel och ladda sen upp filen <B>mgmt.o</B> med hj�lp av formul�ret nedan.',
'upload' => 'Ladda upp', # button
'upload successfull' => 'Uppladdningen lyckades.',
'could not create file' => 'Kunde inte skapa fil.',
'mgmt upload' => 'USB-ADSL-drivrutins uppladdning:',
'upload filec' => 'Ladda upp fil:',

# updates.cgi
'updates' => 'Uppdateringar',
'could not open available updates file' => 'Kunde inte �ppna filen f�r tillg�ngliga uppdateringar.',
'could not download the available updates list' => 'Kunde inte ladda ner listan �ver tillg�ngliga uppdateringar.',
'could not create directory' => 'Kunde inte skapa skapa katalog.',
'could not open updatefile for writing' => 'Kunde inte �ppna filen f�r skrivning.',
'this is not an authorised update' => 'Detta �r inte en godk�nd uppdatering, eller s� �r din patchlista gammal.',
'this is not a valid archive' => 'Detta �r inte ett giltigt arkiv',
'could not open update information file' => 'Kunde inte �ppna filen med information om uppdateringar.  Filen �r korrumperad.',
'this update is already installed' => 'Denna uppdatering �r redan installerad.',
'package failed to install' => 'Misslyckades med installation av paket.',
'update installed but' => 'Uppdatering installerad men databasen �ver installerade paket kunde inte uppdateras',
'refresh update list' => 'FORNYA UPPDATERINGSLISTAN', # button
'installed updates' => 'Installerade uppdateringar:',
'id' => 'ID',
'title' => 'Titel',
'description' => 'Beskrivning',
'released' => 'Sl�ppt',
'installed' => 'Installerad',
'could not open installed updates file' => 'Kunde inte �ppna filen med installerade uppdateringar',
'available updates' => 'Tillg�ngliga uppdateringar:',
'there are updates available' => 'Det finns uppdateringar f�r ditt system. Det �r v�ldigt viktigt att du installerar dessa s� snart som m�jligt.',
'info' => 'Info',
'all updates installed' => 'All uppdateringar installerade',
'install new update' => 'Installera ny uppdatering:',
'to install an update' => 'Var sn�ll och ladda upp .tar.gz file nedan f�r att installera uppdateringen:',
'upload update file' => 'Ladda upp  uppdateringsfil:',
'could not download latest patch list' => 'Kunde inte ladda ner den senaste listan �ver patcher (inte ansluten).',
'could not connect to smoothwall org' => 'Kunde inte ansluta till smoothwall.org',
'successfully refreshed updates list' => 'Lyckades med att f�rnya uppdateringslistan.',
'the following update was successfully installedc' => 'F�ljande uppdatering var framg�ngsrikt installerad:',

# ids.cgi
'snort is enabled' => 'Snort �r aktiverat',
'snort is disabled' => 'Snort �r avaktiverat',
'intrusion detection system2' => 'System f�r att Uppt�cka Intr�ng (IDS):',

);
