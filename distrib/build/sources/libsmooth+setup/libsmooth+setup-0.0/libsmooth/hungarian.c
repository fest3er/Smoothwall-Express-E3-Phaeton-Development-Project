/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: hungarian.c
 * Contains hungarain strings. */

#include "libsmooth.h"

char *hungarian_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Megszakitás",
/* TR_INSTALLATION_CANCELED */
"A telepités megszakadt",
/* TR_HELPLINE */
"                 <Tab>/<Alt-Tab> mozgás   |  <Space> kiválasztás",
/* TR_QUIT */
"Kilépés",
/* TR_DISABLED */
"Tiltva",
/* TR_ENABLED */
"Engedélyezve",
/* TR_UNSET */
"NINCS BEÁLLITVA",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Nem lehet megnyitni a beállitásokat tartalmazó fájlt",
/* TR_DONE */
"Kész",
/* TR_PROBE_FAILED */
"Az automatikus felismerés sikertelen",

/*************/
/* LIBSMOOTH */
/***** ********/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"IP cim:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Hálózati cim:",
/* TR_NETMASK_PROMPT */
"Hálozati maszk:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Kérem az IP cim informáciokat",
/* TR_INVALID_FIELDS */
"A következö mezök érvénytelenek:\n\n",
/* TR_IP_ADDRESS_CR */
"IP cim\n",
/* TR_NETWORK_ADDRESS_CR */
"Hálózati cim\n",
/* TR_NETWORK_MASK_CR */
"Hálózati maszk mask\n",
/* TR_INTERFACE (%s is interface name) */
"%s interface",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Kérem az IP cim információkat a  %s interface számára.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Keresem: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"A SmoothWall a következö kártyát ismerte fel a gépben: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"A modulnév nem lehet üres.",
/* TR_STATIC */
"Statikus",
/* TR_DHCP_HOSTNAME */
"DHCP Hostnév:",
/* TR_DHCP_HOSTNAME_CR */
"DHCP Hostnév\n",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Nem lehet irni a /var/smoothwall/main/hostname.conf",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Nem lehet irni /etc/hosts.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"nem lehet irni /etc/hosts.deny.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Nem lehet irni  /etc/hosts.allow.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Nem lehet beállitani a hostnevet.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Köszönti a SmoothWall installáló program. Kérjük látogassa meg " \
"honlapunkat a  http://www.smoothwall.org cimen.  A Megszakit kiválasztásával " \
"ujraindul a számitógép.",
/* TR_NO_IDE_HARDDISK */
"Nem találtam IDE hard discet.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Válasszon telepitési mediumot",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"A SmoothWall többféle forrásból installálható. A legegyszerübb ha " \
"CDROM drive-rol telepitjuk. Ha nincs ilyenünk akkor " \
"hálozaton keresztül is telepithetjuk egy masik gepröl,melyen az installáló fájlok  " \
"elérhetök HTTP-n keresztül. Ebben az esetben szükség van a hálózati driverek " \
"lemezére.",
/* TR_NO_IDE_CDROM */
"Nem találtam IDE CD-Rom-ot.",
/* TR_INSERT_CDROM */
"Kérem helyezze be a SmoothWall CD-t a meghajtoba.",
/* TR_INSERT_FLOPPY */
"Kérem helyezze a SmoothWall driver disket a floppy drive-ba.",
/* TR_PREPARE_HARDDISK */
"Az installáló program elökésziti a hard disket  %s. " \
"Elöször a lemez particionálás történik meg," \
"majd fájlrendszer kerül rájuk.",
/* TR_PARTITIONING_DISK */
"Perticionálom a lemezt...",
/* TR_UNABLE_TO_PARTITION */
"Nem lehet particionálni a lemezt.",
/* TR_MAKING_SWAPSPACE */
"A swap elkészitése...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Nem lehet elkésziteni a swap területet.",
/* TR_MAKING_ROOT_FILESYSTEM */
"A root fájlrendszer kialakitása...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Nem lehet kialakitani a root fájlrendszert.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"A root fájlrendszer mountolása...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Nem lehet mountolni a root fájlrendszert.",
/* TR_MAKING_BOOT_FILESYSTEM */
"A boot fájlrendszer kialakitása...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Nem lehet kialakitani a boot fájlredszert.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"A boot fájlredszer mountolása...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Mem lehet mountolni a boot fájlrendszert.",
/* TR_MAKING_LOG_FILESYSTEM */
"A log fájlrendszer kailakitása...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Nem lehet kialakitani a log fájlrendszert.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"A log fájlrendszer mountolása...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Nem lehet mountolni a log fájlrendszert.",
/* TR_MOUNTING_SWAP_PARTITION */
"A swap particio mountolása...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Nem lehet mountolni a swap particiot.",
/* TR_NETWORK_SETUP_FAILED */
"A hálozat beállitása nem sikerült.",
/* TR_NO_TARBALL_DOWNLOADED */
"Nincs letölthetö tar fájl.",
/* TR_INSTALLING_FILES */
"Fájlok telepitése...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Nem lehet a fájlokat telepiteni.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Nem lehet eltávolitani az ideiglenesen letöltött fájlokat.",
/* TR_ERROR_WRITING_CONFIG */
"Huiba a konfigurációs fájl irásakor.",
/* TR_INSTALLING_LILO */
" A LILO installálása...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Nem lehet a LILO-t installálni.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Nem lehet unmountolni a harddisket.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Nem lehet unmountolni a CDROM-ot vagy a floppyt.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Nem lehet kiadni a CDROM.",
/* TR_CONGRATULATIONS */
"Gratulálunk!",
/* TR_CONGRATULATIONS_LONG */
"A SmoothWall szerencsésen installálódott. Kérjük vegye ki a floppy lemezt vagy a " \
"Cd lemezt a számitógépböl. A Setup program fog elindulni, amellyel beállithatjuk az ISDN-t," \
" a hálózati kártyákat és a rendszer jelszavait. Miután a Setup program" \
"befejezödött, a web böngészödbe ird a következöket:  http://smoothwall:81 vagy " \
"https://smoothwall:445 (vagy ami a SmoothWall gép hostneve), és állitsd be  " \
"a tárcsázó hozzáférést(ha szükséges) és a távoli hozzáférést. Ne felejtsd el beállitani " \
"a 'dial' felhasználó jelszavát, ha nem a SmoothWall " \
"'admin' felhasználói vezérlik a tárcsázást.",
/* TR_PRESS_OK_TO_REBOOT */
"Nyomj Ok-t az ujrainditáshoz.",
/* TR_ERROR */
"Hiba",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"A mudulfüggöségek kiszámitása...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Nem lehet a modulfüggöségeket kiszámitani.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Válassz CDROM tipust",
/* TR_SELECT_CDROM_TYPE_LONG */
"Nincs IDE CDROM a rendszerben. Válaszd ki a következök közül " \
"a megfelelöt, hogy a SmoothWall hozzá tudjon férni.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(javasolt %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(javasolt %d)",
/* TR_CONFIGURE_THE_CDROM */
"Állitsd be a CDROM-ot a megfelelö IO cimmel , és/vagy IRQ-val.",
/* TR_INVALID_IO (note extra space) */
"Az IO port érvénytelen. ",
/* TR_INVALID_IRQ */
"Az IRQ érvénytelen.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Nem lehet irni /var/smoothwall/main/settings.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Nem lehet irni  /var/smoothwall/ethernet/settings.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Nem lehet szimlinket késziteni /dev/harddisk.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Nem lehet szimlinket késziteni /dev/harddisk1.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Nem lehet szimlinket késziteni /dev/harddisk2.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Nem lehet szimlinket késziteni /dev/harddisk3.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Unable to create symlink /dev/harddisk4.",

