/*
 * beep2 version 1.2a
 * Original Copyright (C) 1997 Josef Pavlik <jetset@ibm.net>
 * Copyright (C) 2002 YAMAGUCHI Shingo <shingo@kip.iis.toyama-u.ac.jp>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "beep.h"

param_t get_parameter_from_sequence_source(char *instr);
param_t get_parameter_from_freqency_source(char *instr);
void output_beep_sound(param_t *beepbuf);
void output_usage();
void output_version();
#ifdef SYSTEM_LOGGING
void write_log(int argc, char **argv);
#endif

int main (int argc, char **argv) {
  FILE    *in            = NULL;
  int      i;
  int      arg;
  int      line;
  int      parsedline;
  param_t  parameter;
  param_t  beepbuf[BUFFER];
  char     instr[BUFFER] = "";

  debug = OFF;
  source = SOURCE_FREQ;
  tempo = 100;

#ifdef SYSTEM_LOGGING
  write_log(argc, argv);
#endif

  while ((arg = getopt(argc, argv, "dfst:v")) != -1) {
    switch (arg) {
    case 'd':
      debug = ON;
      break;
    case 'f':
      source = SOURCE_FREQ;
      break;
    case 's':
      source = SOURCE_SEQUENCE;
      break;
    case 't':
      if (isdigit(*optarg)) {
	tempo = atoi(optarg);
      } else {
	output_usage();
	exit(1);
      }
      break;
    case 'v':
      output_version();
      exit(0);
      break;
    case ':':
    case '?':
      output_usage();
      exit(1);
      break;
    default:
      break;
    }
  }

  if (debug == ON) {
    fprintf(stderr, "beep2: optind=%d\n", optind);
    fprintf(stderr, "beep2: source=%d\n", source);
    fprintf(stderr, "beep2: tempo=%d%%\n", tempo);
    fprintf(stderr, "beep2: argc=%d\n", argc);
    for (i = 0; i <= argc; i++) {
      fprintf(stderr, "beep2: argv[%d]=%s\n", i, argv[i]);
    }
  }

  if (optind == argc) {
    in = stdin;
  } else if (optind == argc - 1) {
    in = fopen(argv[optind], "r");
    if (!in) {
      fprintf(stderr,"beep2: Can't open file %s.\n", argv[optind]);
      output_usage();
      exit(1);
    }
  } else {
    output_usage();
    exit(1);
  }

  line = 0;
  parsedline = 0;

  if (debug == ON) {
    fprintf(stderr, "beep2: Parsing");
  }

  while (1) {
    *instr = '\0';
    fgets(instr, sizeof(instr), in);
    if ((*instr == '\n' || *instr == '\0') && feof(in)) {
      break;
    }

    if (source == SOURCE_SEQUENCE) {
      parameter = get_parameter_from_sequence_source(instr);
    } else {
      parameter = get_parameter_from_freqency_source(instr);
    }

    if (parameter.status == PARSE_OK) {
      beepbuf[parsedline] = parameter;
      parsedline++;
    } else if (parameter.status == PARSE_ERROR) {
      fprintf(stderr, "beep2: Parse error at line %d\n", line+1);
    }
    parameter.status = PARSE_COMMENT;
    parameter.freq   = 0;
    parameter.length = 0;

    if (debug == ON) {
      if (line % 10 == 0) {
	fprintf(stderr, ".");
      }
    }
    line++;
  }
  fclose(in);

  beepbuf[parsedline].status = PARSE_TERMINATE;

  if (debug == ON) {
    fprintf(stderr, "done.\n");
  }

  output_beep_sound(beepbuf);

  exit(0);
}
