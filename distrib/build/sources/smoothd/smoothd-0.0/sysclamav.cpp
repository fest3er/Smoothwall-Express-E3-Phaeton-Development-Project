/* SysIM Module for the SmoothWall SUIDaemon                              */
/* Contains functions relating to the management of the P3Scan            */
/* (c) 2005 SmoothWall Ltd                                                */
/* ---------------------------------------------------------------------- */
/* Original Author : D.K.Taylor                                           */

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

	int restart_clamav(std::vector<std::string> & parameters, std::string & response);
	int   start_clamav(std::vector<std::string> & parameters, std::string & response);
	int    stop_clamav(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair restart_clamav_function("clamavrestart", "restart_clamav", 0, 0);
	CommandFunctionPair   start_clamav_function("clamavstart",     "start_clamav", 0, 0);
	CommandFunctionPair    stop_clamav_function("clamavstop",       "stop_clamav", 0, 0);

	pairs.push_back(restart_clamav_function);
	pairs.push_back(  start_clamav_function);
	pairs.push_back(   stop_clamav_function);

	return 0;
}

int restart_clamav(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	error += stop_clamav(parameters, response);

	if (!error)
		error += start_clamav(parameters, response);

	if (!error)
		response = "ClamAV Restart Successful";

	return error;
}


int stop_clamav(std::vector<std::string> & parameters, std::string & response)
{	
	response = "ClamAV Process Terminated";

	killunknownprocess("clamd");

	return 0;
}

int start_clamav(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	response = "ClamAV Process started";

	ConfigVAR settings("/var/smoothwall/clamav/settings");

	if (settings["ENABLE_ZAP"] == "on" || settings["ENABLE_GUARDIAN"] == "on")
	{
		error = simplesecuresysteml("/usr/sbin/clamd", "--config-file=/usr/lib/smoothwall/clamd.conf", NULL);

		if (error)
			response = "ClamAV Start Failed!";
		else
			response = "ClamAV Start Successful";
	}

	return error;
}
