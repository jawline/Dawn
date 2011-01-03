#include <printf.h>
#include <process/end_process.h>

#include <clock/clock.h>

extern "C" {

	int _start(int argc, void* argv)
	{
		unsigned long ticks = clock();
		unsigned long ticks_per_second = getClocksPerSecond();

		unsigned int seconds = 0;
		unsigned int minutes = 0;
		unsigned int hours = 0;

		seconds = ticks / ticks_per_second; //Use this temporarely

		hours = (seconds / (60 * 60));
		
		unsigned int remainder = seconds % (60 * 60);

		minutes = remainder / 60;

		seconds = remainder % 60;

		printf("The system has been alive for %i hours %i minutes and %i seconds\n", hours, minutes, seconds);
		exit(0);
	}

}
