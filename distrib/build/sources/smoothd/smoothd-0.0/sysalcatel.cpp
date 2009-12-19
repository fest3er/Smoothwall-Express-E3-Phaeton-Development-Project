/* SysAlcatel Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to downloading Alcatel USB ADSL firmware    */
/* (c) 2007 SmoothWall Ltd                                                */
/* ---------------------------------------------------------------------- */
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
	int load(std::vector<CommandFunctionPair> & );
	int set_alcatel(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair set_alcatel_function("alcateladslfw", "set_alcatel", 0, 0);

	pairs.push_back(set_alcatel_function);

	return 0;
}

int set_alcatel(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	struct stat sb;

	if ((stat("/var/smoothwall/adsl/mgmt.o", &sb) == 0) && sb.st_mode & S_IRUSR)
	{
		error = simplesecuresysteml("/usr/bin/modem_run", "-f", "/var/smoothwall/adsl/mgmt.o", "-v", "1", NULL);
		response = "Alcatel USB firmware loaded";
		if (error)
			response += stringprintf(" but with error return %d", error);
	}
	else
	{
		response = "No Alcatel USB firmware to load!";
		error = 1;
	}

	return error;
}
