/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: hostname.c
 * Stuff for setting the hostname. */
 
#include "setup.h"
 
extern FILE *flog;
extern char *logname;

extern char **ctr;

extern int automode;

int handlehostname(void)
{
	char hostname[STRING_SIZE] = "";
	struct keyvalue *kv = initkeyvalues();
	const char *values[] = { hostname, NULL };	/* pointers for the values. */
	struct newtWinEntry entries[] =
		{ { "", &values[0], 0,}, { NULL, NULL, 0 } };
	int rc;
	int result;
	
	if (!(readkeyvalues(kv, CONFIG_ROOT "main/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}	
	
	strcpy(hostname, "smoothwall"); findkey(kv, "HOSTNAME", hostname);
	
	for (;;)
	{	
		rc = newtWinEntries(ctr[TR_HOSTNAME], ctr[TR_ENTER_HOSTNAME],
			50, 5, 5, 40, entries, ctr[TR_OK], ctr[TR_CANCEL], NULL);	
		
		if (rc == 1)
		{
			strcpy(hostname, values[0]);
			if (!(strlen(hostname)))
				errorbox(ctr[TR_HOSTNAME_CANNOT_BE_EMPTY]);
			else if (strspn(hostname, LETTERS_NUMBERS "-.") != strlen(hostname))
				errorbox(ctr[TR_HOSTNAME_CANNOT_CONTAIN_SPACES]);
			else
			{			
				replacekeyvalue(kv, "HOSTNAME", hostname);
				writekeyvalues(kv, CONFIG_ROOT "main/settings");
				writehostsfiles();
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
	
	freekeyvalues(kv);
	
	return result;
}	
