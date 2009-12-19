/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * (c) Romainan Translation Team:
 * Marian Radulescu
 *
 * filename: romaian.c
 * Contains romanian strings. */

#include "libsmooth.h"

char *romanian_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Renunta",
/* TR_INSTALLATION_CANCELED */
"S-a renuntat la instalare.",
/* TR_HELPLINE */
"              <Tab>/<Alt-Tab> intre elemente   |  <Space> selecteaza",
/* TR_QUIT */
"Iesire",
/* TR_DISABLED */
"Dezactivat",
/* TR_ENABLED */
"Activat",
/* TR_UNSET */
"NESETAT",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Nu se poate deschide fisierul de configurare",
/* TR_DONE */
"Gata",
/* TR_PROBE_FAILED */
"Auto detectare esuata.",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"Adresa IP:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Adresa de retea:",
/* TR_NETMASK_PROMPT */
"Masca pentru retea:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Introduceti informatiile pentru IP",
/* TR_INVALID_FIELDS */
"Urmatoarele campuri sunt incorecte:\n\n",
/* TR_IP_ADDRESS_CR */
"Adresa IP\n",
/* TR_NETWORK_ADDRESS_CR */
"Adresa de retea\n",
/* TR_NETWORK_MASK_CR */
"Masca pentru retea\n",
/* TR_INTERFACE (%s is interface name) */
"interfata %s",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Introduceti informatiile de adresa IP pentru interfata %s.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Caut dupa: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall a detectat urmatoarea interfata de retea in acest sistem: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"Numele de modul nu poate fi necompletat.",
/* TR_STATIC */
"Static",
/* TR_DHCP_HOSTNAME */
"Nume DHCP:",
/* TR_DHCP_HOSTNAME_CR */
"Nume DHCP\n",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Nu pot scrie fisierul /var/smoothwall/main/hostname.conf",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Nu pot scrie fisierul /etc/hosts.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"Nu pot scrie fisierul /etc/hosts.deny.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Nu pot scrie fisierul /etc/hosts.allow.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Nu pot seta numele sistemului.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Bun venit in programul de instalare al SmoothWall. Va rugam vizitati-ne " \
"la adresa http://www.smoothwall.org.  Selectind Renunta in oricare din " \
"ecranele urmatoare, sistemul pe care instalati va fi rebootat.",
/* TR_NO_IDE_HARDDISK */
"Nu am detectat harddisk-uri IDE.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Selectati mediul de instalare",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"SmoothWall poate fi instalat folosind diverse metode.  Cea ma simpla este sa " \
"folositi unitatea CDROM din sistem. Daca nu dispuneti de unitate de CDROM, puteti " \
"sa faceti instalarea de pe un alt sistem din retea care are fisierele de instalare " \
" disponibile pe  un server de HTTP. In acest caz, discheta cu driverele de retea " \
" va fi necesara .",
/* TR_NO_IDE_CDROM */
"Nu am gasit un CDROM IDE.",
/* TR_INSERT_CDROM */
"Introduceti CD-ul cu SmoothWall unitate.",
/* TR_INSERT_FLOPPY */
"Introduceti discheta cu drivere de retea in unitatea floppy.",
/* TR_PREPARE_HARDDISK */
"Programul de instalare va pregati acum harddisk-ul IDE on %s. " \
"Primul disc din sistem va fi partitionat, si apoi partitiile vor avea " \
"un sistem de fisiere pe.",
/* TR_PARTITIONING_DISK */
"Partitionez discul...",
/* TR_UNABLE_TO_PARTITION */
"Nu pot crea partitii pe disc.",
/* TR_MAKING_SWAPSPACE */
"Creez spatiul de swap...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Nu pot crea spatiul de swap.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Creez sistemul de fisiere in root...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Nu pot crea sistemul de fisiere in root.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Montez sistemul de fisiere din root...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Nu pot monta sistemul de fisiere din root.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Creez sistemul de fisiere in boot...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Nu pot crea sistemul de fisiere in boot.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Montez sistemul de fisiere din boot...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Nu pot monta sistemul de fisiere din boot.",
/* TR_MAKING_LOG_FILESYSTEM */
"Creez sitemul de fisiere in log...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Nu pot crea sistemul de fisiere in log.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Montez sistemul de fisiere din log...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Nu pot monta sistemul de fisiere din log.",
/* TR_MOUNTING_SWAP_PARTITION */
"Mounting swap partition...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Nu pot monta partitia de swap.",
/* TR_NETWORK_SETUP_FAILED */
"Configurarea retelei nu a reusit.",
/* TR_NO_TARBALL_DOWNLOADED */
"Nu s-a descarcat nici o arhiva.",
/* TR_INSTALLING_FILES */
"Instalez fisierele...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Nu pot instala fisierele.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Nu pot sterge fisiserele temporare descarcate.",
/* TR_ERROR_WRITING_CONFIG */
"Eroare la scrierea informatiilor de configurare.",
/* TR_INSTALLING_LILO */
"Instalez LILO...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Nu pot instala LILO.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Nu pot umonta harddisk-ul.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Nu pot umonta CDROM/floppy disk.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Nu pot sa ejectez CDROM-ul.",
/* TR_CONGRATULATIONS */
"Felicitari!",
/* TR_CONGRATULATIONS_LONG */
"SmoothWall a fost instalat cu succes. Va rog sa scoateti orice floppy disc sau " \
"CDROM din unitatile sistemului. Se va rula programul de configurare unde veti " \
"configura ISDN, interfete de retea, si parolele de sistem. Dupa ce configurarea " \
"a fost completata, puteti porni browserul de web si accesa adresa http://smoothwall:81 " \
"sau https://smoothwall:445 (schimbati smoothwall cu numele pe care l-ati folosit la " \
"instalare) pentru modificarea parolei de dialup (daca este necesara) si accesului la " \
"distanta. Amintitiva sa configurati parola pentru utilizatorul 'dial' daca doriti ca " \
"alti utilizatori decat 'admin' sa poata controla legatura dialup.",
/* TR_PRESS_OK_TO_REBOOT */
"Apasati Ok pentru a reboota.",
/* TR_ERROR */
"Eroare",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"Calculez dependentele pentru module...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Nu pot calcula dependentele pentru module.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Selectati tipul de CDROM",
/* TR_SELECT_CDROM_TYPE_LONG */
"In acest sistem nu a fost detectat un CDROM IDE.  Selectati care din driverele urmatoare " \
"doriti sa fie folosit astfel incat SmoothWall sa poata accesa CDROM-ul.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(sugerez %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(sugerez %d)",
/* TR_CONFIGURE_THE_CDROM */
"Configurati CDROM-ul alegand valorile corecte pentru adresa IO si/sau IRQ.",
/* TR_INVALID_IO (note extra space) */
"Valoarea introdusa pentru adresa IO este incorecta. ",
/* TR_INVALID_IRQ */
"Valoarea introdusa pentru IRQ este incorecta.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Nu pot scrie fisierul /var/smoothwall/main/settings.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Nu pot scrie fisierul /var/smoothwall/ethernet/settings.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Nu pot crea symlink-ul pentru /dev/harddisk.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Nu pot crea symlink-ul pentru /dev/harddisk1.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Nu pot crea symlink-ul pentru /dev/harddisk2.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Nu pot crea symlink-ul pentru /dev/harddisk3.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Nu pot crea symlink-ul pentru /dev/harddisk4.",

