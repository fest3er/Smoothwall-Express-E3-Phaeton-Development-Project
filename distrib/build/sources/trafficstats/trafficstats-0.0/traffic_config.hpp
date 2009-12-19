#ifndef TRAFFIC_CONFIG_INC
#define TRAFFIC_CONFIG_INC

#include <smoothwall/config.h>
#include <time.h>
#include <string>
#include <map>



// typedef class ConfigVar ConfigVar;

class traffic_config {
private:
  std::string _dev;
  // our various config files
  ConfigVAR rule_names;
  ConfigVAR rule_numbers;
  ConfigVAR class_names; 
  ConfigVAR rule_to_class;
  ConfigVAR chosen_speeds;
  ConfigVAR to_imq;
  time_t rule_names_last_mod;
  time_t rule_numbers_last_mod;
  time_t class_names_last_mod;
  time_t rule_to_class_last_mod;
  time_t chosen_speeds_last_mod;
  time_t to_imq_last_mod;
    
public:

  traffic_config() {
    _dev = "";
    rule_names_last_mod = 0; 
    rule_numbers_last_mod = 0;
    class_names_last_mod = 0;
    rule_to_class_last_mod = 0;
    chosen_speeds_last_mod = 0; 
    to_imq_last_mod = 0;
  };
  void set_dev(std::string pdev) { _dev = pdev; };
  const std::string dev() { return _dev; };
  double interface_speed(std::string direction = "");
  int pos_to_rulenum(const int rule);
  int pos_to_rulenum(const std::string &rstr);
  const std::string rule_to_classid(const int rule);
  const std::string rule_to_classid(const std::string &rstr);
  const std::string class_name(const std::string &res);
  const std::string rule_name(const int rulenum);
  const std::string rule_name(const std::string & rulenum);
  const std::string imq();
};

std::string itostr(int i);
std::string dtostr(double d);
 


// all our config info is maps indexed by device 
typedef std::map<std::string, traffic_config> traf_config_hash;
typedef traf_config_hash ::iterator traf_config_hash_iterator;
typedef traf_config_hash ::const_iterator traf_config_hash_const_iterator;


// defined in the main prog
extern traf_config_hash traf_config;
#endif
