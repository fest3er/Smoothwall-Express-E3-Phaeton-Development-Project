#ifndef RELTIMES_INC
#define RELTIMES_INC
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "timestamp.hpp" // where we get the timestamp class

// Get an idea of this and previous minute, hour, week etc
// based on what the time is now.
// so we can test where other times are in relation to us


class reltimes {
private:
    std::map<std::string,timestamp> startof,endof;
public:
    
    timestamp now;  // gets current time put in it on creation

    reltimes () {
        struct timeval then;
        struct tm tm_then;
        
        // we use local timezone for deciding begining of day etc.
        localtime_r(&now.t.tv_sec, &tm_then);
        // find end of minute
        if(tm_then.tm_sec < 59) 
            tm_then.tm_sec = 59;
        then.tv_sec = mktime(&tm_then);
        then.tv_usec = 999999;
        endof["this_minute"] = then; // into the future
        // now go back to start of this minute
        then.tv_sec -= (60-1);
        then.tv_usec = 0;
        startof["this_minute"] = then;
        then.tv_sec -= 1;
        then.tv_usec = 999999;
        endof["prev_minute"] = then;
        then.tv_sec -= (60-1); // we count 0-59 secs
        then.tv_usec = 0;
        startof["prev_minute"] = then;

        // work back from now again - prev_minute may have been different hour
        tm_then.tm_min = 59;
        tm_then.tm_sec = 59;
        then.tv_sec = mktime(&tm_then);
        then.tv_usec = 999999;
        endof["this_hour"] = then;
        then.tv_sec -= (60*60)-1;
        then.tv_usec = 0;
        startof["this_hour"] = then;

        // now previous hour
        then.tv_sec -= 1;
        then.tv_usec = 999999;
        endof["prev_hour"] = then;
        then.tv_sec -= (60*60)-1;
        then.tv_usec = 0;
        startof["prev_hour"] = then;

        // now find end of today
        tm_then.tm_hour = 23;
        tm_then.tm_min = 59;
        tm_then.tm_sec = 59;
        then.tv_sec = mktime(&tm_then);
        then.tv_usec = 999999;
        endof["this_day"] = then;
        // with the start 24 hours less
        then.tv_sec -= (24*60*60)-1; 
        then.tv_usec = 0;
        startof["this_day"] = then;
        // and the previous day
        then.tv_sec -= 1;
        then.tv_usec = 999999;
        endof["prev_day"] = then;
        then.tv_sec -= (24*60*60)-1; 
        then.tv_usec = 0;
        startof["prev_day"] = then;


        // now end of this week - midnight Saturday next
        // cant do this using mktime, have to work it out
        // get us back to end of today
        then.tv_sec = mktime(&tm_then);
        // take us to saturday night
        then.tv_sec += 24*60*60*(6-tm_then.tm_wday);
        then.tv_usec = 999999;
        endof["this_week"] = then;
        // start 7 days less
        then.tv_sec -= (7*24*60*60)-1; 
        then.tv_usec = 0;
        startof["this_week"] = then;
        // and the previous week - 1 microsecond earlier
        then.tv_sec -= 1;
        then.tv_usec = 999999;
        endof["prev_week"] = then;
        then.tv_sec -= (7*24*60*60)-1; 
        then.tv_usec = 0;
        startof["prev_week"] = then;
        tm_then.tm_mday = 1;
        tm_then.tm_hour = 0;
        tm_then.tm_min = 0;
        tm_then.tm_sec = 0; // midnight
        // do start of next month then work back (days in month not constant)
        if(tm_then.tm_mon == 11) {
            // rollover
            tm_then.tm_mon = 0;
            tm_then.tm_year++;
            then.tv_sec = mktime(&tm_then);
            tm_then.tm_year--;
            tm_then.tm_mon = 11; 
        }
        else {           
            tm_then.tm_mon++;
            then.tv_sec = mktime(&tm_then);
            tm_then.tm_mon--;
        }
        
        // remember this was start of next month so take one sec off
        then.tv_sec--;
        then.tv_usec = 999999;
        endof["this_month"] = then;

        
        // get start of this month
        then.tv_sec = mktime(&tm_then);
        then.tv_usec = 0;
        startof["this_month"] = then;   
        then.tv_sec -= 1;
        then.tv_usec = 999999;
        endof["prev_month"] = then;
        // lastly start of the prev month
        if(tm_then.tm_mon == 0) {
            // rollover
            tm_then.tm_mon = 11;
            tm_then.tm_year--;
            then.tv_sec = mktime(&tm_then);
        }
        else {           
            tm_then.tm_mon--;
            then.tv_sec = mktime(&tm_then);
        }
        then.tv_usec = 0;
        startof["prev_month"] = then;  
               
    };
    
