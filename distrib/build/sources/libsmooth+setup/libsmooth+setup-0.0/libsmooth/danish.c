/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * (c) Danish Translation Team:
 * Jacob Anderson
 * Karina Anderson 
 *
 * filename: danish
 * Contains danish strings. */
 
#include "libsmooth.h"

char *danish_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Annuler",
/* TR_INSTALLATION_CANCELED */
"Installation annuleret.",
/* TR_HELPLINE */
"          <Tab>/<Alt-Tab> mellem elementerne   |  <Mellemrum> vælger",
/* TR_QUIT */
"Afbryd",
/* TR_DISABLED */
"Slået Fra",
/* TR_ENABLED */
"Slået Til",
/* TR_UNSET */
"Ikke Sat",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Kan ikke åbne indstillingsfilen",
/* TR_DONE */
"Udført",
/* TR_PROBE_FAILED */
"Auto detektering mislykkedes.",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"IP adresse:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Netværks adresse:",
/* TR_NETMASK_PROMPT */
"Netværks maske:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Indtast IP adresse information",
/* TR_INVALID_FIELDS */
"De følgende felter er ugyldige:\n\n",
/* TR_IP_ADDRESS_CR */
"IP adresse\n",
/* TR_NETWORK_ADDRESS_CR */
"Netværks adresse\n",
/* TR_NETWORK_MASK_CR */
"Netværks maske\n",
/* TR_INTERFACE (%s is interface name) */
"%s interface",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Indtast IP adresse information for %s interfacet.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Leder efter: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall har fundet følgende NIC'er på din maskine: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"Modul navn kan ikke være blankt.",
/* TR_STATIC */
"Statisk",
/* TR_DHCP_HOSTNAME */
"DHCP Værtsnavn:",
/* TR_DHCP_HOSTNAME_CR */
"DHCP Værtsnavn\n",

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
"Ude af stand til at sætte værtsnavn.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Velkommen til SmoothWall installationsprogrammet. Besøg vores " \
"hjemmeside på http://www.smoothwall.org.  Ved at vælge Annuler " \
"på de følgende skærmbilleder kan du reboote (genstarte) computeren.",
/* TR_NO_IDE_HARDDISK */
"Der bliv IKKE fundet nogen IDE harddisk.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Vælg installations medie",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"SmoothWall kan installed fra flere kilder.  Det nemmeste er at bruge " \
"maskinens CDROM drev. Hvis computer ikke har et CDROM drev, kan du " \
"installere via en anden maskine på et LAN som har installationsfilerne " \
"tilgængelige via HTTP. I dette tilfælde vil en netværks driver diskette være " \
"nødvendig.",
/* TR_NO_IDE_CDROM */
"Ingen IDE cdrom fundet.",
/* TR_INSERT_CDROM */
"Indsæt venligst SmoothWall CD i CDROM drevet.",
/* TR_INSERT_FLOPPY */
"Indsæt venligst SmoothWall driver disketten i floppy drevet.",
/* TR_PREPARE_HARDDISK */
"Installations programmet vil nu forberede IDE harddisken på %s. " \
"Først vil disken blive partitioneret, derefter vil partitionerne få lagt et " \
"filsystem på.",
/* TR_PARTITIONING_DISK */
"Partitionerer disk...",
/* TR_UNABLE_TO_PARTITION */
"Ikke i stand til at partitionere disken.",
/*s TR_MAKING_SWAPSPACE */
"Laver swap plads...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Ikke i stand til at lave swap plads.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Laver root (rod) filsystem...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Ikke i stand til at lave root filsystem.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Mounter root filsystemet...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Ikke i stand til at mounte root filsystemet.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Laver boot filsystemet...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Ikke i stand til at lave boot filsystemet.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Mounter boot filsystemet...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Ikke i stand til at mounte boot filsystemet.",
/* TR_MAKING_LOG_FILESYSTEM */
"Laver lognings filsystemet...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Ikke i stand til at lave lognings filsystemet.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Mounter lognings filsystemet...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Ikke i stand til at mounte lognings filsystemet.",
/* TR_MOUNTING_SWAP_PARTITION */
"Mounter swap partitionen...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Ikke i stand til at mounte swap partitionen.",
/* TR_NETWORK_SETUP_FAILED */
"Fejl i netværksopsætningen.",
/* TR_NO_TARBALL_DOWNLOADED */
"Ingen tarball er downloadet.",
/* TR_INSTALLING_FILES */
"Installerer filer...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Ikke i stand til at installere filer.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Ikke i stand til at fjerne midlertidige downloadede filer.",
/* TR_ERROR_WRITING_CONFIG */
"Fejl under skrivning af konfigurations informationer.",
/* TR_INSTALLING_LILO */
"Installerer LILO...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Ikke i stand til at installere LILO.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Ikke i stand til at unmounte harddisken.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Ikke i stand til at unmounte CDROM/floppydisk.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Ikke i stand til at skubbe cd skuffen ud.",
/* TR_CONGRATULATIONS */
"Tillykke!",
/* TR_CONGRATULATIONS_LONG */
"SmoothWall blev succesfuldt installeret. Fjern venligst alle floppy disketter eller " \
"CDROMer i computeren. Opsætningsprogrammet vil nu køre der hvor du kan konfigurere din ISDN, " \
" netværks kort, og system kodeord. Efter at opsætningsprogrammet er fuldført, " \
"skal du i din web browsers adresse felt skrive http://smoothwall:81 eller " \
"https://smoothwall:445 (eller hvad du nu har kaldt din SmoothWall), og konfigurere " \
"opkalds netværk (hvis nødvendigt) og fjern-adgang. Husk at sætte " \
"et kodeord til SmoothWall 'opkalds' bruger, hvis du ønsker at det skal være muligt for ikke SmoothWall " \
"'admin' brugere at kontrollere opkaldsforbindelsen.",
/* TR_PRESS_OK_TO_REBOOT */
"Tryk Ok for at genstarte.",
/* TR_ERROR */
"Fejl",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"Udregner modul afhængigheder...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Ikke i stand til at udrege modul afhængigheder.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Vælg CDROM type",
/* TR_SELECT_CDROM_TYPE_LONG */
"Ingen IDE CDROM blev fundet på denne maskine.  Vælg venligst hvilket " \
"af de følgende drivere du ønsker at bruge så SmoothWall kan få adgang til CDROMen.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(foreslå %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(foreslå %d)",
/* TR_CONFIGURE_THE_CDROM */
"Konfigurer CDROMen ved at vælge den passende IO adresse og/eller IRQ.",
/* TR_INVALID_IO (note extra space) */
"Detaljerne indtastet for IO porten er ugyldige. ",
/* TR_INVALID_IRQ */
"Indtastede IRQ detaljer er ugyldige.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Ikke i stand til at skrive til /var/smoothwall/main/settings.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Ikke i stand til at skrive til /var/smoothwall/ethernet/settings.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Ikke i stand til at skabe symlink /dev/harddisk.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Ikke i stand til skabe symlink /dev/harddisk1.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Ikke i stand til at skabe symlink /dev/harddisk2.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Ikke i stand til at skabe symlink /dev/harddisk3.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Ikke i stand til at skabe symlink /dev/harddisk4.",

