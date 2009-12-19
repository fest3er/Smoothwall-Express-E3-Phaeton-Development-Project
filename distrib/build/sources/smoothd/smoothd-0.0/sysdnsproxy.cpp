/* SysDNSProxy Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to starting/restarting dns proxy              */
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
	int load(std::vector<CommandFunctionPair> &  );
	int restart_dnsproxy(std::vector<std::string> & parameters, std::string & response );
	int start_dnsproxy(std::vector<std::string> & parameters, std::string & response );
	int stop_dnsproxy(std::vector<std::string> & parameters, std::string & response );
	int hup_dnsproxy(std::vector<std::string> & parameters, std::string & response );
}

int load(std::vector<CommandFunctionPair> & pairs )
{
	/* CommandFunctionPair name("command", "function" ); */
	CommandFunctionPair restart_dnsproxy_function("dnsproxyrestart", "restart_dnsproxy", 0, 0 );
	CommandFunctionPair start_dnsproxy_function("dnsproxystart", "start_dnsproxy", 0, 0 );
	CommandFunctionPair stop_dnsproxy_function("dnsproxystop", "stop_dnsproxy", 0, 0 );
	CommandFunctionPair hup_dnsproxy_function("dnsproxyhup", "hup_dnsproxy", 0, 0 );

	pairs.push_back(restart_dnsproxy_function );
	pairs.push_back(start_dnsproxy_function );
	pairs.push_back(stop_dnsproxy_function );
	pairs.push_back(hup_dnsproxy_function );

	return 0;
}

int restart_dnsproxy(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	
	error += stop_dnsproxy(parameters, response);
	
	if (!error)
		error += start_dnsproxy(parameters, response);
	
	if (!error)
		response = "DNS Proxy Restart Successful";
	
	return error;
}

int stop_dnsproxy(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	
	killunknownprocess("dnsmasq");
	sleep(1);

	response = "DNS Proxy Terminated";

	return error;
}

int start_dnsproxy(std::vector<std::string> & parameters, std::string & response)
{
	FILE *resolvfile = NULL;
	int error = 0;

	if (!(resolvfile = fopen("/etc/resolv.conf.dnsmasq", "w")))
	{
		response = "Couldn't write to /etc/resolv.conf.dnsmasq";
		error = 1;
		goto EXIT;
	}
	
	for (std::vector<std::string>::iterator i = parameters.begin();
		i != parameters.end(); i++)
	{
		if ((*i).find_first_not_of(IP_NUMBERS) != std::string::npos)
			syslog(LOG_ERR, "Invalid DNS server IP %s", (*i).c_str());
			fputs("nameserver ", resolvfile);
		fputs((*i).c_str(), resolvfile);
		fputs("\n", resolvfile);
	}
	fclose(resolvfile);

	error = simplesecuresysteml("/usr/bin/dnsmasq", "-r", "/etc/resolv.conf.dnsmasq", NULL);
	
	if (error)
		response = "DNS Proxy Start failure";
	else
		response = "DNS Proxy Start Successful";

EXIT:
	return error;
}

int hup_dnsproxy(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	
	error = signalunknownprocess("dnsmasq", SIGHUP);

	if (error)
		response = "DNS Proxy HUP failure";
	else
		response = "DNS Proxy HUP successful";

	return error;
}
