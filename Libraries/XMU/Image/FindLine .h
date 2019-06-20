#ifndef FindLine_H
#define FindLine_H

void FindLineNormal(int Fill);
void FindLineLost(void);
int FirstRowProcess(void);
Point FindLowPoint(int row, int col1, int col2, int Step);
int JudgeInBroken(Point pa, Point pb);
int JudgeInBroken_V2(Point pa, Point pb);
int IsConnectPoint(Point pa, Point pb);

void FirstLineV4(void);
int FirstJudge(void);

int Diff_R_B(int state);
int SpeedLine(int middle,int left_row, int right_row);
#endif // !FindLine_H
#pragma once
