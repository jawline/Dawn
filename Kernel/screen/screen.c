#include <screen/screen.h>

static unsigned int g_screenMode;

static unsigned int g_screenWidth;
static unsigned int g_screenHeight;

void init_screen()
{
	g_screenMode = MODE_TEXTMODE;

	g_screenWidth = 80;
	g_screenHeight = 25;

	text_mode_clearscreen();
}

unsigned int get_mode()
{
	return g_screenMode;
}

unsigned int get_attr(unsigned int ID)
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
