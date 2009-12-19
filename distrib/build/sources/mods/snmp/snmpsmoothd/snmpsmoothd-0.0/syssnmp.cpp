/* SysRouting Module for the SmoothWall SUIDaemon                           */
/* Contains functions relating to the management of SNMP	       */
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
#include <smoothwall/module.h>

extern "C" {
	int load( std::vector<CommandFunctionPair> &  );

	int restart_snmp( std::vector<std::string> & parameters, std::string & response );
	int   start_snmp( std::vector<std::string> & parameters, std::string & response );
	int    stop_snmp( std::vector<std::string> & parameters, std::string & response );
}

int load( std::vector<CommandFunctionPair> & pairs )
{
	/* CommandFunctionPair name( "command", "function" ); */
	CommandFunctionPair restart_snmp_function( "snmprestart", "restart_snmp", 0, 0 );
	CommandFunctionPair   start_snmp_function( "snmpstart",     "start_snmp", 0, 0 );
	CommandFunctionPair    stop_snmp_function( "snmpstop",       "stop_snmp", 0, 0 );

	pairs.push_back( restart_snmp_function );
	pairs.push_back(   start_snmp_function );
	pairs.push_back(    stop_snmp_function );

	return ( 0 );
}

int restart_snmp( std::vector<std::string> & parameters, std::string & response )
{
	int error = 0;
	
	error += stop_snmp( parameters, response );

	if ( !error )
		error += start_snmp( parameters, response );
	
	if ( !error )
		response = "SNMP restart successful";

	return error;
}
	

int stop_snmp( std::vector<std::string> & parameters, std::string & response )
{
	response = "SNMP rocess terminated";
	killprocess("/var/run/snmpd.pid");
	return 0;
}
	
int start_snmp( std::vector<std::string> & parameters, std::string & response )
{
	response = "SNMP process started";
	stop_snmp( parameters, response );
	
	ConfigVAR settings("/var/smoothwall/snmp/settings");
	
	syslog(LOG_ERR, "starting accordingly");
	if (settings["ENABLE"] == "on")
	{
		std::vector<std::string> args;

		args.push_back("/usr/sbin/snmpd");
		args.push_back("-p");
		args.push_back("/var/run/snmpd.pid");
		args.push_back("-Ls");
		args.push_back("daemon");
		args.push_back("-u");
		args.push_back("99");
		args.push_back("-g");
		args.push_back("99");

//		ConfigCSV ipcsv("/var/smoothwall/red/local-ipaddress");
//		std::string ip = ipcsv[0];

//		if (ip != "")
//			args.push_back(ip);

		syslog(LOG_ERR, "SNMP enabled, starting accordingly");
		simplesecuresystemvector(args);
	}

	return 0;
}
