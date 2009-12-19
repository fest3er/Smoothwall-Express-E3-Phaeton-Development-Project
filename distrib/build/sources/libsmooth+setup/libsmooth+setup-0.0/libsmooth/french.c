/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * (c) French Translation Team:
 * David Boileau
 * Thierry Charbonnel
 * Frederic Gonnet   
 * Francois Thomas
 * Fabien Illide
 * Christopher Heger 
 * Manu El Poulpe     
 * Frederic Legrain   
 * Roger De Riemaecker
 * Eric Vaneberck
 * Ismail Simoes
 * Denis Renard
 * Pierre
 * Andre Joanisse 
 * Xavier de Gelis
 * Nicolas Micheli
 * Hughes Devaux
 * John S. Gage
 * Jeff   
 * nicolas
 * Ikos
 * Cédric Thévenet
 * Veronique Marie Hammonds
 *
 * filename: french.c
 * Contains french strings. */

#include "libsmooth.h"

char *french_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Annuler",
/* TR_INSTALLATION_CANCELED */
"L'installation a été annulée.",
/* TR_HELPLINE */
"              <Tab>/<Alt-Tab> entre champs   |  <Space> sélectionner",
/* TR_QUIT */
"Quitter",
/* TR_DISABLED */
"Désactiver",
/* TR_ENABLED */
"Activer",
/* TR_UNSET */
"INACTIF",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Le fichier de configuration ne peut pas être ouVERTEEEE",
/* TR_DONE */
"Terminer",
/* TR_PROBE_FAILED */
"La détection automatique a échoué.",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"Adresse IP:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Adresse réseau:",
/* TR_NETMASK_PROMPT */
"Masque de réseau:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Inscrivez l'information de l'adresse IP",
/* TR_INVALID_FIELDS */
"Les champs qui suivent ne sont pas valables:\n\n",
/* TR_IP_ADDRESS_CR */
"Adresse IP\n",
/* TR_NETWORK_ADDRESS_CR */
"Adresse réseau\n",
/* TR_NETWORK_MASK_CR */
"Masque de réseau\n",
/* TR_INTERFACE (%s is interface name) */
"%s interface",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Inscrivez l'information de l'adresse IP pour l'interface %s.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Recherche: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall a détecté l'interface réseau dans votre machine: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"Le nom du module ne peut pas être vide.",
/* TR_STATIC */
"Statique",
/* TR_DHCP_HOSTNAME */
"Nom d'hôte DHCP:",
/* TR_DHCP_HOSTNAME_CR */
"Nom d'hôte DHCP\n",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Il n'est pas possible d'écrire /var/smoothwall/main/hostname.conf",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Il n'est pas possible d'écrire /etc/hosts.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"Il n'est pas possible d'écrire /etc/hosts.deny.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Il n'est pas possible d'écrire /etc/hosts.allow.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Il n'est pas possible de créer le nom d'hôte.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Bienvenue au programme d'installation de SmoothWall.  Venez voir " \
"notre page d'accueil à http://www.smoothwall.org.  Si vous annulez " \
"l'un des écrans qui suivent l'ordinateur redémarrera.",
/* TR_NO_IDE_HARDDISK */
"Aucun disque dur IDE.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Choisir une source d'installation",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"L'installation smoothwall peut se faire de plusieurs sources.  La méthode " \
"la plus simple est d'utiliser le lecteur CDROM de l'ordinateur.  Si " \
"l'ordinateur ne possède pas de lecteur CDROM, une installation est " \
"encore possible si une autre machine, qui est connectée au réseau, " \
"offre les fichiers par HTTP.  Dans ce cas la disquette avec les " \
"pilotes (drivers) des cartes réseaux est requise.",
/* TR_NO_IDE_CDROM */
"Aucun CDROM IDE.",
/* TR_INSERT_CDROM */
"Placer le CD de SmoothWall dans le lecteur CDROM.",
/* TR_INSERT_FLOPPY */
"Insérer la disquette des pilotes (drivers) de SmoothWall dans le lecteur.",
/* TR_PREPARE_HARDDISK */
"Le programme d'installation va maintenant préparer le disque dur IDE " \
"de %s. Le disque sera partitionné, et un système de gestion " \
"de fichiers créé sur chaque partition.",
/* TR_PARTITIONING_DISK */
"Le partitionnement du disque est commencé...",
/* TR_UNABLE_TO_PARTITION */
"Il n'est pas possible de partitionner le disque dur.",
/* TR_MAKING_SWAPSPACE */
"Création du fichier d'échange...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Il n'est pas possible de créer le fichier d'échange.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Création du répertoire racine...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Il n'est pas possible de créer le répertoire racine.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Montage du répertoire racine...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Il n'est pas possible de monter le répertoire racine.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Création du répertoire boot...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Il n'est pas possible de créer le répertoire boot.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Montage du répertoire boot...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Il n'est pas possible de monter le répertoire boot.",
/* TR_MAKING_LOG_FILESYSTEM */
"Création du répertoire log...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Il n'est pas possible de créer le répertoire log.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Montage du répertoire log...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Il n'est pas possible de monter le répertoire log.",
/* TR_MOUNTING_SWAP_PARTITION */
"Montage du fichier d'échange...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Il n'est pas possible de monter le fichier d'échange...",
/* TR_NETWORK_SETUP_FAILED */
"Le paramétrage du réseau a échoué.",
/* TR_NO_TARBALL_DOWNLOADED */
"Aucune archive tar à télécharger.",
/* TR_INSTALLING_FILES */
"Installation des fichiers...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Il n'est pas possible d'installer les fichiers.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Il n'est pas possible d'enlever les fichiers temporaires qui ont été téléchargés.",
/* TR_ERROR_WRITING_CONFIG */
"Erreur en écrivant les données de configuration.",
/* TR_INSTALLING_LILO */
"Installation de LILO...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Il n'est pas possible d'installer LILO.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Il n'est pas possible de démonter le disque dur.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Il n'est pas possible de démonter le lecteur CDROM/disquette.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Il n'est pas possible d'expulser le CDROM.",
/* TR_CONGRATULATIONS */
"Félicitations!",
/* TR_CONGRATULATIONS_LONG */
"L'installation de SmoothWall a réussit. Retirez le CDROM et la disquette " \
"de l'ordinateur.  La configuration du système va maintenant commencer. " \
"Vous aurez la possibilité de faire la configuration de vos périphériques RNIS et réseaux " \
"ainsi que de choisir les mots de passe du système. Une fois que la " \
"configuration sera terminée, connectez-vous à l'adresse suivante: " \
"http://(adresse IP Smoothwall ou nom):81 ou https://(adresse IP Smoothwall ou nom):445 " \
"et il est suggéré que vous configuriez la connexion à internet" \
"(si vous en avez besoin) et l'accès à distance.  N'oubliez pas " \
"de spécifier un mot de passe pour l'utilisateur 'dial' si vous voulez " \
"que les utilisateurs de SmoothWall non 'admin' puissent avoir contrôle de la ligne.",
/* TR_PRESS_OK_TO_REBOOT */
"Pressez OK pour redémarrer l'ordinateur.",
/* TR_ERROR */
"Erreur",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"Calcul des dépendances de module...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Il n'est pas possible de calculer les dépendances de module.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Sélectionnez un type de CDROM",
/* TR_SELECT_CDROM_TYPE_LONG */
"Un lecteur de CDROM IDE n'a été pas trouvé dans l'ordinateur. " \
"Sélectionnez les pilotes suivants que SmoothWall utilisera pour accéder " \
"au lecteur CDROM.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(suggestion %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(suggestion %d)",
/* TR_CONFIGURE_THE_CDROM */
"Configurez le lecteur  CDROM en choisissant une adresse IO et/ou un IRQ",
/* TR_INVALID_IO (note extra space) */
"Les paramètres pour l'adresse IO ne sont pas valables. ",
/* TR_INVALID_IRQ */
"Les paramètres pour l'IRQ ne sont pas valables.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Il n'est pas possible d'écrire /var/smoothwall/main/settings.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Il n'est pas possible d'écrire /var/smoothwall/ethernet/settings.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Il n'est pas possible de créer le lien symbolique /dev/harddisk.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Il n'est pas possible de créer le lien symbolique /dev/harddisk1.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Il n'est pas possible de créer le lien symbolique /dev/harddisk2.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Il n'est pas possible de créer le lien symbolique /dev/harddisk3.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Il n'est pas possible de créer le lien symbolique /dev/harddisk4.",

