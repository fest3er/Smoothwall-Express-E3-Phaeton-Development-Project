/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * (c) Finnish Translation Team:
 * Jussi Siponen
 * Pasi Parkkinen
 *
 * filename: finnish.c
 * Contains finnish strings. */

#include "libsmooth.h"

char *finnish_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Peruuta",
/* TR_INSTALLATION_CANCELED */
"Asennus keskeytetty.",
/* TR_HELPLINE */
"        <Tab>/<Alt-Tab> elementtien välillä   |  <Välilyönti> valitsee",
/* TR_QUIT */
"Poistu",
/* TR_DISABLED */
"Ei käytössä",
/* TR_ENABLED */
"Käytössä",
/* TR_UNSET */
"EI ASETETTU",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Asetustiedostoa ei voi avata",
/* TR_DONE */
"Valmis",
/* TR_PROBE_FAILED */
"Automaattinen tunnistus epäonnistui.",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"IP-osoite:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Verkko-osoite:",
/* TR_NETMASK_PROMPT */
"Aliverkon peite:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Anna IP-osoite -asetukset",
/* TR_INVALID_FIELDS */
"Näiden kenttien arvot eivät kelpaa:\n\n",
/* TR_IP_ADDRESS_CR */
"IP-osoite\n",
/* TR_NETWORK_ADDRESS_CR */
"Verkko-osoite\n",
/* TR_NETWORK_MASK_CR */
"Aliverkon peite\n",
/* TR_INTERFACE (%s is interface name) */
"%s verkkoliitäntä",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Anna IP-asetukset verkkoliitännälle %s.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Etsin: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall on löytänyt verkkosovittimen: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"Moduulin nimi on annettava.",
/* TR_STATIC */
"Kiinteä",
/* TR_DHCP_HOSTNAME */
"DHCP-palvelin:",
/* TR_DHCP_HOSTNAME_CR */
"DHCP-palvelin\n",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Tiedostoa /var/smoothwall/main/hostname.conf ei voida kirjoittaa.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Tiedostoa /etc/hosts ei voida kirjoittaa.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"Tiedostoa /etc/hosts.deny ei voida kirjoittaa.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Tiedostoa /etc/hosts.allow ei voida kirjoittaa.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Isäntänimeä ei voida asettaa.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Tervetuloa SmoothWall asennusohjelmaan. Pyydämme käymään kotisivuillamme " \
"osoitteessa http://www.smoothwall.org. Jos valitset Peruuta " \
"seuraavilla näytöillä, tietokone käynnistetään uudelleen.",
/* TR_NO_IDE_HARDDISK */
"Yhtään IDE kiintolevyä ei löytynyt.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Valitse asennusmedia",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"SmoothWall voidaan asentaa useista eri lähteistä.  Helpointa on " \
"käyttää tietokoneen CDROM-asemaa. Jos tietokoneessa ei ole CDROM-asemaa, voit " \
"asentaa toiselta verkossa olevalta tietokoneelta, jolta asennustiedostot " \
"noudetaan HTTP-protokollalla. Tässä tapauksessa tarvitset verkkosovittimen " \
"laiteohjainlevykkeen.",
/* TR_NO_IDE_CDROM */
"Yhtään IDE CDROM-asemaa ei löytynyt.",
/* TR_INSERT_CDROM */
"Laita SmoothWall CD CDROM-asemaan.",
/* TR_INSERT_FLOPPY */
"Laita SmoothWall laiteohjainlevyke levykeasemaan.",
/* TR_PREPARE_HARDDISK */
"Asennusohjelma valmistelee IDE kiintolevyn %s. " \
"Aluksi levy osioidaan ja sitten osioille luodaan " \
"tiedostojärjestelmät.",
/* TR_PARTITIONING_DISK */
"Osioidaan levyä...",
/* TR_UNABLE_TO_PARTITION */
"Levyn osiointi epäonnistui.",
/* TR_MAKING_SWAPSPACE */
"Luodaan swap-osiota...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Swap-osion luominen epäonnistui.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Luodaan root-tiedostojärjestelmä...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"root-tiedostojärjestelmän luominen epäonnistui.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Liitän root-tiedostojärjestelmän...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"root-tiedostojärjestelmän liittäminen epäonnistui.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Luodaan boot-tiedostojärjestelmä...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"boot-tiedostojärjestelmän luominen epäonnistui.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Liitän boot-tiedostojärjestelmän...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"boot-tiedostojärjestelmän liittäminen epäonnistui.",
/* TR_MAKING_LOG_FILESYSTEM */
"Luodaan log-tiedostojärjestelmä...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"log-tiedostojärjestelmän luominen epäonnistui.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Liitän log-tiedostojärjestelmän...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"log-tiedostojärjestelmän liittäminen epäonnistui.",
/* TR_MOUNTING_SWAP_PARTITION */
"Liitän swap-osion...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"swap-osion liittäminen epäonnistui.",
/* TR_NETWORK_SETUP_FAILED */
"Verkkoasennus epäonnistui.",
/* TR_NO_TARBALL_DOWNLOADED */
"Tar-pakettia ei ladattu.",
/* TR_INSTALLING_FILES */
"Kopioidaan tiedostoja...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Tiedostojen kopiointi epäonnistui.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Väliaikaistiedostojen poistaminen epäonnistui.",
/* TR_ERROR_WRITING_CONFIG */
"Asetustietojen kirjoittaminen epäonnistui.",
/* TR_INSTALLING_LILO */
"Asennetaan LILO...",
/* TR_UNABLE_TO_INSTALL_LILO */
"LILOn asennus epäonnistui.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Kiintolevyn irroittaminen epäonnistui.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"CDROM/levykkeen irroittaminen epäonnistui.",
/* TR_UNABLE_TO_EJECT_CDROM */
"CDROM-levyn poistaminen epäonnistui.",
/* TR_CONGRATULATIONS */
"Onneksi olkoon!",
/* TR_CONGRATULATIONS_LONG */
"SmoothWall on nyt asennettu. Poista levykkeet ja " \
"CDROM:t asemista. Vuorossa on nyt asetusten tekeminen (ISDN, " \
"verkkosovittimet ja salasanat). Kun asennus on valmis, käy hallintasivuilla " \
"osoitteessa http://smoothwall:81 tai https://smoothwall:445 " \
"(korvaa 'smoothwall' asennuksessa käyttämälläsi isäntänimellä), " \
"jos sinun tarvitsee luoda puhelinverkkoyhteyden asetukset. Muista antaa " \
"SmoothWallin 'dial'-käyttäjälle salasana, jos tahdot muidenkin kuin " \
"pääkäyttäjän voivan avata ja sulkea yhteyden.",
/* TR_PRESS_OK_TO_REBOOT */
"Paina OK käynnistääksesi tietokoneen uudelleen.",
/* TR_ERROR */
"Virhe",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"Tarkistetaan moduulien riippuvuussuhteet...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Riippuvuussuhteiden tarkistus epäonnistui.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Valitse CDROMin tyyppi",
/* TR_SELECT_CDROM_TYPE_LONG */
"Yhtään IDE CDROM-asenaa ei löytynyt.  Valitse, mitä näistä laiteohjaimista " \
"haluat SmoothWallin käyttävän CDROM-aseman ohjaamiseen.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(ehdotus %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(ehdotus %d)",
/* TR_CONFIGURE_THE_CDROM */
"Valitse CDROMin käyttämä IO-osoite ja/tai IRQ (keskeytys).",
/* TR_INVALID_IO (note extra space) */
"IO-osoite ei kelpaa. ",
/* TR_INVALID_IRQ */
"IRQ ei kelpaa.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Tiedostoa /var/smoothwall/main/settings ei voida kirjoittaa.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Tiedostoa /var/smoothwall/ethernet/settings ei voida kirjoittaa.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Symbolista linkkiä /dev/harddisk ei voida luoda.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Symbolista linkkiä /dev/harddisk1 ei voida luoda.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Symbolista linkkiä /dev/harddisk2 ei voida luoda.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Symbolista linkkiä /dev/harddisk3 ei voida luoda.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Symbolista linkkiä /dev/harddisk4 ei voida luoda.",

