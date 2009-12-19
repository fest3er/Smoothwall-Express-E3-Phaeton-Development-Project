/* SysIpBlock Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to setting IP block rules                    */
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
	int set_ipblock(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair set_ipblock_function("setipblock", "set_ipblock", 0, 0);
	pairs.push_back(set_ipblock_function);

	return (0);
}

int set_ipblock(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	ConfigCSV config("/var/smoothwall/ipblock/config");
	std::vector<std::string>ipb;
	std::string::size_type n;
	
	ipb.push_back("iptables  -F ipblock");
	for (int line = config.first(); line == 0; line = config.next())
	{
		const std::string & remip = config[0];
		const std::string & log = config[1];
		const std::string & target = config[2];
		const std::string & enabled = config[3];

		// are we complete?
		if (remip == "" && enabled != "")
			break;

		if ((n = remip.find_first_not_of(IP_NUMBERS)) != std::string::npos)
		{
			response = "Bad remote IP: " + remip;
			error = 1;
			goto EXIT;
		}
		if (!(target == "DROP" || target == "REJECT"))
		{
			response = "Bad target: " + target;
			error = 1;
			goto EXIT;
		}
		if (enabled == "on")
		{
			if (log == "on")
				ipb.push_back("iptables -A ipblock -s " + remip + " -j LOG");
			ipb.push_back("iptables -A ipblock -s " + remip + " -j " + target);
		}
	}

	error = ipbatch(ipb);
	
	if (error)
	{
		response = "ipbatch failure";
		goto EXIT;
	}
	
	response = "Block rules set";

EXIT:
	return error;
}
