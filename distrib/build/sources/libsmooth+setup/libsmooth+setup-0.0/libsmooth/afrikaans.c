/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * (c) Afrikaans Translation Team:
 * Johann du Preez
 *
 * filename: afrikaans.c
 * Contains afrikaans strings. */

#include "libsmooth.h"

char *afrikaans_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Kanselleer",
/* TR_INSTALLATION_CANCELED */
"Installasie gekanselleer.",
/* TR_HELPLINE */
"             <Tab>/<Alt-Tab> tussen elemente   |  <Space> selekteer",
/* TR_QUIT */
"Verlaat",
/* TR_DISABLED */
"Gesper",
/* TR_ENABLED */
"Geaktiveer",
/* TR_UNSET */
"ONSET",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Nie in staat om konfigurasie lêer oop te maak nie",
/* TR_DONE */
"Gedoen",
/* TR_PROBE_FAILED */
"Auto deteksie het gefaal.",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"IP adres:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Netwerk adres:",
/* TR_NETMASK_PROMPT */
"Netwerk masker:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Voorsien die IP adres inligting",
/* TR_INVALID_FIELDS */
"Die volgende velde is ongeldig:\n\n",
/* TR_IP_ADDRESS_CR */
"IP adres\n",
/* TR_NETWORK_ADDRESS_CR */
"Netwerk adres\n",
/* TR_NETWORK_MASK_CR */
"Netwerk masker\n",
/* TR_INTERFACE (%s is interface name) */
"%s koppelvlak",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Voorsien die IP adres inligting vir die %s koppelvlak.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Soek na: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall het die volgende NIC in jou masjien gevind: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"Module naam kan nie blanko wees nie.",
/* TR_STATIC */
"Staties",
/* TR_DHCP_HOSTNAME */
"DHCP Masjiennaam:",
/* TR_DHCP_HOSTNAME_CR */
"DHCP Masjiennaam\n",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Nie in staat om /var/smoothwall/main/hostname.conf te skryf nie",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Nie in staat om /etc/hosts te skryf nie.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"Nie in staat om /etc/hosts.deny te skryf nie.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Nie in staat om /etc/hosts.allow te skryf nie.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Nie in staat om hostname te stel nie.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Welkom by die SmoothWall installasie program. Besoek asb ons " \
"webtuiste by http://www.smoothwall.org.  Deur Kanselleer op enige " \
"van die volgende skerms te druk, sal die masjien oorbegin (\"reboot\").",
/* TR_NO_IDE_HARDDISK */
"Geen IDE hardeskyf gevind nie.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Selekteer installasie media",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"SmoothWall kan van verskeie media geïnstalleer word.  Die eenvoudigste is om " \
"die masjien se CDROM aandrywer te gebruik. Indien die masjien nie CDROMs kan " \
"hanteer nie, kan jy via 'n ander masjien op die LAN installeer wat die installasie " \
"lêers via HTTP beskikbaar stel. In hierdie geval sal die netwerk drywer disket " \
"benodig word.",
/* TR_NO_IDE_CDROM */
"Geen IDE cdrom gevind nie.",
/* TR_INSERT_CDROM */
"Plaas asb die SmoothWall CD in die CDROM aandrywer.",
/* TR_INSERT_FLOPPY */
"Plaas asb die SmoothWall drywer disket in die floppie aandrywer.",
/* TR_PREPARE_HARDDISK */
"Die installasie program sal nou die IDE hardeskyf op %s voorberei. " \
"Die skyf sal eestens ge-partisioneer word, alvorens 'n lêerstelsel " \
"op die partisies geplaas sal word.",
/* TR_PARTITIONING_DISK */
"Besig met partisionering...",
/* TR_UNABLE_TO_PARTITION */
"Nie in staat om partisies op die skyf te skep nie.",
/* TR_MAKING_SWAPSPACE */
"Besig met skep van uitruil spasie...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Nie in staat om uitruil spasie te skep nie.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Besig met skep van root lêerstelsel...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Nie in staat om root lêerstelsel te skep nie.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Monteer root lêerstelsel...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Nie in staat om root lêerstelsel te monteer nie.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Besig met skep van boot lêerstelsel...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Nie in staat om boot lêerstelsel te skep nie.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Monteer boot lêerstelsel...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Nie in staat om boot lêerstelsel te monteer nie.",
/* TR_MAKING_LOG_FILESYSTEM */
"Besig met skep van log lêerstelsel...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Nie in staat om log lêerstelsel te skep nie.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Monteer log lêerstelsel...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Nie in staat om log lêerstelsel te monteer nie.",
/* TR_MOUNTING_SWAP_PARTITION */
"Monteer uitruil (swap) partisie...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Nie in staat om uitruil (swap) partisie te monteer nie.",
/* TR_NETWORK_SETUP_FAILED */
"Netwerk konfigurasie het gefaal.",
/* TR_NO_TARBALL_DOWNLOADED */
"Geen tar lêers afgelaai nie.",
/* TR_INSTALLING_FILES */
"Besig met lêer installasie...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Nie in staat om lêers te installeer nie.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Nie in staat om die tydelik afgelaaide lêers te verwyder nie.",
/* TR_ERROR_WRITING_CONFIG */
"Fout met met skryf van konfigurasie inligting.",
/* TR_INSTALLING_LILO */
"Besig om LILO te installeer...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Nie in staat om LILO te installeer nie.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Nie in staat om hardeskyf te demonteer nie.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Nie in staat om die CDROM/floppie te demonteer nie.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Nie in staat om die  CDROM uit te laat nie.",
/* TR_CONGRATULATIONS */
"Geluk!",
/* TR_CONGRATULATIONS_LONG */
"SmoothWall is suksesvol geïnstalleer. Verwyder asb enige floppie skywe of " \
"CDROMs in die rekenaar. 'Setup' sal nou loop waar jy ISDN, netwerk kaarte en " \
"stelsel wagwoorde kan opstel. Nadat die opstelling voltooi is moet jy jou " \
"webbladleser mik na http://smoothwall:81 of https://smoothwall:445 (of wat " \
"ookal jou SmoothWall se naam is), en dan die inbel netwerk (indien benodig) " \
"en afgeleë toegang opstel. Onthou om 'n wagwoord vir die SmoothWall 'dial' " \
"gebruiker op te stel, indien jy gebruikers anders as die SmoothWall 'admin' " \
"gebruiker wil toelaat om die inbel skakel te beheer.",
/* TR_PRESS_OK_TO_REBOOT */
"Druk Ok onm die majien te laat oorbegin ('reboot').",
/* TR_ERROR */
"Fout",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"Bereken module afhanklikhede...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Nie in staat om module afhanklikhede te bepaal nie.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Selekteer die CDROM tipe",
/* TR_SELECT_CDROM_TYPE_LONG */
"Geen IDE CDROM is op die masjien gevind nie.  Selekteer asb watter van die volgende " \
"drywers jy wil gebruik om SmoothWall in staat te stel om jou CDROM te gebruik.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(stel %x voor)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(stel %d voor)",
/* TR_CONFIGURE_THE_CDROM */
"Konfigureer die CDROM deur die toepaslike IO adres en/of IRQ te selekteer.",
/* TR_INVALID_IO (note extra space) */
"Die gespesifiseerde IO poort details is ongeldig. ",
/* TR_INVALID_IRQ */
"Die gespesifiseerde IRQ details is ongeldig.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Nie in staat om /var/smoothwall/main/settings te skryf nie.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Nie in staat om /var/smoothwall/ethernet/settings te skryf nie.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Nie in staat om symlink /dev/harddisk te skep nie.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Nie in staat om symlink /dev/harddisk1 te skep nie.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Nie in staat om /dev/harddisk2 te skep nie.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Nie in staat om symlink /dev/harddisk3 te skep nie.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Nie in staat om symlink /dev/harddisk4 te skep nie.",

