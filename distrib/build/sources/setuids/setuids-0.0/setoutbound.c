/* SmoothWall helper program - setoutbound.c
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Daniel Goscomb, 2001
 * Modifications and improvements by Lawrence Manning.
 * 10/04/01 Aslak added protocol support
 * 01/03/06 Josh converted to open outbound ports
 *
 * filename: setoutbound.c
 */

#include "setuid.h"

void strToLower(char* inString);

int main(void)
{
	FILE *configFile = NULL;
	int count;
	char iface[STRING_SIZE];

	char *variableName;
	char *variableValue;
	char *redDev;
	
	char *colour;
	char *protocol;
	char *interface;
	char *port;
	char *enabled;
	
	char s[STRING_SIZE];
	char *result;
	
	char command[STRING_SIZE];
	char remdouble[STRING_SIZE];

	if (!(initsetuid(1))) {
		exit(1);
	}

	/* Outbound config */
 	if (!(configFile = fopen("/var/smoothwall/outbound/config", "r"))) {
		fprintf(stderr, "Couldn't open outbound config file\n");
		goto EXIT;
	}
	
	system("/sbin/iptables -F outgreen");
	system("/sbin/iptables -F outorange");
	system("/sbin/iptables -F outpurple");

	while (fgets(s, STRING_SIZE, configFile) != NULL) {
		if (s[strlen(s) - 1] == '\n') {
			s[strlen(s) - 1] = '\0';		        	
		}
		result = strtok(s, ",");

		count = 0;
		colour = NULL;
		protocol = NULL;
		port = NULL;
		enabled = NULL;
		
		while (result) {
			if (count == 0) {
				colour = result;
				strToLower(colour);
			}
			else if (count == 1)
				protocol = result;
			else if (count == 2)
				port = result;
			else
				enabled = result;
			count++;
			result = strtok(NULL, ",");
		}
		
		if (!(colour && protocol && port && enabled))
			break;
		
		if (strspn(colour, LETTERS) != strlen(colour)) {
			fprintf(stderr, "Bad colour: %s\n", colour);
			goto EXIT;
		}
		
		if (strspn(port, NUMBERS) != strlen(port)) {
			fprintf(stderr, "Bad remote port: %s\n", port);
			goto EXIT;
		}

		if (strcmp(enabled, "on") == 0) {
			memset(command, 0, STRING_SIZE);
			snprintf(command, STRING_SIZE - 1,
				"/sbin/iptables -A out%s -p %s --dport %s -j ACCEPT",
				colour, protocol, port);
			system(command);
		}
	}
	system("/sbin/iptables -A outgreen -j REJECT");
	system("/sbin/iptables -A outorange -j REJECT");
	system("/sbin/iptables -A outpurple -j REJECT");

	/* Outbound Settings */
 	if (!(configFile = fopen("/var/smoothwall/outbound/settings", "r"))) {
		fprintf(stderr, "Couldn't open outbound settings file\n");
		goto EXIT;
	}

	system("iptables -F outbound");
	
	while (fgets(s, STRING_SIZE, configFile) != NULL) {
		if (s[strlen(s) - 1] == '\n') {
			s[strlen(s) - 1] = '\0';		        	
		}
		result = strtok(s, ",");

		count = 0;
		colour = NULL;
		interface = NULL;
		enabled = NULL;
		
		while (result) {
			if (count == 0) {
				colour = result;
				strToLower(colour);
			}
			else if (count == 1)
				interface = result;
			else
				enabled = result;
			count++;
			result = strtok(NULL, ",");
		}
		
		if (!(colour && interface && enabled))
			break;
		
		if (strspn(colour, LETTERS) != strlen(colour)) {
			fprintf(stderr, "Bad colour: %s\n", colour);
			goto EXIT;
		}
		
		if (strcmp(enabled, "on") == 0) {
			memset(command, 0, STRING_SIZE);
			snprintf(command, STRING_SIZE - 1,
				"/sbin/iptables -A outbound -i %s -j out%s",
				interface, colour);
			system(command);
		}
	}

	system("iptables -A outbound -j ACCEPT");
	
EXIT:
	if (configFile)
		fclose(configFile);
	
	return 0;
}

void strToLower(char* inString) {
	int len = strlen(inString);
	int i;
	for(i=0; i<len; i++) {
		inString[i] = (char)tolower((int)inString[i]);
	}
}
