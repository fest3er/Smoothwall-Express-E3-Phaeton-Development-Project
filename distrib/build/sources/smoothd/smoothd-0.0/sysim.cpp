/* SysIM Module for the SmoothWall SUIDaemon                              */
/* Contains functions relating to the management of the IMspector         */
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

	int restart_im(std::vector<std::string> & parameters, std::string & response);
	int   start_im(std::vector<std::string> & parameters, std::string & response);
	int    stop_im(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair restart_im_function("imrestart", "restart_im", 0, 0);
	CommandFunctionPair   start_im_function("imstart",     "start_im", 0, 0);
	CommandFunctionPair    stop_im_function("imstop",       "stop_im", 0, 0);

	pairs.push_back(restart_im_function);
	pairs.push_back(  start_im_function);
	pairs.push_back(   stop_im_function);

	return 0;
}

int restart_im(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	error += stop_im(parameters, response);

	if (!error)
		error += start_im(parameters, response);

	if (!error)
		response = "IMSpector Restart Successful";

	return error;
}


int stop_im(std::vector<std::string> & parameters, std::string & response)
{	
	std::vector<std::string>ipb;
	response = "IMSpector Process Terminated";

	killprocess("/var/run/imspector.pid");
	ipb.push_back("iptables -t nat -F im");
	ipbatch(ipb);

	return 0;
}

int start_im(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	std::vector<std::string>ipb;

	response = "IMSpector Process started";

	ConfigVAR settings("/var/smoothwall/im/settings");

	ipb.push_back("iptables -t nat -F im");
	
	if (settings["ENABLE"] == "on")
	{
		if (settings["MSN"] == "on")
			ipb.push_back("iptables -t nat -A im -p tcp --dport 1863 -j REDIRECT --to-ports 16667");
		if (settings["ICQ"] == "on")
			ipb.push_back("iptables -t nat -A im -p tcp --dport 5190 -j REDIRECT --to-ports 16667");
		if (settings["YAHOO"] == "on")
			ipb.push_back("iptables -t nat -A im -p tcp --dport 5050 -j REDIRECT --to-ports 16667");
		if (settings["IRC"] == "on")
			ipb.push_back("iptables -t nat -A im -p tcp --dport 6667 -j REDIRECT --to-ports 16667");
		if (settings["GG"] == "on")
			ipb.push_back("iptables -t nat -A im -p tcp --dport 8074 -j REDIRECT --to-ports 16667");
		if (settings["XMPP"] == "on")
			ipb.push_back("iptables -t nat -A im -p tcp --dport 5222 -j REDIRECT --to-ports 16667");
		if (settings["XMPP"] == "on")
			ipb.push_back("iptables -t nat -A im -p tcp --dport 5223 -j REDIRECT --to-ports 16667");
	}

	error = ipbatch(ipb);

	if (error)
	{
		response = "ipbatch failure";
		goto EXIT;
	}

	if (settings["ENABLE"] == "on")
	{
		error = simplesecuresysteml("/usr/sbin/imspector", "-c", 
			"/var/smoothwall/im/imspector.conf", NULL);

		if (error)
			response = "IMSpector Start Failed!";
		else
			response = "IMSpector Start Successful";
	}

EXIT:
	return error;
}
