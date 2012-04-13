// C++ interface between smoothwall and tc/netfilter/ipt_ACCOUNT statistics collection
// (c) SmoothWall Ltd 2007
// This code is distributed under the terms of the GPL v2.
// Written by Martin Houston <martin.houston@smoothwall.net> 2004 - 2007


extern "C" {
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/pkt_sched.h>
#include <resolv.h>
#include <libnetlink.h>
#include <ll_map.h>
#include <rtm_map.h>
// has to be C linkage, as iptables stuff wont compile with C++
#include "trafstats_iptables.h"
}

#include <string>
#include <iostream>
#include <iterator>
#include <ostream>
#include <istream>
#include <sstream>
#include <smoothwall/config.h>
#include "trafstats_core.hpp"

static bool ctx_good = false;
static struct ipt_ACCOUNT_context ctx;

// constructor for an individual stats snapshot
traf_stat_collection_item::traf_stat_collection_item(bool interfaces_only, bool use_account) {
	Vtraf_stat in,out,uptmp,dntmp;
	Vtraf_iterator ia;
	StrSet_iterator d;
	
	// oh this would be sooo much easier in perl
	
	// make sure these are all empty
	collection_start.t.tv_sec = 0;
	collection_start.t.tv_usec = 0;
	collection_end.t.tv_sec = 0;
	collection_end.t.tv_usec = 0;
	
	// clear all the vectors
	int_devs.clear();
	ext_devs.clear();
	classes.clear();
	rules.clear();
	addresses.clear();
	stats.clear(); 
	
	in.clear(); 
	out.clear();
	
	// pre/postrouting-1 contains entries for all possible
	// internal interfaces, even ones that are not up
	// ipsec and ppp9+ are considered internal but are really to do with VPN
	in = list_rules("prerouting-1");
	out = list_rules("postrouting-1");
	
	// timestamp this lot
	gettimeofday(&collection_start.t, NULL);
	for(ia = in.begin(); ia != in.end(); ia++) {
		// use a ref so debugger can see this stuff!
		const std::string &dev = ia->dev;
		if(dev.size() > 0) {
			int_devs.insert(dev); // remember dev
			std::string label = dev + "_int_in"; 
			traf_stat &item = *ia;
			stats[label] = item;
		}
	} 
	
	for(ia = out.begin(); ia != out.end(); ia++) {
		const std::string &dev = ia->dev;
		if(dev.size() > 0) {
			// e.g ethA_int_out
			std::string label = dev + "_int_out";
			traf_stat &item = *ia;
			stats[label] = item;
		}
	}
	
	// external interfaces should exist in iptables even if
	// not up. Get ppp and ippp 
	// may not always have external interfaces
	in = list_rules("prerouting-2"); 
	out = list_rules("postrouting-2");
	
	for(ia = in.begin(); ia != in.end(); ia++) { 
		const std::string &dev = ia->dev;
		if(dev.size() > 0) {
			ext_devs.insert(dev); // remember dev
			std::string label = dev + "_ext_in";
			// std::cerr << label << std::endl;
			traf_stat &item = *ia;
			stats[label] = item;
		}
	} 

	for(ia = out.begin(); ia != out.end(); ia++) {
		const std::string &dev = ia->dev;
		if(dev.size() > 0) {
			std::string label = dev + "_ext_out";
			traf_stat &item = *ia;
			stats[label] = item;
		}
	
	}
	if(!interfaces_only) {
		// go through internal then external devices - potentialy expensive as traffic makes many rules
		for(d = int_devs.begin(); d != ext_devs.end(); d++) {
			if(d == int_devs.end())
				d = ext_devs.begin();
			if(d == ext_devs.end())
				break;
			// iterator can be considerd as a pointer to the current element in the vector
			// so we make a reference for easier access
			const std::string &dev = *d;
			// get defaults if not present
		
			traf_config[dev].set_dev(dev);
	

			traffic_config & myconf = traf_config[dev];
		
			uptmp = list_rules_for_dev(dev, "up");	
			dntmp = list_rules_for_dev(dev, "dn");
		
			if(uptmp.size() > 0) {
				for(ia = uptmp.begin(); ia != uptmp.end(); ia++) {
					// and store them 
					std::string rule_name = myconf.rule_name(ia->rule_num);
					rules.insert(rule_name);
					std::string label = dev + "_up_rule_" + rule_name; 
					traf_stat &item = *ia;
					stats[label] = item;
				}
			}
			if(dntmp.size() > 0) {
				for(ia = dntmp.begin(); ia != dntmp.end(); ia++) {
					// and store them 
					std::string rule_name = myconf.rule_name(ia->rule_num);
					rules.insert(rule_name);
					std::string label = dev + "_dn_rule_" + rule_name; 
			
					traf_stat &item = *ia;
					stats[label] = item; 
				}
			}
			// upload classes are related to the dev
			// download classes are related to the imq partner (if present)
			uptmp = list_class(dev);
			if(uptmp.size() > 0) {
				for(ia = uptmp.begin(); ia != uptmp.end(); ia++) {
					// and store them
					std::string class_name = myconf.class_name(ia->classid);
					classes.insert(class_name);
					std::string label = dev + "_up_class_" + class_name;
					traf_stat &item = *ia;
					stats[label] = item;
				}
			}
		
			const std::string & imq = myconf.imq();
			if(imq.size() > 0) {
				dntmp = list_class(imq);
				if(dntmp.size() > 0) {
					for(ia = dntmp.begin(); ia != dntmp.end(); ia++) {
						// and store them
						std::string class_name = myconf.class_name(ia->classid);
						classes.insert(class_name);
						std::string label = dev + "_dn_class_" + class_name; 
						traf_stat &item = *ia;
						stats[label] = item;
					}
				}
			}
		}
	} // end of interfaces_only
    if(use_account) {
		// get all the address records we know about
		uptmp = list_account();
		if(uptmp.size() > 0) {
			for(ia = uptmp.begin(); ia != uptmp.end(); ia++) {
				traf_stat &item = *ia;
				// and store them
				std::string label = std::string(item.addr_as_string()) + " " + item.account_table +" (" + item.direction + ")";
				// syslog(LOG_WARNING, "Getting stats for %s\n", label.c_str());
				addresses.insert(label);
				// label is the address
				stats[label] = item;
			}
		}

	}

		 
	// this should not have taken too long...
	gettimeofday(&collection_end.t, NULL);
	
}

