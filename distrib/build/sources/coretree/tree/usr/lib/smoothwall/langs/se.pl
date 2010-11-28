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
'back' => 'Bakåt',
'help' => 'Hjälp',
'primary dns' => 'Primär DNS:',
'secondary dns' => 'Sekundär DNS:',
'invalid primary dns' => 'Ogiltig primär DNS.',
'invalid secondary dns' => 'Ogiltig sekundär DNS.',
'dhcp server' => 'DHCP-server',
'username' => 'Användarnamn:',
'password' => 'Lösenord:',
'enabled' => 'Aktiverad:',
'this field may be blank' => 'Detta fält kan vara tomt.',
'these fields may be blank' => 'Dessa fält kan vara tomma.',

# header.pl
'sshome' => 'hem',
'ssstatus' => 'status',
'sstraffic graphs' => 'trafikgrafer',
'ssppp settings' => 'ppp inställningar',
'ssmodem' => 'modem',
'ssusb adsl firmware upload' => 'usb adsl firmware uppladdning',
'ssssh' => 'ssh',
'sspasswords' => 'lösenord',
'ssweb proxy' => 'webproxy',
'ssdhcp' => 'dhcp',
'ssport forwarding' => 'vidarebefodring av portar',
'ssexternal service access' => 'extern åtkomst till tjänster',
'ssdmz pinholes' => 'dmz pinholes',
'ssdynamic dns' => 'dynamisk dns',
'ssids' => 'system för att upptäcka intrång (IDS)',
'sscontrol' => 'kontrol',
'ssconnections' => 'anslutningar',
'ssother' => 'annat',
'ssfirewall' => 'brandvägg',
'ssshutdown' => 'stäng av',
'ssshell' => 'skal (shell)',
'ssupdates' => 'uppdateringar',
'sshelp' => 'hjälp',
'sscredits' => 'tack till',
'ssip info' => 'IP-Information',
'powered by' => 'powered by',
'alt home' => 'Hem', # alt
'alt information' => 'Information', # alt
'alt dialup' => 'Uppringning', # alt
'alt remote access' => 'Extern Access', # alt
'alt services' => 'Tjänster', # alt
'alt ids' => 'System för att Upptäcka Intrång (IDS)', # alt
'alt vpn' => 'Virtuella Privata Nätverk', # alt
'alt logs' => 'Loggar', #alt 
'alt shutdown' => 'Stäng av', # alt
'alt shell' => 'Skal (Shell)', # alt
'alt updates' => 'Uppdateringar', # alt

# changepw.cgi
'admin user password has been changed' => 'Lösenord för admin har ändrats.',
'dial user password has been changed' => 'Lösenord för uppringd användare har ändrats.',
'password cant be blank' => 'Lösenord kan inte vara tomt.',
'passwords do not match' => 'Lösenorden är olika.',
'change passwords' => 'Ändra lösenord',
'administrator user password' => 'Lösenord för administratör:',
'dial user password' => 'Lösenord för uppringd användare:',
'again' => 'Repetera:',
'passwords must be at least 6 characters in length' => 'Lösenordet måste vara minst 6 tecken långt',
'password contains illegal characters' => 'Lösenordet innehåller otillåtna tecken',

# credits.cgi
'credits' => 'Tack till',
'version' => 'Version: ',
'sponsors' => 'Sponsorer',
'links' => 'Länkar',
'smoothwall homepage' => 'Hemsida för SmoothWall',
'translation teams' => 'översättningsgrupper',

# dhcp.cgi
'invalid start address' => 'Ogiltig startadress.',
'invalid end address' => 'Ogiltig slutadress.',
'cannot specify secondary dns without specifying primary' => 'Kan inte specificera sekundär DNS utan att specificera primär.',
'invalid default lease time' => 'Ogiltig standardleasetid.',
'invalid max lease time' => 'Ogiltig max leasetid.',
'dhcp server enabled' => 'DHCP server aktiverad.  Startar om.',
'dhcp server disabled' => 'DHCP server avaktiverad.  Stoppad.',
'dhcp configuration' => 'DHCP-konfiguration',
'start address' => 'Startadress:',
'end address' => 'Slutadress:',
'default lease time' => 'Standardlånetid (min):',	
'max lease time' => 'Max lånetid (min):',
'domain name suffix' => 'Domän (domän suffix):',	

