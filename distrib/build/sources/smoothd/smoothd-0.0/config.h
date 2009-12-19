#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>

int safeatoi( const char * a );
int safeatoi( std::string a );

int list_files( std::vector<std::string> & files, const char * path );
int module_files( std::vector<std::string> & files, const char * file );


class ConfigCSV {
	public:
		ConfigCSV();
		ConfigCSV( const char * filename, const char * delimiter = "," );
		ConfigCSV( std::string filename, const char * delimiter = "," );
		int readcsv( const char * filename, const char * delimiter = ",");

		std::string entry( const int reference );
		std::string entry( const int row, const int reference );
		int first();
		int next();

		std::string operator[] ( const int reference );
		int operator++ ();
		int operator++ ( int );
	private:
		std::vector< std::vector<std::string> > csvarray;
		std::vector< std::vector<std::string> >::iterator current;
};

class ConfigVAR {
	public:
		ConfigVAR();
		ConfigVAR( const char * filename, const char * delimiter = "=" );
		ConfigVAR( std::string filename, const char * delimiter = "=" );
		int readvar( const char * filename, const char * delimiter = "=");

		std::string entry( const char * reference );

		std::string operator[] ( const char * reference );

		std::vector<std::string> keys( void );
		std::vector<std::string> interfaces( void );
	private:

		struct ltstr
		{
			bool operator()(std::string s1, std::string s2) const
			{
				return strcmp(s1.c_str(), s2.c_str()) < 0;
			}
		};

  		std::map<std::string, std::string, ltstr> params;
};

class ConfigSTR {
	public:
		ConfigSTR();
		ConfigSTR( const char * filename);
		ConfigSTR( std::string filename);
		int readvar( const char * filename);
		std::string str();
	private:
		std::string thisstr;
};


#endif
