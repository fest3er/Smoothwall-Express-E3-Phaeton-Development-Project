# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# (c) Spanish Translation Team
# Luis Ludovico
# Bruno Maderni
# Jaime Herrera
# Juan Moncayo
# Yonathan Sabbah
# Marco van Beek
# Ra|l Martmnez Peris

%basetr = (

%basetr,

# common
'invalid input' => 'Dato incorrecto',
'save' => 'Grabar', # button
'refresh' => 'Refrescar', # button
'restore' => 'Recuperar', # button
'error messages' => 'Mensajes de error:',
'back' => 'VOLVER',
'help' => 'Ayuda',
'primary dns' => 'DNS primario:',
'secondary dns' => 'DNS secundario:',
'invalid primary dns' => 'DNS primario inválido .',
'invalid secondary dns' => 'DNS secundario inválido.',
'dhcp server' => 'Server DHCP',
'username' => 'Nombre de usuario:',
'password' => 'Contraseña:',
'enabled' => 'Activo:',
'this field may be blank' => 'Este campo puede quedar vacio.',
'these fields may be blank' => 'Estos campos pueden quedar vacios.',

# header.pl
'sshome' => 'home',
'ssstatus' => 'estado',
'sstraffic graphs' => 'gráfica de tráfico',
'ssppp settings' => 'configuración ppp',
'ssmodem' => 'modem',
'ssusb adsl firmware upload' => 'actualización de fimrware usb adsl',
'ssssh' => 'ssh',
'sspasswords' => 'contraseñas',
'ssweb proxy' => 'proxy de web',
'ssdhcp' => 'dhcp',
'ssport forwarding' => 'reenvio de puertos',
'ssexternal service access' => 'acceso externo a los servicios',
'ssdmz pinholes' => 'dmz pinholes',
'ssdynamic dns' => 'dns dinamico',
'ssids' => 'sistema de detección de intrusos',
'sscontrol' => 'control',
'ssconnections' => 'conecciones',
'ssother' => 'otros',
'ssfirewall' => 'cortafuegos',
'ssshutdown' => 'apagar',
'ssshell' => 'shell',
'ssupdates' => 'actualizaciones',
'sshelp' => 'ayuda',
'sscredits' => 'creditos',
'ssip info' => 'Informción IP',
'powered by' => 'powered by',
'alt home' => 'Home', # alt
'alt information' => 'Información', # alt
'alt dialup' => 'Discado', # alt
'alt remote access' => 'Acceso remoto', # alt
'alt services' => 'Servicios', # alt
'alt ids' => 'Sistema de detección de intrusos', # alt
'alt vpn' => 'Redes privadas virtuales', # alt
'alt logs' => 'Registros', #alt 
'alt shutdown' => 'Apagar', # alt
'alt shell' => 'Shell', # alt
'alt updates' => 'Actualizaciones', # alt

# changepw.cgi
'admin user password has been changed' => 'La contraseña del usuario admin ha sido cambiada.',
'dial user password has been changed' => 'La contraseña del usuario user ha sido cambiada.',
'password cant be blank' => 'La contraseña no puede dejarse vacia.',
'passwords do not match' => 'La contraseñas ingresadas no conciden.',
'change passwords' => 'Cambiar las contraseñas',
'administrator user password' => 'Contraseña del usuario Admin:',
'dial user password' => 'Contraseña del usuario Dial:',
'again' => 'Nuevamente:',
'passwords must be at least 6 characters in length' => 'las contraseñas deben tener al menos 6 caracteres', 
'password contains illegal characters' => 'La contraseña contiene caracteres ilegales.',

# credits.cgi
'credits' => 'Créditos',
'version' => 'Versión: ',
'sponsors' => 'Patrocinadores',
'links' => 'Enlaces',
'smoothwall homepage' => 'Página de SmoothWall',
'translation teams' => 'equipos de la traducción',

# dhcp.cgi
'invalid start address' => 'Dirección inicial no válida.',
'invalid end address' => 'Dirección final no válida.',
'cannot specify secondary dns without specifying primary' => 'No se puede especificar un DNS secundario sin especificar el primario.',
'invalid default lease time' => 'Tiempo de asignación por defecto no válido.',
'invalid max lease time' => 'Tiempo máx de asignación no válido.',
'dhcp server enabled' => 'Servidor DHCP habilitado.  Reiniciando.',
'dhcp server disabled' => 'Servidor DHCP deshabilitado.  Detenido.',
'dhcp configuration' => 'Configuración DHCP',
'start address' => 'Dirección inicial:',
'end address' => 'Dirección final:',
'default lease time' => 'Tiempo de asignación por defecto (mins):',
'max lease time' => 'Máx tiempo de asignación (mins):',
'domain name suffix' => 'Sufijo del nombre de dominio:',

# proxy.cgi
'web proxy configuration' => 'Configuración del Web proxy',
'web proxyc' => 'Web proxy:',
'cache size' => 'Tamaño de Caché(MB):',
'invalid cache size' => 'Tamaño del caché no valido.',
'remote proxy' => 'Proxy remoto:',
'invalid maximum object size' => 'Tamaño máximo de objeto no válido.',
'invalid minimum object size' => 'Tamaño mínimo de objeto no válido.',
'invalid maximum outgoing size' => 'Tamaño máximo de salida no válido.',
'invalid maximum incoming size' => 'Tamaño máximo de entrada no válido.',
'transparent' => 'Transparente:',
'max size' => 'Tamaño máximo de objeto (KB):',
'min size' => 'Tamaño mínimo de objeto (KB):',
'max outgoing size' => 'Tamaño máximo de salida (KB):',
'max incoming size' => 'Tamaño máximo de entrada (KB):',

# common to logs cgis
'january' => 'Enero',
'february' => 'Febrero',
'march' => 'Marzo',
'april' => 'Abril',
'may' => 'Mayo',
'june' => 'Junio',
'july' => 'Julio',
'august' => 'Agosto',
'september' => 'Septiembre',
'october' => 'Octubre',
'november' => 'Noviembre',
'december' => 'Diciembre',
'month' => 'Mes:',
'day' => 'Día:',
'update' => 'Actualizar', # button
'export' => 'Exportar', # button
'older' => 'Anterior',
'newer' => 'Siguiente',
'settingsc' => 'Configuraciones:',

# logs.cgi/firewalllog.dat
'firewall log' => 'Registro del firewall',
'firewall log2' => 'Registro del firewall:',
'date' => 'Día:',
'time' => 'Hora',
'action' => 'Acción',
'chain' => 'Cadena',
'iface' => 'Interfaz',
'protocol' => 'Protocolo',
'source' => 'Fuente',
'src port' => 'Puerto origen',
'destination' => 'Destino',
'dst port' => 'Puerto Destino',
'unknown' => 'DESCONOCIDO',
'lookup' => 'Consulta',

# logs.cgi/log.dat
'log viewer' => 'Visor de registros',
'section' => 'Sección:',
'kernel' => 'Kernel',
'loginlogout' => 'Apertura/cierre de sesiones',
'update transcript' => 'Actualizar transcripciones',
'log' => 'Registro:',

# logs.cgi/proxylog.dat
'proxy log viewer' => 'Visualización del registro del Proxy',
'bad ignore filter' => 'Ignorar filtro erróneo:',
'caps all' => 'TODO',
'ignore filterc' => 'Ignorar filtro:',
'enable ignore filterc' => 'Activar ignorar filtro:',
'source ip' => 'Origen IP',
'website' => 'Sitio World Wide Web',

# logs.cgi/ids.dat
'ids log viewer' => 'Visor del registro IDS',
'datec' => 'Fecha:',
'namec' => 'Nombre:',
'priorityc' => 'Prioridad:',
'typec' => 'Tipo:',
'ipinfoc' => 'Información de IP:',
'referencesc' => 'Referencias:',
'none found' => 'no se encuentran datos',

# index.cgi
'main page' => 'Página principal',
'dial' => 'Marcar', # button
'hangup' => 'Colgar', # button
'current profile' => 'Perfil actual:',
'connected' => 'Conectado',
'dialing' => 'Marcando...',
'modem idle' => 'Módem inactivo',
'isdn idle' => 'RDSI inactiva',
'profile has errors' => 'El perfil tiene errores',
'modem settings have errors' => 'La configuración del móodem tiene errores',
'user pages' => 'Páginas del usuario',
'mstatus information' => 'Información&nbsp;de&nbsp;situación',
'mnetwork traffic graphs' => 'Gráficas&nbsp;de&nbsp;tráfico&nbsp;en&nbsp;la&nbsp;red',
'administrator pages' => 'Páginas del administrador',
'mppp setup' => 'Configuración&nbsp;de&nbsp;PPP&nbsp;(Protocolo de Punto a Punto)',
'mmodem configuration' => 'Configuración del módem',
'mchange passwords' => 'Cambio de contraseñas',
'mremote access' => 'Acceso remoto',
'mdhcp configuration' => 'Configuración&nbsp;de&nbsp;DHCP',
'mproxy configuration' => 'Configuración&nbsp;del&nbsp;proxy',
'mport forwarding configuration' => 'Configuración del puerto de reenvío',
'mshutdown control' => 'Control de apagado',
'mlog viewer' => 'Visor de registros',
'mfirewall log viewer' => 'Visor de registros del cortafuegos',
'msecure shell' => 'Consola de mandatos Segura (SSH - Secure Shell)',
'modem dod waiting' => 'Módem a la espera para marcar bajo demanda',
'isdn dod waiting' => 'RDSI a la espera para marcar bajo demanda',
'pppoe idle' => 'PPPOE inactiva',
'usbadsl idle' => 'Conexión ADSL por USB inactiva',
'pppoe dod waiting' => 'PPPOE a la espera para marcar bajo demanda',
'there are updates' => 'Hay actualizaciones disponibles para su sistema. Por favor, visite la sección "Actualizaciones" para más información.',
'updates is old1' => 'Su archivo de actualización ',
'updates is old2' => 'días desfasado. Le recomendamos actualice su sistema visitando la páginas "Actualizaciones".',

# pppsetup.cgi
'unable to alter profiles while red is active' => 'Inahbilitado de modificar el perfil mientras la interfaz RED este activa.', 
'profile name not given' => 'No se ha dado nombre al perfil.',
'telephone not set' => 'Teléfono no configurado.',
'bad characters in the telephone number field' => 'Carácteres erróneos en el campo del número de teléfono',
'username not set' => 'Usuario no establecido.',
'spaces not allowed in the username field' => 'No se permiten espacios para el nombre de usuario.',
'password not set' => 'Contraseña no establecida.',
'spaces not allowed in the password field' => 'No se permiten espacios para la contraseña de usuario.',
'idle timeout not set' => 'Tiempo máximo de inactividad no establecido.',
'only digits allowed in the idle timeout' => 'Sólo se permiten números en el campo del tiempo máximo de inactividad.',
'bad characters in script field' => 'Caracteres no válidos en el campo del guión',
'max retries not set' => 'Número máximo de reintentos no establecido.',
'only digits allowed in max retries field' => 'Sólo se permiten números en el campo del máximo número de reintentos.',
'profile saved' => 'Perfil guardado: ',
'select' => 'Seleccionar', # button
'profile made current' => 'Perfil seleccionado actualmente: ',
'the selected profile is empty' => 'El perfil seleccionado está vacío.',
'delete' => 'Borrar', # button
'profile deleted' => 'Perfil borrado: ',
'empty' => 'Vacío',
'unnamed' => 'Anónimo',
'ppp setup' => 'Configuración de PPP',
'profiles' => 'Perfiles:',
'profile name' => 'Nombre del perfil:',
'telephony' => 'Teléfono:',
'interface' => 'Interfaz:',
'modem on com1' => 'Módem en COM1',
'modem on com2' => 'Módem en COM2',
'modem on com3' => 'Módem en COM3',
'modem on com4' => 'Módem en COM4',
'isdn tty' => 'RDSI (ISDN) a TTY',
'isdn1' => 'RDSI (ISDN) sencillo',
'isdn2' => 'RDSI (ISDN) dual',
'computer to modem rate' => 'Velocidad del puerto del ordenador al módem:',
'number' => 'Número:',
'modem speaker on' => 'Altavoz del módem encendido:',
'dialing mode' => 'Modo de marcado:',
'tone' => 'Tono',
'pulse' => 'Pulso',
'maximum retries' => 'Número máximo de reintentos:',
'idle timeout' => 'Tiempo máximo de inactividad (en minutos; 0 para deshabilitar):',
'persistent connection' => 'Conexión constante (vuelve a conectar en caso de desconexión):',
'authentication' => 'Autentificación:',
'method' => 'Método:',
'pap or chap' => 'PAP o CHAP',
'standard login script' => 'Guión de conexión',
'demon login script' => 'Guión demonio de conexión ',
'other login script' => 'Otro guión de conexión',
'script name' => 'Nombre del guión:',
'type' => 'Tipo:',
'manual' => 'Manual',
'automatic' => 'Automático',
'dod' => 'Marcado bajo demanda:',
'dod for dns' => 'Marcado bajo demanda para DNS:',
'connect on smoothwall restart' => 'Conectar al reiniciar SmoothWall:',
'pppoe settings' => 'Configuración adicional de PPPoE:',
'usb adsl settings' => 'Configuración adicional USB ADSL:',
'service name' => 'Nombre del servicio:',
'concentrator name' => 'Nombre del concentrador:',
'vpi number' => 'Número de VPI:',
'vci number' => 'Número de VCI:',
'firmwarec' => 'Microcódigo del dispositivo:',
'firmware present' => 'Microcódigo del dispositivo disponible',
'firmware not present' => 'Microcódigo del dispositivo <B>NO</B> disponible',
'upload usb adsl firmware' => 'Cargar microcódigo de USB ADSL',
'dial on demand for this interface is not supported' => 'Discado bajo demanda no esta soportado para esta interfaz.',
'no usb adsl firmware' => 'No hay firmware usb adsl',

# remote.cgi
'ssh is enabled' => 'SSH activo. Reiniciando',
'ssh is disabled' => 'SSH inactivo. Desconectando',
'remote access' => 'Acceso remoto',
'remote access2' => 'Acceso remoto:',

# shutdown.cgi
'shutting down smoothwall' => 'Apagando SmoothWall',
'shutdown control' => 'Funciones de apagado',
'shutdown' => 'Apagar', # button
'shutdown2' => 'Apagar:',
'shutting down' => 'Apagando',
'smoothwall has now shutdown' => 'SmoothWall se ha apagado correctamente',
'rebooting smoothwall' => 'Reiniciando SmoothWall', 
'reboot' => 'Reinciar', # button 
'rebooting' => 'Reiniciando', 
'smoothwall has now rebooted' => 'SmoothWall esta siendo reinciado ahora.', 

# status.cgi
'web server' => 'Servidor de Web',
'cron server' => 'Servidor de CRON',
'dns proxy server' => 'Servidor de DNS proxy',
'logging server' => 'Servidor de registro',
'kernel logging server' => 'Servidor de registro del Núcleo (SSH)',
'secure shell server' => 'Consola segura del servidor',
'vpn' => 'VPN',
'web proxy' => 'Web proxy',
'intrusion detection system' => 'Sistema de detección de intrusos',
'status information' => 'Informe de situación',
'services' => 'Servicios:',
'memory' => 'Memoria:',
'uptime and users' => 'Tiempo en marcha de las operaciones y usuarios:',
'interfaces' => 'Interfaces:',
'disk usage' => 'Cantidad de disco usado:',
'loaded modules' => 'Módulos cargados:',
'kernel version' => 'Versión del Núcleo:',
'stopped' => 'PARADO',
'running' => 'EN MARCHA',
'swapped' => 'INTERCAMBIADO',

# portfw.cgi and dmzhole.cgi and xtaccess.cgi
'source port numbers' => 'Puerto de origen debe ser un número.',
'source is bad' => 'Dirección IP o de red invalida.',
'destination ip bad' => 'Dirección IP de destino invalida',
'destination port numbers' => 'Puerto de destino debe ser un número.',
'unable to open file' => 'Incapaz de abrir el archivo',
'source port in use' => 'Puerto de destino en uso:',
'forwarding rule added' => 'Regla de reenvio agregada; reiniciando reenvio',
'forwarding rule removed' => 'Regla de reenvio eliminada; reiniciando reenvio',
'external access rule added' => 'Regla de acceso externa agregada; reiniciando control de acceso',
'external access rule removed' =>' Regla de acceso externa eliminada; reiniciando control de acceso',
'dmz pinhole rule added' => 'Regla DMZ pinhole agregada; reiniciando DMZ pinhole',
'dmz pinhole rule removed' => 'Regla DMZ pinhole eliminada; reiniciando DMZ pinhole',
'port forwarding configuration' => 'Configuración de reenvio de puertos',
'dmz pinhole configuration' => 'Configuración DMZ pinhole',
'external access configuration' => 'Configuración de acceso externo',
'add a new rule' => 'Agregar un nueva regla:',
'sourcec' => 'IP de origen, o red (vacio para "TODAS"):',
'source ipc' => 'IP de origen:',
'source portc' => 'Puerto de origen:',
'destination ipc' => 'IP de destino:',
'destination portc' => 'Puerto de destino:',
'current rules' => 'Reglas actuales:',
'source ip' => 'IP de origen',
'source port' => 'Puerto de origen',
'destination ip' => 'IP de destino',
'destination port' => 'Puerto de destino',
'add' => 'Agregar', # button
'remove' => 'Eliminar', # button
'edit' => 'Editart', # button
'enabledtitle' => 'Activo',
'nothing selected' => 'Nada seleccionado',
'you can only select one item to edit' => 'Debe seleccionar solo un item para editar',
'mark' => 'Seleccionar',
'all' => 'TODOS',

# ddns.cgi
'dynamic dns' => 'DNS dinámico',
'add a host' => 'Agregar un ordenador:',
'servicec' => 'Servicio:',
'behind a proxy' => 'Detras de un proxy:',
'enable wildcards' => 'Activar comodines:',
'hostnamec' => 'Nombre de Ordenador:',
'domainc' => 'Dominio:',
'current hosts' => 'Ordenadores actuales:',
'service' => 'Servicioe',
'hostname' => 'Nombre de ordenador',
'domain' => 'Dominio',
'proxy' => 'Proxy',
'wildcards' => 'Comodines',
'hostname not set' => 'Nombre de ordenador no configurado.',
'domain not set' => 'Dominio no configurado.',
'invalid hostname' => 'Nombre de ordenador inválido.',
'invalid domain name' => 'Nombre de dominio inválido.',
'hostname and domain already in use' => 'Nombre de ordenador y dominio ya se encuentran en uso.',
'ddns hostname added' => 'Nombre de ordenador agregado al DNS dinámico',
'ddns hostname removed' => 'Nombre de ordenador eliminado del DNS dinámico',
'force update' => 'Forzar actualización',

# ipinfo.cgi
'ip info' => 'Información de dirección IP',
'lookup failed' => 'Fallo la busqueda invertida',

# shell.cgi
'secure shellc' => 'Shell seguro:',

# modem.cgi
'restore defaults' => 'Valores predeterminados', # button
'timeout must be a number' => 'Timeout debe ser un número.',
'modem configuration' => 'Configuración del Modem',
'modem configurationc' => 'Configuración del Modem:',
'init string' => 'Inicialización:',
'hangup string' => 'Cortar:',
'speaker on' => 'Activar parlante:',
'speaker off' => 'Descativar parlante:',
'tone dial' => 'Discado por tono:',
'pulse dial' => 'Discado por pulso:',
'connect timeout' => 'Tiempo para establecer conexión:',
'send cr' => 'ISP requiere Retorno de Carro:',

# vpnmain.cgi
'restart' => 'Reiniciar',
'stop' => 'Detener',
'vpn configuration main' => 'Configuración de VPN - Principal',
'main' => 'Principal',
'connections' => 'Connecciones',
'global settingsc' => 'Configuración general:',
'local vpn ip' => 'IP local de VPN:',
'if blank the currently configured ethernet red address will be used' => 'Si es vacio, se utilizara la configuración actual de la Zona Ethernet RED.',
'manual control and status' => 'Control manual y estado:',
'connection name' => 'Nombre',
'connection status' => 'Estado',
'capsclosed' => 'CERRADO',
'capsdisabled' => 'INACTIVO',
'capsopen' => 'ABIERTO',

# vpn.cgi/vpnconfig.dat
'name must only contain characters' => 'El nombre debe contener sólo caracteres.',
'left ip is invalid' => 'IP izquierdo inválido.',
'left next hop ip is invalid' => 'El próximo salto de la izquierda tiene IP inválido.',
'left subnet is invalid' => 'La subred izquierda es inválida.',
'right ip is invalid' => 'El IP derecho es inválido.',
'right next hop ip is invalid' => 'El próximo salto de la derecha tiene IP inválido.',
'right subnet is invalid' => 'La subred derecha es inválida.',
'vpn configuration connections' => 'Configuración de VPN - Conexiones',
'add a new connection' => 'Agregar una nueva conexión:',
'namec' => 'Nombre:',
'leftc' => 'Izquierda:',
'left next hopc' => 'Próximo salto de la izquierda:',
'left subnetc' => 'Subred izquierda:',
'rightc' => 'Derecha:',
'right next hopc' => 'Próximo salto de la derecha:',
'right subnetc' => 'Subred derecha:',
'secretc' => 'Secreto:',
'current connections' => 'Conexiones actuales:',
'markc' => 'Marca:',
'import and export' => 'Importar y Exportar:',
'import' => 'Importar', # button

# graphs.cgi
'network traffic graphs' => 'Gráficos de tráfico en la red',
'network traffic graphsc' => 'Gráficos de tráfico en la red:',
'no graphs available' => 'No hay gráficos disponibles.',
'no information available' => 'No hay información disponible.',

# usbadsl.cgi
'usb adsl setup' => 'Configuración de ADSL por USB',
'usb adsl help' => 'Para utilizar el módem USB debe debe cargar el firmware en su computador SmoothWall. Por favor descargue el tarball desde Alcatel y después cargue el archivo <B>mgmt.o</B> usando el formulario de abajo.',
'upload' => 'Cargar', # button
'upload successfull' => 'Carga exitosa.',
'could not create file' => 'No se pudo crear el archivo.',
'mgmt upload' => 'Cargar controlador para ADSL por USB:',
'upload filec' => 'Cargar archivo:',

# updates.cgi
'updates' => 'Actualizaciones',
'could not open available updates file' => 'No se pudo abrir el archivo de actualizaciones disponibles.',
'could not download the available updates list' => 'No se pudo descargar la lista de actulizaciones disponibles.',
'could not create directory' => 'No se pudo crear directorio.',
'could not open updatefile for writing' => 'No se pudo abrir para escritura el archivo de actualización.',
'this is not an authorised update' => 'Esta no es una actualización autorizada, o su lista de parches es antigua.',
'this is not a valid archive' => 'Este no el un archivo válido.',
'could not open update information file' => 'No se pudo abrir el archivo de información para actualización. La actualización esta corrupta.',
'this update is already installed' => 'Esta actualización ya se encuentra instalada.',
'package failed to install' => 'Fall&oacute la instalaci&oacuten del paquete.',
'update installed but' => 'Actualilización instalada pero la base de datos de paquetes instalados no pudo ser actualizada',
'refresh update list' => 'Refrescar lista de actualizaciones', # button
'installed updates' => 'Actualizaciones instaladas:',
'id' => 'ID',
'title' => 'Título',
'description' => 'Descripción',
'released' => 'Liberado',
'installed' => 'Instalado',
'could not open installed updates file' => 'No pudo ser abierto el archivo de actualizaciones',
'available updates' => 'Actualizaciones disponibles:',
'there are updates available' => 'Hay actualizaciones para su sistema. Es muy importante que las instale tan pronto como le sea posible.',
'info' => 'Información',
'all updates installed' => 'Todas las actualizaciones instaladas',
'install new update' => 'Instalar actualización:',
'to install an update' => 'Para instalar una actualización por favor cargue abajo el archivo .tar.gz:',
'upload update file' => 'Cargar archivo de actualización:',
'could not download latest patch list' => 'No se pudo descargar la última lista de parches (no conectado).',
'could not connect to smoothwall org' => 'No se pudo conectar con smoothwall.org',
'successfully refreshed updates list' => 'La lista de actualizaciones fue actualizada exitosamente.',
'the following update was successfully installedc' => 'La siguiente actualización fue instalada exitosamente:',

# ids.cgi
'snort is enabled' => 'SNORT está habilitado',
'snort is disabled' => 'SNORT está deshabilitado',
'intrusion detection system2' => 'Sistema de Detección de Intrusiones:',

);