/* net.c */
/* TR_DOWNLOADING */
"Besig om af te laai...",
/* TR_FAILED_TO_DOWNLOAD */
"Aflaai get gefaal.",
/* TR_ENTER_URL */
"Verskaf die URL na die SmoothWall tar.gz lêer. " \
"WAARSKUWING: DNS nie beskikbaar!  Die URL moet met 'smoothwall.tgz' eindig.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Netwerk konfigurasie",
/* TR_CONFIGURE_NETWORKING_LONG */
"Jy behoort nou die netwerk te konfigureer deur eerstens die korrekte drywer te laai vir " \
"die GROEN koppelvlak. Jy kan dit doen of deur te poog om dit outomaties te vind, " \
"of deur die korrekte drywer van die lys te selekteer. Let daarop dat, indien jy meer as " \
"een netwerk kaart geïnstaleer het, jy later die ander sal kan konfigureer. " \
"Let ook daarop dat indien jy meer as een kaart van dieselfde tipe as GROEN het, " \
"elk van hulle spesiale module parameters benodig.  Jy moet die parameters vir al die " \
"kaarte nou verskaf sodat almal aktief word sodra jy die GROEN koppelvlak "  \
"konfigureer.",
/* TR_INTERFACE_FAILED_TO_COME_UP */
"Koppelvlak het gefaal om te aktiveer.",
/* TR_ENTER_NETWORK_DRIVER */
"Kon nie netwerk kaart outomaties opspoor nie. Spesifiseer die drywer en " \
"opsionele parameters vir die netwerk kaart.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Masjiennaam",
/* TR_NETWORKING */
"Netwerk",
/* TR_DHCP_SERVER_CONFIGURATION */
"DHCP bediener konfigurasie",
/* TR_ISDN_CONFIGURATION */
"ISDN konfigurasie",
/* TR_ROOT_PASSWORD */
"\'root\' wagwoord",
/* TR_SETUP_PASSWORD */
"\'setup\' wagwoord",
/* TR_ADMIN_PASSWORD */
"Admin wagwoord",
/* TR_SECTION_MENU */
"Seksie kieslys",
/* TR_SELECT_THE_ITEM */
"Selekteer die item wat jy wil konfigureer.",
/* TR_SETUP_FINISHED */
"Opstelling is afgehandel.  Druk Ok om masjien te laat oorbegin ('reboot').",
/* TR_SETUP_NOT_COMPLETE */
"Aanvanklike opstelling was nie heeltemal voltooi nie.  Jy moet verseker dat die opstelling " \
"behoorlik voltooi word deur Setup in die konsole te loop.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Tik die 'root' gebruiker se wagwoord. Teken aan as die gebruiker vir bevelslyn toegang.",
/* TR_SETTING_ROOT_PASSWORD */
"Besig om 'root' se wagwoord te stel....",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Probleem met stel van 'root' wagwoord.",
/* TR_ENTER_SETUP_PASSWORD */
"Tik die 'setup' gebruiker se wagwoord. Teken aan as die gebruiker om die setup " \
"te loop.",
/* TR_SETTING_SETUP_PASSWORD */
"Besig om 'setup' se wagwoord te stel....",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Probleem met stel van 'setup' wagwoord.",
/* TR_ENTER_ADMIN_PASSWORD */
"Tik die SmoothWall admin wagwoord.  Teken as die gebruiker aan " \
"op die SmoothWall web admin bladsye.",
/* TR_SETTING_ADMIN_PASSWORD */
"Besig om SmoothWall admin se wagwoord te stel....",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Probleme met stel van SmoothWall admin wagwoord.",
/* TR_PASSWORD_PROMPT */
"Wagwoord:",
/* TR_AGAIN_PROMPT */
"Herhaal:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"Wagwoord kan nie blanko wees nie.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"Wagwoorde is nie dieselfde nie.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Tik die masjiennaam.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"Masjiennaam moet gespesifiseer word.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"Masjiennaam mag nie spasies bevat nie.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Nie in staat om /etc/hostname te skryf nie",

