// This collects live stats and displays them as a realtime graph using curses
// C++ interface between smoothwall and tc/netfilter/ipt_ACCOUNT statistics collection
// (c) SmoothWall Ltd 2007
// This code is distributed under the terms of the GPL v2.
// Written by Martin Houston <martin.houston@smoothwall.net> 2004 - 2007

// Martin Houston, Smoothwall Ltd.
#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <time.h>
#include "traffic_config.hpp"
#include "trafstats_core.hpp"
#include </usr/include/linux/types.h>
#include </usr/include/netinet/in.h> 



// these are needed to keep the iptables libs happy
const char *program_name = "trafficmon";
const char *program_version = "4";
bool quiet_mode = false;
bool graph_mode = false;

// here is our config class
traf_config_hash traf_config;
// signal hander so we cleanly finish using curses
static void finish(int sig);

// signal hander so we can cope with X term resize events
static void resize(int sig);

// package all the repeatable stuff from when we start curses both
// originally and after a resize event
static void start_curses();

// these are globals so render_line can pick them up
// displayable rows
int screen_max_rows = 0;
// displayable cols
int screen_max_cols = 0;
int do_resize = 0;
// where we start - 0 to begin with, gets updated as scroll
int start_row = 0;
// total amount of data there is to show
int max_rows = 0;

void render_line(int linenum,const char *direction, 
					const char *legend, const char *dev,
					double barpercent, const char *shownrate,
					int bgcolor = COLOR_BLACK) {
	
	int startpos, graphcols, graphlimit;
	int barcolour;
	int barchar;
	int i;
	// are we below what is visible or above?

	// first return if not this many lines on the screen
	if(linenum < start_row || linenum > (start_row+(screen_max_rows-1)))
		return;
	linenum -= start_row; // displaying this section on screen

	startpos = 0;
	graphcols = screen_max_cols - startpos;
	graphlimit = int(graphcols * barpercent);
	if(barpercent < 0.30) {
		barcolour = COLOR_GREEN;
		barchar = ACS_HLINE; // for mono terminals
	}
	else if(barpercent < 0.75) {
		barcolour = COLOR_YELLOW;
		barchar = ACS_BOARD; // chequered block > 30%
	}
	else {
		barcolour = COLOR_RED;
		barchar = ACS_BLOCK; // solid block > 75%
	}
	// be more pretty if can use background colour to show the graph,
	// else use _
	
	mvprintw(linenum,0,"%s %s", direction,dev); 
	clrtoeol();
	// partial overwrite of the legend possible
	mvprintw(linenum,12," %s",legend); 
	
	if(!has_colors()) {
		// need the line of whatever character is appropriate
		for(i = 0; i < graphlimit;i++) addch(barchar);
	}
		// now the rate - put on top of the bar in the case of the no color version
	mvprintw(linenum, 40 + ((screen_max_cols-startpos)/2-strlen(shownrate)),shownrate);
	if(has_colors()) {
		// whole line background
		mvchgat(linenum,0,screen_max_cols,A_NORMAL,bgcolor,NULL);
		// show the graph part as a colour if possible
		mvchgat(linenum,startpos,graphlimit, A_NORMAL,barcolour, NULL );
	}
}

int handle_input() {
	int c = getch();
	
	switch(c) {
	case KEY_UP:
		
		if(start_row > 0)
			start_row--; // back down towards 0
		break;
		
	case KEY_PPAGE:
		if(start_row > screen_max_rows+1) // are we > than size of a page
			start_row -= (screen_max_rows+1); // if so can go back
		else
			start_row = 0; // else all the way back to the beginning
		break;
		
		case KEY_DOWN:
			if(start_row < (max_rows - screen_max_rows))
	start_row++; // can show more
			break;
			
	case KEY_NPAGE:
		if(start_row < (max_rows - (screen_max_rows*2)))
			start_row += screen_max_rows;
		else
			start_row = (max_rows - screen_max_rows); // last visible page
		break;
	}
	return c;
}
// exit curses in an orderley fashon.