Vstring traf_stat_collection_item::class_indexes_in_order() {
	traf_stat_hash_iterator si;
		
	Vstring idxes;
	Vstring_iterator fs;
	Vtraf_stat classstats; // tmp so can sort
	Vtraf_iterator i;
	std::string::size_type pos; 
		
	for(si = stats.begin(); si != stats.end(); si++) { 
		pos = si->first.find("_class_");
		if(pos != std::string::npos) {
			// a class 
			classstats.push_back(si->second);
		}
	}
	sort(classstats.begin(),classstats.end());
	for(i = classstats.begin(); i != classstats.end(); i++) {
		std::string classname = traf_config[i->dev].class_name(i->classid);
		fs = find(idxes.begin(), idxes.end(), classname);
		if(fs == idxes.end())
			// only one copy of each
			idxes.push_back(classname);
	}
	return idxes;
	
}

// keeps latest known stats for each label in the latest_stats global.

void traf_stat_collection_item::compress () {
	traf_stat_hash_iterator si;
	StrSet erasethese;
	StrSet_iterator e;
	for(si = stats.begin(); si != stats.end(); si++) {
		const std::string &label = si->first;
		traf_stat &data = si->second;
		
		if(latest_stats.count(label)) {

			traf_stat &latest = latest_stats[label];
			// see if things have changed since last time
			// have we wrapped around? Most likley new rules have been added
			if(data.bytes() < latest.bytes()) {
				std::ostringstream log;
				
				log << "counter wrap around for " << label << " old = (" << latest << ") new = (" << data << ")" << std::endl;
				syslog(LOG_WARNING,log.str().c_str());
				
				// zero the older counters, get the last part of the
				// wrap around at least
				latest.stats.bytes = 0; 
				latest.stats.packets = 0; 
			}

			// now normal data cases
			if(data.bytes() == latest.bytes()) {
				// nothing has changed - dont keep this
				// std::cout << "erasing " << label << std::endl;
				erasethese.insert(label);
				
			}
			else {
				// update the latest to be this
				calculate_rates(latest, data); 
				// std::cout << "delta " << data.bytes() - latest.bytes() << " for " << label << std::endl;
				
				latest_stats[label] = data;
			}
		}
		else {
			// there is no latest so we are it
			// std::cout << "1st sample for " << label << std::endl;
			latest_stats[label] = data;
		}
	}
	// do erasing once iteration has finished - otherwise messes up loop
	for(e = erasethese.begin(); e != erasethese.end(); e++) {
		stats.erase(*e);
	}
	// are we empty now - so can loose the dev maps too
	// all there is left is the timestamp,
	// but that is enough to note that we did look but nothing
	// changed
		
	if(stats.size() == 0) {
		// std::cout << "now empty sample " << std::endl;
		int_devs.clear();
		ext_devs.clear();
		classes.clear();
		rules.clear();
	}
}

