# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = gawk
VERSION = 3.1.5

BASE_URL = ftp://ftp.gnu.org/gnu/gawk

ifeq ($(BUILD_PHASE), Stage_3)
CONFIG_OPTS = --prefix=/tools ac_cv_func_working_mktime=yes
else
CONFIG_OPTS = ac_cv_func_working_mktime=yes
endif


include ../Makefile.rules
