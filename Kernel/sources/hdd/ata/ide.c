#include "ide.h"
#include <stdio.h>

void initializeIdeHardDrive(unsigned long primary_io, unsigned long primary_ctrl, unsigned long secondary_io, unsigned long secondary_ctrl)
{
	printf("Initializing hard drive 0x%x 0x%x 0x%x 0x%x\n", primary_io, primary_ctrl, secondary_io, secondary_ctrl);
}