// indicate if the rules we need are minimally in place
// this uses an arrangement of IPtables with the SmoothWall AF
// the 'prerouting-1' mangle table is a point for counting all
// traffic comming from an external interface.
// if this is not there then we assume that none of the iptables
// infrastructure we need has been set up yet.
// constructor - go fetch current stats!

bool traffic_iptables_missing() {
	struct iptable_data res[1];
	
	// If the INTERNAL tables have no entres yet then inhibit all stats
	// collection.
	// try to fetch just one item from this table, dont care
	// about the readings, just care it the table reading fails.
	// fetch_counts is from trafstats_iptables.c - as the iptables
	// code will NOT compile directly into a C++ program.
	return(fetch_counts("prerouting-1", res, 1) == 0);
}

// indicate that the ipt_account interface is operational and we have at least one table to read address info from
bool traffic_account_missing() {
	struct ipt_ACCOUNT_context ctx;
	if(ipt_ACCOUNT_init(&ctx))
		return true; // its missing/ not working

	// but do we have any tables to read?
	if(ipt_ACCOUNT_get_table_names(&ctx) < 0)
		return true; // no tables

	return false;
}


// This converts a numeric classid into a string representation
// with special treatment for "root" and "none"
// otherwise the classid is shown as top 16bits : bottom 16 bits
// if either of these is 0 it is omitted so top part only set 
// looks like 1: and bottom part only set looks like :1

std::string traf_stat::tc_classid(__u32 h) const {
	std::ostringstream out;
	if(h == TC_H_ROOT)
		out << "root";
	else if(h == TC_H_UNSPEC) 
		out << "none";
	else if (TC_H_MAJ(h) == 0)
		out << std::hex << TC_H_MIN(h);
	else if (TC_H_MIN(h) == 0)
		out << std::hex << (TC_H_MAJ(h)>>16) << ":";
	else
		out << std::hex << (TC_H_MAJ(h)>>16) << ":" << std::hex << TC_H_MIN(h);
	return out.str();
}
// need a < operator
// so can sort traf_stat in classid/parent order
// so each parent is followed by immediate children as that is what
// makes sense when stats are printed out.
// the purpose of this is to return true if a < b, false otherwise.
// 1st key is dev, then classid with "root" considered lowest and "none" highest
bool operator < (const traf_stat &a, const traf_stat &b) {

	// std::cout << "A= " << a << " B = " << b << std::endl;

	if(a.classid == b.classid && a.parentid == b.parentid &&
		 a.dev == b.dev)
		return false; // if equal a not < b
	// do initial sort on dev, only carry on if dev same
	if(a.dev != b.dev) {
		// std::cout << (a.dev < b.dev) << std::endl;
		return (a.dev < b.dev);
	}
		 
	if(b.classid == "root") {
		// std::cout << "false as b root" << std::endl;
		return false; // no way a can be < b
	}
	if(b.classid == "none") {
		// std::cout << (a.classid != "none") << " as b none" << std::endl;
		return (a.classid != "none"); // a <= b
	}
	if(a.classid == "root") {
		// std::cout << "true as a root" << std::endl;
		return true; // root is first
	}
	if(a.parentid == "root" && b.parentid != "root") {
		// std::cout << "true as parent root" << std::endl;
		return true; // we are 1st level, other isnt so a less
	}
	if(b.parentid == "root" && a.parentid != "root") {
		// std::cout << "false as b parent root" << std::endl;
		return false; // other is 1st level,
	}
	if(a.parentid < b.parentid) {
		// std::cout << "true as < parent" << std::endl;
		return true; // smaller parents
	}
	if(a.classid < b.classid) {
		// std::cout << "true as < class" << std::endl;
		return true;
	}

	// std::cout << "false" << std::endl;
	return false; // give up
}

