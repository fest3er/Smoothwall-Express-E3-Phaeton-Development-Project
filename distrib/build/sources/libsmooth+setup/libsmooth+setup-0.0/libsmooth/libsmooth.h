/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: libsmooth
 * Contains prototypes for library functions. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <newt.h>
#include <dirent.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <linux/cdrom.h>
#include <linux/if.h>

#include "langs.h"

#define TITLE "SmoothWall Express 3.0"

#define STRING_SIZE 256

#define ADDRESS 0
#define NETADDRESS 1
#define NETMASK 2
#define DHCP 3
#define NETCHANGE_TOTAL 4

#define CONFIG_ROOT "/var/smoothwall/"
/* #define CONFIG_ROOT "" */

#define STRING_SIZE 256
#define BIG_STRING_SIZE 1024
#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMBERS "0123456789"
#define LETTERS_NUMBERS LETTERS NUMBERS
#define IP_NUMBERS "./" NUMBERS


struct keyvalue
{
 	char key[STRING_SIZE];
 	char value[STRING_SIZE];
 	struct keyvalue *next;  
};

/* for stuff in net.c */
struct nic
{
	char description[STRING_SIZE];
	char modulename[STRING_SIZE];
};

/* main.c */
void reboot(void);
void stripnl(char *s);
int mysystem(char *command);
int mysystemnolog(char *command);
void errorbox(char *message);
void statuswindow(int width, int height, char *title, char *text, ...);
int runcommandwithprogress(int width, int height, char *title, char *command,
	int lines, char *text, ...);
int runcommandwithstatus(char *command, char *message);
int runcommandwithstatusnolog(char *command, char *message);
int checkformodule(char *module); 
int touchfile(char *filename);
int gettotalmemory(void);

/* netstuff.c */
int changeaddress(struct keyvalue *kv, char *colour, int typeflag,
	char *defaultdhcphostname);
int gettype(char *type);
int setnetaddress(struct keyvalue *kv, char *colour);
void networkdialogcallbacktype(newtComponent cm, void *data);
int interfacecheck(struct keyvalue *kv, char *colour);
int countcards(void);
int findnicdescription(char *modulename, char *description);
int getnicmac(char *dest, int size, char *dev);
	  
/* data.c */
struct keyvalue *initkeyvalues(void);
void freekeyvalues(struct keyvalue *head);
int readkeyvalues(struct keyvalue *head, char *filename);
int writekeyvalues(struct keyvalue *head, char *filename);
int findkey(struct keyvalue *head, char *key, char *value);
void appendkeyvalue(struct keyvalue *head, char *key, char *value);
void replacekeyvalue(struct keyvalue *head, char *key, char *value);

