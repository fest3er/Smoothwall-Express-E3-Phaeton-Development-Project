/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: adsl.c
 * ADSL things. */
 
#include "setup.h"
 
extern FILE *flog;
extern char *logname;

extern char **ctr;

extern int automode;
extern int usbfail;

struct device
{
	char *name;
	char *device;
	char *ecitype;
	char *ifacetype;
};

struct device devices[] = {
	{ "Alcatel SpeedTouch USB", "ALCATEL", "", "usb" },
	{ "ECI (generic) USB", "ECI", "generic", "usb" },
	{ "Fujitsu FDX310 USB", "ECI", "fdx310", "usb" },
	{ "Zoom 5510 ADSL USB", "ECI", "5510", "usb" },
	{ "US Robotics 8500 USB", "ECI", "8500", "usb" },
	{ "Asus AAM6000UG USB", "ECI", "aam6000ug", "usb" },
	{ "Askey USB", "ECI", "askey", "usb" },
	{ "Eicon Diva USB", "ECI", "diva", "usb" },
	{ "Aztech DSL100U USB", "ECI", "dsl100u", "usb" },
	{ "GVC BB039 USB", "ECI", "gvcbb039", "usb" },
	{ "ECI HiFocus and B-Focus USB", "ECI", "hifocus", "usb" },
	{ "Ericsson hm120dp USB", "ECI", "hm120dp", "usb" },
	{ "Digicom MichelAngelo USB", "ECI", "michelangelo", "usb" },
	{ "Zyxel Prestige 630-41 USB", "ECI", "prestige63041", "usb" },
	{ "Siemens Santis USB", "ECI", "santis", "usb" },
	{ "Solwise SAR100/EA100 USB", "ECI", "sar100", "usb" },
	{ "Archtek UGW-8000 USB", "ECI", "ugw8000", "usb" } ,
	{ "BT Voyager USB", "ECI", "voyager", "usb" },
	{ "Ipmdatacom Webpower USB", "ECI", "webpower", "usb" },
	{ "Topcom Webr@cer 850 USB", "ECI", "webracer850", "usb" },
	{ "Wisecom ws-ad80usg USB", "ECI", "wsad80usg", "usb" },
	{ "Xentrix USB", "ECI", "xentrix", "usb" },
	{ "Topcom Xplorer 850 USB", "ECI", "xplorer850", "usb" },
	{ "Aethra Starmodem", "ECI", "starmodem", "usb" },
	{ "Atlantis I-Storm", "ECI", "istorm", "usb" },
	{ "Cypress Globespan G7000", "ECI", "globespan", "usb"},
	{ "Dynalink ALE070", "ECI", "dynalink", "usb" },
	{ "GreatSpeed GS-U210G Neo", "ECI", "neo", "usb" },
	{ "Cellpipe 20A USB", "ECI", "cellpipe", "usb" },
	{ "Nortek 2020 Model 1 & 2", "ECI", "nortek", "usb" },
	{ "Supergrass Web Surfer", "ECI", "supergrass", "usb" },
	{ "Telewell TW-EA100", "ECI", "telewell", "usb" },
	{ "Turbocomm EA100", "ECI", "turbocomm", "usb" },
	{ "Virata ADSL USB WAN modem", "ECI", "virta", "usb" },
	{ "Wisecom WS-AD80-USG", "ECI", "wisecom", "usb" },
	{ "Wytek FMO28", "ECI", "wytek", "usb" },
	{ "Sagem F@st 800", "EAGLE", "sagem800", "usb" },
	{ "Sagem F@st 908", "EAGLE", "sagem908", "usb" },
	{ "Eagle (generic)", "EAGLE", "generic", "usb" },
	{ "USR SureConnect USB", "EAGLE" , "usresureconnect", "usb" },
	{ "BeWAN ADSL PCI", "UNICORN", "", "pci"},
	{ "Conexant ADSL PCI", "CNX", "", "pci" },
	{ "Pulsar and Sangoma S518-C", "PULSAR", "", "pci" },
	{ NULL, NULL }
};

static int adslenabledpressed(void);
static int adsldisabledpressed(void);
static void handledevicetype(void);
static void handleothersettings(void);

