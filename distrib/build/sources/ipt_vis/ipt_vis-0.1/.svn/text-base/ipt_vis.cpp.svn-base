// ipt_vis - a curses front end onto iptableproc
// an xml file is used to describe which rules to read and what
// format the output should be in.
// choices are curses, plain and anything else which is assumed to be a filename in which to write a single entry in plain form
// see README
// (c) 2006 SmoothWall Ltd. Author: <martin.houston@smoothwall.net>
// Released under the GPLv2

#include "ipt_vis.hpp"

// curses helpers
static void finish(int sig);

// signal hander so we can cope with X term resize events
static void resize(int sig);

// package all the repeatable stuff from when we start curses both
// originally and after a resize event
static void start_curses();

static int handle_input();

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

struct strCmp {
  bool operator()( const char* s1, const char* s2 ) const {
    return strcmp( s1, s2 ) < 0;
  }
};

std::map<const char *, int, strCmp> curses_colours;
static int next_screen_line = 0; // so can work out where each rule starts on screen

// 
// The expat library is C not C++ and we want to get to our callback member function ASAP
// so we pass a pointer to it as the data so all the real work can be done by the member function.
static void wrapper_start_hdlr(void *data, const XML_Char *el, const XML_Char **attr) {
  ipt_vis *p = (ipt_vis *)data;
  if(p)
    p->start_hdlr(data,el,attr);
}

// we dont have any need for end of entity handling yet but may do at some point...
static void wrapper_end_hdlr(void *data, const XML_Char *el) { 
  ipt_vis *p = (ipt_vis *)data;
  if(p)
    p->end_hdlr(data,el);
}



// If this attribute matches the string we are after copy its value into our target string
// saves messy if/elses
inline bool assign(std::string & x,  const char *n, const XML_Char **attr, int i) {
  if(!strcmp(n, attr[i])) {
    x = attr[i+1];
    return true;
  }
  else {
    return false;
  }
}

// we have two sorts of object:
// outer object is ipt_vis
// inner repeating object is entry, if things are not defined in the entry they are
// taken from the parent.
// its is in no way a validating parser - quick and dirty

void ipt_vis::start_hdlr(void *data, const XML_Char *el, const XML_Char **attr) {
  int i,n,max;
  int commacount = 0;
  std::string rule = "";
  const char *crule;

  if(!strcmp(el, "ipt_vis")) {
    // some defaults
    measuring = "bytes";
    samples_per_sec = "1";
    average_over = "1";
    title = "";
    output_method = "plain";
    table = "filter"; // if dont specify the table we want assume filter (same as iptables -L)
    // max rate is in the raw units - either bits per sec or packets per sec, dont waste time here
    // parsing units
    // default is standard ethernet 100mbit which is:
    // 100 * 1024 * 1024 - which gives in bits then / 8 to give bytes = 13107200
    max_rate="13107200";
    bg_colour = "black";
    // bar_colour can be either just a colour for a solid bar or a rate dependednt colour
    // so that high usage shows as different colours
    bar_colour = "green:30,yellow:50,red:100";
    for (i = 0; attr[i]; i += 2) { 
      assign(table,"table",attr, i); 
      assign(samples_per_sec, "samples_per_sec", attr, i); 
      assign(average_over, "average_over", attr, i);
      assign(measuring, "measuring", attr, i);
      assign(output_method, "output_method", attr, i);
      assign(title, "title", attr, i);
      assign(bg_colour, "bg_colour", attr, i);
      assign(bar_colour, "bar_colour", attr, i);
      assign(max_rate, "max_rate", attr, i);
      assign(output_format, "output_format", attr, i);


    }
  }
  else if(!strcmp(el, "entry")) {
    // first see if this is a multi line entry
    for (i = 0; attr[i]; i += 2)   
      if(assign(rule, "rule", attr, i))
	break;
    
    // so num of commas is num of extra elements
    crule = rule.c_str();
    // get first val
    max = atoi(crule);
    for(i = 0; i < rule.size(); i++) {
      if(rule[i] == ',') {
	commacount++;
	crule = rule.c_str();
	crule += i+1; // pointer move
	n = atoi(crule);
	if(n > max)
	  max = n; // how big do the collection buffers need to be
      }
    }
  

    
    // create an object here - first decide is it multi line or single line
    // if multi line how many buffers do we need
    // remember this is a pointer to current object 
    data_line *d = new data_line(this, commacount+1, (commacount ? max : 1), atoi(samples_per_sec.c_str()) * atoi(average_over.c_str()));
   
    d->rule = rule;
    // some defaults 
    d->table = this->table;
    d->bg_colour = this->bg_colour;
    d->bar_colour = this->bar_colour;
    d->max_rate = this->max_rate;
    d->output_format = this->output_format;

    for (i = 0; attr[i]; i += 2) {  
      // get the rest
      assign(d->name, "name", attr, i);
      assign(d->table, "table", attr, i);
      assign(d->chain, "chain", attr, i);
      assign(d->bg_colour, "bg_colour", attr, i);
      assign(d->bar_colour, "bar_colour", attr, i);
      assign(d->max_rate, "max_rate", attr, i);
      assign(d->output_format, "output_format", attr, i);

    } 
    if(!d->open_handle((measuring == "bytes" ? "raw_bcnt" : "raw_pcnt"))) {
      std::cerr << "ipt_vis failed to open /proc/iptableproc/" << d->table << "/" << d->chain << "/" << d->rule <<(measuring == "bytes" ? "/raw_bcnt" : "/raw_pcnt") <<  std::endl;
      return; // ignorring this one
    } 
    d->curses_start_line = next_screen_line;
    next_screen_line += commacount+1;
    
    lines.push_back(d); // this new object now on the vector
    
  }

}

