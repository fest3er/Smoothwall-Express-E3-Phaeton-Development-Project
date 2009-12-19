/* SmoothWall helper program - header file
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * filename: setuid.h
 * Simple header file for all setuid progs. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#include <syslog.h>
#include <signal.h>

#define STRING_SIZE 256
#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMBERS "0123456789"
#define NUMBERS_COLON "0123456789:"
#define LETTERS_NUMBERS LETTERS NUMBERS
#define IP_NUMBERS "./" NUMBERS

int initsetuid(int closefd)
{
	if (closefd)
	{
		if (close(0)) { fprintf(stderr, "Couldn't close 0\n"); return 0; }
		if (close(1)) { fprintf(stderr, "Couldn't close 1\n"); return 0; }
	}
	if (setgroups(0, NULL)) { fprintf(stderr, "Couldn't clear group list\n"); exit(1); }
	if (setgid(0)) { fprintf(stderr, "Couldn't set GID to 0\n"); return 0; }
	if (setuid(0)) { fprintf(stderr, "Couldn't set UID to 0\n"); return 0; }

	return 1;
}

