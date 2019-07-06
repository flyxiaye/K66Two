#include "headfile.h"
#include "math.h"
#include "GlobalVar.h"

//================================================================//
//  @brief  :		延距清环岛7状态标志位
//  @param  :		void
//  @return :		void
//  @note   :		放在中断
//================================================================//
void Dist_ClearIslandSeven(void)
{
	static int sum_dist = 1.3 * ONE_METER;
	static int acc_dist = 0;
	if (7 == CircleState)
	{
		acc_dist += curSpeed;
		if (acc_dist > sum_dist)
		{
			Dist_ClearSevenFlag = 1;
		}
		else
			Dist_ClearSevenFlag = 0;
	}
	else
	{
		Dist_ClearSevenFlag = 0;
		acc_dist = 0;
	}
}

//================================================================//
//  @note   :		放在中断
//================================================================//

void DistCI(void)
{
	static int CI_sum_dist = 7000;
	static unsigned char CI_enable = 0; //判断使能

	static int CI_acc_dist = 0;
	static int state = 0;
	switch (state)
	{
	case 0:
		if (CircleFlag && 7 != CircleState)
		{
			state = 1;
			ind_mid_flag = mid_norm;
			ind_left_flag = left_line_norm;
			ind_right_flag = right_line_norm;
			CircleIsland_into_flag = 1;
		}
		break;
	case 1:
		CI_acc_dist += curSpeed;
		if (CI_acc_dist > CI_sum_dist)
		{
			state = 0;
			CircleFlag = 0;
			CircleState = 0;
			CI_acc_dist = 0;
			CircleIsland_into_flag = 0;
		}
		//		else if (ind_left_cross + ind_right_cross > 1800 && ind_left_cross > 450 && ind_right_cross > 450)
		//		{
		//			if (CircleState > 3) CircleState = 3;
		//			state = 2;
		//			CI_acc_dist = 0;
		//		}
		if ((left_column_norm - right_column_norm) > 0.3 || -(left_column_norm - right_column_norm) > 0.3)

		{
			CircleIsland_into_flag = 2;
			if ((left_line_norm > ind_left_flag && right_line_norm > ind_right_flag && mid_norm > 1.4 * ind_mid_flag) || (left_line_norm > 1.8 * ind_left_flag && ind_mid_flag < mid_norm) || (right_line_norm > 1.8 * ind_right_flag && ind_mid_flag < mid_norm))
			{
				CircleIsland_into_flag = 3;
				if (CircleState > 3)
					CircleState = 3;
				state = 2;
				CI_acc_dist = 0;
			}
		}
		break;
	case 2:
		if (7 == CircleState)
		{
			state = 0;
		}
		break;
	default:
		break;
	}
	/*if (CircleFlag && 0 == CI_enable && 7 != CircleState)
		CI_enable = 1;
	if (1 == CI_enable)
	{
		CI_acc_dist += curSpeed;
		if (CI_acc_dist > CI_sum_dist)
		{
			CircleFlag = 0;
			CircleState = 0;
			CI_enable = 0;
			CI_acc_dist = 0;
		}
		else if (left_norm + right_norm > 2000)
		{
			CI_enable = 2;
			CI_acc_dist = 0;
		}
		else;
	}
	if (2 == CI_enable && 7 == CircleState)
	{
		CI_enable = 0;
	}*/
}

