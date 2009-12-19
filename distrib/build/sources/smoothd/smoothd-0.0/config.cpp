#include "config.h"
#include "setuid.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <dirent.h>

ConfigCSV::ConfigCSV()
{
	current = csvarray.begin();
}

ConfigCSV::ConfigCSV( std::string filename, const char * delimiter )
{
	current = csvarray.begin();
	readcsv( filename.c_str(), delimiter );
}

ConfigCSV::ConfigCSV( const char * filename, const char * delimiter )
{
	current = csvarray.begin();
	readcsv( filename, delimiter );
}

int ConfigCSV::readcsv( const char * filename, const char * delimiter )
{
	/* get options from a file ... */
	std::ifstream input ( filename );
	char buffer[ 2048 ];
	
	if ( !input ) return 1;

	csvarray.clear();
	
	while ( input ){
		if ( !input.getline( buffer, sizeof( buffer )) ) {
			break;
		}

		std::vector<std::string> line;

		char * start = buffer;
		char * last  = buffer + strlen( buffer );

		while( start ){
			char * stop = strstr( start, delimiter );

			if ( !stop ){
				line.push_back( start );
				break;
			}

			*stop = '\0';

			line.push_back( start );

			start = stop + strlen( delimiter );
			
			if ( !start || (start > last) ){
				break;
			}
		}

		csvarray.push_back( line );
	}

	input.close();

	current = csvarray.begin();
	return 0;
}

int ConfigCSV::first()
{
	current = csvarray.begin();
	if ( current == csvarray.end() ){
		return 1;
	}
	return 0;
}

int ConfigCSV::operator++ ()
{
	return next();
}

int ConfigCSV::operator++ ( int )
{
	return next();
}

int ConfigCSV::next()
{
	current ++;

	if ( current == csvarray.end() ){
		current--;
		return 1;
	}

	return 0;
}

std::string ConfigCSV::operator[] ( const int reference )
{
	return entry( reference );
}


std::string ConfigCSV::entry( const int reference )
{
	if ( current == csvarray.end() ){
		return "";
	}
	
	if ( reference < 0 || reference >= (int) (*current).size() ){
		return "";
	}
	
	std::string array_entry = (*current)[ reference ];
	
	return array_entry;
}

std::string ConfigCSV::entry( const int row, const int reference )
{
	std::vector<std::string> selected;
	
	if ( row < 0 || row >= (int) csvarray.size() ){
		return "";
	}

	selected = csvarray[ row ];

	if ( reference < 0 || reference >= (int) selected.size() ){
		return "";
	}
	
	std::string array_entry = selected[ reference ];
	
	return array_entry;

}

ConfigVAR::ConfigVAR()
{
}

ConfigVAR::ConfigVAR( std::string filename, const char * delimiter )
{
	readvar( filename.c_str(), delimiter );
}

ConfigVAR::ConfigVAR( const char * filename, const char * delimiter )
{
	readvar( filename, delimiter );
}

int ConfigVAR::readvar( const char * filename, const char * delimiter )
{
	/* get options from a file ... */
	std::ifstream input ( filename );
	char buffer[ 2048 ];

	params.clear();
	
	if ( !input ) return 1;

	while ( input ){
		if ( !input.getline( buffer, sizeof( buffer )) ) {
			break;
		}

		char * command = buffer;
		if ( !( *command ) ) continue;

		char * parameter = strstr( buffer, delimiter );
		if ( !parameter ) continue;

		if ( (parameter + strlen( delimiter )) > buffer + strlen( buffer ))
			continue;

		*parameter = '\0';
		parameter += strlen( delimiter );
	
		/* strip delimiters */
		while ( *parameter == '"' || *parameter == '\'' || *parameter == ' ') parameter++;
		int offset = strlen( parameter ) - 1;

		while ( parameter[ offset ] == '"' || parameter[ offset ] == '\'' ) parameter[ offset-- ] = '\0';

		offset = strlen( command ) - 1;
		while ( command[ offset ] == ' ' ) command[ offset-- ] = '\0';
	
		params[ command ] = parameter;
	}

	input.close();

	return 0;
}

std::string ConfigVAR::entry( const char * reference )
{
	return params[ reference ];
}

