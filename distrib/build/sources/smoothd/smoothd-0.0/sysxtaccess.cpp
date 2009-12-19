/* SysXtAccess Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to external Access                           */
/* (c) 2007 SmoothWall Ltd                                                */
/* ----------------------------------------------------------------------  */
/* Original Author  : Daniel Goscomb                                       */
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
	int set_xtaccess(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair set_xtaccess_function("setxtaccess", "set_xtaccess", 0, 0);

	pairs.push_back(set_xtaccess_function);

	return 0;
}

int set_xtaccess(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	ConfigSTR ifacef("/var/smoothwall/red/iface");
	ConfigCSV config("/var/smoothwall/xtaccess/config");
	std::vector<std::string>ipb;
	std::string::size_type n;
	std::string iface = ifacef.str();

	if (iface.substr(0, 3) == "ppp" || iface.substr(0, 4) == "ippp")
		iface = ""; // ignore ppp
	if (iface != "" && ((n = iface.find_first_not_of(LETTERS_NUMBERS)) != std::string::npos))
	{
		response = "Bad iface: " + iface; 
		error = 1;
		goto EXIT;
	}
	
	ipb.push_back("iptables  -F xtaccess");

	for (int line = config.first(); line == 0; line = config.next())
	{
		const std::string & protocol = config[0];
		const std::string & remip = config[1];
		const std::string & locport = config[2];
		const std::string & enabled = config[3];

		// are we complete?
		if (protocol == "" || remip == "" || locport == "" || enabled == "")
			break;

		if ((n = protocol.find_first_not_of(LETTERS)) != std::string::npos)
		{
			response = "Bad protocol: " + protocol;
			error = 1;
			goto EXIT;
		}
		if ((n = remip.find_first_not_of(IP_NUMBERS)) != std::string::npos)
		{
			response = "Bad remote IP: " + remip;
			error = 1;
			goto EXIT;
		}
		if ((n = locport.find_first_not_of(NUMBERS_COLON)) != std::string::npos)
		{
			response = "Bad port: " + locport;
			error = 1;
			goto EXIT;
		}
		if (enabled == "on")
		{
			ipb.push_back("iptables -A xtaccess -i ppp0 -p " + protocol + 
				" -s " + remip + 
				" --destination-port " + locport + " -j ACCEPT");
			ipb.push_back("iptables -A xtaccess -i ippp0 -p " + protocol + 
				" -s " + remip + 
				" --destination-port " + locport + " -j ACCEPT");

			if (iface != "")
			{
				ipb.push_back("iptables -A xtaccess -i " + iface + " -p " + protocol + 
					" -s " + remip + 
					" --destination-port " + locport + " -j ACCEPT");
			}		
		}
	}

	error = ipbatch(ipb);
	if (error)
		response = "ipbatch failure";
	else
		response = "xtaccess set";

EXIT:
	return error;
}