/* net.c */
/* TR_DOWNLOADING */
"En cours de téléchargement...",
/* TR_FAILED_TO_DOWNLOAD */
"Le téléchargement a échoué.",
/* TR_ENTER_URL */
"Inscrivez l'URL du fichier tar.gz de SmoothWall. " \
"AVERTEEEEISSEMENT: Le DNS n'est pas disponible!  Ceci devrait terminer avec " \
"le fichier 'smoothwall.tgz'.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Configuration du réseau",
/* TR_CONFIGURE_NETWORKING_LONG */
"Vous pouvez maintenant configurer les périphériques du réseau en chargeant " \
"le bon pilote pour l'interface VERTE. Vous avez le choix d'utiliser la " \
"méthode automatique pour trouver votre carte réseau, ou de choisir " \
"le bon pilote de la liste.  Notez que si vous avez plus d'une carte réseau " \
"installée, vous aurez la chance de configurer les autres plus tard. " \
"C'est aussi très important de noter que si vous avez plus d'une carte " \
"réseau du même type que la carte VERTE, et que chaque carte a besoin de " \
"paramètres spéciaux pour le module, vous devrez inscrire les paramètres " \
"pour toutes les cartes de ce type pour qu'elles deviennent toutes actives " \
"quand vous configurerez l'interface VERTE.",
/* TR_INTERFACE_FAILED_TO_COME_UP */
"Le montage de l'interface a échoué.",
/* TR_ENTER_NETWORK_DRIVER */
"La détection automatique de la carte réseau a échoué.  Inscrivez le pilote " \
"et les paramètres optionnels pour la carte réseau.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Nom d'hôte",
/* TR_NETWORKING */
"Réseau",
/* TR_DHCP_SERVER_CONFIGURATION */
"Configuration du serveur DHCP",
/* TR_ISDN_CONFIGURATION */
"Configuration RNIS",
/* TR_ROOT_PASSWORD */
"Mot de passe \'root\'",
/* TR_SETUP_PASSWORD */
"Mot de passe \'setup\'",
/* TR_ADMIN_PASSWORD */
"Mot de passe \'admin\'",
/* TR_SECTION_MENU */
"Section menu",
/* TR_SELECT_THE_ITEM */
"Choisissez l'élément que vous voulez configurez.",
/* TR_SETUP_FINISHED */
"Le paramétrage est complet.  Pressez OK pour redémarrer l'ordinateur.",
/* TR_SETUP_NOT_COMPLETE */
"L'initialisation n'est pas entièrement complète. Prière de vous assurer que "\
"la configuration soit complète en redémarrant la configuration depuis le "\
"shell.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Inscrivez le mot de passe pour l'utilisateur 'root'. Seulement " \
"l'utilisateur 'root' peut accéder à la ligne de commande.",
/* TR_SETTING_ROOT_PASSWORD */
"Sauvegarde du mot de passe pour 'root'....",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Il n'est pas possible d'écrire le mot de passe pour 'root'.",
/* TR_ENTER_SETUP_PASSWORD */
"Inscrivez le mot de passe pour l'utilisateur 'setup'. Seulement " \
"l'utilisateur 'setup' peut accéder au programme de configuration.",
/* TR_SETTING_SETUP_PASSWORD */
"Sauvegarde du mot de passe pour 'setup'....",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Il n'est pas possible d'écrire le mot de passe pour 'setup'.",
/* TR_ENTER_ADMIN_PASSWORD */
"Inscrivez le mot de passe pour l'administrateur de SmoothWall. " \
"Seulement l'utilisateur 'admin' peut accéder aux pages webs d'administration.",
/* TR_SETTING_ADMIN_PASSWORD */
"Sauvegarde du mot de passe pour administrateur....",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Il n'est pas possible d'écrire le mot de passe pour l'administrateur.",
/* TR_PASSWORD_PROMPT */
"Mot de passe:",
/* TR_AGAIN_PROMPT */
"Encore:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"Le mot de passe ne peut pas être vide.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"Les mots de passe ne sont pas identiques.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Inscrivez le nom d'hôte de la machine.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"Le nom d'hôte ne peut pas être vide.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"Le nom d'hôte ne peut pas contenir d'espace.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Il n'est pas possible d'écrire /etc/hostname",

