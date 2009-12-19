/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * -= Swedish translation team =-
 * Fredrik Johansson	<frejo@home.se>
 * Francis j. Morris	<franman@visto.com>
 * Thomas Persson	<thomas.persson@mbox2.swipnet.se> 
 * Torulf Wiberg" 	<torulf@dcab.se> 
 * Oden Eriksson	<oden.eriksson@kvikkjokk.net> 
 * Hans Laakso		<Hans.Laakso@evoxrifa.com>
 *
 * filename: swedish.c
 * Contains swedish strings. */

#include "libsmooth.h"

char *swedish_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Ångra",
/* TR_INSTALLATION_CANCELED */
"Installation avbruten.",
/* TR_HELPLINE */
"            <Tab>/<Alt-Tab> mellan elements   |  <Mellanslag> väljer",
/* TR_QUIT */
"Lämna",
/* TR_DISABLED */
"Avaktiverad",
/* TR_ENABLED */
"Aktiverad",
/* TR_UNSET */
"UNSET",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Kan inte öppna inställnings filen",
/* TR_DONE */
"Done",
/* TR_PROBE_FAILED */
"Auto detecting misslyckades.",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"IP-address:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Nätverksaddress:",
/* TR_NETMASK_PROMPT */
"Nätmask:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Ange IP-addressinformation",
/* TR_INVALID_FIELDS */
"Följande fält är felaktiga:\n\n",
/* TR_IP_ADDRESS_CR */
"IP-address\n",
/* TR_NETWORK_ADDRESS_CR */
"Nätverksaddress\n",
/* TR_NETWORK_MASK_CR */
"Nätmask\n",
/* TR_INTERFACE (%s is interface name) */
"%s nätverksgränssnitt",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Fyll i IP-address information för varje %s nätverksgränssnitt.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Söker efter: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall har hittat föjande Nätverkskort i din maskin: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"Modulnamnet kan inte vara blankt.",
/* TR_STATIC */
"Statisk",
/* TR_DHCP_HOSTNAME */
"DHCP-värdnamn:",
/* TR_DHCP_HOSTNAME_CR */
"DHCP-värdnamn\n",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Kan inte skriva till /var/smoothwall/main/hostname.conf",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Kan inte skriva till /etc/hosts.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"Kan inte skriva till /etc/hosts.deny.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Kan inte skriva till /etc/hosts.allow.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Kan inte ställa in värdnamn.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Wälkommen till SmoothWall installationsprogram. Besök gärna vår " \
"hemsida på http://www.smoothwall.org. Om du väljer Avbryt på någon " \
"av följande fönster kommer datorn att startas om.",
/* TR_NO_IDE_HARDDISK */
"Ingen IDE-hårddisk hittad.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Välj installations media",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"SmoothWall kan installeras från olika media.   Enklast är att " \
"använda Datorns CDROM läsare. Om datorn inte har detta, kan du alltid " \
"installera via en annan Dator på ditt lokala Nätverk där installations filerna " \
"finns via HTTP. I detta fall behöver du Smoothwall driver diskett.",
/* TR_NO_IDE_CDROM */
"Ingen IDE-cdrom hittad.",
/* TR_INSERT_CDROM */
"Stoppa in SmoothWall CD:n i CDROM läsaren.",
/* TR_INSERT_FLOPPY */
"Stoppa in SmoothWall driver diskett i diskettläsaren.",
/* TR_PREPARE_HARDDISK */
"Installationsprogramet kommer nu att preparera din IDE hårddisk på %s. " \
"Först kommer disken att bli partitionerad, så att installationsprogrammet " \
"kan kopiera filerna till hårddisken.",
/* TR_PARTITIONING_DISK */
"Skapar partition på hårddisken...",
/* TR_UNABLE_TO_PARTITION */
"Kan inte skapa partition på hårddisken.",
/* TR_MAKING_SWAPSPACE */
"Skapar swaputrymme...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Kan inte skapa swaputrymme.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Skapar root filsystem...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Kan inte skapa rootfilsystem.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Monterar rootfilsystem...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Kan inte montera rootfilsystem.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Skapar bootfilsystem...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Kan inte skapa bootfilsystem.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Monterar bootfilsystem...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Kan inte montera bootfilsystem.",
/* TR_MAKING_LOG_FILESYSTEM */
"Skapar loggfiler...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Kan inte skapa loggfilsystemet.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Monterar loggfilsystemet...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Kan inte montera loggfilsystemet.",
/* TR_MOUNTING_SWAP_PARTITION */
"Monterar swappartitionen...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Kan inte montera swappartitionen.",
/* TR_NETWORK_SETUP_FAILED */
"Nätverksinställningarna är inte godkända.",
/* TR_NO_TARBALL_DOWNLOADED */
"Ingen tarball nerladdad.",
/* TR_INSTALLING_FILES */
"Installerar filer...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Kan inte installera filer.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Kan inte ta bort temporära TEMP-filer.",
/* TR_ERROR_WRITING_CONFIG */
"Fel vid skrivning till konfigurationsinformation.",
/* TR_INSTALLING_LILO */
"Installarerar LILO...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Kan inte installarera LILO.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Kan inte montera hårddisk.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Kan inte avmontera CDROM/floppydisk.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Kan inte mata ut CD.",
/* TR_CONGRATULATIONS */
"Grattis!",
/* TR_CONGRATULATIONS_LONG */
"SmoothWall har lyckats med installationen. Ta bort ev floppydisketter eller " \
"CD-skivor ur datorn. Konfigurationsprogrammet kommer att fortsätta att konfigurera ISDN, " \
"nätverkskort och system lösenord. När installationen är " \
"klar skall du peka din webläsare på http://smoothwall:81 eller " \
"https://smoothwall:445 (dvs det namnet du gav SmoothWall), där kan du ställa in " \
"modem uppkoppling (om så behövs) fjärraccess. Kom ihåg att ställa in " \
"lösenord för SmoothWall modemuppkopplingsanvändare, om du vill förhindra att icke SmoothWall " \
"'admin' användare ska kunna ta kontroll över länken.",
/* TR_PRESS_OK_TO_REBOOT */
"Tryck Ok för att starta om.",
/* TR_ERROR */
"Fel",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"Kalkylerar modul beroenden...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Kan inte kalkylera modul beroenden.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Välj CDROM-model",
/* TR_SELECT_CDROM_TYPE_LONG */
"Ingen IDE CDROM hittad i Datorn.   Välj vilken av " \
"följande drivrutiner du vill använda så SmoothWall kan använda CD läsaren.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(letar %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(letar %d)",
/* TR_CONFIGURE_THE_CDROM */
"Ställ in CD-läsaren med att välja rätt IO address och/eller IRQ.",
/* TR_INVALID_IO (note extra space) */
"Felaktigt värde på IO-port!!. ",
/* TR_INVALID_IRQ */
"Felaktigt värde på IRQ!!.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Kan inte skriva /var/smoothwall/main/settings.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Kan inte skriva /var/smoothwall/ethernet/settings.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Kan inte skapa symlink /dev/harddisk.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Kan inte skapa symlink /dev/harddisk1.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Kan inte skapa symlink /dev/harddisk2.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Kan inte skapa symlink /dev/harddisk3.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Kan inte skapa symlink /dev/harddisk4.",

