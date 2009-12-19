/* SysRouting Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to the management of bird	       */
/* (c) 2004 SmoothWall Ltd                                                */
/* ---------------------------------------------------------------------- */
/* Original Author : D.K.Taylor                                           */

/* Based on "SmoothWall helper program - restartsquid                    */
/*									  */
/* (c) Lawrence Manning, 2001						  */
/*									  */


/* include the usual headers.  iostream gives access to stderr (cerr)     */
/* module.h includes vectors and strings which are important              */
#include <iostream>
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>

#include "module.h"
#include "ipbatch.h"
#include "setuid.h"

extern "C" {
	int load( std::vector<CommandFunctionPair> &  );

	int restart_sip( std::vector<std::string> & parameters, std::string & response );
	int   start_sip( std::vector<std::string> & parameters, std::string & response );
	int    stop_sip( std::vector<std::string> & parameters, std::string & response );
}

int load( std::vector<CommandFunctionPair> & pairs )
{
	/* CommandFunctionPair name( "command", "function" ); */
	CommandFunctionPair restart_sip_function( "siprestart", "restart_sip", 0, 0 );
	CommandFunctionPair   start_sip_function( "sipstart",     "start_sip", 0, 0 );
	CommandFunctionPair    stop_sip_function( "sipstop",       "stop_sip", 0, 0 );

	pairs.push_back( restart_sip_function );
	pairs.push_back(   start_sip_function );
	pairs.push_back(    stop_sip_function );

	return ( 0 );
}

int restart_sip( std::vector<std::string> & parameters, std::string & response )
{
	int error = 0;
	
	error += stop_sip( parameters, response );

	if ( !error )
		error += start_sip( parameters, response );
	
	if ( !error )
		response = "SIP Restart Successful";

	return error;
}

int stop_sip( std::vector<std::string> & parameters, std::string & response )
{
	int error = 0;
	std::vector<std::string>ipbnat;
	std::vector<std::string>ipbfilter;

	response = "siproxd Process Terminated";
	killprocess("/var/run/siproxd.pid");

	ipbnat.push_back("iptables -t nat -F sip");
	ipbfilter.push_back("iptables -t filter -F siprtpports");

	error = ipbatch(ipbnat);
	if (error)
	{
		response = "ipbatch failure (nat)";
		goto EXIT;
	}

	error = ipbatch(ipbfilter);
	if (error)
	{
		response = "ipbatch failure (filter)";
		goto EXIT;
	}

EXIT:
	return error;
}
	
int start_sip( std::vector<std::string> & parameters, std::string & response )
{
	int error = 0;
	std::vector<std::string>ipbnat;
	std::vector<std::string>ipbfilter;

	response = "SIP Process started";
	stop_sip( parameters, response );

	ipbnat.push_back("iptables -t nat -F sip");
	ipbfilter.push_back("iptables -t filter -F siprtpports");

	ConfigVAR settings("/var/smoothwall/sipproxy/settings");
	
	int clients = atol(settings["CLIENTS"].c_str());
	
	if (!clients) clients = 50;

	int highport = 7070 + clients;
	
	syslog(LOG_ERR, "starting accordingly");
	if (settings["ENABLE"] == "on")
	{
		syslog(LOG_ERR, "SIP enabled, starting accordingly");
		simplesecuresysteml("/usr/sbin/siproxd", NULL);

		ipbfilter.push_back(stringprintf("iptables -t filter -A siprtpports -p udp --destination-port 7070:%d -j ACCEPT", highport));

		if (settings["TRANSPARENT"] == "on")
		{
			ipbnat.push_back("iptables -t nat -A sip -p udp --destination-port 5060 -j REDIRECT");
		}
	}
		
	error = ipbatch(ipbnat);
	if (error)
	{
		response = "ipbatch failure (nat)";
		goto EXIT;
	}

	error = ipbatch(ipbfilter);
	if (error)
	{
		response = "ipbatch failure (filter)";
		goto EXIT;
	}

EXIT:
	return error;
}
