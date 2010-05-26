#include "timestamp.h"
#include <time/cmos_time.h>

timestamp ts_from_cmos()
{
	cmos_time_struct tstruct;
	fill_cmos_tstruct(&tstruct);

}
