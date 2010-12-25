#ifndef VSPRINTF_H
#define VSPRINTF_H
#include "va_list.h"
#include <common.h>

int vsprintf(char *buf, const char *fmt, va_list args);

#endif
