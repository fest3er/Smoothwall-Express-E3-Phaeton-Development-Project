/* SysXtAccess Module for the SmoothWall SUIDaemon                              */
/* Contains functions relating to external Access                               */
/* (c) 2007 SmoothWall Ltd                                                      */
/* ============================================================================ */
/* Original Author  : Daniel Goscomb                                            */
/* Translated to C++: M. W. Houston                                             */
/*                                                                              */
/* include the usual headers.  iostream gives access to stderr (cerr)           */
/* module.h includes vectors and strings which are important                    */
/* ============================================================================ */
/* Modifications for Full Firewall Control by Stanford Prescott MD              */
/* ============================================================================ */
/* Combined sysportfw.cpp into this module for Full Firewall Control because    */
/* some changes will be needed here, to support FFC.                            */
/*                                          09/04/25         --Steven L Pittman */
/* **************************************************************************** */
/*         Comments for sysportfw.cpp                                           */
/* ============================================================================ */
/* include the usual headers.  iostream gives access to stderr (cerr)           */
/* module.h includes vectors and strings which are important                    */
/*============================================================================= */
/* No original author listed --  Assume that Drew S DuPont and                  */
/*                                                       Toby Long Leather      */
/*  contributed to  02/24/2008 - Stanford Prescott MD                           */
/*  collection of routines to manipulate IPTables for                           */
/*  multiple IP interfaces and Full Firewall Control                            */
/*============================================================================= */
/* Modifications:  Steven L Pittman                     08/05/18          --slp */
/*  Heavy modifications to streamline routines and reduce bulk                  */
/*  hopefully making the code easier to read.                                   */
/* ============================================================================ */
/* Modifications to improve speed and avoid the problems associated with        */
/* calls to ipbatch hanging when either there is too much execution time        */
/* or a counter overflows (the source of neither has been found)          --slp */
/* ============================================================================ */
/* Modifications to search in the vector and not add any duplicates to the      */
/* iptables rules                                          08/06/06       --slp */
/* ============================================================================ */
/* Modifications to reestablish bouncing operations  IPTables would report      */
/* negated inputs, but will not use them in mangle  having the input in         */
/* mangle where MARK is established prevented it frome being used in nat        */
/* for any interpretation  The net result is that we now have a rule for each   */
/* internal interface with a unique mark and a rule for each mark for NAT --slp */
/* ============================================================================ */
/* Modifications to support masking outbounds to alias IPs.  Red interface      */
/* detection was reworked, and source IP was added to the DNAT rules for        */
/* differentiation.  We now throw an error if the DNAT is not logical.          */
/*                                                        08/06/16        --slp */
/* ============================================================================ */
/* Modification to SNAT for Aliases to accept individually in portfw_post       */
/*                                                       08/06/23         --slp */
/* ============================================================================ */
/* Modification to use MARK vice CONNMARK because of a conflict with QoS        */
/* Modifications to jump the proxies                                            */
/*                                                       08/06/25         --slp */
/* ============================================================================ */
/* Modification to portfwf to include the destination interface                 */
/* Modifications to test protocol before assuming it is a port forward          */
/*                                                        08/07/12        --slp */
/* ============================================================================ */
/* Modifications add "FFC Log:" information to the smoothd log for errors and   */
/* successes                                              08/09/01        --slp */
/* ============================================================================ */
/* Added source interface to the DNAT entry Added error check for non-matched   */
/* input interface/cop-out to default IP from /local-ip on forwards             */
/*                                                        08/09/16        --slp */
/* ============================================================================ */
/* Added check to alias settings to determine if the alias is enabled.          */
/* Changed the CONNMARK to use the upper twelve bits only, and share with       */
/* another CONNMARK user, if it is aware of and uses a mask to not disturb our  */
/* CONNMARKs. We continue to use MARKs without mask because of stock conflicts  */
/* Added code to allow negation of the source IP or MAC.  Added code to limit   */
/* source IPs on each of the three possible interfaces to their supposed        */
/* subnets, to avoid anal race conditions.  Removed portfw_b and portfw_pre     */
/* chains from legacy FFC, and use stock chains.  Set return values to zero     */
/* to make "response" a readable value by the caller.  Added a new chain,       */
/* portfwfi, for the INPUT chain, to block possible proxy bypasses of FFC       */
/* rules.  Added code to sysxtaccess.cpp  (Could not do alisup/dn internally.)  */
/*                                                        09/05/09        --slp */
/* ============================================================================ */
/* Changed the bouncing SNAT to only be applied to packets that are coming in   */
/* and going out of the same interface.  Bumped the version to 3.1.4 to avoid   */
/* confusion, since this is for SW 3.  Slight modification to the log prefix    */
/* for the subnet limiting rules.                                               */
/*                                                        09/09/05        --slp */
/* ============================================================================ */
/* Fix a logic issue for the portfwi rules. version 3.1.5                       */
/* target logic return for ACCEPT                         09/11/14        --slp */
/* ============================================================================ */
/* Fix a logic issue for the portfwi rules. version 3.1.6                       */
/* dport was not included in portfwi                      09/12/07        --slp */
/* ============================================================================ */
/* Fix a logic issue for the portfwi rules. version 3.1.8                       */
/* portfwi rules are generated for no output interface (Any) for discrete       */
/* control of the proxies                                 10/03/16        --slp */
/* ============================================================================ */
/* Changed all error responses to have the first word in the response to be     */
/* "Abort".  Moved the subnet check rule generator to a subfunction.  Changed   */
/* ifaliasup to create an up to date list of HOME_NET for snort (also correct   */
/* long standing error for DNS_SERVERS, meant to be local DNS servers) in       */
/* /var/smoothwall/portfw/snort.var                                             */
/* a few other syntax changes.     Version 3.2.0          10/03/31        --slp */
/* ============================================================================ */
/* Error in External Access.       Version 3.2.1          10/06/02        --slp */
/* ============================================================================ */
/* Allow DHCP to bypass the subnet checks. Version 3.2.2  10/06/25        --slp */
/* ============================================================================ */
/* ifaliasup check and update aliases      Version 3.3.0  10/08/18        --slp */
/* ============================================================================ */

