/* SmoothWall helper program - header file
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * filename: ipbatchc.c
 * Direct interface into iptables. */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <iptables.h>
#include <xtables.h>
#include <errno.h>
#include <syslog.h>

#define BATCHSTORE_SIZE 10240
#define TABLE_NAME_SIZE 16
// no iptables command can have more parts to it than this!
#define MAX_ARGS 100

static char * table = "filter";
static struct iptc_handle *handle = NULL;
static char prev_table_name[TABLE_NAME_SIZE] = "";
struct xtables_globals iptables_globals;


#define program_name iptables_globals.program_name
#define program_version iptables_globals.program_version

// see what table is mentioned in this command - if it is different need to
// flush anything we already have before changing to new table.

int table_changed(char *buf)
{
	char *tp;
	int i=0;
	char new_table_name[TABLE_NAME_SIZE];
	
	// a missing -t means that the filter table is used
	strcpy(new_table_name, "filter");
	tp = strstr(buf,"-t");
	if(tp != NULL) {
		tp += 3; // move past -t 
		while(*tp && *tp != ' ' && i < sizeof(new_table_name)-2) 
		{
			new_table_name[i++] = *tp++;
		}
		// null terminate
		new_table_name[i] = 0;
	}
	i = strcmp(new_table_name, prev_table_name);
	if(i) 
	{
		strcpy(prev_table_name,new_table_name);
	}
	return i;
}

int execute( char * commands )
{
	/* split the command into an array of the appropriate */
	int rval = 0;
	char * arguments[ MAX_ARGS ];
	
	char * current_reference;
	int counter;
	
	// syslog(LOG_WARNING, "execute: %s\n", commands); 
	for (counter=0, current_reference = strtok( commands, " " );
			 counter < MAX_ARGS && current_reference != NULL;
			 current_reference = strtok( NULL, " " ) )
		arguments[ counter++ ] = current_reference;
	arguments[counter] = 0; // mate it pleasant for the debugger
	
	// call down to iptables
	rval = do_command( counter, arguments, &table, &handle );
	// syslog(LOG_WARNING, "execute: %s rval = %d\n", commands, rval);
	return rval;
	
}

// the buffer store is a sequence of newline separated commands
int dobatch(char *store) 
{
	int size = strlen(store);
	int have_committed = 1; // prevent double commits, 'clean' state to begin with

	char linebuf[1024];
	char *eol;
	char *pos;
	int error = 0;
	int linelen;
        int c;

	if(size <= 0 || store == NULL)
		return 1;

	program_name = "ipbatchc.c";
	program_version = IPTABLES_VERSION;

	// this is really important for locating shared libs
/*	lib_dir = getenv("IPTABLES_LIB_DIR");*/
/*	if (!lib_dir)*/
/*		lib_dir = IPT_LIB_DIR;*/
	pos = store; // start at beginning
	while(pos < &store[size] && (eol = index(pos,'\n')) != NULL) {
		linelen = eol - pos;
		if(linelen > 0 && linelen < sizeof(linebuf)) 
		{
			strncpy(linebuf, pos, linelen);

			linebuf[linelen] = 0;
			// syslog(LOG_WARNING, "dobatch: linebuf %s\n", linebuf);
	c = xtables_init_all(&iptables_globals, NFPROTO_IPV4);
	if (c < 0) {
		syslog (LOG_CRIT, "do_batch: %s/%s Failed to initialize xtables; couldn't execute \"%s\".\n",
		program_name,
		program_version,
		store);
		exit(1);
	}
			if((pos + linelen) < &store[size])
				pos += linelen + 1;
			else
				pos = store + size;
			if(!have_committed) {
				if(table_changed(linebuf)) {
					error =  iptc_commit(handle);
					iptc_free(handle);
					handle = NULL;
					have_committed = 1;
				}
			}
			if(strlen(linebuf) > 0) {
				error = execute(linebuf);
				if (error == 0) {
				}
				// syslog(LOG_WARNING, "do_batch: have execed 1\n");
			}
			have_committed = 0;
		}
		else {
			// empty or too big
			if(eol == NULL)
				break;
			else {
				pos = eol +1;
			}
		}
	}

	if(!have_committed) {
		error =  iptc_commit(handle);
		iptc_free(handle);
		handle = NULL;
		// syslog(LOG_WARNING, "do_batch: have committed 2\n");
	}
	      
	return !error;

}
