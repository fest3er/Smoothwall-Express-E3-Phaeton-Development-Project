/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * (c) Norweigian Translation Team:
 * Asbjørn Høiland Aarrestad
 * Tore Høiland Aarrestad.
 *
 * filename: norwegian.c
 * Contains norwegian strings. */

#include "libsmooth.h"

char *norwegian_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Avbryt",
/* TR_INSTALLATION_CANCELED */
"Installasjonen er avbrutt.",
/* TR_HELPLINE */
"              <Tab>/<Alt-Tab> mellom elementene   |  <Space> velger",
/* TR_QUIT */
"Avslutt",
/* TR_DISABLED */
"Deaktivert",
/* TR_ENABLED */
"aktivert",
/* TR_UNSET */
"IKKE SATT",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Kan ikke åpne instillings filen",
/* TR_DONE */
"Ferdig",
/* TR_PROBE_FAILED */
"Automatisk oppdaging feilet.",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"IP adresse:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Nettverks adresse:",
/* TR_NETMASK_PROMPT */
"Nettverks maske:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Skriv inn IP adresse informasjon",
/* TR_INVALID_FIELDS */
"De følgende feltene er ikke gyldige:\n\n",
/* TR_IP_ADDRESS_CR */
"IP adresse\n",
/* TR_NETWORK_ADDRESS_CR */
"Nettverks adresse\n",
/* TR_NETWORK_MASK_CR */
"Nettverk maske\n",
/* TR_INTERFACE (%s is interface name) */
"%s port",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Skriv IP adresse informasjon for %s porten.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Leter etter: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall har funnet følgende nettverkskort i maskinen din: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"Modul navnet kan ikke være blankt.",
/* TR_STATIC */
"Statisk",
/* TR_DHCP_HOSTNAME */
"DHCP Vertsnavn:",
/* TR_DHCP_HOSTNAME_CR */
"DHCP Vertsnavn\n",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Kan ikke skrive til /var/smoothwall/main/hostname.conf",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Kan ikke skrive til /etc/hosts.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"Kan ikke skrive til /etc/hosts.deny.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Kan ikke skrive til /etc/hosts.allow.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Kan ikke sette vertsnavn.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Velkommen til SmoothWall installasjonsprogrammet. Vennligst"\
"besøk våre hjemmesider på http://www.smoothwall.org. Velg"\
"Hvis du trykker Avbryt på de følgende skjermene, vil maksinen"\
"bli startet på ny",
/* TR_NO_IDE_HARDDISK */
"Fant ingen IDE harddisk.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Velg installasjons medium",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"SmoothWall kan bli installert fra flere kilder. Det enkleste er å bruke "\
"maskinens CDROM driver. Hvis datamaskinen ikke har dette, kan du "\
"installere via en annen maskin på lokalnettet som har installasjonsfilene "\
"tilgjengelige via HTTP. I dette tilfellet vil du bli spurt om å sette i disketten "\
"med nettverk drivere",
/* TR_NO_IDE_CDROM */
"Fant ingen IDE CDROM.",
/* TR_INSERT_CDROM */
"Vennligst sett din SmoothWall CD i CDROM spilleren.",
/* TR_INSERT_FLOPPY */
"Vennligst sett SmoothWall nettverks driver disketten i diskettstasjonen.",
/* TR_PREPARE_HARDDISK */
"Installasjons programmet vil nå klargjøre IDE disken på %s. "\
"Først vil disken bli partisjonert, og så vil det bli laget filsystemer "\
"på partisjonene.",
/* TR_PARTITIONING_DISK */
"Partisjonerer disken... ",
/* TR_UNABLE_TO_PARTITION */
"Kan ikke partisjonere disken.",
/* TR_MAKING_SWAPSPACE */
"Lager bytte område...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Kan ikke lage bytte område.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Lager rotfilsystem...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Kan ikke lage rotfilsystem.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Monterer rotfilsystemet...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Kan ikke montere rotfilsystemet.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Lager bootfilsystem...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Kan ikke lage bootfilsystem.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Monterer bootfilsystemet...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Kan ikke montere bootfilsystemet.",
/* TR_MAKING_LOG_FILESYSTEM */
"Lager loggfilsystem...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Kan ikke lage loggfilsystem.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Monterer loggfilsystem...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Kan ikke montere loggfilsystemet.",
/* TR_MOUNTING_SWAP_PARTITION */
"Monterer byttepartisjonen...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Kan ikke montere byttepartisjonen.",
/* TR_NETWORK_SETUP_FAILED */
"Nettverks oppsettet feilet.",
/* TR_NO_TARBALL_DOWNLOADED */
"Ingen tar-ball ble lastet ned.",
/* TR_INSTALLING_FILES */
"Installerer filene...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Kan ikke installere filene.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Kan ikke slette midlertidig nedlastede filer.",
/* TR_ERROR_WRITING_CONFIG */
"En feil oppstod når konfigurasjons informasjonen ble skrevet.",
/* TR_INSTALLING_LILO */
"Installerer LILO...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Kan ikke installere LILO.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Kan ikke demontere harddisken.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Kan ikke demontere CDROM/diskett.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Kan ikke åpne CDROM skuffen.",
/* TR_CONGRATULATIONS */
"Gratulerer!",
/* TR_CONGRATULATIONS_LONG */
"SmoothWall ble vellykket installert. Vennligst fjern alle disketter og "\
"CDROM plater fra datamaskinen. Setup programmet vil nå bli kjørt, og du "\
"kan konfigurere ISDN, nettverkskort og systempassord. Etter at setup er ferdig, "\
"kan du fortsette konfigurasjon ved å peke nettleseren til http://smoothwall:81/ "
"eller https://smoothwall:445/ (hvor smoothwall er navnet du gav til din SmoothWall). "\
"Her kan du konfigurer passord for \'dial\' brukeren og mye mer. ",
/* TR_PRESS_OK_TO_REBOOT */
"Trykk ok for å starte på ny.",
/* TR_ERROR */
"Feil",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"Kalkulerer modulavhengigheter...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Kan ikke kalkulere modulavhengigheter.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Velg CDROM type",
/* TR_SELECT_CDROM_TYPE_LONG */
"Ingen IDE CDROM ble funnet på denne maskinen. Vennligst velg hvilken "\
"av de følgende driverne du ønsker å bruke slik at SmoothWall får tilgang til "\
"din CDROM. ",
/* TR_SUGGEST_IO (%x is an IO number) */
"(foreslå %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(foreslå %d)",
/* TR_CONFIGURE_THE_CDROM */
"Konfigurer din CDROM ved å velge riktig IO adresse og/eller IRQ.",
/* TR_INVALID_IO (note extra space) */
"Ikke gyldige IO verdier. ",
/* TR_INVALID_IRQ */
"Ikke gyldige IRQ verdier.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Kan ikke skrive til /var/smoothwall/main/settings.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Kan ikke skrive til /var/smoothwall/ethernet/settings.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Kan ikke lage symbolsk link /dev/harddisk.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Kan ikke lage symbolsk link /dev/harddisk1.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Kan ikke lage symbolsk link /dev/harddisk2.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Kan ikke lage symbolsk link /dev/harddisk3.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Kan ikke lage symbolsk link /dev/harddisk4.",

