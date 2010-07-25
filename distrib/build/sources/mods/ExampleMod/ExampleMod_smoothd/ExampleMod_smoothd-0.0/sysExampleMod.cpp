/* SysIM Module for the SmoothWall SUIDaemon                              */
/* Contains functions relating to the management of the P3Scan            */
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

	int restart_ex(std::vector<std::string> & parameters, std::string & response);
	int   start_ex(std::vector<std::string> & parameters, std::string & response);
	int    stop_ex(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs) {
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair restart_ex_function("exrestart", "restart_ex", 0, 0);
	CommandFunctionPair   start_ex_function("exstart",     "start_ex", 0, 0);
	CommandFunctionPair    stop_ex_function("exstop",       "stop_ex", 0, 0);

	pairs.push_back(restart_ex_function);
	pairs.push_back(  start_ex_function);
	pairs.push_back(   stop_ex_function);

	return 0;
}

int restart_ex(std::vector<std::string> & parameters, std::string & response) {
	int error = 0;

	error += stop_ex(parameters, response);

	if (!error)
		error += start_ex(parameters, response);

	if (!error)
		response = "ExampleMod Restart Successful";

	return error;
}

int stop_ex(std::vector<std::string> & parameters, std::string & response) {
	std::vector<std::string>ipb;	
	response = "ExampleMod Process Terminated";
	
	killunknownprocess("ExampleMod");
	
	return 0;
}

int start_ex(std::vector<std::string> & parameters, std::string & response) {

	int exerror = 0;
	std::vector<std::string>ipb;

	response = "ExampleMod Process started";


	if (exerror) {
		response = "ExampleMod Start Failed!";
	} else {
		response = "ExampleMod Start Successful";
	}

	return 0;
}
