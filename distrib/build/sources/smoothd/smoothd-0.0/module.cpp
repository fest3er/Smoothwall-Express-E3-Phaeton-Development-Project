/* Skelington system for a "module" and the module list structure */
/* (c) 2004 SmoothWall ltd                                        */
/* Original Author: D.K.Taylor                                    */

#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <dlfcn.h>
#include "module.h"
#include <iostream>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

ModuleFunction::ModuleFunction()
{
	name = "";
	user_id = 0;
	group_id = 0;
	version = 0;
        timeout = -1;
	owner = "nobody";
	logging = 2;
	function = NULL;
}


ModuleFunction::ModuleFunction( std::string nName, void * nHandle, int nTimeout, uid_t uid, gid_t gid, int ver, std::string nowner, int logginglevel )
{
	name = nName;
	handle = nHandle;
	version = ver;
	user_id = uid;
	group_id = gid;
        timeout = nTimeout;
	owner   = nowner;
	logging = logginglevel;
	function = (MFunction) dlsym( handle, name.c_str() );
	if ( !function ){
		handle = NULL;
		name   = "";
		user_id = 0;
		group_id = 0;
		version = 0;
	        timeout = -1;
	}
}

ModuleFunction::ModuleFunction( const ModuleFunction & a ){ 
	function = a.function; 
	handle = a.handle; 
	name = a.name;
	user_id = a.user_id;
	group_id = a.group_id;
	version = a.version;
        timeout = a.timeout;
	owner = a.owner;
	logging = a.logging;
};

int register_module( std::vector<ModuleReg> & modules, ModuleMap & functions, std::vector<ModuleFunction> & timedfunctions, const char * moduleName )
{
	/* open the module with the name moduleName */

	/* load the module (if possible) */
	void * handle = NULL;
	char * error  = NULL;

	if ( moduleName == NULL ){
		syslog( LOG_ERR, "Unable to load Module, no module name given" );
		return 0;
	}

	handle = dlopen( moduleName, RTLD_LAZY );

	if ( handle == NULL ){
		syslog( LOG_ERR, "Unable to load Module %s : %s", moduleName, dlerror() );
		return 0;
	}

	error = dlerror(); 	/* clear any possible error handler */
	
	/* now call the register_module function */
	
	int ( *registerFunction )( std::vector<CommandFunctionPair> & );

	registerFunction = ( int (*)( std::vector<CommandFunctionPair> &) ) dlsym( handle, "load" );
	
	if ( (error = dlerror()) != NULL)  {
		syslog( LOG_ERR, "Unable to load Module %s : Module lacks load method.", moduleName );
		return 0;
	}

	ModuleReg newModule( moduleName, handle );
	modules.push_back( newModule );
	
	std::vector<CommandFunctionPair> pairs;
	
	(*registerFunction)( pairs );

	for ( std::vector<CommandFunctionPair>::iterator funcref = pairs.begin();
			funcref != pairs.end();
			funcref++ ){
	
		/* create the appropriate structure to place this information in */
		ModuleFunction newFunction( funcref->function, handle, funcref->timeout, funcref->user_id, funcref->group_id, funcref->version, funcref->owner, funcref->logging);
	 
	        if ( newFunction.timeout != -1 ){
			syslog( LOG_INFO, "    Registering timed function : %s:%d", funcref->command.c_str(), newFunction.timeout );
			timedfunctions.push_back( newFunction );
			continue;
                }
		
	        syslog( LOG_INFO, "    Registering command : %s", funcref->command.c_str() );

		if ( newFunction.version == 0 ){
			syslog( LOG_ERR, "Unable to determine action for command \"%s\", skipping definition", funcref->command.c_str() );
			continue;
		}

		if ( functions[ funcref->command ].version >= newFunction.version ){
			syslog( LOG_WARNING, "    Command \"%s\" already exists and is of later version \"%d\" (thisver:%d)", funcref->command.c_str(), functions[ funcref->command ].version, newFunction.version );
			continue;
		} else if ( functions[ funcref->command ].version < newFunction.version && functions[ funcref->command ].version > 0 ){
			syslog( LOG_WARNING, "    Command \"%s\" is already defined but of an earlier version (%d), using this one instead (thisver:%d)", funcref->command.c_str(), functions[ funcref->command ].version, newFunction.version );
		}
		
		functions[ funcref->command ] = newFunction;
	}	
	return 1;
}

