#include <types/memory.h>
#include <common.h>
#include <printf.h>
#include <messages/messages.h>
#include <input/devices.h>
#include <process/message.h>
#include <process/events.h>
#include <process/process_info.h>
#include <process/postbox_api.h>
#include <process/end_process.h>
#include <process/sleep.h>
#include <scancodes.h>


#define BOARD_WIDTH 80
#define BOARD_HEIGHT 23

#define NUM_SECONDS_TOTAL 15
#define NUM_STEPS_SECOND 4
#define NUM_STEPS_TOTAL NUM_SECONDS_TOTAL * NUM_STEPS_SECOND

char Board[(BOARD_WIDTH * BOARD_HEIGHT) + 1];
char NextBoard[(BOARD_WIDTH * BOARD_HEIGHT) + 1];

uint8_t running = 0;

unsigned int cursor_tile = 0;

void updateBlock(int i)
{
	if (running)
	{

		int numberNearMe = 0;

		if (Board[i + 1] == '#')
		{
			numberNearMe++;
		}

		if (Board[i - 1] == '#')
		{
			numberNearMe++;
		}

		if (i - BOARD_WIDTH > 0)
		{
			if (Board[i - BOARD_WIDTH] == '#')
			{
				numberNearMe++;
			}

			if (Board[i - BOARD_WIDTH - 1] == '#')
			{
				numberNearMe++;
			}

			if (Board[i - BOARD_WIDTH + 1] == '#')
			{
				numberNearMe++;
			}
		}

		if (i + BOARD_WIDTH < BOARD_WIDTH * BOARD_HEIGHT)
		{
			if (Board[i + BOARD_WIDTH] == '#')
			{
				numberNearMe++;
			}

			if (Board[i + BOARD_WIDTH + 1] == '#')
			{
				numberNearMe++;
			}

			if (Board[i + BOARD_WIDTH - 1] == '#')
			{
				numberNearMe++;
			}
		}

		if (Board[i] == ' ')
		{
			if (numberNearMe == 3)
			{
				NextBoard[i] = '#';
			}
		}
		else
		{
			if (numberNearMe < 2)
			{
				NextBoard[i] = ' '; //KILL MEE
			}
			if (numberNearMe > 3)
			{
				NextBoard[i] = ' ';
			}
		}
	}

}

void setupBoard()
{
	int i = 0;

	for (i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
	{

		Board[i] = ' ';

	}

	Board[23] = '#';
	Board[24] = '#';
	Board[103] = '#';
	Board[186] = '#';
	Board[186 + BOARD_WIDTH] = '#';
	Board[186 + BOARD_WIDTH - 1] = '#';

	Board[53] = '#';
	Board[132] = '#';
	Board[133] = '#';
	Board[134] = '#';
	Board[213] = '#';
}

void redraw()
{
	cls();
	printf("GAME OF LIFE: State ");

	if (running)
		printf("running");
	else
		printf("not running");
	printf("\n");

	unsigned int i = 0;
	for (i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
	{

		if (!running)
		{

			if (i == cursor_tile)
				printf("@");
			else
				printf("%c", Board[i]);

		}
		else
		{
			printf("%c", Board[i]);
		}
	}

}

void updateLogic()
{
	memcpy(NextBoard, Board, (BOARD_WIDTH * BOARD_HEIGHT) + 1);

	unsigned int i = 0;

	for (i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
	{
		updateBlock(i);
	}

	memcpy(Board, NextBoard, (BOARD_WIDTH * BOARD_HEIGHT) + 1);
}

int cSleep = 0;

/**
 * @brief The entry point of a text-based remake of the game of life for Dawn
 * @params int Argc and void* Argv are given by the OS and corrospond to the equivilents on other operating systems
 * @return The result of the application
 * @callgraph
 */

int main(int argc, void* argv)
{
	postboxSetFlags(INPUT_BIT);

	cursor_tile = 0;

	running = 0;

	setupBoard();
	Board[BOARD_WIDTH * BOARD_HEIGHT] = '\0';
	redraw();

	for (;;)
	{

		if (postboxHasNext() == 1)
		{
			process_message message;
			message = postboxGetNext();

			if (message.ID == INPUT_MESSAGE)
			{
				//Its a input message alright
				if (message.message_data[0] == DEVICE_KEYBOARD)
				{
					//its a keyboard message even!
					char C = getAsciFromScancode(message.message_data[1], message.message_data[2]);

					if (C == 'e')
					{
						exit(0);
					}
					else if (C == 'w')
					{
						if (cursor_tile > BOARD_WIDTH)
						{
							cursor_tile -= BOARD_WIDTH;
						}
					}
					else if (C == 's')
					{
						if ((cursor_tile + BOARD_WIDTH) < (BOARD_WIDTH * BOARD_HEIGHT))
						{
							cursor_tile += BOARD_WIDTH;
						}
					}
					else if (C == 'a')
					{
						if (cursor_tile > 0)
						{
							cursor_tile -= 1;
						}
					}
					else if (C == 'd')
					{
						if (cursor_tile < (BOARD_WIDTH * BOARD_HEIGHT) - 1)
						{
							cursor_tile += 1;
						}
					}
					else if (C == 'r')
					{
						if (running) running = 0;
						else
							running = 1;
					}
					else if (C == 'f')
					{
						if (Board[cursor_tile] == ' ')
						{
							Board[cursor_tile] = '#';
						}
						else
						{
							Board[cursor_tile] = ' ';
						}
					}
					redraw();
				}
			}
		}


		cSleep++;

		if (running)
		{
			updateLogic();
			redraw();
			sleepUnprecise(125);
		}

	}

	return 1;
}

extern "C" {

	void _start(int argc, void* argv)
	{
		int result = main(argc, argv);
		exit(result);
	}

}
