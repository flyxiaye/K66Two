#ifndef BasicFun_H
#define BasicFun_H

float LeastSquare(int *array, int row1, int row2);

int ArrayMax(int *Array, int n);
int ArrayMin(int *Array, int n);

int IsEage(int row, int col);
int SearchUpEage(int row, int col);
int SearchDownEage(int row, int col);
int SearchLeftEage(int row, int col);
int SearchRightEage(int row, int col);
int SearchLeftNoEage(int row, int col);
int SearchRightNoEage(int row, int col);
Point SearchLeftUpEage(int row, int col);
Point SearchRightUpEage(int row, int col);
Point SearchLeftDownEage(int row, int col);
Point SearchRightDownEage(int row, int col);
Point SearchLeftUpEage45(int row, int col);
Point SearchRightUpEage45(int row, int col);

int GetLL(int row, int col);
int GetRL(int row, int col);
int JumpType(int row, int col);
void RepareLine(int dir, int row, int *flag);
int AveGray(void);
#endif //!BasicFun_H
#pragma once
