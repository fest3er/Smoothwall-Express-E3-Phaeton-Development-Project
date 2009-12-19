/* SysInstall Module for the SmoothWall SUIDaemon                         */
/* Contains functiosn relating to the installation system                 */
/* (c) 2006 SmoothWall Ltd                                                */
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

#include <sys/wait.h>

extern "C" {
	int load( std::vector<CommandFunctionPair> &  );
	int install(std::vector<std::string> & parameters, std::string & response);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair install_function("install", "install", 0, 0);
	pairs.push_back(install_function);

	return 0;
}

int install(std::vector<std::string> & parameters, std::string & response)
{
	response = "Installation complete";

	int error = 0;

	std::string pids = "0";
	
	for (std::vector<std::string>::iterator parameter = parameters.begin();
		parameter != parameters.end(); parameter++)
	{
		pids = *parameter;
	}

	if (pids == "0")
	{
		response = "Invalid Identifier";
		return 1;
	}
	
	char temp[STRING_SIZE];
	int fd = 0;
	FILE *hpipe = NULL;
	char buffer[STRING_SIZE];
	pid_t pid = 0;
	int status = 0;
	std::string output;

	memset(temp, 0, STRING_SIZE);
	memset(buffer, 0, STRING_SIZE);
	
	snprintf(temp, STRING_SIZE - 1, "/var/patches/%s/", pids.c_str());
	chdir(temp);
	snprintf(temp, STRING_SIZE - 1, "/var/patches/%s/setup", pids.c_str());

	if ((fd = simplesecurepopenl(0, &pid, temp, NULL)) == -1)
	{
		response = "Unable to execute Setup process";
		error = 2;
		goto EXIT;
	}

	if ((hpipe = fdopen(fd, "r")) == NULL)
	{
		response = "Unable to execute Setup process";
		error = 3;
		goto EXIT;
	}

	while (fgets(buffer, STRING_SIZE, hpipe))
	{
		output = "install: ";
		output += buffer;
		syslog(LOG_ERR, output.c_str());
	}
				
	fclose(hpipe);
	
	waitpid(pid, &status, 0);

	if (WIFEXITED(status))
	{
		if ((error = WEXITSTATUS(status)))
			response = "Setup process returned an usual response";
	} 
	else
	{
		response = "Unable to execute Setup process";
		error = 2;
	}	

EXIT:
	snprintf(temp, STRING_SIZE - 1, "/var/patches/%s", pids.c_str());
	simplesecuresysteml("/bin/rm", "-rf", temp, NULL);
	
	return error;
}