// need a traf_stat operator << so can print out everything.
// if the special error element has been set then print that,
// otherwise print the object
// Symbolic names, speeds etc are attached to the object with
// reference to the traf_config object.
std::ostream& operator << (std::ostream& out, const traf_stat& i) {
	if(i.error != "") {
		out << "ERROR: " << i.error << std::endl;
	}
	else if(i.dev != "" && i.dev != "unknown") {
		out << "dev=" << i.dev;
		double ispeed = traf_config[i.dev].interface_speed();
		if(ispeed == 0.0) {
			// have separate up and download speeds
			ispeed = traf_config[i.dev].interface_speed("upload");
			out << "(upload=" << format_rate(ispeed);
			ispeed = traf_config[i.dev].interface_speed("download"); 
			out << ", download=" << format_rate(ispeed) << ") ";
		}
		else {
			out << "(speed=" << format_rate(ispeed) << ") ";
		}
		out << std::endl;
	}

	if(i.is_class()) {
		out << "Class " << traf_config[i.dev].class_name(i.classid) << "(" << i.classid << ") ";
		if(i.has_parent()) {
			out << traf_config[i.dev].class_name(i.parentid) << "(" << i.parentid << ") ";
		}
		if(i.rate >= 1.0) {
			out << " rate=" << format_rate(i.rate) << " ceiling=" << format_rate(i.ceiling) << " ";
		}
		out << std::endl;
		
	}
	else if(i.is_rule()) { 
		if(i.has_class()) {
			// rule names only relivant for rules attached to a class
			out << "Rule " << traf_config[i.dev].rule_name(i.rule_num) << "(" << i.rule_num << ") ";
			out << traf_config[i.dev].class_name(i.classid) << "(" << i.classid << ") ";
		}
		else {
			out << "Rule (" << i.rule_num << ") ";
		}
		out << std::endl;
		
	}
	
	// now the calculated stats, floating point
	out << "bits per sec=" << i.bits_per_sec << " bytes_per_sec= " << i.bytes_per_sec << " pkts_per_sec= " << i.pkts_per_sec;
	// and the raw data, integers
	out << " raw data( bytes=" << i.bytes() << " packets=" << i.packets() << " drops=" << i.drops() << " overlimits=" << i.overlimits() << " bps=" << i.bps() << " pps=" << i.pps() << " qlen=" << i.qlen() << " backlog= " << i.backlog() << ")"; 
	out << std::endl;
	return out;
}

// print a vector of traf_stat by using the copy algorithm 
// to apply the << operator for traf_stat to each item in the vector
std::ostream& operator << (std::ostream& out, const Vtraf_stat & i) {
	std::copy(i.begin(), i.end(), std::ostream_iterator<traf_stat>(out, "\n"));
	return out;
}
// somehow the passed traf_stat_collection_item and everything in it is considerd as const
// this interferes with finding map methods

 std::ostream& operator << (std::ostream& out, const traf_stat_collection_item & i) { 
	 
	 traf_stat_hash_const_iterator e;
	 out << std::endl << "---------------------------------------------" << std::endl;
	 out << "collection from " << i.collection_start.t.tv_sec << "." << i.collection_start.t.tv_usec << " took " << time_interval(i.collection_start,i.collection_end) << " microsecs" << std::endl;
	 out << "ext devs (" ;
	 std::copy(i.ext_devs.begin(), i.ext_devs.end(), std::ostream_iterator<std::string>(out, ","));
	 out << ")" << std::endl;
	 out << "int devs (" ;
	 std::copy(i.int_devs.begin(), i.int_devs.end(), std::ostream_iterator<std::string>(out, ","));
	 out << ")" << std::endl;
	 out << "classes (" ;
	 std::copy(i.classes.begin(), i.classes.end(), std::ostream_iterator<std::string>(out, ","));
	
	 out << ")" << std::endl;
	 out << "rules (" ; 
	 std::copy(i.rules.begin(), i.rules.end(), std::ostream_iterator<std::string>(out, ","));
	 
	 out << ")" << std::endl;

	 out << "stats " << std::endl;
	 
	 for(e = i.stats.begin(); e != i.stats.end(); e++) 
		 out << e->first << " = " << e->second << std::endl;
	 
	 out << "---------------------------------------------" << std::endl;
	 return out;
 }

		
 
// callback from the tc code to get queue stats
// unlike iptables this compiles OK in C++
	 
