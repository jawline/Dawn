#include <clock/rate.h>
#include <clock/pit.h>

void initializeSystemClock()
{
	initializePit(CLOCKS_PER_SECOND);
}
