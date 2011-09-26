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

#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMBERS "0123456789"
#define HEX "0123456789abcdefABCDEF"
#define LETTERS_NUMBERS LETTERS NUMBERS
#define MULTI_PORTS "0123456789:,"
#define MAC_HEX HEX ":"
#define IP_RANGE IP_NUMBERS "-"

extern "C" {
	int load(std::vector<CommandFunctionPair> & );

	int set_outgoing(std::vector<std::string> & parameters, std::string & response);
	int set_internal(std::vector<std::string> & parameters, std::string & response);

   	int rmdupes(std::vector<std::string>              & parameters, const std::string & newparm);
   	int errrpt(const std::string                      & parameter);
   	int ipbitch(std::vector<std::string>              & parameters);
}

std::map<std::string, std::vector<std::string>, eqstr> portlist;

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
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

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int load(std::vector<CommandFunctionPair> & pairs)
{
	/* CommandFunctionPair name("command", "function"); */
	CommandFunctionPair  set_outgoing_function("setoutgoing", "set_outgoing", 0, 0);
	CommandFunctionPair  set_internal_function("setinternal", "set_internal", 0, 0);

	pairs.push_back( set_outgoing_function);
	pairs.push_back( set_internal_function);

	return 0;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int set_outgoing(std::vector<std::string> & parameters, std::string & response)
{
   int error = 0;

   ConfigVAR settings("/var/smoothwall/outgoing/settings");
   ConfigVAR netsettings("/var/smoothwall/ethernet/settings");
   ConfigCSV config("/var/smoothwall/outgoing/config");
   std::vector<std::string>ipb;
   std::string::size_type n;

   std::string gdev = netsettings["GREEN_DEV"];
   std::string odev = netsettings["ORANGE_DEV"];
   std::string pdev = netsettings["PURPLE_DEV"];
   std::string chain = "iptables -A timedoutgoing";

   load_portlist();

   rmdupes(ipb, "iptables -F timedoutgoing");
   rmdupes(ipb, "iptables -F tofcdrop");

   for (int line = config.first(); line == 0; line = config.next())
   {
      const std::string & colour = config[0];
      const std::string & enabled = config[1];
      const std::string & port = config[2];
      const std::string & protocol = config[4];
      const std::string & srcipmac = config[5];
      const std::string & action = config[6];
      const std::string & timed = config[8];

      std::string input_dev  = "";

      if (colour == "" || port == "" || enabled == "")
         continue;   
            /* Skip lines with these values empty */

      if (colour == "GREEN")
      {
            // GREEN is always configured
            input_dev = " -i " + gdev;
      }
      if (colour == "ORANGE")
      {
            if (odev == "" ) continue;  // skip if ORANGE is not configured
            input_dev = " -i " + odev;
      }
      if (colour == "PURPLE")
      {     
            if (pdev == "" ) continue;  // skip if PURPLE is not configured
            input_dev = " -i " + pdev;
      }
      if ( input_dev == "" )
      {
         response = "Bad colour: " + colour;
         return errrpt(response);
      }
                           
                            /* ===========Action============= */
                            
      if (enabled == "on")
      {
         std::string ipormac = "";
         std::string action2 = "";
         std::string daysow = "";
         std::string time_str = "";
         std::string tmpdays = "";
         std::string tmpports = "";
         std::vector<std::string> full_time_str;
         std::vector<std::string> protos;
           
         if ( action == "ACCEPT" ) action2 = " -j ACCEPT";
                           
                            /* ===========Source============= */   
                                       
         if ((n = srcipmac.find_first_not_of(IP_NUMBERS)) == std::string::npos)
         {
            ipormac = " -s " + srcipmac;
         }
         else
         {
            if ((n = srcipmac.find_first_not_of(MAC_HEX)) == std::string::npos)
            {
               ipormac = " -m mac --mac-source " + srcipmac;
            }
            else
            {
               if ((n = srcipmac.find_first_not_of(IP_RANGE)) == std::string::npos)
               {
                  ipormac = " -m iprange --src-range " + srcipmac;
               }
               else
               {
                  if (srcipmac == "N/A")
                  {
                     ipormac = "";
                  }
                  else
                  {
                     response = "Bad IP or MAC: " + srcipmac;
                     return errrpt(response);
                  }
               }
            }
         }
                           
                            /* =========Timed========== */
                             
         unsigned int myx = 10;
         if (timed == "on") // <<= it's a timed rule!
         {
            while (config[myx+2] != "")
            {
               const std::string & daysofweek = config[myx++];
               const std::string & timestart = config[myx++];
               const std::string & timestop = config[myx++];
                
               if (daysofweek != "")
               {
                  tmpdays = daysofweek;
                  std::replace( tmpdays.begin(), tmpdays.end(), ' ', ','); 
                                  // replace all spaces with commas
                  daysow = " --weekdays " + tmpdays;
               }
               else
               {
                  daysow = "";
               }
               time_str = " --timestart " + timestart + " --timestop " + timestop;
               full_time_str.push_back(" -m time" + daysow + time_str);
            }
         }
         else
         {
            full_time_str.push_back(" ");
         }
                           
                            /* ==========Port=========== */         
                            
         if (port != "N/A")
         {
            tmpports = port;

            // replace all hyphens with commas
            std::replace(tmpports.begin(), tmpports.end(), '-', ','); 
            
            if (tmpports.find_first_not_of(MULTI_PORTS) == std::string::npos)
            {
		if (tmpports.find_first_not_of(NUMBERS_COLON) == std::string::npos)
		{
		   /* It's a single port */

		   tmpports = " --dport " + tmpports;
		}
		else
		{
		   /* It's a multiport */

                 tmpports = " -m multiport --ports " + tmpports;
		}
            }
            else
            { 
            /* it's an entry from the application or service(s) menu (mapped port) */
               
               if (portlist[port.c_str()].size() > 0)
               {                  
                  std::string nport = "";
                  std::vector<std::string> & vect = portlist[port.c_str()];
                  unsigned int i = 0;
                  while (i < vect.size())
                  {
                     if (strlen ( nport.c_str() ) > 0) nport += ",";
                     nport += vect[i++].c_str();
                  }
                  tmpports = " -m multiport --ports " + nport;
               }
            }
         }
         else tmpports = "";  // if the port == N/A then empty tmpports
                           
                            /* =========Protocol========== */
          
         if ( protocol == "TCP" || protocol == "Both" )
         {
            protos.push_back(" -p 6" + tmpports);
         }
                    
         if ( protocol == "UDP" || protocol == "Both" )
         {
            protos.push_back(" -p 17" + tmpports);
         }
                    
         if ( protocol == "IPSEC" )
         {
            protos.push_back(" -p 50");
            protos.push_back(" -p 51");
         }
          
         if ( protocol == "PPTP" )
         {
            protos.push_back(" -p 47");
         }
          
         if ( ! (protos.size()) )
         {
            response = "Bad entry for protocol: " + protocol;
            return errrpt(response);
         }
                           
                            /* =========Output========= */
                             
         unsigned int x = 0;
         while ( x < protos.size() )
         {
            unsigned int z = 0;
            while ( z < full_time_str.size() )
            {
               rmdupes(ipb, chain + protos[x] + input_dev + ipormac + full_time_str[z] + action2);
               z++;
            }
            x++;
         }
      } //      <<= End of enabled
   } //   <<= End reading config

   // <<= Begin setting up tofcdrop drop table
   // Log then drop packets and allowing ESTABLISHED,RELATED through drop table tofcdrop
   
   std::string log_prefix = " -j LOG --log-prefix Denied-by-outgoing-rules";
   std::string rulehead = "iptables -A tofcdrop -i ";
   std::string relestab = " -m state --state ESTABLISHED,RELATED";
     
   if (settings["GREEN_RELATED"] == "on") 
      rmdupes(ipb, rulehead + gdev + relestab +  " -j ACCEPT");
      
   if (settings["GREEN_REJECTS"] == "on") 
      rmdupes(ipb, rulehead + gdev + log_prefix);
    
   if (settings["GREEN"] == "ACCEPT") 
      rmdupes(ipb, "iptables -A timedoutgoing -i " + gdev +  " -j ACCEPT");

   if (odev != "")
   {
      if (settings["ORANGE_RELATED"] == "on") 
         rmdupes(ipb, rulehead + odev + relestab + " -j ACCEPT");
          
      if (settings["ORANGE_REJECTS"] == "on") 
         rmdupes(ipb, rulehead + odev + log_prefix);

      if (settings["ORANGE"] == "ACCEPT") 
         rmdupes(ipb, "iptables -A timedoutgoing -i " + odev +  " -j ACCEPT");
   }
    
   if (pdev != "")
   {
      if (settings["PURPLE_RELATED"] == "on") 
         rmdupes(ipb, rulehead + pdev + relestab + " -j ACCEPT");
           
      if (settings["PURPLE_REJECTS"] == "on") 
         rmdupes(ipb, rulehead  + pdev + log_prefix);

      if (settings["PURPLE"] == "ACCEPT") 
         rmdupes(ipb, "iptables -A timedoutgoing -i " + pdev +  " -j ACCEPT");
   }
    
   rmdupes(ipb, chain + relestab + " -j ACCEPT");
   rmdupes(ipb, chain + " -j tofcdrop");
   rmdupes(ipb, "iptables -A tofcdrop -j REJECT --reject-with icmp-admin-prohibited");

   error = ipbitch(ipb);
    
   if (error)
      response = "IPTables failure";
   else
      response = "Outbound ports set";
      
   return errrpt(response);
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

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int rmdupes( std::vector<std::string> & argv, const std::string & argc)
{
   unsigned int i = 0;
   int err = 0;

   while ( i < argv.size() )
   {
      if ( argc == argv[ i++ ] ) return err;
   }
   argv.push_back(argc);

/*  ##DEBUG##   */
//    errrpt(argc);


   return err;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int errrpt(const std::string & logdata)
{
   int err = 0;

   syslog(LOG_INFO, "-- TOFC Log:  %s", logdata.c_str());

   return err;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int ipbitch(std::vector<std::string> &arg)
{
 std::string tracker = "";
 unsigned int i = 0;
 int err = 0;
 FILE * output = popen( "/usr/sbin/ipbatch", "w" );

 while (i < arg.size())
 {
  tracker = arg[i++] + " \0";
  errrpt("-> " + tracker);
  fprintf(output, "%s\n", tracker.c_str());
  fflush(output);
 }
 fprintf(output, "commit\n");
 fflush(output);
 fprintf(output, "end\n");
 fflush(output);
 
 err = pclose(output);
 return err;
}
