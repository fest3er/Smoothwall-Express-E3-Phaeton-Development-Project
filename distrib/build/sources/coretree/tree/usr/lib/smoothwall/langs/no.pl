# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# (c) Norweigen translation team:
# Asbjorn Aarrestad
# Tore Hoiland Aarrestad

%basetr = (

%basetr,

# common
'invalid input' => 'Skrevet feil',
'save' => 'Lagre', # button
'refresh' => 'Oppdater', # button
'restore' => 'Gjenopprett', # button
'error messages' => 'Feilmeldinger:',
'back' => 'TILBAKE',
'help' => 'Hjelp',
'primary dns' => 'Prim&aelig;r DNS:',
'secondary dns' => 'Sekund&aelig;r DNS:',
'invalid primary dns' => 'Ugyldig prim&aelig;r DNS.',
'invalid secondary dns' => 'Ugyldig sekund&aelig;r DNS.',
'dhcp server' => 'DHCP tjener',
'username' => 'Brukernavn:',
'password' => 'Passord:',
'enabled' => 'Aktivert:',
'this field may be blank' => 'Dette feltet kan v&aelig;re blankt.',
'these fields may be blank' => 'Disse feltene kan v&aelig;re blanke.',

# header.pl
'sshome' => 'hjem',
'ssstatus' => 'status',
'sstraffic graphs' => 'trafikk grafer',
'ssppp settings' => 'ppp instillinger',
'ssmodem' => 'modem',
'ssusb adsl firmware upload' => 'usb adsl firmware opplasting',
'ssssh' => 'ssh',
'sspasswords' => 'passord',
'ssweb proxy' => 'vev proxy',
'ssdhcp' => 'dhcp',
'ssport forwarding' => 'port videresending',
'ssexternal service access' => 'extern tjeneste tilgang',
'ssdmz pinholes' => 'dmz snarvei',
'ssdynamic dns' => 'dynamisk dns',
'ssids' => 'innbrudds alarm system',
'sscontrol' => 'kontroll',
'ssconnections' => 'tilkoblinger',
'ssother' => 'andre',
'ssfirewall' => 'brannmur',
'ssshutdown' => 'sl&aring; av',
'ssshell' => 'terminal',
'ssupdates' => 'oppdateringer',
'sshelp' => 'hjelp',
'sscredits' => 'Credits', #This is not a real english word, but we couldn't find a norwegian one... it's understandable.....
'ssip info' => 'IP Informasjon',
'powered by' => 'drevet av',
'alt home' => 'Hjem', # alt
'alt information' => 'Informasjon', # alt
'alt dialup' => 'Ring', # alt
'alt remote access' => 'Fjerntilgang', # alt
'alt services' => 'Tjenester', # alt
'alt ids' => 'Innbrudds alarm system', # alt
'alt vpn' => 'Virtuelt Privat Netverk', # alt
'alt logs' => 'Logger', #alt
'alt shutdown' => 'Sl&aring; av', # alt
'alt shell' => 'Terminal', # alt
'alt updates' => 'Oppdateringer', # alt

# changepw.cgi
'admin user password has been changed' => 'Admin bruker passordet har blitt forandret.',
'dial user password has been changed' => 'Dial bruker passordet har blitt forandret.',
'password cant be blank' => 'Passordet kan ikke v&aelig;re blankt.',
'passwords do not match' => 'Passordene er ikke like.',
'change passwords' => 'Bytt passord',
'administrator user password' => 'Admin bruker passord:',
'dial user password' => 'Dial bruker passord:',
'again' => 'Gjenta:',
'passwords must be at least 6 characters in length' => 'Passordet må være minst 6 tegn',
'password contains illegal characters' => 'Passordet inneholder ulovlige tegn.',

# credits.cgi
'credits' => 'Credits', #same thing here......
'version' => 'Versjon: ',
'sponsors' => 'Sponsorer',
'links' => 'Lenker',
'smoothwall homepage' => 'SmoothWall Hjemmeside',
'translation teams' => 'Oversettere',

# dhcp.cgi
'invalid start address' => 'Ugyldig start adresse.',
'invalid end address' => 'Ugyldig slutt adresse.',
'cannot specify secondary dns without specifying primary' => 'Kan ikke spesifisere sekund&aelig;r DNS uten &aring; ha spesifisert prim&aelig;r.',
'invalid default lease time' => 'Ugyldig standard l&aring;netid.',
'invalid max lease time' => 'Ugyldig lengste l&aring;netid.',
'dhcp server enabled' => 'DHCP tjener aktivert.  Starter p&aring; ny.',
'dhcp server disabled' => 'DHCP tjener deaktivert.  Stoppet.',
'dhcp configuration' => 'DHCP konfigurasjon',
'start address' => 'Start adresse:',
'end address' => 'Slutt adresse:',
'default lease time' => 'Standard l&aring;netid (minutter):',
'max lease time' => 'Lengste l&aring;netid (minutter):',
'domain name suffix' => 'Domenenavn suffiks:',

# proxy.cgi
'web proxy configuration' => 'Vev proxy konfigurasjon',
'web proxyc' => 'Vev proxy:',
'cache size' => 'Cache st&oslash;rrelse (MB):',
'invalid cache size' => 'Ugyldig cache st&oslash;rrelse.',
'remote proxy' => 'Fjernproxy:',
'invalid maximum object size' => 'Ugyldig st&oslash;rste objekt st&oslash;rrelse.',
'invalid minimum object size' => 'Ugyldig objekt st&oslash;rrelse.',
'invalid maximum outgoing size' => 'Ugyldig st&oslash;rste utg&aring;ende st&oslash;rrelse.',
'invalid maximum incoming size' => 'Ugyldig st&oslash;rste innkommende st&oslash;rrelse.',
'transparent' => 'Gjennomsiktig:',
'max size' => 'St&oslash;rste objekt st&oslash;rrelse (KB):',
'min size' => 'Minste objekt st&oslash;rrelse (KB):',
'max outgoing size' => 'St&oslash;rste utg&aring;ende st&oslash;rrelse (KB):',
'max incoming size' => 'St&oslash;rste innkommende st&oslash;rrelse (KB):',

# common to logs cgis
'january' => 'Januar',
'february' => 'Februar',
'march' => 'Mars',
'april' => 'April',
'may' => 'Mai',
'june' => 'Juni',
'july' => 'Juli',
'august' => 'August',
'september' => 'September',
'october' => 'Oktober',
'november' => 'November',
'december' => 'Desember',
'month' => 'M&aring;ned:',
'day' => 'Dag:',
'update' => 'Oppdater', # button
'export' => 'Eksporter', # button
'older' => 'Eldre',
'newer' => 'Nyere',
'settingsc' => 'Innstillinger:',

# logs.cgi/firewalllog.dat
'firewall log' => 'Brannmur logg',
'firewall log2' => 'Brannmur logg:',
'date' => 'Dato:',
'time' => 'Tid',
'action' => 'Hendelse',
'chain' => 'Kjede',
'iface' => 'Port',
'protocol' => 'Protokoll',
'source' => 'Kilde',
'src port' => 'Kilde Port',
'destination' => 'M&aring;l',
'dst port' => 'M&aring;l Port',
'unknown' => 'UKJENT',
'lookup' => 'Oppslag',

# logs.cgi/log.dat
'log viewer' => 'Logg viser',
'section' => 'Seksjon:',
'kernel' => 'Kjerne',
'loginlogout' => 'Logg inn/Logg ut',
'update transcript' => 'Oppdater omskriving',
'log' => 'Logg:',

# logs.cgi/proxylog.dat
'proxy log viewer' => 'Proxy logg viser',
'bad ignore filter' => 'Feil ignorerings filter:',
'caps all' => 'ALLE',
'ignore filterc' => 'Ignorerings filter:',
'enable ignore filterc' => 'Aktiver ignorerings filter:',
'source ip' => 'Kilde IP',
'website' => 'Vevside',

# logs.cgi/ids.dat
'ids log viewer' => 'IDS logg viser',
'datec' => 'Dato:',
'namec' => 'Navn:',
'priorityc' => 'Prioritet:',
'typec' => 'Type:',
'ipinfoc' => 'IP info:',
'referencesc' => 'Referanser:',
'none found' => 'ingen funnet',

# index.cgi
'main page' => 'Hovedside',
'dial' => 'Koble til', # button
'hangup' => 'Koble fra', # button
'current profile' => 'Gjeldende profil:',
'connected' => 'Tilkoblet',
'dialing' => 'Ringer...',
'modem idle' => 'Modem ledig',
'isdn idle' => 'ISDN ledig',
'profile has errors' => 'Profilen inneholder feil',
'modem settings have errors' => 'Modem innstillingene inneholder feil',
'user pages' => 'Brukersider',
'mstatus information' => 'Statusinformasjon',
'mnetwork traffic graphs' => 'Nettverkstrafikk grafer',
'administrator pages' => 'Administrator sider',
'mppp setup' => 'PPP oppsett',
'mmodem configuration' => 'Modem konfigurasjon',
'mchange passwords' => 'Endre passord',
'mremote access' => 'Fjerntilgang',
'mdhcp configuration' => 'DHCP konfigurasjon',
'mproxy configuration' => 'Proxy konfiguration',
'mport forwarding configuration' => 'Port videresending konfigurasjon',
'mshutdown control' => 'Sl&aring; av kontroll',
'mlog viewer' => 'Logg viser',
'mfirewall log viewer' => 'Brannmur loggviser',
'msecure shell' => 'Sikker terminal',
'modem dod waiting' => 'Modem Ring Ved Behov venter',
'isdn dod waiting' => 'ISDN Ring Ved Behov venter',
'pppoe idle' => 'PPPOE ledig',
'usbadsl idle' => 'USB ADSL ledig',
'pppoe dod waiting' => 'PPPOE Ring Ved Behov venter',
'dial on demand for this interface is not sypported' => 'Ring Ved Behov er ikke st&oslash;ttet for denne enheten',
'there are updates' => 'Det er tilgjengelige oppdateringer for systemet ditt. Vennligst g&aring; til "Oppdateringer" for mer informasjon',
'updates is old1' => 'Din oppdateringsfil er ',
'updates is old2' => 'dager gammel. Vi anbefaler at du oppdaterer den p&aring; "Oppdatering" siden.',

# pppsetup.cgi
'profile name not given' => 'Profil navn er ikke gitt.',
'telephone not set' => 'Telefon nummer er ikke gitt.',
'bad characters in the telephone number field' => 'Ugyldige tegn i telefon nummer feltet.',
'username not set' => 'Brukernavn er ikke gitt.',
'spaces not allowed in the username field' => 'Ikke tillatt med mellomrom i brukernavn feltet.',
'password not set' => 'Passord er ikke gitt.',
'spaces not allowed in the password field' => 'Ikke tillatt med mellomrom i passord feltet.',
'idle timeout not set' => 'Lengste ventetid ikke gitt.',
'only digits allowed in the idle timeout' => 'Det er bare lov med tall i lengste ventetid feltet.',
'bad characters in script field' => 'Ugyldige tegn i script feltet',
'max retries not set' => 'Maks. antall gjentakelser er ikke gitt.',
'only digits allowed in max retries field' => 'Det er bare lov med tall i maks. antall gjentakelser feltet.',
'profile saved' => 'Profilen er lagret: ',
'select' => 'Velg', # button
'profile made current' => 'Profilen er n&aring; gjeldende profil: ',
'the selected profile is empty' => 'Den valgte profilen er tom.',
'delete' => 'Slett', # button
'profile deleted' => 'Profilen er slettet: ',
'empty' => 'Tom',
'unnamed' => 'Ikke navngitt',
'ppp setup' => 'PPP oppsett',
'profiles' => 'Profiler:',
'profile name' => 'Profil navn:',
'telephony' => 'Telefoni:',
'interface' => 'Porter:',
'modem on com1' => 'Modem p&aring; COM1',
'modem on com2' => 'Modem p&aring; COM2',
'modem on com3' => 'Modem p&aring; COM3',
'modem on com4' => 'Modem p&aring; COM4',
'isdn tty' => 'ISDN p&aring; TTY',
'isdn1' => 'Enkel ISDN',
'isdn2' => 'Dobbel ISDN',
'computer to modem rate' => 'Datamaskin til modem rate:',
'number' => 'Nummer:',
'modem speaker on' => 'Modemh&oslash;ytaler p&aring;:',
'dialing mode' => 'Ringem&aring;te:',
'tone' => 'Tone',
'pulse' => 'Puls',
'maximum retries' => 'Maks antall gjentakelser:',
'idle timeout' => 'Lengste ventetid (minutter; 0 for &aring; deaktivere):',
'persistent connection' => 'Vedvarende forbindelse:',
'authentication' => 'Autentisering:',
'method' => 'Metode:',
'pap or chap' => 'PAP eller CHAP',
'standard login script' => 'Standard p&aring;logging script',
'demon login script' => 'Demon p&aring;logging script',
'other login script' => 'Andre p&aring;logging script',
'script name' => 'Script navn:',
'type' => 'Type:',
'manual' => 'Manuell',
'automatic' => 'Automatisk',
'dod' => 'Ring Ved Behov:',
'dod for dns' => 'Ring Ved Behov for DNS:',
'connect on smoothwall restart' => 'Koble til n&aring;r SmoothWall starter p&aring; ny:',
'pppoe settings' => 'Tilleggsinstillinger for  PPPoE:',
'usb adsl settings' => 'Tilleggsinstillinger USB ADSL:',
'service name' => 'Tjeneste navn:',
'concentrator name' => 'Samlepunktnavn:',
'vpi number' => 'VPI Nummer:',
'vci number' => 'VCI Nummer:',
'firmwarec' => 'Firmware:',
'firmware present' => 'Tilstede',
'firmware not present' => '<B>IKKE</B> tilstede',
'upload usb adsl firmware' => 'Last opp USB ADSL firmware',
'no usb adsl firmware' => 'Ingen USB ADSL firmware. Vennligst last opp',
'unable to alter profiles while red is active' => 'Kan ikke fornadre profil n&aring;r R&Oslash;D er aktiv',

# remote.cgi
'ssh is enabled' => 'SSH er aktivert.  Starter p&aring; ny.',
'ssh is disabled' => 'SSH er daktivert.  Stopper.',
'remote access' => 'Fjerntilgeng',
'remote access2' => 'Fjerntilgang:',

# shutdown.cgi
'shutting down smoothwall' => 'Sl&aring;r av SmoothWall',
'shutdown control' => 'Sl&aring; av kontroll',
'shutdown' => 'SLA AV', # button
'shutdown2' => 'Sl&aring; av:',
'shutting down' => 'Sl&aring;r av',
'smoothwall has now shutdown' => 'SmoothWall har n&aring; blitt sl&aring;tt av.',
'smoothwall has now shutdown' => 'SmoothWall er n&aring; sl&aring;tt av.',
'rebooting smoothwall' => 'Starter SmoothWall p&aring; ny',
'reboot' => 'START PA NY', # button
'rebooting' => 'Starter p&aring; ny',
'smoothwall has now rebooted' => 'SmoothWall har n&aring; startet p&aring; ny.',

# status.cgi
'web server' => 'Vev tjener',
'cron server' => 'CRON tjener',
'dns proxy server' => 'DNS proxy tjener',
'logging server' => 'Logg tjener',
'kernel logging server' => 'Kjerne logg tjener',
'secure shell server' => 'Sikker terminal tjener',
'vpn' => 'VPN',
'web proxy' => 'Vev proxy',
'intrusion detection system' => 'Innbrudds alarm',
'status information' => 'Status informasjon',
'services' => 'Tjenester:',
'memory' => 'Minne:',
'uptime and users' => 'Oppetid og brukere:',
'interfaces' => 'Porter:',
'disk usage' => 'Disk bruk:',
'loaded modules' => 'Lastede moduler:',
'kernel version' => 'Kjerne versjon:',
'stopped' => 'STOPPET',
'running' => 'KJ&Oslash;RER',
'swapped' => 'BYTTET',

# portfw.cgi and dmzhole.cgi and xtaccess.cgi
'source port numbers' => 'Kildeport m&aring; v&aelig;re et nummer.',
'source is bad' => 'Ikke gyldig IP adresse eller nettverks adresse.',
'destination ip bad' => 'Ugyldig m&aring;l IP',
'destination port numbers' => 'M&aring;lporten m&aring; v&aelig;re et nummer.',
'unable to open file' => 'Kan ikke &aring;pne filen',
'source port in use' => 'Kildeporten er i bruk:',
'forwarding rule added' => 'Videresendings regel er lagt til; starter videresenderen p&aring; ny',
'forwarding rule removed' => 'Videresendings regel er fjernet; starter videresenderen p&aring; ny',
'external access rule added' => 'Fjerntilgang regel er lagt til; starter tilgangskontroll p&aring; ny',
'external access rule removed' =>' Fjerntilgang regel er fjernet; starter tilgangskontroll p&aring; ny',
'dmz pinhole rule added' => 'DMZ snarvei regel er lagt til; starter DMZ snarvei p&aring; ny',
'dmz pinhole rule removed' => 'DMZ snarvei regel er fjernet; starter DMZ snarvei p&aring; ny',
'port forwarding configuration' => 'Portvideresending konfigurasjon',
'dmz pinhole configuration' => 'DMZ snarvei konfigurasjon',
'external access configuration' => 'Fjerntilgang konfigurasjon',
'add a new rule' => 'Legg til ny regel:',
'sourcec' => 'Kilde IP, eller network (blank for "ALLE"):',
'source ipc' => 'Kilde IP:',
'source portc' => 'Kilde port:',
'destination ipc' => 'M&aring;l IP:',
'destination portc' => 'M&aring;l port:',
'current rules' => 'Gjeldende regler:',
'source ip' => 'Kilde IP',
'source port' => 'Kilde port',
'destination ip' => 'M&aring;l IP',
'destination port' => 'M&aring;l port',
'add' => 'Legg til', # button
'remove' => 'Fjern', # button
'edit' => 'Rediger', # button
'enabledtitle' => 'Aktiver',
'nothing selected' => 'Ingenting er valgt',
'you can only select one item to edit' => 'Du kan bare velge ett element &aring; redigere',
'mark' => 'Merk',
'all' => 'ALLE',

# ddns.cgi
'dynamic dns' => 'Dynamisk DNS',
'add a host' => 'Legg til en vert:',
'servicec' => 'Tjeneste:',
'behind a proxy' => 'Bak en proxy:',
'enable wildcards' => 'Aktiver jokere:',
'hostnamec' => 'Vertsnavn:',
'domainc' => 'Domene:',
'current hosts' => 'Gjeldende vert:',
'service' => 'Tjeneste',
'hostname' => 'Vertsnavn',
'domain' => 'Domene',
'proxy' => 'Proxy',
'wildcards' => 'Jokere',
'hostname not set' => 'Vertsnavn er ikke gitt.',
'domain not set' => 'Domenenavn er ikke gitt.',
'invalid hostname' => 'Ugyldig vertsnavn.',
'invalid domain name' => 'Ugyldig domenenavn.',
'hostname and domain already in use' => 'Vertsnavn og domene er allerede i bruk.',
'ddns hostname added' => 'Dynamisk DNS vertsnavn er lagt til',
'ddns hostname removed' => 'Dynamisk DNS vertsnavn er fjernet',
'force update' => 'Tving oppdatering',

# ipinfo.cgi
'ip info' => 'IP Informasjon',
'lookup failed' => 'Reversert oppslag feilet',

# shell.cgi
'secure shellc' => 'Sikker terminal:',

# modem.cgi
'restore defaults' => 'Gjenopprett standardverdier', # button
'timeout must be a number' => 'Ventetid m&aring; v&aelig;re et tall.',
'modem configuration' => 'Modem konfigurasjon',
'modem configurationc' => 'Modem konfigurasjon:',
'init string' => 'Init:',
'hangup string' => 'Legg p&aring;:',
'speaker on' => 'H&oslash;ytaler p&aring;:',
'speaker off' => 'H&oslash;ytaler av:',
'tone dial' => 'Tone ringing:',
'pulse dial' => 'Puls ringing:',
'connect timeout' => 'Lengste ventetid:',
'send cr' => 'Internett leverand&oslash;ren krever linjeskift p&aring; slutten av linjen:',

# vpnmain.cgi
'restart' => 'Start p&aring; ny',
'stop' => 'Stopp',
'vpn configuration main' => 'VPN konfigurasjon - Hoved',
'main' => 'Hoved',
'connections' => 'Tilkoblinger',
'global settingsc' => 'Globale instillinger:',
'local vpn ip' => 'Lokal VPN IP:',
'if blank the currently configured ethernet red address will be used' => 'Hvis blank, vil gjeldende R&Oslash;D ethernet adresse bli brukt',
'manual control and status' => 'Manuell kontroll og status:',
'connection name' => 'Navn',
'connection status' => 'Status',
'capsclosed' => 'STENGT',
'capsdisabled' => 'DEAKTIVERT',
'capsopen' => '&Aring;PEN',

# vpn.cgi/vpnconfig.dat
'name must only contain characters' => 'Navn kan bare inneholde bokstaver.',
'left ip is invalid' => 'Venstre IP er ugyldig.',
'left next hop ip is invalid' => 'Venstre maskin har ugyldig IP.',
'left subnet is invalid' => 'Venstre subnett er ugyldig.',
'right ip is invalid' => 'H&oslash;yre IP er ugyldig.',
'right next hop ip is invalid' => 'H&oslash;yre maskin har ugyldig IP.',
'right subnet is invalid' => 'H&oslash;yre subnett er ugyldig.',
'vpn configuration connections' => 'VPN konfigurasjon - Tilkoblinger',
'add a new connection' => 'Legg til ny tilkobing:',
'namec' => 'Navn:',
'leftc' => 'Venstre:',
'left next hopc' => 'Venstre maskin:',
'left subnetc' => 'Venstre subnett:',
'rightc' => 'H&oslash;yre:',
'right next hopc' => 'H&oslash;yre maskin:',
'right subnetc' => 'H&oslash;yre subnett:',
'secretc' => 'Hemmelig:',
'current connections' => 'Gjeldende tilkobling:',
'markc' => 'Merk:',
'import and export' => 'Importer og Eksporter:',
'import' => 'Importer', # button

# graphs.cgi
'network traffic graphs' => 'Nettverkstrafikk grafer',
'network traffic graphsc' => 'Nettverkstrafikk grafer:',
'no graphs available' => 'Det er ingen grafer tilgjengelig.',
'no information available' => 'Det er ingen informasjon tilgjengelig.',

# usbadsl.cgi
'usb adsl setup' => 'USB ADSL oppsett',
'usb adsl help' => 'For &aring; ta i bruk USB modemet, m&aring; du laste opp firmware til SmoothWall boksen. Vennligst last den tar-pakken fra Alcatel for s&aring; &aring; laste opp filen <b>mgmt.o</b> ved hjelp av skjemaet under.',
'upload' => 'Last opp', # button
'upload successfull' => 'Opplastingen var vellykket.',
'could not create file' => 'Kunne ikke lage filen.',
'mgmt upload' => 'USB ADSL driver er lastet opp:',
'upload filec' => 'Opplastet fil:',

# updates.cgi
'updates' => 'Oppdateringer',
'could not open available updates file' => 'Kan ikke &aring;pne tilgjengelige oppdateringer.',
'could not download the available updates list' => 'Kan ikke laste ned listen over tilgjengelige oppdateringer.',
'could not create directory' => 'Kan ikke lage katalogen.',
'could not open updatefile for writing' => 'Kan ikke &aring;pne oppdateringsfilen for skriving.',
'this is not an authorised update' => 'Dette er ikk en autorisert oppdatering, eller s&aring; er patchlisten din utdatert.',
'this is not a valid archive' => 'Dette er ikke et gyldig arkiv.',
'could not open update information file' => 'Kan ikke &aring;pne oppdaterings informasjon filen. Oppdateringsfilen er &oslash;delagt.',
'this update is already installed' => 'Denne oppdateringen er allerede installert.',
'package failed to install' => 'Pakken ble ikke installert.',
'update installed but' => 'Oppdateringen er installert, men oppdateringsdatabasen ble ikke oppdatert',
'refresh update list' => 'Oppdater oppdateringslisten', # button
'installed updates' => 'Installer oppdateringer:',
'id' => 'ID',
'title' => 'Tittel',
'description' => 'Beskrivelse',
'released' => 'Utgitt',
'installed' => 'Installert',
'could not open installed updates file' => 'Kan ikke &aring;pne installert oppdaterings fil',
'available updates' => 'Tilgjengelige oppdateringer:',
'there are updates available' => 'Det er tilgjengelige oppdateringer for systemet ditt. Det er anbefalt p&aring; det sterkeste at du installerer dem s&aring; snart som mulig.',
'info' => 'Informasjon',
'all updates installed' => 'Alle oppdateringene er installert',
'install new update' => 'Installer ny oppdatering:',
'to install an update' => 'For &aring; installere en oppdatering, vennligst last opp .tar.gz filen under:',
'upload update file' => 'Last opp oppdateringsfil:',
'could not download latest patch list' => 'Kan ikke hente ned den siste patch listen (ikke tilkoblet).',
'could not connect to smoothwall org' => 'Kan ikke kobel til smoothwall.org',
'successfully refreshed updates list' => 'Oppdaterings listen er oppdatert.',
'the following update was successfully installedc' => 'De f&oslash;lgende oppdateringene ble installert:',

# ids.cgi
'snort is enabled' => 'Snort er aktivert',
'snort is disabled' => 'Snort er deaktivert',
'intrusion detection system2' => 'Innbrudds alarm:',

);


