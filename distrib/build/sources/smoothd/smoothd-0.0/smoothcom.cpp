#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include "setuid.h"

int main(int argsc, char* argsv[])
{
	openlog("smoothcom", 0, LOG_USER);
	
	int firstParam = 1;
	
	// check for -q
	if (argsc > 1)
	{
		if (strcmp(argsv[1], "-q") == 0)
		{
			// The user wants quiet mode but there is only a 
			// quiet mode so we don't do anything.
			firstParam++;
		}
	}
	
	std::string messageString;
	for (int i = firstParam; i < argsc; i++)
	{
		messageString += argsv[i];
		if (i < (argsc - 1))
			messageString += " ";
	}

	// send message
	std::string success = message(messageString);
	int found = success.find( "Error:", 0 );

	// syslog(LOG_ERR, "'%s' response: '%s', %d", messageString.c_str(), success.c_str(), found);

	closelog();
	
	if (success == "" )
		return 1;
	else if (found >= 0)
		return 2;
	else
		return 0;
}
