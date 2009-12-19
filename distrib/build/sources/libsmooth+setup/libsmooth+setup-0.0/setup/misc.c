/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: misc.c
 * Misc. stuff for the lib. */
 
#include "setup.h"

extern FILE *flog;
extern char *logname;

extern char **ctr;
 
extern int automode;

/* This will rewrite /etc/hosts, /etc/hosts.*, and the apache ServerName file. */
int writehostsfiles(void)
{	
	char address[STRING_SIZE] = "";
	char netaddress[STRING_SIZE] = "";
	char netmask[STRING_SIZE] = "";
	FILE *file;
	struct keyvalue *kv;
	char hostname[STRING_SIZE];
	char commandstring[STRING_SIZE];
	
	kv = initkeyvalues();
	if (!(readkeyvalues(kv, CONFIG_ROOT "ethernet/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}
	findkey(kv, "GREEN_ADDRESS", address);
	findkey(kv, "GREEN_NETADDRESS", netaddress);
	findkey(kv, "GREEN_NETMASK", netmask);	
	freekeyvalues(kv);
	
	kv = initkeyvalues();
	if (!(readkeyvalues(kv, CONFIG_ROOT "main/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}
	strcpy(hostname, "smoothwall"); findkey(kv, "HOSTNAME", hostname);
	freekeyvalues(kv);
		
	if (!(file = fopen(CONFIG_ROOT "main/hostname.conf", "w")))
	{
		errorbox(ctr[TR_UNABLE_TO_WRITE_VAR_SMOOTHWALL_MAIN_HOSTNAMECONF]);
		return 0;
	}
	fprintf(file, "ServerName %s\n", hostname);
	fclose(file);
	
	if (mysystem("/usr/bin/smoothwall/writehosts.pl"))
	{
		errorbox(ctr[TR_UNABLE_TO_WRITE_ETC_HOSTS]);
		return 0;
	}
	
	sprintf(commandstring, "/bin/hostname %s", hostname);
	if (mysystem(commandstring))
	{
		errorbox(ctr[TR_UNABLE_TO_SET_HOSTNAME]);
		return 0;
	}
	
	return 1;
}	
