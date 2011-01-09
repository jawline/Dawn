#include <debug/debugprint.h>

void printDebugMessage(char* fmt, ...)
{
	if (getDebugMode() == 1)
	{
		static char buf [2096]; //Maximum size to be printed is 1024 chars.
		va_list args;

		int i;
	 
		va_start(args, fmt);

		i = vsprintf(buf,fmt,args);

		va_end(args);

		buf[i] = '\0';

		prints(buf);
	}
}
