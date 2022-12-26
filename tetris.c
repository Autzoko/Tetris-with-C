#include "tetris.h"

void HideCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	curInfo.dwSize = 1;
	curInfo.bVisible = FALSE;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &curInfo);
}

void CursorJump(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
}

void InitInterface()
{
	color(7);			//Color set: white
	for(int i = 0; i < ROW; i++)
	{
		for(int j = 0; j < COL + 1; j++)
		{
			if(j == 0 || j == COL - 1 || j == COL + 9)
			{
				face.data[i][j] = 1;	//Mark block present in this coord
				CursorJump(2 * j, i);
				printf("■");
			}
			else if(i == ROW - 1)
			{
				face.data[i][j] = 1;
				printf("■");
			}
			else
			{
				face.data[i][j] = 0; 	//Mark block not present in this coord
			}
		}
	}
	for(int i = COL; i < COL + 10; i++)
	{
		face.data[8][i] = 1;
		CursorJump(2 * i, 8);
		printf("■");
	}

	CursorJump(2 * COL, 1);
	printf("Next Block: ");

	CursorJump(2 * COL + 4, ROW - 19);
	printf("Left shift: ←");

	CursorJump(2 * COL + 4, ROW - 17);
	printf("Right shift: →");

	CursorJump(2 * COL + 4, ROW - 15);
	printf("Accelarate: ↓");

	CursorJump(2 * COL + 4, ROW - 13);
	printf("Spin: Space");

	CursorJump(2 * COL + 4, ROW - 11);
	printf("Exit: ESC");

	CursorJump(2 * COL + 4, ROW - 9);
	printf("Pause: S");

	CursorJump(2 * COL + 4, ROW - 7);
	printf("Restart: R");

	CursorJump(2 * COL + 4, ROW - 5);
	printf("Highest record: %d", max);

	CursorJump(2 * COL + 4, ROW - 3);
	printf("Score: %d", grade);
}

void InitBlockInfo()
{
	// T
	for(int i = 0; i < 3; i++)
		block[0][0].space[1][i] = 1;
	block[0][0].space[2][1] = 1;

	// L
	for(int i = 1; i <= 3; i++)
		block[1][0].space[i][1] = 1;
	block[1][0].space[3][2] = 1;

	// J
	for(int i = 1; i <= 3; i++)
		block[2][0].space[i][2] = 1;
	block[2][0].space[3][1] = 1;

	for(int i = 0; i <= 1; i++)
	{
		// Z
		block[3][0].space[1][i] = 1;
		block[3][0].space[2][i + 1] = 1;

		// S
		block[4][0].space[1][i + 1] = 1;
		block[4][0].space[2][i] = 1;

		// o
		block[5][0].space[1][i + 1] = 1;
		block[5][0].space[2][i + 1] = 1;
	}

	// I
	for(int i = 0; i <= 3; i++)
		block[6][0].space[i][1] = 1;
	
	int temp[4][4];
	for(int shape = 0; shape < 7; shape++)
	{
		for(int form = 0; form < 3; form++)
		{
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					temp[i][j] = block[shape][form].space[i][j];
				}
			}
			// Spin
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					block[shape][form + 1].space[i][j] = temp[3 - j][i];
				}
			}
		}
	}
}

void color(int c)
{
	switch(c)
	{
		case 0:
			c = 13;
			break;
		case 1:
		case 2:
			c = 12;
			break;
		case 3:
		case 4:
			c = 10;
			break;
		case 5:
			c = 14;
			break;
		case 6:
			c = 11;
			break;
		default:
			c = 7;
			break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void DrawBlock(int shape, int form, int x, int y)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(block[shape][form].space[i][j] == 1)
			{
				CursorJump(2 * (x + j), y + i);
				printf("■");
			}
		}
	}
}

void DrawSpace(int shape, int form, int x, int y)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(block[shape][form].space[i][j] == 1)
			{
				CursorJump(2 * (x + j), y + i);
				printf("  ");
			}
		}
	}
}

int IsLegal(int shape, int form, int x, int y)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if((block[shape][form].space[i][j] == 1) && (face.data[y + i][x + j] == 1))
			{
				return 0;
			}
		}
	}
	return 1;
}