extern "C" int tc_callback(const struct sockaddr_nl *who, struct nlmsghdr *n, void *s) {

	// passed pointer to vector of traffic stats
	Vtraf_stat *trafstats = (Vtraf_stat *)s;
	// container for current readings
	traf_stat thisone;

	
	struct tcmsg *t = (struct tcmsg *)NLMSG_DATA(n);
	int len = n->nlmsg_len;
	struct rtattr * tb[TCA_MAX+1];


	// sanity checks
	if (n->nlmsg_type != RTM_NEWTCLASS && n->nlmsg_type != RTM_DELTCLASS) {
	thisone.error = "Not a class";
		goto END;
	
	}
	len -= NLMSG_LENGTH(sizeof(*t));
	if (len < 0) {
		thisone.error = "Wrong len " + len;
	goto END;
	}

	 
	memset(tb, 0, sizeof(tb));
	parse_rtattr(tb, TCA_MAX, TCA_RTA(t), len);

	// see what we have here
	if (tb[TCA_KIND] == NULL) {
		thisone.error = "NULL kind\n";
		goto END;
	}
	if (n->nlmsg_type == RTM_DELTCLASS) { 
		thisone.error = "deleted\n";
		goto END;
	}
	if (t->tcm_handle) {
		thisone.classid = thisone.tc_classid(t->tcm_handle);
		thisone.dev = ll_index_to_name(t->tcm_ifindex);
		thisone.parentid = thisone.tc_classid(t->tcm_parent);
		thisone.info = t->tcm_info;
		// htb stuff? - hardcoded for hdb here
		if (tb[TCA_OPTIONS]) {
			struct rtattr *htb[TCA_HTB_RTAB+1];
			struct tc_htb_opt *hopt;

			memset(htb,0, sizeof(htb));
			parse_rtattr(htb,TCA_HTB_RTAB, (struct rtattr *)RTA_DATA(tb[TCA_OPTIONS]), 
						 RTA_PAYLOAD(tb[TCA_OPTIONS]));

			if (htb[TCA_HTB_PARMS]) {

				hopt = (tc_htb_opt*)RTA_DATA(htb[TCA_HTB_PARMS]);
		
				thisone.rate = hopt->rate.rate;
				thisone.ceiling = hopt->ceil.rate;
			}
		}
		// do we have stats?
		if (tb[TCA_STATS]) {
			if (RTA_PAYLOAD(tb[TCA_STATS]) < sizeof(struct tc_stats))
				thisone.error = "statistics truncated";
			else {
				
				memcpy(&thisone.stats, 
						 RTA_DATA(tb[TCA_STATS]), sizeof(struct tc_stats));
			}
		}
	}
 END:
	// put on end of the list
	trafstats->push_back(thisone);
	return 0;
	
}

	 
// return a string consisting of all classes associated with dev
Vtraf_stat list_class(std::string dev) {
	// just call the C string variant of the same
	return list_class(dev.c_str());
}
// and the C string variant...
// messy stuff with the netlink into the kernel.

Vtraf_stat list_class(const char *dev) {
	Vtraf_stat stats;
	traf_stat errorstat;

	struct tcmsg t;

	// defined in libnetlink.h pair of local socket addresses and a file descriptor
	struct rtnl_handle rth;
	
	stats.clear(); // start with empty collection of stats
	memset(&t, 0, sizeof(t));
	t.tcm_family = AF_UNSPEC;

	// can we open the netlink socket at all?
	if (rtnl_open(&rth, 0) < 0) {
		errorstat.error = "Cannot open rtnetlink";
		stats.push_back(errorstat);
		return stats;
	}

	// this requests a dump of current state and then runs ll_remember_index
	// across the returned result stream.
	// there are 16 possible slots in the static idxmap struct within libnetlink.a

	ll_init_map(&rth);
	// now we select the dev specificaly
	if ((t.tcm_ifindex = ll_name_to_index((char *)dev)) == 0) {
		// thats cool - not a problem, just no content
	
		rtnl_close(&rth);
		return stats;
	}

	if (rtnl_dump_request(&rth, RTM_GETTCLASS, &t, sizeof(t)) < 0) {
		errorstat.error = "Cant send dump request";
		stats.push_back(errorstat);
		rtnl_close(&rth);
		return(stats);
	}
	
	
	if (rtnl_dump_filter(&rth, tc_callback, (void *)&stats, NULL, NULL) < 0) {
	errorstat.error = "dump terminated";
		stats.push_back(errorstat);
		rtnl_close(&rth);
		return(stats);
	}
	rtnl_close(&rth);
	
	// now we sort it, using implicit operator < for the object
	sort(stats.begin(),stats.end());
	// and return the current class hierachy and readings for this device.
	return stats;
}

// Iptables interface:
// general version of list_rules,
// wrapper round the C core code fetch_counts.

