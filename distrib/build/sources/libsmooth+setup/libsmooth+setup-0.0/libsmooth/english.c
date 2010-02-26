/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: english.c
 * Contains english strings. */

#include "libsmooth.h"

char *english_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Cancel",
/* TR_INSTALLATION_CANCELED */
"Installation canceled.",
/* TR_HELPLINE */
"              <Tab>/<Alt-Tab> between elements   |  <Space> selects",
/* TR_QUIT */
"Quit",
/* TR_FINISHED */
"Finished",
/* TR_DISABLED */
"Disabled",
/* TR_ENABLED */
"Enabled",
/* TR_UNSET */
"UNSET",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Unable to open settings file",
/* TR_DONE */
"Done",
/* TR_PROBE_FAILED */
"Auto detecting failed.",
/* TR_YES */
"Yes",
/* TR_NO */
"No",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"IP address:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Network address:",
/* TR_NETMASK_PROMPT */
"Network mask:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Enter the IP address information",
/* TR_INVALID_FIELDS */
"The following fields are invalid:\n\n",
/* TR_IP_ADDRESS_CR */
"IP address\n",
/* TR_NETWORK_ADDRESS_CR */
"Network address\n",
/* TR_NETWORK_MASK_CR */
"Network mask\n",
/* TR_INTERFACE (%s is interface name) */
"%s interface",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Enter the IP address information for the %s interface.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Looking for: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall Express has detected the following NIC in your machine: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"Module name cannot be blank.",
/* TR_STATIC */
"Static",
/* TR_DHCP_HOSTNAME */
"DHCP Hostname:",
/* TR_DHCP_HOSTNAME_CR */
"DHCP Hostname\n",
/* TR_SKIP */
"Skip",
/* TR_SKIP_LONG (%s is a nic name) */
"Setup has detected a %s. Do you wish to use this card, or skip over and " \
"look for an alternative?",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Unable to write /var/smoothwall/main/hostname.conf",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Unable to write /etc/hosts.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"Unable to write /etc/hosts.deny.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Unable to write /etc/hosts.allow.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Unable to set hostname.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Welcome to the SmoothWall Express installation program. Please visit our " \
"homepage at http://www.smoothwall.org.  Selecting Cancel on any " \
"of the following screens will reboot the computer.",
/* TR_NO_HARDDISK */
"No harddisk found.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Select installation media",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"SmoothWall Express can be installed from multiple sources.  The simplest is to " \
"use the machines CDROM drive. If the computer lacks a drive, you may " \
"install via another machine on the LAN which has the installation files " \
"available via HTTP.",
/* TR_NO_IDE_CDROM */
"No IDE cdrom found.",
/* TR_INSERT_CDROM */
"Please insert the SmoothWall Express CD in the CDROM drive.",
/* TR_INSERT_FLOPPY */
"Please insert the SmoothWall Express driver diskette in the floppy drive.",
/* TR_PREPARE_HARDDISK */
"The installation program will now prepare the harddisk on %s. " \
"First the disk will be partitioned, and then the partitions will have " \
"a filesystem put on them.",
/* TR_PREPARE_HARDDISK_WARNING */
"!!! WARNING !!!",
/* TR_PREPARE_HARDDISK_WARNING_LONG */
"You are about to PERMANENTLY ERASE the contents of the harddisk. This will " \
"OVERWRITE ALL DATA ON ALL PARTITIONS on your harddisk. If you " \
"would like to continue with the installation of SmoothWall Express, " \
"press the Ok button. Otherwise press Cancel to ABORT the installation " \
"process. There is NO UNDO FEATURE once this has been done.",
/* TR_PARTITIONING_DISK */
"Partitioning disk...",
/* TR_UNABLE_TO_PARTITION */
"Unable to partition the disk.",
/* TR_MAKING_SWAPSPACE */
"Making swap space...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Unable to make swap space.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Making root filesystem...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Unable to make root filesystem.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Mounting root filesystem...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Unable to mount root filesystem.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Making boot filesystem...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Unable to make boot filesystem.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Mounting boot filesystem...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Unable to mount boot filesystem.",
/* TR_MAKING_LOG_FILESYSTEM */
"Making log filesystem...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Unable to make log filesystem.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Mounting log filesystem...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Unable to mount log filesystem.",
/* TR_MOUNTING_SWAP_PARTITION */
"Mounting swap partition...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Unable to mount the swap partition.",
/* TR_NETWORK_SETUP_FAILED */
"Network setup failed.",
/* TR_NO_TARBALL_DOWNLOADED */
"No tarball downloaded.",
/* TR_INSTALLING_FILES */
"Installing files...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Unable to install files.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Unable to remove temporary downloaded files.",
/* TR_ERROR_WRITING_CONFIG */
"Error writing configuration information.",
/* TR_INSTALLING_LILO */
"Installing LILO...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Unable to install LILO.",
/* TR_SETTING_UP_BOOT_DRIVERS */
"Setting up boot drivers...",
/* TR_UNABLE_TO_SETUP_BOOT_DRIVERS */
"Unable to set up boot drivers.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Unable to unmount harddisk.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Unable to unmount the CDROM/floppydisk.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Unable to eject the CDROM.",
/* TR_CONGRATULATIONS */
"Congratulations!",
/* TR_CONGRATULATIONS_LONG */
"SmoothWall Express was successfully installed. Please remove any floppy disks or " \
"CDROMs in the computer. Setup will now run where you may configure ISDN, " \
"network cards, and the system passwords. After Setup has been " \
"completed, you should point your web browser at http://smoothwall:81 or " \
"https://smoothwall:441 (or whatever you name your SmoothWall Express), and configure " \
"dialup networking (if required) and remote access. Remember to set " \
"a password for the SmoothWall Express 'dial' user, if you wish non SmoothWall Express " \
"'admin' users to be able to control the link.",
/* TR_PRESS_OK_TO_REBOOT */
"Press Ok to reboot.",
/* TR_ERROR */
"Error",
/* TR_CALCULATING_MODULE_DEPENDENCIES */
"Calculating module dependencies...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDENCIES */
"Unable to calculate module dependencies.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Select CDROM type",
/* TR_SELECT_CDROM_TYPE_LONG */
"No IDE CDROM was detected in this machine.  Please select which of the " \
"following drivers you wish to use so that SmoothWall Express can access the CDROM.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(suggest %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(suggest %d)",
/* TR_CONFIGURE_THE_CDROM */
"Configure the CDROM by choosing the appropriate IO address and/or IRQ.",
/* TR_INVALID_IO (note extra space) */
"The IO port details entered are invalid. ",
/* TR_INVALID_IRQ */
"The IRQ details entered are invalid.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Unable to write /var/smoothwall/main/settings.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Unable to write /var/smoothwall/ethernet/settings.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Unable to create symlink /dev/harddisk.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Unable to create symlink /dev/harddisk1.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Unable to create symlink /dev/harddisk2.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Unable to create symlink /dev/harddisk3.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Unable to create symlink /dev/harddisk4.",

