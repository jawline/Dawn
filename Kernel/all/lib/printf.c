#include <types/stdint.h>
#include <printf.h>
#include <va_list.h>

void printf(const char * fmt, ...) {

	static char buf[2096]; //Maximum size to be printed is 1024 chars.
	va_list args;

	int i;

	va_start(args, fmt);

	i = vsprintf(buf, fmt, args);

	va_end(args);

	buf[i] = '\0';

	prints(buf);
}

void printFormattedStringToBuffer(char* buffer, const char * fmt, ...) {

	va_list args;

	int i;

	va_start(args, fmt);

	i = vsprintf(buffer, fmt, args);

	va_end(args);

	buffer[i] = '\0';
}
