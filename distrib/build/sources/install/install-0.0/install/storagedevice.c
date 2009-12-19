/* SmoothWall install program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: storagedevice.c
 * SCSI stuff. */

#include "install.h"

extern FILE *flog;
extern char *logname;

extern char **ctr;

#define MAX_STORAGE_DEVICES 50
#define STORAGE_DEVICES_FILENAME "/etc/storagedevices"

struct storagedevicedriver sdds[MAX_STORAGE_DEVICES];

int initstoragedevices(void)
{
	FILE *hfile = NULL;
	int c = 0;
	char s[STRING_SIZE];
	char *result = NULL;
	int count = 0;
	
	memset(s, 0, STRING_SIZE);
	
	memset(&sdds, 0, sizeof(struct storagedevicedriver) * MAX_STORAGE_DEVICES);
	
	if (!(hfile = fopen(STORAGE_DEVICES_FILENAME, "r")))
		return -1;
	
	while (fgets(s, STRING_SIZE, hfile) != NULL && c < MAX_STORAGE_DEVICES - 1)
	{
		if (s[strlen(s) - 1] == '\n')
			s[strlen(s) - 1] = '\0'; 
		result = strtok(s, "|");
		
		count = 0;
		
		while (result)
		{
			if (count == 0)
				strncpy(sdds[c].description, result, STRING_SIZE - 1);
			else if (count == 1)
				strncpy(sdds[c].path, result, STRING_SIZE - 1);
			else 
				strncpy(sdds[c].modulename, result, STRING_SIZE - 1);
			count++;
			result = strtok(NULL, "|");
		}
		strncpy(sdds[c].moduleoptions, "", STRING_SIZE - 1);
		c++;
	}
	
	fclose(hfile);

	return c;
}	
	
int probestoragedevicedriver(struct storagedevicedriver *sdd)
{
	int c = 0;
	char message[BIG_STRING_SIZE];
	char commandstring[STRING_SIZE];
	struct blockdevice hdbd;
	
	memset(message, 0, BIG_STRING_SIZE);
	memset(commandstring, 0, STRING_SIZE);

	strncpy(sdd->description, "", STRING_SIZE - 1);
	strncpy(sdd->modulename, "", STRING_SIZE - 1);
	strncpy(sdd->moduleoptions, "", STRING_SIZE - 1);
	memset(&hdbd, 0, sizeof(struct blockdevice));

	while (strlen(sdds[c].modulename))
	{
		snprintf(message, BIG_STRING_SIZE - 1, ctr[TR_LOOKING_FOR_STORAGE_INTERFACE], sdds[c].description);
		snprintf(commandstring, STRING_SIZE - 1, "/sbin/modprobe %s", sdds[c].modulename);
		if (runcommandwithstatus(commandstring, message) == 0)
		{
			findharddiskorcdrom(&hdbd, DISK_HD);
			if (hdbd.present)
			{
				strncpy(sdd->description, sdds[c].description, STRING_SIZE - 1);
				strncpy(sdd->path, sdds[c].path, STRING_SIZE - 1);
				strncpy(sdd->modulename, sdds[c].modulename, STRING_SIZE - 1);
				strncpy(sdd->moduleoptions, "", STRING_SIZE - 1);
				return 1;
			}
		}
		
		snprintf(commandstring, STRING_SIZE - 1, "/sbin/rmmod %s", sdds[c].modulename);
		mysystem(commandstring);
		
		c++;
	}
	
	return 0;
}

/* A listbox for selected the card... */
static int choosestoragedevicedriver(struct storagedevicedriver *sdd)
{
	int c = 0;
	char **sections;
	int drivercount = 0;
	int rc = 0;
	int choice = 0;
	char message[BIG_STRING_SIZE];
	char commandstring[STRING_SIZE];
	int done = 0;
	
	memset(message, 0, BIG_STRING_SIZE);
	memset(commandstring, 0, STRING_SIZE);

	strncpy(sdd->description, "", STRING_SIZE - 1);
	strncpy(sdd->path, "", STRING_SIZE - 1);
	strncpy(sdd->modulename, "", STRING_SIZE - 1);
	strncpy(sdd->moduleoptions, "", STRING_SIZE - 1);
	
	/* Count 'em */
	while (strlen(sdds[c].modulename))
	{
		drivercount++;
		c++;
	}
	drivercount++;
	sections = malloc((drivercount + 1) * sizeof(char *));
	
	/* Copy 'em. */
	c = 0;
	while (strlen(sdds[c - 1].modulename))
	{
		sections[c] = sdds[c - 1].description;
		c++;
	}
	sections[c] = NULL;
	
	done = 0; choice = 1;
	while (!done)
	{
		rc = newtWinMenu(ctr[TR_SELECT_STORAGE_DRIVER],
			ctr[TR_SELECT_STORAGE_DRIVER_LONG], 50, 5, 5, 6,
			sections, &choice, ctr[TR_OK], ctr[TR_CANCEL]);
		if (rc == 0 || rc == 1)
		{
			/* Find module number, load module. */
			c = choice - 1;
			snprintf(message, BIG_STRING_SIZE - 1, ctr[TR_LOOKING_FOR_STORAGE_INTERFACE], sdds[c].description);
			snprintf(commandstring, STRING_SIZE - 1, "/sbin/modprobe %s", sdds[c].modulename);
			if (runcommandwithstatus(commandstring, message) == 0)
			{
				strncpy(sdd->description, sdds[c].description, STRING_SIZE - 1);
				strncpy(sdd->path, sdds[c].path, STRING_SIZE - 1);
				strncpy(sdd->modulename, sdds[c].modulename, STRING_SIZE - 1);
				strncpy(sdd->moduleoptions, "", STRING_SIZE - 1);
				done = 1;
			}
			else
				errorbox(ctr[TR_UNABLE_TO_LOAD_DRIVER_MODULE]);
		}
		else
			done = 1;	
	}

	return 1;
}
