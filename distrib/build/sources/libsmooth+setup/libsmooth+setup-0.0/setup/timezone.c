/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: timezone.c
 * Stuff for setting the timezone. */

#include "setup.h"
 
extern FILE *flog;
extern char *logname;

extern char **ctr;

extern int automode;

#define MAX_FILENAMES 5000
#define ZONEFILES "/usr/share/zoneinfo/posix"

static int filenamecount;
static char *filenames[MAX_FILENAMES];
static char *displaynames[MAX_FILENAMES];

static int process(char *prefix, char *path);
static int cmp(void *s1, void *s2);

int handletimezone(void)
{
	int c;
	int choice;
	char *temp;
	struct keyvalue *kv = initkeyvalues();	
	int rc;
	int result;
	int tz_offset = 0;
	char timezone[STRING_SIZE];

	filenamecount = 0;	

	process(ZONEFILES, "");
	filenames[filenamecount] = NULL;
	qsort(filenames, filenamecount, sizeof(char *), cmp);
	
	for (c = 0; filenames[c]; c++)
	{
		displaynames[c] = malloc(STRING_SIZE);
		if ((temp = strstr(filenames[c], ZONEFILES)))
			strcpy(displaynames[c], temp + strlen(ZONEFILES) + 1);
		else
			strcpy(displaynames[c], filenames[c]);
	}
	displaynames[c] = NULL;
	
	if (!(readkeyvalues(kv, CONFIG_ROOT "time/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}	
	
	strcpy(timezone, ZONEFILES "/Europe/London");
	findkey(kv, "TIMEZONE", timezone+strlen(ZONEFILES)+1);
	
	choice = 0;
	for (c = 0; filenames[c]; c++)
	{
		if (strcmp(timezone, filenames[c]) == 0)
			choice = c;
	}
	
	rc = newtWinMenu(ctr[TR_TIMEZONE], ctr[TR_TIMEZONE_LONG], 50, 5, 5, 6, displaynames, &choice,
		ctr[TR_OK], ctr[TR_CANCEL], NULL);

	strcpy(timezone, filenames[choice]);
	
	if (rc != 2)
	{
		if (strncmp(timezone, ZONEFILES, strlen(ZONEFILES)) == 0)
		{
			tz_offset = strlen(ZONEFILES)+1;
		}
		else
		{
			tz_offset = 0;
		}
		replacekeyvalue(kv, "TIMEZONE", timezone+tz_offset);
		writekeyvalues(kv, CONFIG_ROOT "time/settings");
		unlink(CONFIG_ROOT "time/localtime");
		symlink(timezone, CONFIG_ROOT "time/localtime");
		result = 1;
	}
	else
		result = 0;	
	
	for (c = 0; filenames[c]; c++)
	{
		free(filenames[c]);
		free(displaynames[c]);
	}
	freekeyvalues(kv);	
	
	return result;
}

static int process(char *prefix, char *path)
{
	DIR *dir;
	struct dirent *de;
	char newpath[PATH_MAX];
	
	snprintf(newpath, PATH_MAX, "%s%s", prefix, path);
	
	if (!(dir = opendir(newpath)))
	{
		if (filenamecount > MAX_FILENAMES)
			return 1;
		
		filenames[filenamecount] = (char *) strdup(newpath);
		filenamecount++;
		return 0;
	}
			
	while ((de = readdir(dir)))
	{
		if (de->d_name[0] == '.') continue;
		snprintf(newpath, PATH_MAX, "%s/%s", path, de->d_name);
		process(prefix, newpath);
	}
	closedir(dir);
	
	return 1;
}

/* Small wrapper for use with qsort(). */		
static int cmp(void *s1, void *s2)
{
	return (strcmp(* (char **) s1, * (char **) s2));
}
