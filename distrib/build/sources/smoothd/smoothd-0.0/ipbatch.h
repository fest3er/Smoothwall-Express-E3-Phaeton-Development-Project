/* SmoothWall helper program - header file
 *
 * Written by Martin Houston <martin.houston@smoothwall.net>
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * filename: ipbatch.h
 * Direct interface into iptables. 
 * If you need to do iptables commands in youtr plugin include this
 * header file and you have a choice of two APIs.
 * The preferred method is with a vector but there is also
 * a function callable from C code that takes just a char *
 * What is provided is a valid iptables command line
 * e.g. "iptables -nvL"
 * All iptables commands must be valid and must succeed - i.e. you must not 
 * attempt to remove a table that is not there. As commands are 
 * being submitted as a batch any failure will leave iptables
 * in an undefined state.
 */
 
#ifndef __IPBATCH_H
#define __IPBATCH_H
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
// C++ side
#include <string>
#include <cstring>
#include <iostream>
#define BATCHSTORE_SIZE 65536

// this only actualy does something if one of the args is "commit"
// in which case a non zero return code indicates some failure.
int ipbatch(std::vector<std::string> &arg);
int ipbatch(const std::string & arg);

extern "C" {
	int ipbatch(const char *arg);
}

// has to be in C for iptables linkage
// could be used directly from other places
// if they have a buffer ready in place.
extern "C" {
	int dobatch(char *store);
}

// this is here because its handy to use with ipbatch.
inline std::string stringprintf(const char *fmt, ...) 
{
	va_list argp;
	char buffer[BATCHSTORE_SIZE]; // should be enough for most cases - if not malloc
	std::string retstr = "";
	va_start(argp, fmt);
	
	vsnprintf(buffer, BATCHSTORE_SIZE - 1, fmt, argp);
	retstr = buffer;

	return retstr;
}
#endif
