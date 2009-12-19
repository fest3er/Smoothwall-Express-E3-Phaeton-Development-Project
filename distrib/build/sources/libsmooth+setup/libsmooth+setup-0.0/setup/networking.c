/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 * Modified by Daniel Hokka Zakrisson 2004
 *
 * filename: networking.c
 * The big one: networking. */
 
#include "setup.h"

#define DNS1 0
#define DNS2 1
#define DEFAULT_GATEWAY 2
#define DNSGATEWAY_TOTAL 3

#define CONFIG_TYPE_GREEN(c)	(1)
#define CONFIG_TYPE_ORANGE(c)	(c == 1 || c == 3 || c == 5 || c == 7)
#define CONFIG_TYPE_PURPLE(c)	(c == 4 || c == 5 || c == 6 || c == 7)
#define CONFIG_TYPE_RED(c)	(c == 2 || c == 3 || c == 6 || c == 7)

extern FILE *flog;
extern char *logname;

extern char **ctr;

extern int automode;

extern struct nic nics[];

char *configtypenames[] = {
/*0*/	"GREEN (RED is modem/ISDN)", 
/*1*/	"GREEN + ORANGE (RED is modem/ISDN)",
/*2*/	"GREEN + RED",
/*3*/	"GREEN + ORANGE + RED",
/*4*/	"GREEN + PURPLE (RED is modem/ISDN)",
/*5*/	"GREEN + PURPLE + ORANGE (RED is modem/ISDN)",
/*6*/	"GREEN + PURPLE + RED",
/*7*/	"GREEN + PURPLE + ORANGE + RED",
	NULL
};
int netaddresschange;

int oktoleave(char *errormessage);
int firstmenu(void);
int configtypemenu(void);
int drivermenu(void);
int changedrivers(void);
int greenaddressmenu(void);
int addressesmenu(void);
int dnsgatewaymenu(void);
void displaynicinfowithmac(char *dest, int size, char *colour, char *dev, char *description);

int handlenetworking(void)
{
	int done;
	int choice;
	char errormessage[STRING_SIZE];
	
	netaddresschange = 0;

	done = 0;
	while (!done)
	{
		choice = firstmenu();
			
		switch (choice)
		{
			case 1:
				configtypemenu();
				break;

			case 2:
				drivermenu();
				break;
							
			case 3:
				addressesmenu();
				break;
			
			case 4:
				dnsgatewaymenu();
				break;
				
			case 0:
				if (oktoleave(errormessage))
					done = 1;
				else
					errorbox(errormessage);
				break;
				
			default:
				break;
		}				
	}

	if (automode == 0)
	{
		/* Restart netowrking!  Reboot?  BAH! */	
		if (netaddresschange)
		{
			runcommandwithstatus("/etc/rc.d/rc.netaddress.down",
				ctr[TR_PUSHING_NETWORK_DOWN]);
			runcommandwithstatus("/etc/rc.d/rc.netaddress.up",
				ctr[TR_PULLING_NETWORK_UP]);
		}
	}
	
	return 1;
}

