#include "Obstacle.h"
#include <GlobalVar.h>
#include "headfile.h"
#include "PID.h"
//
////�µ�ʶ��
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
//  @brief      ��ȡ����ֵ
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
//  @brief      ���·��2 ���� ������
//  @param      camera_RB_flag    //����ͷʶ����·�ϱ�־λ
//  @param      inf_RB_flag  //�����־λ
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
			yaw_obj = yaw_init + ST[block_count] * st;
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
				g_errorD = -AngleError(_ANGLE, yaw_obj) ;
			}
			if (sum > sum_dist||ST[block_count]==0)
			{
				state = 1;
				yaw_obj = yaw_init - ST[block_count] * st;
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
				g_errorD = -AngleError(_ANGLE, yaw_obj) ;
			}

			if ((sum > sum_dist * 0.9
				&& (ind_left_line > 100 || ind_right_line > 100))||ST[block_count]==0)
			{
				state = 0;
				sum = 0;
				get_flag = 0;
				Img_BlockFlag = 0;
				CircleFlag = 0;
				CircleState = 0;
				Img_BrokenFlag = 0;
				block_count++;
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
//  @brief      �����ǱȽϴ�С(�Ǵ�С)
//  @param      real_angle    //ʵ�ʽǶ�
//  @param      obj_angle  //Ŀ��Ƕ�
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
//  @brief      ���������ǲ�ֵ
//  @param      real_angle    //ʵ�ʽǶ�
//  @param      obj_angle  //Ŀ��Ƕ�
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