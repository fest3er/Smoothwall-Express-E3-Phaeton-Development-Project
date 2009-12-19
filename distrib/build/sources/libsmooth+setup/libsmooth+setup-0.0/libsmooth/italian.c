/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * (c) Italian Translation Team:
 * Giuseppe Marullo
 * Raphael Vincenti
 *
 * filename: italian.c
 * Contains italian strings. */
      
#include "libsmooth.h"
      
char *italian_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Annulla",
/* TR_INSTALLATION_CANCELED */
"Installazione interrotta.",
/* TR_HELPLINE */
"              <Tab>/<Alt-Tab> per muoversi   |  <Spazio> seleziona",
/* TR_QUIT */
"Esci",
/* TR_DISABLED */
"Disabilitato",
/* TR_ENABLED */
"Abilitato",
/* TR_UNSET */
"Non impostato",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Impossibile aprire il file delle impostazioni",
/* TR_DONE */
"Eseguito",
/* TR_PROBE_FAILED */
"L'auto detect non ha funzionato.",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"Indirizzo IP:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Indirizzo di rete:",
/* TR_NETMASK_PROMPT */
"Network mask:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Inserire i dati dell'indirizzo IP",
/* TR_INVALID_FIELDS */
"I seguenti campi non sono validi:\n\n",
/* TR_IP_ADDRESS_CR */
"Indirizzo IP\n",
/* TR_NETWORK_ADDRESS_CR */
"Indirizzo della rete\n",
/* TR_NETWORK_MASK_CR */
"Network mask\n",
/* TR_INTERFACE (%s is interface name) */
"%s interfaccia",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Inserire la configurazione IP per la interfaccia %s.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Sto cercando: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall ha trovato la seguente scheda di rete: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"Il nome del modulo non può essere nullo.",
/* TR_STATIC */
"Statico",
/* TR_DHCP_HOSTNAME */
"Nome host DHCP:",
/* TR_DHCP_HOSTNAME_CR */
"Nome host DHCP\n",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Impossibile scrivere /var/smoothwall/main/hostname.conf",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Impossibile scrivere /etc/hosts.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"Impossibile scrivere /etc/hosts.deny.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Impossibile scrivere /etc/hosts.allow.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Impossibile impostare il nome host.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Benvenuti al programma di installazione di SmoothWall. Visitate il nostro sito " \
"http://www.smoothwall.org.  Selezionando Annulla su una qualsiasi " \
"delle schermate seguenti il computer effettuerà il reboot.",
/* TR_NO_IDE_HARDDISK */
"Non è stato rilevato alcun harddisk IDE.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Seleziona un medium di installatione",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"SmoothWall può essere installato in molti modi. Il più semplice è " \
"quello di usare il CDROM della macchina. Se il computer ne è " \
"sprovvisto, si può installare tramite una altra macchina " \
"che ha i file di installazione disponibili tramite http. " \
"In questo caso è richiesto l'uso del floppy con i driver di rete.",
/* TR_NO_IDE_CDROM */
"Non è stato rilevato alcun cdrom IDE.",
/* TR_INSERT_CDROM */
"Inserisci il CD di SmoothWall nel drive CDROM.",
/* TR_INSERT_FLOPPY */
"Inserisci il floppy dei driver di SmoothWall nel drive del floppy.",
/* TR_PREPARE_HARDDISK */
"Il programma di installazione preparerà l'hardisk IDE su %s. " \
"Come prima cosa il disco verrà partizionato, e quindi verrà creato " \
"il filesystem.",
/* TR_PARTITIONING_DISK */
"Sto partizionando il disco...",
/* TR_UNABLE_TO_PARTITION */
"Impossibile partizionare il disco.",
/* TR_MAKING_SWAPSPACE */
"Sto creando l'area di swap...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Impossibile creare l'area di swap.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Sto creando il filesystem root...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Impossibile creare il filesystem root.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Sto effettuando la mount del filesystem root...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Impossibile effettuare la mount del filesystem root.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Sto creando il filesystem boot...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Impossibile creare il filesystem boot.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Sto effettuando la mount del filesystem boot...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Impossibile effettuare la mount del filesystem boot.",
/* TR_MAKING_LOG_FILESYSTEM */
"Sto creando il filesystem log...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Impossibile creare il filesystem log.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Sto effettuando la mount del filesystem log...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Impossibile effettuare la mount del filesystem log.",
/* TR_MOUNTING_SWAP_PARTITION */
"Sto effettuando la mount della partizione swap...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Impossibile effettuare la mount della partizione swap.",
/* TR_NETWORK_SETUP_FAILED */
"L'impostazione della rete ha fallito.",
/* TR_NO_TARBALL_DOWNLOADED */
"Non è stata scaricata nessuna tarball.",
/* TR_INSTALLING_FILES */
"Sto installando i file...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Impossibile installare i file.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Impossibile rimuovere i file temporanei scaricati.",
/* TR_ERROR_WRITING_CONFIG */
"Impossibile scrivere la configurazione.",
/* TR_INSTALLING_LILO */
"Sto installando LILO...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Impossibile installare LILO.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Impossibile effettuare l'unmount dell'harddisk.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Impossibile effettuare l'unmount del CDROM/floppy.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Non riesco a effettuare l'espulsUnable to eject the CDROM.",
/* TR_CONGRATULATIONS */
"Congratulazioni!",
/* TR_CONGRATULATIONS_LONG */
"SmoothWall è stato installato con successo. Togli floppy e/o CDROM " \
"dal computer. Il programma di configurazione adesso consentirà di " \
"configurare ISDN, schede di rete e le password di sistema. Dopo " \
"aver completato la configurazione, dovresti connetterti a " \
"http://smoothwall:81 oppure https://smoothwall:445 (o " \
"comunque si chiami il tuo SmoothWall), per configurare il dialup " \
"(se richiesto) e l'accesso remoto. Ricordati di impostare una " \
"password per l'utente 'dial', se vuoi che utenti diversi dall'" \
"amministratore siano in grado di controllare il collegamento.",
/* TR_PRESS_OK_TO_REBOOT */
"Premere Ok per effettuare il reboot.",
/* TR_ERROR */
"Errore",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"Sto calcolando le dipendenze dei moduli...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Impossibile calcolare le dipendenze.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Seleziona il tipo di CDROM",
/* TR_SELECT_CDROM_TYPE_LONG */
"Non è stato trovato nessun CDROM IDE in questa macchina. Seleziona " \
"uno dei seguenti driver per accedere al CDROM.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(suggest %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(suggest %d)",
/* TR_CONFIGURE_THE_CDROM */
"Configura il CDROM fornendo l'IO address e/o IRQ.",
/* TR_INVALID_IO (note extra space) */
"L'IO address non è corretto. ",
/* TR_INVALID_IRQ */
"L'IRQ non è corretto.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Impossibile scrivere  /var/smoothwall/main/settings.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Impossibile scrivere /var/smoothwall/ethernet/settings.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Impossibile creare il link simbolico /dev/harddisk.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Impossibile creare il link simbolico /dev/harddisk1.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Impossibile creare il link simbolico /dev/harddisk2.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Impossibile creare il link simbolico /dev/harddisk3.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Impossibile creare il link simbolico /dev/harddisk4.",

