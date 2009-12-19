/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: main.c
 * Contains library functions. */

#include "libsmooth.h"

extern FILE *flog;
extern char *logname;

extern char **ctr;
  
/* reboot().  reboots. */
void reboot(void)
{
	struct stat statbuf;
        
	memset(&statbuf, 0, sizeof(struct stat));
        
	if (!stat("/sbin/shutdown", &statbuf))
		mysystem("/sbin/shutdown -r now");
	else
		mysystem("/etc/halt");
}

/* stripnl().  Replaces \n with \0 */
void stripnl(char *s)
{
	char *t = strchr(s, '\n');
	if (t) *t = '\0';
}

/* Little wrapper. */
int mysystem(char *command)
{
	char mycommand[STRING_SIZE];
	
	snprintf(mycommand, STRING_SIZE, "%s >>%s 2>>%s", command, logname, logname);
	fprintf(flog, "Running command: %s\n", command);
	return system(mycommand);
}

/* Little wrapper. */
int mysystemnolog(char *command)
{
	char mycommand[STRING_SIZE];
	
	snprintf(mycommand, STRING_SIZE, "%s >>%s 2>>%s", command, logname, logname);
	fprintf(flog, "Running command: NOT LOGGED\n");
	return system(mycommand);
}

void errorbox(char *message)
{
	newtWinMessage(ctr[TR_ERROR], ctr[TR_OK], message);
}

void statuswindow(int width, int height, char *title, char *text, ...)
{
	newtComponent t, f;
	char *buf = NULL;
	int size = 0;
	int i = 0;
	va_list args;

	va_start(args, text);

	do {
		size += 1000;
		if (buf) free(buf);
		buf = malloc(size);
		i = vsnprintf(buf, size, text, args);
	} while (i == size);

	va_end(args);

	newtCenteredWindow(width, height, title);

	t = newtTextbox(1, 1, width - 2, height - 2, NEWT_TEXTBOX_WRAP);
	newtTextboxSetText(t, buf);
	f = newtForm(NULL, NULL, 0);

	free(buf);

	newtFormAddComponent(f, t);

	newtDrawForm(f);
	newtRefresh();
	newtFormDestroy(f);
}

int runcommandwithstatus(char *command, char *message)
{
	int rc;
	
	statuswindow(60, 4, TITLE, message);
	rc = mysystem(command);
	newtPopWindow();
	
	return rc;
}

int runcommandwithstatusnolog(char *command, char *message)
{
	int rc;
	
	statuswindow(60, 4, TITLE, message);
	rc = mysystemnolog(command);
	newtPopWindow();
	
	return rc;
}

/* This one borrowed from redhat installer. */
int runcommandwithprogress(int width, int height, char *title, char *command,
	int lines, char *text, ...)
{
	newtComponent t, f, s;
	char *buf = NULL;
	int size = 0;
	int i = 0;
	va_list args;
	int rc = 0;
	FILE *p;
	char buffer[STRING_SIZE];
	int progress = 0;
	char mycommand[STRING_SIZE];

	va_start(args, text);

	do {
		size += 1000;
		if (buf) free(buf);
		buf = malloc(size);
		i = vsnprintf(buf, size, text, args);
	} while (i == size);

	va_end(args);

	newtCenteredWindow(width, height, title);

	t = newtTextbox(1, 1, width - 2, height - 2, NEWT_TEXTBOX_WRAP);
	newtTextboxSetText(t, buf);
	f = newtForm(NULL, NULL, 0);

	free(buf);

	newtFormAddComponent(f, t);
	
	s = newtScale(1, 3, width - 2, lines);
	newtScaleSet(s, progress);
	
	newtFormAddComponent(f, s);

	newtDrawForm(f);
	newtRefresh();
	
	snprintf(mycommand, STRING_SIZE, "%s 2>>%s", command, logname);
	fprintf(flog, "Running command: %s\n", command);
	
	if (!(p = popen(command, "r")))
	{
		rc = 1;
		goto EXIT;
	}
	setvbuf(p, NULL, _IOLBF, 255);
	
	while (fgets(buffer, STRING_SIZE, p))
	{
		newtScaleSet(s, ++progress);
		newtRefresh();	
		fprintf(flog, "%s", buffer);
	}
		
	pclose(p);
	rc = 0;
	
EXIT:
	newtFormDestroy(f);
	newtPopWindow();
		
	return rc;
}

int checkformodule(char *module)
{
	FILE *file;
	char buffer[STRING_SIZE];
	int result = 0;
	
	if (!(file = fopen("/proc/modules", "r")))
	{
		fprintf(flog, "Unable to open /proc/modules in checkformodule()\n");
		return 0;
	}
	
	while (fgets(buffer, STRING_SIZE, file))
	{
		if (strncmp(buffer, module, strlen(module)) == 0)
		{
			if (buffer[strlen(module)] == ' ')
			{
				result = 1;
				goto EXIT;
			}
		}
	}
	
EXIT:
	fclose(file);
	
	return result;
}	

/* Returns 0 for success. */
int touchfile(char *filename)
{
	FILE *file;
  
	if ((file = fopen(filename, "w")))
	{
		fclose(file);
		return 0;
	}
	else
		return 1;
}

/* Returns total amount of system memory in MB. */
int gettotalmemory(void)
{
	struct sysinfo s;
	
	if (!(sysinfo(&s)))
		return (s.totalram / s.mem_unit) / (1024 * 1024);
	else
		return 0;
}
