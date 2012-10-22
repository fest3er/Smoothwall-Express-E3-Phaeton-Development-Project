/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 * 
 * filename: passwords.c
 * Password stuff. */

#include "setup.h"

extern FILE *flog;
extern char *logname;

extern char **ctr;

extern int automode;

int getpassword(char *password, char *text);

/* Root password. */
int handlerootpassword(void)
{
	char password[STRING_SIZE];
	char commandstring[STRING_SIZE];
		
	/* Root password. */
	if (getpassword(password, ctr[TR_ENTER_ROOT_PASSWORD]) == 2)
		return 0;
	
	snprintf(commandstring, STRING_SIZE,
		"/bin/echo 'root:%s' | /usr/sbin/chpasswd", password);
	if (runcommandwithstatusnolog(commandstring, ctr[TR_SETTING_ROOT_PASSWORD]))
	{
		errorbox(ctr[TR_PROBLEM_SETTING_ROOT_PASSWORD]);
		return 0;
	}
	
	return 1;
}

int handlesetuppassword(void)
{
	char password[STRING_SIZE];
	char commandstring[STRING_SIZE];
		
	/* Root password. */
	if (getpassword(password, ctr[TR_ENTER_SETUP_PASSWORD]) == 2)
		return 0;
	
	snprintf(commandstring, STRING_SIZE,
		"/bin/echo 'setup:%s' | /usr/sbin/chpasswd", password);
	if (runcommandwithstatusnolog(commandstring, ctr[TR_SETTING_SETUP_PASSWORD]))
	{
		errorbox(ctr[TR_PROBLEM_SETTING_SETUP_PASSWORD]);
		return 0;
	}
	
	return 1;
}


int handleadminpassword(void)
{
	char password[STRING_SIZE];
	char commandstring[STRING_SIZE];
		
	/* SmoothWall admin password. */
	if (getpassword(password, ctr[TR_ENTER_ADMIN_PASSWORD]) == 2)
		return 0;
	
	snprintf(commandstring, STRING_SIZE,
		"/usr/sbin/htpasswd -m -c -b " CONFIG_ROOT "auth/users admin '%s'", password);
	if (runcommandwithstatusnolog(commandstring, ctr[TR_SETTING_ADMIN_PASSWORD]))
	{
		errorbox(ctr[TR_PROBLEM_SETTING_ADMIN_PASSWORD]);
		return 0;
	}

	return 1;
}

/* Taken from the cdrom one. */
int getpassword(char *password, char *text)
{
	char *values[] = {	NULL, NULL, NULL };	/* pointers for the values. */
	struct newtWinEntry entries[] =
	{ 
		{ ctr[TR_PASSWORD_PROMPT], (const char **) &values[0], NEWT_FLAG_PASSWORD },
		{ ctr[TR_AGAIN_PROMPT], (const char **) &values[1], NEWT_FLAG_PASSWORD },
		{ NULL, NULL, 0 }
	};
	int rc;
	int done;
	
	do
	{
		done = 1;
		rc = newtWinEntries(TITLE, text,
			50, 5, 5, 25, entries, ctr[TR_OK], ctr[TR_CANCEL], NULL);
		
		if (rc != 2)
		{
			if (strlen(values[0]) == 0 || strlen(values[1]) == 0)
			{
				errorbox(ctr[TR_PASSWORD_CANNOT_BE_BLANK]);
				done = 0;
				strcpy(values[0], "");
				strcpy(values[1], "");				
			}
			else if (strcmp(values[0], values[1]) != 0)
			{
				errorbox(ctr[TR_PASSWORDS_DO_NOT_MATCH]);
				done = 0;
				strcpy(values[0], "");
				strcpy(values[1], "");					
			}
		}
	}
	while (!done);

	strncpy(password, values[0], STRING_SIZE);

	if (values[0]) free((char *) values[0]);
	if (values[1]) free((char *) values[1]);

	return rc;
}
	
