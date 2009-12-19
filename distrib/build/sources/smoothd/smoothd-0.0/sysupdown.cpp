/* SysUpDown Module for the SmoothWall SUIDaemon                           */
/* For bringing external interface up or taking it down                    */
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
	int set_updown(std::vector<std::string> & parameters, std::string & response);
	int cycle_networking(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair set_updown_function("updown", "set_updown", 0, 0);
	CommandFunctionPair cycle_networking_function("cyclenetworking", "cycle_networking", 0, 0);
	
	pairs.push_back(set_updown_function);
	pairs.push_back(cycle_networking_function);

	return (0);
}

int set_updown(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	const std::string & choice = parameters[0];

	if (choice == "UP")
		error = simplesecuresysteml("/usr/bin/smoothwall/ppp-on", NULL);
	else
		error = simplesecuresysteml("/usr/bin/smoothwall/ppp-off", NULL);

	if (error)
		response = std::string(choice == "UP" ? "ppp-on" : "ppp-off") + " failed";
	else
		response = std::string(choice == "UP" ? "updown started" : "updown stopped");

	return error;
}

int cycle_networking(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	
	error = simplesecuresysteml("/etc/rc.d/rc.netaddress.down", NULL);
	if (error)
	{
		response = "Unable to take down networking";
		return error;
	}
	
	error = simplesecuresysteml("/etc/rc.d/rc.netaddress.up", NULL);
	if (error)
	{
		response = "Unable to bring up networking";
		return error;
	}
	
	return error;
}
