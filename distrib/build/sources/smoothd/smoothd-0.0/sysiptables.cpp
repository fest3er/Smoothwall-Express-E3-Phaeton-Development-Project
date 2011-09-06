/* SysShutdown Module for the SmoothWall SUIDaemon                        */
/* Contains functions relating to shutting down and rebooting             */
/* (c) 2004 SmoothWall Ltd                                                */
/* ---------------------------------------------------------------------- */
/* Original Author : D.K.Taylor                                           */

/* Based on "SmoothWall helper program - smoothiedeath/smoothierebirth    */
/* (c) Lawrence Manning, 2001                                             */

/* include the usual headers.  iostream gives access to stderr (cerr)     */
/* module.h includes vectors and strings which are important              */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>

#include "module.h"
#include "ipbatch.h"
#include "setuid.h"

#include <glob.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define GREEN_MARK 1
#define ORANGE_MARK 2
#define PURPLE_MARK 3

extern "C" {
	int load(std::vector<CommandFunctionPair> & );

	int set_incoming(std::vector<std::string> & parameters, std::string & response);
	int set_outgoing(std::vector<std::string> & parameters, std::string & response);
	int set_internal(std::vector<std::string> & parameters, std::string & response);

	int ipinsubnet(std::string netaddress, std::string netmask, std::string ip);
}

std::map<std::string, std::vector<std::string>, eqstr> portlist;

int load_portlist()
{
	/* open the portlist file for reading */

	glob_t globbuf;

	memset(&globbuf, 0, sizeof(glob_t));

	glob("/var/smoothwall/knownports/*", GLOB_DOOFFS, NULL, &globbuf);

	for (size_t i = 0; i < globbuf.gl_pathc; i++)
	{
		std::ifstream input(globbuf.gl_pathv[i]);
		char buffer[2048];

		/* determine the filename */
		char *section = globbuf.gl_pathv[i] + strlen("/var/smoothwall/knownports/");
		
		if (!input) continue;

		while (input)
		{
			if (!input.getline(buffer, sizeof(buffer)))
				break;
	
			if (strlen(buffer) > 0)
			{
				char *name = buffer;
				char *value = strstr(name, ",");	

				if (value && *value)
				{
					*value = '\0';
					value++; 
				} 
				else
					value = name;

				std::vector<std::string> & vect = portlist[section];
				vect.push_back(value);
			}
		}
		input.close();
	}

	return 0;
}

int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair  set_incoming_function("setincoming", "set_incoming", 0, 0);
	CommandFunctionPair  set_outgoing_function("setoutgoing", "set_outgoing", 0, 0);
	CommandFunctionPair  set_internal_function("setinternal", "set_internal", 0, 0);

	pairs.push_back( set_incoming_function);
	pairs.push_back( set_outgoing_function);
	pairs.push_back( set_internal_function);

	return 0;
}

