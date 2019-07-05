#include "GlobalVar.h"
#include "headfile.h"
#include "RampandBlock.h"
void Ramp1()// 沙雕识别
{
  static int acc_speed=0,rampflag=0;
  static float initangleset=0;
  if(gpio_get(C8)==0&&g_drive_flag&&!rampflag)
  {
    initangleset=g_angle_set;
    g_angle_set=initangleset-Rampangle;
    g_mode=7;
    rampflag=1;
  }
  if(rampflag)
  {
    acc_speed+=curSpeed;
    if(acc_speed>8000&&gpio_get(C8)==1)
    {
      rampflag=0;
      g_mode=3;
      g_angle_set=initangleset;
    }
  }
}
void Ramp2()//图像识别
{
  static int rampflag=0,all_speed=0,acc_speed=0;
  static float initangleset,initg_mode;
  if(Img_RampFlag&&g_drive_flag&&!rampflag)
  {
    gpio_init(D1,GPO,0);
    rampflag=1;
    initangleset=g_angle_set;
    initg_mode=g_mode;
    g_angle_set=initangleset-Rampangle;
    g_mode=7;
  }
    if(rampflag)
  {
    all_speed+=curSpeed;
    acc_speed+=curSpeed;
    if(acc_speed<7000)
    {
     AD_DirectionControl();
     DirectionControlOutput(); 
    }
    else if(acc_speed>7000)
    {
      acc_speed = 0;
      gpio_init(D1,GPO,1);
      
      g_mode=3;
      g_angle_set=initangleset;
      Img_RampFlag=0;
    }
    if(all_speed<12000)
    {
      Img_BrokenFlag = 0;
      Img_BlockFlag = 0;
      if(all_speed>7000)
      {
        Img_RampFlag=0;
      }
    }
    else if(all_speed>=12000)
    {
      rampflag=0;
    }
    if(!g_drive_flag&&rampflag)
    {
      rampflag=0;
      g_mode=initg_mode;
      rampflag=0;
    }
  }
}
//void inf(void)
//{
//	g_inf = collect(ADC1_SE17);
//}
//#define _ANGLE imu_data.yaw
////-------------------------------------------------------------------------------------------------------------------
////  @brief      横断路障2 两拐 陀螺仪
////  @param      camera_RB_flag    //摄像头识别横断路障标志位
////  @param      inf_RB_flag  //红外标志位
////  Sample usage:				
////-------------------------------------------------------------------------------------------------------------------
//void roadblock3(void)
//{
//	if (Img_BlockFlag)
//	{
//    gpio_init(D1,GPO,0);
//		static pidInit_t SteerInit = { 2,0,0 };
//		static PidObject Steer;
//		static int state = 0;
//		static int yaw_init = 0;
//		static int yaw_obj = 0;
//		static int get_flag = 0;
//		if (!get_flag)
//		{
//			yaw_init = _ANGLE;
//			get_flag = 1;
//			yaw_obj = yaw_init - st;
//			speed_type = 3;
//		}
//		sum += curSpeed;
//		switch (state)
//		{
//		case 0:
//			Error_1 = AngleError(_ANGLE, yaw_obj);
//			if (sum > sum_dist)
//			{
//				state = 1;
//				yaw_obj = yaw_init + st;
//			}
//			break;
//		case 1:
//			Error_1 = AngleError(_ANGLE, yaw_obj);
//			if (sum > (sum_dist << 1)
//				&& ind_left_line > 100 && ind_right_line > 100)
//			{
//				state = 0;
//				sum = 0;
//				get_flag = 0;
//				Img_BlockFlag = 0;
//				CircleFlag = 0;
//				CircleState = 0;
//				Img_BrokenFlag = 0;
//				if (1 == g_camera_open)
//					g_handle_open = 1;
//			}
//			break;
//		default:
//			break;
//		}
//	}
//}
//#undef _ANGLE