Vtraf_stat list_rules(std::string chain) {
 
	std::string::size_type pos;
	std::string dev;
	Vtraf_stat stats;
	struct iptable_data res[MAX_RULES] = {0};
	int rn;
	
	stats.clear();
	unsigned int num = fetch_counts(chain.c_str(), res, MAX_RULES);
	unsigned int i;

	// we are a traffic controlled chain	
	if((pos = chain.find("-traf-tot")) != std::string::npos) { 
		traf_stat thisstat;
		thisstat.direction = chain.substr(0,pos);
		pos = thisstat.direction.find("-");
		thisstat.dev = thisstat.direction.substr(0, pos);
		thisstat.direction.erase(0,pos+1);
	 
		if(traf_config.count(thisstat.dev) == 0) {
			// std::cerr << __LINE__ << " bad device " << thisstat.dev << " direction " << thisstat.direction << " in the chain " << chain << std::endl;
			return stats;
		}
		// good device seen - one under control
		traffic_config & myconf = traf_config[thisstat.dev];
		for(i = 0; i < num; i++) {
			if((rn = myconf.pos_to_rulenum(i)) >= 0) {
				// go from positions in the table back to meaningful
				// rule numbers (and therefore names) using info put
				// in place by trafficloader script
		
				thisstat.rule_num = rn;
				thisstat.classid = myconf.rule_to_classid(rn);
		
				// iptables can only tell us bytes and packets
				// not the rich set of stats that the traffic queues do
				thisstat.stats.bytes = (__u64)res[i].bcnt;
				thisstat.stats.packets = (__u32)res[i].pcnt;
				// copy to the vector
				stats.push_back(thisstat);
			}
		}
	}
	else { 
		if(num == 0) {
			// std::cerr << __LINE__ << " fetch_counts fail for " << chain << std::endl;
			return stats;
		}
		// Assume its non traffic stats (pre|post)routing-(1|2)
		for(i = 0; i < num; i++) {
		
			traf_stat thisstat;
			thisstat.rule_num = i;
		
			// must get dev from the rule, its either attached to
			// incomming or outgoing, never both
			if(strlen(res[i].iniface)) {
				thisstat.dev = res[i].iniface;
			}
			else if (strlen(res[i].outiface)) {
				thisstat.dev = res[i].outiface;
			}
			else {
				// this interface not up
				continue;
			} 
			// iptables can only tell us bytes and packets
			// not the rich set of stats that the traffic queues do
			thisstat.stats.bytes = (__u64)res[i].bcnt;
			thisstat.stats.packets = (__u32)res[i].pcnt;
			stats.push_back(thisstat);
		}
	
	}
	return stats;
}
				
 

Vtraf_stat list_rules_for_dev(std::string dev, std::string direction) {

	return list_rules(dev + "-" + direction + "-traf-tot");
}

// list account info for all addresses

Vtraf_stat list_account() {
	struct ipt_acc_handle_ip *entry;
				
	Vstring tablenames;
	Vstring_iterator ti;
	int rtn;

	Vtraf_stat stats;

	if(!ctx_good) {
		if(ipt_ACCOUNT_init(&ctx)) {
			return stats;
		}
		ctx_good = true;
	}

	// check what table names are available
	if((rtn = ipt_ACCOUNT_get_table_names(&ctx)) >=0) {
		const char * name;
		while ((name = ipt_ACCOUNT_get_next_name(&ctx)) != 0) {
			tablenames.push_back(name);
		}
	}

	// now if we have any tables go through each in turn	
	
	for(ti = tablenames.begin(); ti != tablenames.end(); ti++) {
		const std::string &table = *ti;
		if(ipt_ACCOUNT_read_entries(&ctx, table.c_str(), 1)) {
			// massive failure
			ipt_ACCOUNT_deinit(&ctx);
			ctx_good = false;
			syslog(LOG_ERR,"ipt_ACCOUNT internal error at %s", table.c_str());
			return stats;
		}

		while ((entry = ipt_ACCOUNT_get_next_entry(&ctx)) != NULL) {
			// these addresses are intenal so packets comming from them are
			// outgoing and packets going to them are incomming
		    traf_stat instat;
			traf_stat outstat;

			outstat.account_table = instat.account_table = table;
			outstat.net_ip = instat.net_ip = entry->ip;
			outstat.direction = "up";
			outstat.stats.bytes = (__u64)entry->src_bytes;
			outstat.stats.packets = (__u32)entry->src_packets;
			instat.direction = "dn";
			instat.stats.bytes = (__u64)entry->dst_bytes;
			instat.stats.packets = (__u32)entry->dst_packets;
			stats.push_back(outstat);
			stats.push_back(instat);
		}
	}

	return stats;
}


// takes a difference between timestamps as double floating number of microsecs
// 1 million microsecs in a second
// if the old or new times are not valid then interval is 0
// 

double time_interval( const timestamp &oldtime, const timestamp &newtime) {
	double res = 0.0;
	if(oldtime.t.tv_sec == 0 || newtime.t.tv_sec == 0)
		return res; // can't do difference against start of time

	if(newtime.t.tv_sec > oldtime.t.tv_sec)
		res = (newtime.t.tv_sec-oldtime.t.tv_sec)*1000000;
	if(newtime.t.tv_usec < oldtime.t.tv_usec) {
		// have wrapped, so rest of old part plus new part
		res += (1000000 - oldtime.t.tv_usec) + newtime.t.tv_usec;
	}
	else 
		res += (newtime.t.tv_usec - oldtime.t.tv_usec);
	return res;
}

