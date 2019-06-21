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
//  @brief  :		特殊元素
//  @param  :		坡道 路障 断路 停车线
//  @return :		void
//  @note   :		坡道：补图开 循环清其他标志
//					路障：补图关 补图开启后清其他标志一次
//					断路：补图开 断路补图模式
//					停车线：补图开 循环清其他标志
//================================================================//
void SpecialElemFill(void)
{
	VarInit();
	LeftPnt.Type = RightPnt.Type = 0;
	SelectFirstLine();
	FindLineNormal(0);
	if (1 == BrokenFlag)
	{
		int flag = JudgeSpecialElem(LeftIntLine, RightIntLine);
		if (/*LeftIntLine < UP_EAGE + 15 && RightIntLine < UP_EAGE + 15
			&& */LeftPnt.ErrRow < UP_EAGE + 15 && RightPnt.ErrRow < UP_EAGE + 15
			&& DistStopLine(UP_EAGE + 15))
		{
			StopLineFlag = 1;
			BrokenFlag = 0;
		}
		else if (1 == Img_BlockOpen && 1 == flag)
		{
			BlockFlag = 1;			//路障
			BrokenFlag = 0;
		}
		else if (2 == Img_BlockOpen && flag)
		{
#if INF
			if (g_inf > stop_inf)
			{
				BlockFlag = 1;
				BrokenFlag = 0;
			}
#endif
		}
		else if (JudgeOutBroken())
		{
			BrokenFlag = 2;			//断路
		}
		LeftPnt.ErrRow = MAX(LeftPnt.ErrRow, LeftIntLine);
		RightPnt.ErrRow = MAX(RightPnt.ErrRow, RightIntLine);
		FillMiddleLine();
	}
	else if (2 == BrokenFlag)
	{
		if (JudgeOutBroken())
		{
			BrokenFlag = 0;
			SpecialElemFlag = 0;
		}
	}
	else if (3 == BrokenFlag)
	{
		if (JudgeOutBroken())
		{
			BrokenFlag = 2;			//断路
		}
	}
	else if (BlockFlag)
	{
		LeftPnt.ErrRow = MAX(LeftPnt.ErrRow, LeftIntLine);
		RightPnt.ErrRow = MAX(RightPnt.ErrRow, RightIntLine);
		FillMiddleLine();
	}
	else if (RampFlag)
	{
		FillMiddleLine();
	}
	else if (StopLineFlag)
	{
		FillMiddleLine();
	}
	else
	{
		SpecialElemFlag = 0;
	}
}


//================================================================//
//  @brief  :		路障断路判断
//  @param  :		void 
//  @return :		1路障 2断路
//  @note   :		void
//================================================================//
int JudgeSpecialElem(int left_line, int right_line)
{
	if (JudgeSpecialLine(left_line, right_line, 0))
	{
		if (IsBlock(left_line, right_line))
			return 1;
		else return 2;
	}
	else return 0;
}

//================================================================//
//  @brief  :		内跳线判断封闭曲线
//  @param  :		左线行数 右线行数 type 0为直道断路 1位弯道断路
//  @return :		1 确认有
//  @note   :		全局变量LL RL
//================================================================//
int JudgeSpecialLine(int left_line, int right_line, int type)
{
	const int StartLine = 30;
	if (left_line < StartLine && right_line < StartLine
		|| left_line - right_line > 8 || right_line - left_line > 8)
		return 0;
	if (!type && LL[left_line] + 15 > RL[right_line]) return 0;

	const int diff = 2;
	int NewRow = left_line;
	int NewRow2 = right_line;
	int OldRow = NewRow;
	int OldRow2 = NewRow2;
	int Middle = (LL[left_line] + RL[right_line]) >> 1;
	for (int i = LL[left_line] + 1; i <= Middle; i++)
	{
		NewRow = SearchUpEage(OldRow + 2, i);
		if (NewRow - OldRow > diff || OldRow - NewRow > diff)
			return 0;
		OldRow = NewRow;
	}
	for (int i = RL[right_line] - 1; i >= Middle; i--)
	{
		NewRow2 = SearchUpEage(OldRow2 + 2, i);
		if (NewRow2 - OldRow2 > diff || OldRow2 - NewRow2 > diff)
			return 0;
		OldRow2 = NewRow2;
	}
	if (NewRow == NewRow2)return 1;
	else return 0;
}

