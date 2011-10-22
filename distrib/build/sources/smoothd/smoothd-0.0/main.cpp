/* Set UID Demon System                                                      */
/* (c) 2004 SmoothWall Ltd                                                   */
/* ------------------------------------------------------------------------- */
/* Original Author: D.K.Taylor                                               */

/* The usual culprits */

#include <iostream>
#include <syslog.h>

/* other bits of code */

#include "module.h"
#include "UDSocket.hpp"
#include "pid.h"

/* SetUID functions */
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <cerrno>

#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>

// configuration details

#define SOCKET       "/dev/smoothd"	/*FIXME*/
#define IDENT        "smoothd"
#define BINARY       "smoothd"
#define PID          "/var/run/smoothd.pid"
#define MODULESUBDIR "/usr/lib/smoothd/"

volatile int abort_now = 0;
int master_pid;
volatile pid_t master_pgid = 7;

/* shutdown is used to signal a reload */
static int dshutdown = 0;

extern "C" {
	/* The signal handler called by a "client" connection the "child" it is */
	/* awaiting the death of is the end of the executed "command"           */
	void client_signal_handler(int signo) {
		//syslog( LOG_ERR, "Got Child Signal %d (%d)\n", signo, getpid() );
		while ( waitpid( -1, NULL, WNOHANG | WUNTRACED ) > 0 );  // mop up defuncts
	   	return;
	}

	void master_signal_handler(int signo) {
		exit( 0 );
		return;
	}

	/* The signal handlers */

	void signal_handler(int sig) {
		//syslog( LOG_ERR, "%d Received Signal %d - %d ", getpid(), sig, SIGCHLD );
		if ( sig == SIGTERM ){
		//	syslog( LOG_ERR, "%d Received Signal %d - %d ", getpid(), sig, SIGCHLD );
		    	abort_now = 1;
		} else if ( sig == SIGUSR2 ) {
		//	syslog( LOG_ERR, "%d Received Signal %d - %d ", getpid(), sig, SIGUSR2 );
		    	dshutdown = 1;
		} else if ( sig == SIGCHLD ) {
			client_signal_handler( sig );
		}
	}	

	void reset_signal_handlers() {
		struct sigaction act;
	
		/* Attach the Signal Handler to a function */
		memset(&act, 0, sizeof(act));
		act.sa_handler = signal_handler;

	    	sigemptyset(&act.sa_mask);

		/* dont want child signals, want any interrupted syscalls to restart */
//	    	act.sa_flags = SA_NOCLDSTOP|SA_RESTART;

	    	sigaction(SIGINT,  &act, 0);		/* Interrupt Signal */
	    	sigaction(SIGTERM, &act, 0);	        /* Termination Signal */
	    	sigaction(SIGHUP,  &act, 0);	        /* Hangup Signal */
	    	sigaction(SIGQUIT, &act, 0); 	        /* Quit Signal */
    		sigaction(SIGUSR1, &act, 0);	        /* Child Down Nothing */
    		sigaction(SIGUSR2, &act, 0);	        /* Restart Signal */
	    	sigaction(SIGCHLD, &act, 0);	        /* Child Down Nothing */

		return;
	}
}

int set_user_and_group( uid_t userID, gid_t groupID )
{
	int success = 0;
	
	if ( groupID != 0 ){
		setgroups( 0, NULL );
        }

	success = setgid( groupID );
	
	if ( success ){
		syslog( LOG_ERR, "Unable to change group permissions to appropriate group level (%s), aborting", strerror( errno ) );
		return ( 1 );
	}

	success = setuid( userID );
		
	if ( success ){
		syslog( LOG_ERR, "Unable to change user permissions to appropriate user level (%s), aborting", strerror( errno ) );
		return ( 1 );
	}
	return 0;
}