/* net.c */
/* TR_DOWNLOADING */
"Sto effettuando il download...",
/* TR_FAILED_TO_DOWNLOAD */
"Impossibile scaricare.",
/* TR_ENTER_URL */
"Inserire l'URL del file tar.gz di SmoothWall. " \
"ATTENZIONE: DNS non disponibile!  Dovrebbe terminare con il file 'smoothwall.tgz'.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Configurazione della rete",
/* TR_CONFIGURE_NETWORKING_LONG */
"Devi configurare la rete caricando il driver corretto per l'interfaccia " \
"di rete VERDE. Puoi farlo utilizzando la funzione auto-probe delle schede " \
"di rete oppure selezionando il driver dalla lista. Se hai più di una scheda " \
"di rete installata, potrai configurare le altre successivamente. Inoltre se " \
"hai più schede dello stesso tipo sulla rete VERDE e ogni scheda richiede " \
"parametri specifici, dovresti fornire i parametri di tutte le schede di " \
"questo tipo, così da attivarle tutte quando configuri la rete VERDE.",
/* TR_INTERFACE_FAILED_TO_COME_UP */
"L'interfaccia non è partita.",
/* TR_ENTER_NETWORK_DRIVER */
"L'auto-detect non è riuscito ad identificare la/le schede di rete. " \
"Inserisci il driver e i parametri opzionali della scheda di rete.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Nome Host",
/* TR_NETWORKING */
"Rete",
/* TR_DHCP_SERVER_CONFIGURATION */
"Configurazione server DHCP",
/* TR_ISDN_CONFIGURATION */
"Configurazione ISDN",
/* TR_ROOT_PASSWORD */
"\'root\' password",
/* TR_SETUP_PASSWORD */
"\'setup\' password",
/* TR_ADMIN_PASSWORD */
"Password di Admin",
/* TR_SECTION_MENU */
"Section menu",
/* TR_SELECT_THE_ITEM */
"Seleziona l'elemento che vuoi configurare.",
/* TR_SETUP_FINISHED */
"La configurazione è terminata.  Premere Ok per effettuare il reboot.",
/* TR_SETUP_NOT_COMPLETE */
"La fase di configurazione non è completa.  Dovresti completare il setup " \
"lanciandolo dalla shell.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Inserire la password dell'utente 'root'. Per accedere da console usa questo utente.",
/* TR_SETTING_ROOT_PASSWORD */
"Sto impostando la password di 'root'....",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Impossibile impostare la password di 'root'.",
/* TR_ENTER_SETUP_PASSWORD */
"Inserire la password dell'utente 'setup'. Per accedere al programma di configurazione " \
"usa questo utente.",
/* TR_SETTING_SETUP_PASSWORD */
"Sto impostando la password dell'utente 'setup'....",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Impossibile impostare la password di 'setup'.",
/* TR_ENTER_ADMIN_PASSWORD */
"Inserire la password dell'utente admin.  Questo utente serve per " \
"accedere alle pagine web di gestione di SmoothWall.",
/* TR_SETTING_ADMIN_PASSWORD */
"Sto impostando la password dell'utente admin....",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Impossibile impostare la password di 'admin'.",
/* TR_PASSWORD_PROMPT */
"Password:",
/* TR_AGAIN_PROMPT */
"Ancora:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"La password non può essere vuota.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"Le password non sono uguali.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Inserire l'hostname della macchina.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"Il nome host non può essere vuoto.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"Il nome host non può contenere spazi.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Impossibile scrivere /etc/hostname",