//================================================================//
//  @brief  :		电磁判断入环岛
//  @param  :		void
//  @return :		void
//  @note   :		放在中断
//================================================================//
int DIndLeftcol = 0, DIndRightcol = 0, DIndMid = 0, DistFlag = 0, count = 0;
//	int DIndLeftcol_before=0,DIndRightcol_before=0,DIndMid_before=0;
float Dpit = 0;
void IndCI(void)
{
	static int count;
	//static int DIndLeftcol=0,DIndRightcol=0,DIndMid=0,DistFlag=0,count=0;
	if (count <= 4)
	{
		static int DIndLeftcol_before = 0, DIndRightcol_before = 0, DIndMid_before = 0;
		static float Dpit = 0, pit_before = 0;					 //angle pit
		DIndLeftcol = ABS(ind_left_column - DIndLeftcol_before); //求TMD微分
		DIndRightcol = ABS(ind_right_column - DIndRightcol_before);
		DIndMid = ABS(ind_mid - DIndMid_before);
		Dpit = imu_data.pit - pit_before;
		DIndLeftcol_before = DIndLeftcol;
		DIndRightcol_before = DIndRightcol;
		pit_before = imu_data.pit;
	}
	else
		count++;
	// if(Dpit<1&&!DistFlag&&g_drive_flag)
	// {
	// 	if((DIndLeftcol>0.1*DIndLeftcol_before)||(DIndRightcol>0.1*DIndRightcol_before)||(DIndMid>0.1*DIndMid_before))
	// 	{
	// 		if(DIndMid>0.2&&mid_norm>1.3)
	// 		{
	// 					CircleFlag = 2;
	// 					CircleState = 2;
	// 		}
	// 		else if(mid_norm<0.6&&(DIndLeftcol>0.3||DIndRightcol>0.3))
	// 		{
	// 				CircleFlag = 2;
	// 				CircleState = 2;
	// 		}
	// 		else
	// 		{
	// 			/* code */
	// 		}

	// 	}

	// }
	// 	if(Dpit>1&&g_drive_flag)
	// 	{
	// 		DistFlag=1;
	// 	}
	// 	else if(DistFlag)
	// 	{
	// 		count++;
	// 		if(count>500&&Dpit<1)
	// 		{
	// 			DistFlag=0;
	// 		}
	// 	}
	//  static int mid_normflag=0,col_normflag=0,acc_speed=0;;
	//	if ((mid_norm > 1.3&&(right_column_norm>0.15||left_column_norm>0.15)&&((left_column_norm-right_column_norm)>0.1||(right_column_norm-left_column_norm)>0.1)&&!CircleFlag&&!col_normflag&&!mid_normflag)||
	//	(mid_norm<0.8&&((right_column_norm>0.25)||(left_column_norm>0.25))&&((left_column_norm-right_column_norm)>0.15||(right_column_norm-left_column_norm)>0.15) &&!CircleFlag&&!col_normflag&&!mid_normflag))
	//	{
	//          mid_normflag=1;
	//        }
	//          {
	//            col_normflag=1;
	//          }
	//         else if(mid_normflag&&col_normflag)
	//          {
	//            mid_normflag=0;
	//            col_normflag=0;
	//		CircleFlag = 2;
	//		CircleState = 2;
	//          }
	//		  if(mid_normflag)
	//		  {
	//			  acc_speed+=curSpeed;
	//			  if(acc_speed>3000)
	//			  {
	//				  mid_normflag=0;
	//				  acc_speed=0;
	//			  }
	//		  }
	//        else if(!CircleFlag&&!CircleState)
	//        {
	//          mid_normflag=0;
	//          col_normflag=0;
	//        }
}

