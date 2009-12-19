/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2003
 *
 * filename: restore.c
 * Stuff for restoring from a backup floppy disk. */
 
#include "setup.h"
 
extern FILE *flog;
extern char *logname;

extern char **ctr;

extern int automode;
extern int rebootrequired;
extern int performedrestore;

int handlerestore(void)
{
	int result = 0;

	if (newtWinChoice(TITLE, ctr[TR_OK], ctr[TR_CANCEL], ctr[TR_PLEASE_INSERT_THE_RESTORE_FLOPPY_DISK]) == 2)
                return 0;

	if (runcommandwithstatus("/usr/bin/tar -xf /dev/fd0 -C " CONFIG_ROOT "/restore",
		ctr[TR_EXAMINING_BACKUP_DISK]))
	{
		errorbox(ctr[TR_UNABLE_TO_EXAMINE_BACKUP_DISK]);
		goto ERROR;
	}

	if (newtWinChoice(TITLE, ctr[TR_OK], ctr[TR_CANCEL], ctr[TR_DO_YOU_WISH_TO_RESTORE]) == 2)
                return 0;

	if (runcommandwithstatus("/etc/rc.d/restorescript", ctr[TR_RESTORING]))
	{
		errorbox(ctr[TR_UNABLE_TO_RESTORE]);
		goto ERROR;
	}

	result = 1;
	performedrestore = 1;

	if (!automode)
		rebootrequired = 1;                                                                

	newtWinMessage(TITLE, ctr[TR_OK], ctr[TR_CONFIGURATION_SUCCESSFULLY_RESTORED]);

ERROR:
	return result;
}	
