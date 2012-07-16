/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: dhcp.c
 * Stuff for setting up the DHCP server from the setup prog. */
 
#include "setup.h"

#define TOP 4

#define START_ADDRESS 0
#define END_ADDRESS 1
#define PRIMARY_DNS 2
#define SECONDARY_DNS 3
#define DEFAULT_LEASE_TIME 4
#define MAX_LEASE_TIME 5
#define DOMAIN_NAME_SUFFIX 6
#define MAX_BOXES 7

extern FILE *flog;
extern char *logname;

extern char **ctr;

extern int automode;

newtComponent dhcpform;
newtComponent entries[MAX_BOXES];
newtComponent enabledcheckbox;

void dhcpdialogcallbackdhcp(newtComponent cm, void *data);

int handledhcp(void)
{
	char *results[MAX_BOXES];
	char enabledresult;
	char startenabled;
	struct newtExitStruct es;
	newtComponent header;
	newtComponent labels[MAX_BOXES];
	newtComponent ok, cancel;	
	char message[1000];
	char *labeltexts[MAX_BOXES] = { ctr[TR_START_ADDRESS], ctr[TR_END_ADDRESS],
		ctr[TR_PRIMARY_DNS], ctr[TR_SECONDARY_DNS], ctr[TR_DEFAULT_LEASE],
		ctr[TR_MAX_LEASE], ctr[TR_DOMAIN_NAME_SUFFIX] };
	char *varnames[MAX_BOXES] = { "START_ADDR", "END_ADDR", "DNS1", "DNS2",
		"DEFAULT_LEASE_TIME", "MAX_LEASE_TIME", "DOMAIN_NAME"};
	char defaults[MAX_BOXES][STRING_SIZE]; 
	int result;
	int c;
	char temp[STRING_SIZE];
	struct keyvalue *dhcpkv = initkeyvalues();
	struct keyvalue *ethernetkv = initkeyvalues();
	int error;
	FILE *file;
	char greenaddress[STRING_SIZE];	
	char greennetaddress[STRING_SIZE];
	char greennetmask[STRING_SIZE];
        int intnetaddress = 0;
	int intextra = 0;
	int intfinal = 0;
	char *final;
	struct in_addr addr;
	char greendev[STRING_SIZE];
	
 	memset(defaults, 0, sizeof(char) * STRING_SIZE * MAX_BOXES);
 	memset(greendev, 0, STRING_SIZE);
	
	if (!(readkeyvalues(dhcpkv, CONFIG_ROOT "dhcp/settings-green")))
	{
		freekeyvalues(dhcpkv);
		freekeyvalues(ethernetkv);		
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}
	if (!(readkeyvalues(ethernetkv, CONFIG_ROOT "ethernet/settings")))
	{
		freekeyvalues(dhcpkv);	
		freekeyvalues(ethernetkv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}

	/* Get the green dev. */
	findkey(ethernetkv, "GREEN_DEV", greendev);

	/* Set default values. */	
	findkey(ethernetkv, "GREEN_NETADDRESS", temp);
	intnetaddress = inet_addr(temp);
	intextra = inet_addr("0.0.0.100");
	intfinal = intnetaddress | intextra;
	addr.s_addr = intfinal; 
	final = inet_ntoa(addr);
	strncpy(defaults[START_ADDRESS], final, STRING_SIZE - 1);

	findkey(ethernetkv, "GREEN_NETADDRESS", temp);
	intnetaddress = inet_addr(temp);
	intextra = inet_addr("0.0.0.200");
	intfinal = intnetaddress | intextra;
	addr.s_addr = intfinal; 
	final = inet_ntoa(addr);
	strncpy(defaults[END_ADDRESS], final, STRING_SIZE - 1);

	findkey(ethernetkv, "GREEN_ADDRESS", defaults[PRIMARY_DNS]);
	strcpy(defaults[DEFAULT_LEASE_TIME], "60");
	strcpy(defaults[MAX_LEASE_TIME], "120");
	
	sprintf(message, ctr[TR_DHCP_SERVER_CONFIGURATION]);
	newtCenteredWindow(55, 18, message);

	dhcpform = newtForm(NULL, NULL, 0);
	
	sprintf(message, ctr[TR_CONFIGURE_DHCP]);
	header = newtTextboxReflowed(1, 1, message, 52, 0, 0, 0);
	newtFormAddComponent(dhcpform, header);

	strcpy(temp, ""); findkey(dhcpkv, "ENABLE", temp);
	if (strcmp(temp, "on") == 0)
		startenabled = '*';
	else
		startenabled = ' ';
	enabledcheckbox = newtCheckbox(2, TOP + 0, ctr[TR_ENABLED], startenabled, " *", &enabledresult);
	newtFormAddComponent(dhcpform, enabledcheckbox);
	newtComponentAddCallback(enabledcheckbox, dhcpdialogcallbackdhcp, NULL);		

	for (c = 0; c < MAX_BOXES; c++)
	{
		labels[c] = newtTextbox(2, TOP + 2 + c, 33, 1, 0);
		newtTextboxSetText(labels[c], labeltexts[c]);
		newtFormAddComponent(dhcpform, labels[c]);				
		strcpy(temp, defaults[c]); findkey(dhcpkv, varnames[c], temp);
		entries[c] = newtEntry(34, TOP + 2 + c, temp, 18, (const char **) &results[c], 0);
		newtFormAddComponent(dhcpform, entries[c]);		
		if (startenabled == ' ')
			newtEntrySetFlags(entries[c], NEWT_FLAG_DISABLED, NEWT_FLAGS_SET);			
		
	}
	
	ok = newtButton(10, c + 7, ctr[TR_OK]);
	cancel = newtButton(34, c + 7, ctr[TR_CANCEL]);

	newtFormAddComponents(dhcpform, ok, cancel, NULL);
	
	do
	{
		error = 0;
		newtFormRun(dhcpform, &es);
	
		if (es.u.co == ok)
		{
			/* OK was pressed; verify the contents of each entry. */		
			if (enabledresult == '*')
			{
				strcpy(message, ctr[TR_INVALID_FIELDS]);			
				if (inet_addr(results[START_ADDRESS]) == INADDR_NONE)
				{
					strcat(message, ctr[TR_START_ADDRESS_CR]);
					error = 1;
				}
				if (inet_addr(results[END_ADDRESS]) == INADDR_NONE)
				{
					strcat(message, ctr[TR_END_ADDRESS_CR]);
					error = 1;
				}
				if (strlen(results[SECONDARY_DNS]))
				{
					if (inet_addr(results[PRIMARY_DNS]) == INADDR_NONE)
					{
						strcat(message, ctr[TR_PRIMARY_DNS_CR]);
						error = 1;
					}
				}
				if (strlen(results[SECONDARY_DNS]))
				{
					if (inet_addr(results[SECONDARY_DNS]) == INADDR_NONE)
					{
						strcat(message, ctr[TR_SECONDARY_DNS_CR]);
						error = 1;
					}
				}
				if (!(atol(results[DEFAULT_LEASE_TIME])))
				{
					strcat(message, ctr[TR_DEFAULT_LEASE_CR]);
					error = 1;
				}
				if (!(atol(results[MAX_LEASE_TIME])))
				{
					strcat(message, ctr[TR_MAX_LEASE_CR]);
					error = 1;
				}
			}				
			
			if (error)
				errorbox(message);
			else
			{
				for (c = 0; c < MAX_BOXES; c++)
					replacekeyvalue(dhcpkv, varnames[c], results[c]);
				if (enabledresult == '*')
				{
					replacekeyvalue(dhcpkv, "ENABLE", "on");
					fclose(fopen(CONFIG_ROOT "dhcp/enable", "w"));
					chown(CONFIG_ROOT "dhcp/enable", 99, 99);
				}
				else
				{
					replacekeyvalue(dhcpkv, "ENABLE", "off");
					unlink(CONFIG_ROOT "dhcp/enable");
				}
                                replacekeyvalue(dhcpkv, "VALID", "yes");
                                writekeyvalues(dhcpkv, CONFIG_ROOT "dhcp/settings-green");
                                
                                mysystem("/usr/bin/smoothwall/writedhcp.pl");

                                file = fopen(CONFIG_ROOT "dhcp/green", "w");
                                fprintf(file, "%s", greendev);
				fclose(file);
				
				if (automode == 0)
					mysystem("/usr/bin/smoothcom dhcpdrestart");
			}
			result = 1;
		}
		else
			result = 0;
	}		
	while (error);
	
	newtFormDestroy(dhcpform);
	newtPopWindow();
	
	freekeyvalues(dhcpkv);
	freekeyvalues(ethernetkv);
	
	return result;
}

/* Called when enabled flag is toggled.  Toggle disabled state of other 3
 * controls. */
void dhcpdialogcallbackdhcp(newtComponent cm, void *data)
{
	int c;
	
	for (c = 0; c < MAX_BOXES; c++)
		newtEntrySetFlags(entries[c], NEWT_FLAG_DISABLED, NEWT_FLAGS_TOGGLE);
		
	newtRefresh();
	newtDrawForm(dhcpform);	
}