/* net.c */
/* TR_DOWNLOADING */
"Henter ned...",
/* TR_FAILED_TO_DOWNLOAD */
"Kan ikke hente ned.",
/* TR_ENTER_URL */
"Skriv inn en URL til SmoothWall sin tar.gz fil. "\
"ADVARSEL: DNS er ikke tilgjengelig! Dette bør ende med filen \'Smoohwall.tgz\'.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Konfigurer nettverket",
/* TR_CONFIGURE_NETWORKING_LONG */
"Du bør nå konfigurere nettverket ved først å laste de riktige driverne for "\
"den GRØNNE porten. Du kan gjøre dette enten ved automatisk søking etter et nettverks "\
"kort, eller ved å velge den riktige driveren fra en liste. Merk at hvis du har "\
"mer enn ett nettverkskort installert, vil du få muligheten til å konfigurere de "\
"andre senere i installasjonen. Merk også at hvis du har mer enn ett "\
"kort av samme typen som GRØNN og hvert av kortene krever spesielle modul "\
"parametre, bør du skrive inn parametrene for alle kortene av denne typen slik "\
"at alle kortene kan bli aktivert når du konfigurere den GRØNNE porten. ",
/* TR_INTERFACE_FAILED_TO_COME_UP */
"Porten kom ikke opp.",
/* TR_ENTER_NETWORK_DRIVER */
"Fant ikke nettverkskortet automatisk. Velg en driver og "\
"eventuelle parametre for nettverkskortet.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Vertsnavn",
/* TR_NETWORKING */
"Nettverk",
/* TR_DHCP_SERVER_CONFIGURATION */
"Konfigurasjon av DHCP tjeneren",
/* TR_ISDN_CONFIGURATION */
"ISDN konfigurasjon",
/* TR_ROOT_PASSWORD */
"\'root\' passord",
/* TR_SETUP_PASSWORD */
"\'setup\' passord",
/* TR_ADMIN_PASSWORD */
"Admin passord",
/* TR_SECTION_MENU */
"Seksjons meny",
/* TR_SELECT_THE_ITEM */
"Velg tingen du ønsker å konfigurere.",
/* TR_SETUP_FINISHED */
"Setup er ferdig. Trykk OK for å starte på ny.",
/* TR_SETUP_NOT_COMPLETE */
"Setup ble ikke helt ferdig. Du må være sikker på at Setup er "\
"helt ferdig ved å kjøre setup igjen fra en terminal.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Skriv inn 'root' passordet. Denne brukeren har kommandolinje tilgang.",
/* TR_SETTING_ROOT_PASSWORD */
"Setter 'root' passordet....",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Kan ikke sette 'root' passordet.",
/* TR_ENTER_SETUP_PASSWORD */
"Skriv inn 'setup' passordet. Denne brukeren kjører setup programmet ",
/* TR_SETTING_SETUP_PASSWORD */
"Setter 'setup' passordet....",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Kan ikke sette 'setup' passordet.",
/* TR_ENTER_ADMIN_PASSWORD */
"Skriv inn SmoothWall admin passordet. Denne brukeren "\
"kan logge seg inn på SmoothWall vev administrerings sidene.",
/* TR_SETTING_ADMIN_PASSWORD */
"Setter SmoothWall admin passordet....",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Kan ikke sette SmoothWall admin passordet.",
/* TR_PASSWORD_PROMPT */
"Passord:",
/* TR_AGAIN_PROMPT */
"gjenta:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"Passordet kan ikke være blankt.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"Passordene er ikke like.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Skriv inn maskinens vertsnavn.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"Vertsnavnet kan ikke være blankt.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"Vertsnavnet kan ikke inneholde mellomrom.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Kan ikke skrive til /etc/hostname",