int set_incoming(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;

	std::string::size_type n;
	std::vector<std::string>ipbfilter;
	std::vector<std::string>ipbnat;
	std::vector<std::string>ipbmangle;

	ConfigSTR localip("/var/smoothwall/red/local-ipaddress");
	ConfigSTR iface("/var/smoothwall/red/iface");
	ConfigCSV fwdfile("/var/smoothwall/portfw/config");
	ConfigVAR ethernet("/var/smoothwall/ethernet/settings");

	if ((error = (localip.str() == "")))
	{
		response = "Couldn't open local IP file";
		return error;
	}
	// carry on
	if ((n = localip.str().find_first_not_of(IP_NUMBERS)) != std::string::npos) 
	{
		// illegal characters
		response = "Bad local IP: " + localip.str();
		error = 1;
		return error;
	}
	if ((error = (iface.str() == "")))
	{
		response = "Couldn't open iface file";
		return error;
	}
	// carry on
	if ((n = iface.str().find_first_not_of(LETTERS_NUMBERS)) != std::string::npos) 
	{
		// illegal characters
		response = "Bad iface: " + iface.str();
		error = 1;
		return error;
	}

	ipbfilter.push_back("iptables -t filter -F portfwf");
	ipbnat.push_back("iptables -t nat -F portfw");
	ipbmangle.push_back("iptables -t mangle -F portfwb");

	// iterate the CSV
	for (int line = fwdfile.first(); line == 0; line = fwdfile.next())
	{
		std::string remdouble;
		const std::string & protocol = fwdfile[0];
		const std::string & extip = fwdfile[1];
		const std::string & locport = fwdfile[2];
		const std::string & remip = fwdfile[3];

		// this last one want to change maybe
		std::string remport = fwdfile[4];
		const std::string & enabled = fwdfile[5];
		if ((n = protocol.find_first_not_of(LETTERS)) != std::string::npos)
		{
			response = "Bad protocol: " + protocol;
			error = 1;
			return error;
		}
		if ((n = extip.find_first_not_of(IP_NUMBERS)) != std::string::npos)
		{
			response = "Bad external IP: " + extip;
			error = 1;
			return error;
		}
		if ((n = locport.find_first_not_of(NUMBERS_COLON)) != std::string::npos)
		{
			response = "Bad local port: " + locport;
			error = 1;
			return error;
		}
		if ((n = remip.find_first_not_of(IP_NUMBERS)) != std::string::npos)
		{
			response = "Bad remote IP: " + remip;
			error = 1;
			return error;
		}
		if ((n = remport.find_first_not_of(NUMBERS)) != std::string::npos)
		{
			response = "Bad remote port: " + remport;
			error = 1;
			return error;
		}

		if (enabled == "on")
		{
			if (remport == "0")
				remport = locport;
			// if locport does not have : or originally remport not zero 
			if(locport.find_first_of(":") == std::string::npos || fwdfile[4] != "0")
				remdouble = remip + ":" + remport;
			else 
				remdouble = remip;

			ipbfilter.push_back("iptables -t filter -A portfwf -m state --state NEW -p " + protocol +
				" -s " + extip +  			
				" -d " + remip +  			
				" --dport " + remport + " -j ACCEPT");
			ipbnat.push_back("iptables -t nat -A portfw -p " + protocol +
				" -s " + extip +  
				" -d " + localip.str() +  			
				" --dport " + locport + " -j DNAT --to " + remdouble);
		}
	}

	/* Rules to detect bouncing port forwarding */
	int mark = 0;
	std::string insideinterface;
	while (mark++ < 3)
	{
		switch (mark)
		{
			case 1: insideinterface = ethernet["GREEN_DEV"];  break;
			case 2: insideinterface = ethernet["ORANGE_DEV"]; break;
			case 3: insideinterface = ethernet["PURPLE_DEV"]; break;
			default: break;
		}

		if (insideinterface != "")
		{
			ipbmangle.push_back("iptables -t mangle -A portfwb -i " + insideinterface +
				" -d " + localip.str() + " -j MARK --set-mark " + stringprintf("%d", mark));

		}
	}

	error = ipbatch(ipbfilter);
	if (error)
	{
		response = "ipbatch failure (filter)";
		return error;
	}

	error = ipbatch(ipbnat);
	if (error)
	{
		response = "ipbatch failure (nat)";
		return error;
	}
	
	error = ipbatch(ipbmangle);
	if (error)
	{
		response = "ipbatch failure (mangle)";
		return error;
	}
	
	response = "Portfw rules set";
	return (error);
}

