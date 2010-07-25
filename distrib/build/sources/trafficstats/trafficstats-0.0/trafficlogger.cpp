// This collects live stats and logs them for long term stats.
// C++ interface between smoothwall and tc/netfilter/ipt_ACCOUNT statistics collection
// (c) SmoothWall Ltd 2007
// This code is distributed under the terms of the GPL v2.
// Written by Martin Houston <martin.houston@smoothwall.net> 2004 - 2007

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <syslog.h>
#include <time.h>
#include <map>
#include "reltimes.hpp" 
#include "traffic_config.hpp"
#include "trafstats_core.hpp"
// #define DEBUG 1
#define PIDFILE "/var/run/trafficlogger.pid"
#define LIVE_LOGFILE_NAME "/var/log/trafficstats"
#define QUICK_LOGFILE_NAME "/var/log/quicktrafficstats"

// sleep this many secs between samples
#ifdef DEBUG
#define SAVE_SCALE_FACTOR 1
#else
// production values
// SAMPLE_DELAY is how often an interfaces_only sample is taken
// and FULL_SAMPLE_DELAY is how often a complete sample of all the rule chains 
// is taken
#define FULL_SAMPLE_DELAY 60
#define SAVE_SCALE_FACTOR 1024ull
#endif

// handle signals cleanly

static bool time_to_go = false;

// The iptables libs assume these are here so we have to set them.
const char *program_name;
const char *program_version;
// here is our config class
traf_config_hash traf_config;

// class to keep live stats counts
// transfer totals are kept in in the save file kb as to keep them in bytes would
// give measily 4GB limit. Here all totals are kept 64bit to retain precision
// this means that restarting logging losses any fractions of a kb stats.

typedef unsigned long long int bytecount_t;
class livestats {
	// when did we last add to these stats?
	timestamp last_update;
	std::string logfile;
	// current byte rates for each device etc.
	std::map<std::string, int>current_rates;
	// current and previous counts for periods defined by the reltimes class 
	std::map<std::string, bytecount_t>byte_counts;
	// makeup of the latest sample, which devices etc. it contained
	std::vector<std::string> classes;
	StrSet rules, int_devs, ext_devs, addresses;
  
	// does the repetitive bit of a stats refresh
	// code that is common to all stats
	void refresh_helper(const std::string label, 
						const std::string bclabel_part,
						const std::string crlabel,
						traf_stat_collection_item & last,
						traf_stat_collection_item & prev,
						std::vector<std::string> & range, 
						Vtraf_stat_samples &s); 

	// same for saving stats
	void save_rate_helper(FILE *fd, std::string label);

	void save_period_helper(FILE *fd, std::string plabel, std::vector<std::string> & range);

public:
	// constructor tries to read in the existing file,
	// then decided based on age of file which kbyte_counts totals
	// can be populated 
	livestats(const std::string plogfile = LIVE_LOGFILE_NAME);

	// see it the new proposed timestamp crosses any period boundaries - if so then 
	// all current data for those boundaries is moved to previous data 
	// and the current data is cleared

	bool check_rollovers(const timestamp timestamp, const reltimes &times);

	// write out the saved stats
	// dont know how to do proper C++ IO, tidy this up someone?
	// get passed fd on which to save data, so no need to keep opening/closing it
	// and thus thrashing disk

	bool save(FILE *fd, reltimes & times);

	// take latest stats and refresh the totals cache
	// take differences between last two samples and updates all in range totals
	// takes 1 min average for the average rates
	bool refresh_stats(Vtraf_stat_samples &s, reltimes & times);

};


static void finish(int sig) {
	time_to_go = true;
}



// instate as much of the stats as we can from a saved config
// dont accept anything that is too old.