/* net.c */
/* TR_DOWNLOADING */
"Downloader...",
/* TR_FAILED_TO_DOWNLOAD */
"Fejl opstod under downloadning.",
/* TR_ENTER_URL */
"Indtast URL'en til SmoothWall tar.gz filen. " \
"ADVARSEL: DNS ikke tilgængelig!  Dette skulle ende med filen 'smoothwall.tgz'.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Konfigurer netværket",
/* TR_CONFIGURE_NETWORKING_LONG */
"Du skal nu konfigurere netværket ved først at indlæse den korrekte driver til" \
"den GRØNNE grænseflade (interface). Du kan gøre dette enten ved auto-check for et" \
"netværkskort, eller ved at vælge den korrekte driver fra en liste. Bemærk, at hvis" \
"du har mere end et netværkskort installeret, kan du konfigurere de andre senere" \
"i installationen. Bemærk også, at hvis du har mere end et kort som er af samme " \
"type som det GRØNNE og hvis hvert kort kræver specielle modul parametre, " \
"bør du indtaste disse parametre for alle kort af denne type, "  \
"så alle kort bliver aktive når du konfigurerer det GRØNNE interface.",
/* TR_INTERFACE_FAILED_TO_COME_UP */
"Interfacet (grænsefladen) kunne ikke startes.",
/* TR_ENTER_NETWORK_DRIVER */
"Kunne ikke finde et netværkskort automatisk. Indtast driveren og " \
"yderligere parametre for netværkskortet.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Værtsnavn",
/* TR_NETWORKING */
"Netværket",
/* TR_DHCP_SERVER_CONFIGURATION */
"DHCP server konfiguration",
/* TR_ISDN_CONFIGURATION */
"ISDN Konfiguration",
/* TR_ROOT_PASSWORD */
"\'root\' kodeord",
/* TR_SETUP_PASSWORD */
"\'setup\' kodeord",
/* TR_ADMIN_PASSWORD */
"Admin kodeord",
/* TR_SECTION_MENU */
"Sektions menu",
/* TR_SELECT_THE_ITEM */
"Vælg den enhed du ønsker at konfigurere.",
/* TR_SETUP_FINISHED */
"Opsætingen er fuldført.  Tryk Ok for at genstarte.",
/* TR_SETUP_NOT_COMPLETE */
"Den første opsæting blev ikke helt fuldført.  Du må sikre at Opsætingen bliver " \
"fuldstændig kørt igennem ved at køre Opsætningen igen i shell'en.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Indtast 'root' bruger kodeordet. Log på som denne bruger for at få kommandolinie adgang.",
/* TR_SETTING_ROOT_PASSWORD */
"Indstiller 'root' kodeordet....",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Problem med at indstille 'root' kodeordet.",
/* TR_ENTER_SETUP_PASSWORD */
"Indstast 'setup' bruger kodeordet. Log på som denne bruger for at få adgang til opsætnings " \
"programmet.",
/* TR_SETTING_SETUP_PASSWORD */
"Indstiller 'setup' kodeordet....",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Problem med at indstille 'setup' kodeord.",
/* TR_ENTER_ADMIN_PASSWORD */
"Indtast SmoothWall admin kodeord.  Det er denne bruger der bruges " \
"til at logge på SmoothWalls web administration sider.",
/* TR_SETTING_ADMIN_PASSWORD */
"Indstiller SmoothWall admin kodeordet....",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Problem med at indstille SmoothWalls admin kodeord.",
/* TR_PASSWORD_PROMPT */
"Kodeord:",
/* TR_AGAIN_PROMPT */
"Igen:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"Kodeord kan ikke være blankt.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"De indtastede kodeord er ikke ens.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Indtast maskinens værtsnavn.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"Værtsnavns feltet kan ikke være tomt.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"Værtsnavn kan ikke indeholde mellemrum.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Kan ikke skrive til /etc/hostname",