/* net.c */
/* TR_DOWNLOADING */
"Downloading...",
/* TR_FAILED_TO_DOWNLOAD */
"Failed to download.",
/* TR_ENTER_URL */
"Enter the URL to the SmoothWall tar.gz file. " \
"WARNING: DNS not available!  This should end with the file 'smoothwall.tgz'.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Configure networking",
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
"Interface failed to come up.",
/* TR_ENTER_NETWORK_DRIVER */
"Failed to detect a network card automatically. Enter the driver and " \
"optional parameters for the network card.",

/* storage.c */
/* TR_STORAGE_DRIVER_SELECT_OR_PROBE */
"Please select whether you wish to auto-detect or manually select the storage driver " \
"for the hardware in your system.",
/* TR_UNABLE_TO_DETECT_ANY_STORAGE_INTERFACE */
"Unable to detect any storage interface.",
/* TR_LOOKING_FOR_STORAGE_INTERFACE (%s is the card) */
"Looking for storage card: %s",
/* TR_SELECT_STORAGE_DRIVER */
"Storage device configuration",
/* TR_SELECT_STORAGE_DRIVER_LONG */
"Please select the correct driver for this system's mass storage device.",
/* TR_FOUND_THE_FOLLOWING_STORAGE_DEVICE */
"The following storage devices were found: %s",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Hostname",
/* TR_WEB_PROXY */
"Web proxy",
/* TR_DEFAULT_SECURITY_LEVEL */
"Default security level",
/* TR_NETWORKING */
"Networking",
/* TR_DHCP_SERVER_CONFIGURATION */
"DHCP server configuration",
/* TR_ISDN_CONFIGURATION */
"ISDN Configuration",
/* TR_ROOT_PASSWORD */
"\'root\' password",
/* TR_SETUP_PASSWORD */
"\'setup\' password",
/* TR_ADMIN_PASSWORD */
"Admin password",
/* TR_SECTION_MENU */
"Section menu",
/* TR_SELECT_THE_ITEM */
"Select the item you wish to configure.",
/* TR_SETUP_FINISHED */
"Setup is complete.  Press Ok to reboot.",
/* TR_SETUP_NOT_COMPLETE */
"Inital setup was not entiraly complete.  You must ensure that Setup is " \
"properly fininshed by running setup again at the shell.",
/* TR_RESTORE_CONFIGURATION */
"Restore configuration",
/* TR_RESTORE_LONG */
"Do you want to restore the configuration for this SmoothWall Express from a " \
"previous installation using a backup floppy disk? If you select " \
"'Yes', the areas which were restored from the backup will be " \
"automatically configured and will not be displayed to you. This can also " \
"be used to restore settings from a previous version of SmoothWall Express.",
/* TR_DO_YOU_WANT_TO_REBOOT */
"A reboot is required to make changed settings active. Do you wish to " \
"reboot now?",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Enter the 'root' user password. Login as this user for commandline access.",
/* TR_SETTING_ROOT_PASSWORD */
"Setting 'root' password....",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Problem setting 'root' password.",
/* TR_ENTER_SETUP_PASSWORD */
"Enter the 'setup' user password. Login as this user to access the setup " \
"program.",
/* TR_SETTING_SETUP_PASSWORD */
"Setting 'setup' password....",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Problem setting 'setup' password.",
/* TR_ENTER_ADMIN_PASSWORD */
"Enter SmoothWall admin password.  This is the user to use for " \
"logging into the SmoothWall web administration pages.",
/* TR_SETTING_ADMIN_PASSWORD */
"Setting SmoothWall admin password....",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Problem setting SmoothWall admin password.",
/* TR_PASSWORD_PROMPT */
"Password:",
/* TR_AGAIN_PROMPT */
"Again:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"Password cannot be blank.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"Passwords do not match.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Enter the machine's hostname.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"Hostname cannot be empty.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"Hostname cannot contain spaces or punctuation other then dots (.) and hyphens (-).",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Unable to write /etc/hostname",

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
"Protocol/Country",
/* TR_ISDN_CARD */
"ISDN card",
/* TR_MSN_CONFIGURATION */
"Local phone number (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Set additional module parameters",
/* TR_RED_IN_USE */
"ISDN (or another external connection) is currently in use.  You may not " \
"configure ISDN while the RED interface is active.",
/* TR_ISDN_CONFIGURATION_MENU */
"ISDN configuration menu",
/* TR_ISDN_STATUS */
"ISDN is currently %s.\n\n" \
"   Protocol: %s\n" \
"   Card: %s\n" \
"   Local phone number: %s\n\n" \
"Select the item you wish to reconfigure, or choose to use the current settings.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"ISDN has not yet been configured. Select the item you wish to configure.",
/* TR_ENABLE_ISDN */
"Enable ISDN",
/* TR_DISABLE_ISDN */
"Disable ISDN",
/* TR_INITIALISING_ISDN */
"Initialising ISDN...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Unable to initialise ISDN.",
/* TR_ISDN_NOT_SETUP */
"ISDN not setup. Some items have not been selected.",
/* TR_ISDN_PROTOCOL_SELECTION */
"ISDN protocol selection",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Choose the ISDN protocol you require.",
/* TR_AUTODETECT */
"* AUTODETECT *",
/* TR_ISDN_CARD_SELECTION */
"ISDN card selection",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Choose the ISDN card installed in this computer.",
/* TR_CHECKING_FOR */
"Checking for: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"ISDN card not detected. You may need to specify additional module " \
"parameters if the card is an ISA type or it has special requirements.",
/* TR_DETECTED */
"Detected a: %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"Unable to find an ISDN card in this computer. You may need to specify " \
"additional module parameters if the card is an ISA type or it has special " \
"requirements.",
/* TR_ENTER_THE_LOCAL_MSN */
"Enter the local phone number (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"Phone number cannot be empty.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Some ISDN cards (especially ISA ones) may require additional module " \
"parameters for setting IRQ and IO address information. If you have such " \
"a ISDN card, enter these extra parameters here. For example: " \
"\"io=0x280 irq=9\". They will be used during card detection.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Pushing network down...",
/* TR_PULLING_NETWORK_UP */
"Pulling network up...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Network configuration type",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Drivers and card assignments",
/* TR_ADDRESS_SETTINGS */
"Address settings",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"DNS and Gateway settings",
/* TR_RESTART_REQUIRED */
"\n\nWhen configuration is complete, a network restart will be requrired.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Current config: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Network configuration menu",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Select the network configuration for SmoothWall Express.  The following " \
"configuration types list those interfaces which have ethernet attached. " \
"If you change this setting, a network restart will be required, and you " \
"will have to reconfigure the network driver assignments.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Pushing non local network down...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"Your configuration is set for a single GREEN interface, " \
"which already has a driver assigned.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Configure network drivers, and which interface each card " \
"is assigned to.  The current configuration is as follows:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nDo you wish to change these settings?",
/* TR_UNCLAIMED_DRIVER */
"There is an unclaimed ethernet card of type:\n%s\n\n" \
"You can assign this to:",
/* TR_CARD_ASSIGNMENT */
"Card assignment",
/* TR_PROBE */
"Probe",
/* TR_SELECT */
"Select",
/* TR_NO_UNALLOCATED_CARDS */
"No unallocated cards remaining, " \
"more are required. You may autodetect and look for more cards, or " \
"choose to select a driver from the list.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Unable to find any additional drivers.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"All cards successfully allocated.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Not enough cards were allocated.",
/* TR_MANUAL */
"* MANUAL *",
/* TR_SELECT_NETWORK_DRIVER */
"Select network driver",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Select the network driver for the card installed in this machine. " \
"If you select MANUAL, you will be given an opportunity to enter " \
"the driver module name and parameters for drivers which have " \
"special requirements, such as ISA cards.",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Unable to load driver module.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"This driver module is already loaded.",
/* TR_MODULE_PARAMETERS */
"Enter the module name and parameters for the driver you require.",
/* TR_LOADING_MODULE */
"Loading module...",
/* TR_WARNING */
"WARNING",
/* TR_WARNING_LONG */
"If you change this IP address, and you are logged in remotely, " \
"your connection to the SmoothWall Express machine will be broken, and you will " \
"have to reconnect on the new IP. This is a risky operation, and should " \
"only be attempted if you have physical access to the machine, should " \
"something go wrong.",
/* TR_SINGLE_GREEN */
"Your configuration is set for a single GREEN interface.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Select the interface you wish to reconfigure.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"Your configuration does not utilise an ethernet adaptor for " \
"its RED interface.  DNS and Gateway information for dialup users " \
"is configured automatically at dialup time.",
/* TR_PRIMARY_DNS */
"Primary DNS:",
/* TR_SECONDARY_DNS */
"Secondary DNS:",
/* TR_DEFAULT_GATEWAY */
"Default Gateway:",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Enter the DNS and gateway information.  These settings are used only if DHCP " \
"is disabled on the RED interface.",
/* TR_PRIMARY_DNS_CR */
"Primary DNS\n",
/* TR_SECONDARY_DNS_CR */
"Secondary DNS\n",
/* TR_DEFAULT_GATEWAY_CR */
"Default Gateway\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"Secondary DNS specified without a Primary DNS",
/* TR_UNKNOWN */
"UNKNOWN",
/* TR_NO_GREEN_INTERFACE */
"No GREEN interface assigned.",
/* TR_MISSING_GREEN_IP */
"Missing IP information on the GREEN interface.",
/* TR_NO_ORANGE_INTERFACE */
"No ORANGE interface assigned.",
/* TR_MISSING_ORANGE_IP */
"Missing IP information on the ORANGE interface.",
/* TR_NO_RED_INTERFACE */
"No RED interface assigned.",
/* TR_MISSING_RED_IP */
"Missing IP information on the RED interface.",
/* TR_NO_PURPLE_INTERFACE */
"No PURPLE interface assigned.",
/* TR_MISSING_PURPLE_IP */
"Missing IP information on the PURPLE interface.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Start address:",
/* TR_END_ADDRESS */
"End address:",
/* TR_DEFAULT_LEASE */
"Default lease (mins):",
/* TR_MAX_LEASE */
"Max lease (mins):",
/* TR_DOMAIN_NAME_SUFFIX */
"Domain name suffix:",
/* TR_CONFIGURE_DHCP */
"Configure the DHCP server by entering the settings information.",
/* TR_START_ADDRESS_CR */
"Start address\n",
/* TR_END_ADDRESS_CR */
"End address\n",
/* TR_DEFAULT_LEASE_CR */
"Default lease time\n",
/* TR_MAX_LEASE_CR */
"Max lease time\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Domain name suffix\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Keyboard mapping",
/* TR_KEYBOARD_MAPPING_LONG */
"Choose the type of keyboard you are using from the list below.",