// dont do anything for ends of entities atm.
void ipt_vis::end_hdlr(void *data, const XML_Char *el) { 

}

// parse the xml file to get the contents for this
ipt_vis::ipt_vis(const char *xml_file) {

  XML_Parser p;
  struct stat sbuf;
  char *buf = NULL;
  int rc,fd;
  samples_per_sec = "";
  average_over = "";
  measuring = "";
  lines.clear();
  working = false;
  output_method = "curses";
  bool name_is_config = false;

  p = XML_ParserCreate("UTF-8");
  if(p) {
    XML_SetUserData(p, (void *)this);
    XML_SetElementHandler(p, wrapper_start_hdlr, wrapper_end_hdlr);
  }
 
  if((rc = stat(xml_file, &sbuf)) == 0) {
    if(sbuf.st_size == 0) {
      std::cerr << "ipt_vis config file " << xml_file << " is empty! " << std::endl;
      return; // cant parse an empty file
    }
    buf = (char *)malloc(sbuf.st_size +1);
  }
  else {
    buf = (char *)malloc(strlen(xml_file) + 1);
    name_is_config = true;
  }
  if(buf != NULL) {
    buf[0] = 0;
    if(name_is_config) {
      strcpy(buf, xml_file);
    }
    else {
      if((fd = open(xml_file, O_RDONLY)) >= 0) {
	read(fd, buf, sbuf.st_size);
	close(fd);
      }
    }
    if (! XML_Parse(p, buf, strlen(buf), 0)) {
      std::cerr << "ipt_vis config " << xml_file << " has parse error at line " << XML_GetCurrentLineNumber(p) << ": " <<  XML_ErrorString(XML_GetErrorCode(p)) << std::endl;
    }
    else {
      working = true;
    }
    free(buf);
  }
  else {
    std::cerr << "ipt_vis config " << xml_file << " cannot allocate buffer space! " << std::endl;
    
  }
  // if we are not "curses" or "plain" take this as a file to use
  outfile = (FILE *)NULL;
  if(!(output_method == "curses" || output_method == "plain")) {
    if((outfile = fopen(output_method.c_str(), "w")) == NULL) {
      working = false;
    }
    else
      output_method = "plain";
  }
    
  
}

// We use select to listen to a number of file descriptors
bool ipt_vis::get_samples() {
  int i,numsamples, result;
  int sps = atoi(samples_per_sec.c_str());
  fd_set rfds;
  struct timeval tv;
  if(!working)
    return false;

  // how many samples do we have?
  numsamples = lines.size();
  FD_ZERO(&rfds);
  for(i = 0; i < numsamples; i++) {
    if(lines[i]->iptableproc_handle >= 0)
      FD_SET(lines[i]->iptableproc_handle, &rfds);
  }

  tv.tv_sec = 0;
  tv.tv_usec = 100; // wait just a tick or thereabouts
  // however we are at the mercy of select waking up when it feels like
  // current ver seems good for a few samples per sec though.
  result = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
  if(result == -1)
    return false;

  // process anything ready
  for(i = 0; i < numsamples; i++) {
    if(lines[i]->iptableproc_handle >= 0 && FD_ISSET(lines[i]->iptableproc_handle, &rfds))
      lines[i]->get_sample();
  }

  return true;
}