/*int set_outgoing(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	ConfigVAR settings("/var/smoothwall/outgoing/settings");
	ConfigCSV config("/var/smoothwall/outgoing/config");
	ConfigCSV machineconfig("/var/smoothwall/outgoing/machineconfig");
	std::vector<std::string>ipb;
	std::string::size_type n;
	
	load_portlist();

	ipb.push_back("iptables -F outgreen\n");
	ipb.push_back("iptables -F outorange");
	ipb.push_back("iptables -F outpurple");

	for (int line = config.first(); line == 0; line = config.next())
	{
		const std::string & colour = config[0];
		const std::string & enabled = config[1];
		const std::string & port = config[2];

		// are we complete?
		if (colour == "" || port == "" || enabled == "")
			continue;

		if (!(colour == "GREEN" || colour == "ORANGE" || colour == "PURPLE"))
		{
			response = "Bad colour: " + colour;
			error = 1;
			return error;
		}

		std::string chain = "out" + colour;
		std::transform(chain.begin(), chain.end(), chain.begin(), tolower);
		
		std::string action = settings[colour.c_str()];
		if (action == "") action = "REJECT";
		
		if (enabled == "on")
		{
			if ((n = port.find_first_not_of(NUMBERS_COLON)) != std::string::npos)
			{
				if (portlist[port.c_str()].size() > 0)
				{
					// it's a mapped port! 
					std::vector<std::string> & vect = portlist[port.c_str()];
					for (std::vector<std::string>::iterator i = vect.begin();
						i != vect.end(); i++)
					{
						std::string nport = *i;
						ipb.push_back("iptables -A " + chain + " -p tcp --dport " + nport + " -j " + action);
						ipb.push_back("iptables -A " + chain + " -p udp --dport " + nport + " -j " + action);
					}
				}
				else
				{
					response = "Bad port: " + port;
					error = 1;
					return error;
				}
			} 
			else
			{
				ipb.push_back("iptables -A " + chain + " -p tcp --dport " + port + " -j " + action);
				ipb.push_back("iptables -A " + chain + " -p udp --dport " + port + " -j " + action);
			}
		}
	}

	if (settings["GREEN"] != "ACCEPT") ipb.push_back("iptables -A outgreen -j ACCEPT");
	if (settings["ORANGE"] != "ACCEPT") ipb.push_back("iptables -A outorange -j ACCEPT");
	if (settings["PURPLE"] != "ACCEPT") ipb.push_back("iptables -A outpurple -j ACCEPT");

	ipb.push_back("iptables -F allows");

	for (int line = machineconfig.first(); line == 0; line = machineconfig.next())
	{
		const std::string & ip = machineconfig[0];
		const std::string & enabled = machineconfig[1];

		// are we complete?
		if (ip == "" || enabled == "")
			continue;
	
		if ((n = ip.find_first_not_of(IP_NUMBERS)) != std::string::npos)
		{
			response = "Bad IP: " + ip;
			error = 1;
			return error;
		}
		
		if (enabled == "on")
			ipb.push_back("iptables -A allows -s " + ip + " -j ACCEPT");
	}
	
	error = ipbatch(ipb);
	if (error)
		response = "ipbatch failure";
	else
		response = "Outbound ports set";

	return error;
}*/

int set_outgoing(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	ConfigVAR settings("/var/smoothwall/outgoing/settings");
	ConfigCSV config("/var/smoothwall/outgoing/config");
	//ConfigCSV machineconfig("/var/smoothwall/outgoing/machineconfig");
	std::vector<std::string>ipb;
	std::string::size_type n;
	
	load_portlist();

	ipb.push_back("iptables -F outgreen\n");
	ipb.push_back("iptables -F outorange");
	ipb.push_back("iptables -F outpurple");

	for (int line = config.first(); line == 0; line = config.next())
	{
		const std::string & colour = config[0];
		const std::string & enabled = config[1];
		const std::string & port = config[2];

		// are we complete?
		if (colour == "" || port == "" || enabled == "")
			continue;

		if (!(colour == "GREEN" || colour == "ORANGE" || colour == "PURPLE"))
		{
			response = "Bad colour: " + colour;
			error = 1;
			return error;
		}

		std::string chain = "out" + colour;
		std::transform(chain.begin(), chain.end(), chain.begin(), tolower);
		
		std::string action = settings[colour.c_str()];
		if (action == "") action = "REJECT";
		
		if (enabled == "x")
		{
			if ((n = port.find_first_not_of(NUMBERS_COLON)) != std::string::npos)
			{
				if (portlist[port.c_str()].size() > 0)
				{
					// it's a mapped port! 
					std::vector<std::string> & vect = portlist[port.c_str()];
					for (std::vector<std::string>::iterator i = vect.begin();
						i != vect.end(); i++)
					{
						std::string nport = *i;
						ipb.push_back("iptables -A " + chain + " -p tcp --dport " + nport + " -j " + action);
						ipb.push_back("iptables -A " + chain + " -p udp --dport " + nport + " -j " + action);
					}
				}
				else
				{
					response = "Bad port: " + port;
					error = 1;
					return error;
				}
			} 
			else
			{
				ipb.push_back("iptables -A " + chain + " -p tcp --dport " + port + " -j " + action);
				ipb.push_back("iptables -A " + chain + " -p udp --dport " + port + " -j " + action);
			}
		}
	}

	ipb.push_back("iptables -A outgreen -j ACCEPT");
	ipb.push_back("iptables -A outorange -j ACCEPT");
	ipb.push_back("iptables -A outpurple -j ACCEPT");
/*
	ipb.push_back("iptables -F allows");

	for (int line = machineconfig.first(); line == 0; line = machineconfig.next())
	{
		const std::string & ip = machineconfig[0];
		const std::string & enabled = machineconfig[1];

		// are we complete?
		if (ip == "" || enabled == "")
			continue;
	
		if ((n = ip.find_first_not_of(IP_NUMBERS)) != std::string::npos)
		{
			response = "Bad IP: " + ip;
			error = 1;
			return error;
		}
		
		if (enabled == "on")
			ipb.push_back("iptables -A allows -s " + ip + " -j ACCEPT");
	}
*/	
	error = ipbatch(ipb);
	if (error)
		response = "ipbatch failure";
	else
		response = "Outbound ports set";

	return error;
}

