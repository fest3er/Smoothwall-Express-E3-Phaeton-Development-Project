/* SmoothWall helper program - smoothiedeath
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: smoothiedeath.c
 * Simple program intended to be installed setuid(0) that can be used for
 * starting shutdown. */
         
#include "setuid.h"

int main(void)
{
	if (!(initsetuid(1)))
		exit(1);
	
	system("/sbin/shutdown -h now");
	
	return 0;
}
