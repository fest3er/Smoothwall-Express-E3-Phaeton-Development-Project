/* SmoothWall helper program - usbadslfw
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: usbadslfw.c
 * Updates USB ADSL firmware. */
         
#include "setuid.h"

int main(void)
{
	FILE *f;

	if (!(initsetuid(1)))
		exit(1);
	
	if ((f = fopen("/var/smoothwall/adsl/mgmt.o", "r")))
	{
		system("/usr/bin/modem_run -f /var/smoothwall/adsl/mgmt.o -v 1");
		fclose(f);
	}
	
	return 0;
}