int change_ownership( const char * socket, const char * username, const char * groupname )
{
	/* now that we have a socket, and a file descriptor for it change it's */
	/* ownership so that only Nobody can talk to it                        */

	struct passwd * userinfo = getpwnam( username );

	uid_t user_u = NOBODY;

	if ( userinfo )
		user_u = userinfo->pw_uid;

	struct group * groupinfo = getgrnam( groupname );

	gid_t user_g = NOBODY;

	if ( groupinfo )
		user_g = groupinfo->gr_gid;

	
	int failed = chown( socket, user_u, user_g);

	if ( failed ) {
		syslog( LOG_CRIT, "Unable to change owndership (%s:%s) on incoming connections, aborting", username, groupname );
		exit( 1 );
	}
	
	failed = chmod( socket, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );

	return failed;
}

/* define a new structure to contain a "connection", this will detail both the socket */
/* to listen on, and the list of functions to run etc                                 */

class Client {
public:
	Client( fd_set & master, int & maxfd, ModuleMap & nfunctions, const char * nsocket, const char * nusername, const char * ngroupname );
	~Client();
	ModuleMap functions;
	std::string username;
	std::string groupname;
	std::string socket;
	int process();
private:
	UDSocket listener;

	char buffer[ 1024 ];
	int rc;
	int listenfd;

	fd_set fdSet;  	// our set of fds (only 1) that select monitors for us
    	fd_set fdcpy;  	// select modifes the set so we need to use a copy
};

Client::Client( fd_set & master, int & maxfd, ModuleMap & nfunctions, const char * nsocket, const char * nusername, const char * ngroupname )
{	
	functions = nfunctions;
	username  = nusername;
	groupname = ngroupname;
	socket    = nsocket;

	/* connect to our communication socket and listen for commands */

	//syslog( LOG_INFO, "opening communications channel" );

	/* reset the socket (it's tider that way) */
	listener.reset();

	if ( listener.bind( socket.c_str() )) {  // bind to file
		syslog( LOG_CRIT, "Unable to bind incoming command socket, aborting" );
		exit( 1 );
        }

	if ( listener.listen(256)) {    // set it to listen mode with a kernel
                                  	// queue of 256 backlog connections
		syslog( LOG_CRIT, "Unable to listen for incoming commands, aborting" );
		exit( 1 );
        }

	/* create a file descriptor for this connection */

	listenfd = listener.getFD();
	
    	FD_ZERO( &fdSet);  			// clear the set
    	FD_SET( listenfd, &fdSet );  		// add the socket to the set
    	FD_SET( listenfd, &master );  		// add the socket to the set

	if ( listenfd > maxfd ) maxfd = listenfd;

	int failed = change_ownership( socket.c_str(), username.c_str(), groupname.c_str() );
	
	if ( failed ) {
		syslog( LOG_CRIT, "Unable to set security permissions (%s:%s) on incoming connections, aborting", username.c_str(), groupname.c_str() );
		exit( 1 );
	}
	
	dshutdown = 0;
	
	syslog( LOG_INFO, "Starting normal operations for client %d", listenfd );

}

