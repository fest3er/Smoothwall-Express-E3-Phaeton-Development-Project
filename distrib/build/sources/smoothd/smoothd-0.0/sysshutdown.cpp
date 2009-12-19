/* SysShutdown Module for the SmoothWall SUIDaemon                        */
/* Contains functions relating to shutting down and rebooting             */
/* (c) 2004 SmoothWall Ltd                                                */
/* ---------------------------------------------------------------------- */
/* Original Author : D.K.Taylor                                           */

/* Based on "SmoothWall helper program - smoothiedeath/smoothierebirth    */
/*									  */
/* (c) Lawrence Manning, 2001						  */
/*									  */

/* include the usual headers.  iostream gives access to stderr (cerr)     */
/* module.h includes vectors and strings which are important              */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>

#include "module.h"
#include "ipbatch.h"
#include "setuid.h"

#include <sys/wait.h>

extern "C" {
	int load(std::vector<CommandFunctionPair> & );
	int  restart_smoothie(std::vector<std::string> & parameters, std::string & response);
	int shutdown_smoothie(std::vector<std::string> & parameters, std::string & response);
	int   cancel_shutdown(std::vector<std::string> & parameters, std::string & response);
	int shutdown_command(std::vector<std::string> & parameters, std::string & response, char action);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair  restart_smoothie_function("systemrestart",   "restart_smoothie", 0, 0);
	CommandFunctionPair shutdown_smoothie_function("systemshutdown", "shutdown_smoothie", 0, 0);
	CommandFunctionPair   cancel_shutdown_function("systemcancel",     "cancel_shutdown", 0, 0);

	pairs.push_back( restart_smoothie_function);
	pairs.push_back(shutdown_smoothie_function);
	pairs.push_back(  cancel_shutdown_function);

	return 0;
}

void signal_handler(int sig)
{
	return;
}

int shutdown_command(std::vector<std::string> & parameters, std::string & response, char action)
{
	/* the parameter given to this function should be a time */
	/* to wait before shutting down                          */

	int time_hours = -1;
	int time_to_wait = -1;

	if (parameters.size() >= 1)
	{
		const char * position = parameters[ 0 ].c_str();
	
		while (*position)
		{
			if (((*position) < '0' || (*position) > '9') && ((*position) != ':'))
			{
				response = "Invalid time identifier";
				return 1;
			} 
			else if ((*position) == ':')
				time_hours = 1;
			position++;
		}
	}

	if (parameters.size() >= 1) time_to_wait = safeatoi(parameters[ 0 ]);

	char shutdown_time[ 255 ];

	if (time_hours == 1){
		snprintf(shutdown_time, sizeof(shutdown_time) - 1, "%s", parameters[ 0 ].c_str());
	} else if (time_to_wait > 0){
		snprintf(shutdown_time, sizeof(shutdown_time) - 1, "+%d", time_to_wait);
	} else {
		snprintf(shutdown_time, sizeof(shutdown_time) - 1, "now");
	}	

	unlink("/var/state/smoothd/shutdown");
	simplesecuresysteml("/sbin/shutdown", "-c", NULL);

	if (action != 'c')
	{
		pid_t child;
	
		child = fork();
		char command[ 255 ];
		FILE * output = NULL;
		
		switch (child)
		{
			case -1:
				syslog(LOG_ERR, "Unable to create subprocess for shutdown sequence");
				return 1;
			case 0:
				syslog(LOG_ERR, "Spawning child process\n");
				switch (fork())
				{
					case -1:
						syslog(LOG_ERR, "Unable to create subprocess for shutdown sequence (2)");
						_exit(1);
						
					case 0:
						snprintf(command, sizeof(command) - 1, "-%c", action);
						output = fopen("/var/state/smoothd/shutdown", "w");

						if (output)
						{
							if (time_hours == 1)
								fprintf(output, "%s", shutdown_time);
							else
							{
								char tstr[ 256 ];
								time_t t;
								time(&t);
								t += (time_to_wait*60);
								strftime(tstr, 256, "%H:%M", localtime(&t));
								fprintf(output, "%s", tstr);
							}
							fclose(output);
						}

						syslog(LOG_ERR, "Scheduling Shutdown \"shutdown -%c %s\"", action, shutdown_time);
						simplesecuresysteml("/sbin/shutdown", command, shutdown_time, NULL);
						_exit(1);
						
					default:
						_exit(0);
				}
				
			default:
				waitpid(child, NULL, 0);
		}
	}

	response = "Shutdown Scheduled";

	return 0;
}

int restart_smoothie(std::vector<std::string> & parameters, std::string & response)
{
	return (shutdown_command(parameters, response, 'r'));
}
 
int shutdown_smoothie(std::vector<std::string> & parameters, std::string & response)
{
	return (shutdown_command(parameters, response, 'h'));
}

int cancel_shutdown(std::vector<std::string> & parameters, std::string & response)
{
	return (shutdown_command(parameters, response, 'c'));
}