int oktoleave(char *errormessage)
{
	struct keyvalue *kv = initkeyvalues();
	char temp[STRING_SIZE];
	int configtype;
	
	if (!(readkeyvalues(kv, CONFIG_ROOT "ethernet/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}	

	strcpy(temp, "0");
	findkey(kv, "CONFIG_TYPE", temp); configtype = atol(temp);
	if (configtype < 0 || configtype > 7)
		configtype = 0;

	if (CONFIG_TYPE_GREEN(configtype))
	{
		strcpy(temp, "");
		findkey(kv, "GREEN_DEV", temp);
		if (!(strlen(temp)))
		{
			strcpy(errormessage, ctr[TR_NO_GREEN_INTERFACE]);
			goto EXIT;
		}
		if (!(interfacecheck(kv, "GREEN")))
		{
			strcpy(errormessage, ctr[TR_MISSING_GREEN_IP]);
			goto EXIT;
		}
	}
	if (CONFIG_TYPE_ORANGE(configtype))
	{
		strcpy(temp, "");
		findkey(kv, "ORANGE_DEV", temp);
		if (!(strlen(temp)))
		{
			strcpy(errormessage, ctr[TR_NO_ORANGE_INTERFACE]);
			goto EXIT;
		}
		if (!(interfacecheck(kv, "ORANGE")))
		{
			strcpy(errormessage, ctr[TR_MISSING_ORANGE_IP]);
			goto EXIT;
		}
	}
	if (CONFIG_TYPE_PURPLE(configtype))
	{
		strcpy(temp, "");
		findkey(kv, "PURPLE_DEV", temp);
		if (strlen(temp) == 0)
		{
			strcpy(errormessage, ctr[TR_NO_PURPLE_INTERFACE]);
			goto EXIT;
		}
		if (!interfacecheck(kv, "PURPLE"))
		{
			strcpy(errormessage, ctr[TR_MISSING_PURPLE_IP]);
			goto EXIT;
		}
	}
	if (CONFIG_TYPE_RED(configtype))
	{
		strcpy(temp, "");
		findkey(kv, "RED_DEV", temp);
		if (!(strlen(temp)))
		{
			strcpy(errormessage, ctr[TR_NO_RED_INTERFACE]);
			goto EXIT;
		}
		if (!(interfacecheck(kv, "RED")))
		{
			strcpy(errormessage, ctr[TR_MISSING_RED_IP]);
			goto EXIT;
		}
	}
	strcpy(errormessage, "");
EXIT:
	freekeyvalues(kv);
	
	if (strlen(errormessage))
		return 0;
	else
		return 1;
}

	
/* Shows the main menu and a summary of the current settings. */
int firstmenu(void)
{
	char *sections[] = { ctr[TR_NETWORK_CONFIGURATION_TYPE],
		ctr[TR_DRIVERS_AND_CARD_ASSIGNMENTS],
		ctr[TR_ADDRESS_SETTINGS],
		ctr[TR_DNS_AND_GATEWAY_SETTINGS], NULL };
	int rc;
	static int choice = 0;
	struct keyvalue *kv = initkeyvalues();
	char message[1000];
	char temp[STRING_SIZE];
	int x;
	int result;
	char networkrestart[STRING_SIZE] = "";
	
	if (!(readkeyvalues(kv, CONFIG_ROOT "ethernet/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}	

	if (netaddresschange) 
		strcpy(networkrestart, ctr[TR_RESTART_REQUIRED]);

	strcpy(temp, "");
	findkey(kv, "CONFIG_TYPE", temp);
	x = atol(temp);
	if (x < 0 || x > 7)
		x = 0;
	/* Format heading bit. */
	snprintf(message, 1000, ctr[TR_CURRENT_CONFIG], configtypenames[x],
		networkrestart);
	rc = newtWinMenu(ctr[TR_NETWORK_CONFIGURATION_MENU], message, 50, 5, 5, 6,
		sections, &choice, ctr[TR_OK], ctr[TR_DONE], NULL);

	if (rc == 0 || rc == 1)
		result = choice + 1;
	else
		result = 0;

	return result;
}

/* Here they choose general network config, number of nics etc. */
int configtypemenu(void)
{
	struct keyvalue *kv = initkeyvalues();
	char temp[STRING_SIZE] = "0";
	int choice;
	int rc;

	if (!(readkeyvalues(kv, CONFIG_ROOT "ethernet/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}
	
	findkey(kv, "CONFIG_TYPE", temp); choice = atol(temp);
	rc = newtWinMenu(ctr[TR_NETWORK_CONFIGURATION_TYPE],
		ctr[TR_NETWORK_CONFIGURATION_TYPE_LONG], 50, 5, 5, 6,
		configtypenames, &choice, ctr[TR_OK], ctr[TR_CANCEL], NULL);

	if (rc == 0 || rc == 1)
	{
		runcommandwithstatus("/etc/rc.d/rc.netaddress.down",
			ctr[TR_PUSHING_NETWORK_DOWN]);
	
		sprintf(temp, "%d", choice);
		replacekeyvalue(kv, "CONFIG_TYPE", temp);
		replacekeyvalue(kv, "GREEN_DEV", "");
		replacekeyvalue(kv, "ORANGE_DEV", "");
		replacekeyvalue(kv, "PURPLE_DEV", "");
		replacekeyvalue(kv, "RED_DEV", "");
		writekeyvalues(kv, CONFIG_ROOT "ethernet/settings");
		netaddresschange = 1;
	}
	
	freekeyvalues(kv);
	
	return 0;
}

/* Driver menu.  Choose drivers.. */
int drivermenu(void)
{
	struct keyvalue *kv = initkeyvalues();
	char message[1000];
	char temp[STRING_SIZE], temp1[STRING_SIZE];
	char driver[STRING_SIZE], dev[STRING_SIZE];
	int configtype;
	int rc;

	if (!(readkeyvalues(kv, CONFIG_ROOT "ethernet/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}

	strcpy(temp, "0"); findkey(kv, "CONFIG_TYPE", temp);
	configtype = atol(temp);

	strcpy(message, ctr[TR_CONFIGURE_NETWORK_DRIVERS]);
	
	/* This horrible bit formats the heading :( */
	strcpy(driver, "");
	findkey(kv, "GREEN_DISPLAYDRIVER", driver);
	findnicdescription(driver, temp);
	strcpy(dev, ctr[TR_UNSET]);
	findkey(kv, "GREEN_DEV", dev);
	displaynicinfowithmac(temp1, sizeof(temp1), "GREEN", dev, temp);
	strcat(message, temp1);

	if (CONFIG_TYPE_ORANGE(configtype))
	{
		strcpy(driver, ""); findkey(kv, "ORANGE_DISPLAYDRIVER", driver);
		findnicdescription(driver, temp);
		strcpy(dev, ctr[TR_UNSET]); findkey(kv, "ORANGE_DEV", dev);
		displaynicinfowithmac(temp1, sizeof(temp1), "ORANGE", dev, temp);
		strcat(message, temp1);
	}
	if (CONFIG_TYPE_PURPLE(configtype))
	{
		strcpy(driver, "");
		findkey(kv, "PURPLE_DISPLAYDRIVER", driver);
		findnicdescription(driver, temp);
		strcpy(dev, ctr[TR_UNSET]);
		findkey(kv, "PURPLE_DEV", dev);
		displaynicinfowithmac(temp1, sizeof(temp1), "PURPLE", dev, temp);
		strcat(message, temp1);
	}
	if (CONFIG_TYPE_RED(configtype))
	{
		strcpy(driver, ""); findkey(kv, "RED_DISPLAYDRIVER", driver);
		findnicdescription(driver, temp);
		strcpy(dev, ctr[TR_UNSET]); findkey(kv, "RED_DEV", dev);
		displaynicinfowithmac(temp1, sizeof(temp1), "RED", dev, temp);
		strcat(message, temp1);
	}
	strcat(message, ctr[TR_DO_YOU_WISH_TO_CHANGE_THESE_SETTINGS]);
	rc = newtWinChoice(ctr[TR_DRIVERS_AND_CARD_ASSIGNMENTS], ctr[TR_OK],
		ctr[TR_CANCEL], message);
	if (rc == 0 || rc == 1)
	{
		/* Shit, got to do something.. */
		changedrivers();
	}
	
	freekeyvalues(kv);

	return 1;
}

int changedrivers(void)
{
	struct keyvalue *kv = initkeyvalues();
	char message[1000];
	char temp[STRING_SIZE];
	int configtype;
	int rc;
	int c;
	int needcards, sofarallocated, countofcards, toallocate;
	char *green = "GREEN";
	char *orange = "ORANGE";
	char *purple = "PURPLE";
	char *red = "RED";
	char *sections[3];
	int choice;
	char nexteth[STRING_SIZE];
	int abort;
	char currentdriver[STRING_SIZE], currentdriveroptions[STRING_SIZE];
	char displaydriver[STRING_SIZE];
	char cardinfo[STRING_SIZE];
	char mac[STRING_SIZE];
	int driverc = 0;
	
	if (!(readkeyvalues(kv, CONFIG_ROOT "ethernet/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}
	
	strcpy(temp, "0"); findkey(kv, "CONFIG_TYPE", temp);
	configtype = atol(temp);
	
	runcommandwithstatus("/etc/rc.d/rc.netaddress.down",
		ctr[TR_PUSHING_NETWORK_DOWN]);
	
	/* Remove all modules not needed for green networking. */
	c = 0;
	while (nics[c].modulename)
	{
		if (checkformodule(nics[c].modulename))
		{
			sprintf(temp, "/sbin/rmmod %s", nics[c].modulename);
			mysystem(temp);
		}
		c++;
	}
	
	/* Blank them so the rc.netaddress.up dosnt get confused. */
	replacekeyvalue(kv, "GREEN_DEV", "");
	replacekeyvalue(kv, "ORANGE_DEV", "");
	replacekeyvalue(kv, "PURPLE_DEV", "");
	replacekeyvalue(kv, "RED_DEV", "");
	
	if (configtype == 0)
		needcards = 1;
	else if (configtype == 1 || configtype == 2 || configtype == 4)
		needcards = 2;
	else if (configtype == 3 || configtype == 5 || configtype == 6)
		needcards = 3;
	else
		needcards = 4;

	/* This is the green card. */		
	sofarallocated = 0;

	strcpy(displaydriver, "");
	strcpy(currentdriver, "");
		
	abort = 0;
	driverc = 0;
	/* Keep going till all cards are got, or they give up. */
	while (sofarallocated < needcards && !abort)
	{
		countofcards = countcards();
		/* This is how many cards were added by the last module. */
		toallocate = countofcards - sofarallocated;
		while (toallocate > 0 && sofarallocated < needcards)
		{
			findnicdescription(displaydriver, temp);
			/* Get device name, eth%d is hard coded. */
			sprintf(nexteth, "eth%d", sofarallocated);
			/* Get MAC address. */
			if (getnicmac(mac, STRING_SIZE, nexteth))
				/* If MAC found put at the end of NIC description. */
				snprintf(cardinfo, STRING_SIZE, "%s [%s]", temp, mac);
			else
				/* MAC lookup failed so just display NIC description. */
				snprintf(cardinfo, STRING_SIZE, "%s", temp);
			sprintf(message, ctr[TR_UNCLAIMED_DRIVER], cardinfo);
			c = 0; choice = 0;
			strcpy(temp, ""); findkey(kv, "GREEN_DEV", temp);
			if (CONFIG_TYPE_GREEN(configtype) &&!strlen(temp))
				sections[c++] = green;
			strcpy(temp, ""); findkey(kv, "ORANGE_DEV", temp);
			if (CONFIG_TYPE_ORANGE(configtype) && !strlen(temp))
				sections[c++] = orange;
			strcpy(temp, ""); findkey(kv, "PURPLE_DEV", temp);
			if (CONFIG_TYPE_PURPLE(configtype) && !strlen(temp))
				sections[c++] = purple;
			strcpy(temp, ""); findkey(kv, "RED_DEV", temp);			
			if (CONFIG_TYPE_RED(configtype) && !strlen(temp))
				sections[c++] = red;
			sections[c] = NULL;
			rc = newtWinMenu(ctr[TR_CARD_ASSIGNMENT],
				message, 50, 5,	5, 6, sections, &choice, ctr[TR_OK],
				ctr[TR_CANCEL], NULL);	
			if (rc == 0 || rc == 1)
			{
				/* Now we see which iface needs its settings changed. */
				sprintf(nexteth, "eth%d", sofarallocated);
				if (strcmp(sections[choice], green) == 0)
				{
					replacekeyvalue(kv, "GREEN_DEV", nexteth);
					replacekeyvalue(kv, "GREEN_DRIVER", currentdriver);
					replacekeyvalue(kv, "GREEN_DRIVER_OPTIONS", currentdriveroptions);
					replacekeyvalue(kv, "GREEN_DISPLAYDRIVER", displaydriver);
					sofarallocated++;
					toallocate--;
					strcpy(currentdriver, "");
					strcpy(currentdriveroptions, "");
				}
				if (strcmp(sections[choice], orange) == 0)
				{
					replacekeyvalue(kv, "ORANGE_DEV", nexteth);
					replacekeyvalue(kv, "ORANGE_DRIVER", currentdriver);
					replacekeyvalue(kv, "ORANGE_DRIVER_OPTIONS", currentdriveroptions);
					replacekeyvalue(kv, "ORANGE_DISPLAYDRIVER", displaydriver);
					sofarallocated++;
					toallocate--;
					strcpy(currentdriver, "");
					strcpy(currentdriveroptions, "");
				}
				if (strcmp(sections[choice], purple) == 0)
				{
					replacekeyvalue(kv, "PURPLE_DEV", nexteth);
					replacekeyvalue(kv, "PURPLE_DRIVER", currentdriver);
					replacekeyvalue(kv, "PURPLE_DRIVER_OPTIONS", currentdriveroptions);
					replacekeyvalue(kv, "PURPLE_DISPLAYDRIVER", displaydriver);
					sofarallocated++;
					toallocate--;
					strcpy(currentdriver, "");
					strcpy(currentdriveroptions, "");
				}
				if (strcmp(sections[choice], red) == 0)
				{
					replacekeyvalue(kv, "RED_DEV", nexteth);
					replacekeyvalue(kv, "RED_DRIVER", currentdriver);
					replacekeyvalue(kv, "RED_DRIVER_OPTIONS", currentdriveroptions);
					replacekeyvalue(kv, "RED_DISPLAYDRIVER", displaydriver);
					sofarallocated++;
					toallocate--;
					strcpy(currentdriver, "");
					strcpy(currentdriveroptions, "");
				}
			}
			else
			{
				break;
			}
		}
		
		/* Need another module!  The nitty gritty code is in libsmooth. */
		if (sofarallocated < needcards)
		{
			rc = newtWinTernary(ctr[TR_CARD_ASSIGNMENT], ctr[TR_PROBE], 
				ctr[TR_SELECT], ctr[TR_CANCEL], ctr[TR_NO_UNALLOCATED_CARDS]);
				
			if (rc == 0 || rc == 1)
			{
				probecards(currentdriver, currentdriveroptions, &driverc);
				if (!strlen(currentdriver))
				{
					errorbox(ctr[TR_PROBE_FAILED]);
					driverc = 0;
				}
			}				
			else if (rc == 2)
				choosecards(currentdriver, currentdriveroptions);
			else
				abort = 1;
				
			strcpy(displaydriver, currentdriver);
		}
	}
	
	countofcards = countcards();
	if (countofcards >= needcards)
	{
		newtWinMessage(ctr[TR_CARD_ASSIGNMENT], ctr[TR_OK],
			ctr[TR_ALL_CARDS_SUCCESSFULLY_ALLOCATED]);
	}
	else
		errorbox(ctr[TR_NOT_ENOUGH_CARDS_WERE_ALLOCATED]);
		
	writekeyvalues(kv, CONFIG_ROOT "ethernet/settings");

	freekeyvalues(kv);

	netaddresschange = 1;
	
	return 1;
}

/* Let user change GREEN address. */
int greenaddressmenu(void)
{
	struct keyvalue *kv = initkeyvalues();
	int rc;
	
	if (!(readkeyvalues(kv, CONFIG_ROOT "ethernet/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}

	rc = newtWinChoice(ctr[TR_WARNING], ctr[TR_OK], ctr[TR_CANCEL],
		ctr[TR_WARNING_LONG]);
	
	if (rc == 0 || rc == 1)
	{
		if (changeaddress(kv, "GREEN", 0, ""))
		{
			netaddresschange = 1;
			writekeyvalues(kv, CONFIG_ROOT "ethernet/settings");			
			writehostsfiles();			
		}
	}
	
	freekeyvalues(kv);

	return 0;
}

/* They can change ORANGE and GREEN too :) */
int addressesmenu(void)
{
	struct keyvalue *kv = initkeyvalues();
	struct keyvalue *mainkv = initkeyvalues();
	int rc = 0;
	char *sections[4];
	char *green = "GREEN";
	char *orange = "ORANGE";
	char *purple = "PURPLE";
	char *red = "RED";
	int c = 0;
	char temp[STRING_SIZE];
	int configtype;
	int done;
	int choice;
	char hostname[STRING_SIZE];
	
	if (!(readkeyvalues(kv, CONFIG_ROOT "ethernet/settings")))
	{
		freekeyvalues(kv);
		freekeyvalues(mainkv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}
	if (!(readkeyvalues(mainkv, CONFIG_ROOT "main/settings")))
	{
		freekeyvalues(kv);
		freekeyvalues(mainkv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}

	strcpy(temp, "0"); findkey(kv, "CONFIG_TYPE", temp);
	configtype = atol(temp);
	
	sections[c] = green;
	c++;
	if (CONFIG_TYPE_ORANGE(configtype))
	{
		sections[c] = orange;
		c++;
	}
	if (CONFIG_TYPE_PURPLE(configtype))
	{
		sections[c] = purple;
		c++;
	}
	if (CONFIG_TYPE_RED(configtype))
	{
		sections[c] = red;
		c++;
	}
	sections[c] = NULL;

	choice = 0;	
	done = 0;	
	while (!done)
	{
		rc = newtWinMenu(ctr[TR_ADDRESS_SETTINGS],
			ctr[TR_SELECT_THE_INTERFACE_YOU_WISH_TO_RECONFIGURE], 50, 5,
			5, 6, sections, &choice, ctr[TR_OK], ctr[TR_DONE], NULL);	

		if (rc == 0 || rc == 1)
		{
			if (strcmp(sections[choice], "GREEN") == 0)
			{
				rc = newtWinChoice(ctr[TR_WARNING], ctr[TR_OK], ctr[TR_CANCEL],
					ctr[TR_WARNING_LONG]);
				if (rc == 0 || rc == 1)
				{
					if (changeaddress(kv, "GREEN", 0, ""))
					{
						netaddresschange = 1;
						writekeyvalues(kv, CONFIG_ROOT "ethernet/settings");			
						writehostsfiles();			
					}
				}
			}
			if (strcmp(sections[choice], "ORANGE") == 0)
			{
				if (changeaddress(kv, "ORANGE", 0, ""))
					netaddresschange = 1;
			}
			if (strcmp(sections[choice], "PURPLE") == 0)
			{
				if (changeaddress(kv, "PURPLE", 0, ""))
					netaddresschange = 1;
			}
			if (strcmp(sections[choice], "RED") == 0)
			{
				strcpy(hostname, "");
				findkey(mainkv, "HOSTNAME", hostname);
				if (changeaddress(kv, "RED", 1, hostname))
					netaddresschange = 1;
			}
		}
		else
			done = 1;
	}
	
	writekeyvalues(kv, CONFIG_ROOT "ethernet/settings");
	freekeyvalues(kv);
	freekeyvalues(mainkv);
	
	return 0;
}

/* DNS and default gateway.... */
int dnsgatewaymenu(void)
{
	struct keyvalue *kv = initkeyvalues();
	char message[1000];
	char temp[STRING_SIZE] = "0";
	struct newtWinEntry entries[DNSGATEWAY_TOTAL];
	char *values[DNSGATEWAY_TOTAL];         /* pointers for the values. */
	int error;
	int configtype;
	int rc;

	if (!(readkeyvalues(kv, CONFIG_ROOT "ethernet/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}

	strcpy(temp, "0"); findkey(kv, "CONFIG_TYPE", temp);
	configtype = atol(temp);
	
	if (!CONFIG_TYPE_RED(configtype))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_DNS_GATEWAY_WITH_GREEN]);
		return 0;
	}

	entries[DNS1].text = ctr[TR_PRIMARY_DNS];
	strcpy(temp, ""); findkey(kv, "DNS1", temp);
	values[DNS1] = strdup(temp);
	entries[DNS1].value = (const char **) &values[DNS1];
	entries[DNS1].flags = 0;
	
	entries[DNS2].text = ctr[TR_SECONDARY_DNS];
	strcpy(temp, ""); findkey(kv, "DNS2", temp);
	values[DNS2] = strdup(temp);
	entries[DNS2].value = (const char **) &values[DNS2];
	entries[DNS2].flags = 0;
	
	entries[DEFAULT_GATEWAY].text = ctr[TR_DEFAULT_GATEWAY];
	strcpy(temp, ""); findkey(kv, "DEFAULT_GATEWAY", temp);
	values[DEFAULT_GATEWAY] = strdup(temp);
	entries[DEFAULT_GATEWAY].value = (const char **)  &values[DEFAULT_GATEWAY];
	entries[DEFAULT_GATEWAY].flags = 0;
	
	entries[DNSGATEWAY_TOTAL].text = NULL;
	entries[DNSGATEWAY_TOTAL].value = NULL;
	entries[DNSGATEWAY_TOTAL].flags = 0;
	
	do
	{
		error = 0;
		
		rc = newtWinEntries(ctr[TR_DNS_AND_GATEWAY_SETTINGS], 
			ctr[TR_DNS_AND_GATEWAY_SETTINGS_LONG], 50, 5, 5, 18, entries,
			ctr[TR_OK], ctr[TR_CANCEL], NULL);
		if (rc == 0 || rc == 1)
		{
			strcpy(message, ctr[TR_INVALID_FIELDS]);
			if (strlen(values[DNS1]))
			{
				if (inet_addr(values[DNS1]) == INADDR_NONE)
				{
					strcat(message, ctr[TR_PRIMARY_DNS_CR]);
					error = 1;
				}
			}
			if (strlen(values[DNS2]))
			{
				if (inet_addr(values[DNS2]) == INADDR_NONE)
				{
					strcat(message, ctr[TR_SECONDARY_DNS_CR]);
					error = 1;
				}
			}
			if (strlen(values[DEFAULT_GATEWAY]))
			{
				if (inet_addr(values[DEFAULT_GATEWAY]) == INADDR_NONE)
				{
					strcat(message, ctr[TR_DEFAULT_GATEWAY_CR]);
					error = 1;
				}
			}
			if (!strlen(values[DNS1]) && strlen(values[DNS2]))
			{
				strcpy(message, ctr[TR_SECONDARY_WITHOUT_PRIMARY_DNS]);
				error = 1;
			}

			if (error)
				errorbox(message);
			else
			{
				replacekeyvalue(kv, "DNS1", values[DNS1]);
				replacekeyvalue(kv, "DNS2", values[DNS2]);
				replacekeyvalue(kv, "DEFAULT_GATEWAY", values[DEFAULT_GATEWAY]);
				netaddresschange = 1;
				free((char *) values[DNS1]);
				free((char *) values[DNS2]);
				free((char *) values[DEFAULT_GATEWAY]);
				writekeyvalues(kv, CONFIG_ROOT "ethernet/settings");
			}
		}
	}
	while (error);
	
	freekeyvalues(kv);
	
	return 1;
}			

/* Display the NIC device with MAC address. */
void displaynicinfowithmac(char *dest, int size, char *colour, char *dev,
	char *description)
{
	char mac[STRING_SIZE];
	int found = 0;

	if (!strlen(dev))
	    strcpy(dev, ctr[TR_UNSET]);
	else if (getnicmac(mac, sizeof(mac), dev))
	{
		snprintf(dest, size, "%s: %s (%s) [%s]\n", colour,
			description, dev, mac);
		found = 1;
	}

	if (!found)
		snprintf(dest, size, "%s: %s (%s)\n", colour, description, dev);

}
