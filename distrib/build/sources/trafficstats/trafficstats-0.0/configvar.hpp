#ifndef CONFIGVAR_INC
#define CONFIGVAR_INC 


#include <vector>
#include <string>
#include <map>

int safeatoi( const char * a );
int safeatoi( std::string a );


class configvar {
private:
    
    struct ltstr
    {
        bool operator()(std::string s1, std::string s2) const
        {
            return strcmp(s1.c_str(), s2.c_str()) < 0;
        }
    };
    
    std::map<std::string, std::string, ltstr> params;

public:
    // empty constructor
    configvar() {};
    // constructor from file as a char *
    configvar( const char * filename, const char * delimiter = "=" ) {	
        readvar( filename, delimiter );
    };

    // and from a std::string
    configvar( const std::string & filename, const std::string delimiter = "=" ) {
        readvar( filename, delimiter );
    };
    // body in the cpp file.
    int readvar( const char * filename, const char * delimiter = "=");
    // can start empty and get readvar called
    int readvar( const std::string & filename, const std::string delimiter = "=") {
        return readvar(filename.c_str(), delimiter.c_str());
    };
    // rest are trivial enough to inline
    // cope with either strings or char *
    std::string entry( const char * reference ) {
	return params[ reference ];
    };
    std::string entry( const std::string & reference ) {
	return params[ reference.c_str() ];
    };
    std::string operator []( const char * reference ) {
	return params[ reference ];
    };
    std::string operator []( const std::string & reference ) {
	return params[ reference.c_str() ];
    };

    std::vector<std::string> keys(void) {
        std::vector<std::string> keylist;
        std::map<std::string, std::string, ltstr>::iterator i;
	for (i = params.begin();i != params.end();i++ )
		keylist.push_back( i->first );
	
	return keylist;
    };		

};

#endif