int Client::process( ) {

	fdcpy = fdSet;  // take a copy

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

        rc = select( listenfd + 1, &fdcpy, NULL, NULL, &timeout);   // don't block

	if (rc <= 0) {  // was an error
		return 0;
       	}

	if (!FD_ISSET( listenfd, &fdcpy)) {
		syslog( LOG_ERR, "wrong fd set" );
		return 0;
	}

	/* Create a new process specifically to handle this incoming connection */
	volatile pid_t ppid;
	
	UDSocket connection;
	connection.close();
	connection = listener.accept();

	if ( connection.getFD() < 0) {
		connection.close();
		syslog( LOG_ERR, "unable to accept incoming connection, skipping" );
		return 0;
//		continue; 
	}

//syslog( LOG_ERR, "Producing a child from %d", getpid() );

	ppid = fork();

reset_signal_handlers();
//syslog( LOG_ERR, "Result of fork was %d", ppid );

	if (ppid == -1)
	{
		syslog( LOG_CRIT, "Unable to initialise connection handler.  Unable to continue" );
		return -1;
	}
	else if (ppid == 0)
	{

		//syslog( LOG_ERR, "%d Setting Comms GID to %d", getpid(), master_pgid );
//		setpgid( getpid(), master_pgid );

		/* in child */
		signal( SIGCHLD, SIG_DFL );
		/* Get the command from the client */
		try {
			rc = connection.getline( buffer, sizeof( buffer ) - 2, 3);  // throws on err
		} catch (exception& e) {
			connection.close();  // close the connection
			syslog( LOG_ERR, "Failed to establish command from client (readerr)" );
			return 0;
		}

		std::string response = "General failure";

		/* resolve this command into a function */

		std::vector<std::string> arguments;

		/* split the command on the seperating character into the  */
		/* "command" proper, and it's arguments                    */

		std::string command;

		char * start = strtok( buffer, " " );

		if ( !start ){ 
			syslog( LOG_WARNING, "Client attempted to invoke unrecognised command" );
			response = "Unable to invoke function:  Command not recognised\n";
		} else {
			command = start;

			for ( start = strtok( NULL, " " ); start != NULL; start = strtok( NULL, " " ) ){
				arguments.push_back( start );
			}

			if ( command == "reload" ){
			
				try {
					connection.writeString( "Reloading system configuration" );  // throws on err
				}
				catch (exception& e) {
					syslog( LOG_ERR, "Unable to transmit reply to client" );
				}
				connection.close();
				/* signal the parent thread to SIGHUP, which should force a reload */
				kill( getppid(), SIGHUP );
				exit( 0 );
			}
				
			MFunction functionToCall = functions[ command.c_str() ].function;

			if ( !functionToCall ){
				/* don't recognise the function */
				syslog( LOG_WARNING, "Client %d attempted to invoke unregistered function (%s)", listenfd, command.c_str() );
				response = "Attempt to invoke unregistered command";
			} else {
				int logging_level = functions[ command.c_str() ].logging; 

				std::string commanddef = "invoking command ";
				commanddef += command;
			
				if ( logging_level == 2 ) {
					commanddef += " (";
				
					for ( std::vector<std::string>::iterator i = arguments.begin() ; i != arguments.end() ; i++ ){
						commanddef += *i;
						commanddef += ",";
					}
					commanddef += ")\n";
				}

				if ( logging_level > 0 ){
					syslog( LOG_INFO, "%d:%s", listenfd, commanddef.c_str());
				}

				/* change the ownership of this process (in terms of users) */
				/* to whomever it is configured to run as.  Note, the       */
				/* default is to run as "nobody" as the normal int default  */
				/* of zero would mean "root" */
				uid_t userID  = NOBODY;
				gid_t groupID = NOBODY;

				if ( functions[ command.c_str() ].user_id != -1 )
					userID = functions[ command.c_str() ].user_id; 
				if ( functions[ command.c_str() ].group_id != -1 )
					groupID = functions[ command.c_str() ].group_id; 

				int failure = set_user_and_group( userID, groupID );
					
				if ( failure ){
					exit( 1 );
				}

				/* call the function */
				failure = (*functionToCall)( arguments, response );
//syslog( LOG_ERR, "Command Complete" );
				if ( failure ){
					response = "Error: " + response;
					syslog( LOG_WARNING, "Command Failed \"%s\" (%d)", response.c_str(), failure );
				}
			}		
		}

		/* and respond accordingly */
		try {
			response += "\n";
			connection.writeString( response.c_str() );  // throws on err
		}
		catch (exception& e) {
			syslog( LOG_ERR, "Unable to transmit reply to client" );
		}
		connection.close();

//syslog( LOG_ERR, "%d exiting", getpid() );
		exit( 0 );
	}  else {
		//syslog( LOG_ERR, "%d Parent just spawned connection to handle %d", getpid(), ppid );
//		setpgid( ppid, master_pgid );
		connection.close();  /* this has been duplicated for the child and we don't need it */
		return 0;
	}
}

