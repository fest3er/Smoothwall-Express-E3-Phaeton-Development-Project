/*
 * beep2 version 1.2a
 * Original Copyright (C) 1997 Josef Pavlik <jetset@ibm.net>
 * Copyright (C) 2002 YAMAGUCHI Shingo <shingo@kip.iis.toyama-u.ac.jp>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "beep.h"

void output_beep_sound(param_t *beepbuf)
{
  FILE *dest;
  int parsedline;

  dest = fopen(OUTPUT_DEVICE, "w");
  if (!dest) {
    fprintf(stderr, "beep2: Can't open OUTPUT_DEVICE\n");
    exit(1);
  }

  for (parsedline = 0;
       beepbuf[parsedline].status != PARSE_TERMINATE;
       parsedline++) {
#ifdef OSTYPE_BSD
    int pitch;
    int dulation;
    if (beepbuf[parsedline].freq != 0 ) {
      pitch = 1193182 / beepbuf[parsedline].freq;
    } else {
      pitch = 0;
    }
    dulation = beepbuf[parsedline].length / 10;

    if (debug == ON) {
      fprintf(stderr,
	      "beep2: pitch: %d dulation: %d\n",
	      pitch,
	      dulation);
    }

    fprintf(dest, "\033[=%d;%dB\a\033[=800;1B",
	    pitch, dulation);
#else
    if (debug == ON) {
      fprintf(stderr,
	      "beep2: freq: %d, length: %d\n",
	      beepbuf[parsedline].freq,
	      beepbuf[parsedline].length);
    }
    fprintf(dest,"\033[10;%d]\033[11;%d]\a\033[10]\033[11]",
	    beepbuf[parsedline].freq, beepbuf[parsedline].length);
#endif OSTYPE_BSD
    fflush(dest);
    
    if (beepbuf[parsedline].length) {
      usleep(beepbuf[parsedline].length*1000L);
    }
  }
  fclose(dest);
}