//================================================================//
//  @brief  :		延距判断断路
//  @param  :		void
//  @return :		void
//  @note   :		放在中断
//================================================================//
void DistBroken(void)
{
	static int BR_sum_dist = 7000;
	static int BR_acc_dist = 0;
	static float flipgyro = 0;
	if (1 == Img_BrokenFlag)
	{
		BR_acc_dist += curSpeed;
		if (BR_acc_dist > BR_sum_dist)
			Img_BrokenFlag = 0;
	}
	else
		BR_acc_dist = 0;
}
void ADclearCircle2()
{
	static int CI_sum_dist = 6000, CircleIntoFlag = 0, CI_all_dist = 30000;
	static int clearflag, indmidflag = 0, indlineflag = 0, indcolumnflag = 0; //各项Flag  中竖横各项电感
	static int state = 0, lineflag = 0, columnflag = 0, midflag = 0;
	static int CI_acc_dist = 0, CI_Direction = 0; //1 left  -1 right
	static float entermidnorm = 0, enterleftlinenorm = 0, enterleftcolumnnorm = 0,
				 enterrighttlinenorm = 0, enterightcolumnnorm = 0;
        if()
	switch (state)
	{
	case 0:

		 if (CircleFlag && 7 != CircleState)
		{
			if (CL == ImgJudgeCircle(0))
			{
				CI_Direction = 1;
			}
			else if (CR == ImgJudgeCircle(0))
			{
				CI_Direction = -1;
			}
			state = 1;
			entermidnorm = mid_norm;
			enterleftlinenorm = left_line_norm;
			enterrighttlinenorm = right_line_norm;
			enterleftcolumnnorm = left_column_norm;
			enterightcolumnnorm = right_column_norm;
			CircleIntoFlag = 1;
		}
		break;
	case 1:
	{
		CI_acc_dist += curSpeed;
		if (CI_acc_dist < CI_sum_dist)
		{
			if ((left_line_norm > enterleftlinenorm || right_line_norm > enterrighttlinenorm) && !lineflag)
			{
				CircleIntoFlag++;
				lineflag = 1;
			}
			if ((left_column_norm > 1.4 * enterleftcolumnnorm || right_column_norm > 1.4 * enterightcolumnnorm) && !columnflag)
			{
				CircleIntoFlag++;
				columnflag = 1;
			}
			if ((mid_norm > 1.5 * entermidnorm) && !midflag)
			{
				CircleIntoFlag++;
				midflag = 1;
			}
		}
		else if (CI_acc_dist > CI_sum_dist && CircleIntoFlag < 3)
		{
			CircleIntoFlag = 0;
			columnflag = 0;
			CircleState = 0;
			CircleFlag = 0;
			lineflag = 0;
			midflag = 0;
			CI_acc_dist = 0;
			state = 0;
		}
		else if (CI_acc_dist > CI_sum_dist && 3 == CircleIntoFlag)
		{
			state = 2;
			CircleIntoFlag = 0;
			lineflag = 0;
			midflag = 0;
			columnflag = 0;
			CI_acc_dist = 0;
		}
		break;
	}
	case 2:
	{
		flipgyro += sensor.Gyro_deg.z * 0.002;
		CI_acc_dist += curSpeed;
		if (CI_acc_dist > CI_all_dist)
		{
			if ((left_line_norm > enterleftlinenorm || right_line_norm > enterrighttlinenorm) && !lineflag)
			{
				CircleIntoFlag++;
				lineflag = 1;
			}
			if ((left_column_norm > 1.4 * enterleftcolumnnorm || right_column_norm > 1.4 * enterightcolumnnorm) && !columnflag)
			{
				CircleIntoFlag++;
				columnflag = 1;
			}
			if ((mid_norm > 1.5 * entermidnorm) && !midflag)
			{
				CircleIntoFlag++;
				midflag = 1;
			}
		}
		if (3 == CircleIntoFlag)
		{
			state = 3;
			if (CircleState <= 6)
			{
				CircleState = 6;
				CircleIntoFlag = 0;
			}
		}
		else if (CircleState >= 6)
		{
			state = 3;
			CircleIntoFlag = 0;
		}
		break;
	}

	case 3:

		flipgyro += sensor.Gyro_deg.z * 0.002;
		CI_acc_dist += curSpeed;
		if (CI_acc_dist > 50000 && CircleState <= 7)
		{
			CircleState = 0;
			state = 0;
		}
		if (flipgyro > 300 && CircleState <= 6 && 1 == CI_Direction)
		{
			CircleState = 6;
		}
		else if (flipgyro < 280 && CircleState >= 6 && 1 == CI_Direction)
		{
			CircleState = 5;
		}
		if (flipgyro < -300 && CircleState <= 6 && -1 == CI_Direction)
		{
			CircleState = 6;
		}
		else if (flipgyro > -280 && CircleState >= 6 && -1 == CI_Direction)
		{
			CircleState = 5;
		}
		if (7 == CircleState)
		{
			state = 0;
			flipgyro=0;
			CI_Direction=0;
			CI_acc_dist=0;
		}
	}
}