/* net.c */
/* TR_DOWNLOADING */
"Lataan...",
/* TR_FAILED_TO_DOWNLOAD */
"Lataus epäonnistui.",
/* TR_ENTER_URL */
"Anna SmoothWall tar.gz-tiedoston URL. " \
"VAROITUS: DNS ei ole käytettävissä. Osoitteen tulee päättyä " \
"tiedostonimeen 'smoothwall.tgz'.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Verkkoasetukset",
/* TR_CONFIGURE_NETWORKING_LONG */
"Aloita verkkoasetusten teko lataamalla oikea laiteohjain GREEN-verkkoliitännälle. "  \
"Asennus voi tunnustella sovittimia puolestasi tai voit valita ohjaimen listasta. "  \
"Huomaa että jos koneessa on useita verkkosovittimia, tapahtuu loppujen sovitinten "  \
"käyttönotto myöhemmin. Ota myös huomioon, että jos koneessa on muita "  \
"GREEN-liitännän sovittimen kanssa samantyyppisiä sovittimia ja annat niiden "  \
"laiteohjaimille lisäasetuksia, on näiden lisäasetusten oltava sellaisia että "  \
"kaikki nämä sovittimet voivat käynnistyä samanaikaisesti GREEN-liitännän "  \
"kanssa.",
/* TR_INTERFACE_FAILED_TO_COME_UP */
"Verkkoliitännän käynnistys epäonnistui.",
/* TR_ENTER_NETWORK_DRIVER */
"Verkkosovittimen tunnistus epäonnistui. Anna oikea laiteohjain " \
"ja mahdolliset lisäasetukset verkkosovittimelle.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Isäntänimi",
/* TR_NETWORKING */
"Verkkoasetukset",
/* TR_DHCP_SERVER_CONFIGURATION */
"DHCP-palvelimen asetukset",
/* TR_ISDN_CONFIGURATION */
"ISDN-asetukset",
/* TR_ROOT_PASSWORD */
"\'root\' salasana",
/* TR_SETUP_PASSWORD */
"\'setup\' salasana",
/* TR_ADMIN_PASSWORD */
"Pääkäyttäjän salasana",
/* TR_SECTION_MENU */
"Valikko",
/* TR_SELECT_THE_ITEM */
"Valitse kohde, jonka asetuksia haluat muuttaa.",
/* TR_SETUP_FINISHED */
"Asennus valmis. Paina OK käynnistääksesi tietokoneen uudelleen.",
/* TR_SETUP_NOT_COMPLETE */
"Asennusta ei suoritettu loppuun. Suorita komento setup komentoriviltä " \
"varmistaaksesi asennuksen onnistumisen.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Anna käyttäjän 'root' salasana. Käytä tätä tunnusta komentojen " \
"suorittamiseen komentoriviltä.",
/* TR_SETTING_ROOT_PASSWORD */
"Asetaan käyttäjän 'root' salasana...",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Käyttäjän 'root' salasanan asettaminen epäonnistui.",
/* TR_ENTER_SETUP_PASSWORD */
"Anna käyttäjän 'setup' salasana. Käytä tätä tunnusta " \
"asennusohjelman suorittamiseen.",
/* TR_SETTING_SETUP_PASSWORD */
"Asetaan käyttäjän 'setup' salasana...",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Käyttäjän 'setup' salasanan asettaminen epäonnistui.",
/* TR_ENTER_ADMIN_PASSWORD */
"Anna SmoothWall pääkäyttäjän salasana. Käytä tätä tunnusta " \
"SmoothWallin web-hallintaan kirjautumiseen.",
/* TR_SETTING_ADMIN_PASSWORD */
"Asetetaan SmoothWall pääkäyttäjän salasana...",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"SmoothWall pääkäyttäjän salasanan asettaminen epäonnistui.",
/* TR_PASSWORD_PROMPT */
"Salasana:",
/* TR_AGAIN_PROMPT */
"Salasana uudestaan:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"Salasana ei voi olla tyhjä.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"Salasanat eivät täsmää.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Anna koneen isäntänimi.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"Isäntänimi ei voi olla tyhjä.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"Isäntänimessä ei voi olla välilyöntejä.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Tiedostoa /etc/hostname ei voida kirjoittaa.",

