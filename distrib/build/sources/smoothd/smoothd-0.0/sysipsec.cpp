/* SysIpsec Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to starting/restarting IPSEC     n         */
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
	int load(std::vector<CommandFunctionPair> & );
	int restart_ipsec(std::vector<std::string> & parameters, std::string & response);
	int start_ipsec(std::vector<std::string> & parameters, std::string & response);
	int stop_ipsec(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair restart_ipsec_function("ipsecrestart", "restart_ipsec", 0, 0);
	CommandFunctionPair start_ipsec_function("ipsecstart", "start_ipsec", 0, 0);
	CommandFunctionPair stop_ipsec_function("ipsecstop", "stop_ipsec", 0, 0);

	pairs.push_back(restart_ipsec_function);
	pairs.push_back(start_ipsec_function);
	pairs.push_back(stop_ipsec_function);

	return 0;
}

int restart_ipsec(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	
	error += stop_ipsec(parameters, response);
	
	if (!error)
		error += start_ipsec(parameters, response);
	
	if (!error)
		response = "IPSEC Restart Successful";
	
	return error;
}

int stop_ipsec(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	simplesecuresysteml("/usr/sbin/ipsec", "setup",  "stop", NULL);
	response = "IPSEC Process Terminated";

	return error;
}


int start_ipsec(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	simplesecuresysteml("/usr/sbin/ipsec", "setup", "restart", NULL);
	response = "IPSEC Start Successful";

	return error;
}