// take an older and a newer traf_stat and 
// calculate rates, putting results in refs
void calculate_rates(const traf_stat & older, const traf_stat & newer,
					 double &bytes_per_sec, 
					 double &bits_per_sec, 
					 double &pkts_per_sec) {
	double interval = time_interval(older.tstamp,newer.tstamp);
	if(interval > 0.0 && newer.stats.bytes > older.stats.bytes) {
		bytes_per_sec = (double)(newer.stats.bytes - older.stats.bytes) /
			(interval/1000000.0);
		bits_per_sec = bytes_per_sec * 8; 
		pkts_per_sec = (double)(newer.stats.packets - older.stats.packets) /
			(interval/1000000.0);
		 
	}
	else 
		bytes_per_sec = bits_per_sec = pkts_per_sec = 0.0;
	
}

// calculate rates for a pair of samples 
// updating the rates IN the newer sample
void calculate_rates(traf_stat & older, traf_stat & newer) {
	calculate_rates(const_cast<const traf_stat &>(older),
					const_cast<const traf_stat &>(newer), 
					newer.bytes_per_sec,newer.bits_per_sec,newer.pkts_per_sec);
}

// run calculate_rates over a whole hash of stats

void calculate_rates(traf_stat_hash *oldh, traf_stat_hash *newh) {
	traf_stat_hash_iterator o,n;
	
	
	// go through the indexes in no particular order
	for(o = oldh->begin();
		o != oldh->end();
		o++) {
		if(newh->count(o->first) > 0) {
			// only if this is present in both old and new
			calculate_rates(o->second, (*newh)[o->first]);
		}
		else {
			// std::cout << "Cant find " << o->first << std::endl;
		}
	}
}

// call the individual calculate_rates for all our various sample sets
// this takes the calculate_rates method right up to whole 
// traf_stat_collection_items
void calculate_rates(traf_stat_collection_item & older,
					 traf_stat_collection_item & newer) {
	calculate_rates(&older.stats, &newer.stats);
}

// collect a single sample - true if valid data in it
bool collect_a_sample(Vtraf_stat_samples &s, bool interfaces_only, bool use_account) {
	traf_stat_collection_item thisone(interfaces_only, use_account);
	s.push_back(thisone);
	return thisone.ext_devs.size() > 0 || thisone.int_devs.size() > 0;
}


// do averages over time
// find timespan we are interested in and 
double average_bit_rate(Vtraf_stat_samples &samples, const std::string &idx, const timestamp &start, const timestamp &end) { 
	 
	// find the first sample that is > than start and the last sample that is < end
	// have to do own bsearch here as generic one needs a whole sample
	// object and dont want to do that
	unsigned int middle,min,max,s,e;
	
	// 1st find the lowest sample > than start that has something for idx
	for(min=0, max = (samples.size()-1), middle = (min+max)/2; 
		min < max;
		middle = (min+max)/2) {
	
		if(samples[middle].collection_start.t.tv_sec > start.t.tv_sec) {
			// this sample newer than we want
			max = middle; // so look lower
		}
		else if(samples[middle].collection_start.t.tv_sec < start.t.tv_sec) {
			// this sample is older than we want
			min = middle+1; // so look higher
		}
		else {
			// we have found one with the second we are looking for
			break; // so can do early exit
		}
			
	}
	
	// bsearch could have landed us anywhere in our range
	// adjust down to give the first of the matching records

	while(middle > 0 && 
			samples[middle-1].collection_start.t.tv_sec >= start.t.tv_sec)
		middle--;
	// middle is the nearest thing to start time but does it have content for idx?
	while(samples[middle].stats.count(idx) == 0 && samples[middle].collection_start.t.tv_sec < end.t.tv_sec)
		middle++;

	s = middle;

	// start from there to find end time
	
	for(min=s, max = (samples.size()-1), middle = (min+max)/2; 
		min < max;
		middle = (min+max)/2) {
	
		if(samples[middle].collection_start.t.tv_sec > end.t.tv_sec) {
			// sample newer than we want
			max = middle;
		}
		else if(samples[middle].collection_start.t.tv_sec < end.t.tv_sec) {
			// this sample is older than we want
			min = middle+1;
		}
		else {
			// we have found one with the second we are looking for
			break;
		}
		
	} 
	while(middle < (samples.size()-1) && samples[middle+1].collection_start.t.tv_sec <= end.t.tv_sec)
		middle++; // get the last possible matching value
	// middle is the nearest thing to end time but does it have content for idx?
	
	while((middle > 0) && samples[middle].stats.count(idx) == 0 && samples[middle].collection_start.t.tv_sec >= start.t.tv_sec)
		middle--;
	e = middle;

	double byterate,bitrate,pktrate;
	unsigned start_sample_count = samples[s].stats.count(idx);
	unsigned end_sample_count = samples[e].stats.count(idx);
	if(s < e && start_sample_count > 0 && end_sample_count > 0) {
		// this should be true
		// so can get a rate over the widest possible range
		// data for this label may well be missing in some (or most)
		// of the data but the trouble we have been through above
		// means that we have the farthest possible apart pair of samples
		// to calculate the rate from 
		const traf_stat &start = samples[s].stats[idx];
		const traf_stat &end = samples[e].stats[idx];
		byterate = bitrate = pktrate = 0;
		calculate_rates(start, end, byterate, bitrate, pktrate);
		 
		return bitrate;
	}
	else
		return 0.0; // no samples so no average
}
	

