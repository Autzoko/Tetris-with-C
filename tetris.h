#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>


#define ROW 	29
#define COL 	20

#define DOWN 	80
#define LEFT 	75
#define RIGHT 	77

#define SPACE 	32
#define ESC 	27


struct Face
{
	int data[ROW][COL + 10];
	int color[ROW][COL + 10];
}face;

struct Block 
{
	int space[4][4];
}block[7][4];

int max, grade;

void HideCursor();

void CursorJump(int x, int y);

void InitInterface();

void InitBlockInfo();

void color(int num);

void DrawBlock(int shape, int form, int x, int y);

void DrawSpace(int shape, int form, int x, int y);

int IsLegal(int shape, int form, int x, int y);

int JudgeFunc();

void StartGame();

void ReadGrade();

void WriteGrade();

#endif _TETRIS_H_