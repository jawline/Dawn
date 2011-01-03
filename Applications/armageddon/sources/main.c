#include <printf.h>
#include <process/end_process.h>
#include <clock/clock.h>
#include <process/sleep.h>

extern "C" {

	int _start(int argc, void* argv)
	{
		printf("Self-Destruct sequence activated\n");

		unsigned int tdeath = 16;

		while (1)
		{
			unsigned long time_for_splode = clock() + getClocksPerSecond();
			while (time_for_splode > clock()) {}
			tdeath--;

			printf("%i Seconds remaining\n", tdeath);

			if (tdeath == 0)
				break;
		}

		printf("Hah, just kidding\n");

		exit(0);
	}

}
