/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * (c) Dutch Translation Team:
 * Filip Onkelinx
 * Ard Beenhakker
 * Paul Rijnhout 
 * Maurits De Loe
 * Narfhimself (WouterArts)
 * Alfred de Klerk
 * Wim Van den Meutter
 * Sjoerd Skoelmans   
 * Tim Dobbelaere     
 * Pim Aarts
 * Jeroen Wijnands
 * Marnix Janse   
 * Walter Hebberecht
 * Olaf Beumer
 * Jorrit van Eijk
 * J.van der zijden
 * Vincent Homans  
 * Robert Vis
 * Nico Oosterwijk "MrMan"
 * Hans van Zijl IT diensten
 * Jochen Van Den Bossche
 *
 * filename: dutch.c
 * Contains dutch strings. */

#include "libsmooth.h"

char *dutch_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Annuleren",
/* TR_INSTALLATION_CANCELED */
"Installatie afgebroken.",
/* TR_HELPLINE */
"           <Tab>/<Alt-Tab> tussen velden      |  <Spatie> selecteert",
/* TR_QUIT */
"Stoppen",
/* TR_DISABLED */
"Uitgeschakeld",
/* TR_ENABLED */
"Ingeschakeld",
/* TR_UNSET */
"NIET INGESTELD",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Kan instellingenbestand niet openen",
/* TR_DONE */
"Klaar",
/* TR_PROBE_FAILED */
"Autodetectie is mislukt.",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"IP-adres:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Netwerkadres:",
/* TR_NETMASK_PROMPT */
"Netwerkmasker:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Geef de IP-adresinformatie",
/* TR_INVALID_FIELDS */
"De volgende velden zijn ongeldig:\n\n",
/* TR_IP_ADDRESS_CR */
"IP-adres\n",
/* TR_NETWORK_ADDRESS_CR */
"Netwerkadres\n",
/* TR_NETWORK_MASK_CR */
"Netwerkmasker\n",
/* TR_INTERFACE (%s is interface name) */
"%s interface",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Geef de IP-adresinformatie voor het volgende interface: %s.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Op zoek naar: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall heeft de volgende NIC(s) gevonden in uw computer: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"Modulenaam mag niet leeg zijn.",
/* TR_STATIC */
"Static",
/* TR_DHCP_HOSTNAME */
"DHCP Hostnaam:",
/* TR_DHCP_HOSTNAME_CR */
"DHCP Hostnaam\n",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Kan niet schrijven naar /var/smoothwall/main/hostname.conf",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Kan niet schrijven naar /etc/hosts.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"Kan niet schrijven naar /etc/hosts.deny.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Kan niet schrijven naar /etc/hosts.allow.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Kan hostnaam niet instellen.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Welkom bij het SmoothWall installatieprogramma. Breng een bezoek aan onze " \
"homepage op http://www.smoothwall.org.  Wanneer u de installatie afbreekt " \
"op een van de volgende schermen, dan zal de computer opnieuw starten.",
/* TR_NO_IDE_HARDDISK */
"Geen IDE harde schijf gevonden.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Selecteer installatiemedium",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"SmoothWall kan van verschillende bronnen geinstalleerd worden.  Het eenvoudigste is " \
"om de CDROM-speler van de computer te gebruiken. Als uw computer er geen heeft, dan kunt u " \
"via een andere computer op het netwerk installeren die de installatiebestanden beschikbaar " \
"stelt via HTTP. In dat geval heeft u de netwerk driverdiskette nodig.",
/* TR_NO_IDE_CDROM */
"Geen IDE CDROM-speler gevonden.",
/* TR_INSERT_CDROM */
"Plaats de SmoothWall CD in de CDROM-speler.",
/* TR_INSERT_FLOPPY */
"Plaats de SmoothWall driverdiskette in het diskettestation.",
/* TR_PREPARE_HARDDISK */
"Het installatieprogramma zal nu de IDE harde schijf op %s klaarmaken. " \
"Eerst wordt de schijf gepartitioneerd, daarna wordt er een filesystem op de " \
"partities aangemaakt.",
/* TR_PARTITIONING_DISK */
"Schijf wordt gepartitioneerd...",
/* TR_UNABLE_TO_PARTITION */
"Kan schijf niet partitioneren.",
/* TR_MAKING_SWAPSPACE */
"Swap space wordt aangemaakt...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Kan swap space niet aanmaken.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Root filesystem wordt aangemaakt...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Kan root filesystem niet aanmaken.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Root filesystem mounten...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Kan root filesystem niet mounten.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Boot filesystem wordt aangemaakt...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Kan boot filesystem niet aanmaken.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Boot filesystem mounten...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Kan boot filesystem niet mounten.",
/* TR_MAKING_LOG_FILESYSTEM */
"Log filesystem wordt aangemaakt...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Kan log filesystem niet aanmaken.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Log filesystem mounten...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Kan log filesystem niet mounten.",
/* TR_MOUNTING_SWAP_PARTITION */
"Swap partitie mouten...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Kan swap partitie niet mounten.",
/* TR_NETWORK_SETUP_FAILED */
"Netwerkinstallatie mislukt.",
/* TR_NO_TARBALL_DOWNLOADED */
"Geen tarball gedownload.",
/* TR_INSTALLING_FILES */
"Bestanden installeren...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Kan bestanden niet installeren.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Kan tijdelijk gedownloade bestanden niet verwijderen.",
/* TR_ERROR_WRITING_CONFIG */
"Fout bij wegschrijven configuratie-informatie.",
/* TR_INSTALLING_LILO */
"LILO installeren...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Kan LILO niet installeren.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Kan harde schijf niet unmounten.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Kan CDROM/diskette niet unmounten.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Kan CDROM niet uitwerpen.",
/* TR_CONGRATULATIONS */
"Gefeliciteerd!",
/* TR_CONGRATULATIONS_LONG */
"De SmoothWall installatie is gelukt. Haal a.u.b. de diskette en/of CDROM " \
"uit de computer. Setup zal nu starten. Hiermee kunt u ISDN, netwerkkaarten, " \
"en systeempasswords instellen. Als Setup is voltooid, ga dan met uw " \
"web browser naar http://smoothwall:81 of https://smoothwall:445 " \
"(of hoe uw SmoothWall ook heet), en configureer dialup networking (indien " \
"benodigd) en externe toegang. Vergeet niet een password in te stellen " \
"voor de SmoothWall 'dial' gebruiker, als u niet-SmoothWall 'admin'-gebruikers " \
"de inbelverbinding wilt laten beheren.",
/* TR_PRESS_OK_TO_REBOOT */
"Druk op Ok om opnieuw te starten.",
/* TR_ERROR */
"Fout",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"Berekenen van module-afhankelijkheden...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Kan module-afhankelijkheden niet berekenen.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Selecteer CDROM-type",
/* TR_SELECT_CDROM_TYPE_LONG */
"Er is geen IDE CDROM gedetecteerd in deze computer.  Selecteer welke van de " \
"volgende drivers u wilt gebruiken zodat SmoothWall de CDROM-speler kan gebruiken.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(voorstel %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(voorstel %d)",
/* TR_CONFIGURE_THE_CDROM */
"Configureer de CDROM-speler door het juiste IO-adres en/of IRQ op te geven.",
/* TR_INVALID_IO (note extra space) */
"De IO-poort-gegevens die zijn ingevoerd zijn onjuist. ",
/* TR_INVALID_IRQ */
"De IRQ-gegevens die zijn ingevoerd zijn onjuist.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Kan niet schrijven naar /var/smoothwall/main/settings.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Kan niet schrijven naar /var/smoothwall/ethernet/settings.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Kan symlink niet aanmaken: /dev/harddisk.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Kan symlink niet aanmaken: /dev/harddisk1.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Kan symlink niet aanmaken: /dev/harddisk2.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Kan symlink niet aanmaken: /dev/harddisk3.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Kan symlink niet aanmaken: /dev/harddisk4.",

