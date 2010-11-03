#include <stdio.h>
#include "debug.h"

#if defined(DEBUG_MODE)
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT
#endif