// given a specification for bar colour and a percentage that current speed is (0-100)
// returns the right colour to use as an int curses colour
static int percentage_to_colour(const char *bc, double percent) {

  char *c,*n,*comma;
  char nbuf[80]; // way big enough
  strncpy(nbuf, bc,sizeof(nbuf)-1); // just in case a really long name has been used
  nbuf[sizeof(nbuf)-1] = 0; // would never get that big but hey!
  c = nbuf;
  if(percent > 100.0) {
    percent = 100.0;
  }
  // bc is either just a colour in which case it us used regardless of percent or it is a number
  // of different colours and the max percent they activate to.
  // green:30,yellow:60,red:100
  if((comma =  strchr(c, ',')) != NULL) {
    do {
      // multiple colours - so find one that suits the percentage
      *comma = 0; // terminate this bit
      if((n = strchr(c,':')) != NULL) {
	*n++ = 0;
	// n now points at max percentage this colour applies to
	if(percent <= atoi(n)) {
	  return curses_colours[c];  // in ascending order so found it
	}
      }

      c = comma + 1;
    } while((comma =  strchr(c, ',')) != NULL);
  }
  else {
    // direct lookup 
    return curses_colours[c];
  }
}

// no nonsense just blast out the samples
// to the named file if there
int data_line::plain_render(FILE *outf) { 
  char *crule,*nextr;
  int rulenum;
  const char *fmt = "%.0f,%.0f";

  
  if(numlines == 1) {
    //only one rule so index 0
    crule = NULL;
    rulenum = 0;
  }
  else {
    crule =  (char *)rule.c_str();
    rulenum = atoi(crule) -1; // up to first ,
    if(rulenum >= numrules)
      return 0;
  }
  // special super super terse if samples_per_sec and average_over both 1
  // only show rate
  // super terse - dont even keep saying uints
  if(container->average_over == "1")
    fmt = "%.0f";

  if(output_format == "mbit" && container->measuring == "bytes")
    fprintf(outf, fmt, (rates[rulenum]*8)/(1024*1024), (averages[rulenum]*8)/(1024*1024));
  if(output_format == "kbit" && container->measuring == "bytes")
    fprintf(outf, fmt, (rates[rulenum]*8)/1024, (averages[rulenum]*8)/1024);
  else if(output_format == "bit" && container->measuring == "bytes")
    fprintf(outf,fmt, (rates[rulenum]*8), (averages[rulenum]*8));
  else
    fprintf(outf,fmt, rates[rulenum], averages[rulenum]);

  if(numlines > 1) {
    while((nextr = strchr(crule, ',')) != NULL) {
      crule = nextr + 1;
      if(*crule) {
	rulenum = atoi((const char *)crule) -1;
	// sanity check
	if(rulenum >= numrules)
	  break;
	fputs("\t", outf);
	if(output_format == "mbit" && container->measuring == "bytes") 
	  fprintf(outf,fmt, (rates[rulenum]*8)/(1024*1024), (averages[rulenum]*8)/(1024*1024));
	
	if(output_format == "kbit" && container->measuring == "bytes")  
	  fprintf(outf,fmt, (rates[rulenum]*8)/1024, (averages[rulenum]*8)/1024);
	else if(output_format == "bit" && container->measuring == "bytes")
	  fprintf(outf,fmt, (rates[rulenum]*8), (averages[rulenum]*8));
	else
	  fprintf(outf,fmt, rates[rulenum], averages[rulenum]);
      }
    }
  }
  fprintf(outf,"\t");
  return 0;
}

