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
//  @brief  :		�������߲��ú���
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void SelectFirstLine(void)
{
	FirstLineV4();
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

		ImgJudgeStopLine();		//ʶ��ͣ��
		ImgJudgeRamp();			//ʶ���µ�
		ImgJudgeCurveBroken();	//�����·
#if CIRCLE == 2
		CircleFlag = ImgJudgeCircle(0);
		if (CL == CircleFlag)
		{
			CircleState = 1;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
			CircleFlag = CN;
		}
		else if (CR == CircleFlag)
		{
			CircleState = 1;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
			CircleFlag = CN;
		}
		else
#endif // CIRCLE
			if (LeftPnt.Type == 2 && RightPnt.Type == 2)		//ʮ�ֲ�ͼ
			{
				if (LeftPnt.ErrRow - RightPnt.ErrRow > 10 || RightPnt.ErrRow - LeftPnt.ErrRow > 10)
					FillBevelCross();
				else
					FillLevelCross();
				FindLineNormal(0);

			}
		ImgJudgeBlock();		//ʶ��·��
	}
	if (1 == g_RoadType)
	{
		FindLineLost();
#if CIRCLE == 2
		CircleFlag = ImgJudgeCircle(1);
		if (CL == CircleFlag)
		{
			CircleState = 2;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
			if (Ind_CircleOpen)
				CircleFlag = CN;
		}
		else if (CR == CircleFlag)
		{
			CircleState = 2;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
			if (Ind_CircleOpen)
				CircleFlag = CN;
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
		if (Img_SpecialElemFlag)
			SpecialElemFill();
		if (!Img_SpecialElemFlag)
			MainFill();
	}

	//����У��
	if (RL[DOWN_EAGE] - LL[DOWN_EAGE] <= 40 || ML_Count > DOWN_EAGE - 20		//�±߽��С����Ч��������
		|| RightPnt.ErrCol - LeftPnt.ErrCol > 100)									//�ϱ߽粻����
	{
		ErrorFlag = 4;
	}
	if (!ErrorFlag)
		SpeedRow = GetSpeedRow(ControlMid, LeftPnt.ErrRow, RightPnt.ErrRow);
}
