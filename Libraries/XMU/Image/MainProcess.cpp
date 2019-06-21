#include "headfile.h"
#include "GlobalVar.h"
#include "BasicFun.h"
#include "FindLine.h"
#include "FillSpecialLine.h"
#include "MainProcess.h"
#include "CircleIsland.h"
#include "FirstLineProcess.h"
#include "canny.h"

//================================================================//
//  @brief  :		首行采用函数
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void SelectFirstLine(void)
{
	//GetFirstLineEage();
	FirstLineV4();
	//FindFirstEage_3();
	//if (!FindFirstEageV3())
	//	return;
}
//================================================================//
//  @brief  :		普通补图主程序
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void MainFill(void)
{
	VarInit();
	SelectFirstLine();
	g_RoadType = FirstRowProcess();
	if (0 == g_RoadType)
	{
		FindLineNormal(1);
#if CIRCLE
		if (LeftPnt.Type == 4 && RightPnt.Type == 2 && RightPnt.ErrRow >= UP_EAGE + CIRCLEUP_TH
			&& RightPnt.ErrCol < RIGHT_EAGE - 25)	//右环岛判断
		{
			CircleFlag = IsCircleIsland(CR);
			if (CircleFlag)		//是环岛 环岛补图
			{
				CircleState = 1;
				GetPointA();
				GetPointB();
				GetPointC();
				GetPointD();
				FillLineAB();
				FillLineCD();
				FillAllEage();
			}
			else;
		}
		else if (RightPnt.Type == 4 && LeftPnt.Type == 2 && LeftPnt.ErrRow >= UP_EAGE + CIRCLEUP_TH
			&& LeftPnt.ErrCol > LEFT_EAGE + 25)	//左环岛判断
		{
			CircleFlag = IsCircleIsland(CL);
			if (CircleFlag)		//是环岛 环岛补图
			{
				CircleState = 1;
				GetPointA();
				GetPointB();
				GetPointC();
				GetPointD();
				FillLineAB();
				FillLineCD();
				FillAllEage();
			}
			else;
		}
		else
#endif // CIRCLE
			if (LeftPnt.Type == 2 && RightPnt.Type == 2)
			{
				if (LeftPnt.ErrRow - RightPnt.ErrRow > 10 || RightPnt.ErrRow - LeftPnt.ErrRow > 10)
				{
					FillBevelCross();
				}
				else
				{
					FillLevelCross();
				}
				FindLineNormal(0);
			}
#if BROKEN
			else if (LeftPnt.Type + RightPnt.Type == 3 && !BrokenFlag)
			{
				if (LeftPnt.ErrRow > 45 && RightPnt.ErrRow > 45)
				{
					Point pa = { LeftPnt.ErrRow, LeftPnt.ErrCol };
					Point pb = { RightPnt.ErrRow, RightPnt.ErrCol };
					BrokenFlag = JudgeInBroken(pa, pb);
				}
			}
#endif // BROKEN
		//#if BLOCK_OPEN
		//                if (BlockFlag_1)
		//                {
		//                        if (4 == LeftPnt.Type && 4 == RightPnt.Type)
		//                                 BlockFlag_1 = 0;
		//                        else 
		//                                 BlockFlag_2 = 1;
		//                }
		//                        
		//#endif //BLOCK_OPEN
	}
	if (1 == g_RoadType)
	{
		FindLineLost();
		if (1 == g_RoadType && 2 == LeftPnt.Type && 2 == RightPnt.Type)
		{
			FillBevelCross();
			FindLineNormal(0);
		}
	}
	if (2 == g_RoadType)
	{
		FillFourCross();
		FindLineNormal(0);
	}
	FillMiddleLine();
}


//================================================================//
//  @brief  :		补图主程序
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetML(void)
{
#if BROKEN
	if (BrokenFlag == 2)
		BrokenFlag = JudgeOutBroken();
	if (BrokenFlag == 1)
		if (JudgeOutBroken())
		{
			BrokenFlag = 2;
		}
	if (BrokenFlag == 2)
		;
	else
#endif // BROKEN
	{
		CannyEage();
#if CIRCLE
		if (CircleFlag)		//is CircleIsland 
		{
			CircleFill();
		}
		else
#endif // CIRCLE
			MainFill();
	}
	//中线校验
	if (RL[DOWN_EAGE] - LL[DOWN_EAGE] <= 40)
	{
		ErrorFlag = 4;
		return;
	}
}

//================================================================//
//  @brief  :		判断出断路
//  @param  :		void
//  @return :		1 还处于断路  0已经出了断路
//  @note   :		void
//================================================================//
int JudgeOutBroken(void)
{
	CannyEage();
	VarInit();
	LeftPnt.Type = RightPnt.Type = 0;
	SelectFirstLine();
	static int Num_i = 0;
	static int BrokenAve[5] = { 0 };
	if (BrokenFlag == 1)
	{
		if (Num_i < 5)
		{
			if (Num_i > 0 && BrokenAve[Num_i - 1] - LightThreshold > 30)
				return 1;
			BrokenAve[Num_i++] = LightThreshold;
		}
		else
		{
			for (int i = 0; i < 4; i++)		//更新数组元素
				BrokenAve[i] = BrokenAve[i + 1];
			BrokenAve[4] = LightThreshold;
			//判断条件
			for (int i = 0; i < 4; i++)
			{
				for (int j = i; j < 5; j++)
				{
					if (BrokenAve[i] - BrokenAve[j] > 30)
						return 1;
				}
			}
			return 0;
		}

		//		if (BrokenLastAve == 0)
		//		{
		//			BrokenLastAve = LightThreshold;
		//			return 0;
		//		}
		//		else
		//		{
		//			if (BrokenLastAve - LightThreshold > 30)
		//			{
		//                          BrokenLastAve = LightThreshold;
		//				return 1;
		//			}
		//			else 
		//                        {
		//                          BrokenLastAve = LightThreshold;
		//                          return 0;
		//                        }
		//		}
	}
	else
	{
		FindLineNormal(0);
		if (LeftPnt.ErrRow < DOWN_EAGE - 20 && RightPnt.ErrRow < DOWN_EAGE - 20)
		{
			if (RL[DOWN_EAGE] - LL[DOWN_EAGE] > 94 && RL[DOWN_EAGE - 1] - LL[DOWN_EAGE - 1] > 94
				&& RL[DOWN_EAGE - 2] - LL[DOWN_EAGE - 2] > 94 && RL[DOWN_EAGE - 3] - LL[DOWN_EAGE - 3] > 94)
			{
				BrokenLastAve = 0;
				return 0;
			}
			else return 2;
		}
		else
		{
			return 2;
		}
	}
}