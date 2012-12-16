/* SysSquid Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to starting/restarting squid                  */
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
	int load(std::vector<CommandFunctionPair> &);
	int restart_squid(std::vector<std::string> & parameters, std::string & response);
	int start_squid(std::vector<std::string> & parameters, std::string & response);
	int stop_squid(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair restart_squid_function("squidrestart", "restart_squid", 0, 0);
	CommandFunctionPair start_squid_function("squidstart", "start_squid", 0, 0);
	CommandFunctionPair stop_squid_function("squidstop", "stop_squid", 0, 0);

	pairs.push_back(restart_squid_function);
	pairs.push_back(start_squid_function);
	pairs.push_back(stop_squid_function);

	return 0;
}

int restart_squid(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	response = "Squid Restart Fails";
	
	error += stop_squid(parameters, response);
	
	if (!error)
		error += start_squid(parameters, response);
	
	if (!error)
		response = "Squid Restart Successful";

	return error;
}

int stop_squid(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	killunknownprocess("squid");
	sleep(2);
	response = "Squid Process Terminated";

	return error;
}

int start_squid(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	ConfigVAR settings("/var/smoothwall/proxy/settings");	
	std::vector<std::string>ipb;

	response = "Squid Process started";

	ipb.push_back("iptables -t nat -F squid");
	
	for (unsigned int c = 0; c < parameters.size(); c++)
	{
		if (parameters[c] == "--clearcache")
		{
			simplesecuresysteml("/bin/rm", "-rf",  "/var/spool/squid/cache", NULL);
			simplesecuresysteml("/bin/mkdir", "-p",  "/var/spool/squid/cache", NULL);
			simplesecuresysteml("/bin/chown", "squid:squid", "/var/spool/squid/cache", NULL);
		}
	}
	
	if (settings["ENABLE"] == "on")
	{ 
		simplesecuresysteml("/usr/sbin/squid", "-z", "-f", "/var/smoothwall/proxy/squid.conf", NULL);
		simplesecuresysteml("/usr/sbin/squid", "-f", "/var/smoothwall/proxy/squid.conf", NULL);

		if (settings["TRANSPARENT"] == "on")
			ipb.push_back("iptables -t nat -A squid -p tcp --dport 80 -j REDIRECT --to-ports 800");
	}	
	
	error = ipbatch(ipb);

	if (error)
	{
		response = "ipbatch failure";
		goto EXIT;
	}	
	
	simplesecuresysteml("/bin/chown", "squid:squid", "/var/spool/squid/cache", NULL);
	
	if (!error)
		response = "Squid Start Successful";

EXIT:
	return error;
}
	
