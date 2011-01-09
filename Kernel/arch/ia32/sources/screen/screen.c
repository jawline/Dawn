#include <screen/screen.h>

static unsigned int g_screenMode;

static unsigned int g_screenWidth;
static unsigned int g_screenHeight;

/**
 * @brief Initialize the screen if one exists
 * @callgraph
 */
void initializeScreen()
{
	//The screen starts in text mode (The BIOS & GRUB should see to that)
	g_screenMode = MODE_TEXTMODE;

	//Default/Standard = 80x25 text mode on boot - //TODO: Detect/Set the screen mode prior to setting these values
	g_screenWidth = 80;
	g_screenHeight = 25;

	//Clear any rubbish left on the screen by the bootloader/BIOS/Anything that came before the kernel
	text_mode_clearscreen();
}

//Get the screen mode
unsigned int getScreenMode()
{
	return g_screenMode;
}

//Get a screen attribute (Width, height, Mode, etc)
unsigned int getScreenAttribute(unsigned int ID)
{
	switch (ID)
	{
		case SCREEN_MODE:
		{
			return g_screenMode;
		}

		case SCREEN_WIDTH:
		{
			return g_screenWidth;
		}

		case SCREEN_HEIGHT:
		{
			return g_screenHeight;
		}
		
		default:
			return 0;
	}
}
