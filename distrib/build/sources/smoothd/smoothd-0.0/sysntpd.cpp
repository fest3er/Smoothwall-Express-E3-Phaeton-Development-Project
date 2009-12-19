/* SysNTPD Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to starting/restarting ntp daemon         */
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
	int load( std::vector<CommandFunctionPair> &  );
	int restart_ntpd( std::vector<std::string> & parameters, std::string & response );
	int start_ntpd( std::vector<std::string> & parameters, std::string & response );
	int stop_ntpd( std::vector<std::string> & parameters, std::string & response );
}

int load( std::vector<CommandFunctionPair> & pairs )
{
	/* CommandFunctionPair name( "command", "function" ); */
	CommandFunctionPair restart_ntpd_function("ntpdrestart", "restart_ntpd", 0, 0 );
	CommandFunctionPair start_ntpd_function("ntpdstart", "start_ntpd", 0, 0 );
	CommandFunctionPair stop_ntpd_function("ntpdstop", "stop_ntpd", 0, 0 );

	pairs.push_back(restart_ntpd_function );
	pairs.push_back(start_ntpd_function );
	pairs.push_back(stop_ntpd_function );

	return 0;
}

int restart_ntpd( std::vector<std::string> & parameters, std::string & response )
{
	int error = 0;
	
	error += stop_ntpd( parameters, response );
	
	if (!error)
		error += start_ntpd( parameters, response );
	
	if (!error)
		response = "NTPD Restart Successful";
	
	return error;
}

int stop_ntpd( std::vector<std::string> & parameters, std::string & response )
{
	int error = 0;

	killprocess("/var/run/ntpd.pid");
	unlink("/var/run/ntpd.pid");
	response = "NTPD Process Terminated";

	return error;
}


int start_ntpd( std::vector<std::string> & parameters, std::string & response )
{
	ConfigVAR settings("/var/smoothwall/time/settings");
	int error = 0;

	if (settings["NTPD"] == "on")
	{
		error = simplesecuresysteml("/usr/sbin/ntpd", NULL);
		
		if (error)
			response = "NTPD Start failed!";
		else
			response = "NTPD Start Successful";
	}
		
	return error;
}
	