Client::~Client()
{
	syslog( LOG_INFO, "Shutting down communications subsystem" );
	listener.close();

	return ;
}

int parse_directory( std::vector<ModuleReg> & modules, ModuleMap & functions, std::vector<ModuleFunction> & timedfunctions, std::string & moduleName )
{
	struct stat buffer;
	
	int error = stat( moduleName.c_str(), &buffer );

	if ( !error ){
	//	syslog( LOG_INFO, "Parsing Directory %s for plugins", moduleName.c_str() );
		/* open the module directory and see how many modules reside within */
		DIR * module_directory = opendir( moduleName.c_str() );

		if ( module_directory == NULL ){
			syslog( LOG_CRIT, "Unable to open modules directory, aborting" );
			return 1;
		}

		struct dirent * currentd = NULL;
	
		while ( currentd = readdir( module_directory ) ){

			if ( currentd->d_name[ 0 ] == '.' ){
				continue;
			}
		
			std::string actualmoduleName = moduleName;
			actualmoduleName += currentd->d_name;

			syslog( LOG_INFO, "Loading Plugins for Module \"%s\"", actualmoduleName.c_str() );

			int error = stat( actualmoduleName.c_str(), &buffer );

			if ( S_ISDIR(buffer.st_mode) )
				continue;

			if ( !error ){
				register_module( modules, functions, timedfunctions, actualmoduleName.c_str() );
			}
		}

		closedir( module_directory );
	}

	return 0;
}




void load_modules( std::vector<ModuleReg> & modules, ModuleMap & functions, std::vector<ModuleFunction> & timedfunctions )
{
	/* parse the system modules */
	std::string moduleName = MODULESUBDIR;

	parse_directory( modules, functions, timedfunctions, moduleName );
}



