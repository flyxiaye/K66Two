#include "GlobalVar.h"
#include "BasicFun.h"
#include "FindLine.h"
#include "FillSpecialLine.h"
#include "CircleIsland.h"
#include "FirstLineProcess.h"
#include "MainProcess.h"

#define LEFT_PNT(row, type)	LeftPnt.ErrRow = (row); LeftPnt.ErrCol = LL[LeftPnt.ErrRow]; LeftPnt.Type = (type)
#define RIGHT_PNT(row, type) RightPnt.ErrRow = (row); RightPnt.ErrCol = RL[RightPnt.ErrRow]; RightPnt.Type = (type)
#define CIRFL_TH 6			//CircleIsland Find Line Threshold
#define CIRCROSSUP_TH 7
#define FIVE_SIX_TH 60

#define CIRCLEFINDUP_TH 6
Point PointA, PointB, PointC, PointD;
int LeftLost = 0, RightLost = 0;
int ChangeFlag = 0;
int ConstLeftEage = 30, ConstRightEage = 158;
//int SixFlag = 0;
//================================================================//
//  @brief  :		环岛判断
//  @param  :		void
//  @return :		CN不是 CL左环岛 CR右环岛
//  @note   :		void
//================================================================//
int IsCircleIsland(int type)
{
#define CIRCLEDIFF_TH 2		//环岛边缘差分阈值
	if (CL == type)
	{
		int TmpRow = LeftPnt.ErrRow;
		if (LL[TmpRow] - LL[TmpRow + 1] <= CIRCLEDIFF_TH
			&& LL[TmpRow + 1] - LL[TmpRow] <= CIRCLEDIFF_TH
			&& LL[TmpRow + 1] - LL[TmpRow + 2] <= CIRCLEDIFF_TH
			&& LL[TmpRow + 2] - LL[TmpRow + 1] <= CIRCLEDIFF_TH
			&& LeftPnt.ErrCol > LEFT_EAGE + 10)
			return CL;
		else return CN;
	}
	else if (CR == type)
	{
		int TmpRow = RightPnt.ErrRow;
		if (RL[TmpRow] - RL[TmpRow + 1] <= CIRCLEDIFF_TH
			&& RL[TmpRow + 1] - RL[TmpRow] <= CIRCLEDIFF_TH
			&& RL[TmpRow + 1] - RL[TmpRow + 2] <= CIRCLEDIFF_TH
			&& RL[TmpRow + 2] - RL[TmpRow + 1] <= CIRCLEDIFF_TH
			&& RightPnt.ErrCol < RIGHT_EAGE - 10)
			return CR;
		else return CN;
	}
	else return CN;
}

//================================================================//
//  @brief  :		环岛补图
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleFill(void)
{
	CircleIslandImageInit();
	//首行
	CircleFirstLine();
	//string.Format("\r\n LLDOWN = %d \r\n", LL[DOWN_EAGE]); PrintDebug(string);
	//string.Format("\r\n RLDOWN = %d \r\n", RL[DOWN_EAGE]); PrintDebug(string);
	//类型转化
	ChangeState(0);
	//搜线
	CircleFindLine();
	//搜A点
	GetPointA();
	//搜B点
	GetPointB();
	//搜C点
	GetPointC();
	//搜D点
	GetPointD();
	//连接AB CD
	FillLineAB();
	FillLineCD();
	//补齐赛道
	FillAllEage();
	//补中线
	FillMiddleLine();
	//for (int i = DOWN_EAGE; i > UP_EAGE; i--)
	//{
	//	ML[i] = (LL[i] + RL[i]) >> 1;
	//}
	//后类型转化
	ChangeState(1);
	//string.Format("\r\n LeftPnt = %d \r\n", LeftPnt.Type); PrintDebug(string);
	//string.Format("\r\n	 LeftPnt = %d \r\n", LeftPnt.ErrRow); PrintDebug(string);
	//string.Format("\r\n	 LeftPnt = %d \r\n", LeftPnt.ErrCol); PrintDebug(string);
	//string.Format("\r\n RightPnt = %d \r\n", RightPnt.Type); PrintDebug(string);
	//string.Format("\r\n	 RightPnt = %d \r\n", RightPnt.ErrRow); PrintDebug(string);
	//string.Format("\r\n	 RightPnt = %d \r\n", RightPnt.ErrCol); PrintDebug(string);
}

