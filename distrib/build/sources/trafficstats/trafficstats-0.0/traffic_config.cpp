#include "traffic_config.hpp"
#include <sys/stat.h>
#include <syslog.h>
#include <sstream> // for ostringstream
#include <iostream>

// take a string quanity plus units and converts to double bits  value
// coded for speed, sees NUM then optional k or m or g then bit or bps
// bit is bits bps is bytes so mult by 8

// why cant strings do this themselves?

std::string itostr(int i) {
  std::ostringstream out;
  out << i;
  return out.str();
}

std::string dtostr(double d) {
  std::ostringstream out;
  out << d;
  return out.str();
}

double to_bits(const char *in) {

    char numbit[20] = {0};
    double multiplier = 1.0;
    char c;
    int i;
    for(i = 0; i < 20 && in[i]; i++) {
        c = tolower(in[i]);
                   
        if(isdigit(c))
            numbit[i] = c;
        if(c == 'k')
            multiplier = 1024; 
        
        if(c == 'm')
            multiplier = 1024*1024; 
        
        if(c == 'g')
            multiplier = 1024*1024*1024; 
        
        if(c == 'p') {
            multiplier *= 8; // seen bps bytes per sec -> bits
            break;
        }
       
    }
    return safeatoi(numbit) * multiplier;
}
// and from a string
double to_bits(const std::string &in) {
    return to_bits(in.c_str());
}

double traffic_config::interface_speed(std::string direction) {
    struct stat st;
    std::string idx, file;
    file = std::string(MODSWROOT) + "/chosen_speeds";
    if(stat(file.c_str(), &st)) {
        // chosen_speeds not there so default to stadard ADSL for upload/download
        // and 100mbit ethernet if not
        if(direction == "upload")
            return to_bits("256kbit");
        else if(direction == "download")
            return to_bits("512kbit");
        else
            return to_bits("100mbit");
    }
    if(st.st_mtime >  chosen_speeds_last_mod) {
      chosen_speeds.readvar(file.c_str());
        chosen_speeds_last_mod = st.st_mtime;
    }
    if(direction != "")
        idx = _dev + "_" + direction;
    else
        idx = _dev;
    if(chosen_speeds[idx.c_str()] != "") {
      return to_bits(chosen_speeds[idx.c_str()]);
    }
    else {
      // try without direction
      if(direction != "") {
	idx = _dev;
	if(chosen_speeds[idx.c_str()] != "") {
	  return to_bits(chosen_speeds[idx.c_str()]);
	}
      }
      return 0.0;
    }
}
    
// need to look up the map of numbers to names last left when traffic
// implemented


int traffic_config::pos_to_rulenum(int rule) {
   
    std::ostringstream out;
    out << rule;
    std::string rstr = out.str();
    return pos_to_rulenum(rstr);
}

int traffic_config::pos_to_rulenum(const std::string &rstr) {
    struct stat st;
    std::string file = std::string(MODSWROOT) + "/rulenumbers";
    if(stat(file.c_str(), &st)) {
        // if non zero there is no rulenumbers file so return 0
        return 0;
    }
    if(st.st_mtime >  rule_numbers_last_mod) {
      rule_numbers.readvar(file.c_str());
        rule_numbers_last_mod = st.st_mtime;
    }
    return (rule_numbers[rstr.c_str()] != "" ? safeatoi(rule_numbers[rstr.c_str()]) : -1);
    
}

// change a rule (connection tracking) number into associated class

const std::string traffic_config::rule_to_classid(int rule) {
    return rule_to_classid(itostr(rule));
}

const std::string traffic_config::rule_to_classid(const std::string &res) { 
    struct stat st;     
    
    std::string file = std::string(MODSWROOT) +  "/rule2class";
    if(stat(file.c_str(), &st)) {
        // cant look up rules to classids without this file
        return res;
    }
    if(st.st_mtime >  rule_to_class_last_mod) {
      rule_to_class.readvar(file.c_str());
        rule_to_class_last_mod = st.st_mtime;
    }
   
    return (rule_to_class[res.c_str()] != "" ? rule_to_class[res.c_str()] : res);
    
}

// look for specific one if _dev not ""
const std::string traffic_config::class_name(const std::string &res) {
    struct stat st;
    std::string file = std::string(MODSWROOT) + "/classnames";
    if(stat(file.c_str(), &st)) {
        // if non zero there is no classnames file so return raw classid
      std::ostringstream log;
                
      log << "cant stat  " <<  "classnames!" << std::endl;
      syslog(LOG_WARNING,log.str().c_str());
      return res;
    }
    if(st.st_mtime >  class_names_last_mod) {
      class_names.readvar(file.c_str());
        class_names_last_mod = st.st_mtime;
    }
    if(class_names[res.c_str()] == "") {
      std::ostringstream log;
                
      log << "empty map for " << res << " in classnames" << std::endl;
      syslog(LOG_WARNING,log.str().c_str());
    }
    return (class_names[res.c_str()] != "" ? class_names[res.c_str()] : res);
   
}

// we turn rule numbers into current names
const std::string traffic_config::rule_name(int rulenum) {
    return rule_name(itostr(rulenum));
}

const std::string traffic_config::rule_name(const std::string & rulenum) {
    struct stat st;
    std::string file = std::string(MODSWROOT)  + "/rulenames";
    if(stat(file.c_str(), &st)) {
        // if non zero there is no rulenames file so return raw rule number
        return rulenum;
    }
    if(st.st_mtime >  rule_names_last_mod) {
      rule_names.readvar(file.c_str());
        rule_names_last_mod = st.st_mtime;
    }
    // return name if there is one or number
    return (rule_names[rulenum.c_str()] != "" ? rule_names[rulenum.c_str()] : "DEFAULT");
}

const std::string traffic_config::imq() {
  struct stat st;
  std::string file = std::string(MODSWROOT)  + "/2imq";
  if(stat(file.c_str(), &st)) {
    return "";
  }
  if(st.st_mtime >  to_imq_last_mod) {
    to_imq.readvar(file.c_str());
    to_imq_last_mod = st.st_mtime;
  }
  if(to_imq[_dev.c_str()] == "") {
    return "";
  }
  return to_imq[_dev.c_str()];
}
