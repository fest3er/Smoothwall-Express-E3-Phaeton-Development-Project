/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * Brazillian Portuguese Translation Team:
 * Gilberto Gaudencio
 * Uelinton B. dos Santos
 *
 * filename: brazillain.c
 * Contains brazillian strings. */

#include "libsmooth.h"

char *brazilian_tr[] = {

/**********/
/* COMMON */
/**********/

/* TR_OK */
"Ok",
/* TR_CANCEL */
"Cancelar",
/* TR_INSTALLATION_CANCELED */
"Instalação cancelada.",
/* TR_HELPLINE */
"              <Tab>/<Alt-Tab> movimentar   |  <Espaço> selecionar",
/* TR_QUIT */
"Sair",
/* TR_DISABLED */
"Desativado",
/* TR_ENABLED */
"Ativado",
/* TR_UNSET */
"Não especificado",
/* TR_UNABLE_TO_OPEN_SETTINGS_FILE */
"Não foi possível abrir arquivo de configuração",
/* TR_DONE */
"Feito",
/* TR_PROBE_FAILED */
"A auto-detecção falhou.",

/*************/
/* LIBSMOOTH */
/*************/

/* main.c  */

/* netstuff.c */
/* TR_IP_ADDRESS_PROMPT */
"Endereço IP:",
/* TR_NETWORK_ADDRESS_PROMPT */
"Endereço de rede:",
/* TR_NETMASK_PROMPT */
"Máscara de rede:",
/* TR_ENTER_IP_ADDRESS_INFO */
"Introduza informações sobre o endereço IP",
/* TR_INVALID_FIELDS */
"Os seguintes campos não são válidos:\n\n",
/* TR_IP_ADDRESS_CR */
"Endereço IP\n",
/* TR_NETWORK_ADDRESS_CR */
"Endereço de rede\n",
/* TR_NETWORK_MASK_CR */
"Máscara de rede\n",
/* TR_INTERFACE (%s is interface name) */
"Interface %s",
/* TR_ENTER_THE_IP_ADDRESS_INFORMATION (%s is interface name) */
"Introduza informações sobre o endereço IP da interface %s.",
/* TR_LOOKING_FOR_NIC (%s is a nic name) */
"Procurando: %s",
/* TR_FOUND_NIC (%s is a nic name) */
"SmoothWall detectou o seguinte NIC nesta máquina: %s",
/* TR_MODULE_NAME_CANNOT_BE_BLANK */
"O nome do módulo não pode ser nulo.",
/* TR_STATIC */
"Estático",
/* TR_DHCP_HOSTNAME */
"Nome do servidor DHCP:",
/* TR_DHCP_HOSTNAME_CR */
"Nome do servidor DHCP\n",

/* misc.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF */
"Impossível salvar /var/smoothwall/main/hostname.conf",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS */
"Impossível salvar /etc/hosts.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_DENY */
"Impossível salvar /etc/hosts.deny.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTS_ALLOW */
"Impossível salvar /etc/hosts.allow.",
/* TR_UNABLE_TO_SET_HOSTNAME */
"Impossível salvar nome do servidor.",

/***********/
/* INSTALL */
/***********/
/* TR_WELCOME */
"Seja bem vindo ao programa de instalação SmootWall. Por favor " \
"visite a nossa página em http://www.smoothwall.org. Selecionar " \
"Cancelar em qualquer das telas seguintes reiniciará o computador.",

/* TR_NO_IDE_HARDDISK */
"Nenhum disco rígido IDE encontrado.",
/* TR_SELECT_INSTALLATION_MEDIA */
"Selecione o modo de instalação",
/* TR_SELECT_INSTALLATION_MEDIA_LONG */
"SmoothWall pode ser instalado de diversas formas. A mais simples é usar " \
"o leitor de CDROM do computador. Caso não seja possível, pode-se também "\
"instalar através de outro computador em rede que tenha os arquivos de "\
"instalação disponíveis via HTTP. Neste caso será necessário o disquete "\
"com os controladores de rede.",
/* TR_NO_IDE_CDROM */
"Nenhum CDROM IDE encontrado.",
/* TR_INSERT_CDROM */
"Por favor insira o CD SmoothWall.",
/* TR_INSERT_FLOPPY */
"Por favor insira o disquete de controladores SmoothWall.",
/* TR_PREPARE_HARDDISK */
"O programa de instalação irá agora preparar o disco rígido IDE %s. " \
"Primeiro serão criadas partições no disco, e depois cada partição será " \
"formatada.",
/* TR_PARTITIONING_DISK */
"Criando partições no disco...",
/* TR_UNABLE_TO_PARTITION */
"Impossível criar partições no disco.",
/* TR_MAKING_SWAPSPACE */
"Criando espaço de swap...",
/* TR_UNABLE_TO_MAKE_SWAPSPACE */
"Impossível criar espaço de swap.",
/* TR_MAKING_ROOT_FILESYSTEM */
"Criando sistema de arquivos root...",
/* TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM */
"Impossível criar sistema de arquivos root.",
/* TR_MOUNTING_ROOT_FILESYSTEM */
"Montando sistema de arquivos root...",
/* TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM */
"Impossível montar sistema de arquivos root.",
/* TR_MAKING_BOOT_FILESYSTEM */
"Criando sistema de arquivos boot...",
/* TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM */
"Impossível criar sistema de arquivos boot.",
/* TR_MOUNTING_BOOT_FILESYSTEM */
"Montando boot...",
/* TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM */
"Impossível montar sistema de arquivos boot.",
/* TR_MAKING_LOG_FILESYSTEM */
"Criando sistema de arquivos log...",
/* TR_UNABLE_TO_MAKE_LOG_FILESYSTEM */
"Impossível criar sistema de arquivos log.",
/* TR_MOUNTING_LOG_FILESYSTEM */
"Montando sistema de arquivos log...",
/* TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM */
"Impossível montar sistema de arquivos log.",
/* TR_MOUNTING_SWAP_PARTITION */
"Montando sistema de arquivos swap...",
/* TR_UNABLE_TO_MOUNT_SWAP_PARTITION */
"Impossível montar sistema de arquivos swap.",
/* TR_NETWORK_SETUP_FAILED */
"A configuração da rede falhou.",
/* TR_NO_TARBALL_DOWNLOADED */
"Nenhum arquivo tarball transferido.",
/* TR_INSTALLING_FILES */
"Instalando arquivos...",
/* TR_UNABLE_TO_INSTALL_FILES */
"Impossível instalar arquivos.",
/* TR_UNABLE_TO_REMOVE_TEMP_FILES */
"Impossível remover arquivos temporários.",
/* TR_ERROR_WRITING_CONFIG */
"Erro ao salvar arquivo de configuração.",
/* TR_INSTALLING_LILO */
"Instalando LILO...",
/* TR_UNABLE_TO_INSTALL_LILO */
"Impossível instalar LILO.",
/* TR_UNABLE_TO_UNMOUNT_HARDDISK */
"Impossível desmontar disco rígido.",
/* TR_UNABLE_TO_UNMOUNT_CDROM */
"Impossível desmontar CDROM/disquete.",
/* TR_UNABLE_TO_EJECT_CDROM */
"Impossível ejetar CDROM.",
/* TR_CONGRATULATIONS */
"Parabéns!",
/* TR_CONGRATULATIONS_LONG */
"SmoothWall foi instalado com sucesso. Por favor remova quaisquer disquetes " \
"ou CD-ROMs do computador. Em seguida será possível configurar a conexão ISDN, " \
"interfaces de rede, e senhas do sistema. Após ter terminado a " \
"configuração, deverá apontar o seu browser a http://smoothwall:81 ou " \
"https://smoothwall:445 (assumindo que smoothwall é o nome deste sistema) e " \
"configurar o modem (se necessário) e o acesso remoto. Lembre-se de ativar " \
"a senha para o usuário 'dial' caso queira permitir que outros " \
"usuários além do 'admin' controlem a conexão.",

/* TR_PRESS_OK_TO_REBOOT */
"Pressione Ok para reiniciar.",
/* TR_ERROR */
"Erro",
/* TR_CALCULATING_MODULE_DEPENDANCIES */
"Calculando dependências dos módulos...",
/* TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES */
"Impossível calcular dependências dos módulos.",

/* cdrom.c */
/* TR_SELECT_CDROM_TYPE */
"Selecione o tipo de CDROM",
/* TR_SELECT_CDROM_TYPE_LONG */
"Nenhum CDROM IDE foi detectado nesta máquina. Por favor qual dos seguintes " \
"controladores deseja utilizar para que SmoothWall tenha acesso ao CDROM.",
/* TR_SUGGEST_IO (%x is an IO number) */
"(sugere %x)",
/* TR_SUGGEST_IRQ (%d is an IRQ number) */
"(sugere %d)",
/* TR_CONFIGURE_THE_CDROM */
"Configure o CDROM escolhendo o endereço IO e/ou IRQ apropriados.",
/* TR_INVALID_IO (note extra space) */
"Os detalhes da porta IO introduzidos não são válidos. ",
/* TR_INVALID_IRQ */
"Os detalhes do IRQ introduzidos não são válidos.",

/* config.c */
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_SETTINGS */
"Impossível salvar /var/smoothwall/main/settings.",
/* TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_ETHERNET_SETTINGS */
"Impossível salvar /var/smoothwall/ethernet/settings.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK */
"Impossível criar ligação simbólica /dev/harddisk.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK1 */
"Impossível criar ligação simbólica /dev/harddisk1.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK2 */
"Impossível criar ligação simbólica /dev/harddisk2.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK3 */
"Impossível criar ligação simbólica /dev/harddisk3.",
/* TR_UNABLE_TO_MAKE_SYMLINK_DEV_HARDDISK4 */
"Impossível criar ligação simbólica /dev/harddisk4.",

/* net.c */
/* TR_DOWNLOADING */
"Transferindo...",
/* TR_FAILED_TO_DOWNLOAD */
"A transferência falhou.",
/* TR_ENTER_URL */
"Digite o URL completo para o arquivo tar.gz SmoothWall. " \
"AVISO: DNS indisponível! O URL deve terminar com o arquivo 'smoothwall.tgz'.",

/* nic.c */
/* TR_CONFIGURE_NETWORKING */
"Configuração da rede",
/* TR_CONFIGURE_NETWORKING_LONG */
"Pode agora configurar a rede, carregando primeiro o controlador correto para " \
"a interface VERDE. Pode fazê-lo usando a auto-detecção da interface de rede " \
"ou escolhendo o controlador adequado de uma lista. Note que se possui interfaces " \
"de rede adicionais, poderá configurá-las posteriormente durante a instalação. " \
"Note também que se possui interfaces adicionais do mesmo tipo da interface VERDE, " \
"e se cada uma necessitar de parâmetros especiais, deve especificá-los todos de " \
"de forma a que todas as interfaces se tornem ativas ao configurar a interface VERDE.",

/* TR_INTERFACE_FAILED_TO_COME_UP */
"A interface falhou ao inicializar.",
/* TR_ENTER_NETWORK_DRIVER */
"A detecção automática falhou. Especifique o controlador e " \
"os parâmetros opcionais para a interface de rede.",

/*********/
/* SETUP */
/*********/

/* main.c */
/* TR_HOSTNAME */
"Nome do servidor",
/* TR_NETWORKING */
"Rede",
/* TR_DHCP_SERVER_CONFIGURATION */
"Configuração do servidor DHCP",
/* TR_ISDN_CONFIGURATION */
"Configuração ISDN",
/* TR_ROOT_PASSWORD */
"Senha de \'root\'",
/* TR_SETUP_PASSWORD */
"Senha de \'setup\'",
/* TR_ADMIN_PASSWORD */
"Senha de admin",
/* TR_SECTION_MENU */
"Menú de seções",
/* TR_SELECT_THE_ITEM */
"Selecione o item que deseja configurar.",
/* TR_SETUP_FINISHED */
"Configuração completa.  Pressione Ok para reiniciar.",
/* TR_SETUP_NOT_COMPLETE */
"A configuração inicial não foi terminada com sucesso. Assegure-se que esta " \
"tenha sido completada devidamente antes de tentar executar o programa setup " \
"na linha de comando.",

/* passwords.c */
/* TR_ENTER_ROOT_PASSWORD */
"Digite a senha para o usuário 'root'. Este é o usuário que " \
"tem acesso à linha de comando.",
/* TR_SETTING_ROOT_PASSWORD */
"Atualizando a senha do usuário 'root'...",
/* TR_PROBLEM_SETTING_ROOT_PASSWORD */
"Erro ao atualizar a senha do usuário 'root'.",
/* TR_ENTER_SETUP_PASSWORD */
"Digite a senha para o usuário 'setup'. Este é o usuário que " \
"tem acesso ao programa setup.",
/* TR_SETTING_SETUP_PASSWORD */
"Atualizando a senha do usuário 'setup'...",
/* TR_PROBLEM_SETTING_SETUP_PASSWORD */
"Erro ao atualizar a senha do usuario 'setup'.",
/* TR_ENTER_ADMIN_PASSWORD */
"Digite a senha para o usuário 'admin'. Este é o usuário que " \
"tem acesso à administração SmoothWall via web.",
/* TR_SETTING_ADMIN_PASSWORD */
"Atualizando a senha do usuário 'admin'...",
/* TR_PROBLEM_SETTING_ADMIN_PASSWORD */
"Erro ao atualizar a senha do usuário 'admin'.",
/* TR_PASSWORD_PROMPT */
"Senha:",
/* TR_AGAIN_PROMPT */
"Confirmação:",
/* TR_PASSWORD_CANNOT_BE_BLANK */
"A senha não pode ser nula.",
/* TR_PASSWORDS_DO_NOT_MATCH */
"As senhas não coincidem.",

/* hostname.c */
/* TR_ENTER_HOSTNAME */
"Atribua um nome a este computador.",
/* TR_HOSTNAME_CANNOT_BE_EMPTY */
"O nome do servidor não pode ser nulo.",
/* TR_HOSTNAME_CANNOT_CONTAIN_SPACES */
"O nome do servidor não pode conter espaços.",
/* TR_UNABLE_TO_WRITE_ETC_HOSTNAME */
"Impossível gravar /etc/hostname",

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
"Protocolo/País",
/* TR_ISDN_CARD */
"Adaptador ISDN",
/* TR_MSN_CONFIGURATION */
"Nº de telefone local (MSN/EAZ)",
/* TR_SET_ADDITIONAL_MODULE_PARAMETERS */
"Especifique parâmetros adicionais",
/* TR_RED_IN_USE */
"A linha ISDN (ou outra conexão externa) já está sendo utilizada.  Não é possível " \
"configurar a linha ISDN enquanto a interface VERMELHA estiver ativa.",
/* TR_ISDN_CONFIGURATION_MENU */
"Menú de configuração ISDN",
/* TR_ISDN_STATUS */
"A linha ISDN está %s.\n\n" \
"   Protocolo: %s\n" \
"   Interface: %s\n" \
"   Nº de telefone local: %s\n\n" \
"Selecione o item que deseja reconfigurar, ou utilize a configuração atual.",
/* TR_ISDN_NOT_YET_CONFIGURED */
"A linha ISDN ainda não foi configurada. Selecione o item que deseja configurar.",
/* TR_ENABLE_ISDN */
"Ativar ISDN",
/* TR_DISABLE_ISDN */
"Desativar ISDN",
/* TR_INITIALISING_ISDN */
"Inicializando ISDN...",
/* TR_UNABLE_TO_INITIALISE_ISDN */
"Impossível inicializar ISDN.",
/* TR_ISDN_NOT_SETUP */
"a linha ISDN ainda não foi configurada. Alguns items ainda não foram selecionados.",
/* TR_ISDN_PROTOCOL_SELECTION */
"Seleção de protocolo ISDN",
/* TR_CHOOSE_THE_ISDN_PROTOCOL */
"Escolha o protocolo adequado.",
/* TR_AUTODETECT */
"* AUTO-DETECÇÃO *",
/* TR_ISDN_CARD_SELECTION */
"Seleção do adaptador ISDN",
/* TR_CHOOSE_THE_ISDN_CARD_INSTALLED */
"Escolha o adaptador ISDN instalado neste computador.",
/* TR_CHECKING_FOR */
"Procurando: %s",
/* TR_ISDN_CARD_NOT_DETECTED */
"Adaptador ISDN não detectado. Pode ser necessário especificar parâmetros " \
"adicionais se o tipo do adaptador for ISA ou caso tenha requerimentos especiais.",
/* TR_DETECTED */
"Detectado: %s",
/* TR_UNABLE_TO_FIND_AN_ISDN_CARD */
"Adaptador ISDN não detectado. Pode ser necessário especificar parâmetros " \
"adicionais se o tipo do adaptador for ISA ou caso tenha requerimentos especiais.",
/* TR_ENTER_THE_LOCAL_MSN */
"Digite o nº de telefone local (MSN/EAZ).",
/* TR_PHONENUMBER_CANNOT_BE_EMPTY */
"O nº de telefone não pode ser nulo.",
/* TR_ENTER_ADDITIONAL_MODULE_PARAMS */
"Alguns adaptadores (especialmente os ISA) podem requerer parâmetros adicionais " \
"para especificar o IRQ e o endereço IO. Se possui um destes adaptadores, digite " \
"esses parâmetros aqui. Por exemplo: " \
"\"io=0x280 irq=9\". Estes serão usados durante a detecção.",

/* networking.c */
/* TR_PUSHING_NETWORK_DOWN */
"Desativando a rede...",
/* TR_PULLING_NETWORK_UP */
"Ativando a rede...",
/* TR_NETWORK_CONFIGURATION_TYPE */
"Tipo de configuração da rede",
/* TR_DRIVERS_AND_CARD_ASSIGNMENTS */
"Controladores e atribuição de adaptadores",
/* TR_ADDRESS_SETTINGS */
"Endereços",
/* TR_DNS_AND_GATEWAY_SETTINGS */
"DNS e Gateway",
/* TR_RESTART_REQUIRED */
"\n\nAo completar a configuração, a rede deve ser reinicializada.",
/* TR_CURRENT_CONFIG (first %s is type, second is restart message (or not) */
"Configuração atual: %s%s",
/* TR_NETWORK_CONFIGURATION_MENU */
"Menú de configuração da rede",
/* TR_NETWORK_CONFIGURATION_TYPE_LONG */
"Selecione a configuração de rede para o SmoothWall. Os seguintes tipos " \
"de configuração listam as interfaces que possuem conexão ethernet. " \
"Se alterar esta configuração, será necessário reinicializar a rede, e terá " \
"de reconfigurar a atribuição de controladores de rede.",
/* TR_PUSHING_NON_LOCAL_NETWORK_DOWN */
"Desativando a rede não-local...",
/* TR_YOUR_CONFIGURATION_IS_SINGLE_GREEN_ALREADY_HAS_DRIVER */
"A configuração atual especifica uma única interface VERDE, " \
"a qual já possui um controlador associado.",
/* TR_CONFIGURE_NETWORK_DRIVERS */
"Configure os controladores de rede, e as interfaces a que cada " \
"adaptador está associado. A configuração atual é a seguinte:\n\n",
/* TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS */
"\nPretende alterar estes parâmetros?",
/* TR_UNCLAIMED_DRIVER */
"Existe um adaptador ethernet livre do tipo:\n%s\n\n" \
"Pode atribuí-lo a:",
/* TR_CARD_ASSIGNMENT */
"Atribuição de adaptadores",
/* TR_PROBE */
"Auto-detecção",
/* TR_SELECT */
"Seleção",
/* TR_NO_UNALLOCATED_CARDS */
"Não existem adaptadores de rede livres, " \
"mas são necessários mais adaptadores. Pode optar por auto-detectar ou " \
"por escolher um controlador da lista.",
/* TR_UNABLE_TO_FIND_ANY_ADDITIONAL_DRIVERS */
"Não foi possivel encontrar controladores adicionais.",
/* TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED */
"Todos os adaptadores foram atribuidos com sucesso.",
/* TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED */
"Não foram atribuidos adaptadores suficientes.",
/* TR_MANUAL */
"* MANUAL *",
/* TR_SELECT_NETWORK_DRIVER */
"Selecionar controlador de rede",
/* TR_SELECT_NETWORK_DRIVER_LONG */
"Selecione o controlador de rede para o adaptador instalado neste " \
"computador. Se selecionar MANUAL, terá a oportunidade de especificar " \
"o nome do módulo controlador e eventuais parâmetros para controladores " \
"que tenham requisitos especiais, como por exemplo adaptadores ISA.",
/* TR_UNABLE_TO_LOAD_DRIVER_MODULE */
"Impossível carregar o módulo controlador.",
/* TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED */
"Este módulo controlador já está carregado.",
/* TR_MODULE_PARAMETERS */
"Digite o nome do módulo controlador e os respectivos parâmetros",
/* TR_LOADING_MODULE */
"Carregando módulo...",
/* TR_WARNING */
"AVISO",
/* TR_WARNING_LONG */
"Se alterar este endereço IP enquanto estiver ligado remotamente, " \
"a sua conexão ao computador SmoothWall será interompida, e terá de " \
"voltar a conectar-se através do novo endereço IP. Trata-se de um procedimento " \
"arriscado, e só deve ser tentado caso tenha acesso físico à máquina, para " \
"o caso de algo correr mal.",
/* TR_SINGLE_GREEN */
"O sistema está configurado para uma única interface VERDE.",
/* TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE */
"Selecione a interface que pretende reconfigurar.",
/* TR_DNS_GATEWAY_WITH_GREEN */
"A configuração atual não utiliza um adaptador ethernet associado à " \
"interface VERMELHA. As informações relativas a DNS e gateway para " \
"usuários de conexões telefônicas serão configuradas automaticamente " \
"ao efetuar a conexão.",
/* TR_PRIMARY_DNS */
"DNS primário:",
/* TR_SECONDARY_DNS */
"DNS secundário:",
/* TR_DEFAULT_GATEWAY */
"Gateway:",
/* TR_DNS_AND_GATEWAY_SETTINGS_LONG */
"Digite as informações relativas a DNS e gateway. Estas informações serão " \
"usadas apenas se o DHCP estiver desligado na interface VERMELHA.",
/* TR_PRIMARY_DNS_CR */
"DNS primário\n",
/* TR_SECONDARY_DNS_CR */
"DNS secundário\n",
/* TR_DEFAULT_GATEWAY_CR */
"Gateway\n",
/* TR_SECONDARY_WITHOUT_PRIMARY_DNS */
"DNS secundário especificado sem o respectivo DNS primário",
/* TR_UNKNOWN */
"DESCONHECIDO",
/* TR_NO_ORANGE_INTERFACE */
"Nenhuma interface LARANJA configurada.",
/* TR_MISSING_ORANGE_IP */
"Faltam as informações sobre o endereço IP da interface LARANJA.",
/* TR_NO_RED_INTERFACE */
"Nenhuma interface VERMELHA configurada.",
/* TR_MISSING_RED_IP */
"Faltam as informações sobre o endereço IP da interface VERMELHA.",

/* dhcp.c */
/* TR_START_ADDRESS */
"Endereço inicial:",
/* TR_END_ADDRESS */
"Endereço final:",
/* TR_DEFAULT_LEASE */
"Aluguel por defeito (mins):",
/* TR_MAX_LEASE */
"Aluguel máximo (mins):",
/* TR_DOMAIN_NAME_SUFFIX */
"Nome do domínio (sufixo):",
/* TR_CONFIGURE_DHCP */
"Configure o servidor DHCP digitando a informação dos seus parâmetros.",
/* TR_START_ADDRESS_CR */
"Endereço inicial\n",
/* TR_END_ADDRESS_CR */
"Endereço final\n",
/* TR_DEFAULT_LEASE_CR */
"Tempo de aluguel por defeito\n",
/* TR_MAX_LEASE_CR */
"Tempo de aluguel máximo\n",
/* TR_DOMAIN_NAME_SUFFIX_CR */
"Nome de domínio (sufixo)\n",

/* keymap.c */
/* TR_KEYBOARD_MAPPING */
"Mapas de teclado",
/* TR_KEYBOARD_MAPPING_LONG */
"Escolha o seu tipo de teclado na lista abaixo.",

/* timezone.c */
/* TR_TIMEZONE */
"Fuso horário",
/* TR_TIMEZONE_LONG */
"Escolha o seu fuso horário na lista abaixo.",

/* usbadsl.c */
/* TR_USB_CONTROLLER */
"Escolha o controlador USB",
/* TR_USBADSL_STATUS */
"USB ADSL está atualmente: %s\n" \
"   Controlador: %s\n\n" \
"Selecione o item que pretende reconfigurar, ou use a configuração actual.",
/* TR_USBADSL_CONFIGURATION */
"Configuração USB ADSL",
/* TR_ENABLE_USBADSL */
"Ativar USB ADSL",
/* TR_DISABLE_USBADSL */
"Desativar USB ADSL",
/* TR_INITIALISING_USBADSL */
"Inicializando USB ADSL.",
/* TR_UNABLE_TO_INITIALISE_USBADSL */
"Impossível inicializar USB ADSL",
/* TR_USBADSL_NOT_SETUP */
"USB ADSL não configurado.",
/* TR_USB_CONTROLLER_SELECTION */
"Seleção de controlador USB",
/* TR_CHOOSE_THE_USB_CONTROLLER_INSTALLED */
"Escolha o controlador USB instalado na máquina SmoothWall.",
/* TR_USB_CONTROLLER_NOT_DETECTED */
"Controlador USB não detectado.",
/* TR_UNABLE_TO_FIND_A_USB_CONTROLLER */
"Não foi possível encontrar um controlador USB..",
/* TR_STARTING_UP_USBADSL */
"Ativando USB ADSL..."

};
