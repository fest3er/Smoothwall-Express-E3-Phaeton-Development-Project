/* SmoothWall install program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 * 
 * filename: nic.c
 * Contains stuff related to firing up the network card, including a crude
 * autodector. */

#include "install.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern FILE *flog;
extern char *logname;

extern char **ctr;

extern struct nic nics[];

int networkmenu(struct keyvalue *ethernetkv)
{
	int rc;
	char driver[STRING_SIZE] = "";
	char driveroptions[STRING_SIZE] = "";
	struct keyvalue *kv = initkeyvalues();
	int result = 0;
	char commandstring[STRING_SIZE];
	char address[STRING_SIZE], netmask[STRING_SIZE];
	int done;
	char description[1000];
	char message[1000];
	char cardinfo[STRING_SIZE];
	char mac[STRING_SIZE];
	int c = 0;

	done = 0;
	c = 0;
	while (!done)
	{
		rc = newtWinTernary(ctr[TR_CONFIGURE_NETWORKING], ctr[TR_PROBE], 
			ctr[TR_SELECT], ctr[TR_CANCEL], ctr[TR_CONFIGURE_NETWORKING_LONG]);
		
		if (rc == 0 || rc == 1)
		{
			probecards(driver, driveroptions, &c);
			if (!strlen(driver))
				errorbox(ctr[TR_PROBE_FAILED]);
			else
			{
				findnicdescription(driver, description);
				if ((getnicmac(mac, sizeof(mac), "eth0")))
					/* If MAC found put it at the end of nic description. */
					snprintf(cardinfo, STRING_SIZE, "%s [%s]", description, mac);
				else
					/* MAC lookup failed so just display nic description. */
					snprintf(cardinfo, STRING_SIZE, "%s", description);

				sprintf(message, ctr[TR_FOUND_NIC], cardinfo);
				newtWinMessage(TITLE, ctr[TR_OK], message);
			}		
		}			
		else if (rc == 2)
			choosecards(driver, driveroptions);
		else
			done = 1;	
			
		if (strlen(driver))
			done = 1;
	}
	
	if (!strlen(driver))
		goto EXIT;

	/* Default is a GREEN nic only. */
	/* Smoothie is not untarred yet, so we have to delay actually writing the
	 * settings till later. */
	replacekeyvalue(ethernetkv, "CONFIG_TYPE", "0");
	replacekeyvalue(ethernetkv, "GREEN_DRIVER", driver);
	replacekeyvalue(ethernetkv, "GREEN_DRIVER_OPTIONS", driveroptions);
	replacekeyvalue(ethernetkv, "GREEN_DEV", "eth0");
	replacekeyvalue(ethernetkv, "GREEN_DISPLAYDRIVER", driver);
	
	if (!(changeaddress(ethernetkv, "GREEN", 0, "")))
		goto EXIT;
	
	strcpy(address, ""); findkey(ethernetkv, "GREEN_ADDRESS", address);
	strcpy(netmask, ""); findkey(ethernetkv, "GREEN_NETMASK", netmask);

	snprintf(commandstring, STRING_SIZE, "/bin/ifconfig eth0 %s netmask %s up", 
		address, netmask);
	if (mysystem(commandstring))
	{
		errorbox(ctr[TR_INTERFACE_FAILED_TO_COME_UP]);
		goto EXIT;
	}

	result = 1;
	
EXIT:
	freekeyvalues(kv);
	
	return result;
}
			
