# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# (c) French Translation Team:
# David Boileau
# Thierry Charbonnel
# Frederic Gonnet
# Francois Thomas
# Fabien Illide
# Christopher Heger
# Manu El Poulpe
# Frederic Legrain
# Roger De Riemaecker
# Eric Vaneberck
# Ismail Simoes
# Denis Renard 
# Pierre
# Andre Joanisse
# Xavier de Gelis
# Nicolas Micheli
# Hughes Devaux
# John S. Gage
# Jeff
# nicolas
# Ikos
# Cedric Thevenet

%basetr = (

%basetr,

# common
'invalid input' => 'Informations invalide',
'save' => 'Sauver', # button
'refresh' => 'Rafraichir', # button
'restore' => 'Restaurer', # button
'error messages' => 'Messages d\'erreur:',
'back' => 'Retour',
'help' => 'Aide',
'primary dns' => 'DNS primaire:',
'secondary dns' => 'DNS secondaire:',
'invalid primary dns' => 'DNS primaire invalide.',
'invalid secondary dns' => 'DNS secondaire invalide.',
'dhcp server' => 'Serveur DHCP',
'username' => 'Nom d\'utilisateur:',
'password' => 'Mot de passe:',
'enabled' => 'Activ&eacute;:',
'this field may be blank' => 'Ce champ peut &ecirc;tre vide.',
'these fields may be blank' => 'Ces champs peuvent &ecirc;tre vides.',

# header.pl
'sshome' => 'accueil',
'ssstatus' => 'statut',
'sstraffic graphs' => 'courbes de trafic',
'ssppp settings' => 'param&egrave;tres ppp',
'ssmodem' => 'modem',
'ssusb adsl firmware upload' => 't&eacute;l&eacute;chargement de pilote mat&eacute;riel usb adsl',
'ssssh' => 'acc&egrave;s &agrave; distance s&eacute;curis&eacute; (SSH)',
'sspasswords' => 'mots de passe',
'ssweb proxy' => 'mandataire (proxy)',
'ssdhcp' => 'dhcp',
'ssport forwarding' => 'transfert de ports',
'ssexternal service access' => 'acc&egrave;s aux services externes',
'ssdmz pinholes' => 'acc&egrave;s &agrave; la DMZ',
'ssdynamic dns' => 'dns dynamique',
'ssids' => 'syst&egrave;me de d&eacute;tection des intrusions',
'sscontrol' => 'contr&ocirc;le',
'ssconnections' => 'connexions',
'ssother' => 'autre',
'ssfirewall' => 'pare-feu',
'ssshutdown' => '&eacute;teindre',
'ssshell' => 'ligne de commande',
'ssupdates' => 'mises &agrave; jour',
'sshelp' => 'aide',
'sscredits' => 'credits',
'ssip info' => 'Informations sur l\'adresse IP',
'powered by' => 'fonctionne avec',
'alt home' => 'Accueil', # alt
'alt information' => 'Information', # alt
'alt dialup' => 'Connexion par t&eacute;l&eacute;phone', # alt
'alt remote access' => 'Acc&egrave;s &agrave; distance', # alt
'alt services' => 'Services', # alt
'alt ids' => 'Syst&egrave;me de D&eacute;tection des intrusions', # alt
'alt vpn' => 'R&eacute;seaux Priv&eacute;s Virtuels', # alt
'alt logs' => 'Journaux', #alt 
'alt shutdown' => 'Eteindre', # alt
'alt shell' => 'Ligne de commande', # alt
'alt updates' => 'Mises &agrave; jour', # alt

# changepw.cgi
'admin user password has been changed' => 'Le mot de passe d\'administration a &eacute;t&eacute; modifi&eacute;.',
'dial user password has been changed' => 'Le mot de passe de connexion a &eacute;t&eacute; modifi&eacute;.',
'password cant be blank' => 'Le mot de passe ne peut &ecirc;tre vide.',
'passwords do not match' => 'Les mots de passe ne correspondent pas.',
'change passwords' => 'Changer les mots de passe',
'administrator user password' => 'Mot de passe d\'administration:',
'dial user password' => 'Mot de passe de connexion:',
'again' => 'A nouveau:',
'passwords must be at least 6 characters in length' => 'Les mots de passes doivent être d\'une longeur d\'au moins 6 caractères',
'password contains illegal characters' => 'Le mot de passe contient les caractères illégaux.',

# credits.cgi
'credits' => 'Credits',
'version' => 'Version: ',
'sponsors' => 'Sponsors',
'links' => 'Liens',
'smoothwall homepage' => 'Site web de SmoothWall',
'translation teams' => 'Les équipes de traduction',

# dhcp.cgi
'invalid start address' => 'Adresse de d&eacute;part invalide.',
'invalid end address' => 'Adresse de fin invalide.',
'cannot specify secondary dns without specifying primary' => 'Impossible de sp&eacute;cifier un serveur DNS secondaire sans sp&eacute;cifier de serveur primaire.',
'invalid default lease time' => 'Dur&eacute;e de bail par d&eacute;faut invalide.',
'invalid max lease time' => 'Dur&eacute;e maximum de bail invalide.',
'dhcp server enabled' => 'Serveur DHCP activ&eacute;.  Red&eacute;marrage.',
'dhcp server disabled' => 'Serveur DHCP d&eacute;sactiv&eacute;.  Arr&ecirc;t&eacute;.',
'dhcp configuration' => 'configuration DHCP',
'start address' => 'Adresse de d&eacute;part:',
'end address' => 'Adresse de fin:',
'default lease time' => 'Dur&eacute;e de bail par d&eacute;faut (minutes):',
'max lease time' => 'Dur&eacute;e maximum de bail (minutes):',
'domain name suffix' => 'Suffixe de nom de domaine:',

# proxy.cgi
'web proxy configuration' => 'Configuration du serveur mandataire (proxy)',
'web proxyc' => 'Serveur mandataire:',
'cache size' => 'Taille du cache (Mo):',
'invalid cache size' => 'Taille du cache invalide.',
'remote proxy' => 'Serveur mandataire distant:',
'invalid maximum object size' => 'Taille maximum d\'objet invalide.',
'invalid minimum object size' => 'Taille minimum d\'objet invalide.',
'invalid maximum outgoing size' => 'Taille maximum de sortie invalide.',
'invalid maximum incoming size' => 'Taille maximum d\'entr&eacute;e invalide.',
'transparent' => 'Transparent:',
'max size' => 'Taille maximum d\'objet (Ko):',
'min size' => 'Taille minimum d\'objet (Ko):',
'max outgoing size' => 'Taille maximum de sortie (Ko):',
'max incoming size' => 'Taille maximum d\'entr&eacute;e (Ko):',

# common to logs cgis
'january' => 'Janvier',
'february' => 'F&eacute;vrier',
'march' => 'Mars',
'april' => 'Avril',
'may' => 'Mai',
'june' => 'Juin',
'july' => 'Juillet',
'august' => 'Ao&ucirc;t',
'september' => 'Septembre',
'october' => 'Octobre',
'november' => 'Novembre',
'december' => 'D&eacute;cembre',
'month' => 'Mois:',
'day' => 'Jour:',
'update' => 'MISE A JOUR', # button
'export' => 'EXPORTER', # button
'older' => 'Plus ancien',
'newer' => 'Plus r&eacute;cent',
'settingsc' => 'Param&egrave;tres:',

# logs.cgi/firewalllog.dat
'firewall log' => 'Journal du pare-feu',
'firewall log2' => 'Journal du pare-feu:',
'date' => 'Date:',
'time' => 'Heure',
'action' => 'Action',
'chain' => 'Chaine',
'iface' => 'Interface',
'protocol' => 'Protocole',
'source' => 'Source',
'src port' => 'Port Source',
'destination' => 'Destination',
'dst port' => 'Port de Destination',
'unknown' => 'INCONNU',
'lookup' => 'Recherche DNS',

# logs.cgi/log.dat
'log viewer' => 'Visualisation de journaux',
'section' => 'Section:',
'kernel' => 'Noyau',
'loginlogout' => 'Connexion utilisateur',
'update transcript' => 'Update transcript',
'log' => 'Journal:',

# logs.cgi/proxylog.dat
'proxy log viewer' => 'Visualisation des journaux du serveur mandataire',
'bad ignore filter' => 'Bad ignore filter:',
'caps all' => 'TOUT',
'ignore filterc' => 'Ignore filter:',
'enable ignore filterc' => 'Enable ignore filter:',
'source ip' => 'Adresse IP source',
'website' => 'Site web',

# logs.cgi/ids.dat
'ids log viewer' => 'Visualisation des journaux du Syst&egrave;me de D&eacute;tection des Intrusions',
'datec' => 'Date:',
'namec' => 'Nom:',
'priorityc' => 'Priorit&eacute;:',
'typec' => 'Type:',
'ipinfoc' => 'Informations sur l\'adresse IP:',
'referencesc' => 'R&eacute;f&eacute;rences:',
'none found' => 'aucune entr&eacute;e trouv&eacute;e',

# index.cgi
'main page' => 'Page principale',
'dial' => 'CONNEXION', # button
'hangup' => 'DECONNEXION', # button
'current profile' => 'Profil actuel:',
'connected' => 'Connect&eacute;',
'dialing' => 'Appel en cours...',
'modem idle' => 'Modem inactif',
'isdn idle' => 'RNIS inactif',
'profile has errors' => 'Des erreurs existent dans ce profil',
'modem settings have errors' => 'Les param&egrave;tres du modem contiennent des erreurs',
'user pages' => 'Pages de l\'utilisateur',
'mstatus information' => 'Information&nbsp;sur&nbsp;le&nbsp;statut',
'mnetwork traffic graphs' => 'Courbes&nbsp;du&nbsp;trafic&nbsp;sur&nbsp;le&nbsp;r&eacute;seau',
'administrator pages' => 'Pages de l\'administrateur',
'mppp setup' => 'Param&eacute;trage&nbsp;PPP',
'mmodem configuration' => 'Configuration&nbsp;du&nbsp;modem',
'mchange passwords' => 'Changement&nbsp;des&nbsp;mots&nbsp;de&nbsp;passe',
'mremote access' => 'Acc&egrave;s&nbsp;&agrave;&nbsp;distance',
'mdhcp configuration' => 'Configuration&nbsp;du&nbsp;serveur&nbsp;DHCP',
'mproxy configuration' => 'Configuration&nbsp;du&nbsp;serveur&nbsp;mandataire&nbsp;(proxy)',
'mport forwarding configuration' => 'Configuration&nbsp;du&nbsp;transfert&nbsp;de&nbsp;ports',
'mshutdown control' => 'Contr&ocirc;le&nbsp;de&nbsp;l\'arr&ecirc;t&nbsp;du&nbsp;serveur',
'mlog viewer' => 'Visualisation&nbsp;des&nbsp;journaux',
'mfirewall log viewer' => 'Visualisation&nbsp;des&nbsp;journaux&nbsp;du&nbsp;pare-feu',
'msecure shell' => 'Acc&egrave;s&nbsp;&agrave;&nbsp;distance&nbsp;s&eacute;curis&eacute;&nbsp;(SSH)',
'modem dod waiting' => 'Modem en mode Connexion sur Demande en attente de requ&ecirc;te',
'isdn dod waiting' => 'RNIS en mode Connexion sur Demande en attente de requ&ecirc;te',
'pppoe idle' => 'PPPOE inactif',
'usbadsl idle' => 'USB ADSL inactif',
'pppoe dod waiting' => 'PPPOE en mode Connexion sur Demande en attente de requ&ecirc;te',
'there are updates' => 'Des mises &agrave; jour sont disponibles pour votre syst&egrave;me. Veuillez aller &agrave; la section "Mises &agrave jour" pour plus d\'information.',
'updates is old1' => 'Votre fichier de mise &agrave; jour est vieux de ',
'updates is old2' => 'jours. Il est recommand&eacute; de l\'actualiser &agrave; la section "Mises &agrave; jour".',

# pppsetup.cgi
'profile name not given' => 'Nom du profil non d&eacute;fini.',
'telephone not set' => 'Num&eacute;ro de T&eacute;l&eacute;phone non d&eacute;fini.',
'bad characters in the telephone number field' => 'Caract&egrave;res interdits dans le champ Num&eacute;ro de t&eacute;l&eacute;phone.',
'username not set' => 'Nom d\'utilisateur non d&eacute;fini.',
'spaces not allowed in the username field' => 'Espaces interdits dans le champ Nom d\'utilisateur.',
'password not set' => 'Mot de passe non d&eacute;fini.',
'spaces not allowed in the password field' => 'Espaces interdits dans le champ Mot de passe.',
'idle timeout not set' => 'Temps maximum d\'inactivit&eacute; non d&eacute;fini.',
'only digits allowed in the idle timeout' => 'Seuls les chiffres sont autoris&eacute;s dans le temps maximum d\'inactivit&eacute;.',
'bad characters in script field' => 'Caract&egrave;res interdits dans le champ Script',
'max retries not set' => 'Nombre d\'essais maximum non d&eacute;fini.',
'only digits allowed in max retries field' => 'Seuls les chiffres sont autoris&eacute;s dans le champ Nombre d\'essais maximum.',
'profile saved' => 'Profil enregistr&eacute;: ',
'select' => 'SELECTIONNER', # button
'profile made current' => 'Profil en cours: ',
'the selected profile is empty' => 'Le profil s&eacute;lectionn&eacute; est vide.',
'delete' => 'EFFACER', # button
'profile deleted' => 'Profil effac&eacute;: ',
'empty' => 'Vide',
'unnamed' => 'Non d&eacute;fini',
'ppp setup' => 'Configuration PPP',
'profiles' => 'Profils:',
'profile name' => 'Nom du profil:',
'telephony' => 'T&eacute;l&eacute;phonie:',
'interface' => 'Interface:',
'modem on com1' => 'Modem sur le port COM1',
'modem on com2' => 'Modem sur le port COM2',
'modem on com3' => 'Modem sur le port COM3',
'modem on com4' => 'Modem sur le port COM4',
'isdn tty' => 'RNIS sur TTY',
'isdn1' => 'RNIS Simple',
'isdn2' => 'RNIS Double',
'computer to modem rate' => 'Vitesse de l\'ordinateur au modem:',
'number' => 'Num&eacute;ro:',
'modem speaker on' => 'Haut-Parleur du modem activ&eacute;:',
'dialing mode' => 'Mode d\'appel:',
'tone' => 'Tonalit&eacute;',
'pulse' => 'Impulsions',
'maximum retries' => 'Nombre d\'essais maximum:',
'idle timeout' => 'Dur&eacute;e maximum d\'inactivit&eacute; (en minutes; 0 pour d&eacute;sactiver):',
'persistent connection' => 'Connexion permanente:',
'authentication' => 'Authentification:',
'method' => 'M&eacute;thode:',
'pap or chap' => 'PAP ou CHAP',
'standard login script' => 'Script d\'identification standard',
'demon login script' => 'Script d\'identification automatis&eacute;',
'other login script' => 'Autre script d\'identification',
'script name' => 'Nom du script:',
'type' => 'Type:',
'manual' => 'Manuel',
'automatic' => 'Automatique',
'dod' => 'Connexion sur Demande:',
'dod for dns' => 'Connexion sur Demande pour DNS:',
'connect on smoothwall restart' => 'Connexion au red&eacute;marrage de SmoothWall:',
'pppoe settings' => 'Param&egrave;tres avanc&eacute;s PPPoE:',
'usb adsl settings' => 'Param&egrave;tres avanc&eacute;s USB ADSL:',
'service name' => 'Nom du service:',
'concentrator name' => 'Nom du concentrateur:',
'vpi number' => 'Num&eacute;ro VPI:',
'vci number' => 'Num&eacute;ro VCI:',
'firmwarec' => 'Pilote mat&eacute;riel:',
'firmware present' => 'Pr&eacute;sent',
'firmware not present' => '<B>Non</B> pr&eacute;sent',
'upload usb adsl firmware' => 'T&eacute;l&eacute;chargement de pilote mat&eacute;riel USB ADSL',
'no usb adsl firmware' => 'Pas de pilote mat&eacute;riel USB ADSL. Veuillez en t&eacute;l&eacute;charger un.',
'dial on demand for this interface is not supported' => 'La connexion sur demande n\'est pas support&eacute;e pour cette interface.',
'unable to alter profiles while red is active' => 'Impossible de modifier les profiles quand ROUGE est actif.',

# remote.cgi
'ssh is enabled' => 'Acc&egrave;s &agrave; distance s&eacute;curis&eacute; (SSH) activ&eacute;. Red&eacute;marrage.',
'ssh is disabled' => 'Acc&egrave;s &agrave; distance s&eacute;curis&eacute; (SSH) d&eacute;sactiv&eacute;. Arr&ecirc;t.',
'remote access' => 'Acc&egrave;s &agrave; distance',
'remote access2' => 'Acc&egrave;s &agrave; distance:',

# shutdown.cgi
'shutting down smoothwall' => 'Arr&ecirc;t de SmoothWall en cours',
'shutdown control' => 'Contr&ocirc;le de l\'arr&ecirc;t du serveur',
'shutdown' => 'ARRETER', # button
'shutdown2' => 'Arr&ecirc;ter:',
'shutting down' => 'Arr&ecirc;t en cours',
'smoothwall has now shutdown' => 'SmoothWall est maintenant arr&ecirc;t&eacute;.',
'rebooting smoothwall' => 'SmoothWall red&eacute;marre',
'reboot' => 'REDEMARRAGE', # button
'rebooting' => 'Red&eacute;marrage en cours',
'smoothwall has now rebooted' => 'SmoothWall a maintenant red&eacute;marr&eacute;.',

# status.cgi
'web server' => 'Serveur Web',
'cron server' => 'Serveur CRON',
'dns proxy server' => 'Serveur DNS mandataire (proxy DNS)',
'logging server' => 'Serveur d\'enregistrement de journaux',
'kernel logging server' => 'Serveur de journaux du noyau syst&egrave;me',
'secure shell server' => 'Acc&egrave;s &agrave; distance s&eacute;curis&eacute; (SSH)',
'vpn' => 'R&eacute;seau Priv&eacute; Virtuel',
'web proxy' => 'Serveur mandataire (proxy web)',
'intrusion detection system' => 'Syst&egrave;me de D&eacute;tection des Intrusions',
'status information' => 'Informations sur le statut',
'services' => 'Services:',
'memory' => 'Memoire:',
'uptime and users' => 'Dur&eacute;e de fonctionnement et utilisateurs:',
'interfaces' => 'Interfaces:',
'disk usage' => 'Utilisation du disque:',
'loaded modules' => 'Modules charg&eacute;s:',
'kernel version' => 'Version du noyau:',
'stopped' => 'ARRET&Eacute;',
'running' => 'EN&nbsp;FONCTIONNEMENT',
'swapped' => 'ECHANG&Ecirc;',

# portfw.cgi and dmzhole.cgi and xtaccess.cgi
'source port numbers' => 'Le port source doit &ecirc;tre un nombre.',
'source is bad' => 'L\'adresse IP ou la plage d\'adresses n\'est pas valide.',
'destination ip bad' => 'Adresse IP de destination invalide',
'destination port numbers' => 'Le port de destination doit &ecirc;tre un nombre.',
'unable to open file' => 'Impossible d\'ouvrir le fichier',
'source port in use' => 'Port source utilis&eacute;:',
'forwarding rule added' => 'R&egrave;gle de transfert ajout&eacute;e; red&eacute;marrage du moteur',
'forwarding rule removed' => 'R&egrave;gle de transfert supprim&eacute;e; red&eacute;marrage du moteur',
'external access rule added' => 'R&egrave;gle d\'acc&egrave;s ext&eacute;rieur ajout&eacute;e; red&eacute;marrage du contr&ocirc;leur d\'acc&egrave;s',
'external access rule removed' => 'R&egrave;gle d\'acc&egrave;s ext&eacute;rieur supprim&eacute;e; red&eacute;marrage du contr&ocirc;leur d\'acc&egrave;s',
'dmz pinhole rule added' => 'R&egrave;gle d\'acc&egrave;s &agrave; la DMZ ajout&eacute;e; red&eacute;marrage du moteur',
'dmz pinhole rule removed' => 'R&egrave;gle d\'acc&egrave;s &agrave; la DMZ supprim&eacute;e; red&eacute;marrage du moteur',
'port forwarding configuration' => 'Configuration du transfert de ports',
'dmz pinhole configuration' => 'Configuration des acc&egrave;s &agrave; la DMZ',
'external access configuration' => 'Configuration des acc&egrave;s ext&eacute;rieurs',
'add a new rule' => 'Ajouter une nouvelle r&egrave;gle:',
'sourcec' => 'Adresse IP source, ou r&eacute;seau (vide pour "TOUT"):',
'source ipc' => 'Adresse IP source:',
'source portc' => 'Port source:',
'destination ipc' => 'Adresse IP de destination:',
'destination portc' => 'Port de destination:',
'current rules' => 'R&egrave;gles actuelles:',
'source ip' => 'Adresse IP source',
'source port' => 'Port source',
'destination ip' => 'Adresse IP de destination',
'destination port' => 'Port de destination',
'add' => 'AJOUTER', # button
'remove' => 'SUPPRIMER', # button
'edit' => 'EDITER', # button
'enabledtitle' => 'Activ&eacute;',
'nothing selected' => 'Rien n\'est s&eacute;lectionn&eacute;',
'you can only select one item to edit' => 'Vous ne pouvez &eacute;diter qu\'une seule entr&eacute;e &agrave; la fois',
'mark' => 'S&eacute;lectionner',
'all' => 'TOUT',

# ddns.cgi
'dynamic dns' => 'DNS Dynamique',
'add a host' => 'Ajouter un h&ocirc;te:',
'servicec' => 'Service:',
'behind a proxy' => 'Derri&egrave;re un serveur mandataire (proxy):',
'enable wildcards' => 'Activer les jokers:',
'hostnamec' => 'Nom d\'h&ocirc;te:',
'domainc' => 'Domaine:',
'current hosts' => 'H&ocirc;tes actuels:',
'service' => 'Service',
'hostname' => 'Nom d\'h&ocirc;te',
'domain' => 'Domaine',
'proxy' => 'Serveur mandataire (proxy)',
'wildcards' => 'Jokers',
'hostname not set' => 'Nom d\'h&ocirc;te non d&eacute;fini.',
'domain not set' => 'Domaine non d&eacute;fini.',
'invalid hostname' => 'Nom d\'h&ocirc;te invalide.',
'invalid domain name' => 'Nom de domaine invalide.',
'hostname and domain already in use' => 'Nom d\'h&ocirc;te et domaine d&eacute;j&agrave; utilis&eacute;s.',
'ddns hostname added' => 'Nom d\'h&ocirc;te de DNS dynamique ajout&eacute;',
'ddns hostname removed' => 'Nom d\'h&ocirc;te de DNS dynamique supprim&eacute;',
'force update' => 'Forcer la mise &agrave; jour',

# ipinfo.cgi
'ip info' => 'Informations IP',
'lookup failed' => 'La recherche de nom d\'h&ocirc;te a &eacute;chou&eacute;',

# shell.cgi
'secure shellc' => 'Acc&egrave;s &agrave; distance s&eacute;curis&eacute; (SSH):',

# modem.cgi
'restore defaults' => 'RESTAURER LES REGLAGES PAR DEFAUT', # button
'timeout must be a number' => 'La dur&eacute;e maximum doit &ecirc;tre un nombre.',
'modem configuration' => 'Configuration du modem',
'modem configurationc' => 'Configuration du modem:',
'init string' => 'Cha&icirc;ne d\'initialisation:',
'hangup string' => 'Cha&icirc;ne de d&eacute;connexion:',
'speaker on' => 'Haut-parleur activ&eacute;:',
'speaker off' => 'Haut-parleur d&eacute;sactiv&eacute;:',
'tone dial' => 'Appel par tonalit&eacute;:',
'pulse dial' => 'Appel par pulsation:',
'connect timeout' => 'Dur&eacute;e maximum pour l\'&eacute;tablissement de la connexion:',
'send cr' => 'Le retour chariot (CR) est requis par le fournisseur d\'acc&egrave;s:',

# vpnmain.cgi
'restart' => 'Red&eacute;marrer',
'stop' => 'Arr&ecirc;ter',
'vpn configuration main' => 'Configuration du R&eacute;seau Priv&eacute; Virtuel - Base',
'main' => 'Main',
'connections' => 'Connexions',
'global settingsc' => 'Param&egrave;tres g&eacute;n&eacute;raux:',
'local vpn ip' => 'IP du R&eacute;seau Priv&eacute; Virtuel local:',
'if blank the currently configured ethernet red address will be used' => 'Si non d&eacute;fini, l\'adresse de l\'interface ethernet ROUGE sera utilis&eacute;e.',
'manual control and status' => 'Contr&ocirc;le manuel et statut:',
'connection name' => 'Nom',
'connection status' => 'Statut',
'capsclosed' => 'FERM&Eacute;',
'capsdisabled' => 'D&Eacute;SACTIV&Eacute;',
'capsopen' => 'OUVERT',

# vpn.cgi/vpnconfig.dat
'name must only contain characters' => 'Le nom ne doit contenir que des caract&egrave;res.',
'left ip is invalid' => 'L\'adresse IP de gauche est invalide.',
'left next hop ip is invalid' => 'L\'IP du prochain h&ocirc;te est invalide (gauche).',
'left subnet is invalid' => 'Le sous-r&eacute;seau de gauche est invalide.',
'right ip is invalid' => 'L\'adresse IP de droite est invalide.',
'right next hop ip is invalid' => 'L\'IP du prochain h&ocirc;te est invalide (droite).',
'right subnet is invalid' => 'Le sous-r&eacute;seau de droite est invalide.',
'vpn configuration connections' => 'Configuration des connxions du R&eacute;seau Priv&eacute; Virtuel',
'add a new connection' => 'Ajouter une nouvelle connexion:',
'namec' => 'Nom:',
'leftc' => 'Gauche:',
'left next hopc' => 'Prochain h&ocirc;te de gauche:',
'left subnetc' => 'Sous-r&eacute;seau de gauche:',
'rightc' => 'Droite:',
'right next hopc' => 'Prochain h&ocirc;te de droite:',
'right subnetc' => 'Sous-r&eacute;seau de droite:',
'secretc' => 'Secret:',
'current connections' => 'Connexions actuelles:',
'markc' => 'S&eacute;lectionner:',
'import and export' => 'Importer et Exporter:',
'import' => 'Importer', # button

# graphs.cgi
'network traffic graphs' => 'Courbes du trafic sur le r&eacute;seau',
'network traffic graphsc' => 'Courbes de trafic r&eacute;seau:',
'no graphs available' => 'Aucun graphique disponible.',
'no information available' => 'Aucune donn&eacute;e disponible.',

# usbadsl.cgi
'usb adsl setup' => 'Configuration du modem USB ADSL',
'usb adsl help' => 'Pour utiliser le modem USB vous devez t&eacute;l&eacute;charger le pilote mat&eacute;riel (firmware) sur votre serveur SmoothWall. Veuillez vous procurer l\'archive aupr&egrave;s d\'Alcatel et charger le fichier <B>mgmt.o</B> au moyen du formulaire ci-dessous.',
'upload' => 'TELECHARGER', # button
'upload successfull' => 'T&eacute;l&eacute;chargement effectu&eacute; avec succ&egrave;s.',
'could not create file' => 'Impossible de cr&eacute;er le fichier.',
'mgmt upload' => 'T&eacute;l&eacute;chargement de pilote USB ADSL:',
'upload filec' => 'T&eacute;l&eacute;charger le fichier:',

# updates.cgi
'updates' => 'Mises &agrave; jour',
'could not open available updates file' => 'Impossible d\'ouvrir le fichier des mises &agrave; jour disponibles.',
'could not download the available updates list' => 'Impossible de t&eacute;l&eacute;charger la liste des mises &agrave; jour disponibles.',
'could not create directory' => 'Impossible de cr&eacute;er le r&eacute;pertoire.',
'could not open updatefile for writing' => 'Impossible d\'ouvrir le fichier des mises &agrave; jour en &eacute;criture.',
'this is not an authorised update' => 'Ceci n\'est pas une mise &agrave; jour autoris&eacute;e, ou bien votre liste est p&eacute;rim&eacute;e.',
'this is not a valid archive' => 'Ce n\'est pas une archive valide.',
'could not open update information file' => 'Impossible d\'ouvrir le fichier d\'information de mise &agrave; jour. Le fichier est corrompu.',
'this update is already installed' => 'Cette mise &agrave; jour est d&eacute;j&agrave; install&eacute;e.',
'package failed to install' => 'L\'archive n\'a pu &ecirc;tre install&eacute;e.',
'update installed but' => 'La mise &agrave; jour a &eacute;t&eacute; install&eacute;e mais la base de donn&eacute;es des paquets install&eacute;s n\'a pu &ecirc;tre mise &agrave; jour',
'refresh update list' => 'RAFRAICHIR LA LISTE DES MISES A JOUR', # button
'installed updates' => 'Mises &agrave; jour install&eacute;es:',
'id' => 'ID',
'title' => 'Titre',
'description' => 'Description',
'released' => 'Produit',
'installed' => 'Install&eacute;',
'could not open installed updates file' => 'Impossible d\'ouvrir le fichier des mises &agrave; jour install&eacute;es',
'available updates' => 'Mises &agrave; jour disponibles:',
'there are updates available' => 'Des mises &agrave; jour sont disponibles pour votre syst&egrave;me. Il est fortement recommand&eacute; de les installer le plus rapidement possible.',
'info' => 'Informations',
'all updates installed' => 'Toutes les mises &agrave; jour sont install&eacute;es',
'install new update' => 'Installer une nouvelle mises &agrave; jour:',
'to install an update' => 'Pour installer une mise &agrave; jour veuillez t&eacute;l&eacute;charger le fichier .tar.gz ci-dessous:',
'upload update file' => 'T&eacute;l&eacute;charger le fichier de mise &agrave; jour:',
'could not download latest patch list' => 'Impossible de t&eacute;l&eacute;charger la liste des derniers patches (non connect&eacute;).',
'could not connect to smoothwall org' => 'Impossible de se connecter &agrave; smoothwall.org',
'successfully refreshed updates list' => 'Liste des mises &agrave; jour rafraichie avec succ&egrave;s.',
'the following update was successfully installedc' => 'La mise &agrave; jour suivante a &eacute;t&eacute; install&eacute;e avec succ&egrave;s:',

# ids.cgi
'snort is enabled' => 'Snort est activ&eacute;',
'snort is disabled' => 'Snort est d&eacute;sactiv&eacute;',
'intrusion detection system2' => 'Syst&egrave;me de D&eacute;tection des Intrusions:',

);