/* net.c */
/* TR_DOWNLOADING */
"Downloaden...",
/* TR_FAILED_TO_DOWNLOAD */
"Kon niet downloaden.",
/* TR_ENTER_URL */
"Geef de URL naar het SmoothWall tar.gz bestand. " \
"WAARSCHUWING: DNS niet beschikbaar!  Het moet met bestandsnaam 'smoothwall.tgz' eindigen.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Netwerk configureren",
/* TR_CONFIGURE_NETWORKING_LONG */
"U moet nu het netwerk configureren door eerst de juiste driver voor de " \
"GROENE interface te laden. U kunt dit doen door uw netwerkkaart te auto-proben, " \
"of door de juiste driver uit de lijst te kiezen. Opmerking: als u meer dan 1 " \
"netwerkkaart hebt geinstalleerd, dan kunt u de andere kaarten later in de " \
"installatie configureren. Let er ook op dat als u meer dan 1 kaart heeft " \
"die van hetzelfde type is als de GROENE en elke kaart heeft speciale module " \
"parameters nodig, dan moet u die parameters voor elke kaart opgeven zodat "  \
"alle kaarten aktief kunnen zijn op het moment dat u de GROENE interface configureert.",
/* TR_INTERFACE_FAILED_TO_COME_UP */
"Interface werkt niet goed.",
/* TR_ENTER_NETWORK_DRIVER */
"Kan netwerkkaart niet automatisch detecteren. Kies de driver en " \
"eventuele parameters voor de netwerkkaart.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Hostnaam",
/* TR_NETWORKING */
"Netwerk",
/* TR_DHCP_SERVER_CONFIGURATION */
"DHCP server configuratie",
/* TR_ISDN_CONFIGURATION */
"ISDN configuratie",
/* TR_ROOT_PASSWORD */
"\'root\' password",
/* TR_SETUP_PASSWORD */
"\'setup\' password",
/* TR_ADMIN_PASSWORD */
"Admin password",
/* TR_SECTION_MENU */
"Sectie-menu",
/* TR_SELECT_THE_ITEM */
"Selecteer het item dat u wilt configureren.",
/* TR_SETUP_FINISHED */
"Setup is voltooid.  Druk op Ok om opnieuw te starten.",
/* TR_SETUP_NOT_COMPLETE */
"De eerste setup was niet geheel compleet.  Zorg ervoor dat Setup volledig " \
"is voltooid door setup nogmaals te starten in de shell.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Geef het 'root'-gebruiker password. Log in als deze gebruiker voor toegang tot de commandoregel.",
/* TR_SETTING_ROOT_PASSWORD */
"\'root\' password wordt gezet....",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Probleem met instellen 'root' password.",
/* TR_ENTER_SETUP_PASSWORD */
"Geef het 'setup'-gebruiker password. Log in als deze gebruiker om het setup-programma te " \
"starten.",
/* TR_SETTING_SETUP_PASSWORD */
"\'setup\' password wordt gezet....",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Probleem met instellen 'setup' password.",
/* TR_ENTER_ADMIN_PASSWORD */
"Geef het SmoothWall admin password.  Dit is de gebruikersnaam die u moet gebruiken om in te " \
"loggen op de SmoothWall beheer-webpagina\'s.",
/* TR_SETTING_ADMIN_PASSWORD */
"SmoothWall admin password wordt gezet....",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Probleem met instellen SmoothWall admin password.",
/* TR_PASSWORD_PROMPT */
"Password:",
/* TR_AGAIN_PROMPT */
"Nogmaals:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"Password mag niet leeg zijn.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"Passwords komen niet overeen.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Geef de hostnaam van deze computer.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"Hostnaam mag niet leeg zijn.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"Hostnaam mag geen spaties bevatten.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Kan niet schrijven naar /etc/hostname",

