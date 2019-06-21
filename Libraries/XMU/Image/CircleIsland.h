#ifndef CircleIsland_H
#define CircleIsland_H
int IsCircleIsland(int type);
void CircleIslandImageInit(void);
void CircleFill(void);
void CircleFirstLine(void);
void CircleFindLine(void);
void ChangeState(int Order);
void GetPointA(void);
void GetPointB(void);
void GetPointC(void);
void GetPointD(void);
void FillLineAB(void);
void FillLineCD(void);
void FillAllEage(void);
int FillLineGradient(int *line, int row, int col, float Gradient);
void CircleLight(void);
#endif // !CircleIsland_H
#pragma once
