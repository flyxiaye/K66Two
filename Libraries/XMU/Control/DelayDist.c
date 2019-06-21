#include "headfile.h"
#include "math.h"
#include "GlobalVar.h"	




//================================================================//
//  @note   :		放在中断
//================================================================//

void DistCI(void)
{
	static int CI_sum_dist = 7000;
	static unsigned char CI_enable = 0;     //判断使能

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
		                        if((left_line_norm > ind_left_flag && right_line_norm > ind_right_flag && mid_norm > 1.4 * ind_mid_flag)
		                           || (left_line_norm > 1.8 * ind_left_flag && ind_mid_flag < mid_norm)
		                           || (right_line_norm > 1.8 * ind_right_flag && ind_mid_flag < mid_norm))
		                        {
		                          CircleIsland_into_flag = 3;
		                          if (CircleState > 3) CircleState = 3; 
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
void IndCI(void)
{
	if (right_line_norm > 1.1 && !CircleFlag)
	{
		CircleFlag = 2;
		CircleState = 3;
		gpio_init(D0, GPO, 0);
	}
	if (left_line_norm > 1.1 && !CircleFlag)
	{
		CircleFlag = 1;
		CircleState = 3;
		gpio_init(D0, GPO, 0);
	}
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
          if (1 == BrokenFlag)
          {
            BR_acc_dist += curSpeed;
            if (BR_acc_dist > BR_sum_dist)
              BrokenFlag = 0;
          }
          else BR_acc_dist = 0;
          
}