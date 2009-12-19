/* SmoothWall helper program - header file
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * filename: ipbatch.cpp
 * Direct interface into iptables. */

#include <vector>
#include <string>
#include <cstring>
#include "ipbatch.h"
#include <errno.h>
#include <syslog.h>

// local storage

static int batchstoreidx = 0;
static char batchstore[BATCHSTORE_SIZE];

static std::vector<std::string> mybatch;

// work form char *
int ipbatch(const char *arg) 
{
	int rval = 0;
	if(!strcmp(arg,"commit")) {
		rval = ipbatch(mybatch);
		mybatch.clear();
	}
	else 
		mybatch.push_back(std::string(arg));
	return rval;
}
// or from single strings
int ipbatch(const std::string &arg) 
{
	int rval = 0;
	if(arg == "commit") 
	{
		rval = ipbatch(mybatch);
		mybatch.clear();
	}
	else 
		mybatch.push_back(arg);
	return rval;
}

// but best way is as whole vector as this implies that we want to
// execute now too - no need for commit.
int ipbatch(std::vector<std::string> &arg) 
{
	unsigned int i;
	std::string::size_type pos;
	int rval = 0;
	size_t len;
	
	for(i = 0; i < arg.size(); i++) 
	{
		// fist is it empty
		const std::string & s = arg[i];
		// ignore any leading space
		pos = s.find_first_not_of(" \t\n");

		if(pos == std::string::npos)
			continue; // only whitespace
		if(s[pos] == '#')
			continue; // a comment
		
		if(s.find("commit", pos) == pos) 
		{
			syslog(LOG_WARNING, "commit\n");
			rval = dobatch(batchstore);
			batchstoreidx = 0;
			batchstore[batchstoreidx] = 0;
		}
		else 
		{
			// maybe some sanity checking here?
			syslog(LOG_WARNING, "batchline %s\n", s.c_str());
			len =  strlen(s.c_str());
			if(batchstoreidx + len < BATCHSTORE_SIZE) 
			{
				strncpy(&(batchstore[batchstoreidx]), s.c_str(), len);
				batchstoreidx += len;
				if(batchstore[batchstoreidx-1] != '\n')
					batchstore[batchstoreidx++] = '\n';
				batchstore[batchstoreidx] = 0; // maybe will get overwritten
			}
			else 
				syslog(LOG_ERR, "ipbatch buffer size exceeded");
		}
	}
	if(batchstoreidx > 0) 
		rval = dobatch(batchstore);
	return rval;
}