// get a sub process to collect the samples, main restarts us as needed
void dostats(FILE *quicklog, FILE* mainlog) {
	Vtraf_stat_samples samples, quicksamples;
	livestats stats(LIVE_LOGFILE_NAME);
	livestats quickstats(QUICK_LOGFILE_NAME);


	(void)collect_a_sample(samples,false,true);
	// and a truncated report - no smoothtraffic rules 
	(void)collect_a_sample(quicksamples, true, true);

	while(!time_to_go) {  
		sleep(2);
		reltimes times; // fresh timestamp set each loop...
		timestamp keep;
		// keep the number of samples under control
		if(!(times.now.t.tv_sec % FULL_SAMPLE_DELAY)) {
			// syslog(LOG_WARNING,"traffic full %d second sample", FULL_SAMPLE_DELAY);
			// full sample interval  
			keep = times.now;
			keep.t.tv_sec -= (FULL_SAMPLE_DELAY * 3); 
			truncate_sample_set(samples, keep);
			// take differences between last 2 samples to refresh our running totals
			stats.refresh_stats(samples, times);
			// and save those totals out
			stats.save(mainlog, times);
			(void)collect_a_sample(samples, false,true); 
		}
		else {
			// quick samples 
			// syslog(LOG_WARNING,"traffic 2 second sample");
			keep = times.now;
			keep.t.tv_sec -= 5; 
			truncate_sample_set(quicksamples, keep);
			// take differences between last 2 samples to refresh our running totals
			quickstats.refresh_stats(quicksamples, times);
			// and save those totals out
			quickstats.save(quicklog, times);
			(void)collect_a_sample(quicksamples, true, true);
		}
	}
}

int  main(int argc, char *argv[]) {

#ifndef DEBUG
	pid_t wpid;
	FILE *pid;
	FILE *quicklog;
	FILE *mainlog;
	struct sigaction act;
	if((wpid = fork()) > 0) {
		//we are the parent - so exit right away 
		exit(0);
	}
	// now detached
	if(setsid() < 0) {
		syslog(LOG_ERR,"%s","setsid fails");
		exit(1); /* failure! */
	}

	if((quicklog = fopen(QUICK_LOGFILE_NAME, "w")) == NULL) {
		syslog(LOG_ERR,"opening %s fails",QUICK_LOGFILE_NAME);
		exit(1); /* failure! */
	}    
	if((mainlog = fopen(LIVE_LOGFILE_NAME, "r+")) == NULL) {

		syslog(LOG_ERR,"opening %s fails",LIVE_LOGFILE_NAME);
		exit(1); /* failure! */
	}
	// now setup signal handling 
	act.sa_handler = finish;
	sigemptyset(&act.sa_mask);
	// dont want child signals, want any interrupted syscalls to restart 
	act.sa_flags = SA_NOCLDSTOP|SA_RESTART;

	sigaction(SIGINT, &act, 0);
	sigaction(SIGTERM, &act, 0);
	sigaction(SIGHUP, &act, 0);
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGUSR1, &act, 0);

	freopen("/dev/null", "a+", stdin);
	freopen("/dev/null", "a+", stdout);
	freopen("/dev/null", "a+", stderr);

	program_name = "trafficlogger";
	program_version = "3";


	// if get here we are the working slave
	// kill any existing logger - there can be only one


	if((pid = fopen(PIDFILE,"r")) != NULL) {
		if(flock(fileno(pid), LOCK_EX) == 0) {
			// we have a lock on this file now 
			// kill the current incarnation, can only be one of us 
			pid_t thatpid = 0;
			int killstat;
			int attempt = 0;
	  
			fscanf(pid, "%d", &thatpid);

			if(thatpid) { 
				// see if that pid does exist first
				killstat = kill(thatpid, 0);
				if(killstat < 0 && errno == ESRCH) {
					// process already gone
				}
				else {// process is there
					do{
						killstat = kill(thatpid, SIGTERM);
						attempt++;
						sleep(1);
					} while(killstat && attempt < 10);
					if(killstat) {
						syslog(LOG_ERR,"%s %d","Cannot kill existing process ", thatpid); 
						flock(fileno(pid), LOCK_UN);
						fclose(pid);
						exit(1);
					}
				}
			}
			// write our own pid here now
			freopen(PIDFILE,"w",pid);
			rewind(pid);
		}
		else {
			syslog(LOG_ERR,"%s","Cannot lock " PIDFILE);
			fclose(pid);
			exit(1);
		}
	}
	// else file not there yet
	else if((pid = fopen(PIDFILE,"w")) != NULL) {
		if(flock(fileno(pid), LOCK_EX) == 0) {
			// do nothing 
		}
		else {	
			syslog(LOG_ERR,"%s","Cannot lock " PIDFILE);   
			fclose(pid);
			exit(1);
		}
	}
	// either way pid is now open 
	fprintf(pid,"%d\n", getpid());
	flock(fileno(pid), LOCK_UN);
	fclose(pid); 