/* isdn.c */
/* TR_GERMAN_1TR6 */
"Allemand 1TR6",
/* TR_EURO_EDSS1 */
"Euro (EDSS1)",
/* TR_LEASED_LINE */
"Ligne louée",
/* TR_US_NI1 */
"É-U NI1",
/* TR_PROTOCOL_COUNTRY */
"Protocole/Pays",
/* TR_ISDN_CARD */
"Carte RNIS",
/* TR_MSN_CONFIGURATION */
"Numéro d'accès local (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Parammètres additionnels du module",
/* TR_RED_IN_USE */
"Le RNIS (ou une autre connexion externe) est occupé.  Vous ne pouvez pas " \
"configurer RNIS pendant que l'interface ROUGE est active.",
/* TR_ISDN_CONFIGURATION_MENU */
"Menu de configuration RNIS",
/* TR_ISDN_STATUS */
"Le RNIS actuel %s.\n\n" \
"   Protocole: %s\n" \
"   Carte: %s\n" \
"   Numéro d'accès locale: %s\n\n" \
"Sélectionnez l'élément que vous voulez changer, ou utilisez cette configuration.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"Le RNIS n'est pas configuré. Sélectionnez l'élément que vous voulez configurer.",
/* TR_ENABLE_ISDN */
"Activer RNIS",
/* TR_DISABLE_ISDN */
"Désactiver RNIS",
/* TR_INITIALISING_ISDN */
"Initialisation du RNIS...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Il n'est pas possible d'initialiser RNIS.",
/* TR_ISDN_NOT_SETUP */
"Le RNIS n'est pas configuré.  Certains éléments n'ont pas été sélectionnés.",
/* TR_ISDN_PROTOCOL_SELECTION */
"Protocole RNIS",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Sélectionnez le protocole requis pour RNIS.",
/* TR_AUTODETECT */
"* DETECTION AUTOMATIQUE *",
/* TR_ISDN_CARD_SELECTION */
"Carte RNIS",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Sélectionnez la carte RNIS installée dans cet ordinateur.",
/* TR_CHECKING_FOR */
"Cherche: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"La détection automatique de la carte RNIS a échouée. Il est possible " \
"que vous deviez spécifier des paramètres additionnels pour le module " \
"si la carte est de type ISA ou doit être traitée d'une manière spéciale.",
/* TR_DETECTED */
"Détecter: %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"Une carte RNIS n'a pas été trouvée dans cet ordinateur.  Il est possible " \
"que vous devez spécifier des paramètres additionnels pour le module " \
"si la carte est de type ISA ou doit être traitée d'une manière spéciale.",
/* TR_ENTER_THE_LOCAL_MSN */
"Inscrivez le numéro d'accès local (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"Le numéro d'accès local ne peut pas être vide.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Certaines cartes RNIS (particulièrement les cartes de type ISA) requièrent " \
"des paramètres additionnels pour indiquer au module le bon IRQ ou l'adresse " \
"IO. Si vous possédez une de ces carte, inscrivez les paramètres additionnels"\
"ici.  Par exemple: \"io=0x280 irq=9\".  Les paramètres vont êtres utilisés " \
"pendant que la carte est détectée.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Le réseau s'arrête",
/* TR_PULLING_NETWORK_UP */
"Le réseau redémarre...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Type de configuration du réseau",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Spécification de la carte et du pilote",
/* TR_ADDRESS_SETTINGS */
"Spécification des adresses",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"Spécification du DNS et des passerelles",
/* TR_RESTART_REQUIRED */
"\n\nUne fois la configuration terminée, le réseau devra être relancé.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Configuration actuelle: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Menu de configuration du réseau",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Sélectionnez le configuration du réseau pour SmoothWall.  Les configurations " \
"qui suivent indiquent les interfaces qui sont reliées à l'ethernet. " \
"Si vous changez cet élément le réseau devra être relancé, et " \
"vous serez forcé de reconfigurer les pilotes du réseau.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Le réseau non local s'arrête...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"Votre configuration est faite pour un seul interface VERTE, avec un pilote " \
"déjà spécifié.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Configurez les pilotes du réseau, et indiquer quelles interfaces sont " \
"attribuées à quels pilotes. Voici la configuration actuelle:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nVoulez-vous changer les paramètres?",
/* TR_UNCLAIMED_DRIVER */
"Il y a une carte ethernet non attribuée de type:\n%s\n\n" \
"Vous pouvez l'attribuer à:",
/* TR_CARD_ASSIGNMENT */
"Attribution des cartes",
/* TR_PROBE */
"Sonde",
/* TR_SELECT */
"Sélectionnez",
/* TR_NO_UNALLOCATED_CARDS */
"L'attribution des cartes ne peut pas être faite, car il ne reste plus " \
"de cartes. Vous pouvez utiliser la méthode de détection automatique, ou " \
"sélectionner un pilote de la liste.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Aucun pilote additionnel n'a été trouvé.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Toutes les cartes ont été attribuées avec succès.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Il n'y avait pas assez de cartes pour finir l'attribution.",
/* TR_MANUAL */
"* MANUEL *",
/* TR_SELECT_NETWORK_DRIVER */
"Sélectionnez un pilote de réseau",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Sélectionnez le pilote pour la carte de réseau installés dans cette machine. " \
"Si vous choisi MANUEL, vous aurez la possibilité d'inscrire le nom du module " \
"et les paramètres pour les cartes spéciales comme celles de type ISA.",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Il n'est pas possible de charger le pilote.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"Le pilote est déjà chargé.",
/* TR_MODULE_PARAMETERS */
"Inscrivez le nom du module et les paramètres qui sont requis pour le pilote." ,
/* TR_LOADING_MODULE */
"Le module se charge...",
/* TR_WARNING */
"AVERTEEEEISSEMENT",
/* TR_WARNING_LONG */
"Si vous changez l'adresse IP, et que vous êtes logué par accès à distance, " \
"la connexion à la machine SmoothWall sera interrompue et vous aurez besoin " \
"de vous reconnecter avec la nouvelle adresse IP.  Ceci est une opération qui " \
"est très risquée et qui devrait seulement être essayée si vous avez " \
"directement accès a la machine pour régler les problèmes éventuels.",
/* TR_SINGLE_GREEN */
"Votre configuration est préparée pour un interface VERTE simple.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Sélectionnez l'interface que vous désirez reconfigurer.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"Votre configuration n'utilise pas un périphérique ethernet pour " \
"l'interface ROUGE. Les paramètres pour le DNS et la passerelle sont " \
"configurés automatiquement pour les utilisateurs avec accès réseau quand " \
"la connexion est établie.",
/* TR_PRIMARY_DNS */
"DNS primaire:",
/* TR_SECONDARY_DNS */
"DNS secondaire:",
/* TR_DEFAULT_GATEWAY */
"Passerelle:",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Inscrivez l'information du DNS et de la passerelle. Ces paramètres seront " \
"seulement utilisés si le DHCP est désactivé à partir de l'interface ROUGE.",
/* TR_PRIMARY_DNS_CR */
"DNS primaire\n",
/* TR_SECONDARY_DNS_CR */
"DNS secondaire\n",
/* TR_DEFAULT_GATEWAY_CR */
"Passerelle\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"Le DNS secondaire ne peut pas être spécifié sans un DNS primaire",
/* TR_UNKNOWN */
"INCONNU",
/* TR_NO_ORANGE_INTERFACE */
"Aucune interface ORANGE n'a été allouée.",
/* TR_MISSING_ORANGE_IP */
"L'information IP pour l'interface ORANGE n'est pas complète.",
/* TR_NO_RED_INTERFACE */
"Aucune interface ROUGE n'a été allouée.",
/* TR_MISSING_RED_IP */
"L'information IP pour l'interface ROUGE n'est pas complète.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Adresse de départ:",
/* TR_END_ADDRESS */
"Adresse de fin:",
/* TR_DEFAULT_LEASE */
"Temps de prêt par défaut (mins):",
/* TR_MAX_LEASE */
"Temps de prêt maximum (mins):",
/* TR_DOMAIN_NAME_SUFFIX */
"Suffixe de nom de domaine:",
/* TR_CONFIGURE_DHCP */
"Configurez le serveur DHCP en entrant l'information de configuration.",
/* TR_START_ADDRESS_CR */
"Adresse de départ\n",
/* TR_END_ADDRESS_CR */
"Adresse de fin\n",
/* TR_DEFAULT_LEASE_CR */
"Temps de prêt par défaut\n",
/* TR_MAX_LEASE_CR */
"Temps de prêt maximum\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Suffixe de nom de domaine\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Type de clavier",
/* TR_KEYBOARD_MAPPING_LONG */
"Choisissez le type de clavier que vous utilisez dans la liste ci-dessous.",

