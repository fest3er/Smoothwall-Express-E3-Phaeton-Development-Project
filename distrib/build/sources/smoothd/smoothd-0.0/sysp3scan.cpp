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

	int restart_p3scan(std::vector<std::string> & parameters, std::string & response);
	int   start_p3scan(std::vector<std::string> & parameters, std::string & response);
	int    stop_p3scan(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair restart_p3scan_function("p3scanrestart", "restart_p3scan", 0, 0);
	CommandFunctionPair   start_p3scan_function("p3scanstart",     "start_p3scan", 0, 0);
	CommandFunctionPair    stop_p3scan_function("p3scanstop",       "stop_p3scan", 0, 0);

	pairs.push_back(restart_p3scan_function);
	pairs.push_back(  start_p3scan_function);
	pairs.push_back(   stop_p3scan_function);

	return 0;
}

int restart_p3scan(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	error += stop_p3scan(parameters, response);

	if (!error)
		error += start_p3scan(parameters, response);

	if (!error)
		response = "P3Scan Restart Successful";

	return error;
}


int stop_p3scan(std::vector<std::string> & parameters, std::string & response)
{	
	std::vector<std::string>ipb;
	response = "P3Scan Process Terminated";

	killprocess("/var/run/p3scan.pid");
	ipb.push_back("iptables -t nat -F p3scan");
	ipbatch(ipb);

	return 0;
}

int start_p3scan(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	std::vector<std::string>ipb;

	response = "P3Scan Process started";

	ConfigVAR settings("/var/smoothwall/p3scan/settings");

	ipb.push_back("iptables -t nat -F p3scan");
	
	if (settings["ENABLE"] == "on")
		ipb.push_back("iptables -t nat -A p3scan -p tcp --dport 110 -j REDIRECT --to-ports 8110");

	error = ipbatch(ipb);

	if (error)
	{
		response = "ipbatch failure";
		goto EXIT;
	}

	if (settings["ENABLE"] == "on")
	{
		error = simplesecuresysteml("/usr/sbin/p3scan", "-f", 
			"/var/smoothwall/p3scan/p3scan.conf", NULL);

		if (error)
			response = "P3Scan Start Failed!";
		else
			response = "P3Scan Start Successful";
	}

EXIT:
	return error;
}
