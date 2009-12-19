/* SysDHCPD Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to starting/restarting dhcp daemon         */
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
	int restart_dhcpd(std::vector<std::string> & parameters, std::string & response);
	int start_dhcpd(std::vector<std::string> & parameters, std::string & response);
	int stop_dhcpd(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair restart_dhcpd_function("dhcpdrestart", "restart_dhcpd", 0, 0);
	CommandFunctionPair start_dhcpd_function("dhcpdstart", "start_dhcpd", 0, 0);
	CommandFunctionPair stop_dhcpd_function("dhcpdstop", "stop_dhcpd", 0, 0);

	pairs.push_back(restart_dhcpd_function);
	pairs.push_back(start_dhcpd_function);
	pairs.push_back(stop_dhcpd_function);

	return 0;
}

int restart_dhcpd(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	
	error += stop_dhcpd(parameters, response);
	
	if (!error)
		error += start_dhcpd(parameters, response);
	
	if (!error)
		response = "DHCPD Restart Successful";
	
	return error;
}

int stop_dhcpd(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	
	killprocess("/var/run/dhcpd.pid");
	response = "DHCPD Process Terminated";
	unlink("/var/run/dhcpd.pid");
	
	return error;
}


int start_dhcpd(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	ConfigSTR green("/var/smoothwall/dhcp/green");
	ConfigSTR purple("/var/smoothwall/dhcp/purple");
	std::vector<std::string> args;
	
	args.push_back("/usr/sbin/dhcpd");

	if (green.str() != "")
		args.push_back(green.str());
	if (purple.str() != "")
		args.push_back(purple.str());
		
	if (args.size() > 1)
	{
		error = simplesecuresystemvector(args);
		
		if (error)
			response = "DHCPD Start failed!";
		else
			response = "DHCPD Start Successful";
	}
	
	return error;
}
	