/* timezone.c */
/* TR_TIMEZONE */
"Fuseau horaire",
/* TR_TIMEZONE_LONG */
"Choisissez le fuseau horaire dans lequel voue êtes dans la liste ci-dessous.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Selectionnez le contrôleur USB",
/* TR_USBADSL_STATUS */
"L'ADSL USB est actuellement: %s\n" \
"   Contrôleur: %s\n\n" \
"Selectionez ce que vous voulez reconfigurer, ou choisissez d'utiliser les "\
"paramètres courants.",
/* TR_USBADSL_CONFIGURATION */
"Configuration de l'USB ADSL ",
/* TR_ENABLE_USBADSL */
"Activer l'USB ADSL",
/* TR_DISABLE_USBADSL */
"Désactiver l'USB ADSL",
/* TR_INITIALISING_USBADSL */
"Initialisation de l'USB ADSL.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"Impossible d'initialiser l'USB ADSL",
/* TR_USBADSL_NOT_SETUP */
"USB ADSL non configuré.",
/* TR_USB_CONTROLLER_SELECTION */
"Sélection du contrôleur USB",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Choisissez le contrôleur USB installé sur la machine SmoothWall .",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"Contrôleur USB non détecté.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"Impossible de trouver un contrôleur USB.",
/* TR_STARTING_UP_USBADSL */
"Démarrage de l'USB ADSL..."

};
