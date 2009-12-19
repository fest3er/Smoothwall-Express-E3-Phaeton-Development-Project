/* Set UID Demon System                                                      */
/* (c) 2004 SmoothWall Ltd                                                   */
/* ------------------------------------------------------------------------- */
/* Original Author: D.K.Taylor                                               */

#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

int register_module( char * moduleName )
{
	/* open the module with the name moduleName */

	/* load the module (if possible) */
	void * handle = NULL;
	char * error  = NULL;


	if ( moduleName == 0 ){
		fprintf( stderr, "Unable to load module, no module name given" );
		exit( 1 );
	}	

	handle = dlopen( moduleName, RTLD_LAZY );

	if ( handle == NULL ){
		fprintf( stderr, "Unable to load module %s", moduleName );
		exit( 1 );
	}

	error = dlerror(); 	/* clear any possible error handler */
	
	/* now call the register_module function */
	
	int ( *registerFunction )( void );

	registerFunction = ( int (*)(void) ) dlsym( handle, "load" );
	
	if ( (error = dlerror()) != NULL)  {
		fprintf( stderr, "error with module %s", error );
		exit(1);
	}

	(*registerFunction)();

	dlclose(handle);
}
 
//void module( void );

int main(int argc, char **argv) {
	register_module( "/global/users/darren/dev/suidaemon/libsample.so" );
	//module();
}