/* webproxy.c */
/* TR_HOSTNAMEC */
"Hostname:",
/* TR_PORTC */
"Port:",
/* TR_WEB_PROXY_LONG */
"When requesting the Update list, " \
"SmoothWall Express must connect to a webserver. Some ISPs will block " \
"this direct traffic, and instead they will require SmoothWall Express to access " \
"this server indirectly, via a web proxy. If your ISP requires you to " \
"connect this way, please enter the web proxy Hostname and Port. Most ISPs " \
"do not require you to connect through a web proxy, in which case you should " \
"leave these settings blank. Note that this setting has nothing to do with " \
"SmoothWall Express\' built in proxy service.",
/* TR_PORT_IS_INVALID */
"Port is invalid.",
/* TR_PORT_NOT_GIVEN */
"Port not given.",

/* usbadsl.c */
/* TR_ADSL_STATUS */
"ADSL is currently: %s\n" \
"   Controller: %s\n\n" \
"Select the item you wish to reconfigure, or choose to use the current settings.",
/* TR_ADSL_CONFIGURATION */
"ADSL configuration",
/* TR_ENABLE_ADSL */
"Enable ADSL",
/* TR_DISABLE_ADSL */
"Disable ADSL",
/* TR_INITIALISING_ADSL */
"Initialising ADSL.",
/* TR_UNABLE_TO_INITIALISE_ADSL */
"Unable to initialise ADSL",
/* TR_ADSL_NOT_SETUP */
"ADSL not setup.",
/* TR_STARTING_UP_ADSL */
"Starting up ADSL...",