//================================================================//
//  @brief  :		环岛处理初始化
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleIslandImageInit(void)
{
	VarInit();
	LeftLost = 0;RightLost = 0;
	LeftPnt.Type = RightPnt.Type = 0;
	LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
	LeftPnt.ErrCol = LEFT_EAGE;
	RightPnt.ErrCol = RIGHT_EAGE;
	//LightThreshold = 80;
}
//================================================================//
//  @brief  :		环岛首行搜线以及边缘校正
//  @param  :		void
//  @return :		void
//  @note   :		小于等于3算丢边
//================================================================//
void CircleFirstLine(void)
{
	//	LL[DOWN_EAGE] = GetLL(DOWN_EAGE, LEFT_EAGE + 80);
	//	RL[DOWN_EAGE] = GetRL(DOWN_EAGE, RIGHT_EAGE - 80);
	//FindFirstEageV3();
	SelectFirstLine();//首行搜线
	//GetFirstLineEage();
	//针对状态7搜首行不准确
//方案一，直接偏移
//	if (7 == CircleState && 1 == CircleFlag)
//	{
//		LL[DOWN_EAGE] = GetLL(DOWN_EAGE, MIDDLE - 30);
//		RL[DOWN_EAGE] = GetRL(DOWN_EAGE, MIDDLE - 30);
//	}
//	else if (7 == CircleState && 2 == CircleFlag)
//	{
//		LL[DOWN_EAGE] = GetLL(DOWN_EAGE, MIDDLE + 30);
//		RL[DOWN_EAGE] = GetRL(DOWN_EAGE, MIDDLE + 30);
//	}
	//方案二返回ErrorFlag=3使用电磁


	LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
	LeftPnt.ErrCol = LL[DOWN_EAGE];
	RightPnt.ErrCol = RL[DOWN_EAGE];
	if (LL[DOWN_EAGE] <= LEFT_EAGE + 3)			//left lost
		LeftLost = 1;
	else LeftLost = 0;
	if (RL[DOWN_EAGE] >= RIGHT_EAGE - 3)		//right lost
		RightLost = 1;
	else RightLost = 0;
#define FIND_UP_LINE 0 //向上搜线行数以校正丢边情况
#define LOST_EAGE_TH 4	//判断为非丢边的阈值
	if (!LeftLost & !RightLost)  //左右均不丢，函数返回
		return;
	int LineL, LineR;
	switch (CircleState)
	{
	case 1:
		if (LeftLost)
		{
			if (CL == CircleFlag)
				LineL = DOWN_EAGE - FIND_UP_LINE;
			else LineL = UP_EAGE;
			for (int i = DOWN_EAGE - 1; i > LineL; i--)
			{
				LL[i] = GetLL(i, LL[i + 1]);
				if (LL[i] > LEFT_EAGE + LOST_EAGE_TH)		//no lost eage
				{
					LEFT_PNT(i, 0);
					LeftLost = 0;
					break;
				}
			}
		}
		if (RightLost)
		{
			if (CR == CircleFlag)
				LineR = DOWN_EAGE - FIND_UP_LINE;
			else LineR = UP_EAGE;
			for (int i = DOWN_EAGE - 1; i > LineR; i--)
			{
				RL[i] = GetRL(i, RL[i + 1]);
				if (RL[i] < RIGHT_EAGE - LOST_EAGE_TH)
				{
					RIGHT_PNT(i, 0);
					RightLost = 0;
					break;
				}
			}
		}
		break;
	case 2:
		if (LeftLost && CR == CircleFlag)
		{
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				LL[i] = GetLL(i, LL[i + 1]);
				if (LL[i] > LEFT_EAGE + LOST_EAGE_TH)		//no lost eage
				{
					LEFT_PNT(i, 0);
					LeftLost = 0;
					break;
				}
			}
		}
		if (RightLost && CL == CircleFlag)
		{
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				RL[i] = GetRL(i, RL[i + 1]);
				if (RL[i] < RIGHT_EAGE - LOST_EAGE_TH)
				{
					RIGHT_PNT(i, 0);
					RightLost = 0;
					break;
				}
			}
		}
		break;
	case 3:
		if (LeftLost && CR == CircleFlag)
		{
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				LL[i] = GetLL(i, LL[i + 1]);
				if (LL[i] > LEFT_EAGE + LOST_EAGE_TH)		//no lost eage
				{
					LEFT_PNT(i, 0);
					LeftLost = 0;
					break;
				}
			}
		}
		if (RightLost && CL == CircleFlag)
		{
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				RL[i] = GetRL(i, RL[i + 1]);
				if (RL[i] < RIGHT_EAGE - LOST_EAGE_TH)
				{
					RIGHT_PNT(i, 0);
					RightLost = 0;
					break;
				}
			}
		}
		break;
	case 4:
		if (CL == CircleFlag && RightLost)
		{
			RightPnt.Type = 1;
		}
		else if (CR == CircleFlag && LeftLost)
		{
			LeftPnt.Type = 1;
		}
		break;
	case 5:
		if (CL == CircleFlag && RightLost)
		{
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				RL[i] = GetRL(i, RL[i + 1]);
				if (RL[i] < RIGHT_EAGE - LOST_EAGE_TH)
				{
					RIGHT_PNT(i, 0);
					RightLost = 0;
					break;
				}
			}
		}
		else if (CR == CircleFlag && LeftLost)
		{
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				LL[i] = GetLL(i, LL[i + 1]);
				if (LL[i] > LEFT_EAGE + LOST_EAGE_TH)		//no lost eage
				{
					LEFT_PNT(i, 0);
					LeftLost = 0;
					break;
				}
			}
		}
		break;
	case 7:
		if (CL == CircleFlag && RightLost)
		{
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				RL[i] = GetRL(i, RL[i + 1]);
				if (RL[i] < RIGHT_EAGE - LOST_EAGE_TH)
				{
					RIGHT_PNT(i, 0);
					RightLost = 0;
					break;
				}
			}
		}
		else if (CR == CircleFlag && LeftLost)
		{
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				LL[i] = GetLL(i, LL[i + 1]);
				if (LL[i] > LEFT_EAGE + LOST_EAGE_TH)		//no lost eage
				{
					LEFT_PNT(i, 0);
					LeftLost = 0;
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		环岛状态变化
//  @param  :		0补图前 1补图后
//  @return :		void
//  @note   :		void
//================================================================//
void ChangeState(int Order)
{
	if (0 == Order)			//补图前
	{
		switch (CircleState)
		{
		case 1:
#define LOST_TH 50
			if (CL == CircleFlag && 1 == LeftLost && RL[DOWN_EAGE] > RIGHT_EAGE - LOST_TH)
				CircleState = 3;
			else if (CR == CircleFlag && 1 == RightLost && LL[DOWN_EAGE] < LEFT_EAGE + LOST_TH)
				CircleState = 3;
			break;
		case 2:
			if (CL == CircleFlag && 0 == LeftLost)
				CircleState = 3;
			else if (CR == CircleFlag && 0 == RightLost)
				CircleState = 3;
			break;
		case 3:
			if (CL == CircleFlag && RIGHT_EAGE == RL[DOWN_EAGE])
			{
				//int TmpRow = SearchUpEage(DOWN_EAGE, LL[DOWN_EAGE] + 5);
				//if (TmpRow < 60)
				CircleState = 4;
			}
			else if (CR == CircleFlag && LEFT_EAGE == LL[DOWN_EAGE])
			{
				//int TmpRow = SearchUpEage(DOWN_EAGE, RL[DOWN_EAGE] - 5);
				//if (TmpRow < 60)
				CircleState = 4;
			}
			break;
		case 4:
			if (CL == CircleFlag && 0 == RightLost)
				CircleState = 5;
			else if (CR == CircleFlag && 0 == LeftLost)
				CircleState = 5;
			break;
		case 6:
			if (CL == CircleFlag && 0 == RightLost)
			{
				int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE;
				int TmpCol = RL[DOWN_EAGE];
				while (1)
				{
					NewRow = SearchUpEage(OldRow + 2, TmpCol - 1);
					if (OldRow - NewRow > 12)
					{
						break;
					}
					else if (OldRow <= 50)
					{
						CircleState = 7;
						break;
					}
					else
					{
						OldRow = NewRow;
						TmpCol--;
					}
				}
			}
			else if (CR == CircleFlag && 0 == LeftLost)
			{
				int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE;
				int TmpCol = LL[DOWN_EAGE];
				while (1)
				{
					NewRow = SearchUpEage(OldRow + 2, TmpCol + 1);
					if (OldRow - NewRow > 12)
					{
						break;
					}
					else if (OldRow <= 50)
					{
						CircleState = 7;
						break;
					}
					else
					{
						OldRow = NewRow;
						TmpCol++;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	else			//补图后
	{
		switch (CircleState)
		{
		case 5:
			if (1 == ChangeFlag)
			{
				ChangeFlag = 0;
				CircleState = 6;
			}
			break;
		case 7:
			if (1 == ChangeFlag)
			{
				ChangeFlag = 0;
				CircleState = 0;
				CircleFlag = 0;
			}
			break;
		default:
			break;
		}
	}
}

//================================================================//
//  @brief  :		环岛搜线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleFindLine(void)
{
	FindLineNormal(0);
	switch (CircleState)
	{
	case 5:
	case 6:
		//拐点判断
		if (CL == CircleFlag)
		{
			LeftPnt.Type = 2;
			RightPnt.Type = 1;
			for (int i = DOWN_EAGE - 1; i > RightPnt.ErrRow; --i)
			{
				int TmpRow = SearchUpEage(i, RL[i] - 1);
				if (i - TmpRow > CIRCROSSUP_TH || UP_EAGE == TmpRow)
				{
					RIGHT_PNT(i, 2);
					break;
				}
			}
		}
		else if (CR == CircleFlag)
		{
			RightPnt.Type = 2;
			LeftPnt.Type = 1;
			for (int i = DOWN_EAGE - 1; i > LeftPnt.ErrRow; --i)
			{
				int TmpRow = SearchUpEage(i, LL[i] + 1);
				if (i - TmpRow > CIRCROSSUP_TH || UP_EAGE == TmpRow)
				{
					LEFT_PNT(i, 2);
					break;
				}
			}
		}
		break;
	case 7:
		//detect exit island
#define EXIT_ISLAND 10
		if (RightPnt.ErrRow - LeftPnt.ErrRow < EXIT_ISLAND && LeftPnt.ErrRow - RightPnt.ErrRow < EXIT_ISLAND
			&& RightPnt.ErrCol - LeftPnt.ErrCol < 4 * EXIT_ISLAND)
		{
			ChangeFlag = 1;
		}
		break;
	default:
		break;
	}
}



//================================================================//
//  @brief  :		搜A点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointA(void)
{
	switch (CircleState)
	{
	case 1:
		if (CL == CircleFlag)
		{
			PointA.Row = LeftPnt.ErrRow;
			PointA.Col = LeftPnt.ErrCol;
		}
		else if (CR == CircleFlag)
		{
			PointA.Row = RightPnt.ErrRow;
			PointA.Col = RightPnt.ErrCol;
		}
		else
			return;
		break;
	case 2:
		//give const PointA
		PointA.Row = DOWN_EAGE;
		if (CL == CircleFlag)
			PointA.Col = ConstLeftEage;
		else if (CR == CircleFlag)
			PointA.Col = ConstRightEage;
		break;
	case 3:
	case 4:
		PointA.Row = DOWN_EAGE;
		if (CL == CircleFlag)
		{
			if (1 == LeftLost && 3 == CircleState)
				PointA.Col = LL[DOWN_EAGE] = ConstLeftEage;
			else
				PointA.Col = LL[DOWN_EAGE] + 1;
		}
		else if (CR == CircleFlag)
		{
			if (1 == RightLost && 3 == CircleState)
				PointA.Col = RL[DOWN_EAGE] = ConstRightEage;
			else
				PointA.Col = RL[DOWN_EAGE] - 1;
		}
		break;
	case 7:
		if (CL == CircleFlag)
		{
			int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE - 2, TmpCol = LL[DOWN_EAGE];
			while (1)
			{
				NewRow = SearchUpEage(OldRow + 2, TmpCol + 1);
				if (OldRow - NewRow > 12 || NewRow <= UP_EAGE)
				{
					PointA.Row = OldRow;
					PointA.Col = TmpCol;
					break;
				}
				else if (TmpCol >= LL[DOWN_EAGE] + 30)
				{
					PointA.Row = OldRow;
					PointA.Col = TmpCol;
					break;
				}
				else
				{
					OldRow = NewRow;
					TmpCol++;
				}
			}
		}
		else if (CR == CircleFlag)
		{
			int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE - 2, TmpCol = RL[DOWN_EAGE];
			while (1)
			{
				NewRow = SearchUpEage(OldRow + 2, TmpCol - 1);
				if (OldRow - NewRow > 12 || NewRow <= UP_EAGE)
				{
					PointA.Row = OldRow;
					PointA.Col = TmpCol;
					break;
				}
				else if (TmpCol <= RL[DOWN_EAGE] - 30)
				{
					PointA.Row = OldRow;
					PointA.Col = TmpCol;
					break;
				}
				else
				{
					OldRow = NewRow;
					TmpCol--;
				}
			}
		}
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		搜B点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointB(void)
{

	switch (CircleState)
	{
	case 1:
	case 2:
	case 3:
		int RowNew, RowOld, TmpCol;
		if (CL == CircleFlag)
		{
			RowNew = SearchUpEage(PointA.Row - 1, PointA.Col + 1);
			if (3 == CircleState && PointA.Row - RowNew > 20 && 0 == LeftLost)
			{
				PointB = PointA;
			}
			else if (RowNew == UP_EAGE) //上边界 向上补线
			{
				PointB.Row = UP_EAGE;
			}
			else
			{
				RowOld = RowNew;
				TmpCol = PointA.Col;
				while (1)
				{
					RowNew = SearchUpEage(RowOld + 2, TmpCol + 1);
					if (RowOld - RowNew > CIRCLEFINDUP_TH || RowNew <= UP_EAGE || TmpCol >= RIGHT_EAGE)	//找到B点
					{
						PointB.Row = RowOld;
						PointB.Col = TmpCol;
						break;
					}
					else if (RightPnt.ErrCol <= TmpCol || DOWN_EAGE + 5 < RowNew)
					{
						PointB.Row = UP_EAGE;
						break;
					}
					else
					{
						RowOld = RowNew;
						TmpCol++;
					}
				}
			}

		}
		else if (CR == CircleFlag)
		{
			RowNew = SearchUpEage(PointA.Row - 1, PointA.Col - 1);
			if (3 == CircleState && PointA.Row - RowNew > 20 && 0 == RightLost)
			{
				PointB = PointA;
			}
			else if (RowNew == UP_EAGE) //上边界 向上补线
			{
				PointB.Row = UP_EAGE;
			}
			else
			{
				RowOld = RowNew;
				TmpCol = PointA.Col;
				while (1)
				{
					RowNew = SearchUpEage(RowOld + 2, TmpCol - 1);
					if (RowOld - RowNew > CIRCLEFINDUP_TH || RowNew <= UP_EAGE || TmpCol <= LEFT_EAGE)	//找到B点
					{
						PointB.Row = RowOld;
						PointB.Col = TmpCol;
						break;
					}
					else if (LeftPnt.ErrCol >= TmpCol || DOWN_EAGE + 5 < RowNew)
					{
						PointB.Row = UP_EAGE;
						break;
					}
					else
					{
						RowOld = RowNew;
						TmpCol--;
					}
				}
			}
		}
		else
			return;
		break;
	case 4:
		if (CL == CircleFlag)
		{
			int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE, TmpCol = LL[DOWN_EAGE];
			while (1)
			{
				NewRow = SearchUpEage(OldRow, TmpCol + 1);
				if (OldRow - NewRow > 15 || NewRow == UP_EAGE)
				{
					PointB.Row = OldRow;
					PointB.Col = TmpCol;
					break;
				}
				else if (TmpCol >= RIGHT_EAGE)
				{
					PointB.Row = UP_EAGE;
					break;
				}
				else
				{
					TmpCol++;
					OldRow = NewRow;
				}
			}
		}
		else if (CR == CircleFlag)
		{
			int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE, TmpCol = RL[DOWN_EAGE];
			while (1)
			{
				NewRow = SearchUpEage(OldRow, TmpCol - 1);
				if (OldRow - NewRow > 15 || NewRow == UP_EAGE)
				{
					PointB.Row = OldRow;
					PointB.Col = TmpCol;
					break;
				}
				else if (TmpCol <= LEFT_EAGE)
				{
					PointB.Row = UP_EAGE;
					break;
				}
				else
				{
					TmpCol--;
					OldRow = NewRow;
				}
			}
		}
		break;
	case 7:
		if (CL == CircleFlag)
		{
			Point PointNew = { SearchUpEage(PointA.Row + 1, PointA.Col + 1), PointA.Col + 1 };
			Point PointOld = PointNew;
			if (UP_EAGE == PointNew.Row)
			{
				PointB.Row = UP_EAGE;
				break;
			}
			while (1)
			{
				PointNew = SearchRightUpEage(PointOld.Row + 1, PointOld.Col);
				if (PointOld.Row - PointNew.Row > 3 || PointNew.Row <= UP_EAGE)
				{
					PointB = PointOld;
					break;
				}
				else if (PointNew.Row >= DOWN_EAGE || PointNew.Col >= RIGHT_EAGE)
				{
					PointB.Row == UP_EAGE;
					break;
				}
				else
				{
					PointOld = PointNew;
				}
			}
		}
		else if (CR == CircleFlag)
		{
			Point PointNew = { SearchUpEage(PointA.Row + 1, PointA.Col - 1), PointA.Col - 1 };
			Point PointOld = PointNew;
			if (UP_EAGE == PointNew.Row)
			{
				PointB.Row = UP_EAGE;
				break;
			}
			while (1)
			{
				PointNew = SearchLeftUpEage(PointOld.Row + 1, PointOld.Col);
				if (PointOld.Row - PointNew.Row > 3 || PointNew.Row <= UP_EAGE)
				{
					PointB = PointOld;
					break;
				}
				else if (PointNew.Row >= DOWN_EAGE || PointNew.Col <= LEFT_EAGE)
				{
					PointB.Row = UP_EAGE;
					break;
				}
				else
				{
					PointOld = PointNew;
				}
			}
		}
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		搜C点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointC(void)
{
	const int UpTh = 3;
	switch (CircleState)
	{
	case 1:
		break;
	case 2:
	case 3:
		if (PointB.Row == UP_EAGE) //Not Find PointB
			PointC.Row = UP_EAGE;
		else
		{
			int TmpRow;
			if (CL == CircleFlag)		//Left CircleIsland
			{
				TmpRow = SearchUpEage(PointB.Row - 1, PointB.Col + 1);
				PointC.Row = TmpRow;
				PointC.Col = PointB.Col + 1;
				//{
				//	Point PointNew, PointOld;
				//	PointNew.Row = TmpRow;
				//	PointNew.Col = PointB.Col + 1;
				//	PointOld = PointNew;
				//	while (1)
				//	{
				//		PointNew = SearchRightUpEage(PointOld.Row + 1, PointOld.Col);
				//		if (PointOld.Row - PointNew.Row > UpTh || PointNew.Row <= UP_EAGE || PointNew.Col >= RIGHT_EAGE)
				//		{
				//			PointC = PointOld;
				//			break;
				//		}
				//		else if (PointOld.Row > PointB.Row)			//find PointC error
				//		{
				//			PointC.Row = UP_EAGE;
				//			break;
				//		}
				//		else
				//		{
				//			PointOld = PointNew;
				//		}
				//	}
				//}
			}
			else if (CR == CircleFlag)		//Right CircleIsland
			{
				TmpRow = SearchUpEage(PointB.Row - 1, PointB.Col - 1);
				PointC.Row = TmpRow;
				PointC.Col = PointB.Col - 1;
				//{
				//	Point PointNew, PointOld;
				//	PointNew.Row = TmpRow;
				//	PointNew.Col = PointB.Col - 1;
				//	PointOld = PointNew;
				//	while (1)
				//	{
				//		PointNew = SearchLeftUpEage(PointOld.Row + 1, PointOld.Col);
				//		if (PointOld.Row - PointNew.Row > UpTh || PointNew.Row <= UP_EAGE || PointNew.Col <= LEFT_EAGE)
				//		{
				//			PointC = PointOld;
				//			break;
				//		}
				//		else if (PointOld.Row > PointB.Row)   //find PointC error
				//		{
				//			PointC.Row = UP_EAGE;
				//			break;
				//		}
				//		else
				//		{
				//			PointOld = PointNew;
				//		}
				//	}
				//}
			}
			else return;
		}
		break;
	case 4:
		/*if (CL == CircleFlag)
		{
			int TmpRow = SearchUpEage(PointB.Row, PointB.Col + 1);
			if (UP_EAGE == TmpRow)
			{
				PointC.Row = UP_EAGE + 1;
				PointC.Col = PointB.Col + 1;
			}
			else
			{
				Point PointNew = { TmpRow,PointB.Col + 1 };
				Point PointOld = PointNew;
				while (1)
				{
					PointNew = SearchLeftUpEage(PointOld.Row + 1, PointOld.Col);
					if (PointOld.Row - PointNew.Row > 3 || PointNew.Row <= UP_EAGE)
					{
						PointC = PointOld;
						break;
					}
					else if (PointNew.Row >= DOWN_EAGE || PointNew.Col >= RIGHT_EAGE)
					{
						PointC.Row = UP_EAGE + 1;
						PointC.Col = PointB.Col + 1;
						break;
					}
					else
					{
						PointOld = PointNew;
					}
				}
			}
			LL[PointC.Row] = PointC.Col;
		}
		else if (CR == CircleFlag)
		{
			int TmpRow = SearchUpEage(PointB.Row, PointB.Col - 1);
			if (UP_EAGE == TmpRow)
			{
				PointC.Row = UP_EAGE + 1;
				PointC.Col = PointB.Col - 1;
			}
			else
			{
				Point PointNew = { TmpRow,PointB.Col - 1 };
				Point PointOld = PointNew;
				while (1)
				{
					PointNew = SearchLeftUpEage(PointOld.Row + 1, PointOld.Col);
					if (PointOld.Row - PointNew.Row > 3 || PointNew.Row <= UP_EAGE)
					{
						PointC = PointOld;
						break;
					}
					else if (PointNew.Row >= DOWN_EAGE || PointNew.Col <= LEFT_EAGE)
					{
						PointC.Row = UP_EAGE + 1;
						PointC.Col = PointB.Col - 1;
						break;
					}
					else
					{
						PointOld = PointNew;
					}
				}
			}
			RL[PointC.Row] = PointC.Col;
		}
		else return;*/
		if (CL == CircleFlag)
		{
			Point TmpPnt = SearchRightUpEage45(PointB.Row, PointB.Col + 1);
			RL[TmpPnt.Row + 1] = SearchRightEage(TmpPnt.Row + 1, TmpPnt.Col);
			RIGHT_PNT(TmpPnt.Row + 1, 1);
			int i;
			for (i = TmpPnt.Row + 2; i < DOWN_EAGE; ++i)
			{
				RL[i] = GetRL(i, RL[i - 1]);
				if (RL[i] - RL[i - 1] > 2 * CIRFL_TH || RL[i - 1] - RL[i] > 2 * CIRFL_TH || RL[i] >= RIGHT_EAGE - 3)
				{
					PointC.Row = i - 1;
					PointC.Col = RL[i - 1];
					break;
				}
			}
			if (DOWN_EAGE == i)
			{
				PointC.Row = DOWN_EAGE;
			}
			if (PointC.Row >= PointB.Row)
			{
				PointC.Row = UP_EAGE;
				PointC.Col = PointB.Col;
				RL[PointC.Row] = PointC.Col;
			}
		}
		else if (CR == CircleFlag)
		{
			Point TmpPnt = SearchLeftUpEage45(PointB.Row, PointB.Col - 1);
			LL[TmpPnt.Row + 1] = SearchLeftEage(TmpPnt.Row + 1, TmpPnt.Col);
			LEFT_PNT(TmpPnt.Row + 1, 1);
			int i;
			for (i = TmpPnt.Row + 2; i < DOWN_EAGE; ++i)
			{
				LL[i] = GetLL(i, LL[i - 1]);
				if (LL[i] - LL[i - 1] > 2 * CIRFL_TH || LL[i - 1] - LL[i] > 2 * CIRFL_TH || LL[i] <= LEFT_EAGE + 3)
				{
					PointC.Row = i - 1;
					PointC.Col = LL[i - 1];
					break;
				}
			}
			if (DOWN_EAGE == i)
			{
				PointC.Row = DOWN_EAGE;
			}
			if (PointC.Row >= PointB.Row)
			{
				PointC.Row = UP_EAGE;
				PointC.Col = PointB.Col;
				LL[PointC.Row] = PointC.Col;
			}
		}
		else return;
		break;
	case 5:
		if (CL == CircleFlag)
		{
			if (RightPnt.Type == 2 && RightPnt.ErrRow > FIVE_SIX_TH || DOWN_EAGE == RightPnt.ErrRow)
			{
				ChangeFlag = 1;
				PointC.Col = MIDDLE - 40;
				PointC.Row = SearchUpEage((LeftPnt.ErrRow + RightPnt.ErrRow) >> 1, PointC.Col);
			}
			else PointC.Row = UP_EAGE;
		}
		else if (CR == CircleFlag)
		{
			if (LeftPnt.Type == 2 && LeftPnt.ErrRow > FIVE_SIX_TH || DOWN_EAGE == LeftPnt.ErrRow)
			{
				ChangeFlag = 1;
				PointC.Col = MIDDLE + 40;
				PointC.Row = SearchUpEage((RightPnt.ErrRow + LeftPnt.ErrRow) >> 1, PointC.Col);
			}
			else PointC.Row = UP_EAGE;
		}
		else return;
		break;
	case 6:
		PointC.Col = MIDDLE;
		PointC.Row = SearchUpEage((LeftPnt.ErrRow + RightPnt.ErrRow) >> 1, PointC.Col);
		break;
	case 7:
		if (CL == CircleFlag && DOWN_EAGE - 25 < RightPnt.ErrRow
			|| CR == CircleFlag && DOWN_EAGE - 25 < LeftPnt.ErrRow)
		{
			PointC.Col = (LeftPnt.ErrCol + RightPnt.ErrCol) >> 1;
			PointC.Row = SearchUpEage((LeftPnt.ErrRow + RightPnt.ErrRow) >> 1, PointC.Col);
		}
		else PointC.Row = UP_EAGE;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		搜D点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointD(void)
{
	switch (CircleState)
	{
	case 1:
		break;
	case 2:
	case 3:
		if (UP_EAGE == PointB.Row)			//Not find PointB
			PointD.Row = UP_EAGE;
		else if (CL == CircleFlag)		//left CircleIsland
		{
			PointD.Row = DOWN_EAGE;
			PointD.Col = RL[DOWN_EAGE];
		}
		else if (CR == CircleFlag)
		{
			PointD.Row = DOWN_EAGE;
			PointD.Col = LL[DOWN_EAGE];
		}
		else return;
		break;
	case 4:
		PointD.Row = DOWN_EAGE;
		if (CL == CircleFlag)
		{
			if (LL[DOWN_EAGE] + 130 > RIGHT_EAGE)
				PointD.Col = RIGHT_EAGE;
			else PointD.Col = LL[DOWN_EAGE] + 130;
		}
		else if (CR == CircleFlag)
		{
			if (RL[DOWN_EAGE] - 130 < LEFT_EAGE)
				PointD.Col = LEFT_EAGE;
			else
				PointD.Col = RL[DOWN_EAGE] - 130;
		}
		else return;
		break;
	case 5:
	case 6:
		if (CL == CircleFlag)
		{
			PointD.Row = RightPnt.ErrRow;
			PointD.Col = RightPnt.ErrCol;
		}
		else if (CR == CircleFlag)
		{
			PointD.Row = LeftPnt.ErrRow;
			PointD.Col = LeftPnt.ErrCol;
		}
		break;
	case 7:
		if (UP_EAGE != PointC.Row)
		{
			if (CL == CircleFlag)
			{
				PointD.Row = RightPnt.ErrRow;
				PointD.Col = RL[PointD.Row];
			}
			else if (CR == CircleFlag)
			{
				PointD.Row = LeftPnt.ErrRow;
				PointD.Col = LL[PointD.Row];
			}
		}
		else
			PointD.Row = UP_EAGE;
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		连接AB
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillLineAB(void)
{
	switch (CircleState)
	{
	case 1:
		if (CL == CircleFlag)
		{
			if (UP_EAGE == PointB.Row)
				LeftPnt = FillLineUp(LL, PointA.Row + 6, PointA.Row + 2);
			else
			{
				LL[PointB.Row] = PointB.Col;
				LL[PointA.Row] = PointA.Col;
				FillLinePoint(LL, PointA.Row, PointB.Row);
				LEFT_PNT(PointB.Row, 0);
			}
		}
		else if (CR == CircleFlag)
		{
			if (UP_EAGE == PointB.Row)
				RightPnt = FillLineUp(RL, PointA.Row + 6, PointA.Row + 2);
			else
			{
				RL[PointB.Row] = PointB.Col;
				RL[PointA.Row] = PointA.Col;
				FillLinePoint(RL, PointA.Row, PointB.Row);
				RIGHT_PNT(PointB.Row, 0);
			}
		}
		break;
	case 2:
		if (CL == CircleFlag)
		{
			if (UP_EAGE == PointB.Row)
				LeftPnt = FillLineUp(LL, PointA.Row + 6, PointA.Row + 2);
			else
			{
				LL[PointB.Row] = PointB.Col;
				LL[PointA.Row] = PointA.Col;
				FillLinePoint(LL, PointA.Row, PointB.Row);
				LEFT_PNT(PointB.Row, 0);
			}
		}
		else if (CR == CircleFlag)
		{
			if (UP_EAGE == PointB.Row)
				RightPnt = FillLineUp(RL, PointA.Row + 6, PointA.Row + 2);
			else
			{
				RL[PointB.Row] = PointB.Col;
				RL[PointA.Row] = PointA.Col;
				FillLinePoint(RL, PointA.Row, PointB.Row);
				RIGHT_PNT(PointB.Row, 0);
			}
		}
		break;
	case 3:
		if (CL == CircleFlag)
		{
			if (1 == LeftLost)
			{
				LL[PointA.Row] = PointA.Col;
				LL[PointB.Row] = PointB.Col;
				FillLinePoint(LL, PointA.Row, PointB.Row);
				LEFT_PNT(PointB.Row, 0);
			}
		}
		else if (CR == CircleFlag)
		{
			if (1 == RightLost)
			{
				RL[PointA.Row] = PointA.Col;
				RL[PointB.Row] = PointB.Col;
				FillLinePoint(RL, PointA.Row, PointB.Row);
				RIGHT_PNT(PointB.Row, 0);
			}
		}
		else;
		break;
	case 7:
		if (CL == CircleFlag)
		{
			if (UP_EAGE == PointB.Row)		//no find line AB
			{
				if (RL[UP_EAGE + 20] < LEFT_EAGE + 60)
					;
				else
				{
					PointB.Row = UP_EAGE + 20;
					PointB.Col = RL[UP_EAGE + 20] - 55;
					if (LeftLost)			//lost A
					{
						PointA.Col = LEFT_EAGE;
						PointA.Row = DOWN_EAGE;
					}
					else if (RL[DOWN_EAGE] - PointA.Col > 120)
					{
						PointA.Col = RL[DOWN_EAGE] - 120;
						PointA.Row = DOWN_EAGE;
					}
					LL[PointB.Row] = PointB.Col;
					LL[PointA.Row] = PointA.Col;
					FillLinePoint(LL, PointA.Row, PointB.Row);
					LeftPnt = FillLineUp(LL, PointB.Row + 3, PointB.Row);
				}
			}
			else
			{
				LL[PointB.Row] = PointB.Col;
				LEFT_PNT(PointB.Row, 0);
				if (RL[DOWN_EAGE] - PointA.Col > 120)				//lost A
				{
					//LL[PointB.Row - 1] = GetLL(PointB.Row - 1, LL[PointB.Row]);
					//LL[PointB.Row - 2] = GetLL(PointB.Row - 2, LL[PointB.Row - 1]);
					//LL[PointB.Row - 3] = GetLL(PointB.Row - 3, LL[PointB.Row - 2]);
					//FillLineDown(LL, PointB.Row, PointB.Row - 3);
					PointA.Row = DOWN_EAGE;
					PointA.Col = RL[DOWN_EAGE] - 120;
					FillLinePoint(LL, PointA.Row, PointB.Row);
				}
				else
				{
					LL[PointA.Row] = PointA.Col;
					FillLinePoint(LL, PointA.Row, PointB.Row);
				}
			}
		}
		else if (CR == CircleFlag)
		{
			if (UP_EAGE == PointB.Row)		//no find line AB
			{
				if (LL[UP_EAGE + 20] > RIGHT_EAGE - 60)
					;
				else
				{
					PointB.Row = UP_EAGE + 20;
					PointB.Col = LL[UP_EAGE + 20] + 55;
					if (RightLost)			//lost A
					{
						PointA.Col = RIGHT_EAGE;
						PointA.Row = DOWN_EAGE;
					}
					else if (PointA.Col - LL[DOWN_EAGE] > 120)
					{
						PointA.Col = LL[DOWN_EAGE] + 120;
						PointA.Row = DOWN_EAGE;
					}
					RL[PointB.Row] = PointB.Col;
					RL[PointA.Row] = PointA.Col;
					FillLinePoint(RL, PointA.Row, PointB.Row);
					RightPnt = FillLineUp(RL, PointB.Row + 3, PointB.Row);
				}
			}

			else
			{
				RL[PointB.Row] = PointB.Col;
				RIGHT_PNT(PointB.Row, 0);
				if (PointA.Col - LL[DOWN_EAGE] > 120)				//lost A
				{
					//RL[PointB.Row - 1] = GetRL(PointB.Row - 1, RL[PointB.Row]);
					//RL[PointB.Row - 2] = GetRL(PointB.Row - 2, RL[PointB.Row - 1]);
					//RL[PointB.Row - 3] = GetRL(PointB.Row - 3, RL[PointB.Row - 2]);
					//FillLineDown(RL, PointB.Row, PointB.Row - 3);
					PointA.Row = DOWN_EAGE;
					PointA.Col = LL[DOWN_EAGE] + 120;
					FillLinePoint(RL, PointA.Row, PointB.Row);
				}
				else
				{
					RL[PointA.Row] = PointA.Col;
					FillLinePoint(RL, PointA.Row, PointB.Row);
				}
			}
		}
		break;
	default:
		break;
	}
}
//================================================================//
//  @brief  :		连接CD
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillLineCD(void)
{
	switch (CircleState)
	{
	case 1:
		break;
	case 2:
		if (CL == CircleFlag)
		{
			if (UP_EAGE == PointC.Row);
			else
			{
				RL[PointD.Row] = PointD.Col;
				RL[PointC.Row] = PointC.Col;
				FillLinePoint(RL, PointD.Row, PointC.Row);
				RIGHT_PNT(PointC.Row, 1);
			}
		}
		if (CR == CircleFlag)
		{
			if (UP_EAGE == PointC.Row);
			else
			{
				LL[PointD.Row] = PointD.Col;
				LL[PointC.Row] = PointC.Col;
				FillLinePoint(LL, PointD.Row, PointC.Row);
				LEFT_PNT(PointC.Row, 1);
			}
		}
		break;
	case 3:
		if (UP_EAGE == PointB.Row)
			return;
		else if (CL == CircleFlag)
		{
			RL[PointD.Row] = PointD.Col;
			RL[PointC.Row] = PointC.Col;
			FillLinePoint(RL, PointD.Row, PointC.Row);
			RIGHT_PNT(PointC.Row, 1);
		}
		else if (CR == CircleFlag)
		{
			LL[PointD.Row] = PointD.Col;
			LL[PointC.Row] = PointC.Col;
			FillLinePoint(LL, PointD.Row, PointC.Row);
			LEFT_PNT(PointC.Row, 1);
		}
		break;
	case 4:
		if (CL == CircleFlag)
		{
			RL[PointD.Row] = PointD.Col;
			FillLinePoint(RL, PointD.Row, PointC.Row);
		}
		else if (CR == CircleFlag)
		{
			LL[PointD.Row] = PointD.Col;
			FillLinePoint(LL, PointD.Row, PointC.Row);
		}
		break;
	case 5:
		if (CL == CircleFlag && 2 == RightPnt.Type)
		{
			if (RightPnt.ErrRow < 60)
			{
				RightPnt = FillLineUp(RL, RightPnt.ErrRow + 4, RightPnt.ErrRow + 1);
			}
			else
			{
				RL[PointD.Row] = PointD.Col;
				int TempRow = FillLineGradient(RL, PointD.Row, PointD.Col, 3);
				RIGHT_PNT(TempRow, 1);
			}
		}
		else if (CR == CircleFlag && 2 == LeftPnt.Type)
		{
			if (LeftPnt.ErrRow < 60)
				LeftPnt = FillLineUp(LL, LeftPnt.ErrRow + 4, LeftPnt.ErrRow + 1);
			else
			{
				LL[PointD.Row] = PointD.Col;
				int TempRow = FillLineGradient(LL, PointD.Row, PointD.Col, -3);
				LEFT_PNT(TempRow, 1);
			}
		}
		break;
	case 6:
		if (CL == CircleFlag)
		{
			RL[PointD.Row] = PointD.Col;
			int TempRow = FillLineGradient(RL, PointD.Row, PointD.Col, 3);
			RIGHT_PNT(TempRow, 1);
		}
		else if (CR == CircleFlag)
		{
			LL[PointD.Row] = PointD.Col;
			int TempRow = FillLineGradient(LL, PointD.Row, PointD.Col, -3);
			LEFT_PNT(TempRow, 1);
		}
		break;
	case 7:
		if (CL == CircleFlag)
		{
			if (UP_EAGE != PointC.Row)	//fill line CD
			{
				RL[PointC.Row] = PointC.Col;
				RL[PointD.Row] = PointD.Col;
				FillLinePoint(RL, PointD.Row, PointC.Row);
				RIGHT_PNT(PointC.Row, 0);
			}
		}
		else if (CR == CircleFlag)
		{
			if (UP_EAGE != PointC.Row)	//fill line CD
			{
				LL[PointC.Row] = PointC.Col;
				LL[PointD.Row] = PointD.Col;
				FillLinePoint(LL, PointD.Row, PointC.Row);
				LEFT_PNT(PointC.Row, 0);
			}
		}
		break;
	default:
		break;
	}
}
//================================================================//
//  @brief  :		补齐赛道
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillAllEage(void)
{
	switch (CircleState)
	{
	case 1:
	case 2:
	case 3:
	case 7:
		FindLineNormal(0);
		break;
		//case 3:
	case 4:
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		给定斜率补线
//  @param  :		void
//  @return :		行数
//  @note   :		void
//================================================================//
int FillLineGradient(int *line, int row, int col, float Gradient)
{
	int *p = line + row - 1;
	for (int i = row - 1; i > UP_EAGE; i--)
	{
		*p = (int)((i - row) * Gradient + col + 0.5);
		if (*p > RIGHT_EAGE || *p < LEFT_EAGE)
		{
			return i + 1;
		}
		p--;
	}
	return UP_EAGE + 1;
}