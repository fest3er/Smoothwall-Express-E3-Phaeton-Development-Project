/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: defaults.c
 * Stuff for setting default protection level. */

#include "setup.h"
 
extern FILE *flog;
extern char *logname;

extern char **ctr;

extern int automode;
extern int rebootrequired;

int handledefaults(void)
{
	int c;
	int choice;
	struct keyvalue *kv = initkeyvalues();
	char openness[STRING_SIZE];
	char newopenness[STRING_SIZE];
	char *opennesses[] = { "open", "halfopen", "closed", NULL };
	char *displayopennesses[] = { ctr[TR_OPENNESS_OPEN], ctr[TR_OPENNESS_HALFOPEN],
		ctr[TR_OPENNESS_CLOSED], NULL };
	int rc;
	int result;
	char commandstring[STRING_SIZE - 1];
	
	memset(openness, 0, STRING_SIZE - 1);
	memset(newopenness, 0, STRING_SIZE - 1);
	memset(commandstring, 0, STRING_SIZE - 1);
		
	if (!(readkeyvalues(kv, CONFIG_ROOT "main/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}
	
	strncpy(openness, "closed", STRING_SIZE - 1);
	findkey(kv, "OPENNESS", openness);
	
	choice = 0;
	for (c = 0; opennesses[c]; c++)
	{
		if (strcmp(openness, opennesses[c]) == 0)
			choice = c;
	}
	
	rc = newtWinMenu(ctr[TR_DEFAULT_OPENNESS], ctr[TR_DEFAULT_OPENNESS_LONG], 75, 30, 30, 3, displayopennesses, &choice,
		ctr[TR_OK], ctr[TR_CANCEL], NULL);

	strncpy(newopenness, opennesses[choice], STRING_SIZE - 1);
	
	if (rc != 2)
	{
		/* Copy the config thingies if its changed OR its the inital
		 * install run of the setup program. */
		if (automode || strcmp(openness, newopenness) != 0)
		{
			snprintf(commandstring, STRING_SIZE - 1, "/usr/bin/smoothwall/installdefaults.pl %s",
				newopenness);
			mysystem(commandstring);
		
			replacekeyvalue(kv, "OPENNESS", newopenness);
			writekeyvalues(kv, CONFIG_ROOT "main/settings");
			
			rebootrequired = 1;
		}
		result = 1;
	}
	else
		result = 0;	
	
	freekeyvalues(kv);	
	
	return result;
}