# proxy.cgi
'web proxy configuration' => 'Konfiguration av webproxy',
'web proxyc' => 'Webproxy:',
'cache size' => 'Cachestorlek (MB):',
'invalid cache size' => 'Ogiltig cachestorlek.',
'remote proxy' => 'Fjärrproxy:',
'invalid maximum object size' => 'Ogiltig maximal objektstorlek.',
'invalid minimum object size' => 'Ogiltig minimal objektstorlek.',
'invalid maximum outgoing size' => 'Ogiltig maximal utgående storlek.',
'invalid maximum incoming size' => 'Ogiltig maximal inkommande storlek.',
'transparent' => 'Transparent:',
'max size' => 'Max objektstorlek (KB):',
'min size' => 'Min objektstorlek (KB):',
'max outgoing size' => 'Max utgående storlek (KB):',
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
'month' => 'Månad:',
'day' => 'Dag:',
'update' => 'Uppdatera', # button
'export' => 'Exportera', # button
'older' => 'Äldre',
'newer' => 'Nyare',
'settingsc' => 'Inställningar:',

# logs.cgi/firewalllog.dat
'firewall log' => 'Brandväggslogg',
'firewall log2' => 'Brandväggslogg:',
'date' => 'Datum:',
'time' => 'Tid',
'action' => 'Åtgärd',
'chain' => 'Kedja',
'iface' => 'GrSnitt',
'protocol' => 'Proto',
'source' => 'Källa',
'src port' => 'Källport',
'destination' => 'Destination',
'dst port' => 'Dst port',
'unknown' => 'OKÄND',
'lookup' => 'Slå upp',

# logs.cgi/log.dat
'log viewer' => 'Visa logg',
'section' => 'Sektion:',
'kernel' => 'Kernel',
'loginlogout' => 'Logga in/Logga ut',
'update transcript' => 'Uppdatera transcript',
'log' => 'Logg:',

# logs.cgi/proxylog.dat
'proxy log viewer' => 'Proxylogg visare',
'bad ignore filter' => 'Ignoreradåligt-filter:',
'caps all' => 'ALLA',
'ignore filterc' => 'Ignorerafilter:',
'enable ignore filterc' => 'Aktivera ignorerafilter:',
'source ip' => 'Käll-IP',
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
'profile has errors' => 'Profilen innehåller fel',
'modem settings have errors' => 'Modeminställningar felaktiga',
'user pages' => 'Användarsidor',
'mstatus information' => 'Statusinformation',
'mnetwork traffic graphs' => 'Trafikgrafer&nbsp;nätverk',
'administrator pages' => 'Administratörssidor',
'mppp setup' => 'Inställningar&nbsp;för&nbsp;PPP',
'mmodem configuration' => 'Inställningar&nbsp;för&nbsp;modem',
'mchange passwords' => 'Ändra&nbsp;lösenord',
'mremote access' => 'Fjärråtkomst',
'mdhcp configuration' => 'Inställningar&nbsp;för&nbsp;DHCP',
'mproxy configuration' => 'Inställningar&nbsp;för&nbsp;Proxy',
'mport forwarding configuration' => 'Inställningar&nbsp;för&nbsp;vidarebefordran',
'mshutdown control' => 'Avsluta...',
'mlog viewer' => 'Visa&nbsp;logg',
'mfirewall log viewer' => 'Visa&nbsp;brandväggslogg',
'msecure shell' => 'Säkert&nbsp;skal&nbsp;(SSH)',
'modem dod waiting' => 'Modem Uppringning vid Förfrågan väntar',
'isdn dod waiting' => 'ISDN Uppringning vid Förfrågan väntar',
'pppoe idle' => 'PPPOE inaktivt',
'usbadsl idle' => 'USB ADSL inaktivt',
'pppoe dod waiting' => 'PPPOE Uppringning vid Förfrågan väntar',
'there are updates' => 'There are updates available for your system. Please go to the \"Updates\" section for more information.',
'updates is old1' => 'Your update file is ', 'updates is old2' => 'days old. We recommend you update it on the \"Updates\" page.',