/* isdn.c */
/* TR_GERMAN_1TR6 */
"1TR6 Tedesca",
/* TR_EURO_EDSS1 */
"Euro (EDSS1)",
/* TR_LEASED_LINE */
"Linea Dedicata",
/* TR_US_NI1 */
"US NI1",
/* TR_PROTOCOL_COUNTRY */
"Protocollo/Nazione",
/* TR_ISDN_CARD */
"Scheda ISDN",
/* TR_MSN_CONFIGURATION */
"Numero telefonico locale (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Imposta ulteriori parametri del modulo",
/* TR_RED_IN_USE */
"L'ISDN (o altre connessioni esterne) è attualmente utlizzata.  Non è possibile " \
"configurare l'ISDN mentre l'interfaccia di rete ROSSA è attiva.",
/* TR_ISDN_CONFIGURATION_MENU */
"Menu di configurazione ISDN",
/* TR_ISDN_STATUS */
"La ISDN attualmente è %s.\n\n" \
"   Protocollo: %s\n" \
"   Scheda: %s\n" \
"   Numero di telefono locale: %s\n\n" \
"Selezione l'elemento da riconfigurare, oppure scegli di utilizzare le impostazioni correnti.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"L'ISDN non è stata ancora configurata. Seleziona l'elemento da configurare.",
/* TR_ENABLE_ISDN */
"Abilita l'ISDN",
/* TR_DISABLE_ISDN */
"Disabilita l'ISDN",
/* TR_INITIALISING_ISDN */
"Sto Inizializzando l'ISDN...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Impossibile inizializzare l'ISDN.",
/* TR_ISDN_NOT_SETUP */
"L'ISDN non è configurata. Alcuni elementi non sono stati selezionati.",
/* TR_ISDN_PROTOCOL_SELECTION */
"Selezione del protocollo ISDN",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Scegli il protocollo ISDN.",
/* TR_AUTODETECT */
"* AUTODETECT *",
/* TR_ISDN_CARD_SELECTION */
"Selezione della scheda ISDN",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Seleziona la scheda ISDN installata in questo computer.",
/* TR_CHECKING_FOR */
"Sto cercando: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"La scheda ISDN non è stata rilevata. Potresti avere bisogno di specificare " \
"ulteriori parametri se è una scheda ISA or se ha particolari caratteristiche.",
/* TR_DETECTED */
"Rilevata come: %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"La scheda ISDN non è stata rilevata. Potresti avere bisogno di specificare " \
"ulteriori parametri se è una scheda ISA or se ha particolari caratteristiche.",
/* TR_ENTER_THE_LOCAL_MSN */
"Specifica il numero di telefono locale (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"Il numero di telefono deve essere specificato.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Alcune schede ISDN (specialmente quelle ISA) possono richiedere " \
"ulteriori parametri per impostare IRQ e indirizzi di IO. Se hai una di " \
"queste schede, inserisci ora questi parametri. Per esempio: " \
"\"io=0x280 irq=9\". Verranno usati per rilevare correttamente la scheda.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Sto disabilitando la rete...",
/* TR_PULLING_NETWORK_UP */
"Sto abilitando la rete...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Tipo di configurazione di rete",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Assegnazioni di driver e schede",
/* TR_ADDRESS_SETTINGS */
"Impostazioni ",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"Impostazioni del Gateway e del DNS",
/* TR_RESTART_REQUIRED */
"\n\nAlla fine della configurazione, la rete dovrà essere fatta ripartire.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Configurazione attuale: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Menu di configurazione di rete",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Seleziona la configurazione di rete di SmoothWall. Le seguenti " \
"configurazioni elencano le interfaccie ethernet. " \
"Se cambi le impostazioni, sarà necessario fare ripartire la rete, e dovrai " \
"riconfigurare l'assegnamento dei driver.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Pushing non local network down...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"La tua configurazione è impostata per una singola scheda di rete VERDE, " \
"alla quale è già stato assegnato un driver.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Configura i driver di rete, e a quale interfaccia ogni scheda è assegnata" \
".  La configurazione attuale è:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nVuoi cambiare queste impostazioni?",
/* TR_UNCLAIMED_DRIVER */
"Manca ancora l'assegnazione di una scheda ethernet di tipo:\n%s\n\n" \
"Puoi assegnarla a:",
/* TR_CARD_ASSIGNMENT */
"Card assignment",
/* TR_PROBE */
"Probe",
/* TR_SELECT */
"Seleziona",
/* TR_NO_UNALLOCATED_CARDS */
"Tutte le schede sono state assegnate, " \
"ne servono ancora. Puoi effettuare l'autodetect per cercare le altre schede, oppure " \
"scegliere di selezionare un driver dalla lista.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Impossibile trovare altri driver.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Tutte le schede sono state assegnate.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Non sono state allocate abbastanza schede.",
/* TR_MANUAL */
"* MANUALE *",
/* TR_SELECT_NETWORK_DRIVER */
"Seleziona il driver di rete",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Seleziona il driver di rete per la scheda installata in questa macchina. " \
"Se selezioni MANUALE, potrai inserire il nome del modulo e i parametri " \
"per i driver che hanno caratteristiche particolari come le schede ISA.",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Impossibile caricare il modulo driver.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"Questo modulo driver è già caricato.",
/* TR_MODULE_PARAMETERS */
"Inserisci il nome del modulo e i parametri che il driver richiede.",
/* TR_LOADING_MODULE */
"Sto caricando il modulo...",
/* TR_WARNING */
"WARNING",
/* TR_WARNING_LONG */
"Se cambi l'IP address, e sei collegato da remoto, la connessione alla " \
"macchina SmoothWall si interromperà e dovrai ricollegarti utilizzando il " \
"nuovo IP. Questa è una operazione rischiosa e dovrebbe essere eseguita " \
"solo se hai accesso fisico alla macchina, nel caso qualche cosa andasse " \
"storta.",
/* TR_SINGLE_GREEN */
"La tua configurazione è impostata per una sola interfaccia di rete VERDE.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Seleziona l'interfaccia che vuoi riconfigurare.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"La tua configurazione non utilizza una scheda ethernet come interfaccia per la rete ROSSA." \
"  Le impostazioni del DNS e del Gateway saranno configurate automaticamente durante la connessione.",
/* TR_PRIMARY_DNS */
"DNS primario:",
/* TR_SECONDARY_DNS */
"DNS secondario:",
/* TR_DEFAULT_GATEWAY */
"Default Gateway:",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Inserisci la configurazione del  DNS e del Gateway.  Queste impostazioni sono utilizzate solo se il DHCP " \
"è disabilitato sull'interfaccia ROSSA.",
/* TR_PRIMARY_DNS_CR */
"DNS primario\n",
/* TR_SECONDARY_DNS_CR */
"DNS secondario\n",
/* TR_DEFAULT_GATEWAY_CR */
"Default Gateway\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"E' stato specificato un DNS secondario senza indicare un DNS primario",
/* TR_UNKNOWN */
"SCONOSCIUTO",
/* TR_NO_ORANGE_INTERFACE */
"Non è stata assegnata l'interfaccia ARANCIONE.",
/* TR_MISSING_ORANGE_IP */
"Manca la configurazione IP dell'interfaccia ARANCIONE.",
/* TR_NO_RED_INTERFACE */
"Non è stata assegnata l'interfaccia ROSSA.",
/* TR_MISSING_RED_IP */
"Manca la configurazione IP delle interfaccia ROSSA.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Indirizzo iniziale:",
/* TR_END_ADDRESS */
"Indirizzo finale:",
/* TR_DEFAULT_LEASE */
"Durata di default della lease:",
/* TR_MAX_LEASE */
"Durata massima della lease:",
/* TR_DOMAIN_NAME_SUFFIX */
"Suffisso DNS:",
/* TR_CONFIGURE_DHCP */
"Imposta il server DHCP inserendo i parametri di configurazione.",
/* TR_START_ADDRESS_CR */
"Indirizzo iniziale\n",
/* TR_END_ADDRESS_CR */
"Indirizzo finale\n",
/* TR_DEFAULT_LEASE_CR */
"Durata di default della lease\n",
/* TR_MAX_LEASE_CR */
"Durata massima della lease\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Suffisso DNS\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Tipo della tastiera",
/* TR_KEYBOARD_MAPPING_LONG */
"Scegli il tipo di tastiera attualmente in uso dalla lista.",

/* timezone.c */
/* TR_TIMEZONE */
"Timezone",
/* TR_TIMEZONE_LONG */
"Scegli il timezone attualmente in uso dalla lista.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Scegli il controller USB",
/* TR_USBADSL_STATUS */
"Il supporto ADSL USB è: %s\n" \
"   Controller: %s\n\n" \
"Scegli l'elemento da riconfigurare, o scegli di utlizzare le impostazioni correnti.",
/* TR_USBADSL_CONFIGURATION */
"Configurazione dell'ADSL USB",
/* TR_ENABLE_USBADSL */
"Abilita l'ADSL USB",
/* TR_DISABLE_USBADSL */
"Disabilita l'ADSL USB",
/* TR_INITIALISING_USBADSL */
"Sto inizializzando l'ADSL USB.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"Impossibile inizializzare l'ADSL USB",
/* TR_USBADSL_NOT_SETUP */
"L'USB ADSL not è configurata.",
/* TR_USB_CONTROLLER_SELECTION */
"Selezione del controller USB",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Seleziona il controller USB presente nella macchina SmoothWall.",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"Controller USB non rilevato.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"Impossibile trovare un controller USB.",
/* TR_STARTING_UP_USBADSL */
"Sto facendo partire l'ADSL USB..."

};
