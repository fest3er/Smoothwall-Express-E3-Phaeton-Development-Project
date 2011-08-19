/* SysTimedOutbound Module for the SmoothWall SUIDaemon                      */
/* Contains functions relating to outbound control and timed access          */
/* SmoothWall and smoothd (c) SmoothWall Ltd                                 */
/* This code (c) Stan Prescott                                               */
/*===========================================================================*/
/* include the usual headers.  iostream gives access to stderr (cerr)        */
/* module.h includes vectors and strings which are important                 */
/*===========================================================================*/
/* Straightened out the nesting on the damn PERL "else if" bug.  In c "else" */
/* and "if" are two seperate keywords, and create their own nest, so using   */
/* them both together creates two layers of nesting, where PERL treats them  */
/* as a selector and does not extend the nesting.  Added rmdupes and errrpt  */
/* back into the code.  Removed the label and the "goto" statements.         */
/*                              110508                                 --slp */
/*===========================================================================*/
/* Redesigned this module to use the iptables add-on modules xt_iprange,     */
/* xt_multiport and xt_time that allow use of multiple IP addresses and      */
/* IP ranges, multiple ports and ranges and multiple time frames in a        */
/* single rule that are managed by iptables and not by multiple iptables     */
/* rules or by programs external to the smoothd module and iptables. Once    */
/* again, this would not have been possible without considerable contribu-   */
/* tions from Steve Pittman who helped design much of the logic in this      */
/* module.                                                                   */
/*                              110801                                 --stp */
/*===========================================================================*/
/* Rearranged logic to use only a single area to output the various rules    */
/* Simplified and labeled the logic for ease of maintenance                  */
/* Reduced the overhead of the various output statements                     */
/*                              110809                                 --slp */
/*===========================================================================*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>

#include "module.h"
#include "ipbatch.h"
#include "setuid.h"

#include <glob.h>

#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMBERS "0123456789"
#define HEX "0123456789abcdefABCDEF"
#define LETTERS_NUMBERS LETTERS NUMBERS
#define MULTI_PORTS "0123456789:,"
#define MAC_HEX HEX ":"
#define IP_RANGE IP_NUMBERS "-"

extern "C" {
   int load(std::vector<CommandFunctionPair> & );
   int set_timedoutgoing(std::vector<std::string>    & parameters, std::string       & response);
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
   CommandFunctionPair  set_timedoutgoing_function("settimedoutgoing", "set_timedoutgoing", 0, 0);

   pairs.push_back( set_timedoutgoing_function);

   return 0;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int set_timedoutgoing(std::vector<std::string> & parameters, std::string & response)
{
   int error = 0;

   ConfigVAR settings("/var/smoothwall/outgoing/config_files/settings");
   ConfigVAR netsettings("/var/smoothwall/ethernet/settings");
   ConfigCSV config("/var/smoothwall/outgoing/config_files/config");
   std::vector<std::string>ipb;
   std::string::size_type n;

   std::string gdev = netsettings["GREEN_DEV"];
   std::string odev = netsettings["ORANGE_DEV"];
   std::string pdev = netsettings["PURPLE_DEV"];
   std::string chain = "iptables -A timedoutgoing";

   load_portlist();

   rmdupes(ipb, "iptables -F timedoutgoing");
   rmdupes(ipb, "iptables -F tofcdrop");

   rmdupes(ipb, chain + " -p tcp -i " + gdev + " --dport 441 -j RETURN");

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

      if (colour == "ORANGE")
            input_dev = " -i " + odev;
      if (colour == "GREEN")
            input_dev = " -i " + gdev;
      if (colour == "PURPLE")
            input_dev = " -i " + pdev;
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
           
         if ( action == "REJECT" )
            action2 = " -j tofcdrop";
         else
            action2 = " -j RETURN";
                           
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
            std::replace(tmpports.begin(), tmpports.end(), '-', ','); 
                           // replace all hyphens with commas
            
            if (tmpports.find_first_not_of(MULTI_PORTS) == std::string::npos)
            {
               tmpports = " -m multiport --ports " + tmpports;
            }
            else
            { 
            // it's an entry from the application or service(s) menu
               
               if (portlist[port.c_str()].size() > 0)
               {
                     /* it's a mapped port! */
                  
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
               else
               {
               // User must have somehow entered rubbish from the Application or services(s) menu
                  
                  response = "Bad entry from services menu: " + port;
                  return errrpt(response);
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
                    
         if (protocol == "ICMP" )
         {
            protos.push_back(" -p icmp");
         }
          
         if ( protocol == "IPSEC" )
         {
            protos.push_back(" -p 50");
            protos.push_back(" -p 51");
            protos.push_back(" -p 17 --dport 500");
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
               rmdupes(ipb, chain + input_dev + ipormac + protos[x] + full_time_str[z] + action2);
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
      rmdupes(ipb, rulehead + gdev + relestab +  " -j RETURN");
      
   if (settings["GREEN_REJECTS"] == "on") 
      rmdupes(ipb, rulehead + gdev + log_prefix);
          
   rmdupes(ipb, rulehead + gdev + " -j REJECT");
      
   if (settings["GREEN"] == "REJECT" || settings["GREEN"] == "CLOSED") 
      rmdupes(ipb, chain + " -i " + gdev + " -j tofcdrop");
    
   if (odev != "")
   {
      if (settings["ORANGE_RELATED"] == "on") 
         rmdupes(ipb, rulehead + odev + relestab + " -j RETURN");
          
      if (settings["ORANGE_REJECTS"] == "on") 
         rmdupes(ipb, rulehead + odev + log_prefix);
          
      rmdupes(ipb, rulehead + odev + " -j REJECT");
      
      if (settings["ORANGE"] == "REJECT" || settings["ORANGE"] == "CLOSED") 
         rmdupes(ipb, chain + " -i " + odev + " -j tofcdrop");
   }
    
   if (pdev != "")
   {
      if (settings["PURPLE_RELATED"] == "on") 
         rmdupes(ipb, rulehead + pdev + relestab + " -j RETURN");
           
      if (settings["PURPLE_REJECTS"] == "on") 
         rmdupes(ipb, rulehead  + pdev + log_prefix);
          
      rmdupes(ipb, rulehead + odev + " -j REJECT");
      
      if (settings["PURPLE"] == "REJECT" || settings["PURPLE"] == "CLOSED") 
         rmdupes(ipb, chain + " -i " + pdev + " -j tofcdrop");
   }
    
   error = ipbitch(ipb);
    
   if (error)
      response = "IPTables failure";
   else
      response = "Outbound ports set";
      
   return errrpt(response);
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