#endif
	while(traffic_iptables_missing()) {
		sleep(5);
	}
		
	syslog(LOG_WARNING,"%s", "starting");
	dostats(quicklog, mainlog);
	syslog(LOG_WARNING,"%s", "finishing");
	fclose(quicklog);
	fclose(mainlog); // only time these are explicily closed
}

void livestats::refresh_helper(	const std::string label, 
								const std::string bclabel_part,
								const std::string crlabel,
								traf_stat_collection_item & last,
								traf_stat_collection_item & prev,
								std::vector<std::string> & range, 
								Vtraf_stat_samples &s) {
	// do we have stats entries now and previous for this item
	// if not then cant do very much!
	if(last.stats.count(label) && prev.stats.count(label)) {
	// use references for efficiency
		traf_stat &st = last.stats[label];
		traf_stat &pst = prev.stats[label];
		// have we seen any traffic at all in this time period?
		if(st.bytes() > pst.bytes()) {
			bytecount_t delta = st.bytes() - pst.bytes();
			// update all this_ counters for this device pattern

			for(std::vector<std::string> ::iterator s = range.begin(); s != range.end(); s++) {
				std::string bclabel = *s + bclabel_part;

				if(byte_counts.count(bclabel) == 0) {
					// syslog(LOG_WARNING, "Initialising %s", bclabel.c_str());
					bytecount_t &count = byte_counts[bclabel];
					count = 0ull;
				}
				// get a reference
				bytecount_t &count = byte_counts[bclabel];
				count += delta;
				// if(delta != 0ull) syslog(LOG_WARNING, "%s now %lld", bclabel.c_str(), count);
			}
		}
		// and the rate - go back 2 min
		timestamp oldtime = last.collection_start;
		oldtime.t.tv_sec -= 120;
		int bitrate = int(average_bit_rate(s, label, oldtime, last.collection_start));
		if(bitrate > 0) {
			current_rates[crlabel] = bitrate;
		}
		else {
			current_rates[crlabel] = 0; // no rate at all
		}
	} // if(last.stats.count(label) && prev.stats.count(label)
	else {
		// syslog(LOG_WARNING, "no stats for %s!", label.c_str());
		// std::ostringstream log;
		// log << last;
		// syslog(LOG_WARNING,log.str().c_str());

	}
} 

void livestats::save_rate_helper(FILE *fd, std::string label) {
	std::map<std::string,int> ::iterator h; 
	std::map<std::string, int> &nc_current_rates = const_cast<std::map<std::string, int> &>(current_rates); 
	if((h = nc_current_rates.find(label)) != nc_current_rates.end()) {
		long int rate =  h->second;
		fprintf(fd,"%s=%ld\n", label.c_str(), rate);
	}
	else { 
		// omit zero values
		// fprintf(fd,"%s=0\n",label.c_str());
	}
}

void livestats::save_period_helper(FILE *fd, std::string plabel, std::vector<std::string> & range) {
	std::map<std::string, bytecount_t> &nc_byte_counts = const_cast<std::map<std::string, bytecount_t> &>(byte_counts);
	std::map<std::string,bytecount_t> ::iterator i;
	std::string label;
	std::vector<std::string> ::iterator k;
	for(k = range.begin(); k != range.end(); k++) {
		label = *k + plabel;
		if((i = nc_byte_counts.find(label)) != nc_byte_counts.end() && (i->second/SAVE_SCALE_FACTOR) != 0) {
			fprintf(fd,"%s=%ld\n", label.c_str(), i->second/SAVE_SCALE_FACTOR);
		}
		else {
			fprintf(fd, "%s=0\n", label.c_str());
		}
	}
}
livestats::livestats(const std::string plogfile) {
	reltimes times;
	timestamp timestamp;
	ConfigVAR *trafficstats;
	std::vector<std::string> confkeys;
		
	struct stat st;
	std::vector<std::string> keys;
	std::vector<std::string> ::iterator i,ki;
	std::map<std::string,bool> :: iterator cw;
	logfile = plogfile;

	if(stat(logfile.c_str(), &st)) { // this stat fails
		syslog(LOG_INFO,"No %s so cannot restore saved values", logfile.c_str()); 
		sleep(1);
		// no saved file no init possible
	}
	else {
		trafficstats = new ConfigVAR(logfile);
		confkeys = trafficstats->keys();

		// do we have a last_update key?
		if(((*trafficstats)["last_update"]) != "") {
			timestamp.t.tv_sec = (time_t)safeatoi((*trafficstats)["last_update"].c_str());
			timestamp.t.tv_usec = 0;
			keys = times.in_range(timestamp);

			// for each key we can transfer info from the old file into
			// our current one
			// keys are this_week etc

			// so now go through all the various patterns in the file
			// seeing which contain key phrases - if they do then
			// that whole data item can be imported

			// outer loop : what is in the config file
			for(i = confkeys.begin(); i != confkeys.end(); i++) {
				// inner loop the time range patterns
				for(ki = keys.begin(); ki != keys.end(); ki++) {
				// have we found time range as a substring of key from config file?
		
					if(i->find(*ki) != std::string::npos) {
						// one of our qualifying keys

						// only saved to nearest kb
						std::string sval = (*trafficstats)[ i->c_str() ];
						bytecount_t val =  safeatoi( sval.c_str() ) * SAVE_SCALE_FACTOR;
						// syslog(LOG_INFO,"using saved value for %s = %lld", i->c_str(), val); 
						byte_counts[*i] = val;
					}
					// dont bother restoring bitrates!
				}

			} // for(i = confkeys.begin(); i != confkeys.end(); i++)
		}
		delete trafficstats;
	}
}