/* net.c */
/* TR_DOWNLOADING */
"Incarc...",
/* TR_FAILED_TO_DOWNLOAD */
"Incarcare nereusita.",
/* TR_ENTER_URL */
"Introduceti URL-ul pentru fisierul SmoothWall tar.gz. " \
"ATENTIE: Serverul de DNS nu este disponibil!  URL-ul trebuie sa se termine cu fisierul 'smoothwall.tgz'.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Configurare retea",
/* TR_CONFIGURE_NETWORKING_LONG */
"You should now configure networking by first loading the correct driver for " \
"the GREEN interface. You can do this by either auto-probing for a network " \
"card, or by choosing the correct driver from a list. Note that if you have " \
"more then one network card installed, you will be able to configure the " \
"others later on in the installation. Also note that if you have more then one " \
"card which is the same type as GREEN and each card requires special module " \
"parameters, you should enter parameters for all cards of this type such "  \
"that all cards can become active when you configure the GREEN interface.",
/* TR_INTERFACE_FAILED_TO_COME_UP */
"Interfata nu poate fi activata.",
/* TR_ENTER_NETWORK_DRIVER */
"Detaectarea automata a interfetei de retea nu a reusit. Introduceti driverul si " \
"parametri optionali pentru placa de retea.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Nume host",
/* TR_NETWORKING */
"Retea",
/* TR_DHCP_SERVER_CONFIGURATION */
"Configurare server DHCP",
/* TR_ISDN_CONFIGURATION */
"Configurare ISDN",
/* TR_ROOT_PASSWORD */
"Parola de \'root\'",
/* TR_SETUP_PASSWORD */
"Parola de \'setup\'",
/* TR_ADMIN_PASSWORD */
"Parola de Admin",
/* TR_SECTION_MENU */
"Sectiune menu",
/* TR_SELECT_THE_ITEM */
"Selectati elementul pe care doriti sa il configurati.",
/* TR_SETUP_FINISHED */
"Configurarea este completa.  Apasati butonul Ok pentru a reboota.",
/* TR_SETUP_NOT_COMPLETE */
"Configurarea initiala nu este completa.  Trebuie sa va asigurati ca ati " \
"terminat corect configurarea ruland programul de configurare intr-un shell.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Introduceti parola de 'root'. Folositi acest cont pentru a avea acces la utilitarele in linie de comanda.",
/* TR_SETTING_ROOT_PASSWORD */
"Setez parola pentru contul 'root'....",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Au aparut probleme la setarea parolei pentru 'root'.",
/* TR_ENTER_SETUP_PASSWORD */
"Introduceti parola pentru contul 'setup'. Folositi acest cont pentru a accesa programul " \
"de configurare.",
/* TR_SETTING_SETUP_PASSWORD */
"Setez parola pentru contul 'setup'....",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Au aparut probleme la setarea parolei pentru 'setup'.",
/* TR_ENTER_ADMIN_PASSWORD */
"Introduceti parola pentru contul 'admin'.  Acest cont este folosit pentru conectarea " \
"la interfata de administrare web a SmoothWall.",
/* TR_SETTING_ADMIN_PASSWORD */
"Setez parola pentru contul SmoothWall 'admin'....",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Au aparut probleme la setarea parolei pentru 'admin'.",
/* TR_PASSWORD_PROMPT */
"Parola:",
/* TR_AGAIN_PROMPT */
"Parola din nou:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"Parola nu poate fi vida.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"Parolele introduse nu sunt identice.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Introduceti numele pentru acest sistem.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"Numele sistemului nu poate fi vid.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"Numele de sistem nu poate contine spatii.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Nu pot scrie fisierul /etc/hostname",