/* net.c */
/* TR_DOWNLOADING */
"Letöltés...",
/* TR_FAILED_TO_DOWNLOAD */
"A letöltés nem sikerült.",
/* TR_ENTER_URL */
"Ird be a SmoothWall tar.gz fájl elérési helyét. " \
"FIGYELEM: DNS nem hozzáférhetö!  'smoothwall.tgz'-vel kell végzödni a cimnek.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"A hálozat beállitása",
/* TR_CONFIGURE_NETWORKING_LONG */
"Most konfigurálnod kell a hálozatot, elöször a megfelelö meghajtót kell betölteni a " \
"GREEN interfész számára. Megteheted ezt a hálozati kártya automatikus felismerésével " \
"vagy a megfelelö driver listábol valo kiválasztásával. Ha több mind egy kártyád van " \
"a késöbbiekben lesz lehetöséged, hogy a többit is konfiguráld" \
" Továbbá megjegyezzük, ha több kártyád van, amely " \
"azonos tipusu mint a GREEN interfész akkkor mindegyik kártyának speciális " \
"paraméterekre van szüksége, minden ilyen tipusu kártyának meg kell adni a paramétereket "  \
"igy minden kártya aktiválódik amikor a GREEN interfészt beállitod.",
/* TR_INTERFACE_FAILED_TO_COME_UP */
"Az interfész nem aktiv.",
/* TR_ENTER_NETWORK_DRIVER */
"Nem sikerült a hálózati kártya automatikus felismerése. Ird be a driver és  " \
"paramétereket a hálózati kártya számára.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Hostnév",
/* TR_NETWORKING */
"Hálózat",
/* TR_DHCP_SERVER_CONFIGURATION */
"DHCP szerver konfiguració",
/* TR_ISDN_CONFIGURATION */
"ISDN konfiguráció",
/* TR_ROOT_PASSWORD */
"\'root\' jelszó",
/* TR_SETUP_PASSWORD */
"\'setup\' jelszó",
/* TR_ADMIN_PASSWORD */
"Admin jelszó",
/* TR_SECTION_MENU */
"Section menu",
/* TR_SELECT_THE_ITEM */
"Válaszd ki mit szeretnél beállitani.",
/* TR_SETUP_FINISHED */
"A Setup kész.  Nyomj Ok -t az ujrainduláshoz .",
/* TR_SETUP_NOT_COMPLETE */
"Az alap beállitás nincs teljesen befejezve. Bizonyosodj meg róla, hogy a Setup program " \
"normálisan befejezödött. Futtasd újra a setup-ot a shellböl.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Ird be a 'root' felhasználó jelszavát. Jelentkezz be ezzel a felhasználóval a parancssori hozzáféréshez.",
/* TR_SETTING_ROOT_PASSWORD */
"Beállitom a 'root' jelszót....",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Probléma a 'root' jelszó beállitásával",
/* TR_ENTER_SETUP_PASSWORD */
"Ird be a 'setup' felhasználó jelszavát. Jelentkezz be ezzel a felhasználóval hogy lefuttasd a setup " \
"programot.",
/* TR_SETTING_SETUP_PASSWORD */
"Beállitom a  'setup' jelszót....",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Probléma a 'setup' jelszó beállitásával",
/* TR_ENTER_ADMIN_PASSWORD */
"Ird be a SmoothWall admin jelszavát.  Ezt a felhasználót használjuk " \
"a SmoothWall web adminisztrációs lapjaira történö bejelentkezéshez.",
/* TR_SETTING_ADMIN_PASSWORD */
"Beállitom a SmoothWall admin jelszót....",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Probléma a  SmoothWall admin jelszó beállitásával.",
/* TR_PASSWORD_PROMPT */
"Jelszó:",
/* TR_AGAIN_PROMPT */
"Ismét:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"A jelszó nem lehet üres.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"A jelszavak nem egyeznek.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Ird be a gép hostnevét.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"A hostnév nem lehet üres.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"A hostnév nem tartalmazhat szóközöket.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Nem lehet irni /etc/hostname",