bool livestats::check_rollovers(const timestamp timestamp, const reltimes &times) {
	// all the ranges we are interested in
	std::vector<std::string> range = times.curr_range();
	std::vector<std::string> ::iterator s;
	std::map<std::string,bytecount_t> ::iterator i; 
	bool doneit = false;
	std::map<std::string, bytecount_t> &nc_byte_counts = const_cast<std::map<std::string, bytecount_t> &>(byte_counts);
	bytecount_t count;

	if(last_update.t.tv_sec == 0 || timestamp.t.tv_sec == last_update.t.tv_sec && timestamp.t.tv_usec == last_update.t.tv_usec)
		return doneit; // no work to do -  - no prev time known yet or time not changed

	// have any boundaries been crossed
	// is last_update now NOT in any of the current containers? - have advanced to next min/hour etc?

	for(s = range.begin(); s != range.end(); s++) {
		// are we still in this range
		if(!times.in_range(*s, last_update)) {
			std::string prev = *s;
			prev.replace(0,4,"prev"); 
			// syslog(LOG_INFO, "NOW(%ld) tstamp %ld puts us out of range %s %ld-%ld",time(NULL),last_update.t.tv_sec, s->c_str(), times.start(*s), times.end(*s) ); 
			// go through all the relivant byte_counts
			for(i = nc_byte_counts.begin(); i != nc_byte_counts.end(); i++) {
				std::string::size_type pos = i->first.find(*s);
				if(pos  != std::string::npos) {
					// found the "this_week" etc
					const std::string & this_idx = i->first;
					std::string prev_idx = this_idx;
					// now modify
					prev_idx.replace(pos,4,"prev");
					// now do the swap
					count =  byte_counts[this_idx];
					nc_byte_counts[prev_idx] = count;
					nc_byte_counts[this_idx] = 0ull;
					doneit = true;

					// syslog(LOG_INFO,"Rollover %s to %s %lld for tstamp %ld", this_idx.c_str(), 
					// prev_idx.c_str(),count,last_update.t.tv_sec);

				}

			}

		}
		// else still in range so nothing to do
	}
	
	return doneit;
	
}
bool livestats::save(FILE *fd, reltimes & times) {
	// these are the time periods we store data for
	std::vector<std::string> range = times.all_range();
	std::map<std::string,int> ::iterator h; 
	std::map<std::string,bytecount_t> ::iterator i;  
	std::set<std::string> ::iterator devidx,ruleidx, addridx;
	// classes need to be kept in specific - non sorted order
	std::vector<std::string> ::iterator classidx;
	std::set<std::string> ::iterator j;
	std::map<std::string, int> &nc_current_rates = const_cast<std::map<std::string, int> &>(current_rates); 

	// only do stuff if we have some fresh data to save yet
	if(nc_current_rates.size() == 0)
		return false;
	// lock this file
	rewind(fd);
	lockf(fileno(fd), F_LOCK, 0);
	// timestamp first
	fprintf(fd,"last_update=%ld\n", last_update.t.tv_sec);
	// then whole interface stats, only now save for devs that are present - this is a bug, needs to be fixed
	if(ext_devs.size() > 0) {
		for(j = ext_devs.begin(); j != ext_devs.end(); j++) {
			save_rate_helper(fd, "cur_inc_rate_" + *j);                
			save_rate_helper(fd, "cur_out_rate_" + *j);
		
			// and for each time period, totals are scaled down to kbytes
			save_period_helper(fd, "_inc_total_" + *j, range);
			save_period_helper(fd, "_out_total_" + *j, range);
		
			// then class stats
			for (classidx = classes.begin(); classidx != classes.end(); classidx++) {
				save_rate_helper(fd, "cur_out_rate_" + *j + "_class_" + *classidx);
				save_period_helper(fd, "_out_total_" + *j + "_class_" + *classidx, range);
				save_rate_helper(fd, "cur_inc_rate_" + *j + "_class_" + *classidx);
				save_period_helper(fd, "_inc_total_" + *j + "_class_" + *classidx, range);
			}
		  
			// then rule stats
			for (ruleidx = rules.begin(); ruleidx != rules.end(); ruleidx++) { 
				save_rate_helper(fd, "cur_out_rate_" + *j + "_rule_" + *ruleidx);
				save_period_helper(fd, "_out_total_" + *j + "_rule_" + *ruleidx, range);
				save_rate_helper(fd, "cur_inc_rate_" + *j + "_rule_" + *ruleidx);
				save_period_helper(fd, "_inc_total_" + *j + "_rule_" + *ruleidx, range);
			}
		} // dev loop
	} // ext devs
	if(int_devs.size() > 0) {
		for(j = int_devs.begin(); j != int_devs.end(); j++) {
			save_rate_helper(fd, "cur_inc_rate_" + *j);                
			save_rate_helper(fd, "cur_out_rate_" + *j);
			// and for each time period, totals are scaled down to kbytes
			save_period_helper(fd, "_inc_total_" + *j, range);
			save_period_helper(fd, "_out_total_" + *j, range);
			// then class stats
			for (classidx = classes.begin(); classidx != classes.end(); classidx++) {
				save_rate_helper(fd, "cur_out_rate_" + *j + "_class_" + *classidx);
				save_period_helper(fd, "_out_total_" + *j + "_class_" + *classidx, range); 
				save_rate_helper(fd, "cur_inc_rate_" + *j + "_class_" + *classidx);
				save_period_helper(fd, "_inc_total_" + *j + "_class_" + *classidx, range);
			}
			// then rule stats
			for (ruleidx = rules.begin(); ruleidx != rules.end(); ruleidx++) { 
				save_rate_helper(fd, "cur_out_rate_" + *j + "_rule_" + *ruleidx);
				save_period_helper(fd, "_out_total_" + *j + "_rule_" + *ruleidx, range); 
				save_rate_helper(fd, "cur_inc_rate_" + *j + "_rule_" + *ruleidx);
				save_period_helper(fd, "_inc_total_" + *j + "_rule_" + *ruleidx, range);
			}
		}
	}
	for(addridx = addresses.begin(); addridx != addresses.end(); addridx++) {
		const std::string & address = *addridx;
		std::string direction = "(dn)";
		std::string addr = "0.0.0.0";
		std::string::size_type pos = address.find("(");
		if(pos != std::string::npos) {
			direction = address.substr(pos);
			addr = address.substr(0,pos);
		}
		if(direction == "(dn)") {
			save_rate_helper(fd, "cur_inc_rate_" + addr);
			save_period_helper(fd, "_inc_total_" + addr, range);
		}
		else {
			save_rate_helper(fd, "cur_out_rate_" + addr);
			save_period_helper(fd, "_out_total_" + addr, range);
		}
	}
	
	// truncate the file at this point
	ftruncate(fileno(fd), ftell(fd)); 
	fflush(fd);
	// now unlock
	rewind(fd);
	lockf(fileno(fd), F_ULOCK, 0);
	
	return true;

}

