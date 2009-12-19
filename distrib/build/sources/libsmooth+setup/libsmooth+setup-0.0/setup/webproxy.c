/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001,2003
 *
 * filename: webproxy.c
 * Stuff for setting the what proxy smoothie should use itself. */
 
#include "setup.h"

extern int logfd;
extern int nullfd;

extern char **ctr;

extern int automode;

int handlewebproxy(void)
{
	char server[STRING_SIZE] = "";
	char port[STRING_SIZE] = "";
	struct keyvalue *kv = initkeyvalues();
	const char *values[] = { server, port, NULL };	/* pointers for the values. */
	struct newtWinEntry entries[] =
		{ { ctr[TR_HOSTNAMEC], &values[0], 0,}, { ctr[TR_PORTC], &values[1], 0 }, { NULL, NULL, 0 } };
	int rc = 0;
	int result = 0;
	int portn = 0;

	memset(server, 0, STRING_SIZE);
	memset(port, 0, STRING_SIZE);
	
	if (!(readkeyvalues(kv, CONFIG_ROOT "main/proxy")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}	
	
	strncpy(server, "", STRING_SIZE - 1); findkey(kv, "SERVER", server);
	strncpy(port, "", STRING_SIZE - 1); findkey(kv, "PORT", port);
	
	for (;;)
	{	
		rc = newtWinEntries(ctr[TR_WEB_PROXY], ctr[TR_WEB_PROXY_LONG],
			60, 5, 5, 40, entries, ctr[TR_OK], ctr[TR_CANCEL], NULL);	
		
		if (rc == 1)
		{
			strncpy(server, values[0], STRING_SIZE - 1);
			strncpy(port, values[1], STRING_SIZE - 1);
			portn = atol(port);
			if (strlen(server) && strchr(server, ' '))
				errorbox(ctr[TR_HOSTNAME_CANNOT_CONTAIN_SPACES]);
			else if (strlen(port) && !(portn > 0 && portn <= 65535))
				errorbox(ctr[TR_PORT_IS_INVALID]);
			else if (strlen(server) && !strlen(port))
				errorbox(ctr[TR_PORT_NOT_GIVEN]);		
			else
			{			
				replacekeyvalue(kv, "SERVER", server);
				replacekeyvalue(kv, "PORT", port);
				writekeyvalues(kv, CONFIG_ROOT "main/proxy");
				result = 1;
				break;
			}
		}
		else
		{
			result = 0;
			break;
		}
	}
	free((char *) values[0]);
	free((char *) values[1]);
	freekeyvalues(kv);
	
	return result;
}	