//================================================================//
//  @brief  :		判断路障
//  @param  :		left_line right_line
//  @return :		0无   1路障
//  @note   :		全局变量 LL RL
//================================================================//
int IsBlock(int left_line, int right_line)
{
	int left_end = 0, right_end = 0;
	int left_num = 0, right_num = 0;
	int left_flag = 0, right_flag = 0;
	int left_eage[70], right_eage[70];
	left_eage[left_line] = LL[left_line]; right_eage[right_line] = RL[right_line];
	for (int i = left_line - 1; i > UP_EAGE; i--)
	{
		left_eage[i] = GetLL(i, left_eage[i + 1]);
		if (left_eage[i] - left_eage[i + 1] > FINDLINE_TH)
			left_eage[i] = SearchLeftNoEage(i, left_eage[i]) + 3;
		else if (left_eage[i + 1] - left_eage[i] > FINDLINE_TH || left_eage[i] == LEFT_EAGE)
		{
			left_end = i + 1;
			break;
		}
	}
	if (left_end == 0) left_end = UP_EAGE + 1;
	for (int i = right_line - 1; i > UP_EAGE; i--)
	{
		right_eage[i] = GetRL(i, right_eage[i + 1]);
		if (right_eage[i + 1] - right_eage[i] > FINDLINE_TH)
			right_eage[i] = SearchRightNoEage(i, right_eage[i]) - 3;
		else if (right_eage[i] - right_eage[i + 1] > FINDLINE_TH || right_eage[i] == RIGHT_EAGE)
		{
			right_end = i + 1;
			break;
		}
	}
	if (right_end == 0) right_end = UP_EAGE + 1;

	if (left_line - left_end < 5)
		left_flag = 0;
	else left_flag = 1;
	if (right_line - right_end < 5)
		right_flag = 0;
	else right_flag = 1;

	if (left_flag || right_flag)
	{
		if (left_flag)
		{
			for (int i = left_line; i > left_end; i--)
			{
				if (left_eage[i + 1] - left_eage[i] >= 0)
					left_num++;
			}
			if (left_num >= (left_line - left_end) * 0.8)
				left_num = 1;
			else left_num = 0;
		}
		if (right_flag)
		{
			for (int i = right_line; i > right_end; i--)
			{
				if (right_eage[i] - right_eage[i + 1] >= 0)
					right_num++;
			}
			if (right_num >= (right_line - right_end) * 0.8)
				right_num = 1;
			else right_num = 0;
		}
		if (!(left_num ^ left_flag) && !(right_num ^ right_flag))
			return 1;
		else return 0;
	}
	else return 0;
}

//================================================================//
//  @brief  :		识别坡道
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
int IsRamp(void)
{
	const int DiffTh = 500;
	int DiffSum = 0;
	for (int i = UP_EAGE + 20; i > UP_EAGE; i--)
	{
		DiffSum += (RL[i] - LL[i]) - (MidOffset[i] << 1);
	}
	//string.Format("\r\n RampDiffSum = %d \r\n", DiffSum); PrintDebug(string);
	if (DiffSum > DiffTh)
	{
		int left_out_num = 0, right_out_num = 0;
		for (int i = DOWN_EAGE; i > UP_EAGE - 1; i--)
		{
			if (LL[i] - LL[i - 1] > 0)left_out_num++;
			if (RL[i - 1] - RL[i] > 0)right_out_num++;
		}
		if (left_out_num < 5 && right_out_num < 5)
			return 1;
		else return 0;
	}
	else return 0;
}