bool livestats::refresh_stats(Vtraf_stat_samples &s, reltimes & times) { 

	std::set<std::string> ::iterator devidx,ruleidx,addridx;
	std::vector<std::string> ::iterator classidx;
	Vtraf_iterator ia,ib;
	unsigned int numsamples = s.size();

	if(numsamples < 2) {
		return false; // not enough samples yet
	}
	traf_stat_collection_item & last = s[s.size()-1];        
	traf_stat_collection_item & prev = s[s.size()-2];
	std::vector<std::string> range = times.in_range(last.collection_start);
	// now update timestamp as being the most recent sample 
	// before we update the timestamp 
	
	check_rollovers(last.collection_end,times);
	last_update = times.now;
	
	// populate our idea of what devs are present etc.
	ext_devs = last.ext_devs;
	int_devs = last.int_devs; 
	classes = last.class_indexes_in_order();
	rules = last.rules;
	addresses = last.addresses;
	
	// whole interface stats first - external incomming
	for (devidx = ext_devs.begin(); devidx != ext_devs.end(); devidx++) {

		refresh_helper(*devidx + "_ext_in", 
						"_inc_total_" + *devidx, 
						"cur_inc_rate_" + *devidx, 
						last, prev, range, s);
	  
		refresh_helper(*devidx + "_ext_out", 
						"_out_total_" + *devidx, 
						"cur_out_rate_" + *devidx, 
						last, prev, range, s);

		for (classidx = classes.begin(); classidx != classes.end(); classidx++) {

			// take a ref to this particular item
			refresh_helper(*devidx + "_dn_class_" + *classidx, 
							"_inc_total_" + *devidx + "_class_" + *classidx, 
							"cur_inc_rate_" + *devidx + "_class_" + *classidx, 
							last, prev, range, s);
			refresh_helper(*devidx + "_up_class_" + *classidx, 
							"_out_total_" + *devidx + "_class_" + *classidx, 
							"cur_out_rate_" + *devidx + "_class_" + *classidx, 
							last, prev, range, s);
		}
		// and rules
		for (ruleidx = rules.begin(); ruleidx != rules.end(); ruleidx++) {

			// take a ref to this particular item
			refresh_helper(*devidx + "_dn_rule_" + *ruleidx, 
							"_inc_total_" + *devidx + "_rule_" + *ruleidx, 
							"cur_inc_rate_" + *devidx + "_rule_" + *ruleidx , 
							last, prev, range, s);
			refresh_helper(*devidx + "_up_rule_" + *ruleidx, 
							"_out_total_" + *devidx + "_rule_" + *ruleidx, 
							"cur_out_rate_" + *devidx + "_rule_" + *ruleidx , 
							last, prev, range, s);
		}

	}

	// now internal devs
	for (devidx = int_devs.begin(); devidx != int_devs.end(); devidx++) {
		refresh_helper(*devidx + "_int_in", 
						"_inc_total_" + *devidx, 
						"cur_inc_rate_" + *devidx, 
						last, prev, range, s);

		refresh_helper(*devidx + "_int_out", 
						"_out_total_" + *devidx, 
						"cur_out_rate_" + *devidx, 
						last, prev, range, s);
		for (classidx = classes.begin(); classidx != classes.end(); classidx++) {
			// take a ref to this particular item
			refresh_helper(*devidx + "_dn_class_" + *classidx, 
							"_inc_total_" + *devidx + "_class_" + *classidx, 
							"cur_inc_rate_" + *devidx + "_class_" + *classidx, 
							last, prev, range, s);
			refresh_helper(*devidx + "_up_class_" + *classidx, 
							"_out_total_" + *devidx + "_class_" + *classidx, 
							"cur_out_rate_" + *devidx + "_class_" + *classidx, 
							last, prev, range, s);
		}
		// and rules
		for (ruleidx = rules.begin(); ruleidx != rules.end(); ruleidx++) {
			refresh_helper(*devidx + "_dn_rule_" + *ruleidx, 
							"_inc_total_" + *devidx + "_rule_" + *ruleidx, 
							"cur_inc_rate_" + *devidx + "_rule_" + *ruleidx , 
							last, prev, range, s);
			refresh_helper(*devidx + "_up_rule_" + *ruleidx, 
							"_out_total_" + *devidx + "_rule_" + *ruleidx, 	
							"cur_out_rate_" + *devidx + "_rule_" + *ruleidx , 
							last, prev, range, s);
		}
	}

	// now addresses
	for(addridx = addresses.begin(); addridx != addresses.end(); addridx++) {
		const std::string &address = *addridx;
		std::string direction = "(dn)";
		std::string addr = "0.0.0.0";
		std::string::size_type  pos = address.find("(");
		if(pos != std::string::npos) {
			direction = address.substr(pos);
			addr = address.substr(0,pos);
		}
		

		refresh_helper(address, std::string((direction == "(dn)" ? "_inc_total_" : "_out_total_")) + addr,
						std::string((direction == "(dn)" ? "cur_inc_rate_" : "cur_out_rate_")) + addr,
						last,prev,range,s);
	} 
	
	return true;
}
