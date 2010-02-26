/* SmoothWall loader program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: loader.c */

/* Floppy and bootdisktwo stuff removed; 29-Sep-09, NPM */

#include "setuid.h"

#define WAIT_FOR_KEY	fgets(dummy, STRING_SIZE, stdin);
#define WHITE_FG	printf("\033[37m");
#define BLACK_FG	printf("\033[30m");
#define BLUE_BG		printf("\033[44m");
#define BOLD		printf("\033[1m");
#define NORMAL		printf("\033[0m");
#define HOME_CLEAR	printf("\033[2J\033[0;0H");

int main(int argc, char *argv[])
{

	BLUE_BG WHITE_FG HOME_CLEAR BOLD	

	printf("                             SmoothWall " PRODUCT_NAME PRODUCT_VERSION "\n\n");

	NORMAL BLUE_BG WHITE_FG

	system("/bin/install /dev/tty2");

	NORMAL

	printf("\n");			

	return 0;
}
