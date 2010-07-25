/* SysTimedAccess Module for the SmoothWall SUIDaemon                      */
/* For bringing external interface up or taking it down                    */
/* (c) 2007,2008 SmoothWall Ltd and Steven L. Pittman                      */
/* ----------------------------------------------------------------------  */
/* Original Author  : Lawrence Manning                                     */
/* Translated to C++: M. W. Houston                                        */
/* Refactored by    : Steven L. Pittman                                    */
/* Modified to add logging of rejected packets                             */
/* 09-07-08         : Steven L. Pittman                                    */

/* include the usual headers.  iostream gives access to stderr (cerr)      */
/* module.h includes vectors and strings which are important               */
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
	int timed_access(std::vector<std::string> & parameters, std::string & response);
	int set_timed_access(std::vector<std::string> & parameters, std::string & response);
	
	bool indaterange(ConfigVAR &settings);
	int setallowed(bool allowed, bool logging);
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair timed_access_function(1, "timed_access", 0, 4);
	CommandFunctionPair set_timed_access_function("settimedaccess", "set_timed_access", 0, 4);
	
	pairs.push_back(timed_access_function);
	pairs.push_back(set_timed_access_function);
	
	return (0);
}

int timed_access(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
   bool logging = false;
	static bool modeset = true;
	static bool setmode = true;
	static bool firstset = false;

	ConfigVAR settings("/var/smoothwall/timedaccess/settings");

	std::string mode = settings["MODE"];

	if (settings["LOGGING"] == "on") logging = true;
	else logging = false;

	if (settings["ENABLE"] != "on")
	{
		setmode = true;
	}
	else
	{
		/* Check the schedule, determine current status */
		setmode = indaterange(settings);
		/* Complement for REJECT mode */
		if (mode == "REJECT") setmode = !setmode;
	}

	/* If not set correctly, or current setting unknown, set now */
	if (modeset != setmode || !firstset)
	{
		error = setallowed(setmode, logging);
		if (!error) {
			modeset = setmode;
			firstset = true;
		}
	}

	if (error) {
		response = "Error when setting chain timedaction";
		syslog(LOG_INFO, "Timed access: %s", response.c_str());
	} else {
		response = "Timed Access mode set";
	}

	return error;
}

int set_timed_access(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	ConfigCSV config("/var/smoothwall/timedaccess/machines");
	std::vector<std::string>ipb;
	std::string::size_type n;
	
	ipb.push_back("iptables -F timedaccess\n");

	for (int line = config.first(); line == 0; line = config.next())
	{
		const std::string &ip = config[0];
		
		if ((n = ip.find_first_not_of(IP_NUMBERS)) != std::string::npos) 
		{
			// illegal characters
			response = "Bad IP: " + ip;
			error = 1;
			return error;
		}
		
		ipb.push_back("iptables -A timedaccess -s " + ip + " -j timedaction");
		ipb.push_back("iptables -A timedaccess -d " + ip + " -j timedaction");
	}
	
	error = ipbatch(ipb);
	if (error) response = "ipbatch failure when setting chain timedaccess";
	else response = "timed access set";

	return error;
}

bool indaterange(ConfigVAR &settings)
{
	unsigned int starthour = atol(settings["START_HOUR"].c_str());
	unsigned int endhour = atol(settings["END_HOUR"].c_str());
	unsigned int startmin = atol(settings["START_MIN"].c_str());
	unsigned int endmin = atol(settings["END_MIN"].c_str());

	time_t tnow;  // to hold the result from time()
	struct tm *tmnow;  // to hold the result from localtime()
	unsigned int hour, min, wday;

	time(&tnow);  // get the time after the lock so all entries in order
	tmnow = localtime(&tnow);  // convert to local time (BST, etc)

	hour = tmnow->tm_hour;
	min = tmnow->tm_min;
	wday = tmnow->tm_wday;

	/* Wrap week to start on Monday. */
	if (wday == 0) wday = 7;
	wday--;
	
	bool matchday = false;

	std::string key = "DAY_" + stringprintf("%d", wday);

	if (settings[key.c_str()] == "on") matchday = true;

	if (!matchday) return false;
	if (hour < starthour || hour > endhour) return false;
	if (hour == starthour && min < startmin) return false;
	if (hour == endhour && min >= endmin) return false;
	return true;

}

int setallowed(bool allowed, bool logging)
{
	std::vector<std::string> ipb;
	int error = 0;

	if (allowed)
	{
		syslog(LOG_INFO, "Timed access: Ok, allowing");
		ipb.push_back("iptables -R timedaction 1 -j RETURN");
	}
	else
	{
		// Rule number 2 in timedaccess is set to REJECT by rc.firewall.up and is static
		syslog(LOG_INFO, "Timed access: Not allowing");
	   	if (!logging)
	   	{
	   		ipb.push_back("iptables -R timedaction 1 -j REJECT");
		}
		else
		{
			ipb.push_back("iptables -R timedaction 1 -j LOG --log-prefix -Denied-by-Timed-Access:-");
		}
	}
	
	error = ipbatch(ipb);

	return error;
}
