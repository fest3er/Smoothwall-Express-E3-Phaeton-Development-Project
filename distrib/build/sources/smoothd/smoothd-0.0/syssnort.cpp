/* SysSnort Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to starting/restarting snort                  */
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
	int   restart_snort(std::vector<std::string> & parameters, std::string & response);
	int   start_snort(std::vector<std::string> & parameters, std::string & response);
	int   stop_snort(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair restart_snort_function("snortrestart",   "restart_snort", 0, 0);
	CommandFunctionPair start_snort_function("snortstart",     "start_snort", 0, 0);
	CommandFunctionPair stop_snort_function("snortstop",       "stop_snort", 0, 0);

	pairs.push_back(restart_snort_function);
	pairs.push_back(start_snort_function);
	pairs.push_back(stop_snort_function);

	return 0;
}

int restart_snort(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	
	error += stop_snort(parameters, response);
	
	if (!error)
		error += start_snort(parameters, response);
	
	if (!error)
		response = "Snort Restart Successful";
	
	return error;
}

int stop_snort(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	killunknownprocess("snort");
	sleep(1);
	response = "Snort Process Terminated";

	return error;
}


int start_snort(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	ConfigSTR iface("/var/smoothwall/red/iface");
	ConfigSTR dns1("/var/smoothwall/red/dns1");
	ConfigSTR dns2("/var/smoothwall/red/dns2");
	ConfigSTR locip("/var/smoothwall/red/local-ipaddress");
	std::string::size_type n;
	std::string command;
	FILE *varsfile;
	int fd;
	int trys;
	ConfigVAR settings("/var/smoothwall/snort/settings");
	
	if (iface.str() == "")
	{
		response = "Couldn't open iface file";
		goto EXIT;
	}

	if ((n = iface.str().find_first_not_of(LETTERS_NUMBERS)) != std::string::npos) 
	{
		response = "Bad iface " + iface.str();
		error = 1;
		goto EXIT;
	}
	if (dns1.str() != "" && (n = dns1.str().find_first_not_of(IP_NUMBERS)) != std::string::npos) 
	{
		response = "Bad DNS1 IP:  " + dns1.str();
		error = 1;
		goto EXIT;
	}
	if (dns2.str() != "" && (n = dns2.str().find_first_not_of(IP_NUMBERS)) != std::string::npos) 
	{
		response = "Bad DNS2 IP:  " + dns2.str();
		error = 1;
		goto EXIT;
	}
	if (locip.str() == "")
	{
		response = "Couldn't open local ip file";
		goto EXIT;
	}
	
	if (!(varsfile = fopen("/etc/snort/vars", "w")))	
	{
		response = "Couldn't create vars file";
		error = 1;
		goto EXIT;
	}
	fputs("var HOME_NET ", varsfile);
	fputs(locip.str().c_str(), varsfile);
	fputs("\n", varsfile);

	if (dns1.str() != "")
	{
		if (dns2.str() != "")
		{
			fputs("var DNS_SERVERS [", varsfile);
			fputs(dns1.str().c_str(), varsfile);
			fputs(",", varsfile);
			fputs(dns2.str().c_str(), varsfile);
			fputs("]\n", varsfile);
		}
		else
		{
			fputs("var DNS_SERVERS ", varsfile);
			fputs(dns1.str().c_str(), varsfile);
			fputs("\n", varsfile);
		}
	}
	fclose(varsfile);
	
	if (settings["ENABLE_SNORT"] == "on")
	{
		error = simplesecuresysteml("/usr/bin/snort","-c", "/etc/snort.conf", "-D",
			"-u", "snort", "-g", "snort", "-d",  "-e", "-z", "-A", "Full",  "-i" , iface.str().c_str(), NULL);

		if (error)
		{
			response = "Snort FAILED!";
			error = 1;
			goto EXIT;
		}
	
		command = "/var/run/snort_" + iface.str() + ".pid";
		for (trys = 0; trys < 5; trys++)
		{
			sleep(2);
			fd =  open(command.c_str(), 0);
			if (fd >= 0)
				break;
		}
	
		if (fd != -1)
		{
			fchmod(fd, 00644);
			close(fd);
		}
	}
	
EXIT:
	return error;
}
	