# pppsetup.cgi
'profile name not given' => 'Profilnamn saknas.',
'telephone not set' => 'Telefonnummer saknas.',
'bad characters in the telephone number field' => 'Ogiltiga tecken i telefonnummer.',
'username not set' => 'Användarnamn saknas.',
'spaces not allowed in the username field' => 'Blanksteg ej tillåtet i användarnamn.',
'password not set' => 'Lösenord saknas.',
'spaces not allowed in the password field' => 'Blanksteg ej tillåtet i lösenord.',
'idle timeout not set' => 'Inaktivitetstidsgräns ej inställt.',
'only digits allowed in the idle timeout' => 'Enbart siffror tillåtna för inaktivitetstidsgränsen.',
'bad characters in script field' => 'Ogiltiga tecken i skriptfält',
'max retries not set' => 'Max antal försök ej angivet.',
'only digits allowed in max retries field' => 'Enbart siffror tillåtna i max antal försök.',
'profile saved' => 'Profil sparad: ',
'select' => 'VALJ', # button
'profile made current' => 'Aktuell profil: ',
'the selected profile is empty' => 'Den valda profilen är tom.',
'delete' => 'Radera', # button
'profile deleted' => 'Profil raderad: ',
'empty' => 'Tom',
'unnamed' => 'Inget namn',
'ppp setup' => 'Inställningar för PPP',
'profiles' => 'Profiler:',
'profile name' => 'Profilnamn:',
'telephony' => 'Telefon:',
'interface' => 'Gränssnitt:',
'modem on com1' => 'Modem på COM1',
'modem on com2' => 'Modem på COM2',
'modem on com3' => 'Modem på COM3',
'modem on com4' => 'Modem på COM4',
'isdn tty' => 'ISDN på TTY',
'isdn1' => 'Enkanals ISDN',
'isdn2' => 'Tvåkanals ISDN',
'computer to modem rate' => 'Hastighet dator till modem:',
'number' => 'Nummer:',
'modem speaker on' => 'Modemhögtalare på:',
'dialing mode' => 'Uppringning:',
'tone' => 'Ton',
'pulse' => 'Puls',
'maximum retries' => 'Max antal försök:',
'idle timeout' => 'Inaktivitetstidsgräns (min; 0=inaktiverad):',
'persistent connection' => 'Beständig uppkoppling:',
'authentication' => 'Identifiering',
'method' => 'Metod:',
'pap or chap' => 'PAP eller CHAP',
'standard login script' => 'Standard loginskript',
'demon login script' => 'Loginskript för demon',
'other login script' => 'Loginskript övrigt',
'script name' => 'Skriptnamn:',
'type' => 'Typ:',
'manual' => 'Manuell',
'automatic' => 'Automatisk',
'dod' => 'Uppringning vid Förfrågan:',
'dod for dns' => 'Uppringning vid Förfrågan för DNS:',
'connect on smoothwall restart' => 'Anslut vid återstart av SmoothWall:',
'pppoe settings' => 'Ytterligare inställningar för PPPoE:',
'usb adsl settings' => 'Ytterligare inställningar för USB ADSL:',
'service name' => 'Servicenamn:',
'concentrator name' => 'Namn på koncentrator:',
'vpi number' => 'VPI-nummer:',
'vci number' => 'VCI-nummer:',
'firmwarec' => 'Firmware:',
'firmware present' => 'Närvarande',
'firmware not present' => '<B>Inte</B> närvarande',
'upload usb adsl firmware' => 'Uppladdning USB ADSL firmware',
'no usb adsl firmware' => 'Ingen USB ADSL firmware. Var snäll och ladda upp.',
'dial on demand for this interface is not supported' => 'Uppringning vid förfrågan stöds inte av detta gränssnitt.',
'unable to alter profiles while red is active' => 'Kan inte ändra profiler när RÖD är aktiv.',

# remote.cgi
'ssh is enabled' => 'SSH är aktiverad.  Startar om.',
'ssh is disabled' => 'SSH är inaktiverad.  Stoppar.',
'remote access' => 'Fjärråtkomst',
'remote access2' => 'Fjärråtkomst:',

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
'secure shell server' => 'Säkert skal server (SSH)',
'vpn' => 'VPN',
'web proxy' => 'Webproxy',
'intrusion detection system' => 'System för att Upptäcka Intrång (IDS)',
'status information' => 'Statusinformation',
'services' => 'Tjänster:',
'memory' => 'Minne:',
'uptime and users' => 'Drifttid och användare:',
'interfaces' => 'Gränssnitt:',
'disk usage' => 'Diskutnyttjande:',
'loaded modules' => 'Laddade moduler:',
'kernel version' => 'Kernelversion:',
'stopped' => 'STOPPAD',
'running' => 'AKTIV',
'swapped' => 'SWAPPAD',