std::string ConfigVAR::operator[] ( const char * reference )
{
	return params[ reference ];
}

std::vector<std::string> ConfigVAR::keys( void )
{
	std::vector<std::string> keylist;

	for ( 
		std::map<std::string, std::string, ltstr>::iterator i = params.begin();
		i != params.end();
		i++ 
	    ){
		keylist.push_back( (*i).first );
	}

	return keylist;
}

std::vector<std::string> ConfigVAR::interfaces( void )
{
	std::vector<std::string> keylist = keys();
	std::vector<std::string> interfacelist;

	for ( 
		std::vector<std::string>::iterator i = keylist.begin();
		i != keylist.end();
		i++ 
	){
		if (strncmp( (*i).c_str(), "INTERFACE_", strlen("INTERFACE_") ) == 0)
		{
			if (params[ (*i).c_str() ] == "on")
			{
				interfacelist.push_back( (*i).c_str() + strlen("INTERFACE_") );
			}
		}
	}

	return interfacelist;
}

// ConfigSTR stuff is simple, the contents is the whole file - no delimeters etc.
ConfigSTR::ConfigSTR()
{
	thisstr = "";
}

ConfigSTR::ConfigSTR(std::string filename)
{
        readvar( filename.c_str());
}

ConfigSTR::ConfigSTR(const char *filename)
{
        readvar( filename);
}
int ConfigSTR::readvar(const char *filename) 
{
	std::string::size_type n;
	std::ifstream input ( filename );
	if ( !input ) return 1;
	input >> thisstr;
	// remove trailing newlines
	while((n = thisstr.rfind('\n')) != std::string::npos)
		thisstr.resize(n-1);
	input.close();

	return 0;
}

std::string ConfigSTR::str()
{
	return thisstr;
}

#include <syslog.h>

int list_files( std::vector<std::string> & files, const char * path )
{
	/* parse various directories and return a list of files */

	/* first of all the "path" */
	DIR * directory = opendir( path );

	if ( directory ){
		struct dirent * currentd = NULL;
	
		while (( currentd = readdir( directory ) )){
			if (( currentd->d_name[ 0 ] == '.' )){
				continue;
			}

			std::string current = path;
			current += currentd->d_name;
			files.push_back( current );
		}
	}

	closedir( directory );

	/* and now the potential "module" paths */
	DIR * modules = opendir( "/modules" );

	if ( modules ){
		struct dirent * currentmodule = NULL;
		while (( currentmodule = readdir( modules ) )){
			if ( currentmodule->d_name[ 0 ] == '.' ){
				continue;
			}

			/* check the "module" directory */	
			std::string modulepath = "/modules/";
			modulepath += currentmodule->d_name;
			modulepath += path;
			
			directory = opendir( modulepath.c_str() );

			if ( directory ){
				dirent * currentd = NULL;
	
				while (( currentd = readdir( directory )) ){
					if ( currentd->d_name[ 0 ] == '.' ){
						continue;
					}
					std::string current = modulepath;
					current += currentd->d_name;
					files.push_back( current );
				}
			}

			closedir( directory );
		}
		closedir( modules );
	}

	return( 0 );
}

#include <glob.h>

int module_files( std::vector<std::string> & files, const char * file )
{
	/* Equivilent of modulesfiles from header.pm.  Fills out a vector of
	 * filenames looking in the core and in modules. */
	glob_t globbuf;
	char fullfile[STRING_SIZE];
	                        
	memset(&globbuf, 0, sizeof(glob_t));
    memset(fullfile, 0, STRING_SIZE - 1);
    
    snprintf(fullfile, STRING_SIZE - 1, "/modules/*/%s", file);
    
	glob(file, GLOB_DOOFFS, NULL, &globbuf);
	glob(fullfile, GLOB_DOOFFS | GLOB_APPEND, NULL, &globbuf);

	for (size_t i = 0; i < globbuf.gl_pathc; i++)
		files.push_back(globbuf.gl_pathv[i]);

	return 0;
}

int safeatoi( const char * a )
{
	if (strspn(a, NUMBERS) != strlen(a)){
		return 0;
	}

	return atoi( a );
}

int safeatoi( std::string a )
{
	return( safeatoi( a.c_str() ) );
}




	