/* isdn.c */
/* TR_GERMAN_1TR6 */
"Tysk 1TR6",
/* TR_EURO_EDSS1 */
"Euro (EDSS1)",
/* TR_LEASED_LINE */
"Lejet linie",
/* TR_US_NI1 */
"US NI1",
/* TR_PROTOCOL_COUNTRY */
"Protokol/Land",
/* TR_ISDN_CARD */
"ISDN kort",
/* TR_MSN_CONFIGURATION */
"Lokalt telefon nummer (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Opsæt yderligere modul parametre",
/* TR_RED_IN_USE */
"ISDN (eller anden extern forbindelse) er i øjeblikket i brug.  Du kan ikke " \
"koconfigurere ISDN, mens det RØDE interface er aktivt.",
/* TR_ISDN_CONFIGURATION_MENU */
"ISDN konfigurations menu",
/* TR_ISDN_STATUS */
"ISDN er i øjeblikket %s.\n\n" \
"   Protokol: %s\n" \
"   Kort: %s\n" \
"   Lokalt telefon nummer: %s\n\n" \
"Vælg den endhed du ønsker at omkonfigurere, eller vælg at bruge de nuværende indstillinger.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"ISDN er ikke konfigureret endnu. Vælg det emne som du vil konfigurere.",
/* TR_ENABLE_ISDN */
"Slå ISDN til",
/* TR_DISABLE_ISDN */
"Slå ISDN fra",
/* TR_INITIALISING_ISDN */
"Initialiserer ISDN...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Ude af stand til at initialisere ISDN.",
/* TR_ISDN_NOT_SETUP */
"ISDN ikke opsat. Nogle emner er ikke blevet valgt.",
/* TR_ISDN_PROTOCOL_SELECTION */
"ISDN protokol valg",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Vælg den ISDN protokol du vil have.",
/* TR_AUTODETECT */
"* AUTO-FIND *",
/* TR_ISDN_CARD_SELECTION */
"ISDN kard valg",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Vælg det ISDN kort som er installeret i denne computer.",
/* TR_CHECKING_FOR */
"Undersøger: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"ISDN kort ikke detekteret. Du kan være nødt til at specificere yderligere " \
"modul parametre, hvis kortet er et ISA kort eller kræver specielle indstillinger.",
/* TR_DETECTED */
"Fandt et: %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"ISDN kort ikke finde. Du kan være nødt til at specificere yderligere " \
"modul parametre, hvis kortet er et ISA kort eller kræver specielle indstillinger.",
/* TR_ENTER_THE_LOCAL_MSN */
"Indtast det lokale telefon nummer (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"Telefonnummer kan ikke være tomt.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Nogle ISDN kort (især ISA modeller) kræver yderligere modul " \
"parametre for at indstille IRQ og IO adresse information. Hvis du har et sådant " \
"ISDN kort, indtast disse extra parametre her. For eksampel: " \
"\"io=0x280 irq=9\". De vil blive brugt under kort detektering.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Skubber netværk ned...",
/* TR_PULLING_NETWORK_UP */
"Trækker netværk op...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Netværks konfigurationstype",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Driver og kort tildelinger",
/* TR_ADDRESS_SETTINGS */
"Adresse indstillinger",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"DNS og Gateway indstillinger",
/* TR_RESTART_REQUIRED */
"\n\nNår konfigurationen er fuldført, vil en genstart af netværket være nødvendig.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Nuværende konfiguration: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Netværks konfigurations menu",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Vælg en netværks konfiguration til SmoothWall.  De følgende " \
"konfigurations typer viser de interfaces (grænseflader) som har ethernet tilsluttet. " \
"Hvis du ændrer denne indstilling, vil det være nødvendigt at genstarte netværket, og du " \
"vil være nødt til at genkonfigurere netværksdriver tildelingerne.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Skubber ikke lokalt netværk ned...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"Din konfiguration er indstillet til et enligt GRØNT interface, " \
"som allerede har en driver tildelt.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Konfigurer netværk drivere, og hvilket interface hvert kort er " \
"tildelt til.  Den nuværende konfiguration er som følger:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\n ønsker du at ændre disse indstillinger?",
/* TR_UNCLAIMED_DRIVER */
"Der er et ikke tildelt ethernet kort af typen:\n%s\n\n" \
"Du kan tildele det til:",
/* TR_CARD_ASSIGNMENT */
"Kort tildeling",
/* TR_PROBE */
"Forsøg at finde ",
/* TR_SELECT */
"Vælg",
/* TR_NO_UNALLOCATED_CARDS */
"Ingen utildelte kort tilbage, " \
"der er brug for flere. Du bliver nødt til at søge efter flere kort, eller " \
"Vælge at vælge en driver fra listen.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Ikke i stand til at finde yderligere drivere.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Alle kort er successfuldt allokeret.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"For få kort blev allokeret.",
/* TR_MANUAL */
"* MANUEL *",
/* TR_SELECT_NETWORK_DRIVER */
"Vælg netværks driver",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Vælg netværks driveren for det kort, som er installeret i denne maskine. " \
"Hvis du vælger MANUEL, vil du få mulighed for at indtaste " \
"driverens modul navn og parametre for de drivere som har " \
"specielle krav, såsom ISA kort.",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Ikke i stand til at indlæse driver modulet.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"Dette driver module er allerede indlæst.",
/* TR_MODULE_PARAMETERS */
"Indtast modul navn og parametre for den driver du efterspørger.",
/* TR_LOADING_MODULE */
"Indlæser modul...",
/* TR_WARNING */
"ADVARSEL",
/* TR_WARNING_LONG */
"Hvis du ændrer denne IP adresse, og du er logget ind via fjern-adgang, " \
"vil din forbindelse til SmoothWall maskinen blive afbrudt, og du vil " \
"skulle etablere forbindelse igen på den nye IP. Dette er en risikofyldt operation, som kun " \
"bør prøves, hvis du har fysisk adgang til SmoothWall maskinen, hvis noget " \
"skulle gå galt.",
/* TR_SINGLE_GREEN */
"Din konfiguration er opsat til et enkelt GRØNT interface.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Vælg det interface du vil genkonfigurere.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"Din konfiguration bruger ikke en ethernet adapter til " \
"dets RØDE interface.  DNS og Gateway information for opkaldsbrugere " \
"bliver konfigureret automatisk når der foretages opkald.",
/* TR_PRIMARY_DNS */
"Primær DNS:",
/* TR_SECONDARY_DNS */
"Sekundær DNS:",
/* TR_DEFAULT_GATEWAY */
"Standard Gateway:",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Indtast DNS og gateway information.  Disse indstilliner bliver kun brugt hvis DHCP " \
"er slået fra på det RØDE interface.",
/* TR_PRIMARY_DNS_CR */
"Primær DNS\n",
/* TR_SECONDARY_DNS_CR */
"Sekundær DNS\n",
/* TR_DEFAULT_GATEWAY_CR */
"Standard Gateway\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"Sekundær DNS angivet uden at der er en Primær DNS",
/* TR_UNKNOWN */
"UKENDT",
/* TR_NO_ORANGE_INTERFACE */
"Intet ORANGE interface tildelt.",
/* TR_MISSING_ORANGE_IP */
"Manglende IP information for det ORANGE interface.",
/* TR_NO_RED_INTERFACE */
"Intet RØDT interface tildelt.",
/* TR_MISSING_RED_IP */
"Manglende IP information for det RØDE interface.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Start adresse:",
/* TR_END_ADDRESS */
"Slut adresse:",
/* TR_DEFAULT_LEASE */
"Standard lease tid(min):",
/* TR_MAX_LEASE */
"Max lease tid(min):",
/* TR_DOMAIN_NAME_SUFFIX */
"Domæne navns suffix:",
/* TR_CONFIGURE_DHCP */
"Konfigurer DHCP serveren, ved at indtaste opsætnings information.",
/* TR_START_ADDRESS_CR */
"Start adresse\n",
/* TR_END_ADDRESS_CR */
"Slut adresse\n",
/* TR_DEFAULT_LEASE_CR */
"Standard lease tid\n",
/* TR_MAX_LEASE_CR */
"Max lease tid\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Domæne navns suffix\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Tegnsæt",
/* TR_KEYBOARD_MAPPING_LONG */
"Vælg den type tastatur du bruger fra nedenstående liste.",

/* timezone.c */
/* TR_TIMEZONE */
"Tidszone",
/* TR_TIMEZONE_LONG */
"Vælg den tidszone du er i fra nedenstående liste.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Vælg USB controlleren",
/* TR_USBADSL_STATUS */
"USB ADSL er i øjblikket: %s\n" \
"   Controller: %s\n\n" \
"Vælg det du vil genkonfigurere, eller vælg at bruge nuværende indstillinger.",
/* TR_USBADSL_CONFIGURATION */
"USB ADSL konfiguration",
/* TR_ENABLE_USBADSL */
"Slå USB ADSL til",
/* TR_DISABLE_USBADSL */
"Slå USB ADSL fra",
/* TR_INITIALISING_USBADSL */
"Initialisirer USB ADSL.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"Ikke i stand til at initialisere USB ADSL",
/* TR_USBADSL_NOT_SETUP */
"USB ADSL ikke sat op.",
/* TR_USB_CONTROLLER_SELECTION */
"USB controller valg",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Vælg den USB controller som er installeret på SmoothWall maskinen.",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"USB controller ikke detekteret.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"Ikke i stand til at finde en USB controller.",
/* TR_STARTING_UP_USBADSL */
"Starter USB ADSL op..."

};
