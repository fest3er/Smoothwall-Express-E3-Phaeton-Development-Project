#ifndef IPT_VIS_INC
#define IPT_VIS_INC
// ipt_vis - a curses front end onto iptableproc
// (c) 2006 Martin Houston <martin.houston@smoothwall.net>
// Written for speed! There is no point in distorting what is being measured by the measurement process.
// Released under the GPLv2

#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <sys/select.h>
#include <curses.h>
#include <signal.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "expat.h" // for the expat XML parser

class ipt_vis;

typedef unsigned long long ulong_t;

// callback for actual curses rendering of a line
// linenum is which line within the curses vitual screen we think we are
// legend is what to put at the left of the screen
// barpercent how much of the screen to display the bar as
// bar_colour - what colour the bar is
// text_colour - what colour the text is
// bg_colour - what colour the background is
// shownrate - what to display at right of screen
void render_line (int linenum, const char *legend, double barpercent,
		 int bar_colour, int bg_colour,
		 const char *shownrate);


// this is all self contained so no data hiding needed
class data_line {
public:
  bool working;
  bool gotsample;
  unsigned int numlines; // 1 unless this is a whole chain
  unsigned int numrules; // number of rules we need to collect in order to show numlines (can only collect whole chains)
  unsigned int numsamples;
  unsigned int avsamples; // important until our sample buffer is full, so that
                          // in order to get an average we devide by the 
  // number of samples actualy  collected.
  // once we have been running long enough for the buffer to be full this stays
  // == numsamples
  unsigned int curses_start_line; // where in the screen buffer to render this
  std::string name; // may be comma separated if whole chain
  std::string table;
  std::string chain;
  std::string rule; // may be comma separated if whole chain
  std::string bg_colour;
  std::string bar_colour; // background attribute to use as current rate indicator;
  std::string max_rate; // max rate in either bytes or packets per sec
  std::string output_format; // how to show the rates
  ipt_vis *container; // for globaly applicable  stuff 
  ulong_t **counters; 
  double *rates; // for the instant rate calculations
  double *averages; // for the average calculations
 
  unsigned int cur_counter; // which counter is the current one - cycles round in buffer
  // once we get going the instantaneous rate is this sample - previous one and
  // rate over the sample period is this sample - the one we replace.
  // this saves tedious and timeconsuming scanning through the table
  // this is lean & mean
  int iptableproc_handle; // handle for reading the counters on;

  // The data_line represents the association with a single file descriptor 
  // in iptableproc. We need to know two things, How many rules we are looking at
  // which in the case of individual rules will be 1 and in the case of a chain a number big enough
  // to hold all the counters up to the last we are interested in.
  // e.g. we have a chain with 100 rules and we are interested in rules 1,5,and 57 - we need to say that
  // numrules is 57
  // numsamples is sample rate per sec multiplied by the number of seconds we want to keep the rolling average over
  data_line(ipt_vis *pcontainer, int pnumlines, int pnumrules, int pnumsamples) {
    int i;
    
    working = false;
    container = pcontainer; // who we belong to
    gotsample = false; // set to true once read first sample
    if(pnumlines < 1)
      pnumlines = 1;
    if(pnumsamples < 1)
      pnumsamples = 1;
    numlines = pnumlines;
    numrules = pnumrules;
    numsamples = pnumsamples;
    name = "";
    table = "";
    chain = "";
    rule = "";
    bg_colour = "";
    bar_colour = "";
    max_rate = "";
    output_format = "";
    // stage 1 allocate the pointers to pointers array - plus extra pointer for NULL termination
    // first index is which part of the sample history buffer we are in
    // always have one more sample than ask for
    counters = (ulong_t **)calloc(numsamples+2, sizeof(ulong_t **));
    if(counters) {
      for(i = 0; i < numsamples+1; i++)
	// second index is the counters for the rules themselves.
	counters[i] = (ulong_t *)calloc(numrules+1, sizeof(ulong_t));
      counters[numsamples+1] = NULL;
    }
    rates = (double *)calloc(numrules+1, sizeof(double)); // calloc means they start zero
    averages = (double *)calloc(numrules+1, sizeof(double)); // calloc means they start zero
    // no filehandle yet
    iptableproc_handle = -1;
    cur_counter = 0;
  };