int JudgeFunc()
{
	for(int i = ROW - 2; i > 4; i--)
	{
		int sum = 0;
		for(int j = 1; i < COL - 1; j++)
		{
			sum += face.data[i][j];
		}
		if(sum == 0)
		{
			break;
		}
		if(sum == COL - 2)
		{
			grade += 10;
			color(7);
			CursorJump(2 * COL + 4, ROW - 3);
			printf("Score: %d", grade);
			for(int j = 1; j < COL - 1; j++)
			{
				face.data[i][j] = 0;
				CursorJump(2 * j, i);
				printf("  ");
			}
			for(int m = i; m > 1; m--)
			{
				for(int n = 1; n < COL - 1; n++)
				{
					sum += face.data[m - 1][n];
					face.data[m][n] = face.data[m - 1][n];
					face.color[m][n] = face.color[m - 1][n];
					if(face.data[m][n] == 1)
					{
						CursorJump(2 * n, m);
						color(face.color[m][n]);
						printf("■");
					}
					else
					{
						CursorJump(2 * n, m);
						printf("  ");
					}
					if(sum == 0)
					{
						return 1;
					}
				}
			}
		}
	}
	for(int j = 1; j < COL - 1; j++)
	{
		if(face.data[1][j] == 1)
		{
			Sleep(1000);
			system("cls");
			color(7);
			CursorJump(2 * (COL / 3), ROW / 2 - 3);
			if(grade > max)
			{
				printf("Highest record updated: %d", grade);
				WriteGrade();
			}
			else if(grade == max)
			{
				printf("Achieve highest record: %d", grade);
			}
			else
			{
				printf("Game Over, Score: %d", grade);
			}
			CursorJump(2 * (COL / 3), ROW / 2);
			printf("GAME OVER");
			while(1)
			{
				char ch;
				CursorJump(2 * (COL / 3), ROW / 2 + 3);
				printf("Restart? (y/n):");
				scanf("%c", &ch);
				if(ch == 'y' || ch == 'Y')
				{
					system("cls");
					main();
				}
				else if(ch == 'n' || ch == 'N')
				{
					CursorJump(2 * (COL / 3), ROW / 2 + 5);
					exit(0);
				}
				else
				{
					CursorJump(2 * (COL / 3), ROW / 2 + 4);
					printf("Error");
				}
			}
		}
	}
	return 0;
}

void StartGame()
{
	int shape = rand() % 7, form = rand() % 4;
	while(1)
	{
		int t = 0;
		int nextShape = rand() % 7, nextForm = rand() % 4;
		int x = COL / 2 -2, y = 0;
		color(nextShape);
		DrawBlock(nextShape, nextForm, COL + 3, 3);
		while(1)
		{
			color(shape);
			DrawBlock(shape, form, x, y);
			if(t == 0)
			{
				t = 15000;
			}
			while(--t)
			{
				if(kbhit() != 0)
					break;
			}
			if(t == 0)
			{
				if(IsLegal(shape, form, x, y + 1) == 0)
				{
					for(int i = 0; i < 4; i++)
					{
						for(int j = 0; j < 4; j++)
						{
							if(block[shape][form].space[i][j] == 1)
							{
								face.data[y + i][x + j] = 1;
								face.color[y + i][x + j] = shape;
							}
						}
					}
					while(JudgeFunc());
					break;
				}
				else
				{
					DrawSpace(shape, form, x, y);
					y++;
				}
			}
			else
			{
				char ch = getch();
				switch(ch)
				{
					case DOWN:
						if(IsLegal(shape, form, x, y + 1) == 1)
						{
							DrawSpace(shape, form, x, y);
							y++;
						}
						break;
					case LEFT:
						if(IsLegal(shape, form, x - 1, y) == 1)
						{
							DrawSpace(shape, form, x, y);
							x--;
						}
						break;
					case RIGHT:
						if(IsLegal(shape, form, x + 1, y) == 1)
						{
							DrawSpace(shape, form, x, y);
							x++;
						}
						break;
					case SPACE:
						if(IsLegal(shape, (form + 1) % 4, x, y + 1) == 1)
						{
							DrawSpace(shape, form, x, y);
							y++;
							form = (form + 1) % 4;
						}
						break;
					case ESC:
						system("cls");
						color(7);
						CursorJump(COL, ROW / 2);
						printf(" GAME OVER ");
						CursorJump(COL, ROW / 2 + 2);
						exit(0);
					case 's':
					case 'S':
						system("pause>nul");
						break;
					case 'r':
					case 'R':
						system("cls");
						main();
				}
			}
		}
		shape = nextShape, form = nextForm;
		DrawSpace(nextShape, nextForm, COL + 3, 3);
	}
}

void ReadGrade()
{
	FILE* pf = fopen("./cache/record.info", "r");
	if(pf == NULL)
	{
		pf = fopen("./cache/record.ini", "w");
		fwrite(&grade, sizeof(int), 1, pf);
	}
	fseek(pf, 0, SEEK_SET);		// Set pointer *pf to file head
	fread(&max, sizeof(int), 1, pf);
	fclose(pf);
	pf = NULL;
}

void WriteGrade()
{
	FILE* pf = fopen("./cache/record.ini", "w");
	if(pf == NULL)
	{
		printf("Record save error, _error_code_0X11A77E\n");
		exit(0);
	}
	fwrite(&grade, sizeof(int), 1, pf);
	fclose(pf);
	pf = NULL;
}