# portfw.cgi and dmzhole.cgi and xtaccess.cgi
'source port numbers' => 'Källport måste vara ett nummer.',
'source is bad' => 'Inte en giltlig IP- eller nätverksadress.',
'destination ip bad' => 'Ogiltig destinations-IP',
'destination port numbers' => 'Destinationsport måste vara ett nummer.',
'unable to open file' => 'Kan inte öppna fil',
'source port in use' => 'Källport används:',
'forwarding rule added' => 'Lagt till regel för vidarebefodring; startar om vidarebefodrare',
'forwarding rule removed' => 'Tagit bort regel för vidarebefodring; startar om vidarebefodrare',
'external access rule added' => 'Lagt till regel för extern access; startar om access kontrolant',
'external access rule removed' =>' Tagit bort regel för extern access; startar om access kontrolant',
'dmz pinhole rule added' => 'Lagt till DMZ pinhole regel; startar om DMZ pinhole',
'dmz pinhole rule removed' => 'Tagit bort DMZ pinhole regel; startar om DMZ pinhole',
'port forwarding configuration' => 'Konfiguration för vidarebefordring av portar',
'dmz pinhole configuration' => 'DMZ pinhole konfiguration',
'external access configuration' => 'Konfiguration av extern access',
'add a new rule' => 'Lägg till en ny regel:',
'sourcec' => 'Käll-IP, eller nätverk (blank för \"ALLA\"):',
'source ipc' => 'Käll-IP:',
'source portc' => 'Källport:',
'destination ipc' => 'Destinations-IP:',
'destination portc' => 'Destinationsport:',
'current rules' => 'Nuvarande regler:',
'source ip' => 'Käll-IP',
'source port' => 'Källport',
'destination ip' => 'Destinations-IP',
'destination port' => 'Destinationsport',
'add' => 'LAGG TILL', # button
'remove' => 'Ta bort', # button
'edit' => 'Redigera', # button
'enabledtitle' => 'Aktiverad',
'nothing selected' => 'Ingenting markerat',
'you can only select one item to edit' => 'Du kan bara markera ett föremål för redigering',
'mark' => 'Markera',
'all' => 'ALLA',

# ddns.cgi
'dynamic dns' => 'Dynamisk DNS',
'add a host' => 'Lägg till en värd:',
'servicec' => 'Tjänst:',
'behind a proxy' => 'Bakom en proxy:',
'enable wildcards' => 'Aktivera wildcards:',
'hostnamec' => 'Värdnamn:',
'domainc' => 'Domän:',
'current hosts' => 'Nuvarande värdar:',
'service' => 'Tjänst',
'hostname' => 'Värd',
'domain' => 'Domän',
'proxy' => 'Proxy',
'wildcards' => 'Wildcards',
'hostname not set' => 'Värdnamn inte inställt.',
'domain not set' => 'Domän inte inställd.',
'invalid hostname' => 'Ogiltigt värdnamn.',
'invalid domain name' => 'Ogiltigt domännamn.',
'hostname and domain already in use' => 'Värdnamn och domän används redan.',
'ddns hostname added' => 'Lagt till Dynamiskt DNS värdnamn',
'ddns hostname removed' => 'Tagit bort DNS värdnamn',
'force update' => 'Tvinga uppdatering',

# ipinfo.cgi
'ip info' => 'IP-information',
'lookup failed' => 'Omvänd uppslagning misslyckades',

# shell.cgi
'secure shellc' => 'Säkert Skal (SSH):',

# modem.cgi
'restore defaults' => 'ATERSTALL GRUNDINSTALLNINGAR', # button
'timeout must be a number' => 'Timeout måste vara numeriskt.',
'modem configuration' => 'Konfiguration av modem',
'modem configurationc' => 'Konfiguration av modem:',
'init string' => 'Initiera:',
'hangup string' => 'Lägg på:',
'speaker on' => 'Högtalare till:',
'speaker off' => 'Högtalare från:',
'tone dial' => 'Tonringning:',
'pulse dial' => 'Pulsringning:',
'connect timeout' => 'Anslutningstimeout:',
'send cr' => 'ISP kräver vagnretur (CR):',

# vpnmain.cgi
'restart' => 'ATERSTARTA', # button
'stop' => 'Stoppa', # button
'vpn configuration main' => 'VPN-konfiguration - Allmänt',
'main' => 'Allmänt',
'connections' => 'Anslutningar',
'global settingsc' => 'Globala inställningar:',
'local vpn ip' => 'Lokal VPN-IP:',
'if blank the currently configured ethernet red address will be used' => 'Om blank, kommer aktuell RÖD Ethernetadress att användas.',
'manual control and status' => 'Manuell hantering och status:',
'connection name' => 'Namn',
'connection status' => 'Status',
'capsclosed' => 'STÄNGD',
'capsdisabled' => 'INAKTIVERAD',
'capsopen' => 'ÖPPEN',

