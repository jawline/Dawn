#include <types/memory.h>
#include <common.h>
#include <printf.h>

#define BOARD_WIDTH 80
#define BOARD_HEIGHT 23

char Board[(BOARD_WIDTH * BOARD_HEIGHT) + 1];
char NextBoard[(BOARD_WIDTH * BOARD_HEIGHT) + 1];

void updateBlock(int i)
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

int cSleep = 0;

/**
 * @brief The entry point of a text-based remake of the game of life for Dawn
 * @params int Argc and void* Argv are given by the OS and corrospond to the equivilents on other operating systems
 * @return The result of the application
 * @callgraph
 */
int main(int argc, void* argv)
{
	setupBoard();
	Board[BOARD_WIDTH * BOARD_HEIGHT] = '\0';

	for (;;)
	{
		cls();
		printf("GAME OF LIFE: Remake\n");


		memcpy(NextBoard, Board, (BOARD_WIDTH * BOARD_HEIGHT) + 1);
		int i = 0;
		for (i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
		{
			updateBlock(i);
		}

		memcpy(Board, NextBoard, (BOARD_WIDTH * BOARD_HEIGHT) + 1);

		for (i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
		{
			printf("%c", Board[i]);
		}

		cSleep++;

		sleepUnprecise(250);
	}
	return 1;
}

void _start(int argc, void* argv)
{
	int result = main(argc, argv);
	exit(result);
}