#include <iostream>
#include <fstream>

#include "module.h"
#include "ipbatch.h"
#include "setuid.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <glob.h>
#include <syslog.h>
#include <signal.h>
#include <unistd.h>

#define HEX "0123456789abcdefgABCDEFG"
#define MAC_HEX ":" HEX
#define INTERFACE "ethipbondrwlau" NUMBERS
#define INTERFACE_ALIAS ":" INTERFACE

extern "C"
{
 int load(std::vector<CommandFunctionPair> & );

 int set_xtaccess(std::vector<std::string> & parameters, std::string          & response);
 int set_portfw(std::vector<std::string>   & parameters, std::string          & response);
 int ifaliasup(std::vector<std::string>    & parameters, std::string          & response);
 int ifaliasdown(std::vector<std::string>  & parameters, std::string          & response);
 int get_alias(std::vector<std::string>    & parameters);
 int rmdupes(std::vector<std::string>      & parameters, const std::string    & newparm);
 int errrpt(const std::string              & parameter);
 int snet2cidr(const std::string           & parameters);
 int readether(std::vector<std::string>    & parameters, std::string    & target);
 int chkaliases(std::vector<std::string>   & parameters);
 int wrtaliases(std::string                & response);
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int load(std::vector<CommandFunctionPair> & pairs)
{
 /*
 CommandFunctionPair name("command" for external smoothcom call,
          "function" internal name of the function,
          int user that is switched to during execution,
          int group that is switched to during execution,
          int version will supersede earlier .so versions);
 */
 CommandFunctionPair set_xtaccess_function ("setxtaccess","set_xtaccess",0,0,330);
 CommandFunctionPair set_portfw_function   ("setportfw",    "set_portfw",0,0,330);
 CommandFunctionPair ifalias_down_function ("ifaliasdown", "ifaliasdown",0,0,330);
 CommandFunctionPair ifalias_up_function   ("ifaliasup",     "ifaliasup",0,0,330);

 pairs.push_back(set_xtaccess_function);
 pairs.push_back(set_portfw_function);
 pairs.push_back(ifalias_down_function);
 pairs.push_back(ifalias_up_function);

 return 0;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int set_xtaccess(std::vector<std::string> & parameters, std::string & response)
{
 std::string ifacefile    = "/var/smoothwall/red/iface";
 std::string configfile   = "/var/smoothwall/xtaccess/config";
 std::string aliasfile    = "/var/smoothwall/portfw/aliases";

 ConfigSTR ifacef(ifacefile);
 ConfigCSV config(configfile);
 ConfigCSV aliases(aliasfile);

 std::vector<std::string> ipb;
 std::string red_ip            = "";
 std::string iface             = ifacef.str();
 std::string destination       = "";
 std::string dport_out         = "";
 int error                     = 0;

 // preset the response to a success, changed only on error, always return zero

 response = "External Access Rules set";

 if (ifacef.str() == "")
 {
  response = "Abort, could not open red local interface file (" + ifacefile + ")";
  return errrpt (response);
 }
 if (iface.find_first_not_of(INTERFACE) != std::string::npos)
 {
  response = "Abort, bad interface: " + iface;
  return errrpt (response);
 }

 rmdupes(ipb, "iptables -t filter -F xtaccess");
 rmdupes(ipb, "iptables -t nat -F portfw_pre");

 //=============================================================================>
 // Any destination IP that is local and not forwarded goes to us through INPUT

 for (int line = config.first(); line == 0; line = config.next())
 {
  const std::string & alias    = config[0];
  const std::string & protocol = config[1];
  const std::string & remip    = config[2];
  const std::string & locport  = config[3];
  const std::string & enabled  = config[4];

  if (protocol.find_first_not_of(NUMBERS) != std::string::npos)
  {
   response = "Abort, bad protocol: " + protocol;
   return errrpt (response);
  }
  if (remip.find_first_not_of(IP_NUMBERS) != std::string::npos)
  {
   response = "Abort, bad remote IP: " + remip;
   return errrpt (response);
  }
  if (locport.find_first_not_of(NUMBERS_COLON) != std::string::npos)
  {
   response = "Abort, bad port: " + locport;
   return errrpt (response);
  }
  dport_out = "";
  if (locport !=  "0" && locport !=  "") dport_out = " --dport " + locport;

  red_ip = "";
  for (int aline = aliases.first(); aline == 0; aline = aliases.next())

  // we need to find the alias IP of the red interface
  {
   const std::string & f_ifalias = aliases[1];
   const std::string & f_ipaddress = aliases[3];

   if (alias == f_ifalias)
   {
    red_ip = f_ipaddress;
    break;
   }
  }
  if (red_ip == "")
  {
   response = "Abort, could not find an alias match in (" + configfile +
    ") with (" + aliasfile + ") for an IP in the XTAccess rule";
   return errrpt(response);
  }
  destination = " -d " + red_ip;

  //============================================================================>
  // This only creates the rules if they are present and enabled, the alias
  // may be disabled which would make the rule ineffective since the IP would
  // not be present in ifconfig

  if (enabled == "on")
  {
   rmdupes(ipb, "iptables -t filter -A xtaccess -i " + iface + " -d " +
    red_ip + " -p " + protocol + dport_out + " -s " + remip + " -j ACCEPT");
  }
 }

 error = ipbatch(ipb);

 if (error) response = "ipbatch failure";

 return errrpt (response);
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int set_portfw(std::vector<std::string> & parameters, std::string & response)
{
 int error = 0;
 std::string::size_type n;
 std::string::size_type p;
 std::vector<std::string>ipb;

 std::string localipfile = "/var/smoothwall/red/local-ipaddress";
 std::string ifacefile = "/var/smoothwall/red/iface";
 std::string configfile = "/var/smoothwall/portfw/config";
 std::string aliasfile = "/var/smoothwall/portfw/aliases";
 std::string sncheckfile = "/var/smoothwall/portfw/subcheck";
 ConfigSTR   localip(localipfile);
 ConfigSTR   red_iface(ifacefile);
 ConfigCSV   fwdconf(configfile);
 ConfigCSV   aliases(aliasfile);
 ConfigVAR   subnetcheck(sncheckfile);

 std::string greencheck  = subnetcheck["GREEN"];
 std::string purplecheck = subnetcheck["PURPLE"];
 std::string orangecheck = subnetcheck["ORANGE"];

 // preset the response to a success, changed only on error, always return zero

 response = "Port forwarding rules set";

 if (localip.str() == "")
 {
  response = "Abort, could not open red local IP file (" + localipfile + ")";
  return errrpt (response);
 }
 if (localip.str().find_first_not_of(IP_NUMBERS) != std::string::npos)
 {
  response = "Abort, bad local IP: " + localip.str();
  return errrpt (response);
 }
 if (red_iface.str() == "")
 {
  response = "Abort, could not open red interface file (" + ifacefile + ")";
  return errrpt (response);
 }
 if (red_iface.str().find_first_not_of(INTERFACE) != std::string::npos)
 {
  response = "Abort, bad red interface specifier: " + red_iface.str();
  return errrpt (response);
 }
 std::string red_if      = red_iface.str();
 std::string fwdest_out  = "";
 std::string dest_out    = "";
 std::string ifc_in_out  = "";
 std::string ifc_out_out = "";
 std::string fwdportdest = "";
 std::string srcipmac_out= "";
 std::string dport_out   = "";
 std::string fwdport_out = "";
 std::string prot_out    = "";
 std::string dnat_out    = "";
 std::string tgt_out     = "";
 std::string in_dest     = "";
 bool bounce_type        = false;
 bool forwarding         = false;
 bool translating        = false;
 bool negated_source     = false;
 std::string temp_source = "";
 std::string internal_if = "";
 std::string conn_green  = "0xD0000000";
 std::string conn_purple = "0xE0000000";
 std::string conn_orange = "0xF0000000";
 std::string green_if    = "";
 std::string green_ifip  = "";
 std::string orange_if   = "";
 std::string orange_ifip = "";
 std::string purple_if   = "";
 std::string purple_ifip = "";
/*
 The mark_mask value can only be used if we switch to CONNMARK tracking
 std::string mark_mask   = "0xFFF00000/";
 //=============================================================================>
 // We use the upper three nybbles of the available CMARK sequence numbers for
 // identifying the CMARKs associated with Full Firewall Control.  Any other
 // applications using CMARKs must be aware of this, always using a CMARK mask
 // that is not greater than 0x000FFFFF.  Available only if we use CONNMARK
 //=============================================================================>
*/
 rmdupes (ipb, "iptables -t filter -F portfwf");  // filtering rules  (moved stk)
 rmdupes (ipb, "iptables -t nat -F portfw");      // DNAT forwarding        (stk)
 rmdupes (ipb, "iptables -t mangle -F portfwb");  // MARK bounce & OMask    (stk)
 rmdupes (ipb, "iptables -t nat -F portfw_post"); // SNAT bounce & OMask    (add)
 rmdupes (ipb, "iptables -t filter -F portfwi");  // filter rules for INPUT (add)
 rmdupes (ipb, "iptables -t filter -F subnetchk");// filter rules FOR/INPUT (add)

 // ============================================================================>
 // we allow masking outbound to a red alias by having an association in
 // the alias file between a single internal IP and the alias interface
 // ============================================================================>
 // Here we find the internal interfaces for possible bouncing and create the
 // rules for masking outbounds to alias interfaces as we read the alias file

 unsigned int mark_seq = 0x800;

 for (int line = aliases.first(); line == 0; line = aliases.next())
 {
  char conn_mark[15] = "";

  const std::string & f_ifcolor   = aliases[0];
  const std::string & f_ifalias   = aliases[1];
  const std::string & f_realif    = aliases[2];
  const std::string & f_ipaddress = aliases[3];
  const std::string & f_addmask   = aliases[4];
  const std::string & f_enabled   = aliases[7];
  const std::string & f_mask2add  = aliases[9];
  std::string scprefixf = "iptables -t filter -A";
  std::string sclogpre = " -j LOG --log-prefix ..FFC..not.";
  std::string sclogpost = ".subnet..";
  std::string sclogrej = " -j REJECT";

  //  Allow DHCP to bypass subnet checking
  rmdupes (ipb, scprefixf + " subnetchk -p udp --dport 67 -j RETURN");
  rmdupes (ipb, scprefixf + " portfwi -j subnetchk");
  rmdupes (ipb, scprefixf + " portfwf -j subnetchk");

  //============================================================================>
  //  log and reject any packets with a source that is not in the correct
  //  subnet for the interface that received them
  //===>>  To disable:  set "(GREEN|PURPLE|ORANGE)=off" in the "sncheckfile"

  if (f_ifcolor == "GREEN")
  {
   green_if = f_realif;
   green_ifip = f_ipaddress;
   if (greencheck != "off")
   {
    char cidr[5] = "";
    sprintf((char*) cidr, "/%d", snet2cidr(f_addmask));

    std::string scpostfixl = (" -i " + green_if + " -s ! " + green_ifip + cidr +
     sclogpre + f_ifcolor + sclogpost);
    std::string scpostfixr = (" -i " + green_if + " -s ! " + green_ifip + cidr +
     sclogrej);

    rmdupes (ipb, scprefixf + " subnetchk" + scpostfixl);
    rmdupes (ipb, scprefixf + " subnetchk" + scpostfixr);
   }
  }
  if (f_ifcolor == "ORANGE")
  {
   orange_if = f_realif;
   orange_ifip = f_ipaddress;
   if (orangecheck != "off")
   {
    char cidr[5] = "";
    sprintf((char*) cidr, "/%d", snet2cidr(f_addmask));

    std::string scpostfixl = (" -i " + orange_if + " -s ! " + orange_ifip + cidr +
     sclogpre + f_ifcolor + sclogpost);
    std::string scpostfixr = (" -i " + orange_if + " -s ! " + orange_ifip + cidr +
     sclogrej);

    rmdupes (ipb, scprefixf + " subnetchk" + scpostfixl);
    rmdupes (ipb, scprefixf + " subnetchk" + scpostfixr);
   }
  }
  if (f_ifcolor == "PURPLE")
  {
   purple_if = f_realif;
   purple_ifip = f_ipaddress;
   if (purplecheck != "off")
   {
    char cidr[5] = "";
    sprintf((char*) cidr, "/%d", snet2cidr(f_addmask));

    std::string scpostfixl = (" -i " + purple_if + " -s ! " + purple_ifip + cidr +
     sclogpre + f_ifcolor + sclogpost);
    std::string scpostfixr = (" -i " + purple_if + " -s ! " + purple_ifip + cidr +
     sclogrej);

    rmdupes (ipb, scprefixf + " subnetchk" + scpostfixl);
    rmdupes (ipb, scprefixf + " subnetchk" + scpostfixr);
   }
  }
  if (f_mask2add != "" && f_mask2add.find_first_of("/") == std::string::npos &&
   f_ifalias.find_first_of(":") != std::string::npos && f_enabled == "on")

  //============================================================================>
  // Make sure we have an alias interface and a single IP to mask
  {
   sprintf((char*) conn_mark, "%#010x", (++mark_seq) * 0x100000);
   conn_mark[10] = 0;

   rmdupes (ipb, "iptables -t mangle -A portfwb -s " + f_mask2add +
    " -j MARK --set-mark " + conn_mark);

   rmdupes (ipb, "iptables -t nat -A portfw_post -o " + red_if +
    " -m mark --mark " + conn_mark + " -j SNAT --to-source " +
     f_ipaddress);
  }
  if (mark_seq > 0x8FF)
  {
   response = "Abort, more than 255 aliases are not supported, reduce alias count.";
   return errrpt (response);
  }
  if (error)
  {
   response = "Abort in alias file for " + f_ifalias + " interface.";
   return errrpt (response);
  }
 }//     <<== End of reading "aliasfile"

 //=============================================================================>
 // This is to clean up any previously entered jumps around the proxies, then
 // we set the jumps around the proxies for any outbound masking to an alias

 std::string proxy_jmph = "iptables -t nat ";
 std::string proxy_jmpt = " -m mark --mark 0x80000000/0x80000000 -j RETURN";

 // the match mark mask does not correctly mask for some reason

 rmdupes (ipb, proxy_jmph + "-D jmpim" + proxy_jmpt);
 rmdupes (ipb, proxy_jmph + "-D jmpp3scan" + proxy_jmpt);
 rmdupes (ipb, proxy_jmph + "-D jmpsip" + proxy_jmpt);
 rmdupes (ipb, proxy_jmph + "-D jmpsquid" + proxy_jmpt);

 if (mark_seq > 0x800)
 {
  rmdupes (ipb, proxy_jmph + "-I jmpim" + proxy_jmpt);
  rmdupes (ipb, proxy_jmph + "-I jmpp3scan" + proxy_jmpt);
  rmdupes (ipb, proxy_jmph + "-I jmpsip" + proxy_jmpt);
  rmdupes (ipb, proxy_jmph + "-I jmpsquid" + proxy_jmpt);
 }
 //=============================================================================>
 // Read the rules, in sequence from "configfile" and build

 for (int line = fwdconf.first(); line == 0; line = fwdconf.next())
 {
  const std::string & f_input   = fwdconf[1];
  const std::string & f_source  = fwdconf[2];
  const std::string & f_dport   = fwdconf[3];
  const std::string & f_output  = fwdconf[4];
  const std::string & f_fwdest  = fwdconf[5];
  const std::string & f_fwdport = fwdconf[6];
  const std::string & f_proto   = fwdconf[7];
  const std::string & f_action  = fwdconf[8];
  const std::string & f_enabled = fwdconf[9];

  if (f_input.find_first_not_of(INTERFACE) != std::string::npos &&
   (f_input.find_first_not_of(INTERFACE_ALIAS) != std::string::npos) &&
    f_input != "any")
  {
   response = "Abort, bad input interface: (" + f_input + ") rule number " +
    fwdconf[0];
   return errrpt (response);
  }
  if (f_output.find_first_not_of(INTERFACE) != std::string::npos &&
   (f_output.find_first_not_of(INTERFACE_ALIAS) != std::string::npos) &&
    f_output != "any")
  {
   response = "Abort, bad output interface: " + f_output + " rule number " +
    fwdconf[0];
   return errrpt (response);
  }
  if (f_proto.find_first_not_of(NUMBERS) != std::string::npos &&
   f_proto != "all")
  {
   response = "Abort, bad protocol: (" + f_proto + ") rule number " + fwdconf[0];
   return errrpt (response);
  }
  if (f_dport.find_first_not_of(NUMBERS_COLON) != std::string::npos)
  {
   response = "Abort, bad initial destination port: (" + f_dport +
    ") rule number " + fwdconf[0];
   return errrpt (response);
  }
  // First operation is to remove negation, if present

  negated_source = false;
  if ((p = f_source.find_first_of("! ")) != std::string::npos &&
   (n = f_source.find_first_not_of("! ")) != std::string::npos)
  {
     negated_source = true;
     temp_source = f_source;
     temp_source = temp_source.erase(p, n - p);
  }
  else temp_source = f_source;

  if (temp_source.find_first_not_of(IP_NUMBERS) != std::string::npos &&
   temp_source.find_first_not_of(MAC_HEX) != std::string::npos)
  {
   response = "Abort, bad source IP or MAC: (" + f_source + ") rule number " +
    fwdconf[0];
   return errrpt (response);
  }
  if (f_fwdest.find_first_not_of(IP_NUMBERS) != std::string::npos)
  {
   response = "Abort, bad new destination IP: (" + f_fwdest + ") rule number " +
    fwdconf[0];
   return errrpt (response);
  }
  if (f_fwdport.find_first_not_of(NUMBERS_COLON) != std::string::npos)
  {
   response = "Abort, bad new destination port: (" + f_fwdport +
    ") rule number " + fwdconf[0];
   return errrpt (response);
  }
  if (f_action != "REJECT" && f_action != "RETURN" && f_action != "ACCEPT" &&
   f_action  != "DROP"   &&	f_action != "LOG")
  {
   response = "Abort, bad action: (" + f_action + ") rule number " + fwdconf[0];
   return errrpt (response);
  }
  if (f_enabled == "on")
  {
   // ==========================================================================>
   // we allow red bouncing by specifying that it comes from * interface
   // because bouncing requires that DNAT and SNAT be done on the packets
   // specifying an interface on bounces is not possible, the source is the
   // destination for bounces, if the source must be limited on bounces,
   // use an IP limitation
   // ==========================================================================>
   // Here we create the source strings, or leave null if unused

   forwarding = false;
   in_dest = "";
   if (f_input.find(red_if) != std::string::npos)
   {
    if (f_proto == "1" || f_proto == "6" || f_proto == "17" ||
     f_proto == "47" || f_proto == "50" || f_proto == "51")
    {
     forwarding = true;

     // ICMP:1 TCP:6 UDP:17 GRE:47 ESP:50 AH:51   <---Forwarded protocols

     in_dest = localip.str();

     // default IP for the actual red interface, changed only for aliases
     // If it is an alias

     if (f_input.find(":") != std::string::npos)
     {
      for (int line = aliases.first(); line == 0; line = aliases.next())

      // forwarding from the red interface we need to find the IP
      {
       const std::string & f_ifalias = aliases[1];
       const std::string & f_ipaddress = aliases[3];

       if ((f_input == f_ifalias  && aliases[7] == "on"))
        in_dest = f_ipaddress;
      }
     }
     if (in_dest.find_first_not_of(IP_NUMBERS) != std::string::npos &&
      in_dest.find_first_of("/") != std::string::npos)
     {
      response = "Abort, could not find match for interface (" + f_input +
       ") and IP (" + in_dest +
        ") to construct a forwarding rule in rule number " + fwdconf[0];
      return errrpt (response);
     }
    }  //  <<== End of protocol
   }  //  <<== End of Red IF

   ifc_in_out = "";
   if (f_input != "any" && ! forwarding)

   // If it is not "any" and not being forwarded set the input interface
   {
    ifc_in_out = f_input;

    // If it is an alias interface, truncate the string starting with the colon

    if ((n = ifc_in_out.find_first_of(":")) != std::string::npos)
     ifc_in_out.erase(n, 4);

    ifc_in_out = " -i " + ifc_in_out;
   }
   ifc_out_out = "";
   if (f_output != "any" && f_output != "")
   {
    ifc_out_out = f_output;

    if ((n = ifc_out_out.find_first_of(":")) != std::string::npos)
     ifc_out_out.erase(n, 4);

    ifc_out_out = " -o " + ifc_out_out;
   }
   prot_out = "";
   if (f_proto != "all" && f_proto != "") prot_out = " -p " + f_proto;

   dest_out = "";
   if (in_dest != "") dest_out = " -d " + in_dest;

   dport_out = "";
   if (f_dport !=  "0" && f_dport !=  "") dport_out = " --dport " + f_dport;

   translating = false;
   fwdport_out = "";
   fwdportdest = "";
   if (f_fwdport !=  "0" && f_fwdport !=  "")
   {
    translating = true;
    fwdport_out = " --dport " + f_fwdport;
    fwdportdest = f_fwdport;

    if ((n = fwdportdest.find_first_of(":")) != std::string::npos)
     fwdportdest.replace(n, 1, "-");

    fwdportdest = ":" + fwdportdest;
   }

   fwdest_out = "";
   dnat_out = "";
   if (translating || forwarding)
   {
    if (f_fwdest != "" &&
      f_fwdest.find_first_of("/") == std::string::npos)

    // an IP is being forwarded, but we must have a destination IP to
    // forward to for DNAT to be sensible
    {
     fwdest_out = " -d " + f_fwdest;
     dnat_out = " -j DNAT --to-destination " + f_fwdest + fwdportdest;
    }
    // If we expected to build a DNAT rule and failed, the IP is not singular

    if (dnat_out == "")
    {
     response = "Abort, bad new destination IP (" + f_fwdest +
      "), must be single IP in rule number " + fwdconf[0];
     return errrpt (response);
    }
    // translation requires the same IP in the DNAT initial destination

    if (! forwarding) dest_out = fwdest_out;
   }
   srcipmac_out = "";
   if (temp_source.find_first_not_of(IP_NUMBERS) == std::string::npos)
   {
    // must be a space between "!" and the value

    if (negated_source) temp_source = "! " + temp_source;
    srcipmac_out = " -s " + temp_source;
   }
   if (temp_source.find_first_not_of(MAC_HEX) == std::string::npos)
   {
    if (negated_source) temp_source = "! " + temp_source;
    srcipmac_out = " -m mac --mac-source " + temp_source;
   }
   tgt_out = " -j " + f_action;
   if (f_action == "LOG") tgt_out = " -j LOG --log-prefix ..FFC..";

   // if we aren't going to DNAT we still need some information in portfwf

   if (! translating && dport_out != "") fwdport_out = dport_out;

   if (dnat_out == "" && f_fwdest != "") fwdest_out = " -d " + f_fwdest;

   // ==========================================================================>
   // Here we get down to the business of creating the rules from the strings

   if (forwarding || translating) rmdupes (ipb, "iptables -t nat -A portfw " +
    ifc_in_out + prot_out + srcipmac_out + dest_out + dport_out + dnat_out);

   rmdupes (ipb, "iptables -t filter -A portfwf -m state --state NEW" +
    ifc_in_out + ifc_out_out + prot_out + srcipmac_out + fwdest_out +
     fwdport_out + tgt_out);

   // In the INPUT chain we should not ACCEPT since EXTACCESS should be the
   // source for that type of rule, and only process rules that are not
   // forwarding or translating rules, meaning no specific output interface

   if (tgt_out == " -j ACCEPT") tgt_out = " -j RETURN";

   // Note: If the destination interface is specified then we don't create
   // a rule.  This allows the proxies to bypass a block rule for outbounds
   // which may be blocked by an additional specific rule with the Any
   // interface specified.  Another option explored was to skip only ACCEPT
   // rules, but that has limited usability as well.

   if (ifc_out_out == "")
    rmdupes (ipb, "iptables -t filter -A portfwi -m state --state NEW" +
     ifc_in_out + prot_out + srcipmac_out + dport_out + tgt_out);

   // ==========================================================================>
   // Here we create rules for bouncing if the input interface is null and
   // there has been a DNAT for the red interface, we skip translating DNAT

   if (forwarding)
   {
    std::string forward_pre = "iptables -t mangle -A portfwb -i ";
    std::string forward_post = " -j MARK --set-mark ";

    if (green_if  != "") rmdupes (ipb, forward_pre + green_if  + dest_out +
     forward_post +  conn_green);

    if (purple_if != "") rmdupes (ipb, forward_pre + purple_if + dest_out +
     forward_post + conn_purple);

    if (orange_if != "") rmdupes (ipb, forward_pre + orange_if + dest_out +
     forward_post + conn_orange);

    bounce_type = true;
   }

  }//   <<==== End of enabled

 }//   <<==== End of reading portfw/config file

 //=============================================================================>
 // The rules have been created, now we need to allow bounces, if we are doing
 // any portforwarding that allows bouncing

 if (bounce_type)
 {
  // We only need to SNAT those packets that came in the same interface that
  // they are going out on to ensure the return path is through the interface

  std::string pfw_post_pre = "iptables -t nat -A portfw_post -m mark --mark ";
  std::string pfw_post_post = " -j SNAT --to-source ";

  if (green_if != "")
   rmdupes (ipb, pfw_post_pre + conn_green  + " -o " + green_if  +
   pfw_post_post + green_ifip);

  if (purple_if != "")
   rmdupes (ipb, pfw_post_pre + conn_purple + " -o " + purple_if +
   pfw_post_post + purple_ifip);

  if (orange_if != "")
   rmdupes (ipb, pfw_post_pre + conn_orange + " -o " + orange_if +
   pfw_post_post + orange_ifip);
 }
 //=============================================================================>
 // Pass the built up vector of strings to ipbatch to build IPTables entries

 error = ipbatch(ipb);

 if (error) response = "Abort flushing rules to IPTables";
 return errrpt (response);
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
 return err;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int errrpt(const std::string & logdata)
{
 int err = 0;

 syslog(LOG_INFO, "-- FFC Log:  %s", logdata.c_str());

 return err;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int snet2cidr(const std::string & argc)
{
 int rval       = 0;
 int count      = 0;
 int counter    = 0;
 int data       = 0;
 char largc[16] = "";
 char * ref     = largc;
 bool done      = false;

 strncpy((char*) largc, argc.c_str(), 15);

 // copy 15 characters (xxx.xxx.xxx.xxx) break them at the periods, and
 // select the possible binary values of netmask converting to the CIDR

 for (counter = 0, ref = strtok((char*) largc, "."); (!done) &&
  ref != NULL; ref = strtok(NULL, "."))
 {
  data = safeatoi (ref);
  switch (data)
  {
   case 255: count+= 8; break;
   case 254: count++;
   case 252: count++;
   case 248: count++;
   case 240: count++;
   case 224: count++;
   case 192: count++;
   case 128: count++;
   default: done = true;
  }
  if ( ++counter > 3) done = true;
 }
 rval = count;

 return rval;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int wrtaliases(std::string & response)
{
 int error = 0;
 unsigned int i = 0;
 std::string varfile   = "/var/smoothwall/portfw/aliases";
 std::vector<std::string> argv;
 FILE * varhandle;

 error = chkaliases(argv);

 if (!(varhandle = fopen(varfile.c_str(), "w")))
 {
  response = "Abort, could not create or open (" + varfile + ") file";
  return errrpt(response);
 }

 while ( i < argv.size() )
 {
  fputs((char*) argv[i++].c_str(), varhandle);
 }
 fclose(varhandle);

 error += simplesecuresysteml("/bin/chown", "nobody:nobody", varfile.c_str(), NULL);

 response = "Successfully updated aliases file.";

 return errrpt(response);
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int chkaliases(std::vector<std::string> & argv)
{
 std::string aliasfile      = "/var/smoothwall/portfw/aliases";
 ConfigCSV   aliases(aliasfile);
 std::string args = "RED";
 int error = 0;

 error += readether(argv, args);
 args   = "GREEN";
 error += readether(argv, args);
 args   = "ORANGE";
 error += readether(argv, args);
 args   = "PURPLE";
 error += readether(argv, args);

 for (int line = aliases.first(); line == 0; line = aliases.next())
 {
  if (aliases[0].find_first_of(":") != std::string::npos)
  {
	std::string larg;
   unsigned int i = 0;

   while (i < 9)
   {
  	 larg += aliases[i++] + ",";
   }
   larg += aliases[9] + "\n";
   argv.push_back(larg);
  }
 }
 return 0;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int readether(std::vector<std::string> & argv, std::string & args)
{
 std::string localipfile    = "/var/smoothwall/red/local-ipaddress";
 std::string redfile        = "/var/smoothwall/red/iface";
 std::string etherfile      = "/var/smoothwall/ethernet/settings";
 std::string buildit        = "";
 char between[32]           = "";

 ConfigVAR   ether(etherfile);
 ConfigSTR   localip(localipfile);
 ConfigSTR   rediface(redfile);

 buildit += args + ",";

 if (args == "RED")
 {
  buildit += rediface.str() + ",";
  buildit += rediface.str() + ",";
  buildit += localip.str() + ",";
 }
 else
 {
  sprintf((char *)between, "%s%s", args.c_str(), "_DEV");
  if (ether[(const char *) between] == "") return 0;
  buildit += ether[(const char *) between] + ",";
  buildit += ether[(const char *) between] + ",";
  sprintf((char *)between, "%s%s", args.c_str(), "_ADDRESS");
  buildit += ether[(const char *) between] + ",";
 }
 
 sprintf((char *)between, "%s%s", args.c_str(), "_NETMASK");
 buildit += ether[(const char *) between] + ",";
 sprintf((char *)between, "%s%s", args.c_str(), "_BROADCAST");
 buildit += ether[(const char *) between] + ",";
 buildit += "on,on,,\n";

 argv.push_back(buildit);

return 0;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int ifaliasdown(std::vector<std::string> & parameters, std::string & response)
{
 std::vector<std::string> argc;
 int error = 0;
 unsigned int i = 0;

 response = "Taking alias interfaces down";
 error = errrpt(response);

 error = get_alias(argc);

 if (error)
 {
  response = "Abort reading current aliases on the interface.";
  return errrpt(response);
 }

 while ( i < argc.size() )
 {
  errrpt("pulling down (" + argc[i] + ")");
  error += simplesecuresysteml("/sbin/ifconfig", argc[i++].c_str(), "down", NULL);
 }

 response = "Successfully brought down alias interfaces.";

 if (error) response = "Abort while bringing down alias interfaces";

 return errrpt(response);
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int get_alias(std::vector<std::string> &argc)
{
 unsigned int i = 0;
 int error = 0;
 std::vector<std::string> argv;
 std::vector<std::string> args;
 char temp[10] = "";
 std::string temps = "";
 std::string::size_type n;

 args.push_back("/sbin/ifconfig");

 argv = simplesecurepopenvector(args, argc);

 while (i < argv.size())
 {
  strncpy((char*) temp, argv[i++].c_str(), 9);
  temp[9] = 0;
  temps = temp;
  if (temps.find_first_of(":") != std::string::npos)
  {
   n = temps.find_first_not_of(INTERFACE_ALIAS);
   temps.erase(n);
   argc.push_back(temps);
  }
 }
 return error;
}

//#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@>
int ifaliasup(std::vector<std::string> & parameters, std::string & response)
{
 int error = 0;

 error = wrtaliases (response);
 if (error) return errrpt(response);

 std::vector<std::string> argv;

 std::string varfile        = "/var/smoothwall/portfw/snort.var";
 std::string localipfile    = "/var/smoothwall/red/local-ipaddress";
 std::string aliasfile      = "/var/smoothwall/portfw/aliases";
 ConfigCSV   aliases(aliasfile);
	ConfigSTR   localip(localipfile);
 std::string::size_type n;
 char home_net[5000] = "";
 char * hnptr = home_net;
 FILE * varhandle;

 if (localip.str() == "")
 {
  response = "Abort, could not open red local IP file (" + localipfile + ")";
  return errrpt (response);
 }
 if (localip.str().find_first_not_of(IP_NUMBERS) != std::string::npos)
 {
  response = "Abort, bad local IP: " + localip.str();
  return errrpt (response);
 }
 std::string homenet = "var HOME_NET [" + localip.str();

 response = "Bringing alias interfaces up";
 error = errrpt(response);
 hnptr += sprintf(hnptr, "%s/32,", homenet.c_str());

 for (int line = aliases.first(); line == 0; line = aliases.next())
 {
  std::string f_color   = aliases[0];
  std::string f_ifalias = aliases[1];
  std::string f_ip      = aliases[3];
  std::string f_net     = aliases[4];
  std::string f_bcst    = aliases[5];

  if ((n = f_ifalias.find_first_of(":")) != std::string::npos &&
   aliases[7] == "on")
  {
   // Add the alias to HOME_NET if it is enabled and we have room (with
   // 255 aliases maximum should be less than 4878 plus 28 chars at finish)

   if ((n = f_color.find("RED")) != std::string::npos)
    hnptr += sprintf(hnptr, "%s/32,", f_ip.c_str());


   errrpt("bringing up alias (" + f_ifalias + ")");

   if (f_ifalias.find_first_of("p") != std::string::npos)
    error += simplesecuresysteml("/sbin/ifconfig", f_ifalias.c_str(),
     f_ip.c_str(), "netmask", f_net.c_str(), "up", NULL);
   else
   	error += simplesecuresysteml("/sbin/ifconfig", f_ifalias.c_str(), f_ip.c_str(),
    	"netmask", f_net.c_str(), "broadcast", f_bcst.c_str(), "up", NULL);
  }
 }
 hnptr += sprintf(--hnptr, "]%s", "\n");

 if (!(varhandle = fopen(varfile.c_str(), "w")))
 {
  response = "Abort, could not create or open (" + varfile + ") file";
  return errrpt(response);
 }
 fputs((char*) home_net, varhandle);
 fclose(varhandle);

 errrpt("NOTE:  snort should be restarted manually to update its HOME_NET value");
 errrpt("NOTE:  /etc/snort.conf can be further customized for each scenario");

 response = "Successfully brought up alias interfaces.";
 if (error) response = "Abort while bringing up alias interfaces";

 return errrpt(response);
}
