#include "UDSocket.hpp"
#include <iostream>

int main(int argc, char **argv) {
	
	UDSocket listener;

	/* reset the socket (it's tider that way) */
	listener.reset();

	if ( listener.bind( "/tmp/suidaemon" )) {  // bind to file
        	std::cerr << "Error binding listening Socket\n" << std::endl;
            	return 1;
        }

	if ( listener.listen(256)) {    // set it to listen mode with a kernel
                                  	// queue of 256 backlog connections
        	std::cerr << "Error listening to url ipc server file" << std::endl;
	        return 1;
        }

	/* create a file descriptor for this connection */

	int listenfd = listener.getFD();

	fd_set fdSet;  // our set of fds (only 1) that select monitors for us
    	fd_set fdcpy;  // select modifes the set so we need to use a copy
    	FD_ZERO( &fdSet);  		// clear the set
    	FD_SET( listenfd, &fdSet );  	// add the socket to the set
	
	UDSocket connection;
	
	char buffer[ 1024 ];
	
	int rc;
	
    	while (true) {  
		fdcpy = fdSet;  // take a copy
	        rc = select( listenfd + 1, &fdcpy, NULL, NULL, NULL);   // block
                                                  			// until something happens
        
		if (rc < 0) {  // was an error
		        if (errno == EINTR) {
                		continue;  // was interupted by a signal so restart
            		}
            		continue;
        	}
        
		if (FD_ISSET( listenfd, &fdcpy)) {
			connection = listener.accept();
			
		        if ( connection.getFD() < 0) {
		                connection.close();
				std::cerr << "Error accepting\n";
		                continue; 
                	}
            	}

		/* now we can do the actual communication things */
	        try {
                	rc = connection.getline( buffer, sizeof( buffer ), 3);  // throws on err
	        } catch (exception& e) {
                	connection.close();  // close the connection
                        std::cerr << "error with read\n";
			continue;
              	}

		try {
        		connection.writeString( "standard reply\n" );  // throws on err
    		}
		catch (exception& e) {
            		std::cerr << "Unable to reply\n";
    		}
		
		connection.close();
		
        }

	return 0;
}