# vpn.cgi/vpnconfig.dat
'name must only contain characters' => 'Namn får bara innehålla tecken.',
'left ip is invalid' => 'Vänster IP är ogiltigt.',
'left next hop ip is invalid' => 'Vänster nästa hopp IP är ogiltigt.',
'left subnet is invalid' => 'Vänster subnät är ogiltigt.',
'right ip is invalid' => 'Höger IP är ogiltigt.',
'right next hop ip is invalid' => 'Höger nästa hopp IP är ogiltigt.',
'right subnet is invalid' => 'Höger subnät är ogiltigt.',
'vpn configuration connections' => 'VPN-konfiguration - Anslutningar',
'add a new connection' => 'Lägg till ny anslutning:',
'namec' => 'Namn:',
'leftc' => 'Vänster:',
'left next hopc' => 'Vänster nästa hopp:',
'left subnetc' => 'Vänster subnät:',
'rightc' => 'Höger:',
'right next hopc' => 'Höger nästa hopp:',
'right subnetc' => 'Höger subnät:',
'secretc' => 'Hemlig:',
'current connections' => 'Aktuella anslutningar:',
'markc' => 'Märk:',
'import and export' => 'Importera och exportera:',
'import' => 'Importera', # button

# graphs.cgi
'network traffic graphs' => 'Nätverkstrafikgrafer',
'network traffic graphsc' => 'Nätverkstrafikgrafer:',
'no graphs available' => 'Inga grafer tillgängliga.',
'no information available' => 'Ingen information tillgänglig.',

# usbadsl.cgi
'usb adsl setup' => 'USB-ADSL-inställningar',
'usb adsl help' => 'För att använda USB modem måste du ladda upp \"firmware\" till din SmoothWall burk. Ladda ner \"tarball\" från Alcatel och ladda sen upp filen <B>mgmt.o</B> med hjälp av formuläret nedan.',
'upload' => 'Ladda upp', # button
'upload successfull' => 'Uppladdningen lyckades.',
'could not create file' => 'Kunde inte skapa fil.',
'mgmt upload' => 'USB-ADSL-drivrutins uppladdning:',
'upload filec' => 'Ladda upp fil:',

# updates.cgi
'updates' => 'Uppdateringar',
'could not open available updates file' => 'Kunde inte öppna filen för tillgängliga uppdateringar.',
'could not download the available updates list' => 'Kunde inte ladda ner listan över tillgängliga uppdateringar.',
'could not create directory' => 'Kunde inte skapa skapa katalog.',
'could not open updatefile for writing' => 'Kunde inte öppna filen för skrivning.',
'this is not an authorised update' => 'Detta är inte en godkänd uppdatering, eller så är din patchlista gammal.',
'this is not a valid archive' => 'Detta är inte ett giltigt arkiv',
'could not open update information file' => 'Kunde inte öppna filen med information om uppdateringar.  Filen är korrumperad.',
'this update is already installed' => 'Denna uppdatering är redan installerad.',
'package failed to install' => 'Misslyckades med installation av paket.',
'update installed but' => 'Uppdatering installerad men databasen över installerade paket kunde inte uppdateras',
'refresh update list' => 'FORNYA UPPDATERINGSLISTAN', # button
'installed updates' => 'Installerade uppdateringar:',
'id' => 'ID',
'title' => 'Titel',
'description' => 'Beskrivning',
'released' => 'Släppt',
'installed' => 'Installerad',
'could not open installed updates file' => 'Kunde inte öppna filen med installerade uppdateringar',
'available updates' => 'Tillgängliga uppdateringar:',
'there are updates available' => 'Det finns uppdateringar för ditt system. Det är väldigt viktigt att du installerar dessa så snart som möjligt.',
'info' => 'Info',
'all updates installed' => 'All uppdateringar installerade',
'install new update' => 'Installera ny uppdatering:',
'to install an update' => 'Var snäll och ladda upp .tar.gz file nedan för att installera uppdateringen:',
'upload update file' => 'Ladda upp  uppdateringsfil:',
'could not download latest patch list' => 'Kunde inte ladda ner den senaste listan över patcher (inte ansluten).',
'could not connect to smoothwall org' => 'Kunde inte ansluta till smoothwall.org',
'successfully refreshed updates list' => 'Lyckades med att förnya uppdateringslistan.',
'the following update was successfully installedc' => 'Följande uppdatering var framgångsrikt installerad:',

# ids.cgi
'snort is enabled' => 'Snort är aktiverat',
'snort is disabled' => 'Snort är avaktiverat',
'intrusion detection system2' => 'System för att Upptäcka Intrång (IDS):',

);