// render the line(s) for this data item - returning the new curses row num 
// for line after the ones we use
// limit is a speed to show as 100%
int data_line::curses_render() {
  int rval;
  std::string legend, shownrate;
  int i, barcolour, bgcolour;
  char nbuf[80]; // way big enough
  char ratebuf[80];
  // rules contains either a single rule number or a comma separated list of which rules we want to display
  int rulenum;
  char *crule,*nextr,*cname,*nextn;
  unsigned int linenum = curses_start_line+2; 
  double percentage = 0.0;
 
  if(numlines == 1) {
    //only one rule so index 0
    crule = NULL;
    rulenum = 0;
    cname = (char *)name.c_str();
  }
  else {
    crule =  (char *)rule.c_str();
    rulenum = atoi(crule) -1; // up to first ,	
    if(rulenum >= numrules)
      return linenum;
    strncpy(nbuf, name.c_str(), sizeof(nbuf)-1); // just in case a really long name has been used
    nbuf[sizeof(nbuf)-1] = 0; // would never get that big but hey!
    cname = nbuf;
    if((nextn =  strchr(cname, ',')) != NULL) {
      *nextn++ = 0; // null term first name
    }
  }

  // work out speed percentage
  percentage = (rates[rulenum]/atoi(max_rate.c_str())) * 100;
  // prepare ratebuf
  if(output_format == "mbit" && container->measuring == "bytes")
    sprintf(ratebuf,"%6.0f mbit %6.0f mbit", (rates[rulenum]*8)/(1024*1024), (averages[rulenum]*8)/(1024*1024));
  if(output_format == "kbit" && container->measuring == "bytes")
    sprintf(ratebuf,"%6.0f kbit %6.0f kbit", (rates[rulenum]*8)/1024, (averages[rulenum]*8)/1024);
  else if(output_format == "bit" && container->measuring == "bytes")
    sprintf(ratebuf,"%6.0f bit %6.0f bit", (rates[rulenum]*8), (averages[rulenum]*8));
  else
    sprintf(ratebuf,"%6.0f %6.0f", rates[rulenum], averages[rulenum]);
 
  // render first line
 
  render_line(linenum, cname, percentage, percentage_to_colour(bar_colour.c_str(), percentage),
	      curses_colours[bg_colour.c_str()], ratebuf);
  
  linenum++;
  if(numlines > 1) {
    // do we have more
    while((nextr = strchr(crule, ',')) != NULL) {
      crule = nextr + 1;
      if(*crule) {
	cname = nextn;
	if((nextn =  strchr(cname, ',')) != NULL) {
	  *nextn++ = 0; // null term next name
	} 
	rulenum = atoi((const char *)crule) -1;
	if(rulenum >= numrules)
	  break;
	percentage = (rates[rulenum]/atoi(max_rate.c_str())) * 100;
	// prepare ratebuf 
	if(output_format == "mbit" && container->measuring == "bytes")
	  sprintf(ratebuf,"%6.0f mbit %6.0f mbit", (rates[rulenum]*8)/(1024*1024), (averages[rulenum]*8)/(1024*1024));
	if(output_format == "kbit" && container->measuring == "bytes")
	  sprintf(ratebuf,"%6.0f kbit %6.0f kbit", (rates[rulenum]*8)/1024, (averages[rulenum]*8)/1024);
	else if(output_format == "bit" && container->measuring == "bytes")
	  sprintf(ratebuf,"%6.0f bit %6.0f bit", (rates[rulenum]*8), (averages[rulenum]*8));
	else
	  sprintf(ratebuf,"%6.0f %6.0f", rates[rulenum], averages[rulenum]);
	
	// render first line
	render_line(linenum, cname, percentage, percentage_to_colour(bar_colour.c_str(), percentage),
		    curses_colours[bg_colour.c_str()], ratebuf);	// render next line, options are all identical 


	linenum++;
      }
      else
	break;
    }
  }
  return linenum; // where we got to down the screen
}
  
void ipt_vis::render_samples(void) {
  int i;
  int linenum = 0;
  FILE *outf = (outfile ? outfile : stdout);
  //std::cout << *this << std::endl;
  if(output_method == "curses") {
    if(do_resize) {
      endwin();
      start_curses();
      refresh();           
      do_resize = 0;
      getmaxyx(stdscr,screen_max_rows,screen_max_cols); 
    }
    move(0,0); // get ready to draw
    if(title != "") {
      const char *c = title.c_str();
      mvprintw(0, (screen_max_cols-strlen(c))/2,c);
      move(1,0);
    }
	       
  }
 
  // position back at start
  if(outfile) {
    rewind(outf);
    // and lock
    lockf(fileno(outf), F_LOCK, 0);
  }
  // now render each line
  for(i = 0; i <  lines.size(); i++) {
    if(output_method == "curses") {
      linenum = lines[i]->curses_render();
    }
    else { 
      linenum = lines[i]->plain_render((outfile ? outfile : stdout));
    }

  } 
  if(output_method == "curses") { 
    max_rows = linenum; 
    move(max_rows,0);
    clrtobot(); // rest of the screen
    refresh();
  }
  else {
    fprintf(outf,"\n");
    fflush(outf);
    if(outfile) {
      // and unlock 
      rewind(outf);
      lockf(fileno(outf), F_ULOCK, 0);
    }
  }
}

