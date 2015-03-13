#include <debug/debugprint.h>

void printDebugMessage(char* fmt, ...) {
	if (getDebugMode()) {
		static char buf [1024*4]; //Maximum size to be printed is 4kb.
		va_list args;
		va_start(args, fmt);
		int i = vsprintf(buf,fmt,args);
		va_end(args);
		buf[i] = '\0';
		prints(buf);
	}
}