/* isdn.c */
/* TR_GERMAN_1TR6 */
"German 1TR6",
/* TR_EURO_EDSS1 */
"Euro (EDSS1)",
/* TR_LEASED_LINE */
"Bérelt vonal",
/* TR_US_NI1 */
"US NI1",
/* TR_PROTOCOL_COUNTRY */
"Protocol/ország",
/* TR_ISDN_CARD */
"ISDN kártya",
/* TR_MSN_CONFIGURATION */
"Saját telefonszám (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"További modul paraméterek beállitása",
/* TR_RED_IN_USE */
"Az ISDN (vagy egy másik külsö kapcsolat) használatban van.  Nem tudod " \
"konfigurálni az ISDN-t amig a RED interfész aktiv.",
/* TR_ISDN_CONFIGURATION_MENU */
"ISDN configurációs menu",
/* TR_ISDN_STATUS */
"ISDN jelenlegi %s.\n\n" \
"   Protokoll: %s\n" \
"   Kártya: %s\n" \
"   Helyi telefonszám: %s\n\n" \
"Válaszd ki, minek a beállitását akarod megváltoztatni, vagy válaszd a jelenlegi beállitást .",
/* TR_ISDN_NOT_YET_CONFIGURED */
"Az ISDN még nincs konfigurálva. Válaszd ki mit szeretnél beállitani.",
/* TR_ENABLE_ISDN */
"ISDN engedélyezése",
/* TR_DISABLE_ISDN */
"ISDN tiltás",
/* TR_INITIALISING_ISDN */
"ISDN inicializálás...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Nem lehet inicializálni az ISDN-t.",
/* TR_ISDN_NOT_SETUP */
"Az ISDN nincs beállitva. Néhány dolog még nincs kiválasztva.",
/* TR_ISDN_PROTOCOL_SELECTION */
"ISDN protokoll kiválasztás",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Válaszd ki a szükséges ISDN protokollt.",
/* TR_AUTODETECT */
"* AUTODETECT *",
/* TR_ISDN_CARD_SELECTION */
"ISDN kártya kiválasztás",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Válaszd ki a megfelelö ISDN kártyát.",
/* TR_CHECKING_FOR */
"Kipróbálom: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"Nem találtam ISDN kártyát. Lehet hogy szükség van további modul paraméterek megadására" \
"ha a kártya ISA tipusú vagy speciális követelményei vannak.",
/* TR_DETECTED */
"Találtam : %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"Nem találtam ISDN kártyát a gépben. Lehet hogy szükség van további modul paraméterek megadására " \
"ha a kártya ISA tipusú vagy speciális követelményei" \
"vannak.",
/* TR_ENTER_THE_LOCAL_MSN */
"Kérem a saját telefonszámod (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"A telefonszám mezö nem lehet üres.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Néhány ISDN kártya (különösen az ISA kártyák) további modul paramétereket " \
"igényel az IRQ és az IO cim beállitásáshoz. Ha ilyened van " \
"ird be a megfelelö paramétereket. Például: " \
"\"io=0x280 irq=9\". Ezeket az információk lesznek felhasználva a kártya felismerésnél.",