//================================================================//
//  @brief  :		判断出断路
//  @param  :		void
//  @return :		1还处于断路  0已经出了断路
//  @note   :		void
//================================================================//
int JudgeOutBroken(void)
{
	//CannyEage();
	//VarInit();
	//LeftPnt.Type = RightPnt.Type = 0;
	//SelectFirstLine();
	static int Num_i = 0;
	static int BrokenAve[5] = { 0 };
	if (BrokenFlag == 1 || BrokenFlag == 3)
	{
		if (Num_i < 5)
		{
			if (Num_i > 0 && BrokenAve[Num_i - 1] - LightThreshold > 45)
			{
				Num_i = 0;
				return 1;
			}
			BrokenAve[Num_i++] = LightThreshold;
			return 0;
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
					if (BrokenAve[i] - BrokenAve[j] > 45)
					{
						Num_i = 0;
						return 1;
					}
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
	else if (2 == BrokenFlag)
	{
		if (Num_i < 5)
		{
			if (Num_i > 0 && LightThreshold - BrokenAve[Num_i - 1] > 45)
			{
				Num_i = 0;
				return 2;
			}
			BrokenAve[Num_i++] = LightThreshold;
			return 0;
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
					if (BrokenAve[j] - BrokenAve[i] > 45)
					{
						Num_i = 0;
						return 2;
					}
				}
			}
			return 0;
		}
		////FindLineNormal(0);
		//if (LeftPnt.ErrRow < DOWN_EAGE - 20 && RightPnt.ErrRow < DOWN_EAGE - 20)
		//{
		//	if (RL[DOWN_EAGE] - LL[DOWN_EAGE] > 94 && RL[DOWN_EAGE - 1] - LL[DOWN_EAGE - 1] > 94
		//		&& RL[DOWN_EAGE - 2] - LL[DOWN_EAGE - 2] > 94 && RL[DOWN_EAGE - 3] - LL[DOWN_EAGE - 3] > 94)
		//	{
		//		BrokenLastAve = 0;
		//		return 0;
		//	}
		//	else return 2;
		//}
		//else
		//{
		//	return 2;
		//}
	}
	return 0;
}

//================================================================//
//  @brief  :		识别停车线
//  @param  :		void
//  @return :		1停车线  0非停车线
//  @note   :		void
//================================================================//
int IsStopLine(int line, int left, int right)
{
	int count = 0;
	for (int i = left + 1; i < right; )
	{
		i = SearchRightEage(line, i);
		i = SearchRightNoEage(line, i);
		count++;
		if (count > 6) return 1;
	}
	return 0;
}

//================================================================//
//  @brief  :		识别停车线以及距离
//  @param  :		void
//  @return :		1停车线  0非停车线
//  @note   :		void
//================================================================//
int DistStopLine(int line)
{
	int a = IsStopLine(line, LL[line], RL[line]);
	line += 2;
	int b = IsStopLine(line, LL[line], RL[line]);
	line += 2;
	int c = IsStopLine(line, LL[line], RL[line]);
	if (a || b || c)return 1;
	else return 0;
}

//================================================================//
//  @brief  :		判断进入断路V2
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
int JudgeInBroken_V2(Point pa, Point pb)
{
	const int diff = 2;
	int NewRow = pa.Row;
	int NewRow2 = pb.Row;
	int OldRow = NewRow;
	int OldRow2 = NewRow2;
	int Middle = (pa.Col + pb.Col) >> 1;
	for (int i = pa.Col + 1; i <= Middle; i++)
	{
		NewRow = SearchUpEage(OldRow + 2, i);
		if (NewRow - OldRow > diff || OldRow - NewRow > diff)
			return 0;
		OldRow = NewRow;
	}
	for (int i = pb.Col - 1; i >= Middle; i--)
	{
		NewRow2 = SearchUpEage(OldRow2 + 2, i);
		if (NewRow2 - OldRow2 > diff || OldRow2 - NewRow2 > diff)
			return 0;
		OldRow2 = NewRow2;
	}
	if (NewRow == NewRow2)return 1;
	else return 0;
}

//================================================================//
//  @brief  :		判断进入断路
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
int JudgeInBroken(Point pa, Point pb)
{
	if (pb.Row - pa.Row < 10 && pa.Row - pb.Row < 10)
	{
		if (pa.Col < pb.Col)
			return IsConnectPoint(pa, pb);
		else return IsConnectPoint(pb, pa);
	}
	else return 0;
}

