#include <stdio.h>
#include "debug.h"

#if defined(DEBUG_MODE)
#define DEBUG_PRINT(x) printf(x)
#else
#define DEBUG_PRINT(x)
#endif

#if defined(DEBUG_MODE)
#define DEBUG_PRINTX(v) printf("%x", v)
#else
#define DEBUG_PRINTX(v)
#endif

#if defined(DEBUG_MODE)
#define DEBUG_PRINTI(v) printf("%i", v)
#else
#define DEBUG_PRINTI(v)
#endif
