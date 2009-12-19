/* SmoothWall install program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: net.c
 * Stuff for downloading the smoothwall tarball using wget. */
 
#include "install.h"

extern FILE *flog;
extern char *logname;

extern char **ctr;

static int gettarballurl(char *url);

int downloadtarball(void)
{
	char url[STRING_SIZE];
	int done;
	int rc;
	char commandstring[STRING_SIZE];
	
	done = 0;
	while (!done)
	{
		rc = gettarballurl(url);
		if (rc != 1)
			return 0;
		
		snprintf(commandstring, STRING_SIZE, "/bin/wget -P /harddisk %s", url);
		if (!(runcommandwithstatus(commandstring, ctr[TR_DOWNLOADING])))
			done = 1;
		else
			errorbox(ctr[TR_FAILED_TO_DOWNLOAD]);
	}
	
	return 1;
}	
		
static int gettarballurl(char *url)
{
	const char *values[] = {	NULL, NULL };	/* pointers for the values. */
	struct newtWinEntry entries[] =
		{ { "", &values[0], 0,}, { NULL, NULL, 0 } };
	int rc;
	
	rc = newtWinEntries(TITLE, ctr[TR_ENTER_URL],
		60, 5, 5, 50, entries, ctr[TR_OK], ctr[TR_CANCEL], NULL);
		
	strncpy(url, values[0], STRING_SIZE);
	
	free((char *) values[0]);
	
	return rc;
}