/* isdn.c */
/* TR_GERMAN_1TR6 */
"Tysk 1TR6",
/* TR_EURO_EDSS1 */
"Euro (EDSS1)",
/* TR_LEASED_LINE */
"Lånt linje",
/* TR_US_NI1 */
"US NI1",
/* TR_PROTOCOL_COUNTRY */
"Protokoll/Land",
/* TR_ISDN_CARD */
"ISDN kort",
/* TR_MSN_CONFIGURATION */
"Lokalt telefonnummer (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Sett flere modul parametre",
/* TR_RED_IN_USE */
"ISDN (eller en annen ekstern tilkoblign) er i bruk. Du kan ikke " \
"konfiguere ISDN mens den RØDE porten er i bruk.",
/* TR_ISDN_CONFIGURATION_MENU */
"ISDN konfigurasjons meny",
/* TR_ISDN_STATUS */
"ISDN er nå %s.\n\n" \
"   Protokoll: %s\n" \
"   Kort: %s\n" \
"   Lokalt telefonnummer: %s\n\n" \
"Velg elementet du ønsker å rekonfigurere, eller bruk gjeldende innstillinger.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"ISDN har ikke blitt konfigurert. Velg et element å konfigurere.",
/* TR_ENABLE_ISDN */
"Aktiver ISDN",
/* TR_DISABLE_ISDN */
"Deaktiver ISDN",
/* TR_INITIALISING_ISDN */
"Initialiserer ISDN...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Kan ikke initialisere ISDN.",
/* TR_ISDN_NOT_SETUP */
"ISDN er ikke satt opp. Noen elementer har ikke blitt valgt.",
/* TR_ISDN_PROTOCOL_SELECTION */
"Valg av ISDN protokol.",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Velg den ISDN protokollen du trenger.",
/* TR_AUTODETECT */
"* FINN AUTOMATISK *",
/* TR_ISDN_CARD_SELECTION */
"Velg ISDN kort",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Velg det ISDN kortet som er  installert på denne datamaskinen.",
/* TR_CHECKING_FOR */
"Leter etter: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"ISDN kortet ble ikke funnet. Du må kanskje spesifisere noen ekstra "\
"modul parametre hvis kortet er av ISA type eller har spesielle behov. ",
/* TR_DETECTED */
"Fant et: %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"Kan ikke finne et ISDN kort i denne datamaskinen. Det kan være at du må "\
"spesifisere noen ekstra modul parametre hvis kortet er av ISA type eller har "\
"spesielle behov.",
/* TR_ENTER_THE_LOCAL_MSN */
"Skriv inn lokalt telefonnummer (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"Telefonnummeret kan ikke være blankt.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Noen ISDN kort (spesielt ISA kort) kan trenge ekstra modul "\
"parametre for å sette IRQ og IO adresse informasjon. Hvis du har et slikt "\
"ISDN kort, skriv inn disse ekstra paramentere her. For eksempel: "\
"\"io=0x280 irq=9\". Disse vil bli brukt for å finne kortet.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Tar ned nettverket...",
/* TR_PULLING_NETWORK_UP */
"Tar opp nettverket...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Nettverks konfigurasjons type",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Driver og kort tildeling",
/* TR_ADDRESS_SETTINGS */
"Adresse instillinger",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"DNS og Gateway instillinger",
/* TR_RESTART_REQUIRED */
"\n\nNår konfigurasjonen er ferdig, må nettverket startes på ny.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Gjeldende konfigurasjon: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Meny for å konfigurere nettverket",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Velg nettverks konfigurasjon for SmoothWall. De følgende "\
"konfigurasjonstypene lister de portene som har ethernet koblet til. "\
"Hvis du forandrer disse instillingene, må nettverket startes på ny, og du "\
"må konfigurere nettverk driver tildelingen på ny.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Tar ned ikke-lokalt nettverk...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"Du er konfigurert for en enkel GRØNN port, som allerede er tildelt en driver.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Konfigurer nettverk driverne, og hvilken port hvert kort er på. "\
"Gjeldende konfigurasjon:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nØnsker du å forandre disse innstillingene?",
/* TR_UNCLAIMED_DRIVER */
"Det er et ubrukt ethernet kort av typen:\n%s\n\n" \
"Du kan tildele dette:",
/* TR_CARD_ASSIGNMENT */
"Kort tildeling",
/* TR_PROBE */
"Finn",
/* TR_SELECT */
"Velg",
/* TR_NO_UNALLOCATED_CARDS */
"Ingen tilgjengelige kort igjen, "\
"men du trenger flere. Du kan søke etter flere kort, eller "\
"velge en driver fra listen.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Kan ikke finne flere drivere.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Alle kortene er på plass.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Det ble ikke satt nok kort på plass.",
/* TR_MANUAL */
"* MANUELL *",
/* TR_SELECT_NETWORK_DRIVER */
"Velg nettverk driver",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Velg nettverks driveren for kortet som er installert på denne maskinen. "\
"Hvis du velger MANUELL, vil du bli gitt muligheten til å skrive "\
"driveren sitt modul navn og paremetre for drivere som har "\
"spesielle behov, slik som ISA kort.",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Kan ikke laste driver modulen.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"Denne driver modulen er allerede lastet.",
/* TR_MODULE_PARAMETERS */
"Skriv inn modulnavn og parametre for driveren.",
/* TR_LOADING_MODULE */
"Laster modul...",
/* TR_WARNING */
"ADVARSEL",
/* TR_WARNING_LONG */
"Hvis du forandrer denne IP adressen, og du er logget inn fra en annen maskin, "\
"vil din forbindelse til SmoothWall maskinen bli brutt, og du må koble til en ny IP. "\
"Dette er en svært risikabel operasjon, og bør "\
"bare benyttes hvis du har fysisk tilgang til maskinen dersom noe går galt.",
/* TR_SINGLE_GREEN */
"Du er konfigurert for en enkel GRØNN port, som allerede er tildelt en driver.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Velg porten du ønsker å konfigurere på ny.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"Din konfigurasjon inneholder ikke en ethernet kobling for "\
"den RØDE porten. DNS og Gateway informasjon for oppringt brukere "\
"blir konfigurert automatisk når du kobler opp.",
/* TR_PRIMARY_DNS */
"Primær DNS:",
/* TR_SECONDARY_DNS */
"Sekundær DNS:",
/* TR_DEFAULT_GATEWAY */
"Standard Gateway:",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Skriv inn DNS og Gateway informasjon. Disse instillingene blir brukt bare hvis DHCP "\
"er deaktivert på den RØDE porten.",
/* TR_PRIMARY_DNS_CR */
"Primær DNS\n",
/* TR_SECONDARY_DNS_CR */
"Sekundær DNS\n",
/* TR_DEFAULT_GATEWAY_CR */
"Standard Gateway\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"Sekundær DNS er spesifisert uten en primær DNS",
/* TR_UNKNOWN */
"UKJENT",
/* TR_NO_ORANGE_INTERFACE */
"Det er ikke tildelt en ORANGE port.",
/* TR_MISSING_ORANGE_IP */
"Mangler IP informasjon for den ORANGE porten.",
/* TR_NO_RED_INTERFACE */
"Det er ikke tildelt en RØD port.",
/* TR_MISSING_RED_IP */
"Mangler IP informasjon om den RØDE porten.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Start adresse:",
/* TR_END_ADDRESS */
"Slutt adresse:",
/* TR_DEFAULT_LEASE */
"Standard lånetid (minutter):",
/* TR_MAX_LEASE */
"Lengste lånetid (minutter):",
/* TR_DOMAIN_NAME_SUFFIX */
"Domene navn suffiks:",
/* TR_CONFIGURE_DHCP */
"Konfigurer DHCP tjeneren ved å skrive inn konfigurerings informasjon.",
/* TR_START_ADDRESS_CR */
"Start adresse\n",
/* TR_END_ADDRESS_CR */
"Slutt adresse\n",
/* TR_DEFAULT_LEASE_CR */
"Standard lånetid\n",
/* TR_MAX_LEASE_CR */
"Lengste lånetid\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Domene navn suffiks\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Tastatur kart",
/* TR_KEYBOARD_MAPPING_LONG */
"Velg hvilken type tastatur du bruker fra listen under.",

