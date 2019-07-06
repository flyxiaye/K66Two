#include "Obstacle.h"
#include <GlobalVar.h>
#include "headfile.h"
#include "PID.h"
//
////坡道识别
//void JudgeRamp(void)
//{
//  static int angle_init = 0;
//  if (0 == angle_init)
//    angle_init = imu_data.pit;
//  if (RampFlag == 2)
//  {
//    if (imu_data.pit - angle_init < 5 
//        && angle_init - imu_data.pit < 5)
//    {
//      RampFlag = 0;
//      CircleFlag = 0;
//      BrokenFlag = 0;
//      BlockFlag = 0;
//    }
//  }
//  else if (RampFlag == 1)
//  {
//    if (angle_init - imu_data.pit > 12)
//      RampFlag = 2;
//  }
//  else
//  {
//    if (imu_data.pit - angle_init > 12)
//    {
//      RampFlag = 1;
//      CircleFlag = 0;
//      BrokenFlag = 0;
//      BlockFlag = 0;
//    }
//  }
//}
//
//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取红外值
//  @param      void
//  @param      void
//  @return     void 
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void inf(void)
{
	g_inf = collect(ADC1_SE17);
}
#define _ANGLE imu_data.yaw
//-------------------------------------------------------------------------------------------------------------------
//  @brief      横断路障2 两拐 陀螺仪
//  @param      camera_RB_flag    //摄像头识别横断路障标志位
//  @param      inf_RB_flag  //红外标志位
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void roadblock3(void)
{
	if (Img_BlockFlag)
	{
		static pidInit_t SteerInit = { 2,0,0 };
		static PidObject Steer;
		static int state = 0;
		static int yaw_init = 0;
		static int yaw_obj = 0;
		static int get_flag = 0;
		if (!get_flag)
		{
			yaw_init = _ANGLE;
			get_flag = 1;
			yaw_obj = yaw_init + ST[block_count++];
			speed_type = 3;
		}

		switch (state)
		{
		case 0:
			sum += curSpeed;
			if (dialSwitchFlg2)
			{
				g_errorD = -AngleError(_ANGLE, yaw_obj);
			}
			else
			{
				g_errorD = -AngleError(_ANGLE, yaw_obj) / 94;
			}
			if (sum > sum_dist)
			{
				state = 1;
				yaw_obj = yaw_init - st;
				sum = 0;
			}
			break;
		case 1:
			sum += curSpeed;
			if (dialSwitchFlg2)
			{
				g_errorD = -AngleError(_ANGLE, yaw_obj);
			}
			else
			{
				g_errorD = -AngleError(_ANGLE, yaw_obj) / 94;
			}

			if (sum > sum_dist
				&& ind_left_line > 100 && ind_right_line > 100)
			{
				state = 0;
				sum = 0;
				get_flag = 0;
				Img_BlockFlag = 0;
				CircleFlag = 0;
				CircleState = 0;
				Img_BrokenFlag = 0;
				if (1 == g_camera_open)
					g_handle_open = 1;
			}
			break;
		default:
			break;
		}
	}
}

#undef _ANGLE  
//-------------------------------------------------------------------------------------------------------------------
//  @brief      陀螺仪比较大小(非大即小)
//  @param      real_angle    //实际角度
//  @param      obj_angle  //目标角度
//  Sample usage:				
//------------------------------------------------------------------------------------------------------------------- 
int JudgeMaxAngle(float real_angle, float obj_angle)
{
	if (obj_angle > 180) obj_angle -= 360;
	else if (obj_angle < -180) obj_angle += 360;
	if (real_angle > 0 && obj_angle >= 0 && real_angle > obj_angle)
		return 1;
	else if (real_angle <= 0 && obj_angle < 0 && real_angle > obj_angle)
		return 1;
	else return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      返回陀螺仪差值
//  @param      real_angle    //实际角度
//  @param      obj_angle  //目标角度
//  Sample usage:				
//------------------------------------------------------------------------------------------------------------------- 
float AngleError(float real_angle, float obj_angle)
{
	if (obj_angle > 180) obj_angle -= 360;
	else if (obj_angle < -180) obj_angle += 360;
	if (obj_angle - real_angle > 180)
	{
		return (obj_angle - 360 - real_angle);
	}
	else if (real_angle - obj_angle > 180)
	{
		return (obj_angle + 360 - real_angle);
	}
	else
	{
		return (obj_angle - real_angle);
	}
}