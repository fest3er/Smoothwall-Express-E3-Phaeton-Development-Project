/* SmoothWall setup program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: setup.h
 * Main include file. */

#include "../libsmooth/libsmooth.h"

/* hostanme.c */
int handlehostname(void);

/* isdn.c */
int handleisdn(void);

/* networking.c */
int handlenetworking(void);

/* dhcp.c */
int handledhcp(void);

/* passwords.c */
int handlerootpassword(void);
int handlesetuppassword(void);
int handleadminpassword(void);

/* misc.c */
int writehostsfiles(void);

/* keymap.c */
int handlekeymap(void);

/* adsl.c */
int handleadsl(void);

/* webproxy.c */
int handlewebproxy(void); 

/* defaults.c */
int handledefaults(void);
