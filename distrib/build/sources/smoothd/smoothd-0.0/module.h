/* Skelington system for a "module" and the module list structure */
/* (c) 2004 SmoothWall ltd                                        */
/* Original Author: D.K.Taylor                                    */

#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <dlfcn.h>
#include "setuid.h"
#include "config.h"

#define NOBODY 99

typedef int (*MFunction)( std::vector<std::string> &, std::string & );

class CommandFunctionPair {
public:
	CommandFunctionPair( const CommandFunctionPair & a ) { command = a.command; function = a.function; user_id = a.user_id; group_id = a.group_id; version = a.version; timeout = a.timeout; owner = a.owner; logging = a.logging; };
	CommandFunctionPair( std::string nCommand, std::string nFunction, int uid = NOBODY, int gid = NOBODY, int vers = 1, std::string nowner = "nobody", int nlogging = 2 ) { command = nCommand; function = nFunction;user_id = uid; group_id = gid; version = vers; timeout = -1; owner = nowner; logging = nlogging; };
	CommandFunctionPair( int nTimeout, std::string nFunction, int uid = NOBODY, int gid = NOBODY, int vers = 1 ) { command = ""; function = nFunction;user_id = uid; group_id = gid; version = vers; timeout = nTimeout;};
	std::string command;
	std::string function;
	uid_t user_id;
	gid_t group_id;
	int version;
        int timeout;
	std::string owner;
	int logging;
};

class ModuleFunction {
public:
	ModuleFunction();
	ModuleFunction( std::string nName, void * nHandle, int nTimeout, uid_t uid = NOBODY, gid_t gid = NOBODY, int ver = 0, std::string nowner = "nobody", int logginglevel = 2 ); 
	ModuleFunction( const ModuleFunction & a );
	MFunction function;	/* the function */
	void * handle;		/* the handle to it */
	std::string name;
	uid_t user_id;
	gid_t group_id;
	int version;
        int timeout;
	std::string owner;
	int logging;		/* the level of logging to perform 0 = none, 1 = minimal, 2 (default) = verbose */
};

struct eqstr
{
	bool operator()( std::string s1, std::string s2) const
  	{
		return strcmp(s1.c_str(), s2.c_str()) < 0;
  	}
};

typedef std::map<std::string, ModuleFunction, eqstr> ModuleMap; 

class ModuleReg {
public:
	ModuleReg() { name = ""; handle = NULL; }
	ModuleReg( const char * nname, void * nhandle ){ name = nname; handle = nhandle; }
	ModuleReg( const ModuleReg & a ){ name = a.name; handle = a.handle; }
	std::string name;
	void * handle;
};
		

int register_module( std::vector<ModuleReg> & modules, ModuleMap & functions, std::vector<ModuleFunction> & timedfunctions, const char * moduleName );

/* because asking for the status of a PID is reasonably common, provide some */
/* standard methods for doing so                                             */

std::string status( const char * pid_file );
std::string status( const int pid );

int killprocess( const char * pid_file );
int killprocess( int pid ) ;
int killunknownprocess( const char * processname );

int signalprocess( const char * pid_file, int signal = 1 );
int signalunknownprocess( const char * processname, int signal = 1 );

int ipisrange(std::string ip);
std::string getsrcfragment(std::string ip);
std::string getdstfragment(std::string ip);
