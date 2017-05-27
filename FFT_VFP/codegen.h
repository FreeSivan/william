#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include <limits.h>

#include "ffts.h"

void ffts_generate_func_code(ffts_plan_t *, size_t N, size_t leafN, int sign); 

#endif