// remove samples older than certain time
// As vector is in time order just need to start from the beginning
// only care about seconds not microsec
void truncate_sample_set(Vtraf_stat_samples &samples, const timestamp &oldest) {
	Vtraf_stat_samples_iterator i = samples.begin();
	if(i == samples.end())
		return; // empty set anyway
	while(i != samples.end() && i->collection_start.t.tv_sec < oldest.t.tv_sec) 
		i++;
	// iterator now set at first item new enough to keep (or end)
	
	// never remove all the samples!
	// have to have a previous one to calculate rates from
	// even if line has otherwise been inactive for a 'long' time
	if(i == samples.end()) 
		i--; // back onto last item
	if(i != samples.begin()) {
		// erase to i-1 so always keep at least one
		samples.erase(samples.begin(), i-1);
		
	}
}

// nice user firendly rate - 2 decimal places kbit mbit or gbit
// Then followed by same as bytes
std::string format_rate(double t1, double t2) {
	return format_rate(t1) + " (20 sec avg) " + format_rate(t2) + " (last sec)";
} 
std::string format_rate(double tmp) {
	std::ostringstream out;
	char floatbuf[20];
	
	if(rint(tmp) != 0) {
		if(tmp >= 1024 * 1024 * 1023) {
			sprintf(floatbuf, "%*.1f", 7,tmp/(1024*1024*1024));
			out << floatbuf << " Gbit"; 
		}
		else if(tmp >= 1024 * 1023) {
			sprintf(floatbuf, "%*.1f", 7,tmp/(1024*1024));
			out << floatbuf << " Mbit"; 
		}
		else if(tmp >= 1024){
			sprintf(floatbuf, "%*.1f", 7,tmp/(1024));
			out << floatbuf << " Kbit";
		}
	else {
		sprintf(floatbuf, "%*.0f", 7,tmp);
		out << floatbuf << "  bit";
	}
	
#ifdef NEVER
		tmp /= 8;
		out << ' ';
		if(rint(tmp) != 0) {
			if(tmp >= 1024 * 1024 * 1023) {
				sprintf(floatbuf, "%*.2f", 7,tmp/(1024*1024*1024));
				out << floatbuf << " GB"; 
			}
			else if(tmp >= 1024 * 1023) {
				sprintf(floatbuf, "%*.2f", 7,tmp/(1024*1024));
				out << floatbuf << " MB"; 
			}
			else {
				sprintf(floatbuf, "%*.2f", 7,tmp/(1024));
				out << floatbuf << " KB";
			}
		}
		else {
			sprintf(floatbuf, "%*.2f", 7,0.0);
			out << floatbuf<< " KB";
		}
#endif
	}
	else {
		sprintf(floatbuf, "%*.0f", 7,0.0); 
		out << floatbuf << "  bit";
#ifdef NEVER
		out << ' ';
		sprintf(floatbuf, "%*.0f", 7,0.0);
		out << floatbuf << " B";
#endif
	}
	return out.str();
}

// indicate if a specified interface is up at the moment 
bool interface_up(const char *dev) {
	bool up = false;
	struct ifreq ifr;
	// make any old socket
	int skfd = socket(AF_UNIX, SOCK_STREAM, 0);

	strncpy(ifr.ifr_name, dev, IFNAMSIZ-1);
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) >= 0)
	up = (ifr.ifr_flags & IFF_UP) != 0;
	close(skfd);
	return up;
}


