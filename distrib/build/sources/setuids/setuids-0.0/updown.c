/* SmoothWall helper program - restartssh
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: restartssh
 * Simple program intended to be installed setuid(0) that can be used for
 * restarting sshd. */

#include "setuid.h"

int main(int argc, char *argv[])
{
	if (!(initsetuid(1)))
		goto EXIT;
		
	if (argc < 2)
		goto EXIT;
		
	if (strcmp(argv[1], "UP") == 0)
		system("/usr/bin/smoothwall/ppp-on");
	else
		system("/usr/bin/smoothwall/ppp-off");

EXIT:

	return 0;
}
