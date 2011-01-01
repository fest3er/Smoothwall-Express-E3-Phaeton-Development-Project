/*
 * beep2 version 1.2a
 * Original Copyright (C) 1997 Josef Pavlik <jetset@ibm.net>
 * Copyright (C) 2002 YAMAGUCHI Shingo <shingo@kip.iis.toyama-u.ac.jp>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <syslog.h>

#include "beep.h"

#ifdef SYSTEM_LOGGING

void write_log(int argc, char **argv)
{
  uid_t         execuid;
  struct passwd *execpwuid;
  char          *execuname;

  int           i;
  char          *execstr;

  execuname = (char*)malloc(BUFFER);
  execstr = (char*)malloc(BUFFER);

  /*
   * get execute user ID
   */
  execuid = getuid();
  execpwuid = getpwuid(execuid);
  strcpy(execuname, execpwuid->pw_name);

  /*
   * get execute string
   */
  for (i = 0; i <= argc - 1; i++) {
    sprintf(execstr, "%s %s", execstr, argv[i]);
  }

  /*
   * send log message to syslog
   */
  syslog(LOG_INFO, "user = %s, exec =%s\n", execuname, execstr);

  free(execuname);
  free(execstr);
}
#endif