void doit(const char *file) {
  ipt_vis workload(file);

  if(!workload.working)
    return; // cant do anything

  // print out what we have parsed
  // std::cout << workload << std::endl;
  if(workload.output_method == "curses") {
    start_curses(); // Only terminate via ^C etc. 
    getmaxyx(stdscr,screen_max_rows,screen_max_cols);
  }
  while(workload.get_samples()) {
    workload.render_samples(); 
    if(workload.output_method == "curses") {
      clrtobot(); // rest of the screen
      refresh(); 
      (void) handle_input();
    }
  }
}

// do the business with curses
void render_line (int linenum, const char *legend, double barpercent,
		 int bar_colour, int bg_colour,
		 const char *shownrate) {
    int startpos, graphcols, graphlimit;
    int barchar;
    int i;
    // are we below what is visible or above?
    
    // first return if not this many lines on the screen
    if(linenum < start_row || linenum > (start_row+(screen_max_rows-1))) {
      mvprintw(linenum, 0, "linenum=%d, < start_row = %d name=%s percent %f %s\n", linenum, start_row,legend, barpercent, shownrate);
        return; 
    }
    linenum -= start_row; // displaying this section on screen
    startpos = 0;
    graphcols = screen_max_cols - startpos;
    graphlimit = int(graphcols * (barpercent/100));

    // hack for mono terminals - translate bar colour
    if(bar_colour == COLOR_GREEN)
      barchar = ACS_HLINE; 
    if(bar_colour == COLOR_YELLOW)
      barchar = ACS_BOARD;
    if(bar_colour == COLOR_RED)
      barchar = ACS_BLOCK;
    mvprintw(linenum,0,"%s", legend);
    clrtoeol();
    if(!has_colors()) {
      // need the line of whatever character is appropriate
      for(i = 0; i < graphlimit;i++)
	addch(barchar);
    }
    // now the rate - put on top of the bar in the case of the no color version
    mvprintw(linenum, 35, "%d", graphlimit);
    mvprintw(linenum, 40 + ((screen_max_cols-startpos)/2-strlen((const char *)shownrate)),(const char *)shownrate);
    if(has_colors()) {
        // whole line background
        mvchgat(linenum,0,screen_max_cols,A_NORMAL,bg_colour,NULL);
        // show the graph part as a colour if possible
        mvchgat(linenum,startpos,graphlimit, A_NORMAL,bar_colour, NULL );
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

    if (has_colors())
    {
      // we hope so - it dont look very
        start_color();       
        // fg first then bg
        // white text on black
        init_pair(COLOR_BLACK, COLOR_WHITE,COLOR_BLACK);
	// black text on white
	init_pair(COLOR_WHITE, COLOR_BLACK,COLOR_WHITE);
        // white text on dark blue
        init_pair(COLOR_BLUE, COLOR_WHITE, COLOR_BLUE);
        init_pair(COLOR_MAGENTA, COLOR_BLACK, COLOR_MAGENTA);
        init_pair(COLOR_CYAN, COLOR_BLACK, COLOR_CYAN);

        // graph bar colours
        init_pair(COLOR_GREEN, COLOR_BLACK, COLOR_GREEN);       
        init_pair(COLOR_YELLOW, COLOR_BLACK, COLOR_YELLOW);
        init_pair(COLOR_RED, COLOR_BLACK, COLOR_RED);
        
    }
}

// these cant be in header
bool data_line::open_handle(std::string data) {
  std::string filename;
  std::string interval_filename;
  int samp_per_sec;
  FILE *fd;
  if(table == "" || chain == "")
    return false;
  if(!(data == "raw_bcnt" || data == "raw_pcnt"))
    return false;
  // default to 1 sample a sec if not a valid num - otherwise get div by 0 err!
  samp_per_sec = atoi(container->samples_per_sec.c_str());
  
  // this is the bit that may fail - open either the rule or whole chain
  if(numrules > 1) {
    // whole chain
    filename = std::string("/proc/iptableproc/") + table + "/" + chain + "/" + data;
    filename = std::string("/proc/iptableproc/") + table + "/" + chain + "/interval";
  }
  else {
    // just the rule
    if(rule == "")
      return false;
    
    filename =  std::string("/proc/iptableproc/") + table + "/" + chain + "/" + rule + "/" + data; 
    interval_filename =  std::string("/proc/iptableproc/") + table + "/" + chain + "/" + rule + "/interval";
  }
  // fopen so can use fprintf
  if((fd = fopen(interval_filename.c_str(), "w")) == NULL)
    return false;
  fprintf(fd,"%d\n", 1000/samp_per_sec);
  fclose(fd);
  if((iptableproc_handle = open(filename.c_str(), O_RDONLY)) < 0)
    return false;
  working = true;
  return working;
}

// called when there is sample data waiting to be read
void data_line::get_sample() {
  unsigned int i,rcnt, prev_cnt, next_cnt;
  ulong_t *cnt; 
  int samp_per_sec;
  char bigbuf[4096]; // Has to be exactly this size! Smaller and it screws up, bigger and it screws up
  if(iptableproc_handle < 0)
    return; // sanity
  // default to 1 sample a sec if not a valid num - otherwise get div by 0 err!
  samp_per_sec = atoi(container->samples_per_sec.c_str())|| 1;
  cnt = &(counters[cur_counter][0]);
  memset(cnt, 0, numrules * sizeof(ulong_t));
  rcnt = read(iptableproc_handle, bigbuf, sizeof(bigbuf)-1);
  if(rcnt <  numrules * sizeof(ulong_t)) {
    std::cerr << "ipt_vis short read " << rcnt << " expected " << numrules * sizeof(ulong_t) <<  std::endl;
    return;
  }
  memcpy(cnt, bigbuf,  numrules * sizeof(ulong_t));
  // std::cout << rcnt << " read cnt " << counters[cur_counter][0] << " cur_counter " << cur_counter << std::endl;
  
  // stats
  // instananeous stats is these less previous
  // over peroid stats is these less oldest
  
 
  if(cur_counter == 0) {
    prev_cnt = numsamples-1;
    next_cnt = cur_counter + 1;
  }
  else if(cur_counter == numsamples-1) {
    prev_cnt = cur_counter - 1;
    next_cnt = 0;
  }
  else {
    // normal
    prev_cnt = cur_counter - 1; 
    next_cnt = cur_counter + 1;
  }

  if(!gotsample) {
    // first ever sample - so set our starting value into our 'history' so we dont just jump from 0
    // also we can assume that cur_counter will only ever be 0 so just clone our readings upwards
    for(i = 1; i < numsamples; i++) {
      if(counters[i])
	memcpy(counters[i], counters[0],  numrules * sizeof(ulong_t));
    }
    avsamples = 1;

    gotsample = true; // so only happens once
  } 
  else {
    if(avsamples < numsamples)
      avsamples++;
  }
  // check that counter[prev_cnt] is non null before derefing it
  for(i = 0; i < numrules; i++) {
    if(counters[prev_cnt] && counters[cur_counter][i] >= counters[prev_cnt][i]) {
      rates[i] = (long)(counters[cur_counter][i] - counters[prev_cnt][i]);
      rates[i] *= samp_per_sec;
    }
    else 
      rates[i] = 0;


    // next is in fact the oldest we have so it is amount transferred over the period we are scanning
    if(counters[next_cnt] && counters[cur_counter][i] >= counters[next_cnt][i]) {
      averages[i] = (long)(counters[cur_counter][i] - counters[next_cnt][i]);
      averages[i] *= samp_per_sec;
      averages[i] /= avsamples;
    }
    
  }     
  if(cur_counter == numsamples-1)
    cur_counter = 0;
  else
    cur_counter++;
}

int  main(int argc, char *argv[]) {

  // initialise our idea of valid colours before XML parsing
  curses_colours["black"] = COLOR_BLACK;
  curses_colours["red"] = COLOR_RED;
  curses_colours["green"] = COLOR_GREEN;
  curses_colours["yellow"] = COLOR_YELLOW;
  curses_colours["blue"] = COLOR_BLUE;
  curses_colours["magenta"] = COLOR_MAGENTA;
  curses_colours["cyan"] = COLOR_CYAN;
  curses_colours["white"] = COLOR_WHITE;
      
  if(argc > 1) {
    doit(argv[1]);
  }
  else {
    std::cerr << "ipt_vis must have an xml config file or actual config as string as 1st parameter, no default action" << std::endl;
    exit(1);
  }
}
