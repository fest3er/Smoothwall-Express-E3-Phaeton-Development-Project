/* SmoothWall helper program - header file
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * filename: ipbatch.cpp
 * Direct interface into iptables. */
 /* Modified to check the size of the next line plus current buffer position
  * against the maximum buffer limit and flush the buffer before adding the
  * next line to the buffer.  08/11/29  Steven L Pittman  */

#include <vector>
#include <string>
#include <errno.h>
#include <syslog.h>

#include "ipbatch.h"

// local storage

static std::vector<std::string> mybatch;

// work from char *
int ipbatch(const char *arg) 
{
   int rval = 0;
   if (strcmp(arg,"commit") == 0) {
      rval = ipbatch(mybatch);
      mybatch.clear();
   }
   else 
      mybatch.push_back(std::string(arg));
   return rval;
}
// or from single strings
int ipbatch(const std::string &arg) 
{
   int rval = 0;
   if (arg == "commit") 
   {
      rval = ipbatch(mybatch);
      mybatch.clear();
   }
   else 
      mybatch.push_back(arg);
   return rval;
}

// but best way is as whole vector as this implies that we want to
// execute now too - no need for commit.
int ipbatch(std::vector<std::string> &arg)
{
   FILE * output = popen( "/usr/sbin/ipbatch", "w" );

   for (std::vector<std::string>::iterator i = arg.begin(); i != arg.end(); i++)
   {
      fprintf(output, "%s\n", (*i).c_str());
      fflush(output);
   }
   fprintf(output, "commit\n");
   fflush(output);
   fprintf(output, "end\n");
   fflush(output);
   
   return pclose(output);
}