int handleadsl(void)
{
	struct keyvalue *kv;
	int rc;
	char devicename[STRING_SIZE] = "";
	char vpi[STRING_SIZE] = "";
	char vci[STRING_SIZE] = "";
	char temp[STRING_SIZE];
	char ecitype[STRING_SIZE] = "";
	char message[1000];
	int c;
	char *sections[] = { "Device type", "Other settings", NULL, NULL };
	int choice;
	char enableddisabled[STRING_SIZE];
	FILE *f;
	
	if ((f = fopen(CONFIG_ROOT "red/active", "r")))
	{
		fclose(f);
		errorbox(ctr[TR_RED_IN_USE]);
		return 1;
	}
	
	/* adsl-cleanup is a small script... */
	mysystem("/etc/ppp/adsl-cleanup");

	choice = 0;	
	for (;;)
	{
		kv = initkeyvalues();
		if (!(readkeyvalues(kv, CONFIG_ROOT "adsl/settings")))
		{
			freekeyvalues(kv);
			errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
			return 0;
		}

		strcpy(enableddisabled, ctr[TR_DISABLED]);
		findkey(kv, "ENABLED", temp);
		if (strcmp(temp, "on") == 0)
			strcpy(enableddisabled, ctr[TR_ENABLED]);
						
		findkey(kv, "VPI", vpi);
		if (!strlen(vpi))
			strcpy(vpi, ctr[TR_UNSET]);
		findkey(kv, "VCI", vci);
		if (!strlen(vci))
			strcpy(vci, ctr[TR_UNSET]);

		findkey(kv, "DEVICE", temp);
		findkey(kv, "ECITYPE", ecitype);
		c = 0;
		while (devices[c].device)
		{
			if (strcmp(devices[c].device, temp) == 0 &&
				strcmp(devices[c].ecitype, ecitype) == 0)
			{
				strcpy(devicename, devices[c].name);
				break;
			}
			c++;
		}
		if (!strlen(devicename))
			strcpy(devicename, ctr[TR_UNSET]);		

		sprintf(message, "ADSL is currently: %s\n\n" \
			"   Device type: %s\n" \
			"   VPI: %s VCI: %s\n\n" \
			"Select the item you wish to reconfigure, or choose to use the current settings.",
			enableddisabled, devicename, vpi, vci);
		
		freekeyvalues(kv);
		
		rc = newtWinMenu(ctr[TR_ADSL_CONFIGURATION], message, 50, 5, 5, 6,
			sections, &choice, ctr[TR_OK], ctr[TR_ENABLE_ADSL],
			ctr[TR_DISABLE_ADSL], NULL);
		
		if (rc == 1 || rc == 0)
		{
			switch (choice)
			{
				case 0:
					handledevicetype();
					break;
					
				case 1:
					handleothersettings();	
					break;
					
				default:
					break;
			}
		}
		else if (rc == 2)
		{
			if (!adslenabledpressed())
				break;
		}
		else
		{
			if (!(adsldisabledpressed()))
				break;
		}	
	}
	
	return 1;
}