std::string status( const char * pid_file )
{
	int fd = -1;
	char buffer[STRING_SIZE];
	int pid = 0;
	
	memset(buffer, 0, STRING_SIZE);
	
	if ((fd = open( pid_file, O_RDONLY)) == -1)
	{
		return "Stopped";
	}
	if (read(fd, buffer, STRING_SIZE - 1) == -1)
	{
		return "Errored";
	}
	pid = atoi(buffer);
	if (pid <= 1)
	{
		return "Errored";
	}

	return status( pid );
}

std::string status( const int pid )
{
	if ( kill(pid, 0) == -1)
	{
		return "Stopped";
	}
	return "Running";
}

int killprocess( const char * pid_file )
{
	FILE *file = NULL;
	char buffer[STRING_SIZE];
	int pid = 0;
	
	memset(buffer, 0, STRING_SIZE);
	
	if ((file = fopen(pid_file, "r")) == NULL)
	{
		return -1;
	}
	if (fgets(buffer, STRING_SIZE - 1, file) == NULL)
	{
		fclose(file);
		return -2;
	}
	stripnl(buffer);
	pid = safeatoi(buffer);
	if (pid <= 1)
	{
		fclose(file);
		return -3;
	}

	fclose(file);

	int error = killprocess( pid );
	unlink( pid_file );
	return( error );
}

int killprocess( int pid )
{
	int tries = 0;
	
	while( kill( pid, 15 ) == 0 ){
		sleep( 1 );
		
		if ( tries++ > 3 )
		{		
			syslog(LOG_ERR, "Unable to kill pid %d normally, resorting to SIGKILL", pid);
			
			int error = kill( pid, 9 );
			sleep( 2 );
			return ( error );	
		}
	}
	return ( 0 );
}
		
int killunknownprocess( const char * processname )
{
	int trys = 0;
	trys = 0;
	while (!simplesecuresysteml("/usr/bin/killall", processname, NULL))
	{
		sleep(1);
		if (trys++ > 3)
		{
			int error = simplesecuresysteml("/usr/bin/killall", "-9", processname, NULL);
			sleep(2);
			return( error );
		}
	}
	return 0;
}

int signalprocess( const char * pid_file, int signal )
{
	FILE *file = NULL;
	char buffer[STRING_SIZE];
	int pid = 0;
	
	memset(buffer, 0, STRING_SIZE);
	
	if ((file = fopen(pid_file, "r")) == NULL)
	{
		return -1;
	}
	if (fgets(buffer, STRING_SIZE - 1, file) == NULL)
	{
		fclose(file);
		return -2;
	}
	stripnl(buffer);
	pid = safeatoi(buffer);
	if (pid <= 1)
	{
		fclose(file);
		return -3;
	}

	fclose(file);

	int error = kill( pid, signal );
	return( error );
}

int signalunknownprocess( const char * processname, int signal )
{
	int trys = 0;
	trys = 0;
	char signal_text[ 255 ];
	snprintf( signal_text, sizeof( signal_text ), "-%d", signal );

	return (simplesecuresysteml("/usr/bin/killall", signal_text, processname, NULL));
}

int ipisrange(std::string ip)
{
	if (strchr(ip.c_str(), '-'))
		return 1;
	else
		return 0;
}

std::string getsrcfragment(std::string ip)
{
	std::string ret;

	if (ipisrange(ip))
		ret = "--src-range " + ip;
	else
		ret = "-s " + ip;

	return ret;
}

std::string getdstfragment(std::string ip)
{
	std::string ret;

	if (ipisrange(ip))
		ret = "--dst-range " + ip;
	else
		ret = "-d " + ip;

	return ret;
}
