/* pid.h -  Contains functions relating to managing the process  */
/* such as the creation, and manipulation of our process ID file */

#ifndef __PID_H_
#define __PID_H_

void create_pid_file( const char * pidfilename );
void remove_pid_file( const char * pidfilename );
int amrunning( const char * pidfilename, const char * processname );

#endif