//================================================================//
//  @brief  :		判断两点是否相连
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
int IsConnectPoint(Point pa, Point pb)
{
	image[pa.Row][pa.Col] = 255;
	if (pa.Col == pb.Col && pa.Row == pb.Row) return 1;
	if (pa.Col < pb.Col)
	{
		if (ImageEage[pa.Row][pa.Col + 1] == HIGH_TH)
		{
			pa.Col++;
			return IsConnectPoint(pa, pb);
		}
		else if (pa.Row < pb.Row)
		{
			if (ImageEage[pa.Row + 1][pa.Col] == HIGH_TH)
			{
				pa.Row++;
				return IsConnectPoint(pa, pb);
			}
			else return 0;
		}
		else if (pa.Row > pb.Row)
		{
			if (ImageEage[pa.Row - 1][pa.Col] == HIGH_TH)
			{
				pa.Row--;
				return IsConnectPoint(pa, pb);
			}
			else return 0;
		}
		else return 0;
	}
	else if (pa.Row < pb.Row)
	{
		if (ImageEage[pa.Row + 1][pa.Col] == HIGH_TH)
		{
			pa.Row++;
			return IsConnectPoint(pa, pb);
		}
		else return 0;
	}
	else if (pa.Row > pb.Row)
	{
		if (ImageEage[pa.Row - 1][pa.Col] == HIGH_TH)
		{
			pa.Row--;
			return IsConnectPoint(pa, pb);
		}
		else return 0;
	}
	else return 0;
}

//================================================================//
//  @brief  :		坡道路障区分
//  @param  :		void
//  @return :		1坡道 2路障
//  @note   :		void
//================================================================//
int Diff_R_B(int state)
{
	if (state == 1)		//检测标志
	{
		if (LeftPnt.Type == 4 && RightPnt.Type == 4)
		{
			int left_eage = MaxArray(&LL[DOWN_EAGE], DOWN_EAGE - UP_EAGE);
			int right_eage = MinArray(&RL[DOWN_EAGE], DOWN_EAGE - UP_EAGE);
			int count = 0;
			for (int i = left_eage + 1; i < right_eage; i++)
			{
				if (UP_EAGE == SearchUpEage(DOWN_EAGE - 20, i))
					count++;
			}
			if (count > (right_eage - left_eage) * 0.8) return 1; //坡道
			else return 2;//路障
		}
		else
		{
			return 2; //路障
		}
	}
}

//================================================================//
//  @brief  :		期望速度控制
//  @param  :		middle 车辆中值
//  @return :		void
//  @note   :		void
//================================================================//
int GetSpeedRow(int middle, int left_row, int right_row)
{
	Point P[3] = {
		{DOWN_EAGE, middle - 5},
	{DOWN_EAGE, middle},
	{DOWN_EAGE, middle + 5} };
	int min_row = MIN(left_row, right_row);
	for (int i = DOWN_EAGE; i > UP_EAGE; i--)
	{
		if ((LL[i] - P[0].Col) * (LL[i - 1] - P[0].Col) <= 0
			|| (RL[i] - P[0].Col) * (RL[i - 1] - P[0].Col) <= 0
			|| UP_EAGE + 1 == i)
		{
			P[0].Row = MAX(i, min_row);
			break;
		}
	}
	for (int i = DOWN_EAGE; i > UP_EAGE; i--)
	{
		if ((LL[i] - P[1].Col) * (LL[i - 1] - P[1].Col) <= 0
			|| (RL[i] - P[1].Col) * (RL[i - 1] - P[1].Col) <= 0
			|| UP_EAGE + 1 == i)
		{
			P[1].Row = MAX(i, min_row);
			break;
		}
	}
	for (int i = DOWN_EAGE; i > UP_EAGE; i--)
	{
		if ((LL[i] - P[2].Col) * (LL[i - 1] - P[2].Col) <= 0
			|| (RL[i] - P[2].Col) * (RL[i - 1] - P[2].Col) <= 0
			|| UP_EAGE + 1 == i)
		{
			P[2].Row = MAX(i, min_row);
			break;
		}
	}
	return (P[1].Row + P[0].Row + P[2].Row) / 3;
}

//================================================================//
//  @brief  :		期望速度计算
//  @param  :		row 赛道边缘行数
//  @return :		void
//  @note   :		void
//================================================================//
int GetSpeedCount(int row, int speed_max, int speed_min, float a)
{
	if (50 < row) return speed_min;
	int temp = (int)(a * (50 - row) * (50 - row) + speed_min + 0.5f);
	return MIN(speed_max, temp);
}
