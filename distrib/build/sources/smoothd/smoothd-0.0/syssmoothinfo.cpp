/* Syssmoothinfo Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to starting/restarting ntp daemon         */
/* (c) 2007 SmoothWall Ltd                                                */
/* ----------------------------------------------------------------------  */
/* Original Author  : Lawrence Manning                                     */
/* Translated to C++: M. W. Houston                                        */

/* include the usual headers.  iostream gives access to stderr (cerr)     */
/* module.h includes vectors and strings which are important              */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>

#include "module.h"
#include "ipbatch.h"
#include "setuid.h"

extern "C" {
	int load( std::vector<CommandFunctionPair> &  );
	int smoothinfo_getchains(std::vector<std::string> & parameters, std::string & response);
	int smoothinfo_generate(std::vector<std::string> & parameters, std::string & response);
}

int load( std::vector<CommandFunctionPair> & pairs )
{
	/* CommandFunctionPair name( "command", "function" ); */
	CommandFunctionPair smoothinfo_getchains_function("smoothinfogetchains", "smoothinfo_getchains", 0, 0);
	CommandFunctionPair smoothinfo_generate_function("smoothinfogenerate", "smoothinfo_generate", 0, 0);

	pairs.push_back(smoothinfo_getchains_function);
	pairs.push_back(smoothinfo_generate_function);

	return 0;
}

int smoothinfo_getchains(std::vector<std::string> & parameters, std::string & response)
{
   int error = 0;
   response = "smoothinfo: listing iptables chains...";
      error = simplesecuresysteml("/usr/bin/smoothwall/getchains.pl", NULL);
      if (!error)
         response = "smoothinfo: list created successfully.";
      else
         response = "smoothinfo: unable to create list!";
	
   return 0;
}

int smoothinfo_generate(std::vector<std::string> & parameters, std::string & response)
{
   int error = 0;
   response = "smoothinfo: generating report...";
      error = simplesecuresysteml("/usr/bin/smoothwall/smoothinfo.pl", NULL);
      if (!error)
         response = "smoothinfo: report generated successfully.";
      else
         response = "smoothinfo: unable to generate report";
	
   return 0;
}