/* networking.c */ 
/* TR_PUSHING_NETWORK_DOWN */
"Hálózat deaktválás...",
/* TR_PULLING_NETWORK_UP */
"Hálózat aktiválás...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"A hálózati konfiguráció tipusa",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Meghajtóprogram és kártya hozzárendelések",
/* TR_ADDRESS_SETTINGS */
"Cim beállitások",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"DNS és  Gateway beállitások",
/* TR_RESTART_REQUIRED */
"\n\nHa kész a konfiguráció, a hálózat újrainditása szükséges.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Jelenlegi beállitás: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Hálózati beállitások menuje",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Válaszd ki a hálózati konfigurációt a SmoothWall részére.  A következö " \
"konfiguráció tipusok azok, amelyekhez ethernet van csatlakoztatva" \
"Ha megváltoztatod ezt a beállitást, akkor szükséges a hálózat újrainditása, " \
"és át kell konfigurálni a hálózati kártya drivereket is.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Lelövöm a helyi hálózatot...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"A konfiguráció egy GREEN interfészre van állitva, " \
"amelyhez már van driver hozzárendelve.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Configure network drivers, and which interface each card " \
"is assigned to.  The current configuration is as follows:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nMeg akarod változtatni ezeket a beállitásokat ?",
/* TR_UNCLAIMED_DRIVER */
"There is an unclaimed ethernet card of type:\n%s\n\n" \
"Ehhez tudod hozzárendelni:",
/* TR_CARD_ASSIGNMENT */
"Kártya hozzárendelés",
/* TR_PROBE */
"Felismerés",
/* TR_SELECT */
"Kiválasztás",
/* TR_NO_UNALLOCATED_CARDS */
"Nem maradt kiválasztatlan kártya, " \
"pedig szükséges lenne. Automatikusan megkerestetheted a szükséges kártyákat,vagy " \
"válassz drivert a listából.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Nem lehet további drivereket találni.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Minden kártya sikeresen hozzárendelve.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Nincs elegendö kártya hozzárendelve.",
/* TR_MANUAL */
"* MANUALIS *",
/* TR_SELECT_NETWORK_DRIVER */
"Válassz hálózati drivert",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Válaszd ki az installált kártyának megfelelö drivert. " \
"ha a MANUALIS-t választod , akkor lehetöség edlesz megadni " \
"a driver modul nevét és a paramétereket. " \
"(Különösen ISA kártyák esetében lehet szükséges)",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Nem lehet betölteni a driver modult.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"A driver modul már be van töltve.",
/* TR_MODULE_PARAMETERS */
"Ird be a modul nevét , és az általa igényelt paramétereket.",
/* TR_LOADING_MODULE */
"A modul betöltése...",
/* TR_WARNING */
"WARNING",
/* TR_WARNING_LONG */
"Ha megváltoztatod ezt az IP cimet, és távolról jelentkeztél be, " \
"a kapcsolat a  Smoothwall géppel meg fog szakadni, és újra kell " \
"csatlakoznod ezen az új IP cimen. Ez egy veszélyes müvelet, " \
"és csak akkor próbáld meg, ha fizikailag is hozzáférsz a géphez, " \
"ha netán valami mégsem sikerülne.g.",
/* TR_SINGLE_GREEN */
"A konfigurációd GREEN interfészre van állitva.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Válaszd ki az interfészt amit át akarsz konfigurálni.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"A konfigurációd nem használ ethernet kártyát " \
" RED interfész számára. A DNS és a  Gateway információk a dialup felhasználók " \
"számára automatikusan konfigurálódnak a tárcsázáskor.",
/* TR_PRIMARY_DNS */
"Elsödleges DNS:",
/* TR_SECONDARY_DNS */
"Másodlagos DNS:",
/* TR_DEFAULT_GATEWAY */
"Default Gateway:",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Kérem a DNS és a gateway információkat. Ezek a beállitások csak akkor használatosakha a DHCP " \
"tiltva van a RED interfészen.",
/* TR_PRIMARY_DNS_CR */
"Elsödleges DNS\n",
/* TR_SECONDARY_DNS_CR */
"Másodlagos DNS\n",
/* TR_DEFAULT_GATEWAY_CR */
"Default Gateway\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"Másodlagos DNS van megadva Elsödleges DNS nélkül",
/* TR_UNKNOWN */
"ISMERETLEN",
/* TR_NO_ORANGE_INTERFACE */
"Nincs ORANGE interfész hozzárendelve.",
/* TR_MISSING_ORANGE_IP */
"Hiányzó IP információ az ORANGE interfész számára.",
/* TR_NO_RED_INTERFACE */
"Nincs RED interfész hozzárendelve.",
/* TR_MISSING_RED_IP */
"Hiányzó IP információ a RED interfész számára.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Kezdöcim:",
/* TR_END_ADDRESS */
"Végcim:",
/* TR_DEFAULT_LEASE */
"Alapértelmezett bérlet (perc):",
/* TR_MAX_LEASE */
"Max bérlet (perc):",
/* TR_DOMAIN_NAME_SUFFIX */
"Domain név utótag:",
/* TR_CONFIGURE_DHCP */
"A DHCP server beállitása.",
/* TR_START_ADDRESS_CR */
"Kezdöcim\n",
/* TR_END_ADDRESS_CR */
"Végcim\n",
/* TR_DEFAULT_LEASE_CR */
"Alapértelmezett bérlet\n",
/* TR_MAX_LEASE_CR */
"Max bérlet\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Domain név utótag\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Billentyüzet kezelés",
/* TR_KEYBOARD_MAPPING_LONG */
"Válaszd ki a billentyüzet tipusát az alábbi listából.",

