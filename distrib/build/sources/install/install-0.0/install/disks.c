/* SmoothWall install program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: disks.c
 * Contains some functs for scanning /proc for ide info on CDROMS and
 * harddisks. */

#include "install.h"

extern FILE *flog;
extern char *logname;

extern char **ctr;

static int checkide(char letter);

/* checkide().  Scans the named drive letter and returns the DISK_??? type. */
static int checkide(char letter)
{
	FILE *f = NULL;
	char filename[STRING_SIZE];
	char buffer[STRING_SIZE];

	memset(filename, 0, STRING_SIZE);
	memset(buffer, 0, STRING_SIZE);
	
	snprintf(filename, STRING_SIZE - 1, "/proc/ide/hd%c/media", letter);
	
	if (!(f = fopen(filename, "r")))
		return DISK_EMPTY;
		
	if (!(fgets(buffer, STRING_SIZE, f)))
	{
		fclose(f);
		return DISK_EMPTY;
	}
		
	fclose(f);
	
	stripnl(buffer);
	
	if (strcmp(buffer, "cdrom") == 0)
		return DISK_CDROM;
	else if (strcmp(buffer, "disk") == 0)
		return DISK_HD;
	else
		return DISK_UNKNOWN;
}

/* findharddiskorcdrom().  Finds the first disk or CDROM, IDE, SCSI, etc. */
int findharddiskorcdrom(struct blockdevice *bd, int type)
{
	char letter = '\0';
	int fd = -1;
	int result = 0;
	
	bd->present = 0;

	/* Check IDE first. */
	for (letter = 'a'; letter <= 'z'; letter++)
	{
		if ((checkide(letter)) == type)
		{
			snprintf(bd->devnode, STRING_SIZE - 1, "/dev/hd%c", letter);
			bd->present = 1;
			result = 1;
		}
	}
	
	if (type == DISK_CDROM)
	{
		if ((fd = open("/dev/scd0", O_RDONLY|O_NONBLOCK)) != -1)
		{
			close(fd);
			strncpy(bd->devnode, "/dev/scd0", STRING_SIZE - 1);
			bd->present = 1;
			result = 1;
		}
	}
	else
	{	
		if ((fd = open("/dev/sda", O_RDONLY|O_NONBLOCK)) != -1)
		{
			close(fd);
			strncpy(bd->devnode, "/dev/sda", STRING_SIZE - 1);
			bd->present = 1;
			result = 1;
		}
	}
	
	return result;
}

int getdisksize(char *device)
{
	FILE *f = NULL;
	char buffer[STRING_SIZE];
	char *result = NULL;
	int count = 0;
	char *major = NULL, *minor = NULL, *blocks = NULL, *name = NULL;
	int size = 0;
	char *newdevice = NULL;
	
	if (!(newdevice = strstr(device, "/dev/")))
		return 0;
	newdevice += strlen("/dev/");
	
	memset(buffer, 0, STRING_SIZE - 1);
	
	if (!(f = fopen("/proc/partitions", "r")))
		return 0;
	
	fgets(buffer, STRING_SIZE, f);
	fgets(buffer, STRING_SIZE, f);	
	while (fgets(buffer, STRING_SIZE, f))
	{
		stripnl(buffer);		
		result = strtok(buffer, " ");
		count = 0;
		major = NULL; minor = NULL;
		blocks = NULL; name = NULL;
		while (result)
		{
			if (count == 0)
				major = result;
			else if (count == 1)
				minor = result;
			else if (count == 2)
				blocks = result;
			else
				name = result;
			count++;
			result = strtok(NULL, " ");
		}
		
		if (major && minor && blocks && name)
		{
			if (strcmp(name, newdevice) == 0)
			{
				size = atoi(blocks);
				break;
			}
		}
	}

	fclose(f);
		
	return size;
}
