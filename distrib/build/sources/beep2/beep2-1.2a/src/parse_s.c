/*
 * beep2 version 1.2a
 * Original Copyright (C) 1997 Josef Pavlik <jetset@ibm.net>
 * Copyright (C) 2002 YAMAGUCHI Shingo <shingo@kip.iis.toyama-u.ac.jp>
 */

#include <stdio.h>
#include <ctype.h>

#include "beep.h"

int get_frequency_from_sequence(char *scale);

param_t get_parameter_from_sequence_source(char *instr)
{
  int     num;
  int     status;
  int     value;
  char    scale[3];
  param_t parameter;

  parameter.status = PARSE_COMMENT;
  parameter.freq   = 0;
  parameter.length = 0;

  if (instr[0] != '\n' && instr[0] != '#') {
    status = NOW_START;
    value = 0;
    for (num = 0; instr[num] != '\0'; num++) {
      if (instr[num] == '-') {
	  switch (status) {
	  case NOW_START:
	    status = NOW_REST;
	    break;
	  case NOW_SEQCHR:
	  case NOW_SEQNUM:
	  case NOW_SEQSHP:
	  case NOW_BLANK:
	    status = NOW_END;
	    parameter.status = PARSE_ERROR;
	    break;
	  default:
	    break;
	  }
      } else if (instr[num] == 'A' ||
		 instr[num] == 'B' ||
		 instr[num] == 'C' ||
		 instr[num] == 'D' ||
		 instr[num] == 'E' ||
		 instr[num] == 'F' ||
		 instr[num] == 'G') {
	switch (status) {
	case NOW_START:
	  status = NOW_SEQCHR;
	  sprintf(scale, "%c", instr[num]);
	  break;
	case NOW_SEQNUM:
	case NOW_SEQSHP:
	case NOW_BLANK:
	  status = NOW_END;
	  parameter.status = PARSE_ERROR;
	  break;
	default:
	  break;
	}
      } else if (isdigit(instr[num])) {
	switch (status) {
	case NOW_START:
	case NOW_SEQNUM:
	  status = NOW_END;
	  parameter.status = PARSE_ERROR;
	  break;
	case NOW_SEQCHR:
	  status = NOW_SEQNUM;
	  sprintf(scale, "%s%c", scale, instr[num]);
	  break;
	case NOW_BLANK:
	case NOW_LENG:
	  status = NOW_LENG;
	  value *= 10;
	  value += instr[num] - '0';
	  break;
	default:
	  break;
	}
      } else if (instr[num] == '#') {
	switch (status) {
	case NOW_START:
	  status = NOW_END;
	  break;
	case NOW_SEQCHR:
	case NOW_SEQSHP:
	case NOW_LENG:
	  status = NOW_END;
	  parameter.status = PARSE_ERROR;
	  break;
	case NOW_SEQNUM:
	  status = NOW_SEQSHP;
	  sprintf(scale, "%s%c", scale, instr[num]);
	  break;
	default:
	  break;
	}
      } else if (instr[num] == ' ' || instr[num] == '\t') {
	switch (status) {
	case NOW_SEQCHR:
	  status = NOW_END;
	  parameter.status = PARSE_ERROR;
	  break;
	case NOW_SEQNUM:
	case NOW_SEQSHP:
	  status = NOW_BLANK;
	  parameter.freq = get_frequency_from_sequence(scale);
	  break;
	case NOW_REST:
	  status = NOW_BLANK;
	  parameter.freq = 0;
	  break;
	case NOW_LENG:
	  status = NOW_END;
	  parameter.status = PARSE_OK;
	  parameter.length = value * (double)(100.0 / tempo);
	  value = 0;
	  break;
	default:
	  break;
	}
      } else if (instr[num] == '\n') {
	switch (status) {
	case NOW_SEQCHR:
	case NOW_SEQNUM:
	case NOW_SEQSHP:
	case NOW_REST:
	case NOW_BLANK:
	  status = NOW_END;
	  parameter.status = PARSE_ERROR;
	  break;
	case NOW_LENG:
	  status = NOW_END;
	  parameter.status = PARSE_OK;
	  parameter.length = value * (double)(100.0 / tempo);
	  value = 0;
	  break;
	default:
	  break;
	}
      }
    }
  }
  return parameter;
}