int main( int argc, char ** argv )
{
	/* Main function :                                                     */
	/*   Operate as a Daemon, so we should do the fork thing               */

	/* Start logging as soon as possible */
	openlog( IDENT, LOG_NDELAY | LOG_CONS, LOG_DAEMON );

	syslog( LOG_INFO, IDENT" Starting Up..." );

	/* are we already running ? */

	master_pid  = getpid();

	int old_pid = 0;
	
	if ( old_pid = amrunning( PID, BINARY ) ){
		std::cerr << "Process already exists with process ID " << old_pid << "\n";
		exit( 0 );
	}
	
	volatile pid_t pid;

	/* register the child signal handler, we will probably register a different one later */
	/* This handler is used solely for smoothd to background itself. */
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &master_signal_handler;	
    	if (sigaction(SIGUSR1, &sa, NULL)) {  	
		syslog( LOG_CRIT, "Unable to register Signal handler, aborting" );
	        exit( 1 );
	}

    	/* detatch ourselves from calling process group */
	if( ( pid = fork() ) > 0) {
//syslog( LOG_ERR, "Setting Master GID to %d", master_pgid );
		/* we are the parent - so exit as soon as SIGUSR1 is received */
		sleep ( 20 );
		/* if we have reached this point, our children have failed to start    */
		/* hence, we should kill them, just incase and then exit with suitable */
		/* errors */
		syslog( LOG_ERR, "One or more communications channels failed to start, aborting\n" );
		struct sigaction sa, oldsa;
		memset(&sa, 0, sizeof(sa)); sa.sa_handler = SIG_IGN;
		sigaction(SIGTERM, &sa, &oldsa); // ignore sigterm for us
      		int ret = kill( -pid, SIGTERM );
				  	// send everyone in this process group a TERM
					// which causes them to exit as the default action
					// but it also seems to send itself a TERM
					// so we ignore it
      		sigaction(SIGTERM, &oldsa, NULL); // restore prev state			
		int status = -1;
		volatile pid_t child = wait3( &status, 0, NULL );
		exit(0);
	}

	/* set our process group */	
	master_pgid = getpgid( getpid() );

	//syslog( LOG_ERR, "Setting Master GID to %d", master_pgid );
	setpgid( getpid(), master_pgid );

	if( setsid() < 0 ){
		syslog( LOG_CRIT, "Unable to Spawn Daemon Process" );
		exit(1); /* failure! */
    	}

	/* capture Signals accordingly */
	reset_signal_handlers();
	
	/* create our PID File */
	create_pid_file( PID );
		
	/* and become nobody */
	
	struct passwd *st;  // prepare a struct
	
	if ((st = getpwnam("nobody")) != 0) {  // find uid
		/* change the ownership of the PID file */
		chown( PID, st->pw_uid, st->pw_gid );
		chmod( PID, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
	} else {
		syslog(LOG_CRIT, "Unable to find user/group 'nobody', unable to continue");
		return 1;	// was unable to lockup the user id from passwd
			        // for some reason, so exit with error
	}

	/* redirect STDIN, STDOUT, STDERR etc */
	freopen("/dev/null", "a+", stdin);
	freopen("/dev/null", "a+", stdout);
    	freopen("/var/log/smoothderror", "a+", stderr);
	
	ModuleMap functions; 
	std::vector<ModuleReg> modules;
	std::vector<ModuleFunction> timedfunctions;

	while ( !abort_now ){
		syslog( LOG_INFO, "Registering Functions..." );

		load_modules( modules, functions, timedfunctions );
		
		//syslog( LOG_INFO, "Initialising System services" );
	
		for ( std::vector<ModuleFunction>::iterator i = timedfunctions.begin() ; i != timedfunctions.end() ; i++ ){  
	                syslog( LOG_INFO, "    Starting Timed function %s:%d:%d", (*i).name.c_str(), (*i).handle, (*i).timeout );
			/* do some fork stuff */
			volatile pid_t ppid;
	
			ppid = fork();
			if (ppid == -1)
			{
				syslog( LOG_CRIT, "Unable to initialise connection handler.  Unable to continue" );
				return -1;
			}
			else if (ppid == 0)
			{
				/* This child process handles the timed functions. */
//				reset_signal_handlers();
				/* Do ignore SIGUSR2 */
				struct sigaction sa_ign;
				memset(&sa_ign, 0, sizeof(sa_ign));
				sa_ign.sa_handler = SIG_IGN;
				sigaction(SIGUSR2, &sa_ign, 0);

				/* set our process group */
//				syslog( LOG_ERR, "Setting Timed GID to %d -> %d", getpid() , master_pgid );
				setpgid( getpid(), master_pgid );

				/* in child */
			
//				signal( SIGCHLD, SIG_IGN ); 
	
				/* change user to someone else */
				uid_t userID  = NOBODY;
				gid_t groupID = NOBODY;

				if ( (*i).user_id != -1 )
					userID = (*i).user_id; 

				if ( (*i).group_id != -1 )
					groupID = (*i).group_id; 

				int failure = set_user_and_group( userID, groupID );
					
				if ( failure ){
					exit( 1 );
				}
				
				signal(SIGCHLD, SIG_DFL);
				
				/* call the function */
				MFunction functionToCall = (*i).function;
				std::vector<std::string> arguments;
				std::string response;

				while ( !abort_now ){
					int start_time = time( NULL );

		                	//syslog( LOG_INFO, "Performing Timed function \"%s:%d\" with timeout of %d\n", (*i).name.c_str(), (*i).handle, (*i).timeout );
					failure = (*functionToCall)( arguments, response );

					if ( failure ){
						response = "Error: " + response;
						syslog( LOG_WARNING, "Timed Command Failed \"%s\" (%d)", response.c_str(), failure );
					}
				
					int end_time = time( NULL );
					int sleepfor = ( (*i).timeout * 60 ) - ( end_time - start_time );
				
	 				//syslog( LOG_ERR, "Having performed timed function, sleeping for %d seconds\n", sleepfor );
					sleep( sleepfor );
				}			
	 			exit( 0 );	
			} else {
				setpgid(ppid, master_pgid );
			}
	        }

		std::vector<std::string> module_socket_descriptors ;

		list_files( module_socket_descriptors, "/usr/lib/smoothd/sockets/" );

		std::vector<Client *> client_sockets;

		fd_set fdSet;  	// our set of fds (only 1) that select monitors for us
	    	fd_set fdcpy;  	// select modifes the set so we need to use a copy

		int maxfd = 0;

    		FD_ZERO( &fdSet);  			// clear the set

		for ( 
			std::vector<std::string>::iterator config_file = module_socket_descriptors.begin() ;
			config_file != module_socket_descriptors.end();
			config_file ++ 
		    ){
			//syslog( LOG_ERR, "Creating Socket Descriptor for %s", (*config_file).c_str());
			ConfigVAR descriptor( (*config_file) );

			std::string owner = descriptor[ "user" ];
			ModuleMap relevantfunctions;

			/* map the function list into the functions defined for this user */
			for ( 
				ModuleMap::iterator function = functions.begin();
				function != functions.end();
				function++ 
			    ){
				std::string key = (*function).first;
				ModuleFunction thefunction = (*function).second;
//syslog( LOG_ERR, "Mapping function %s (%s:%s)", key.c_str(), thefunction.owner.c_str(), owner.c_str() );	
				if ( thefunction.owner == owner ){
					relevantfunctions[ key ] = thefunction;
				}
			}

			Client * nobody = new Client( fdSet, maxfd, relevantfunctions, descriptor[ "socket" ].c_str(), owner.c_str(), descriptor[ "group" ].c_str() );
			client_sockets.push_back( nobody );
		}


		/* Handle the "user" or client connections  */

		/* Init complete, signal dad */
		if ( master_pid != 0 ){
			kill( master_pid, SIGUSR1 );
			master_pid = 0;
		}

		while ( !abort_now && !dshutdown ) {  
			fdcpy = fdSet;  // take a copy
		        int rc = select( maxfd + 1, &fdcpy, NULL, NULL, NULL);   // block
                                                 			// until something happens        

			if (rc < 0) {  // was an error
			        if (errno == EINTR) {
//syslog( LOG_ERR, "was interupted\n" );
					continue;
       				}
				continue;
       			}

			for ( 
				std::vector<Client *>::iterator client = client_sockets.begin() ;
				client != client_sockets.end();
				client ++ 
			    ){
				(*client)->process();
			}
		}	

		syslog( LOG_INFO, "Initiating smoothd shutdown" );
		syslog( LOG_INFO, "    Shutting down Timed function(s)" );

		struct sigaction oldsa;
		memset(&sa, 0, sizeof(sa)); sa.sa_handler = SIG_IGN;
		sigaction(SIGTERM, &sa, &oldsa); // ignore sigterm for me (this process)
      		int ret = kill( -pid, SIGTERM );
				  	// send everyone in this process group a TERM
					// which causes them to exit as the default action
					// but it also seems to send itself a TERM
					// so we ignore it
		int status = -1;
		/* Reap all children as they perish; we don't want zombies */
		pid_t child = wait3( &status, 0, NULL );

		for ( std::vector<ModuleReg>::iterator rmod = modules.begin(); rmod != modules.end(); rmod++ ){
			syslog( LOG_INFO, "    Unregistering module %s", (*rmod).name.c_str() );
			/* there is no point closing all the handles, as we */
			/* will run into problems if we replace a module and */
			/* attempt a reload */
		}

		modules.clear();
		functions.clear();
		timedfunctions.clear();
      		sigaction(SIGTERM, &oldsa, NULL); // restore prev state
		syslog( LOG_INFO, "Smoothd shutdown complete." );
	}

	remove_pid_file( PID );

	return 0;

}

