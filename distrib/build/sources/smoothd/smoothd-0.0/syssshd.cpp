/* SysSshd Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to starting/restarting sshd                  */
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
	int restart_sshd(std::vector<std::string> & parameters, std::string & response);
	int start_sshd(std::vector<std::string> & parameters, std::string & response);
	int stop_sshd(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair restart_sshd_function("sshdrestart", "restart_sshd", 0, 0);
	CommandFunctionPair start_sshd_function("sshdstart", "start_sshd", 0, 0);
	CommandFunctionPair stop_sshd_function("sshdstop", "stop_sshd", 0, 0);

	pairs.push_back(restart_sshd_function);
	pairs.push_back(start_sshd_function);
	pairs.push_back(stop_sshd_function);

	return 0;
}

int restart_sshd(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	
	error += stop_sshd(parameters, response);
	
	if (!error)
		error += start_sshd(parameters, response);
	
	if (!error)
		response = "Sshd Restart Successful";
	
	return error;
}

int stop_sshd(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	killprocess("/var/run/sshd.pid");
	sleep(1);
	response = "Sshd Process Terminated";

	return error;
}


int start_sshd(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	ConfigVAR settings("/var/smoothwall/remote/settings");
	
	if (settings["ENABLE_SSH"] == "on")
	{
		error = simplesecuresysteml("/usr/sbin/sshd", NULL);

		if (error)
			response = "Can't start sshd";
		else
			response = "Sshd Start Successful";
	}
	
	return error;
}
