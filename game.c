#include "tetris.h"

int main()
{
#pragma warining (disable:4996)
	max = 0, grade = 0;
	system("title Tetris");
	system("mode con lines=29 cols=60");
	HideCursor();
	ReadGrade();
	InitInterface();
	InitBlockInfo();
	srand((unsigned int)time(NULL));
	StartGame();
	return 0;
}