/* net.c */
/* TR_DOWNLOADING */
"Laddar ned...",
/* TR_FAILED_TO_DOWNLOAD */
"fel vid nerladdning.",
/* TR_ENTER_URL */
"Skriv URL till SmoothWall tar.gz filen. " \
"VARNING: DNS är inte tillgänlig!  URL:n ska alltid sluta med 'smoothwall.tgz'.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Nätverksinställning",
/* TR_CONFIGURE_NETWORKING_LONG */
"Du skall nu ställa in nätverket med att först ladda rätt drivrutin för " \
"GRöNT nätverkskort. Du kan göra detta genom att antingen låta systemet leta upp" \
"nätverkskortet, eller så väljer du själv rätt drivrutin från en lista. Notera att om du har " \
"mera än ett nätverkskort installerat, så kan du alltid konfigurera dessa " \
"senare i installationen. Notera ochså att om du har flera nätverkskort " \
"med samma model som GRöNT så krävs speciella" \
"parameterar, du måste alltid ändra alla parameterar för nätverkskortet så att "  \
"alla nätverkskort kan aktiveras när du ställer in GRÖNA sidan.",
/* TR_INTERFACE_FAILED_TO_COME_UP */
"Nätverket kan inte initieras.",
/* TR_ENTER_NETWORK_DRIVER */
"Hittar inte nätverkskort automatiskt ställ in rätt drivrutin och " \
"använd specifika parameterar för nätverkskort.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Datornamn",
/* TR_NETWORKING */
"Nätverk",
/* TR_DHCP_SERVER_CONFIGURATION */
"DHCP-server inställningar",
/* TR_ISDN_CONFIGURATION */
"ISDN-inställningar",
/* TR_ROOT_PASSWORD */
"\'root\' lösenord",
/* TR_SETUP_lösenord */
"\'setup\' password",
/* TR_ADMIN_PASSWORD */
"Admin lösenord",
/* TR_SECTION_MENU */
"Huvudmeny",
/* TR_SELECT_THE_ITEM */
"Välj vad du vill konfigurera.",
/* TR_SETUP_FINISHED */
"Konfigureringen är färdig.  Tryck på Ok för omstart.",
/* TR_SETUP_NOT_COMPLETE */
"Installationen slutfördes inte korrekt.  Du måste försäkra dig om att installationen blev " \
"korrekt slutförd. Kör setup igen via fjärrkonsol.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Välj ett'root' lösenord. Du kan logga in som denna användare för access via kommandotolk.",
/* TR_SETTING_ROOT_PASSWORD */
"Sparar  'root' lösenord....",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Kan inte spara 'root' lösenord.",
/* TR_ENTER_SETUP_PASSWORD */
"Skriv in 'setup' användarens lösenord. Logga in som denna användare för att komma " \
"till konfigureringsprogrammet programmet.",
/* TR_SETTING_SETUP_PASSWORD */
"Sparar 'setup' lösenord....",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Kan inte spara 'setup' lösenord.",
/* TR_ENTER_ADMIN_PASSWORD */
"Skriv in SmoothWall administratör lösenord.Denna användare är till för " \
"inlogging som SmoothWalls webbadministratör.",
/* TR_SETTING_ADMIN_PASSWORD */
"Sparar SmoothWalls administratörlösenord....",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Kan inte spara SmoothWall administratör lösenord.",
/* TR_PASSWORD_PROMPT */
"lösenord:",
/* TR_AGAIN_PROMPT */
"Igen:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"Lösenordet kan inte vara tomt.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"Lösenorden matchar inte varandra.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Skriv in datornamn.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"Datornamn kan inte vara tomt.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"Datornamn får inte innehålla blanksteg.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Kan inte skriva datornamn till /etc/hostname",

