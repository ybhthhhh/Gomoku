#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <mmsystem.h>
#include <string.h>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#pragma comment(lib,"winmm.lib")

using namespace std;

#define Max 15
#define Row 15
#define Col 15
#define Unit 46

extern MOUSEMSG m;
extern int x_m, y_m;
extern int Chess_broad[15][15];
extern int Step;
extern int scoreMap[15][15];

void Mouse1();
void Mouse2();
bool Judge(int a,int b);
void change1();
void change2();
void DrawFirstScreen();
void BGM();