int set_internal(std::vector<std::string> & parameters, std::string & response)
{
	int error = 0;
	ConfigCSV config("/var/smoothwall/dmzholes/config");
	std::vector<std::string>ipb;
	std::string::size_type n;
	
	load_portlist();

	ipb.push_back("iptables -F dmzholes");

	for (int line = config.first(); line == 0; line = config.next())
	{
		const std::string & protocol = config[0];
		const std::string & locip = config[1];
		const std::string & remip = config[2];
		const std::string & remport = config[3];
		const std::string & enabled = config[4];

		// are we complete?
		if (protocol == "" || locip == "" || remip == "" || remport == "" || enabled == "")
			continue;

		if ((n = protocol.find_first_not_of(LETTERS)) != std::string::npos)
		{
			response = "Bad protocol: " + protocol;
			error = 1;
			return error;
		}
		if ((n = locip.find_first_not_of(IP_NUMBERS)) != std::string::npos)
		{
			response = "Bad local IP: " + locip;
			error = 1;
			return error;
		}
		if ((n = remip.find_first_not_of(IP_NUMBERS)) != std::string::npos)
		{
			response = "Bad remote IP: " + remip;
			error = 1;
			return error;
		}
		
		if (enabled == "on")
		{
			if ((n = remport.find_first_not_of(NUMBERS_COLON)) != std::string::npos)
			{
				if (portlist[remport.c_str()].size() > 0)
				{
					/* it's a mapped port! */
					std::vector<std::string> & vect = portlist[remport.c_str()];
					for (std::vector<std::string>::iterator i = vect.begin();
						i != vect.end(); i++)
					{
						std::string nport = *i;
						ipb.push_back("iptables -A dmzholes -m state --state NEW -p " + protocol + " -s " + locip + " -d " + remip + " --dport  " + nport + " -j ACCEPT");
					}
				}
				else
				{
					response = "Bad remote port: " + remport;
					error = 1;
					return error;
				}
			}
			else
				ipb.push_back("iptables -A dmzholes -m state --state NEW -p " + protocol + " -s " + locip + " -d " + remip + " --dport  " + remport + " -j ACCEPT");
		}
	}

	error = ipbatch(ipb);

	if (error)
		response = "ipbatch failure";
	else
		response = "DMZ Holes set";

	return (error);
}

int ipinsubnet(std::string netaddress, std::string netmask, std::string ip)
{
	in_addr_t netaddressaddr = inet_addr(netaddress.c_str());
	in_addr_t netmaskaddr = inet_addr(netmask.c_str());
	in_addr_t ipaddr = inet_addr(ip.c_str());
	
	if (netaddressaddr == INADDR_NONE || netmaskaddr == INADDR_NONE) return 0;
	
	if ((ipaddr & netmaskaddr) == netaddressaddr)
		return 1;
	else
		return 0;
}
