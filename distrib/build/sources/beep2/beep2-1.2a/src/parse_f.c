/*
 * beep2 version 1.2a
 * Original Copyright (C) 1997 Josef Pavlik <jetset@ibm.net>
 * Copyright (C) 2002 YAMAGUCHI Shingo <shingo@kip.iis.toyama-u.ac.jp>
 */

#include <stdio.h>
#include <ctype.h>

#include "beep.h"

param_t get_parameter_from_freqency_source(char *instr)
{
  int     num;
  int     status;
  int     value;
  param_t parameter;

  parameter.status = PARSE_COMMENT;
  parameter.freq   = 0;
  parameter.length = 0;

  if (instr[0] != '\n' && instr[0] != '#') {
    status = NOW_START;
    value = 0;
    for (num = 0; instr[num] != '\0'; num++) {
      if (isdigit(instr[num])) {
	switch (status) {
	case NOW_START:
	case NOW_FREQ:
	  status = NOW_FREQ;
	  value *= 10;
	  value += instr[num] - '0';
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
      } else if (instr[num] == ' ' || instr[num] == '\t') {
	switch (status) {
	case NOW_FREQ:
	  status = NOW_BLANK;
	  parameter.freq = value;
	  value = 0;
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
	case NOW_START:
	  status = NOW_END;
	  parameter.status = PARSE_COMMENT;
	  break;
	case NOW_FREQ:
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