static void finish(int sig)
{

	if(!quiet_mode && !graph_mode)
		endwin();
	
	// any non-curses wrapup here

	exit(0);
}

// Can't call curses funcs reliably during signal handling so
// just set a global flag.

static void resize(int sig)
{
	(void) signal(SIGWINCH, SIG_IGN); 
	do_resize = 1; // set the flag
	 
}

// encapsulate this into a func so can reinit after window change too.
// dont care about any previous window contents
static void start_curses() {
	 
	(void) signal(SIGWINCH, resize);
	
	(void) initscr();      // initialize the curses library
	keypad(stdscr, TRUE);  // enable keyboard mapping 
	(void) nonl();         // tell curses not to do NL->CR/NL on output 
	(void) nodelay(stdscr,true);       // take input chars one at a time, no wait for \n 
	(void) noecho();       // don't echo input 
	curs_set(0);           // make cursor invisible - no visible user input

	if (has_colors()) {
		start_color();
		// fg first then bg
		// white text on black
		init_pair(COLOR_BLACK, COLOR_WHITE,COLOR_BLACK);
		// white text on dark blue
		init_pair(COLOR_BLUE, COLOR_WHITE, COLOR_BLUE);
		init_pair(COLOR_WHITE, COLOR_BLACK, COLOR_WHITE);
		init_pair(COLOR_CYAN, COLOR_BLACK, COLOR_CYAN);

		// graph bar colours
		init_pair(COLOR_GREEN, COLOR_BLACK, COLOR_GREEN);
		init_pair(COLOR_YELLOW, COLOR_BLACK, COLOR_YELLOW);
		init_pair(COLOR_RED, COLOR_BLACK, COLOR_RED);
	}
}


	// if in quiet mode then we write 
