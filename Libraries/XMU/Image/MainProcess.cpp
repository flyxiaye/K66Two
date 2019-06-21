#include "GlobalVar.h"
#include "BasicFun.h"
#include "FindLine.h"
#include "FillSpecialLine.h"
#include "MainProcess.h"
#include "CircleIsland.h"
#include "FirstLineProcess.h"
#include "canny.h"
#include "SpecialElem.h"

//================================================================//
//  @brief  :		���в��ú���
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void SelectFirstLine(void)
{
	//GetFirstLineEage();
	FirstLineV4();
	//FindFirstEage_3();
}
//================================================================//
//  @brief  :		��ͨ��ͼ������
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
#if STOPLINE
		//ʶ��ͣ��
		if (Img_StopOpen && LeftIntLine < UP_EAGE + 15 && RightIntLine < UP_EAGE + 15
			&& LeftPnt.ErrRow <= UP_EAGE + 15 && RightPnt.ErrRow <= UP_EAGE + 15
			&& !StopLineFlag && DistStopLine(UP_EAGE + 15))
		{
			StopLineFlag = 1;
			SpecialElemFlag = 1;
		}
#endif
#if RAMP
		//ʶ���µ�
		if (Img_RampOpen && !SpecialElemFlag
			&& LeftPnt.ErrRow <= UP_EAGE + 1 && RightPnt.ErrRow <= UP_EAGE + 1)
		{
			if (IsRamp())
			{
				RampFlag = 1;
				SpecialElemFlag = 1;
			}
		}
#endif
#if CURVE_BROKEN
		//�����·
		if (LeftPnt.ErrRow - RightPnt.ErrRow < 5 && RightPnt.ErrRow - LeftPnt.ErrRow < 5
			&& LeftPnt.ErrCol - RightPnt.ErrCol < 10 && RightPnt.ErrCol - LeftPnt.ErrCol < 10)
		{
			if (JudgeSpecialLine(LeftPnt.ErrRow, RightPnt.ErrRow, 1))
			{
				BrokenFlag = 3;
				SpecialElemFlag = 1;
			}
			else BrokenFlag = 0;
		}
		else BrokenFlag = 0;
#endif

#if CIRCLE == 1
		if (RightPnt.Type == 2 && RightPnt.ErrRow - LeftPnt.ErrRow >= CIRCLEUP_TH && LeftPnt.ErrRow <= UP_EAGE + 20
			&& RightPnt.ErrCol < RIGHT_EAGE - 25)	//�һ����ж�
		{
			CircleFlag = IsCircleIsland(CR);
			string.Format("\r\n CircleFlag = %d \r\n", CircleFlag); PrintDebug(string);
			if (CircleFlag)		//�ǻ��� ������ͼ
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
		else if (LeftPnt.Type == 2 && LeftPnt.ErrRow - RightPnt.ErrRow >= CIRCLEUP_TH && RightPnt.ErrRow <= UP_EAGE + 20
			&& LeftPnt.ErrCol > LEFT_EAGE + 25)	//�󻷵��ж�
		{
			CircleFlag = IsCircleIsland(CL);
			string.Format("\r\n CircleFlag = %d \r\n", CircleFlag); PrintDebug(string);
			if (CircleFlag)		//�ǻ��� ������ͼ
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
#elif CIRCLE == 2
		if (Img_CircleOpen && !SpecialElemFlag
			&& LeftPnt.Type == 2 && LeftPnt.ErrRow > UP_EAGE + 20 && RightPnt.ErrRow < UP_EAGE + 10
			&& LeftPnt.ErrCol < MIDDLE && RightPnt.ErrCol > MIDDLE - 7 && IsCircleIsland(CL))
		{
			//CircleFlag = CL;
			CircleState = 1;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
		}
		else if (Img_CircleOpen && !SpecialElemFlag
			&& RightPnt.Type == 2 && RightPnt.ErrRow > UP_EAGE + 20 && LeftPnt.ErrRow < UP_EAGE + 10
			&& RightPnt.ErrCol > MIDDLE && LeftPnt.ErrCol < MIDDLE + 7 && IsCircleIsland(CR))
		{
			//CircleFlag = CR;
			CircleState = 1;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
		}
		else
#endif // CIRCLE
			if (LeftPnt.Type == 2 && RightPnt.Type == 2)		//ʮ�ֲ�ͼ
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
#if BLOCK_BROKEN
		//ʶ������Ԫ��
		if ((Img_BlockOpen || Img_BrokenOpen) && !SpecialElemFlag)		//��··���ж�
		{
			int flag = JudgeSpecialElem(LeftIntLine, RightIntLine);
			if (1 == Img_BlockOpen && 1 == flag)
			{
				BlockFlag = 1;
				SpecialElemFlag = 1;
			}
			else if (2 == Img_BlockOpen && flag)			//����ʶ��·��
			{
				BrokenFlag = 1;
				SpecialElemFlag = 1;
#if INF
				if (g_inf > stop_inf)
				{
					BlockFlag = 1;
					BrokenFlag = 0;
				}
#endif 
			}
			else if (Img_BrokenOpen && 2 == flag)
			{
				BrokenFlag = 1;
				SpecialElemFlag = 1;
			}
			else;
		}
#endif
	}
	if (1 == g_RoadType)
	{
		FindLineLost();
#if CIRCLE == 2
		if (Img_CircleOpen && !SpecialElemFlag
			&& LL[DOWN_EAGE] == LEFT_EAGE && RightPnt.ErrRow < UP_EAGE + 10 && RightPnt.ErrCol > MIDDLE)
		{
			CircleFlag = CL;
			CircleState = 2;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
		}
		else if (Img_CircleOpen && !SpecialElemFlag
			&& RL[DOWN_EAGE] == RIGHT_EAGE && LeftPnt.ErrRow < UP_EAGE + 10 && LeftPnt.ErrCol < MIDDLE)
		{
			CircleFlag = CR;
			CircleState = 2;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
		}
		else
#endif
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
//  @brief  :		��ͼ������
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetML(void)
{
	CannyEage();

#if CIRCLE
	if (CircleFlag)		//is CircleIsland 
	{
		CircleFill();
	}
	else
#endif // CIRCLE
	{
		if (SpecialElemFlag)
			SpecialElemFill();
		if (!SpecialElemFlag)
			MainFill();
	}
	//����У��
        if (1 == dialSwitchFlg4 && (1 == BrokenFlag || 3 == BrokenFlag) && g_StateMaster == 15)
          BrokenFlag = 0;
          
	if (RL[DOWN_EAGE] - LL[DOWN_EAGE] <= 40 || ML_Count > DOWN_EAGE - 20		//�±߽��С����Ч��������
		|| RightPnt.ErrCol - LeftPnt.ErrCol > 100)									//�ϱ߽粻����
	{
		ErrorFlag = 4;
	}
}