/* timezone.c */
/* TR_TIMEZONE */
"Tidssone",
/* TR_TIMEZONE_LONG */
"Velg den tidssonen du er i fra listen under.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Velg USB kontrollør",
/* TR_USBADSL_STATUS */
"USB ADSL er nå: %s\n" \
"   Kontrollør: %s\n\n" \
"Velg det elementet du ønsker å konfigurere på ny, eller velg å bruke de "\
"gjeldende instillingene.",
/* TR_USBADSL_CONFIGURATION */
"USB ADSL konfigurasjon",
/* TR_ENABLE_USBADSL */
"Aktiver USB ADSL",
/* TR_DISABLE_USBADSL */
"Deaktiver USB ADSL",
/* TR_INITIALISING_USBADSL */
"Initialiser USB ADSL.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"Kan ikke initialisere USB ADSL",
/* TR_USBADSL_NOT_SETUP */
"USB ADSL er ikke satt opp.",
/* TR_USB_CONTROLLER_SELECTION */
"Valg av USB kontrollør",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Velg den USB kontrolløren som er installert på denne SmoothWall maskinen.",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"USB kontrolløren ble ikke funnet.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"Kan ikke finne en USB kontrollør.",
/* TR_STARTING_UP_USBADSL */
"Starter USB ADSL..."

};


