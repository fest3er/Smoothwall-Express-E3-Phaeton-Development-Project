# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# (c) German Translation Team:
# Lukas Frey
# Dirk Bergner
# Ralf Stahlmann
# Sven Hummel
# Boris Friedrichs
# Sebastian Ahrens
# Martin Schluter
# Andrej Huschka
# Ingo Bradtka
# Georg Lorenz
# Jens Kruse
# Marcel Gsteiger
# Dirk Siedle
# Arne Matzke
# Dirk Schwilski
# Joerg Dulz
# Alexis Lawrence
# Robert Krauss
# Erkan Sapmaz
# Detlef Braas
# Egino
# Thomas Sempach
# Heiko Rittelmeier
# Florian Moesch
# Gunther Bohm

%basetr = (

%basetr,

# common
'invalid input' => 'Ung&uuml;ltige eingabe',
'save' => 'Sichern', # button
'refresh' => 'Aktualisieren', # button
'restore' => 'Wiederherstellen', # button
'error messages' => 'Fehlermeldungen:',
'back' => 'Zur&uuml;ck',
'help' => 'Hilfe',
'primary dns' => 'Prim&auml;rer DNS:',
'secondary dns' => 'Sekund&auml;rer DNS:',
'invalid primary dns' => 'Ung&uuml;ltiger Prim&auml;rer DNS.',
'invalid secondary dns' => 'Ung&uuml;ltiger Sekund&auml;rer DNS.',
'dhcp server' => 'DHCP Server',
'username' => 'Benutzername:',
'password' => 'Pa&szlig;wort:',
'enabled' => 'Aktiviert:',
'this field may be blank' => 'Dieses Feld kann leer bleiben.',
'these fields may be blank' => 'Diese Felder k&ouml;nnen leer bleiben.',

# header.pl
'sshome' => 'Startseite',
'ssstatus' => 'Status',
'sstraffic graphs' => 'Transfer Grafiken',
'ssppp settings' => 'PPP Einstellungen',
'ssmodem' => 'Modem',
'ssusb adsl firmware upload' => 'USB ADSL Firmware Upload',
'ssssh' => 'SSH',
'sspasswords' => 'Pa&szlig;worte',
'ssweb proxy' => 'Web Proxy',
'ssdhcp' => 'DHCP',
'ssport forwarding' => 'Port Weiterleitung',
'ssexternal service access' => 'Fernwartungszugang',
'ssdmz pinholes' => 'DMZ-Weiterleitungen',
'ssdynamic dns' => 'Dynamischer DNS',
'ssids' => 'Einbruchserkennung(IDS)',
'sscontrol' => 'Steuerung',
'ssconnections' => 'Verbindungen',
'ssother' => 'Andere',
'ssfirewall' => 'Firewall',
'ssshutdown' => 'Herunterfahren',
'ssshell' => 'Shell',
'ssupdates' => 'Updates',
'sshelp' => 'Hilfe',
'sscredits' => 'Danksagungen',
'ssip info' => 'IP Informationen',
'powered by' => 'Betrieben von',
'alt home' => 'Hauptseite', # alt
'alt information' => 'Information', # alt
'alt dialup' => 'Einwahl', # alt
'alt remote access' => 'Fernzugriff', # alt
'alt services' => 'Dienste', # alt
'alt ids' => 'Einbruchserkennung(IDS)', # alt
'alt vpn' => 'Virtuelle Private Netzwerke', # alt
'alt logs' => 'Protokolle', # alt
'alt shutdown' => 'Herunterfahren', # alt
'alt shell' => 'Shell', # alt
'alt updates' => 'Updates', # alt

# changepw.cgi
'admin user password has been changed' => 'Administrator Passwort ge&auml;ndert.',
'dial user password has been changed' => 'Benutzer Passwort ge&auml;ndert.',
'password cant be blank' => 'Passworteingabe erforderlich.',
'passwords do not match' => 'Passworte stimmen nicht &uuml;berein.',
'change passwords' => 'Passworte &auml;ndern',
'administrator user password' => 'Administrator Passwort:',
'dial user password' => 'Benutzer Passwort:',
'again' => 'Wiederholen:',
'passwords must be at least 6 characters in length' => 'Passwörter müssen mindestens 6 Zeichen lang sein',
'password contains illegal characters' => 'Das Passwort enth&auml;lt ung&uuml;ltige Zeichen',


# credits.cgi
'credits' => 'Credits',
'version' => 'Version: ',
'sponsors' => 'Sponsoren',
'links' => 'Links',
'smoothwall homepage' => 'SmoothWall Homepage',
'translation teams' => 'Übersetzer-Teams',

# dhcp.cgi
'invalid start address' => 'Ung&uuml;ltige Start-Adresse.',
'invalid end address' => 'Ung&uuml;ltige End-Adresse.',
'cannot specify secondary dns without specifying primary' => 'Ohne das prim&auml;re DNS definiert zu haben, darf kein sekund&auml;res definiert werden.',
'invalid default lease time' => 'Vorgegebener Lease-Zeitraum ung&uuml;ltig.',
'invalid max lease time' => 'Maximaler Lease-Zeitraum ung&uuml;ltig.',
'dhcp server enabled' => 'DHCP Server aktiviert.  Neustart.',
'dhcp server disabled' => 'DHCP Server deaktiviert.  Angehalten.',
'dhcp configuration' => 'DHCP Konfiguration',
'start address' => 'Start-Adresse:',
'end address' => 'End-Adresse:',
'default lease time' => 'Vorgegebener Lease-Zeitraum (Min):',
'max lease time' => 'Maximaler Lease-Zeitraum (Min):',
'domain name suffix' => 'Suffix des Dom&auml;nennamens:',

# proxy.cgi
'web proxy configuration' => 'Web Proxy Konfiguration',
'web proxyc' => 'Web Proxy:',
'cache size' => 'Cache Gr&ouml;sse (MB):',
'invalid cache size' => 'Ung&uuml;ltige Cache Gr&ouml;sse.',
'remote proxy' => 'Externer Proxy:',
'invalid maximum object size' => 'Maximale Objekt-Gr&oumlsse ung&uuml;ltig.',
'invalid minimum object size' => 'Minimale Objekt-Gr&oumlsse ung&uuml;ltig.',
'invalid maximum outgoing size' => 'Maximale ausgehende Gr&oumlsse ung&uuml;ltig.',
'invalid maximum incoming size' => 'Maximale ankommende Gr&oumlsse ung&uuml;ltig.',
'transparent' => 'Transparent:',
'max size' => 'Maximale Objekt-Gr&ouml;sse (KB):',
'min size' => 'Minimale Objekt-Gr&ouml;sse (KB):',
'max outgoing size' => 'Maximale ausgehende Gr&ouml;sse (KB):',
'max incoming size' => 'Maximale ankommende Gr&ouml;sse (KB):',

# common to logs cgis
'january' => 'Januar',
'february' => 'Februar',
'march' => 'M&auml;rz',
'april' => 'April',
'may' => 'Mai',
'june' => 'Juni',
'july' => 'Juli',
'august' => 'August',
'september' => 'September',
'october' => 'Oktober',
'november' => 'November',
'december' => 'Dezember',
'month' => 'Monat:',
'day' => 'Tag:',
'update' => 'Update', # button
'export' => 'Export', # button
'older' => '&Auml;lter',
'newer' => 'Neuer',
'settingsc' => 'Einstellungen:',

# logs.cgi/firewalllog.dat
'firewall log' => 'Firewall-Protokoll',
'firewall log2' => 'Firewall-Protokoll:',
'date' => 'Datum:',
'time' => 'Uhrzeit',
'action' => 'Aktion',
'chain' => 'IP-Chain',
'iface' => 'Schnittstelle',
'protocol' => 'Protokoll',
'source' => 'Quelle',
'src port' => 'Quell-Port',
'destination' => 'Ziel',
'dst port' => 'Ziel-Port',
'unknown' => 'UNBEKANNT',
'lookup' => 'Nachschlagen',

# logs.cgi/log.dat
'log viewer' => 'Protokollansicht',
'section' => 'Abschnitt:',
'kernel' => 'Kernel',
'loginlogout' => 'Login/Logout',
'update transcript' => 'Update Skript',
'log' => 'Protokoll:',

# logs.cgi/proxylog.dat
'proxy log viewer' => 'Proxy Protokollansicht',
'bad ignore filter' => 'Filter "Ignorieren" fehlerhaft:',
'caps all' => 'ALLE',
'ignore filterc' => 'Filter "Ignorieren":',
'enable ignore filterc' => 'Filter "Ignorieren" aktivieren:',
'source ip' => 'Quell-IP',
'website' => 'Webseite',

# logs.cgi/ids.dat
'ids log viewer' => 'IDS Protokollansicht',
'datec' => 'Datum:',
'namec' => 'Name:',
'priorityc' => 'Priorit&auml;t:',
'typec' => 'Typ:',
'ipinfoc' => 'IP Info:',
'referencesc' => 'Referenzen:',
'none found' => 'keine gefunden',

# index.cgi
'main page' => 'Startseite',
'dial' => 'Verbinden', # button
'hangup' => 'Trennen', # button
'current profile' => 'Aktuelles Profil:',
'connected' => 'Verbunden',
'dialing' => 'W&auml;hle...',
'modem idle' => 'Modem nicht in Benutzung',
'isdn idle' => 'ISDN nicht in Benutzung',
'profile has errors' => 'Profil hat Fehler',
'modem settings have errors' => 'Modem Einstellungen haben Fehler',
'user pages' => 'Seiten f&uuml;r Benutzer',
'mstatus information' => 'Statusinformationen',
'mnetwork traffic graphs' => 'Diagramme zur Netzwerkaktivit&auml;t',
'administrator pages' => 'Seiten f&uuml;r den Administrator',
'mppp setup' => 'PPP-Einstellungen',
'mmodem configuration' => 'Modem-Konfiguration',
'mchange passwords' => 'Passw&ouml;rter &auml;ndern',
'mremote access' => 'Fernwartung',
'mdhcp configuration' => 'DHCP-Konfiguration',
'mproxy configuration' => 'Webproxy-Konfiguration',
'mport forwarding configuration' => 'Port-Weiterleitung',
'mshutdown control' => 'Herunterfahren',
'mlog viewer' => 'Protokolle',
'mfirewall log viewer' => 'Firewall Protokollansicht',
'msecure shell' => 'Secure&nbsp;shell',
'modem dod waiting' => 'Modem wartet im Dial-on-Demand-Modus',
'isdn dod waiting' => 'ISDN wartet im Dial-on-Demand-Modus',
'pppoe idle' => 'PPPOE nicht in Benutzung',
'usbadsl idle' => 'USB ADSL nicht in Benutzung',
'pppoe dod waiting' => 'PPPOE wartet im Dial-on-Demand-Modus',
'there are updates' => 'Neue System-Updates verf&uuml;gbar. Weitere Informationen finden Sie in der Sektion "Updates".',
'updates is old1' => 'Ihre Update-Datei ist ',
'updates is old2' => 'Tage alt. Wir empfehlen sie auf der "Updates"-Seite auf den neusten Stand zu bringen.',

# pppsetup.cgi
'profile name not given' => 'Kein Profilname angegeben.',
'telephone not set' => 'Keine Telefonnummer angegeben.',
'bad characters in the telephone number field' => 'Ung&uuml;ltige Zeichen im Feld \'Telefonnummer\'.',
'username not set' => 'Kein Benutzername angegeben.',
'spaces not allowed in the username field' => 'Im Feld \'Benutzername\' sind Leerzeichen nicht erlaubt.',
'password not set' => 'Kein Passwort angegeben.',
'spaces not allowed in the password field' => 'Im Feld \'Passwort\' sind Leerzeichen nicht erlaubt.',
'idle timeout not set' => 'Keine Leerlauftrennung angegeben.',
'only digits allowed in the idle timeout' => 'Bei der Leerlauftrennung sind nur Ziffern erlaubt.',
'bad characters in script field' => 'Ung&uuml;ltige Zeichen im Skript-Feld',
'max retries not set' => '\'Maximale Anzahl Wahlwiederholungen\' nicht angegeben.',
'only digits allowed in max retries field' => 'Im Feld \'Maximale Anzahl Wahlwiederholungen\' sind nur Ziffern erlaubt.',
'profile saved' => 'Profil abgespeichert: ',
'select' => 'AUSWAHLEN', # button
'profile made current' => 'Profil aktiviert: ',
'the selected profile is empty' => 'Das ausgew&auml;hlte Profil ist leer.',
'delete' => 'LOSCHEN', # button
'profile deleted' => 'Profil gel&ouml;scht: ',
'empty' => 'Leer',
'unnamed' => 'Unbenannt',
'ppp setup' => 'PPP-Einstellungen',
'profiles' => 'Profile:',
'profile name' => 'Profilname:',
'telephony' => 'Telephonie:',
'interface' => 'Schnittstelle:',
'modem on com1' => 'Modem an COM1',
'modem on com2' => 'Modem an COM2',
'modem on com3' => 'Modem an COM3',
'modem on com4' => 'Modem an COM4',
'isdn tty' => 'ISDN an TTY',
'isdn1' => 'Einkanal-ISDN',
'isdn2' => 'Zweikanal-ISDN',
'computer to modem rate' => 'Schnittstellengeschwindigkeit zum Modem:',
'number' => 'Nummer:',
'modem speaker on' => 'Modemlautsprecher an:',
'dialing mode' => 'W&auml;hlmodus:',
'tone' => 'Tonwahl',
'pulse' => 'Impulswahl',
'maximum retries' => 'Maximale Anzahl Wahlwiederholungen:',
'idle timeout' => 'Leerlauftrennung (Minuten; 0 zum Abschalten):',
'persistent connection' => 'Standleitung:',
'authentication' => 'Authentifizierung:',
'method' => 'Methode:',
'pap or chap' => 'PAP oder CHAP',
'standard login script' => 'Standardm&auml;&szlig;iges Login-Skript',
'demon login script' => 'Demon Login-Skript',
'other login script' => 'Anderes Login-Skript',
'script name' => 'Name des Skripts:',
'type' => 'Art:',
'manual' => 'Manuell',
'automatic' => 'Automatisch',
'dod' => 'Verbindung bei Bedarf (Dial-on-Demand):',
'dod for dns' => 'Dial-on-Demand f&uuml;r DNS:',
'connect on smoothwall restart' => 'Verbinden bei Neustart:',
'pppoe settings' => 'Zus&auml;tzliche PPPoE-Einstellungen:',
'usb adsl settings' => 'Zus&auml;tzliche USB ADSL-Einstellungen:',
'service name' => 'Name des Dienstes:',
'concentrator name' => 'Name des Konzentrators:',
'vpi number' => 'VPI-Nummer:',
'vci number' => 'VCI-Nummer:',
'firmwarec' => 'Firmware:',
'firmware present' => 'Vorhanden',
'firmware not present' => '<B>Nicht</B> vorhanden',
'upload usb adsl firmware' => 'USB ADSL-Firmware hochladen',
'no usb adsl firmware' => 'Keine USB ADSL-Firmware. Bitte hochladen.',
'dial on demand for this interface is not supported' => 'Verbindung bei Bedarf (Dial-on-Demand) ist f&uuml;r diese Schnittstelle nicht unterst&uuml;tzt.',
'unable to alter profiles while red is active' => 'Profil&auml;nderung nicht m&ouml;glich, solange ROT aktiv ist.',


# remote.cgi
'ssh is enabled' => 'SSH ist aktiviert.  Starte neu.',
'ssh is disabled' => 'SSH ist deaktiviert.  Halte an.',
'remote access' => 'Fernzugang',
'remote access2' => 'Fernzugang:',

# shutdown.cgi
'shutting down smoothwall' => 'Beende SmoothWall',
'shutdown control' => 'Kontrolle zum Herunterfahren',
'shutdown' => 'Beenden', # button
'shutdown2' => 'Beenden:',
'shutting down' => 'fahre herunter',
'smoothwall has now shutdown' => 'SmoothWall ist jetzt heruntergefahren.',
'rebooting smoothwall' => 'SmoothWall wird neu gestartet.',
'reboot' => 'Neustart', # button
'rebooting' => 'Starte neu',
'smoothwall has now rebooted' => 'SmoothWall wurde neu gestartet.',

# status.cgi
'web server' => 'Webserver',
'cron server' => 'CRON server',
'dns proxy server' => 'DNS-Proxy-Server',
'logging server' => 'Logging-Server',
'kernel logging server' => 'Kernel-Logging-Sserver',
'secure shell server' => 'Secure-Shell-Server',
'vpn' => 'VPN',
'web proxy' => 'Web proxy',
'intrusion detection system' => 'St&ouml;rungserkennungssystem',
'status information' => 'Statusinformation',
'services' => 'Dienste:',
'memory' => 'Speicher:',
'uptime and users' => 'Laufzeit und Benutzer:',
'interfaces' => 'Interfaces:',
'disk usage' => 'Festplattenbenutzung:',
'loaded modules' => 'Geladene Module:',
'kernel version' => 'Kernel Version:',
'stopped' => 'GESTOPPED',
'running' => 'L&Auml;UFT',
'swapped' => 'AUSGELAGERT',

# portfw.cgi and dmzhole.cgi and xtaccess.cgi
'source port numbers' => 'Der Quellport muss eine Zahl sein.',
'source is bad' => 'Ung&uuml;ltige IP-Adresse oder Netzwerkadresse.',
'destination ip bad' => 'Ung&uuml;ltige Ziel-IP',
'destination port numbers' => 'Der Zielport muss eine Zahl sein.',
'unable to open file' => 'Die Datei kann nicht ge&ouml;ffnet werden',
'source port in use' => 'Der Quellport wird schon benutzt:',
'forwarding rule added' => 'Weiterleitungsregel hinzugef&uuml;gt; Die Weiterleitung wird neu gestartet',
'forwarding rule removed' => 'Weiterleitungsregel gel&ouml;scht; Die Weiterleitung wird neu gestartet',
'external access rule added' => 'Zugriffsregel f&uuml;r externe Zugriffe hinzugef&uuml;gt; Die Zugriffskontrolle wird neu gestartet',
'external access rule removed' =>' Zugriffsregel f&uuml;r externe Zugriffe gel&ouml;scht; Starte die Zugriffskontrolle neu',
'dmz pinhole rule added' => 'DMZ-Weiterleitungs-Regel hinzugefügt; Starte DMZ-Weiterleitung neu',
'dmz pinhole rule removed' => 'DMZ-Weiterleitungs-Regel gel&ouml;scht; Starte DMZ-Weiterleitung neu',
'port forwarding configuration' => 'Portweiterleitungskonfiguration',
'dmz pinhole configuration' => 'Konfiguration DMZ-Weiterleitung',
'external access configuration' => 'Fernzugriffskonfiguration',
'add a new rule' => 'Neue Regeln hinzuf&uuml;gen:',
'sourcec' => 'Quell-IP, oder Netzwerk (leer lassen f&uuml;r "ALLE"):',
'source ipc' => 'Quell-IP:',
'source portc' => 'Quellport:',
'destination ipc' => 'Ziel-IP:',
'destination portc' => 'Zielport:',
'current rules' => 'Aktuelle Regeln:',
'source ip' => 'Quell-IP',
'source port' => 'Quellport',
'destination ip' => 'Ziel-IP',
'destination port' => 'Zielport',
'add' => 'HINZUFULGEN', # button
'remove' => 'LOSCHEN', # button
'edit' => 'ANDERN', # button
'enabledtitle' => 'Aktiviert',
'nothing selected' => 'Nichts ausgew&auml;hlt',
'you can only select one item to edit' => 'Sie k&ouml;nen nur ein Teil ausw&auml;hlen, um es zu bearbeiten.',
'mark' => 'Markieren',
'all' => 'ALLE',

# ddns.cgi
'dynamic dns' => 'Dynamisches DNS',
'add a host' => 'Rechner hinzuf&uuml;gen:',
'servicec' => 'Dienst:',
'behind a proxy' => 'Hinter einem Proxy:',
'enable wildcards' => 'Platzhalter erlauben:',
'hostnamec' => 'Rechnername:',
'domainc' => 'Domain:',
'current hosts' => 'Aktuelle Rechner:',
'service' => 'Dienst',
'hostname' => 'Rechnername',
'domain' => 'Domain',
'proxy' => 'Proxy',
'wildcards' => 'Platzhalter',
'hostname not set' => 'Rechnername nicht angegeben.',
'domain not set' => 'Domain nicht angegeben.',
'invalid hostname' => 'Ung&uuml;ltiger Rechnername.',
'invalid domain name' => 'Ung&uuml;ltiger Domainname.',
'hostname and domain already in use' => 'Rechnername und Domain sind bereits in Benutzung.',
'ddns hostname added' => 'Dynamischen DNS-Rechnernamen hinzugef&uuml;gt',
'ddns hostname removed' => 'Dynamischen DNS-Rechnernamen entfernt',
'force update' => 'Update erzwingen',

# ipinfo.cgi
'ip info' => 'IP Information',
'lookup failed' => 'Reverse Namensaufl&ouml;sung gescheitert',

# shell.cgi
'secure shellc' => 'Secure shell (SSH):',

# modem.cgi
'restore defaults' => 'Zuruecksetzen', # button
'timeout must be a number' => 'Timeout mu&szlig; eine Zahl sein.',
'modem configuration' => 'Modemkonfiguration',
'modem configurationc' => 'Modemkonfiguration:',
'init string' => 'Initialisierungsstring:',
'hangup string' => 'Auflegen:',
'speaker on' => 'Modemlautsprecher an:',
'speaker off' => 'Modemlautsprecher aus:',
'tone dial' => 'Tonwahl:',
'pulse dial' => 'Impulswahl:',
'connect timeout' => 'Leerlauftrennung:',
'send cr' => 'ISP ben&ouml;tigt Wagenr&uuml;cklauf (CR):',

# vpnmain.cgi
'restart' => 'Neu starten',
'stop' => 'Stop',
'vpn configuration main' => 'VPN-Konfiguration - Hauptbereich',
'main' => 'Hauptbereich',
'connections' => 'Verbindungen',
'global settingsc' => 'Globale Einstellungen:',
'local vpn ip' => 'Lokale VPN IP:',
'if blank the currently configured ethernet red address will be used' => 'Falls leer, so wird die aktuelle IP-Adresse der roten Schnittstelle verwendet.',
'manual control and status' => 'Manuelle Steuerung und Status:',
'connection name' => 'Verbindungsname',
'connection status' => 'Verbindungsstatus',
'capsclosed' => 'GESCHLOSSEN',
'capsdisabled' => 'DEAKTIVIERT',
'capsopen' => 'OFFEN',

# vpn.cgi/vpnconfig.dat
'name must only contain characters' => 'Der Name darf nur Buchstaben enthalten.',
'left ip is invalid' => 'Die linke IP-Adresse ist ung&uuml;ltig.',
'left next hop ip is invalid' => 'Der n&auml;chste Sprung links ist ung&uuml;ltig.',
'left subnet is invalid' => 'Das linke Subnetz ist ung&uuml;ltig.',
'right ip is invalid' => 'Die rechte IP-Adresse ist ung&uuml;ltig.',
'right next hop ip is invalid' => 'Der n&auml;chste Sprung rechts ist ung&uuml;ltig.',
'right subnet is invalid' => 'Das rechte Subnetz ist &uuml;ltig.',
'vpn configuration connections' => 'VPN-Konfiguration - Verbindungen',
'add a new connection' => 'Eine neue Verbindung hinzuf&uuml;gen:',
'namec' => 'Name:',
'leftc' => 'Links:',
'left next hopc' => 'N&auml;chster Sprung links:',
'left subnetc' => 'Linkes Subnetz:',
'rightc' => 'Rechts:',
'right next hopc' => 'N&auml;chster Sprung rechts:',
'right subnetc' => 'Rechtes Subnetz:',
'secretc' => 'Geheimnis:',
'current connections' => 'Bestehende Verbindungen:',
'markc' => 'Markierung:',
'import and export' => 'Im- und Export:',
'import' => 'Import', # button

# graphs.cgi
'network traffic graphs' => 'Netzverkehrsstatistik',
'network traffic graphsc' => 'Netzverkehrsstatistik:',
'no graphs available' => 'Keine Statistik verf&uuml;gbar.',
'no information available' => 'Keine Informationen verf&uuml;gbar.',

# usbadsl.cgi
'usb adsl setup' => 'USB ADSL-Setup',
'usb adsl help' => 'Um das USB-Modem nutzen zu k&ouml;nnen m&uuml;ssen Sie die Modem-Firmware zu Ihrer SmoothWall hochladen. Bitte holen Sie das Archiv von Alcatels WebSite und laden Sie anschlie&szlig;end die Datei <B>mgmt.o</B> in die SmoothWall hoch indem Sie folgendes Formular verwenden.',
'upload' => 'Hochladen', # button
'upload successfull' => 'Datei&uuml;bertragung erfolgreich.',
'could not create file' => 'Datei konnte nicht erstellt werden.',
'mgmt upload' => 'Hochladen des USB ADSL-Treibers:',
'upload filec' => 'Hochzuladende Datei:',

# updates.cgi
'updates' => 'Updates',
'could not open available updates file' => 'Die Datei mit den verf&uuml;gbaren Updates konnte nicht ge&ouml;ffnet werden.',
'could not download the available updates list' => 'Die Liste der verf&uuml;gbaren Updates konnte nicht heruntergeladen werden.',
'could not create directory' => 'Das Verzeichnis konnte nicht erstellt werden.',
'could not open updatefile for writing' => 'Die Update-Datei konnte nicht zum Schreiben ge&ouml;ffnet werden.',
'this is not an authorised update' => 'Dies ist entweder kein authorisiertes Update oder Ihre Updateliste ist nicht aktuell.',
'this is not a valid archive' => 'Das Archiv ist fehlerhaft.',
'could not open update information file' => 'Die Datei mit den Update-Informationen konnte nicht ge&ouml;ffnet werden. Die Datei ist ung&uuml;ltig.',
'this update is already installed' => 'Dieses Update ist bereits installiert.',
'package failed to install' => 'Das Paket konnte nicht installiert werden.',
'update installed but' => 'Das Update wurde installiert, allerdings konnte das Verzeichnis der installierten Pakete nicht aktualisiert werden',
'refresh update list' => 'Updateliste aktualisieren', # button
'installed updates' => 'Installierte Updates:',
'id' => 'ID',
'title' => 'Titel',
'description' => 'Beschreibung',
'released' => 'Ver&ouml;ffentlicht',
'installed' => 'Installiert',
'could not open installed updates file' => 'Die Datei mit den installierten Updates konnte nicht ge&ouml;ffnet werden',
'available updates' => 'Verf&uuml;gbare Updates:',
'there are updates available' => 'Es sind f&uuml;r Ihr System Updates verf&uuml;gbar. Es wird Ihnen dringend geraten, diese so schnell wie m&ouml;glich zu installieren.',
'info' => 'Info',
'all updates installed' => 'Alle Updates installiert',
'install new update' => 'Neues Update installieren:',
'to install an update' => 'Um ein Update zu installieren laden Sie bitte die .tar.gz-Datei nachfolgend hoch:',
'upload update file' => 'Update-Datei hochladen:',
'could not download latest patch list' => 'Konnte die aktuelle Liste der Patches nicht herunterladen (keine Verbindung).',
'could not connect to smoothwall org' => 'Konnte nicht zu smoothwall.org verbinden',
'successfully refreshed updates list' => 'Updateliste erfolgreich aktualisiert.',
'the following update was successfully installedc' => 'Das folgende Update wurde erfolgreich installiert:',

# ids.cgi
'snort is enabled' => 'Snort ist aktiviert',
'snort is disabled' => 'Snort ist deaktiviert',
'intrusion detection system2' => 'Einbruchsmeldesystem (IDS):',

);