/* restore.c */
/* TR_PLEASE_INSERT_THE_RESTORE_FLOPPY_DISK */
"Please insert the restore floppy disk.",
/* TR_EXAMINING_BACKUP_DISK */
"Examining backup disk...",
/* TR_UNABLE_TO_EXAMINE_BACKUP_DISK */
"Unable to examine backup disk.",
/* TR_DO_YOU_WISH_TO_RESTORE */
"Do you wish to restore the configuration from the backup disk? If you " \
"continue, the current configuration (if any) will be erased and replaced " \
"with what is on the backup floppy disk.",
/* TR_RESTORING */
"Restoring...",
/* TR_UNABLE_TO_RESTORE */
"Unable to restore.",
/* TR_CONFIGURATION_SUCCESSFULLY_RESTORED */
"Configuration successfully restored.  Please remove the disk from the " \
"floppy disk drive before continuing.",
/* TR_CLEANING_UP */
"Cleaning up...",

/* defaults.c */
/* TR_OPENNESS_OPEN */
"Open",
/* TR_OPENNESS_HALFOPEN */
"Half-open",
/* TR_OPENNESS_CLOSED */
"Closed",
/* TR_DEFAULT_OPENNESS */
"Default security policy",
/* TR_DEFAULT_OPENNESS_LONG */
"Select the default security policy for outgoing requests. This setting does NOT " \
"affect incoming requests in any way, which are always blocked unless explicitily allowed.\n" \
"\n" \
"   'Open' : all outgoing requests are allowed (this is the same as previous versions of SmoothWall Express).\n" \
"   'Half-open' : most outoing requests are allowed, except for potentionally harmful ones.\n" \
"   'Closed' : all outgoing requests are blocked, and must be explicitly enabled on the " \
"ports and services you require.\n" \

};