/* isdn.c */
/* TR_GERMAN_1TR6 */
"German 1TR6",
/* TR_EURO_EDSS1 */
"Euro (EDSS1)",
/* TR_LEASED_LINE */
"Linie inchiriata",
/* TR_US_NI1 */
"US NI1",
/* TR_PROTOCOL_COUNTRY */
"Protocol/Tara",
/* TR_ISDN_CARD */
"Interfata ISDN",
/* TR_MSN_CONFIGURATION */
"Numarul local de telefon (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Parametri aditionali pentru modul",
/* TR_RED_IN_USE */
"ISDN (sau o alta conexiune) exte folosita in acest moment.  Nu puteti configura ISDN " \
"cat timp interfata ROSU (externa) este activa.",
/* TR_ISDN_CONFIGURATION_MENU */
"Meniu de configurare ISDN",
/* TR_ISDN_STATUS */
"Configuratia ISDN este %s.\n\n" \
"   Protocol: %s\n" \
"   Interfata: %s\n" \
"   Nr. local tel.: %s\n\n" \
"Selectati elementul pe care doriti sa il reconfigurati sau alegeti utilizarea setarilor curente.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"ISDN nu a fost configurat. Selectati elementul pe care doriti sa il configurati.",
/* TR_ENABLE_ISDN */
"Activeaza ISDN",
/* TR_DISABLE_ISDN */
"Dezactiveaza ISDN",
/* TR_INITIALISING_ISDN */
"Initializez ISDN...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Nu pot initaliza ISDN.",
/* TR_ISDN_NOT_SETUP */
"ISDN nu este configurat. Exista elemente neselectate pentru configurare.",
/* TR_ISDN_PROTOCOL_SELECTION */
"Selectarea protocolului ISDN",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Alegeti protocolul ISDN pe care doriti sa il folositi.",
/* TR_AUTODETECT */
"* AUTODETECTIE *",
/* TR_ISDN_CARD_SELECTION */
"Selectarea interfetei ISDN",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Alegeti cartela ISDN instalata in acest sistem.",
/* TR_CHECKING_FOR */
"Verific: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"Cartela ISDN nu a fost detectata. Este posibil sa fie nevoie sa specificati parametri " \
"suplimentari pentru modul daca aveti un card ISA sau care necesita setari speciale.",
/* TR_DETECTED */
"Am detectat un: %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"Nu am detectat nici o cartela ISDN in acest sistem. Probabil este nevoie sa specificati " \
"parametri suplimentari pentru modul daca aveti un card ISA sau care necesita setari " \
"speciale.",
/* TR_ENTER_THE_LOCAL_MSN */
"Introduceti numarul local de telefon (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"Numarul de telefon nu poate fi vid.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Anumite cartele ISDN (in special cele ISA) necesita parametri suplimentari " \
"pentru modul, parametri necesari la configurarea IRQ si a adresei IO. Daca aveti " \
"un asemenea card, introduceti aceste date suplimentare aici. De exemplu: " \
"\"io=0x280 irq=9\". Aceste date vor fi folosite pentru detectia cartelei.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Opresc serviciile de retea...",
/* TR_PULLING_NETWORK_UP */
"Pornesc serviciile de retea...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Tipul configuratiei de retea",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Asignarea driverelor si interfetelor",
/* TR_ADDRESS_SETTINGS */
"Configurarea adreselor",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"Configurarea DNS si Gateway",
/* TR_RESTART_REQUIRED */
"\n\nLa terminarea configrarii retelei, va fi necesara o restartare a acesteia.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Configuratie curenta: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Meniu de configurare retea",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Selectati tipul configuratiei de retea pentru SmoothWall.  Urmatoarele " \
"tipuri de configurare listeaza interfetele ethernet. " \
"Daca schimbati aceste setari, va fi necesara o restartare a retelei si v trebui " \
"sa reconfigurati asignarea drverelor pentru interfetele de retea.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Opresc reteaua externa...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"Configuratia este setata pentru o singura interfata VERDE (interna), " \
"care are deja un driver asignat.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Configurati dirverele de retea si asignarea interfetelor pentru fiecare " \
"cartela.  Configuratia curenta este urmatoarea:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nDoriti sa schimbati aceste setari?",
/* TR_UNCLAIMED_DRIVER */
"Exista o cartela de retea nefolosita:\n%s\n\n" \
"Puteti asigna acest card la:",
/* TR_CARD_ASSIGNMENT */
"Assignarea cartelelor",
/* TR_PROBE */
"Probe",
/* TR_SELECT */
"Selectare",
/* TR_NO_UNALLOCATED_CARDS */
"Nu mai sunt cartele disponibile pentru alocare si mai aveti nevoie. " \
"Puteti incerca o detectie automata pentru cele care nu sunt listate, sau " \
"puteti alege un driver din lista.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Nu am gasit nici un driver suplimentar.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Toate cartelele au fost alocate cu succes.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Numarul de cartele alocate nu este suficient.",
/* TR_MANUAL */
"* MANUAL *",
/* TR_SELECT_NETWORK_DRIVER */
"Selectati driverul de retea",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Selectati driveru de retea pentru cartela instalata in acest sistem. " \
"Daca selectati MANUAL, aveti posibilitatea de a introduce numele modulului " \
"folosit de driver si parametri pentru driverele care au cerinte speciale, cum " \
"ar fi de exemplu cartelele ISA.",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Nu pot incarca modulul pentru driver.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"Acest modul este deja incarcat.",
/* TR_MODULE_PARAMETERS */
"Introduceti numele de modul si parametri ceruti de driver.",
/* TR_LOADING_MODULE */
"Incarc modulul...",
/* TR_WARNING */
"ATENTIE",
/* TR_WARNING_LONG */
"Daca modificati aceasta adresa IP, si sunteti conectati de pe un alt sistem, " \
"legatura cu sistemul pe care ruleaza SmoothWall va fi intrerupta, si va trebui " \
"sa va reconectati la noua adresa de IP. Aceasta operatie este riscanta si ar trebui " \
"incercata doar daca aveti acces fizic la sistem, deoarece se poate intampla ca " \
"ceva sa nu functioneze corect.",
/* TR_SINGLE_GREEN */
"Configuratia este setata pentru o singura interfata VERDE (interna).",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Selectati interfata pe care doriti sa o reconfigurati.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"Configuratia aleasa nu foloseste o cartela de retea ethernet pentru " \
"interfata ROSU.  Informatiile pentru DNS si Gateway pentru utilizatorii " \
"de dialup sunt configurate automat la conectare.",
/* TR_PRIMARY_DNS */
"DNS primar:",
/* TR_SECONDARY_DNS */
"DNS secundar:",
/* TR_DEFAULT_GATEWAY */
"Gateway implicit:",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Introduceti informatiile pentru DNS si gateway.  Aceste informatii sunt folosite doar daca " \
"interfata ROSU are dezactivat DHCP.",
/* TR_PRIMARY_DNS_CR */
"DNS primar\n",
/* TR_SECONDARY_DNS_CR */
"DNS secundar\n",
/* TR_DEFAULT_GATEWAY_CR */
"Gateway implicit\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"S-a specificat un server DNS secundar fara a se specifica serverul primar",
/* TR_UNKNOWN */
"NECUNOSCUT",
/* TR_NO_ORANGE_INTERFACE */
"Nu s-a asignat o interfata PORTOCALIE.",
/* TR_MISSING_ORANGE_IP */
"Lipsesc informatiile IP pentru interfata PORTOCALIE.",
/* TR_NO_RED_INTERFACE */
"Nu s-a asignat o interfata ROSIE.",
/* TR_MISSING_RED_IP */
"Lipsesc informatiile IP pentru interfata ROSIE.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Adresa de inceput:",
/* TR_END_ADDRESS */
"Adresa de sfarsit:",
/* TR_DEFAULT_LEASE */
"Timp de asignare implicit (min):",
/* TR_MAX_LEASE */
"Timp maxim de asignare (min):",
/* TR_DOMAIN_NAME_SUFFIX */
"Sufixul de domeniu:",
/* TR_CONFIGURE_DHCP */
"Configurati serverul DHCP introducand informatiile de setare.",
/* TR_START_ADDRESS_CR */
"Adresa de inveput\n",
/* TR_END_ADDRESS_CR */
"Adresa de sfarsit\n",
/* TR_DEFAULT_LEASE_CR */
"Timp de asignare implicit\n",
/* TR_MAX_LEASE_CR */
"Timp maxim de asignare\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Sufixul de domeniu\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Maparea de tastatura",
/* TR_KEYBOARD_MAPPING_LONG */
"Alegeti din lista de mai jos tipul de tastatura folosit.",

