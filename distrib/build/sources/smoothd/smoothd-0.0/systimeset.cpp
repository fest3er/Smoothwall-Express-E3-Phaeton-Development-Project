/* SysTimeSet Module for the SmoothWall SUIDaemon                           */
/* For setting the system time                                              */
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
	int set_time(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair set_time_function("settime", "set_time", 0, 0);
	
	pairs.push_back(set_time_function);

	return (0);
}

int set_time(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	const std::string & time = parameters[0];
	const std::string & date = parameters[1];
	std::vector<std::string> cmd;
	std::string::size_type n;
	
	cmd.push_back("/bin/date");
	cmd.push_back("-s");

	// check that we have a time and a date - that they only contain right characters

	if (time == "" || ((n = time.find_first_not_of(TIME_NUMBERS)) != std::string::npos))
	{
		response = "time parameter bad: " + time;
		error = 1;
		goto EXIT;
	}
	
	if (date == "" || ((n = date.find_first_not_of(DATE_NUMBERS)) != std::string::npos))
	{
		response = "date parameter bad: " + date;
		error = 1;
		goto EXIT;
	}
	
	cmd.push_back(time + " " + date);
	error = simplesecuresystemvector(cmd);

	if (error)
		response = "setting time failed";
	else
		response = "System time set to " + time + " " + date;

EXIT:
	return error;
}