    // given a label return the start time (seconds part) associated with that
    // label
    time_t start(const std::string &label) const {
        // map operators dont like working on const data - which we may get passed
        // this is a kludge to work around that. (nc = not const :)

        std::map<std::string,timestamp>& nc_startof = const_cast<std::map<std::string,timestamp>& >(startof);
        
        if(nc_startof.count(label) > 0)
            return nc_startof[label].t.tv_sec;
        else
            return (time_t)0;
    
    };
    // given a label return the end time (seconds part) associated with that
    // label
    time_t end(const std::string &label) const {
        std::map<std::string,timestamp>& nc_endof = const_cast<std::map<std::string,timestamp>& >(endof);
        if(nc_endof.count(label) > 0)
            return nc_endof[label].t.tv_sec;
        else
            return (time_t)0;
    
    };
    // when given a label and a timestamp true if this timestamp is in this range
    bool in_range(const std::string &label,const timestamp &tstamp) const {
        std::map<std::string,timestamp>::iterator i;
        std::map<std::string,timestamp>::iterator j;
        std::map<std::string,timestamp>& nc_startof = const_cast<std::map<std::string,timestamp>& >(startof);
        std::map<std::string,timestamp>& nc_endof = const_cast<std::map<std::string,timestamp>& >(endof);
        
        // find needs a non const map to work with
        i = nc_startof.find(label);
        j = nc_endof.find(label);

        if( i == nc_startof.end() || j == nc_endof.end()) {
            return false; // bad label!
        }
        // use refs so things are visible in the debugger.
        // also probably more efficient as short-circult
        // some expensive lookups.
        const timestamp &start = i->second;
        const timestamp &end = j->second;

        // well - is this time in this range or not?
        bool range = (start.t.tv_sec <= tstamp.t.tv_sec &&
                      start.t.tv_usec <= tstamp.t.tv_usec &&
                      tstamp.t.tv_sec <= end.t.tv_sec &&
                      tstamp.t.tv_usec <= end.t.tv_usec);
        // again we use a temp var so we can have a chance to see value before
        // returning.
        return range;
    };

    // this given a timestamp returns vector of strings
    // that fall within that time
    // strings are then used for as index parts to get to matching data
    // i.e. in perl grep(&in_range($_,$tstamp), keys %startof)
    std::vector<std::string> in_range (const timestamp &tstamp) const {
        std::vector<std::string> range;
        std::map<std::string, timestamp> ::iterator i;
        std::map<std::string,timestamp>& nc_startof = const_cast<std::map<std::string,timestamp>& >(startof);
        // use startof as a source for labels
        for(i = nc_startof.begin(); i != nc_startof.end(); i++) {
            
            // is this in range - startof and endof tested against
            if(in_range(i->first, tstamp)) {
                // in this range
                range.push_back(i->first);
            }
        }
        return range;
    };

    // and return all the current i.e. this_ ranges we know about
    // i.e in perl grep(/this_/, keys %startof)
    std::vector<std::string> curr_range() const {
         std::vector<std::string> range; 
         std::map<std::string, timestamp> ::iterator i;
         std::map<std::string,timestamp>& nc_startof = const_cast<std::map<std::string,timestamp>& >(startof);
         for(i = nc_startof.begin(); i != nc_startof.end(); i++) {
             // does the range name match what we are after?
             if(i->first.find("this_") != std::string::npos)
                 range.push_back(i->first);
         } 
         return range;
    };   
    // and return all the previous i.e. prev_ ranges we know about
    // i.e in perl grep(/prev_/, keys %startof)
    std::vector<std::string> prev_range() const {
         std::vector<std::string> range; 
         std::map<std::string, timestamp> ::iterator i;
         std::map<std::string,timestamp>& nc_startof = const_cast<std::map<std::string,timestamp>& >(startof);
         for(i = nc_startof.begin(); i != nc_startof.end(); i++) {
             if(i->first.find("prev_") != std::string::npos)
                 range.push_back(i->first);
         } 
         return range;
    };   
    // and return all ranges we know about
    // i.e in perl keys %startof
    std::vector<std::string> all_range() const {
         std::vector<std::string> range; 
         std::map<std::string, timestamp> ::iterator i;
         std::map<std::string,timestamp>& nc_startof = const_cast<std::map<std::string,timestamp>& >(startof);
         for(i = nc_startof.begin(); i != nc_startof.end(); i++) {
            
             range.push_back(i->first);
         } 
         return range;
    };
};
#endif
