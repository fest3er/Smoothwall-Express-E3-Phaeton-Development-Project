# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# (c) Dutch Translation Team:
# Filip Onkelinx
# Ard Beenhakker
# Paul Rijnhout
# Maurits De Loe
# Narfhimself (WouterArts)
# Alfred de Klerk
# Wim Van den Meutter
# Sjoerd Skoelmans
# Tim Dobbelaere
# Pim Aarts
# Jeroen Wijnands
# Marnix Janse
# Walter Hebberecht
# Olaf Beumer
# Jorrit van Eijk
# J.van der zijden
# Vincent Homans
# Robert Vis
# Nico Oosterwijk "MrMan"
# Hans van Zijl IT diensten
# Jochen Van Den Bossche

%basetr = (

%basetr,

# common
'invalid input' => 'Ongeldige ingave',
'save' => 'Bewaren', # button
'refresh' => 'Opnieuw', # button
'restore' => 'Terugzetten', # button
'error messages' => 'Fout meldingen:',
'back' => 'TERUG',
'help' => 'Help',
'primary dns' => 'Primaire DNS:',
'secondary dns' => 'Secondaire DNS:',
'invalid primary dns' => 'Ongeldige primaire DNS.',
'invalid secondary dns' => 'Ongeldige secondaire DNS.',
'dhcp server' => 'DHCP server',
'username' => 'Gebruikersnaam:',
'password' => 'Wachtwoord:',
'enabled' => 'Aan:',
'this field may be blank' => 'Dit veld mag leeg zijn.',
'these fields may be blank' => 'Deze velden mogen leeg zijn.',

# header.pl
'sshome' => 'begin',
'ssstatus' => 'status',
'sstraffic graphs' => 'Gebruiksgrafieken',
'ssppp settings' => 'PPP instellingen',
'ssmodem' => 'modem',
'ssusb adsl firmware upload' => 'usb adsl firmware upload',
'ssssh' => 'ssh',
'sspasswords' => 'wachtwoorden',
'ssweb proxy' => 'web proxy',
'ssdhcp' => 'dhcp',
'ssport forwarding' => 'poort doorsturing',
'ssexternal service access' => 'externe service toegang',
'ssdmz pinholes' => 'dmz pinholes',
'ssdynamic dns' => 'dynamische dns',
'ssids' => 'inbraak detectie systeem',
'sscontrol' => 'controle',
'ssconnections' => 'connecties',
'ssother' => 'anders',
'ssfirewall' => 'firewall',
'ssshutdown' => 'uitschakelen',
'ssshell' => 'shell',
'ssupdates' => 'updates',
'sshelp' => 'help',
'sscredits' => 'credits',
'ssip info' => 'IP Informatie',
'powered by' => 'powered by',
'alt home' => 'Begin', # alt
'alt information' => 'Informatie', # alt
'alt dialup' => 'Inbellen', # alt
'alt remote access' => 'Toegang van buitenaf', # alt
'alt services' => 'Services', # alt
'alt ids' => 'Inbraak detectie systeem', # alt
'alt vpn' => 'Virtuele Prive Netwerken', # alt
'alt logs' => 'Logs', #alt 
'alt shutdown' => 'Uitschakelen', # alt
'alt shell' => 'Shell', # alt
'alt updates' => 'Updates', # alt

# changepw.cgi
'admin user password has been changed' => 'wachtwoord van de Admin is veranderd.',
'dial user password has been changed' => 'wachtwoord van de inbel gebruiker is veranderd.',
'password cant be blank' => 'wachtwoord mag niet leeg zijn.',
'passwords do not match' => 'wachtwoorden zijn niet hetzelfde.',
'change passwords' => 'Verander wachtwoorden',
'administrator user password' => 'Admin gebruikers wachtwoord:',
'dial user password' => 'Inbel gebruikers wachtwoord:',
'again' => 'Herhaal:',
'passwords must be at least 6 characters in length' => 'passwords moeten minimaal 6 karakters lang zijn',
'password contains illegal characters' => 'Wachtwoord bevat ongeldige karakters.',
'passwords must be at least 6 characters in length' => 'uw wachtwoord moet minstens 6 karakters lang zijn',

# credits.cgi
'credits' => 'Credit',
'version' => 'Versie: ',
'sponsors' => 'Sponsors',
'links' => 'Links',
'smoothwall homepage' => 'SmoothWall Homepage',
'translation teams' => 'Vertalings team',

# dhcp.cgi
'invalid start address' => 'Niet geldig start adres.',
'invalid end address' => 'Niet geldig eind adres.',
'cannot specify secondary dns without specifying primary' => 'Kan geen secondaire DNS specificeren zonder een primaire DNS.',
'invalid default lease time' => 'Ongeldige standaard lease tijd.',
'invalid max lease time' => 'Ongeldige maximale lease tijd.',
'dhcp server enabled' => 'DHCP server aan.  Herstart.',
'dhcp server disabled' => 'DHCP server uit.  Gestopt.',
'dhcp configuration' => 'DHCP configuratie',
'start address' => 'Start adres:',
'end address' => 'Eind adres:',
'default lease time' => 'Standaard lease tijd (min):',
'max lease time' => 'Max lease tijd (min):',
'domain name suffix' => 'Domeinnaam achtervoegsel:',

# proxy.cgi
'web proxy configuration' => 'Web proxy configuratie',
'web proxyc' => 'Web proxy:',
'cache size' => 'Cache grootte (MB):',
'invalid cache size' => 'Ongeldige cache grootte.',
'remote proxy' => 'Remote proxy:',
'invalid maximum object size' => 'Ongeldige maximale object grootte.',
'invalid minimum object size' => 'Ongeldige minimale object grootte.',
'invalid maximum outgoing size' => 'Ongeldige maximale uitgaande grootte.',
'invalid maximum incoming size' => 'Ongeldige maximale inkomende grootte.',
'transparent' => 'Transparant:',
'max size' => 'Max object grootte (KB):',
'min size' => 'Min object grootte (KB):',
'max outgoing size' => 'Max uitgaande grootte (KB):',
'max incoming size' => 'Max inkomende grootte (KB):',

# common to logs cgis
'january' => 'Januari',
'february' => 'Februari',
'march' => 'Maart',
'april' => 'April',
'may' => 'Mei',
'june' => 'Juni',
'july' => 'Juli',
'august' => 'Augustus',
'september' => 'September',
'october' => 'Oktober',
'november' => 'November',
'december' => 'December',
'month' => 'Maand:',
'day' => 'Dag:',
'update' => 'Update', # button
'export' => 'Export', # button
'older' => 'Ouder',
'newer' => 'Nieuwer',
'settingsc' => 'Instellingen:',

# logs.cgi/firewalllog.dat
'firewall log' => 'Firewall log',
'firewall log2' => 'Firewall log:',
'date' => 'Datum:',
'time' => 'Tijd',
'action' => 'Actie',
'chain' => 'Regel',
'iface' => 'Iface',
'protocol' => 'Proto',
'source' => 'Bron',
'src port' => 'Bron Poort',
'destination' => 'Bestemming',
'dst port' => 'Bestemming Poort',
'unknown' => 'ONBEKEND',
'lookup' => 'Opzoeken',

# logs.cgi/log.dat
'log viewer' => 'Log bekijken',
'section' => 'Sectie:',
'kernel' => 'Kernel',
'loginlogout' => 'Login/Logout',
'update transcript' => 'Update transcript',
'log' => 'Log:',

# logs.cgi/proxylog.dat
'proxy log viewer' => 'Proxy log bekijken',
'bad ignore filter' => 'Foutief negeer filter:',
'caps all' => 'ALLES',
'ignore filterc' => 'Negeer filter:',
'enable ignore filterc' => 'Activeer negeer filter:',
'source ip' => 'Bron IP',
'website' => 'Website',

# logs.cgi/ids.dat
'ids log viewer' => 'IDS log bekijken',
'datec' => 'Datum:',
'namec' => 'Naam:',
'priorityc' => 'Prioriteit:',
'typec' => 'Type:',
'ipinfoc' => 'IP info:',
'referencesc' => 'Referentie:',
'none found' => 'niets gevonden',

# index.cgi
'main page' => 'Hoofd pagina',
'dial' => 'Verbinden', # button
'hangup' => 'Stop verbinding', # button
'current profile' => 'Huidig profiel:',
'connected' => 'Verbonden',
'dialing' => 'Bezig verbinding te maken...',
'modem idle' => 'Modem inactief',
'isdn idle' => 'ISDN inactief',
'profile has errors' => 'Profiel bevat fouten',
'modem settings have errors' => 'Modem instellingen bevat fouten',
'user pages' => 'Gebruikers pagina',
'mstatus information' => 'Status&nbsp;informatie',
'mnetwork traffic graphs' => 'Netwerk&nbsp;verkeer&nbsp;grafieken',
'administrator pages' => 'Beheerders pagina',
'mppp setup' => 'PPP&nbsp;setup',
'mmodem configuration' => 'Modem&nbsp;configuratie',
'mchange passwords' => 'Wijzig&nbsp;wachtwoord',
'mremote access' => 'Buitenaf&nbsp;toegang',
'mdhcp configuration' => 'DHCP&nbsp;configuratie',
'mproxy configuration' => 'Proxy&nbsp;configuratie',
'mport forwarding configuration' => 'Poort&nbsp;doorsturings&nbsp;configuratie',
'mshutdown control' => 'Afsluit&nbsp;controle',
'mlog viewer' => 'Log&nbsp;bekijken',
'mfirewall log viewer' => 'Firewall&nbsp;log&nbsp;bekijken',
'msecure shell' => 'Beveiligde&nbsp;shell',
'modem dod waiting' => 'Modem Dial on Demand waiting',
'isdn dod waiting' => 'ISDN Dial on Demand wachtend',
'pppoe idle' => 'PPPoE inactief',
'usbadsl idle' => 'USB ADSL inactief',
'pppoe dod waiting' => 'PPPoE Dial on Demand wachtend',
'there are updates' => 'Er zijn updates beschikbaar voor uw systeem. Ga alstublieft naar de "Updates" sectie voor meer informatie.',
'updates is old1' => 'Uw update bestand is ',
'updates is old2' => 'dagen oud. Wij bevelen u aan te updaten op de "Updates" pagina.',

# pppsetup.cgi
'profile name not given' => 'Profiel naam niet opgegeven.',
'telephone not set' => 'Telefoon niet ingevuld.',
'bad characters in the telephone number field' => 'Ongeldige karakters in het telefoon nummerveld.',
'username not set' => 'Gebruikersnaam niet ingevuld.',
'spaces not allowed in the username field' => 'Spaties niet toegestaan in de gebruikersnaam.',
'password not set' => 'wachtwoord niet ingevuld.',
'spaces not allowed in the password field' => 'Spaties niet toegestaan in het wachtwoord.',
'idle timeout not set' => 'Timeout voor inactiviteit niet ingesteld.',
'only digits allowed in the idle timeout' => 'Alleen nummers toegestaan in timeout voor inactiviteit.',
'bad characters in script field' => 'Ongeldige karakters in het script veld',
'max retries not set' => 'Max herhalingen niet ingevuld.',
'only digits allowed in max retries field' => 'Alleen nummers toegestaan in het max herhalingen veld.',
'profile saved' => 'Profiel opgeslagen: ',
'select' => 'Selecteer', # button
'profile made current' => 'Profiel is huidig gemaakt: ',
'the selected profile is empty' => 'Het geselcteerde profiel is leeg.',
'delete' => 'Wis', # button
'profile deleted' => 'Profiel gewist: ',
'empty' => 'Leeg',
'unnamed' => 'Geen naam',
'ppp setup' => 'PPP setup',
'profiles' => 'Profielen:',
'profile name' => 'Profiel naam:',
'telephony' => 'Telefoon:',
'interface' => 'Interface:',
'modem on com1' => 'Modem op COM1',
'modem on com2' => 'Modem op COM2',
'modem on com3' => 'Modem op COM3',
'modem on com4' => 'Modem op COM4',
'isdn tty' => 'ISDN op TTY',
'isdn1' => 'Enkele ISDN',
'isdn2' => 'Dubbele ISDN',
'computer to modem rate' => 'Computer naar modem ratio:',
'number' => 'Nummer:',
'modem speaker on' => 'Modem speaker aan:',
'dialing mode' => 'Manier van bellen:',
'tone' => 'Toon',
'pulse' => 'Puls',
'maximum retries' => 'Maximaal aantal keer proberen:',
'idle timeout' => 'Timeout voor inactiviteit (min; 0 voor deactiveren):',
'persistent connection' => 'Blijvende connectie:',
'authentication' => 'Authenticatie:',
'method' => 'Methode:',
'pap or chap' => 'PAP of CHAP',
'standard login script' => 'Standaard login script',
'demon login script' => 'Demon login script',
'other login script' => 'Ander login script',
'script name' => 'Script naam:',
'type' => 'Type:',
'manual' => 'Handmatig',
'automatic' => 'Automatisch',
'dod' => 'Dial on Demand:',
'dod for dns' => 'Dial on Demand voor DNS:',
'connect on smoothwall restart' => 'Connectie na herstart SmoothWall:',
'pppoe settings' => 'Additionele PPPoE instellingen:',
'usb adsl settings' => 'Additionele USB ADSL instellingen:',
'service name' => 'Service naam:',
'concentrator name' => 'Concentrator naam:',
'vpi number' => 'VPI Nummer:',
'vci number' => 'VCI Nummer:',
'firmwarec' => 'Firmware:',
'firmware present' => 'Aanwezig',
'firmware not present' => '<B>Niet</B> aanwezig',
'upload usb adsl firmware' => 'Upload USB ADSL firmware',
'unable to alter profiles while red is active' => 'Niet mogelijk om het profiel te wijzigen als ROOD actief is.',
'dial on demand for this interface is not supported' => 'Dial on Demand voor deze interface wordt niet ondersteunt.',

# remote.cgi
'ssh is enabled' => 'SSH is geactiveerd.  Bezig met herstarten.',
'ssh is disabled' => 'SSH is gedeactiveerd.  Bezig met stoppen.',
'remote access' => 'Toegang van buitenaf',
'remote access2' => 'Toegang van buitenaf:',

# shutdown.cgi
'shutting down smoothwall' => 'Bezig met afsluiten van SmoothWall',
'shutdown control' => 'Afsluit controle',
'shutdown' => 'Afsluiten', # button
'shutdown2' => 'Afsluiten:',
'shutting down' => 'Bezig met afsluiten',
'smoothwall has now shutdown' => 'SmoothWall is afgesloten.',
'rebooting smoothwall' => 'Smoothwall wordt herstart.',
'reboot' => 'Herstart', # button
'rebooting' => 'Herstarten',
'smoothwall has now rebooted' => 'Smoothwall is nu herstart.',

# status.cgi
'web server' => 'Web server',
'cron server' => 'CRON server',
'dns proxy server' => 'DNS proxy server',
'logging server' => 'Log server',
'kernel logging server' => 'Kernel log server',
'secure shell server' => 'Beveiligde shell server',
'vpn' => 'VPN',
'web proxy' => 'Web proxy',
'intrusion detection system' => 'Inbraak Detectie Systeem',
'status information' => 'Status informatie',
'services' => 'Services:',
'memory' => 'Geheugen:',
'uptime and users' => 'Uptime en gebruikers:',
'interfaces' => 'Interfaces:',
'disk usage' => 'Harde schijf gebruik:',
'loaded modules' => 'Geladen modules:',
'kernel version' => 'Kernel versie:',
'stopped' => 'GESTOPT',
'running' => 'DRAAIEND',
'swapped' => 'GESWAPPED',

# portfw.cgi and dmzhole.cgi and xtaccess.cgi
'source port numbers' => 'Bron poort moet een nummer zijn.',
'source is bad' => 'Ongeldig IP adres of netwerk adres.',
'destination ip bad' => 'Ongeldig bestemmings IP',
'destination port numbers' => 'Bestemmings poort moet een nummer zijn.',
'unable to open file' => 'Kan bestand niet openen',
'source port in use' => 'Bron poort in gebruik:',
'forwarding rule added' => 'Regel voor poortdoorsturing toegevoegd; Doorstuurder wordt herstart',
'forwarding rule removed' => 'Regel voor poortdoorsturing verwijderd; Doorstuurder wordt herstart',
'external access rule added' => 'Regel toegang van buitenaf toegevoegd; toegang controle wordt herstart',
'external access rule removed' =>' Regel toegang van buitenaf verwijderd; toegang controle wordt herstart',
'dmz pinhole rule added' => 'DMZ Pinhole regel toegevoegd.',
'dmz pinhole rule removed' => 'DMZ Pinhole regel verwijderd.',
'port forwarding configuration' => 'Configuratie poortdoorsturing',
'dmz pinhole configuration' => 'DMZ pinhole configuratie:',
'external access configuration' => 'configuratie toegang van buitenaf',
'add a new rule' => 'Nieuwe regel toevoegen:',
'sourcec' => 'Bron IP, of netwerk (leeg is "ALLES"):',
'source ipc' => 'Bron IP:',
'source portc' => 'Bron poort:',
'destination ipc' => 'Bestemmings IP:',
'destination portc' => 'Bestemmings poort:',
'current rules' => 'Huidige regels:',
'source ip' => 'Bron IP',
'source port' => 'Bron poort',
'destination ip' => 'Bestemmings IP',
'destination port' => 'Bestemmings pport',
'add' => 'Toevoegen', # button
'remove' => 'Verwijderen', # button
'edit' => 'Wijzigen', # button
'enabledtitle' => 'Aan',
'nothing selected' => 'Niets geselecteerd',
'you can only select one item to edit' => 'U kan maar 1 item selecteren om te wijzigen',
'mark' => 'Markering',
'all' => 'ALLES',
'no usb adsl firmware' => 'Geen usb adsl firmware.',

# ddns.cgi
'dynamic dns' => 'Dynamische DNS',
'add a host' => 'Host toevoegen:',
'servicec' => 'Service:',
'behind a proxy' => 'Achter een proxy:',
'enable wildcards' => 'Wildcards aanzetten:',
'hostnamec' => 'Hostnaam:',
'domainc' => 'Domein:',
'current hosts' => 'Huidige hosts:',
'service' => 'Service',
'hostname' => 'Hostnaam',
'domain' => 'Domein',
'proxy' => 'Proxy',
'wildcards' => 'Wildcards',
'hostname not set' => 'Hostnaam niet opgegeven.',
'domain not set' => 'Domein niet opgegeven.',
'invalid hostname' => 'Ongeldig hostnaam.',
'invalid domain name' => 'Ongeldig domein naam.',
'hostname and domain already in use' => 'Hostnaam en domein zijn al in gebruik.',
'ddns hostname added' => 'Dynamische DNS hostnaam toegevoegd',
'ddns hostname removed' => 'Dynamische DNS hostnaam verwijderd',
'force update' => 'Geforceerde update',

# ipinfo.cgi
'ip info' => 'IP Informatie',
'lookup failed' => 'Reverse lookup mislukt',

# shell.cgi
'secure shellc' => 'Beveiligde shell:',

# modem.cgi
'restore defaults' => 'Default instellingen terugzetten', # button
'timeout must be a number' => 'Timeout moet een nummer zijn.',
'modem configuration' => 'Modem configuratie',
'modem configurationc' => 'Modem configuratie:',
'init string' => 'Init:',
'hangup string' => 'Ophangen:',
'speaker on' => 'Luidspreker aan:',
'speaker off' => 'Luidspreker uit:',
'tone dial' => 'Toon dial:',
'pulse dial' => 'Puls dial:',
'connect timeout' => 'Connectie timeout:',
'send cr' => 'ISP verwacht een Carriage Return:',

# vpnmain.cgi
'restart' => 'Herstart',
'stop' => 'Stop',
'vpn configuration main' => 'VPN configuratie - Begin',
'main' => 'Begin',
'connections' => 'Connecties',
'global settingsc' => 'Globale instellingen:',
'local vpn ip' => 'Lokale VPN IP:',
'if blank the currently configured ethernet red address will be used' => 'Wanneer leeg gelaten wordt het huidige RODE ethernet adres gebruikt.',
'manual control and status' => 'Handmatige controle en status:',
'connection name' => 'Naam',
'connection status' => 'Status',
'capsclosed' => 'GESLOTEN',
'capsdisabled' => 'UITGESCHAKELD',
'capsopen' => 'OPEN',

# vpn.cgi/vpnconfig.dat
'name must only contain characters' => 'Naam mag alleen karakters bevatten.',
'left ip is invalid' => 'Linker IP is ongeldig.',
'left next hop ip is invalid' => 'IP nummer linker vlogende hop is ongeldig.',
'left subnet is invalid' => 'Linker subnet is ongeldig.',
'right ip is invalid' => 'Rechter IP is ongeldig.',
'right next hop ip is invalid' => 'IP nummer rechter volgende hop is ongeldig.',
'right subnet is invalid' => 'Rechter subnet is ongeldig.',
'vpn configuration connections' => 'VPN configuratie - Verbindingen',
'add a new connection' => 'Voeg een nieuwe connectie toe:',
'namec' => 'Naam:',
'leftc' => 'Links:',
'left next hopc' => 'Linker volgende hop:',
'left subnetc' => 'Linker subnet:',
'rightc' => 'Rechts:',
'right next hopc' => 'Rechter volgende hop:',
'right subnetc' => 'Rechter subnet:',
'secretc' => 'Geheim:',
'current connections' => 'Huidige connecties:',
'markc' => 'Markeer:',
'import and export' => 'Import en Export:',
'import' => 'Import', # button

# graphs.cgi
'network traffic graphs' => 'Grafieken Netwerkverkeer',
'network traffic graphsc' => 'Grafieken Netwerkverkeer:',
'no graphs available' => 'Geen grafieken beschikbaar.',
'no information available' => 'Geen informatie beschikbaar.',

# usbadsl.cgi
'usb adsl setup' => 'USB ADSL setup',
'usb adsl help' => 'Om het USB modem te activeren moet u de firmware uploaden naar uw Smoothwall machine. Download alstublieft de tarball van Alcatel en upload het bestand <B>mgmt.o</B> door middel van het formulier hier beneden.',
'upload' => 'Upload', # button
'upload successfull' => 'Upload succesvol.',
'could not create file' => 'Kan geen bestand maken.',
'mgmt upload' => 'USB ADSL driver upload:',
'upload filec' => 'Upload bestand:',

# updates.cgi
'updates' => 'Updates',
'could not open available updates file' => 'Kan de lijst met beschikbare updates niet openen.',
'could not download the available updates list' => 'Kan de lijst met beschikbare updates niet downloaden.',
'could not create directory' => 'Kan geen directory maken.',
'could not open updatefile for writing' => 'Kan het update bestand niet openen.',
'this is not an authorised update' => 'Dit is geen geauthoriseerde update, of uw patchlijst is verouderd.',
'this is not a valid archive' => 'Dit is niet een geldig archief bestand.',
'could not open update information file' => 'Kon de update informatie file niet openen.  Het update bestand is verminkt.',
'this update is already installed' => 'Deze update is al geinstalleerd.',
'package failed to install' => 'Kan het bestand niet installeren.',
'update installed but' => 'update is geinstalleerd maar de database met geinstalleerde updates kon niet worden geupdate',
'refresh update list' => 'Herlaad update lijst', # button
'installed updates' => 'Geinstalleerde updates:',
'id' => 'ID',
'title' => 'Titel',
'description' => 'Omschrijving',
'released' => 'Vrijgegeven',
'installed' => 'Geinstalleerd',
'could not open installed updates file' => 'Kan het geinstalleerde update bestand niet openen',
'available updates' => 'Beschikbare updates:',
'there are updates available' => 'Er zijn updates beschikbaar voor uw systeem. Het is zeer aanbevolen deze zo spoedig mogelijk te installeren.',
'info' => 'Info',
'all updates installed' => 'Alle updates zijn geinstalleerd',
'install new update' => 'Installeer de nieuwe update:',
'to install an update' => 'Upload het tar.gz. bestand om de update te installeren:',
'upload update file' => 'Upload update bestand:',
'could not download latest patch list' => 'Kan niet de laatste patchlijst laden (niet verbonden).',
'could not connect to smoothwall org' => 'Kan geen connectie maken met smoothwall.org',
'successfully refreshed updates list' => 'Herladen updatelijst successvol.',
'the following update was successfully installedc' => 'De volgende update was succesvol geinstalleerd:',

# ids.cgi
'snort is enabled' => 'Snort staat aan',
'snort is disabled' => 'Snort is uit',
'intrusion detection system2' => 'Inbraak Detectie Systeem:',

);