int main(int argc, char *argv[]) {

	bool interfaces_only = false;
	bool use_account = true;
	bool show_classes = true;
	bool show_rules = true;
	char *quietfile = NULL;
	int runtime = 0;
	int delay = 250000; // 1/4 of a sec
	double divfactor = 1024.0; 
	double bitrate = 0;
	double nowrate = 0;
	double maxrate = 1; // set to biggest rate ever seen by this run, start at 1 to avoid / by 0 err
	if(argc > 1) {
		if(!strcmp(argv[1], "-q")) {
			quiet_mode = true;
			delay = 990000; // just under 1 sec
			quietfile = argv[2];
			interfaces_only = true;
			runtime = (argc > 3 ? atoi(argv[3]) : 0); // how long to run for
		}
		else if(!strcmp(argv[1], "-g")) {
			graph_mode = true;
			delay = 990000; // just under 1 sec
			quietfile = argv[2];
			runtime = (argc > 3 ? atoi(argv[3]) : 0); // how long to run for
		}
		else {
			if(atoi(argv[1]) > 0) {
				delay = atoi(argv[1]);
			}
			else {
				fprintf(stderr, "trafficmon [-q|-g] file [runtime] - for quiet or graph mode or just trafficmon <delay> where delay is in microsecs\nWARNING - graph mode file will be appended to 4 times a second and will grow rapidly!");
			}
		}
	}
	if(traffic_iptables_missing()) {
		
		fprintf(stderr,"tables not in place yet, cannot run\n");
		exit(0);
	}
	if(traffic_account_missing()) {
		use_account = false;
	}


	Vtraf_stat_samples s;
	Vtraf_stat_samples_iterator si;
	StrSet_iterator devidx,ruleidx;
	Vstring_iterator classidx;
	StrSet_iterator addressidx;
	Vtraf_iterator ia,ib;
	(void) signal(SIGINT, finish); 
	if(!(quiet_mode||graph_mode)) {
		// now init curses
		start_curses();
	
		max_rows = 0;
		getmaxyx(stdscr,screen_max_rows,screen_max_cols); // get the number of rows and columns 
		
		if(!collect_a_sample(s, interfaces_only,use_account)) {
			move(0,0);
			clrtobot();
			printw("Cannot get first sample");
			refresh();
			sleep(1);
			move(0,0);
			clrtobot();
			exit(1);
		}
		usleep(delay); // pause a short while
	 
		// loop until killed...
		for (;;) {
			timestamp now, last_sec,avg_time,expiry;
			gettimeofday(&now.t, NULL);

			last_sec = avg_time = expiry = now;
			// take off 1/2 a sec
			if(last_sec.t.tv_usec >= 500000) {
				// just trim microsecs
					last_sec.t.tv_usec -= 500000;
			}
			else {
				// in 1st half of sec so go back 1 and forward a half
				last_sec.t.tv_sec -= 1;
				last_sec.t.tv_usec += 500000;
			}
			// fudge factor
			last_sec.t.tv_sec -= 1;
			avg_time.t.tv_sec -= 20; // 20 sec average, so doesnt jump madly
			expiry.t.tv_sec -= (21); // Any data 21 sec old is not needed anymore
			// we only need last 20 secs of data
			truncate_sample_set(s, expiry);
			
			// winge if reached 1000 samples - the truncate is failing
			if(s.size() > 1000) {
	
				move(0,0);
				clrtobot();
				printw("sample set %d",s.size());
				refresh();
				sleep(1);
			}
		
			int linenum = 0;
			if(do_resize) {
				endwin();
				start_curses();
				refresh();
				do_resize = 0;
				getmaxyx(stdscr,screen_max_rows,screen_max_cols); 
			}
			(void) handle_input();
			move(0,0);
		
			if(!collect_a_sample(s, interfaces_only, use_account)) {
				clrtobot();
				printw("Cannot get next sample");
				refresh();
				sleep(1);
				move(0,0);
				clrtobot();
			}
			else {
				usleep(delay);
			}
		
		
			// now render the samples
			// first another chance to respond to keypress
			(void) handle_input();
			
			traf_stat_collection_item & last = s.back();
			StrSet &ext_devs = last.ext_devs;
			StrSet &int_devs = last.int_devs;
			StrSet &addresses = last.addresses;
		
			for (devidx = ext_devs.begin(); devidx != ext_devs.end(); devidx++) {
				const std::string & dev = *devidx;
				std::string label = dev + "_ext_in";
				// take a ref to this particular item
				if(last.stats.count(label) > 0) {
					nowrate = average_bit_rate(s,label,last_sec, now); 
					if(nowrate > maxrate) maxrate = nowrate; // set new maxima
					bitrate = average_bit_rate(s,label,avg_time, now);
					if(bitrate > 0) {
						render_line(linenum++,
									"DOWN",
									"ext in", 
									dev.c_str(), 
									nowrate/maxrate, 
									format_rate(bitrate, nowrate).c_str(), 
									COLOR_BLUE);
					}
				}
				label = dev + "_ext_out";
	
				if(last.stats.count(label) > 0) {
		
					nowrate = average_bit_rate(s,label,last_sec, now); 
					if(nowrate > maxrate) maxrate = nowrate; // set new maxima
					bitrate = average_bit_rate(s,label,avg_time, now);
					if(bitrate > 0) {
						render_line(linenum++,
									"UP  ",
									"ext out", 
									dev.c_str(), 
									nowrate/maxrate, 
									format_rate(bitrate, nowrate).c_str(), 
									COLOR_BLUE);
					}
				}
			}
			// end of externals
			for (devidx = int_devs.begin(); devidx != int_devs.end(); devidx++) { 
				const std::string & dev = *devidx;
				std::string label = dev + "_int_out";
				if(last.stats.count(label) > 0) {
					nowrate = average_bit_rate(s,label,last_sec, now); 
					if(nowrate > maxrate) maxrate = nowrate; // set new maxima
					bitrate = average_bit_rate(s,label,avg_time, now);
					if(bitrate > 0) {
						render_line(linenum++,
									"DOWN",
									"int out", 
									dev.c_str(), 
									nowrate/maxrate, 
									format_rate(bitrate, nowrate).c_str(), 
									COLOR_BLUE);
					}
				}
				label = dev + "_int_in";
				if(last.stats.count(label) > 0) {
					nowrate = average_bit_rate(s,label,last_sec, now); 
					if(nowrate > maxrate) maxrate = nowrate; // set new maxima
					bitrate = average_bit_rate(s,label,avg_time, now);
					if(bitrate > 0) {
						render_line(linenum++,
									"UP  ",
									"int in", 
									dev.c_str(), 
									nowrate/maxrate, 
									format_rate(bitrate, nowrate).c_str(), 
									COLOR_BLUE);
					}
				}
			}
			// end of internals  
			// Now breakdown of flow through classes, on a CYAN background
			Vstring classes = last.class_indexes_in_order();
			StrSet & rules = last.rules;
		 
			if(show_classes) {
				for (devidx = last.int_devs.begin(); devidx != last.ext_devs.end(); devidx++) {
					if(devidx == int_devs.end())
						devidx = ext_devs.begin();
		
					for (classidx = classes.begin(); classidx != classes.end(); classidx++) { 
						const std::string & dev = *devidx;
						const std::string & tclass = *classidx;
						std::string label = dev + "_dn_class_" + tclass;
						if(last.stats.count(label) > 0) {
							nowrate = average_bit_rate(s,label,last_sec, now); 
							if(nowrate > maxrate) maxrate = nowrate; // set new maxima
							bitrate = average_bit_rate(s,label,avg_time, now);
							if(bitrate > 0) {
								render_line(linenum++,
											"DOWN",
											tclass.c_str(), 
											dev.c_str(), 
											nowrate/maxrate, 
											format_rate(bitrate, nowrate).c_str(), 
											COLOR_CYAN);
							}
						}
						label = dev + "_up_class_" + tclass;
						if(last.stats.count(label) > 0) {
							nowrate = average_bit_rate(s,label,last_sec, now); 
							if(nowrate > maxrate) maxrate = nowrate; // set new maxima
							bitrate = average_bit_rate(s,label,avg_time, now);
							if(bitrate > 0) {
								render_line(linenum++,
											"UP  ",
											tclass.c_str(), 
											dev.c_str(), 
											nowrate/maxrate, 
											format_rate(bitrate, nowrate).c_str(), 
											COLOR_CYAN);
							}
						}
					} // for classes
				} // for devs
			}

			// if traffic operational show rules on a WHITE background
			if(show_rules) {
				for (devidx = int_devs.begin(); devidx != ext_devs.end(); devidx++) { 
					if(devidx == int_devs.end())
						devidx = ext_devs.begin();
		
					const std::string & dev = *devidx;
					for (ruleidx = rules.begin(); ruleidx != rules.end(); ruleidx++) {
						const std::string & rule = *ruleidx;
						std::string label = dev + "_dn_rule_" + rule;
						if(last.stats.count(label) > 0) {
							nowrate = average_bit_rate(s,label,last_sec, now); 
							if(nowrate > maxrate) maxrate = nowrate; // set new maxima
							bitrate = average_bit_rate(s,label,avg_time, now);
							if(bitrate > 0) {
								render_line(linenum++,
									"DOWN",
									rule.c_str(), 
									dev.c_str(), 
									nowrate/maxrate, 
									format_rate(bitrate, nowrate).c_str(), 
									COLOR_WHITE);
							}
						}
						label = dev + "_up_rule_" + rule;
						if(last.stats.count(label) > 0) {
							nowrate = average_bit_rate(s,label,last_sec, now); 
							if(nowrate > maxrate) maxrate = nowrate; // set new maxima
							bitrate = average_bit_rate(s,label,avg_time, now);
							if(bitrate > 0) {
								render_line(linenum++,
											"UP  ",
											rule.c_str(), 
											dev.c_str(), 
											nowrate/maxrate, 
											format_rate(bitrate, nowrate).c_str(), 
											COLOR_WHITE);
							}
						}
					} // for rules
				} // for devs
			} // show rules

			// now addresses
			if(use_account) {
					for (addressidx = addresses.begin(); addressidx != addresses.end(); addressidx++) { 
						std::string label = *addressidx;
						if(last.stats.count(label) > 0) {
							std::string direction = "(dn)";
							std::string addr = "0.0.0.0";
							std::string::size_type pos = label.find("(");
							if(pos != std::string::npos) {
								direction = label.substr(pos);
								addr = label.substr(0,pos);
							}
									
							nowrate = average_bit_rate(s,label,last_sec, now); 
							if(nowrate > maxrate) maxrate = nowrate; // set new maxima
							bitrate = average_bit_rate(s,label,avg_time, now);
							if(bitrate > 0) {
								render_line(linenum++,
									(direction == "(dn)" ? "DOWN" : "UP  "),
									addr.c_str(), 
									"address", 
									nowrate/maxrate, 
									format_rate(bitrate, nowrate).c_str(), 
									COLOR_WHITE);
							}
						}

					} // for address

			} // use account
		
			max_rows = linenum; 
			move(max_rows,0);
			clrtobot(); // rest of the screen
			refresh();  
			// now loose any unchanged stats
			last.compress();
			
			
			if(max_rows == 0) {
				time_t now;
				time(&now);
				printw("%s", ctime(&now));
				mvprintw(screen_max_rows/2, (screen_max_cols-strlen("NO TRAFFIC AT PRESENT"))/2,"NO TRAFFIC AT PRESENT");
				refresh();
				sleep(1); // ease load
			}
		} // main loop
	}
	else {
		// quiet/graph mode - get stats into a file - use C io for speed
		// just report what speeds are there in the most recent sample, only need to keep 2 samples
		if(collect_a_sample(s, interfaces_only, use_account)) {
			// got one sample at least
			usleep(delay); 
			time_t timetoend = 0;// not active
			if(runtime) {
				time(&timetoend);
				timetoend += runtime;
			}
			for(;;) {
				timestamp now, last_sec,expiry;
				gettimeofday(&now.t, NULL);
				if(timetoend && now.t.tv_sec > timetoend)
					break; // finished
				last_sec = expiry = now;
				// take off 1/2 a sec
				if(last_sec.t.tv_usec >= 500000) {
					// just trim microsecs
					last_sec.t.tv_usec -= 500000;
				}
				else {
					// in 1st half of sec so go back 1 and forward a half
					last_sec.t.tv_sec -= 1;
					last_sec.t.tv_usec += 500000;
				} 
	
				expiry.t.tv_sec -= (3); // Any data 3 sec old is not needed anymore
				truncate_sample_set(s, expiry);
				if(collect_a_sample(s, interfaces_only, use_account)) {
					// now have at least two samples
					traf_stat_collection_item & last = s.back(); // take a ref to make following code less nasty
		
		
					StrSet &ext_devs = last.ext_devs;
					StrSet &int_devs = last.int_devs;
					StrSet & rules = last.rules;
					StrSet & addresses = last.addresses;
					int direction;
					std::string out = "";
					int i = 0;
					int j;
					FILE * fd;
					// we need names of all the rules, names of interfaces and their speeds (down then up)
		
					for (ruleidx = rules.begin(); ruleidx != rules.end(); ruleidx++) {
						const std::string & rule = *ruleidx;
						out += itostr(i++) + ":" + rule + ",";
					}
					out += "\n";
		
					i = 0;
					for (devidx = int_devs.begin(); devidx != int_devs.end(); devidx++) { 
			
			 			const std::string & dev = *devidx;
			 			const std::string & upspeed = dtostr(traf_config[dev].interface_speed("upload")/divfactor);
			 			const std::string & dnspeed = dtostr(traf_config[dev].interface_speed("download")/divfactor);
			 			double dnrate;
			 			double uprate;	
			 			std::string label = dev + "_int_in";
			 			dnrate = average_bit_rate(s,label,last_sec, now);
						if(dnrate > maxrate) maxrate = dnrate;

			 			label = dev + "_int_out";
			 			uprate = average_bit_rate(s,label,last_sec, now);
						if(uprate > maxrate) maxrate = uprate;
						// if we have configured speeds then use them - otherwise base on max speed seen so far
						if( traf_config[dev].interface_speed("upload") != 0 && 
							traf_config[dev].interface_speed("download") != 0)
			 				out += itostr(i++) + ":" + dev + ":" + dtostr(dnrate/divfactor) + ":" + dnspeed + ":" + dtostr(uprate/divfactor) + ":" + upspeed + ",";
						else
			 				out += itostr(i++) + ":" + dev + ":" + dtostr(dnrate/divfactor) + ":" + dtostr(maxrate/divfactor) + ":" + dtostr(uprate/divfactor) + ":" + dtostr(maxrate/divfactor) + ",";
		 			} 
		 			for (devidx = ext_devs.begin(); devidx != ext_devs.end(); devidx++) { 
			 
			 			const std::string & dev = *devidx;
			 			const std::string & upspeed = dtostr(traf_config[dev].interface_speed("upload")/divfactor);
			 			const std::string & dnspeed = dtostr(traf_config[dev].interface_speed("download")/divfactor);
			 			double dnrate;
			 			double uprate;	
			 			std::string label = dev + "_ext_in";
			 			dnrate = average_bit_rate(s,label,last_sec, now);
						if(dnrate > maxrate) maxrate = dnrate;

			 			label = dev + "_ext_out";
			 			uprate = average_bit_rate(s,label,last_sec, now); 
						if(uprate > maxrate) maxrate = uprate;

						if( traf_config[dev].interface_speed("upload") != 0 && 
							traf_config[dev].interface_speed("download") != 0)
			 				out += itostr(i++) + ":" + dev + ":" + dtostr(dnrate/divfactor) + ":" + dnspeed + ":" + dtostr(uprate/divfactor) + ":" + upspeed + ",";
						else
			 				out += itostr(i++) + ":" + dev + ":" + dtostr(dnrate/divfactor) + ":" + dtostr(maxrate/divfactor) + ":" + dtostr(uprate/divfactor) + ":" + dtostr(maxrate/divfactor) + ",";
					}
		 			out += "\n";
		 
		 			// rules only there if traffic running
					bool seenrule = false;
					bool seenaddr = false;
		 			for ( i = 0, ruleidx = rules.begin(); ruleidx != rules.end(); i++,ruleidx++) {
						const std::string & rule = *ruleidx;
						for (j=0,devidx = int_devs.begin(); devidx != ext_devs.end(); j++,devidx++) { 
							if(devidx == int_devs.end())
								devidx = ext_devs.begin();
							const std::string & dev = *devidx;
							for(direction = 0; direction <= 1; direction++) {
								std::string label = dev + "_" + (direction ? "up" : "dn" ) + "_rule_" + rule;
								nowrate = average_bit_rate(s,label,last_sec, now);
								if(nowrate > maxrate) maxrate = nowrate;
								seenrule = true;
								out += itostr(i) + ":" + itostr(j) + ":" + itostr(direction) + ":" + dtostr(nowrate/divfactor) + ","; 
							}
						}
		 			} 
		 			if(seenrule) out += "\n"; 
					// third line if any address info
					if(use_account) {
						for (addressidx = addresses.begin(); addressidx != addresses.end(); addressidx++) { 
							std::string label = *addressidx;
							if(last.stats.count(label) > 0) {
								nowrate = average_bit_rate(s,label,last_sec, now); 
								if(nowrate > maxrate) maxrate = nowrate; // set new maxima
								seenaddr = true;
								out += label + ":" + dtostr(nowrate/divfactor) + ",";
							}
						}
					}
		 			if(seenaddr) out += "\n"; 
						 
					 

		 			out += itostr(last_sec.t.tv_sec) + ":" + itostr(last_sec.t.tv_usec) + "\n";
		 			// get the writing over with quickly
		 			if((fd = fopen(quietfile, (quiet_mode ? "w" : "a")))!= NULL) {
			 			fputs(out.c_str(), fd);
			 			fclose(fd);
		 			}
		 			// else dont worry - just try next time round
				}
				else {
					// wait for things to settle
					sleep(1);
				}
				usleep(delay);
			} // end of loop
		}
	}
}