/* timezone.c */
/* TR_TIMEZONE */
"Zona de timp",
/* TR_TIMEZONE_LONG */
"Alegeti din lista de mai jos, zona de timp a regiunii in care locuiti.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Selectati controlerul USB",
/* TR_USBADSL_STATUS */
"Config. curenta USB ADSL este: %s\n" \
"   Controler: %s\n\n" \
"Selectati elementul pe care doriti sa il reconfigurati, sau continuati sa folositi setarile actuale.",
/* TR_USBADSL_CONFIGURATION */
"Configurare USB ADSL",
/* TR_ENABLE_USBADSL */
"Activeaza USB ADSL",
/* TR_DISABLE_USBADSL */
"Dezactiveaza USB ADSL",
/* TR_INITIALISING_USBADSL */
"Initializeaza USB ADSL.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"Nu pot initializa USB ADSL",
/* TR_USBADSL_NOT_SETUP */
"USB ADSL nu este configurat.",
/* TR_USB_CONTROLLER_SELECTION */
"Selectare controler USB",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Alegeti controlerul USB instalat in sistemul SmoothWall.",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"Nu am detectat controllerul USB.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"Nu gasesc controlerul USB.",
/* TR_STARTING_UP_USBADSL */
"Pornesc USB ADSL..."

};