/* isdn.c */
/* TR_GERMAN_1TR6 */
"German 1TR6",
/* TR_EURO_EDSS1 */
"Euro (EDSS1)",
/* TR_LEASED_LINE */
"Gehuurde ('Leased') lyn",
/* TR_US_NI1 */
"US NI1",
/* TR_PROTOCOL_COUNTRY */
"Protokol/Land",
/* TR_ISDN_CARD */
"ISDN kaart",
/* TR_MSN_CONFIGURATION */
"Lokale telefoonnommer (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Stel addisionele module parameters",
/* TR_RED_IN_USE */
"ISDN (of ander eksterne konneksie) is tans in gebruik.  Jy kan nie " \
"ISDN opstel terwyl die ROOI kopplevlak aktief is nie.",
/* TR_ISDN_CONFIGURATION_MENU */
"ISDN konfigurasie kieslys",
/* TR_ISDN_STATUS */
"Huidige ISDN konfigurasie %s.\n\n" \
"   Protokol: %s\n" \
"   Kaart: %s\n" \
"   Lokale telefoonnommer: %s\n\n" \
"Selekteer die item wat jy wil konfigureer, of kies om die huidige konfigurasie te gebruik.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"ISDN is nog nie opgestel nie. Selekteer die item wat jy wil konfigureer.",
/* TR_ENABLE_ISDN */
"Aktiveer ISDN",
/* TR_DISABLE_ISDN */
"De-aktiveer ISDN",
/* TR_INITIALISING_ISDN */
"Besig om ISDN te inisieër...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Nie in staat om ISDN te inisieër nie.",
/* TR_ISDN_NOT_SETUP */
"ISDN is nie opgestel nie. Sommige item is nie geselekteer nie.",
/* TR_ISDN_PROTOCOL_SELECTION */
"ISDN protokol seleksie",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Selekteer die ISDN protokol soos benodig.",
/* TR_AUTODETECT */
"* AUTO-OPSPOOR *",
/* TR_ISDN_CARD_SELECTION */
"ISDN kaart seleksie",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Selekteer die ISDN kaart soos geïnstalleer.",
/* TR_CHECKING_FOR */
"Op soek na: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"ISDN kaart kon nie opgespoor word nie. Dit mag nodig wees om addisionele module " \
"parameters te verskaf indien die kaart 'n ISA tipe is, of indien dit spesiale opstelling benodig.",
/* TR_DETECTED */
"'n %s is opgespoor",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"Nie in staat om 'n ISDN kaart in die rekenaar te vind nie. Dit mag nodig wees om " \
"addisionele module parameters te verskaf indien die kaart 'n ISA tipe is, of indien dit spesiale " \
"opstelling benodig.",
/* TR_ENTER_THE_LOCAL_MSN */
"Tik die lokale telefoonnommer in (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"Telefoonnommer moet verskaf word.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Sommige ISDN kaarte (veral ISA tipes) mag moontlik addisionele module " \
"parameters vir IRQ en IO adres inligting benodig. Indien jy so 'n " \
"ISDN kaart het, verskaf die ekstra parameters hier. Bv: " \
"\"io=0x280 irq=9\". Dit sal gebruik word vir kaart deteksie.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Besig om netwerk af te skakel...",
/* TR_PULLING_NETWORK_UP */
"Besig om netwerk aan te skakel...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Netwerk konfigurasie tipe",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Drywers en kaart toewysings",
/* TR_ADDRESS_SETTINGS */
"Adres details",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"DNS en 'Gateway' details",
/* TR_RESTART_REQUIRED */
"\n\nSodra konfigurasie voltooi is, sal dit nodig wees om die netwerk oor te laat begin.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Huidige konfigurasie: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Netwerk konfigurasie kieslys",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Selekteer die netwerk konfigurasie vir SmoothWall.  Die volgende " \
"konfigurasietipes lys die koppelvlakke waarop ethernet beskikbaar is. " \
"Indien jy die opsie verander, sal dit nodig wees om die netwerk weer oor te laat begin, en jy " \
"sal die netwerk drywer toewysings weer moet konfigureer.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Besig om die lokale netwerk af te skakel...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"Jou konfigurasie is vir 'n enkele GROEN koppelvlak, " \
"wat reeds 'n toegewysde drywer het.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Konfigureer netwerk drywers, en watter koppelvlak aan elke kaart " \
"toegwys is.  Die huidige konfigurasie is as volg:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nWil jy die opstellings verander/?",
/* TR_UNCLAIMED_DRIVER */
"Daar is 'n on-toegewysde kaart van tipe:\n%s\n\n" \
"Jy kan dit toeken aan:",
/* TR_CARD_ASSIGNMENT */
"Kaart toewysing",
/* TR_PROBE */
"Spoor Op",
/* TR_SELECT */
"Selekteer",
/* TR_NO_UNALLOCATED_CARDS */
"Geen on-toegewysde kaarte meer nie, " \
"meer word benodig. Jy mag dit automaties probeer opspoor, of " \
"'n drywer van die lys selekteer.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Nie in staat om addiosionele kaarte te vind nie.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Alle kaarte is suksesvol toegewys.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Nie genoeg kaarte is toegewys nie.",
/* TR_MANUAL */
"* MET HAND *",
/* TR_SELECT_NETWORK_DRIVER */
"Selekteer netwerk drywer",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Selekteer netwerk drywer wat in die masjien geïnstalleer is. " \
"Indien jy MET HAND selekteer, sal jy die geleentheid kry om die " \
"naam van die drywer module te voorsien, asook parameters vir drywers " \
"met spesiale behoeftes, soos bv ISA kaarte.",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"nie in staat om drywer module te laai nie.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"Die drywer module is reeds gelaai.",
/* TR_MODULE_PARAMETERS */
"Verskaf die module naam en parameters vir die drywer wat jy benodig.",
/* TR_LOADING_MODULE */
"besig om module te laai...",
/* TR_WARNING */
"WAARSKUWING",
/* TR_WARNING_LONG */
"Indien jy die IP verander en jy is via die netwerk aangeteken, " \
"sal jou konneksie met SmoothWall verbreek word en jy sal weer " \
"met die nuwe IP moet konnekteer. Hierdie is 'n riskante operasie en moet " \
"slegs gedoen word indien daar geen fisiese toegang tot die masjien beskikbaar is nie, " \
"ingeval probleme opduik.",
/* TR_SINGLE_GREEN */
"Jou konfigurasie spesifiseer 'n enkele GROEN koppelvlak.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Selekteer die koppelvlak wat jy wil konfigureer.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"Jou konfigurasie benut nie die ROOI ethernet koppelvlak nie. " \
"DNS en 'Gateway' inligting vir inbel gebruikers " \
"word outomaties gedoen wanneer ingeskakel word.",
/* TR_PRIMARY_DNS */
"Primêre DNS:",
/* TR_SECONDARY_DNS */
"Sekondêre DNS:",
/* TR_DEFAULT_GATEWAY */
"Verstek 'Gateway':",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Verskaf die DNS en 'gateway' inligting. Die waardes word slegs bebruik indien DHCP " \
"nie aktief op die ROOI koppelvlak is nie.",
/* TR_PRIMARY_DNS_CR */
"Primêre DNS\n",
/* TR_SECONDARY_DNS_CR */
"Sekondêre DNS\n",
/* TR_DEFAULT_GATEWAY_CR */
"Verstek 'Gateway'\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"Sekondêre DNS gespesifiseer sonder Primêre DNS",
/* TR_UNKNOWN */
"ONBEKEND",
/* TR_NO_ORANGE_INTERFACE */
"Geen ORANJE koppelvlak is tegewys nie.",
/* TR_MISSING_ORANGE_IP */
"Geen IP inligting vir ORANJE koppelvlak nie.",
/* TR_NO_RED_INTERFACE */
"Geen ROOI koppelvlak is tegewys nie.",
/* TR_MISSING_RED_IP */
"Geen IP inligting vir ROOI koppelvlak nie.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Begin adres:",
/* TR_END_ADDRESS */
"Eind adres:",
/* TR_DEFAULT_LEASE */
"Verstek bruiktyd (min):",
/* TR_MAX_LEASE */
"Maks bruiktyd (min):",
/* TR_DOMAIN_NAME_SUFFIX */
"Domeinstert:",
/* TR_CONFIGURE_DHCP */
"Konfigureer die DHCP bediener deur die opsie inligting te verskaf.",
/* TR_START_ADDRESS_CR */
"Begin adres\n",
/* TR_END_ADDRESS_CR */
"Eind adres\n",
/* TR_DEFAULT_LEASE_CR */
"Verstek bruiktyd\n",
/* TR_MAX_LEASE_CR */
"Maks bruiktyd\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Domeinstert\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Sleutelbord afbeelding",
/* TR_KEYBOARD_MAPPING_LONG */
"Selekteer die sleutelbordtipe van die lys hieronder.",

