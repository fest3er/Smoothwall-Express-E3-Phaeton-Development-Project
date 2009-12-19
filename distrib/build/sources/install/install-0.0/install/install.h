/* SmoothWall install program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: install.h
 * Main include file. */

#include <libsmooth.h>

#define DISK_UNKNOWN -1
#define DISK_EMPTY 0
#define DISK_HD 1
#define DISK_CDROM 2

/* CDROMS and harddisks. */
struct blockdevice
{
	int present;
	char devnode[STRING_SIZE];
	char fmtpartdevnode[STRING_SIZE];
};

struct storagedevicedriver
{
	char description[STRING_SIZE];
	char path[STRING_SIZE];
	char modulename[STRING_SIZE];
	char moduleoptions[STRING_SIZE];
};
                                
/* storagedevice.c */
int initstoragedevices(void);
int storagedevicedrvermenu(struct storagedevicedriver *sdd);
int probestoragedevicedriver(struct storagedevicedriver *sdd);

/* disk.c */
int findharddiskorcdrom(struct blockdevice *bd, int type);
int getdisksize(char *device);

/* cdrom.c */
int ejectcdrom(char *dev);

/* nic.c */
int networkmenu(struct keyvalue *ethernetkv);

/* net.c */
int downloadtarball(void);

/* config.c */
int writeconfigs(struct blockdevice *hd, struct keyvalue *ethernetkv, char *lang);