/* Returns 0 if main ADSL setup loop should exit. */
static int adsldisabledpressed(void)
{
	struct keyvalue *kv = initkeyvalues();

	if (!(readkeyvalues(kv, CONFIG_ROOT "adsl/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}

	replacekeyvalue(kv, "ENABLED", "off");
	writekeyvalues(kv, CONFIG_ROOT "adsl/settings");

	freekeyvalues(kv);
	
	return 0;
}

/* Returns 0 if main ADSL setup loop should exit. */
static int adslenabledpressed(void)
{
	struct keyvalue *kv = initkeyvalues();
	char devicename[STRING_SIZE] = "";
	char ecitype[STRING_SIZE] = "";
	char vpi[STRING_SIZE] = "";
	char vci[STRING_SIZE] = "";
	char ifacetype[STRING_SIZE];

	int result = 0;

	if (!(readkeyvalues(kv, CONFIG_ROOT "adsl/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return 0;
	}

	findkey(kv, "DEVICE", devicename);
	findkey(kv, "ECITYPE", ecitype);	
	findkey(kv, "VPI", vpi);
	findkey(kv, "VCI", vci);
	findkey(kv, "IFACETYPE", ifacetype);
	
	if (!(strlen(devicename) && strlen(vpi) && strlen(vci) && strlen(ifacetype)))
	{
		errorbox(ctr[TR_UNABLE_TO_INITIALISE_ADSL]);
		replacekeyvalue(kv, "ENABLED", "off");
		result = 1;		
	}

	if ((strcmp(ifacetype, "usb") == 0) && (usbfail))
	{
		errorbox("Device requires a USB interface which was not dected.");
		replacekeyvalue(kv, "ENABLED", "off");
		result = 1;
	}

	if (automode == 0)
	{
		runcommandwithstatus("/etc/rc.d/rc.adsl",
			ctr[TR_STARTING_UP_ADSL]);
	}
	replacekeyvalue(kv, "ENABLED", "on");

	writekeyvalues(kv, CONFIG_ROOT "adsl/settings");

	freekeyvalues(kv);
	
	return result;
}

void handledevicetype(void)
{
	char **selection;
	int c;
	int rc;
	int choice;
	struct keyvalue *kv = initkeyvalues();
	char devicename[STRING_SIZE];
	char ecitype[STRING_SIZE];
	int done = 0;
	
	if (!(readkeyvalues(kv, CONFIG_ROOT "adsl/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return;
	}
	
	strcpy(devicename, "ALCATEL"); findkey(kv, "DEVICE", devicename);
	strcpy(ecitype, ""); findkey(kv, "ECITYPE", ecitype);

	/* Count controllers. */
	c = 0;
	while (devices[c].name) c++;
	selection = malloc((c + 1) * sizeof(char *));

	/* Fill out section. */
	c = 0;
	while (devices[c].name)
	{
		selection[c] = devices[c].name;
		c++;
	}
	selection[c] = NULL;

	/* Determine inital value for choice. */
	c = 0; choice = 0;
	while (devices[c].name)
	{
		if (strcmp(devices[c].device, devicename) == 0 &&
			strcmp(devices[c].ecitype, ecitype) == 0)
		{
			choice = c;
			break;
		}
		c++;
	}
	
	while (!done)
	{
		rc = newtWinMenu("Select device type", 
			"Choose the description which best fits your ADSL device.",
			50, 5, 5, 10, selection, &choice, ctr[TR_OK], ctr[TR_CANCEL], NULL);
	
		if (rc == 2)
			done = 1;
		else
		{
			replacekeyvalue(kv, "DEVICE", devices[choice].device);
			replacekeyvalue(kv, "ECITYPE", devices[choice].ecitype);
			replacekeyvalue(kv, "IFACETYPE", devices[choice].ifacetype);
			writekeyvalues(kv, CONFIG_ROOT "adsl/settings");
			done = 1;
		}
	}

	free(selection);	
	freekeyvalues(kv);
}

void handleothersettings(void)
{
	char vpi[STRING_SIZE] = "";
	char vci[STRING_SIZE] = "";
	struct keyvalue *kv = initkeyvalues();
	const char *values[] = { vpi, vci, NULL };	/* pointers for the values. */
	struct newtWinEntry entries[] =
		{ { "VPI:", &values[0], 0,}, { "VCI:", &values[1], 0,}, { NULL, NULL, 0 } };
	int rc;
	
	if (!(readkeyvalues(kv, CONFIG_ROOT "adsl/settings")))
	{
		freekeyvalues(kv);
		errorbox(ctr[TR_UNABLE_TO_OPEN_SETTINGS_FILE]);
		return;
	}	
	
	strcpy(vpi, "0"); findkey(kv, "VPI", vpi);
	strcpy(vci, "38"); findkey(kv, "VCI", vci);
	
	for (;;)
	{	
		rc = newtWinEntries("Other ADSL settings", "Please complete the following additional fields to setup your ADSL modem.",
			50, 5, 5, 10, entries, ctr[TR_OK], ctr[TR_CANCEL], NULL);	
		
		if (rc == 1)
		{
			strcpy(vpi, values[0]);
			strcpy(vci, values[1]);
			if (!(strlen(vpi)))
				errorbox("VPI cannot be blank.");
			else if (!(strlen(vci)))
				errorbox("VCI cannot be blank.");
			else if (strspn(vpi, NUMBERS) != strlen(vpi))
				errorbox("VPI is not a number.");
			else if (strspn(vci, NUMBERS) != strlen(vci))
				errorbox("VCI is not a number.");
			else
			{			
				replacekeyvalue(kv, "VPI", vpi);
				replacekeyvalue(kv, "VCI", vci);
				writekeyvalues(kv, CONFIG_ROOT "adsl/settings");
				break;
			}
		}
		else
		{
			break;
		}
	}
	free((char *) values[0]);
	free((char *) values[1]);
	freekeyvalues(kv);
}	