/* timezone.c */
/* TR_TIMEZONE */
"Tydsone",
/* TR_TIMEZONE_LONG */
"Selekteer jou tydsone van die lys hieronder.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Selekteer die USB beheerder",
/* TR_USBADSL_STATUS */
"USB ADSL is tans: %s\n" \
"   Beheerder: %s\n\n" \
"Selekteer die item wat jy wil konfigureer, of kies om die huidige konfigurasie te gebruik.",
/* TR_USBADSL_CONFIGURATION */
"USB ADSL konfigurasie",
/* TR_ENABLE_USBADSL */
"Aktiveer USB ADSL",
/* TR_DISABLE_USBADSL */
"De-aktiveer USB ADSL",
/* TR_INITIALISING_USBADSL */
"Besig om USB ADSL te inisiëer.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"Nie in staat om USB ADSL te inisiëer nie",
/* TR_USBADSL_NOT_SETUP */
"USB ADSL nie opgestel nie.",
/* TR_USB_CONTROLLER_SELECTION */
"USB beheerder seleksie",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Selekteer die USB beheerder wat in die SmoothWall masien geïnstalleer is.",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"USB beheerder nie opgespoor nie.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"Nie in staat om 'n USB beheerder op te spoor nie.",
/* TR_STARTING_UP_USBADSL */
"Besig om USB ADSL te aktiveer..."

};
