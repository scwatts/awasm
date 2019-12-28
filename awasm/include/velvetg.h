#ifndef __VELVETG_H__
#define __VELVETG_H__


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "velvet/src/run.h"

#include "velvet/src/binarySequences.h"
#include "velvet/src/globals.h"

// Defined using compiler flags
#define MAXKMERLENGTH 31
#define CATEGORIES 2


void velvetg(char *prefix);


#endif
