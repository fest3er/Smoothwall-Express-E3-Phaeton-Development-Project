/* IPTables batch processing program */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <iptables.h>
#include <errno.h>
#include <syslog.h>

#define BUFFER_SIZE 1024
#define MAX_ARGS 100

/*extern char *lib_dir;*/
static char * table = "filter";
static struct iptc_handle *handle = NULL;         /*static iptc_handle_t handle = NULL;*/
static char prev_table_name[BUFFER_SIZE] = "";
struct xtables_globals iptables_globals;

#define program_name iptables_globals.program_name
#define program_version iptables_globals.program_version



int table_changed(char *buf)
{
	char *tp;
	int i=0;
	char new_table_name[BUFFER_SIZE];

	// a missing -t means that the filter table is used
	strcpy(new_table_name, "filter");
	tp = strstr(buf,"-t");
	if(tp != NULL) {
		tp += 3; // move past -t 
		while(*tp && *tp != ' ' && i < sizeof(new_table_name)-2) {
			new_table_name[i++] = *tp++;
		}
		// null terminate
		new_table_name[i] = 0;
	}
	i = strcmp(new_table_name, prev_table_name);
	if(i) {
		strcpy(prev_table_name,new_table_name);
	}
	return i;

}

int execute( char * commands )
{
	/* split the command into an array of the appropriate */
	char * arguments[ MAX_ARGS ];

	char * current_reference;
	int counter;

	for (counter=0, current_reference = strtok( commands, " " );
			counter < MAX_ARGS && current_reference != NULL;
			current_reference = strtok( NULL, " " ) )
		arguments[ counter++ ] = current_reference;
	arguments[counter] = 0; // mate it pleasant for the debugger


	return do_command4( counter, arguments, &table, &handle );

}


int main ( int argc, char * argv[] )
{
	int have_committed = 1; // prevent double commits, 'clean' state to begin with
        int c;

	program_name = argv[0];
	program_name = "ipbatch CLI";
	// this is really important for locating shared libs
	program_version = IPTABLES_VERSION;
        c = xtables_init_all(&iptables_globals, NFPROTO_IPV4);
        if (c < 0) {
                fprintf(stderr, "%s/%s Failed to initialize xtables\n",
                                program_name,
                                program_version);
                exit(1);
        }


	openlog( "SmoothIPSubsys", LOG_NDELAY | LOG_CONS, LOG_DAEMON );
	
/*        lib_dir = getenv("IPTABLES_LIB_DIR");*/
/*	if (!lib_dir)*/
/*		lib_dir = IPT_LIB_DIR;*/

	/* read lines from STDIN */

	char buffer[ BUFFER_SIZE ];
	int error = 0;

	// syslog( LOG_ERR, "Checking input" );
	// there are two special commands end is the same as eof and commit
	// does an early commit rather than the changes only being committed at the end
	while ( fgets( buffer, BUFFER_SIZE - 2, stdin ) != NULL ){
		/* terminate the line at the carriage return */

		if ( strlen( buffer ) > BUFFER_SIZE ){
			// silently ignore long lines
			continue;
		}

		buffer[ strlen( buffer ) - 1 ] = '\0';

		//syslog( LOG_ERR, "Received command %s", buffer );

		if ( strcmp( buffer, "end" ) == 0 ){
			break;
		}

		if ( strcmp( buffer, "commit" ) == 0 ){
			/* commit changes */
			error = iptc_commit(handle);
			iptc_free(handle);
			handle = NULL;
			have_committed = 1;
		} else {
			/* excute the command */
			if(!have_committed) {

				if(table_changed(buffer)) {   
					//syslog( LOG_ERR, "Table change for %s", buffer );
					error = iptc_commit(handle);
					iptc_free(handle);
					handle = NULL;
					have_committed = 1;
				}
			}
			if(*buffer)
			    error = execute( buffer );
			have_committed = 0;
		}

		if ( !error ){
			/* if an error has occured then we're */
			/* in trouble and might as well just  */
			/* leave                              */
			syslog( LOG_ERR, "error: %s", iptc_strerror(errno));
			return !error;
		}

	}

	//syslog( LOG_ERR, "Finished" );
	/* commit the changes, that is flush */
	/* the iptables buffer               */

	if(!have_committed) {
		error = iptc_commit(handle);
		iptc_free(handle);
		handle = NULL;
		syslog( LOG_ERR, "Unable to commit IPTables rules \"%s\"", iptc_strerror( errno ) );
	}

	return !error;
}