/* isdn.c */
/* TR_GERMAN_1TR6 */
"German 1TR6",
/* TR_EURO_EDSS1 */
"Euro (EDSS1)",
/* TR_LEASED_LINE */
"Leased line",
/* TR_US_NI1 */
"US NI1",
/* TR_PROTOCOL_COUNTRY */
"Protokoll/Land",
/* TR_ISDN_CARD */
"ISDN-kort",
/* TR_MSN_CONFIGURATION */
"Eget telefonnummer (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Sätt specifika modulparameterar",
/* TR_RED_IN_USE */
"ISDN (eller annan externt koppling) används.  Du kan inte " \
"konfigurera ISDN när det RÖDA nätet är aktivt.",
/* TR_ISDN_CONFIGURATION_MENU */
"ISDN-inställningar",
/* TR_ISDN_STATUS */
"ISDN Status %s.\n\n" \
"   Protocol: %s\n" \
"   Kort: %s\n" \
"   Lokalt telenummer: %s\n\n" \
"Välj vilket du vill omkonfigurera , eller välj nuvarande inställningar.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"ISDN har inte blivit konfigurerat. Välj vilket du vill konfigurera.",
/* TR_ENABLE_ISDN */
"Aktiverar ISDN",
/* TR_DISABLE_ISDN */
"Deaktiverar ISDN",
/* TR_INITIALISING_ISDN */
"Initierar ISDN...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Kan inte initiera ISDN.",
/* TR_ISDN_NOT_SETUP */
"ISDN setup kan ej spara. Vissa fält ej markerade.",
/* TR_ISDN_PROTOCOL_SELECTION */
"ISDN protocol selection",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Välj vilket ISDN-protokoll du behöver.",
/* TR_AUTODETECT */
"* Automatisk *",
/* TR_ISDN_CARD_SELECTION */
"ISDN card selection",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Välj vilket ISDN-kort du har.",
/* TR_CHECKING_FOR */
"Söker efter: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"ISDN-kort ej hittat. Du behöver välja drivrutin manuellt " \
"Om det är ett kort med en ISA-buss behöver du välja IO-adress IRQ .",
/* TR_DETECTED */
"Funnet : %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"Kan inte hitta något ISDN-kort i denna dator. Du måste välja rätt drivrutin " \
"Om det är ett kort med en ISA-buss behöver du manuellt välja IO-adress IRQ " \
"specifika inställningar.",
/* TR_ENTER_THE_LOCAL_MSN */
"Skriv in ditt lokala telefonnummer (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"Telefonnummer kan inte vara blankt.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Vissa ISDN-kort (speciellt ISA-kort) måste du manuellt välja " \
"specifika parameterar för IRQ och IO-adress." \
"skriv dessa extra parameterar här. Till exempel: " \
"\"io=0x280 irq=9\". Dessa kommer att användas för hitta ditt ISDN-kort.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Deaktiverar nätverket...",
/* TR_PULLING_NETWORK_UP */
"Aktiverar nätverket...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Nätverkskonfigurationstyp",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Drivrutin och kortinställningar",
/* TR_ADDRESS_SETTINGS */
"Adressinställningar",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"DNS och Gateway-inställningar",
/* TR_RESTART_REQUIRED */
"\n\n När konfigurationen är klar, kommer nätverket att starta om.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Nuvarande konfiguration: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Nätverkskonfiguration",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Välj vilken nätverkskonfiguration för SmoothWall.  Följande " \
"visar vilket nätverksgränssnitt som är tilldelad till respektive nätverkskort. " \
"Om du ändrar denna inställning, kommer nätverket att starta om," \
"då måste du tilldela om nätverksgränssnitt till respektive nätverkskort.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Deaktiverar lokalt nätverk...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"Din konfiguration är inställd för enkelt GRÖNT nätverksgränssnitt, " \
"som redan har en drivrutin tilldelad.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Tilldela nätverksdrivutiner, till vilket nätverksgränssnitt för varje nätverkskort " \
"du vill tilldela.  Nuvarande konfiguration :\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nDo vill du ändra dessa inställningar?",
/* TR_UNCLAIMED_DRIVER */
"Det finns ej tilldelade nätverkskort av typen:\n%s\n\n" \
"Du kan tilldelad detta till:",
/* TR_CARD_ASSIGNMENT */
"Nätverkskorttilldelning",
/* TR_PROBE */
"Undersöker",
/* TR_SELECT */
"Välj",
/* TR_NO_UNALLOCATED_CARDS */
"Inga ej tilldelade nätverkskort kvar, " \
"fler behövs. Du kan låta systemet söka efter flera nätverkskort, eller " \
"välj manuellt en drivrutin från listan.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Hittar inte lämplig drivrutin.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Alla nätverkskort hittade .",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Inte tillräckligt med nätverkskort hittade.",
/* TR_MANUAL */
"* MANUELLT *",
/* TR_SELECT_NETWORK_DRIVER */
"Välj nätverksdrivrutin",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Välj drivrutin för nätverkskortet du har i datorn. " \
"Om du väljer MANUELLT, har du möjlighet att välja " \
"drivrutin för ditt nätverkskort,samt specifika parametrar " \
" typ IO-adress IRQ.",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Kan inte ladda drivrutin.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"Denna drivrutin är redan laddad.",
/* TR_MODULE_PARAMETERS */
"Skriv in modul(drivrutin) namnet och specifika parametrar för din drivrutin behöver.",
/* TR_LOADING_MODULE */
"Ladda modulen...",
/* TR_WARNING */
"WARNING",
/* TR_WARNING_LONG */
"Om du ändrar denna IP-adress, och är inloggad via fjärrkonsolen, " \
"så kommer du att tappa kontakten till SmoothWall, " \
"du måste du åter igen ta kontakt med nya IP-adressen.Detta kan vara en riskabel åtgärd om något går fel, " \
"Man bör göra detta enbart om fysiskt befinner sig vid datorn.",
/* TR_SINGLE_GREEN */
"Din konfiguration är inställd för enkelt GRÖNT nätverksgränssnitt.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Välj vilket nätverksgränssnitt du vill omkonfigurera.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"Din konfiguration kan inte dra nytta av nätverkskortet för " \
"sitt RÖDA gränssnitt. DNS och Gateway-information för uppringda användare " \
"konfigureras automatiskt vid uppringning.",
/* TR_PRIMARY_DNS */
"Primär DNS:",
/* TR_SECONDARY_DNS */
"Sekundär DNS:",
/* TR_DEFAULT_GATEWAY */
"Standard Gateway (IP):",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Skriv DNS och standardgateway. Dessa inställningar behövs om DHCP " \
"är avaktiverad på det RÖDA nätverksgränssnittet.",
/* TR_PRIMARY_DNS_CR */
"Primär DNS\n",
/* TR_SECONDARY_DNS_CR */
"Sekundär DNS\n",
/* TR_DEFAULT_GATEWAY_CR */
"Standard Gateway\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"Sekundär DNS specificerad utan Primär DNS",
/* TR_UNKNOWN */
"Okänt",
/* TR_NO_ORANGE_INTERFACE */
"Inget ORANGE nätverksgränssnitt tilldelat.",
/* TR_MISSING_ORANGE_IP */
"Ingen IP-information på ORANGE nätverksgränssnitt.",
/* TR_NO_RED_INTERFACE */
"Inget RÖTT nätverksgränssnitt tilldelat.",
/* TR_MISSING_RED_IP */
"Ingen IP-information på RÖTT nätverksgränssnitt.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Startadress:",
/* TR_END_ADDRESS */
"Slutadress:",
/* TR_DEFAULT_LEASE */
"Standardleasetid (mins):",
/* TR_MAX_LEASE */
"Max leasetid (mins):",
/* TR_DOMAIN_NAME_SUFFIX */
"Domän:",
/* TR_CONFIGURE_DHCP */
"Konfigurera DHCP-server genom att skriva in parameterinformation.",
/* TR_START_ADDRESS_CR */
"Startadress\n",
/* TR_END_ADDRESS_CR */
"Slutadress\n",
/* TR_DEFAULT_LEASE_CR */
"Standardleasetid\n",
/* TR_MAX_LEASE_CR */
"Max leasetid\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Domän \n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Tangentbord",
/* TR_KEYBOARD_MAPPING_LONG */
"Välj vilket tangentbord du använder från listan nedan.",

/* timezone.c */
/* TR_TIMEZONE */
"Tidszon",
/* TR_TIMEZONE_LONG */
"Välj vilken tidszon du befinner dig i från listan nedan.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Välj vilken USB kontroller",
/* TR_USBADSL_STATUS */
"USB ADSL är följande: %s\n" \
"   kontroller: %s\n\n" \
"Välj vilken du vill omkonfigurera, eller välj nuvarande inställningar.",
/* TR_USBADSL_CONFIGURATION */
"USB ADSL konfiguration",
/* TR_ENABLE_USBADSL */
"Aktivera USB ADSL",
/* TR_DISABLE_USBADSL */
"Avaktivera USB ADSL",
/* TR_INITIALISING_USBADSL */
"Initierar USB ADSL.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"Kan inte initiera USB ADSL",
/* TR_USBADSL_NOT_SETUP */
"USB ADSL inte konfigurerat.",
/* TR_USB_CONTROLLER_SELECTION */
"USB kontroller val",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Välj vilken USB kontroller som är installarerad i din SmoothWall-dator.",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"USB kontroller inte hittad.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"Hittar inte någon USB kontroller.",
/* TR_STARTING_UP_USBADSL */
"Startar upp USB ADSL..."

};