/* isdn.c */
/* TR_GERMAN_1TR6 */
"Saksalainen 1TR6",
/* TR_EURO_EDSS1 */
"Euro (EDSS1)",
/* TR_LEASED_LINE */
"Kiinteä linja",
/* TR_US_NI1 */
"US NI1",
/* TR_PROTOCOL_COUNTRY */
"Protokolla/Maa",
/* TR_ISDN_CARD */
"ISDN-sovitin",
/* TR_MSN_CONFIGURATION */
"Oma puhelinnumero (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Moduulin lisäasetukset",
/* TR_RED_IN_USE */
"ISDN (tai muu ulkoinen yhteys) on käytössä. Et voi muuttaa " \
"ISDN-asetuksia, kun RED-verkkoliitäntä on aktiivinen.",
/* TR_ISDN_CONFIGURATION_MENU */
"ISDN-asetukset",
/* TR_ISDN_STATUS */
"ISDN käyttää %s.\n\n" \
"   Protokolla: %s\n" \
"   Sovitin: %s\n" \
"   Oma puhelinnumero: %s\n\n" \
"Valitse asetus, jonka haluat muuttaa tai hyväksy nykyiset asetukset.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"ISDN-asetukset epätäydelliset. Valitse asetus, jonka haluat muuttaa.",
/* TR_ENABLE_ISDN */
"Ota ISDN käyttöön",
/* TR_DISABLE_ISDN */
"Poista ISDN käytöstä",
/* TR_INITIALISING_ISDN */
"Alustetaan ISDN...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"ISDN alustus epäonnistui.",
/* TR_ISDN_NOT_SETUP */
"ISDN-asennus ei ole valmis. Joitain asetuksia ei ole annettu.",
/* TR_ISDN_PROTOCOL_SELECTION */
"ISDN-protokollan valinta",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Valitse käytetty ISDN-protokolla.",
/* TR_AUTODETECT */
"* TUNNISTA AUTOMAATTISESTI *",
/* TR_ISDN_CARD_SELECTION */
"ISDN-sovittimen valinta",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Valitse käyttämäsi ISDN-sovitin.",
/* TR_CHECKING_FOR */
"Etsitään: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"Yhtään ISDN-sovitinta ei löytynyt. Moduuli saattaa vaatia lisäasetuksia " \
"jos sovitin on ISA-liitäntäinen tai sovitin muuten vaatii niitä.",
/* TR_DETECTED */
"Löydettin sovitin: %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"Yhtään ISDN-sovitinta ei löytynyt. Moduuli saattaa vaatia lisäasetuksia " \
"jos sovitin on ISA-liitäntäinen tai sovitin muuten vaatii niitä.",
/* TR_ENTER_THE_LOCAL_MSN */
"Anna oma puhelinnumero (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"Puhelinnumero ei voi olla tyhjä.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Jotkin ISDN-sovittimet (varsinkin ISA-liitäntäiset) saattavat vaatia " \
"lisäasetuksia IRQ ja IO-osoitteen osalta. Jos sinulla on " \
"tällainen ISDN-sovitin, anna lisäasetukset tähän. Esimerkiksi: " \
"\"io=0x280 irq=9\". Lisäasetuksia käytetään sovittimen tunnistuksessa.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Sammutetaan verkko-ohjelmisto...",
/* TR_PULLING_NETWORK_UP */
"Käynnistetään verkko-ohjelmisto...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Verkkoasetuksen tyyppi",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Laiteohjaimet ja verkkosovittimet",
/* TR_ADDRESS_SETTINGS */
"Osoitteet",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"DNS ja yhdyskäytävä",
/* TR_RESTART_REQUIRED */
"\n\nKun asetukset on tehty, verkko-ohjelmisto on käynnistettävä uudelleen.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Nykyinen asetus: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Verkkoasetukset",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Valitse SmoothWallin käyttämä verkkoasetus. Valitse listasta, " \
"missä verkkoliitännöissä käytetään ethernet-sovittimia. " \
"Valinnan muuttaminen vaatii verkko-ohjelmiston uudelleenkäynnistyksen " \
"ja nollaa tehdyt laiteohjainvalinnat.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Sammutetaan ulkoinen verkko...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"Tämä asetus käyttää yhtä GREEN-liitäntää, " \
"jolle on jo laiteohjain.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Valitse laiteohjaimet ja kohdista verkkosovittimet " \
"eri liitännöille.  Nykyiset asetukset:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nHaluatko muuttaa näitä asetuksia?",
/* TR_UNCLAIMED_DRIVER */
"Löytyi kohdistamaton ethernet-sovitin:\n%s\n\n" \
"Voit kohdistaa sen:",
/* TR_CARD_ASSIGNMENT */
"Liitännän kohdistus",
/* TR_PROBE */
"Tunnustele",
/* TR_SELECT */
"Valitse",
/* TR_NO_UNALLOCATED_CARDS */
"Kohdistamattomia sovittimia ei löytynyt, " \
"niitä tarvitaan enemmän. Voit tunnustella verkkosovittimia tai " \
"valita laiteohjaimen listasta.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Uusia verkkosovittimia ei löytynyt.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Kaikki verkkosovittimet kohdistettu.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Liian vähän kohdistettuja verkkosovittimia.",
/* TR_MANUAL */
"* KÄSIN *",
/* TR_SELECT_NETWORK_DRIVER */
"Valitse laiteohjain",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Valitse verkkosovittimen käyttämä laiteohjain. " \
"Valita KÄSIN mahdollistaa laitohjainmoduulin nimen ja lisäasetusten " \
"syöttämisen. Tämä voi olla tarpeen joidenkin sovittimien laiteohjainten " \
"kanssa (jotkin ISA-liitäntäiset sovittimet).",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Laiteohjaimen lataus epäonnistui.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"Laiteohjain on jo ladattu.",
/* TR_MODULE_PARAMETERS */
"Anna moduulin nimi ja tarvittaessa lisäasetukset.",
/* TR_LOADING_MODULE */
"Lataan moduulia...",
/* TR_WARNING */
"VAROITUS",
/* TR_WARNING_LONG */
"Jos muutat tämän IP-osoitteen ja olet etäyhteydessä koneeseen, " \
"yhteys katkeaa ja joudut ottamaan uudestaan yhteyttä uuteen IP-osoitteeseen. " \
"Tämä on vaarallinen toimenpide ja sitä tulisi yrittää vain jos " \
"pääset mahdollisessa virhetilanteessa kirjautumaan SmoothWall-koneen " \
"omalle konsolille.",
/* TR_SINGLE_GREEN */
"Tämä asetus käyttää yhtä GREEN-liitäntää.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Valitse liitäntä, jonka asetuksia haluat muuttaa.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"Tämä kokoonpano ei käytä ethernet-sovitinta RED-verkkoliitännässä. " \
"DNS- ja Oletusyhdyskäytävä -asetukset tehdään automaattisesti " \
"yhteyttä luotaessa.",
/* TR_PRIMARY_DNS */
"Ensisijainen DNS:",
/* TR_SECONDARY_DNS */
"Toissijainen DNS:",
/* TR_DEFAULT_GATEWAY */
"Oletusyhdyskäytävä:",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Anna DNS- ja oletusyhdyskäytävä-asetukset. Näitä asetuksia käytetään vain, " \
"jos RED-liitäntä ei käytä DHCPtä.",
/* TR_PRIMARY_DNS_CR */
"Ensisijainen DNS\n",
/* TR_SECONDARY_DNS_CR */
"Toissijainen DNS\n",
/* TR_DEFAULT_GATEWAY_CR */
"Oletusyhdyskäytävä\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"Toissijainen DNS annettu, mutta ensisijainen DNS puuttuu",
/* TR_UNKNOWN */
"TUNTEMATON",
/* TR_NO_ORANGE_INTERFACE */
"ORANGE-liitäntää ei ole annettu.",
/* TR_MISSING_ORANGE_IP */
"ORANGE-liitännän IP-asetuksia ei ole annettu.",
/* TR_NO_RED_INTERFACE */
"RED-liitäntää ei ole annettu.",
/* TR_MISSING_RED_IP */
"RED-liitännän IP-asetuksia ei ole annettu.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Ensimmäinen osoite:",
/* TR_END_ADDRESS */
"Viimeinen osoite:",
/* TR_DEFAULT_LEASE */
"Istunnon oletuskestoaika:",
/* TR_MAX_LEASE */
"Istunnon kestoaika enintään:",
/* TR_DOMAIN_NAME_SUFFIX */
"Toimialueen tunnus:",
/* TR_CONFIGURE_DHCP */
"Anna DHCP-palvelimen asetukset.",
/* TR_START_ADDRESS_CR */
"Ensimmäinen osoite\n",
/* TR_END_ADDRESS_CR */
"Viimeinen osoite\n",
/* TR_DEFAULT_LEASE_CR */
"Oletusistunto\n",
/* TR_MAX_LEASE_CR */
"Istunto enintään\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Toimialueen tunnus\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Näppäimistökartta",
/* TR_KEYBOARD_MAPPING_LONG */
"Valitse käyttämäsi näppäimistö alla olevasta listasta.",