  // called to open our iptableproc_handle
  // return false if cant do it
  // also set the interval for that rule to our global one.
  // if the handle is not specified it defaults to raw_bcnt - raw byte count
  // the only other valid choise is raw_pcnt
  bool open_handle(std::string data = "raw_bcnt");
  
  void close_handle() {
    if(iptableproc_handle >= 0)
      close(iptableproc_handle);
  }
  void get_sample();
  
  int curses_render();
  int plain_render(FILE *outf = stdout);
  // have to free the low level stuff we calloced
  ~data_line() {
    int i;
    // did we open a handle?
    this->close_handle();
    if(counters) {
      // 2 level alloc so needs 2 level free
      for(i = 0; counters[i]; i++)
	free(counters[i]);
      free(counters);
    }
    if(rates)
      free(rates);
    if(averages)
      free(averages);
  };

  friend std::ostream &  operator << (std::ostream & out, const data_line & d) {
    int i,j;
    out << "name=\"" << d.name << "\" table=\"" << d.table << "\" chain=\"" << d.chain << "\" rule=\"" << d.rule << "\"" << std::endl;
    out << "bg_colour=\"" << d.bg_colour << "\" bar_colour=\"" << d.bar_colour << "\" max_rate=\"" << d.max_rate << "\"" << std::endl;
    out << "numlines=\"" << d.numlines << "\" numrules=\"" << d.numrules  << "\"" << std::endl;
    out << "Raw counters:" << std::endl; 
    for(i = 0; i < d.numsamples; i++) {

      out << (i == d.cur_counter ? "*" : " " ) << " Sample " << i+1 << std::endl;
      for(j = 0; j < d.numrules; j++) {
        
	out << "Rule " << j+1 << " " << d.counters[i][j] << std::endl;
      } 	
    }
    out << "Current Rates:" << std::endl; 
    for(i = 0; i < d.numrules; i++) {
      out <<  " Rule " << i+1 << " " << d.rates[i] << std::endl;
    }
    out << "Current Averages:" << std::endl; 
    for(i = 0; i < d.numrules; i++) {
      out <<  " Rule " << i+1 << " " << d.averages[i]/d.numsamples << std::endl;
    }
    return out;
  };
};

// This is the container for the info we are collecting.
// Exactly what and where is defined by the xml file so we come into existance by parsing it
// in using the venerable expat.

class ipt_vis {
public:
  bool working; // is this a viable object (not messing round with throwing exceptions for errors)
  std::string title; // optional but if set will be displayed at top of screen
  std::string samples_per_sec; // how many samples a sec to take
  std::string average_over; // how many samples to keep in circular buffer
  std::string measuring; // bytes or packets
  std::string output_method; // curses or plain (maybe more later)
  std::string table;
  std::string max_rate;
  std::string bg_colour;
  std::string bar_colour; // globals so individual lines dont need to specify
  std::string output_format;  
  FILE *outfile; // if we are writing to a file directly
  std::vector<data_line *> lines; // 1 'line' represents a single collection point from iptableproc
  // may represent several on screen lines if a whole chain
  fd_set *select_fds; // for selecing on for read multiplexing
  // you can find the constructor in the .cpp file as its a bit big!
  ipt_vis(const char *xml_file); // comes into existance from the config file

  // destructor is simple though
  ~ipt_vis() {
    lines.clear();
  };
  // XML parser callbacks - 
  // what gets called when XML entities are seen
  void start_hdlr(void *data, const XML_Char *el, const XML_Char **attr);
  void end_hdlr(void *data, const XML_Char *el);

  // and print ourselves out for debug
  friend std::ostream &  operator << (std::ostream & out, const ipt_vis & v) {
    int i;
    out << "samples_per_sec=\"" << v.samples_per_sec << "\" average_over=\"" << v.average_over << "\" measuring=\"" << v.measuring << "\""<< std::endl;
    for( int i = 0; i < v.lines.size(); i++ ) {
      out << "Entry " << i << " ";
      out << *(v.lines[i]);
    }
    return out;
  };

  // refreshing the counters has to be done for the whole collection at once - we use 
  // a select and keep reading until have all the data we need for this timeslice,
  // then stop

  bool get_samples(void);
  // Now use curses etc. the samples we have
  void render_samples(void);
}; 

#endif