/* isdn.c */
/* TR_GERMAN_1TR6 */
"German 1TR6",
/* TR_EURO_EDSS1 */
"Euro (EDSS1)",
/* TR_LEASED_LINE */
"Huurlijn",
/* TR_US_NI1 */
"US NI1",
/* TR_PROTOCOL_COUNTRY */
"Protocol/Land",
/* TR_ISDN_CARD */
"ISDN-kaart",
/* TR_MSN_CONFIGURATION */
"Lokaal telefoonnummer (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Extra module parameters",
/* TR_RED_IN_USE */
"ISDN (of een andere externe verbinding) is momenteel in gebruik.  U mag ISDN niet " \
"configureren als het RODE interface aktief is.",
/* TR_ISDN_CONFIGURATION_MENU */
"ISDN configuratiemenu",
/* TR_ISDN_STATUS */
"ISDN is momenteel %s.\n\n" \
"   Protocol: %s\n" \
"   Kaart: %s\n" \
"   Lokaal telefoonnummer: %s\n\n" \
"Kies het item dat u opnieuw wilt configureren, of kies ervoor om de huidige instellingen te gebruiken.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"ISDN is nog niet geconfigureerd. Selecteer het item dat u wilt configureren.",
/* TR_ENABLE_ISDN */
"ISDN inschakelen",
/* TR_DISABLE_ISDN */
"ISDN uitschakelen",
/* TR_INITIALISING_ISDN */
"ISDN initialiseren...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Kan ISDN niet initialiseren.",
/* TR_ISDN_NOT_SETUP */
"ISDN niet ingesteld. Een aantal items zijn nog niet geselecteerd.",
/* TR_ISDN_PROTOCOL_SELECTION */
"ISDN protocol selectie",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Kies het ISDN-protocol dat u wilt gebruiken.",
/* TR_AUTODETECT */
"* AUTODETECTIE *",
/* TR_ISDN_CARD_SELECTION */
"ISDN-kaart selectie",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Kies de ISDN-kaart die in deze computer is geinstalleerd.",
/* TR_CHECKING_FOR */
"Kijken naar: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"ISDN-kaart niet gedetecteerd. Waarschijnlijk moet u extra module parameters " \
"opgeven als de kaart van het ISA-type is of als deze speciale instellingen nodig heeft.",
/* TR_DETECTED */
"Gedetecteerd is een: %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"Kan geen ISDN-kaart vinden in deze computer. Waarschijnlijk moet u extra module parameters " \
"opgeven als de kaart van het ISA-type is of als deze speciale instellingen nodig " \
"heeft.",
/* TR_ENTER_THE_LOCAL_MSN */
"Geef het lokale telefoonnummer (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"Telefoonnummer mag niet leeg zijn.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Sommige ISDN-kaarten (met name ISA-kaarten) hebben extra module parameters " \
"nodig om de IRQ en IO adres gegevens. Als u zo'n ISDN-kaart heeft, " \
"geef dan hier de extra parameters op. Bijvoorbeeld: " \
"\"io=0x280 irq=9\". Deze gegevens worden gebruikt bij de kaart-detectie.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Netwerk wordt gestopt...",
/* TR_PULLING_NETWORK_UP */
"Netwerk wordt gestart...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Netwerk-configuratie type",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Drivers en kaart toewijzing",
/* TR_ADDRESS_SETTINGS */
"Adres instellingen",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"DNS en Gateway instellingen",
/* TR_RESTART_REQUIRED */
"\n\nWanneer de configuratie klaar is, zal een netwerk-herstart nodig zijn.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Huidige configuratie: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Netwerk-configuratie menu",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Selecteer de netwerk-configuratie voor SmoothWall.  De volgende " \
"configuratietypes geven de interfaces weer die op ethernet aangesloten zijn. " \
"Als u deze instelling wijzigt, dan zal een netwerk-herstart nodig zijn en zult " \
"u de netwerkdriver-toewijzing moeten herconfigureren.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Niet-lokaal netwerk wordt gestopt...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"Uw configuratie is ingesteld op een enkel GROEN interface, " \
"waar al een driver aan is toegewezen.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Configureer de netwerkdrivers, en aan welk interface elke kaart is " \
"toegewezen.  De huidige configuratie is als volgt:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nWilt u deze instellingen veranderen?",
/* TR_UNCLAIMED_DRIVER */
"Er is een nog niet geclaimde ethernetkaart van het type:\n%s\n\n" \
"U kunt deze toewijzen aan:",
/* TR_CARD_ASSIGNMENT */
"Kaart toewijzing",
/* TR_PROBE */
"Proben",
/* TR_SELECT */
"Selecteren",
/* TR_NO_UNALLOCATED_CARDS */
"Geen ongebruikte kaarten meer over, " \
"maar er zijn meer nodig. U kunt de autodetectie uitvoeren en naar meer kaarten " \
"zoeken, of kies een driver uit de lijst.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Kan niet meer drivers vinden.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Alle kaarten zijn toegewezen.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Niet genoeg kaarten toegewezen.",
/* TR_MANUAL */
"* HANDMATIG *",
/* TR_SELECT_NETWORK_DRIVER */
"Selecteer de netwerkdriver",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Selecteer de netwerkdriver voor de geinstalleerde kaart in deze computer. " \
"Als u HANDMATIG selecteert, dan krijgt u de mogelijkheid om de modulenaam van " \
"de driver in te vullen, en de parameters voor de drivers die speciale " \
"instellingen nodig hebben, zoals ISA-kaarten.",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Kan driver module niet laden.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"Deze driver module is al geladen.",
/* TR_MODULE_PARAMETERS */
"Geef de modulenaam en de parameters voor de driver die u wilt gebruiken.",
/* TR_LOADING_MODULE */
"Module wordt geladen...",
/* TR_WARNING */
"WAARSCHUWING",
/* TR_WARNING_LONG */
"Als u dit IP-adres wijzigt en u bent remote ingelogd, dan zal uw " \
"verbinding met de SmoothWall computer verbroken worden en zult u opnieuw " \
"moeten verbinden via het nieuwe IP-adres. Dit brengt risico's met zich mee en " \
"dit kunt u beter alleen doen als u fysieke toegang heeft tot de computer, voor het " \
"geval er iets fout mocht gaan.",
/* TR_SINGLE_GREEN */
"Uw configuratie is ingesteld op een enkel GROEN interface.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Selecteer het interface dat u wilt herconfigureren.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"Uw configuratie gebruikt geen ethernet-adapter voor de RODE interface. " \
"DNS en Gateway informatie voor dialup-gebruikers wordt automatisch " \
"ingesteld bij het inbellen.",
/* TR_PRIMARY_DNS */
"Primaire DNS:",
/* TR_SECONDARY_DNS */
"Secundaire DNS:",
/* TR_DEFAULT_GATEWAY */
"Standaard Gateway:",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Geef de DNS en gateway informatie.  Deze instellingen worden alleen gebruikt als DHCP " \
"is uitgeschakeld op de RODE interface.",
/* TR_PRIMARY_DNS_CR */
"Primaire DNS\n",
/* TR_SECONDARY_DNS_CR */
"Secundaire DNS\n",
/* TR_DEFAULT_GATEWAY_CR */
"Standaard Gateway\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"Secondaire DNS opgegeven zonder een primaire DNS",
/* TR_UNKNOWN */
"ONBEKEND",
/* TR_NO_ORANGE_INTERFACE */
"Geen ORANJE interface toegewezen.",
/* TR_MISSING_ORANGE_IP */
"IP-informatie op het ORANJE interface ontbreekt.",
/* TR_NO_RED_INTERFACE */
"Geen RODE interface toegewezen.",
/* TR_MISSING_RED_IP */
"Geen IP informatie op het RODE interface.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Startadres:",
/* TR_END_ADDRESS */
"Eindadres:",
/* TR_DEFAULT_LEASE */
"Standaard lease (min.):",
/* TR_MAX_LEASE */
"Max. lease (min.):",
/* TR_DOMAIN_NAME_SUFFIX */
"Domeinnaam achtervoegsel:",
/* TR_CONFIGURE_DHCP */
"Configureer de DHCP server door de instellingen op te geven.",
/* TR_START_ADDRESS_CR */
"Startadres\n",
/* TR_END_ADDRESS_CR */
"Eindadres\n",
/* TR_DEFAULT_LEASE_CR */
"Standaard lease tijd\n",
/* TR_MAX_LEASE_CR */
"Max. lease tijd\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Domeinnaam achtervoegsel\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Toetsenbord-indeling",
/* TR_KEYBOARD_MAPPING_LONG */
"Kies het type toetsenbord dat u gebruikt uit onderstaande lijst.",

/* timezone.c */
/* TR_TIMEZONE */
"Tijdzone",
/* TR_TIMEZONE_LONG */
"Kies de tijdzone waar u nu bent uit onderstaande lijst.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Selecteer de USB controller",
/* TR_USBADSL_STATUS */
"USB ADSL is nu: %s\n" \
"   Controller: %s\n\n" \
"Selecteer het item dat u wilt herconfigureren, of kies ervoor om de huidige instellingen te gebruiken.",
/* TR_USBADSL_CONFIGURATION */
"USB ADSL configuratie",
/* TR_ENABLE_USBADSL */
"USB ADSL inschakelen",
/* TR_DISABLE_USBADSL */
"USB ADSL uitschakelen",
/* TR_INITIALISING_USBADSL */
"USB ADSL initialiseren.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"Kan USB ADSL niet initialiseren",
/* TR_USBADSL_NOT_SETUP */
"USB ADSL niet ingesteld.",
/* TR_USB_CONTROLLER_SELECTION */
"USB controller selectie",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Kies de USB controller die is geinstalleerd in deze SmoothWall computer.",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"USB controller niet gedetecteerd.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"Kan geen USB controller vinden.",
/* TR_STARTING_UP_USBADSL */
"USB ADSL wordt gestart..."

};