/* timezone.c */
/* TR_TIMEZONE */
"Aikavyöhyke",
/* TR_TIMEZONE_LONG */
"Valitse aikavyöhyke alla olevasta listasta.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Valitse USB-ohjain",
/* TR_USBADSL_STATUS */
"USB ADSL käyttää: %s\n" \
"   Ohjain: %s\n\n" \
"Valitse asetus, jonka haluat muuttaa tai hyväksy nykyiset asetukset.",
/* TR_USBADSL_CONFIGURATION */
"USB ADSL-asetukset",
/* TR_ENABLE_USBADSL */
"Ota USB ADSL käyttöön",
/* TR_DISABLE_USBADSL */
"Poista USB ADSL käytöstä",
/* TR_INITIALISING_USBADSL */
"Alustetaan USB ADSL.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"USB ADSL alustus epäonnistui",
/* TR_USBADSL_NOT_SETUP */
"USB ADSL asetuksia ei ole annettu.",
/* TR_USB_CONTROLLER_SELECTION */
"Valitse USB-ohjain",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Valitse käytettävä USB-ohjain.",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"USB-ohjainta ei löytynyt.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"USB-ohjainta ei löytynyt.",
/* TR_STARTING_UP_USBADSL */
"Käynnistän USB ADSL..."

};
