/*
 * beep2 version 1.2a
 * Original Copyright (C) 1997 Josef Pavlik <jetset@ibm.net>
 * Copyright (C) 2002 YAMAGUCHI Shingo <shingo@kip.iis.toyama-u.ac.jp>
 */

#include "../config.h"

#define BUFFER 65535

#define ON 1
#define OFF 0

#define SOURCE_FREQ 1
#define SOURCE_SEQUENCE 2

#define PARSE_OK 1
#define PARSE_ERROR 2
#define PARSE_COMMENT 4
#define PARSE_TERMINATE 8

#define NOW_START 1
#define NOW_BLANK 2
#define NOW_FREQ 4
#define NOW_REST 8
#define NOW_SEQCHR 16
#define NOW_SEQNUM 32
#define NOW_SEQSHP 64
#define NOW_LENG 128
#define NOW_END 256

typedef struct param {
  int status;    /* parse line status */
  int freq;      /* frequency */
  int length;    /* length(milliseconds) */
} param_t;

int      debug;
int      source;
int	 tempo;