/* timezone.c */
/* TR_TIMEZONE */
"Idözona",
/* TR_TIMEZONE_LONG */
"Válaszd ki a megfelelö idözonát az alábbi listából.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Válaszd ki az  USB vezérlöt",
/* TR_USBADSL_STATUS */
"USB ADSL jelenleg: %s\n" \
"   Vezérlö: %s\n\n" \
"Válaszd ki azt a tételt amit konfigurálni szertenéz, vagy válaszd a jelenlegi beállitásokat.",
/* TR_USBADSL_CONFIGURATION */
"USB ADSL konfiguráció",
/* TR_ENABLE_USBADSL */
"USB ADSL engedélyezés",
/* TR_DISABLE_USBADSL */
"USB ADSL tiltás",
/* TR_INITIALISING_USBADSL */
"USB ADSL inicializálás.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"Nem lehet inicializálni az USB ADSL-t",
/* TR_USBADSL_NOT_SETUP */
"A USB ADSL nincs beállitva.",
/* TR_USB_CONTROLLER_SELECTION */
"USB controller kiválasztás",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Válaszd ki az instllált USB controller.",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"Nem találtam USB controllert.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"Nem találtam USB kontrollert.",
/* TR_STARTING_UP_USBADSL */
"Az  USB ADS inditásaL..."

};
