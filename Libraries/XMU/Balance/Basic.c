#include "GlobalVar.h"
#include "headfile.h"
#include "MK60_ftm.h"
#include "PID.h"
//------------电机管脚-------------------------------------------------









//-------------------------------------------------------------------------------------------------------------------
//  @brief      转换模式函数
//  @param      		
//  @param    		
//  @param     		 	
//  @return     			
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//读取20号扇区偏移0数据类型为uint32
//-------------------------------------------------------------------------------------------------------------------
void changemode()
{
	g_mode++;
	if (g_mode > 4) g_mode = 1;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      模式函数
//  @param      mode  模式		
//  @param    		
//  @param     		 	
//  @return     			
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//读取20号扇区偏移0数据类型为uint32
//-------------------------------------------------------------------------------------------------------------------
void motormode(float mode)
{
	int a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
	if (1 == mode)//直立
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		g_drive_left = RateOut_Stand;
		g_drive_right = RateOut_Stand;
=======
=======
>>>>>>> parent of 31a416a... 常神代码移植
		a1 = 1; a2 = 1;
		b1 = 0; b2 = 0;
		c1 = 0; c2 = 0;
	}
	if (2 == mode)//方向+直立
	{
		a1 = 1; a2 = 1;
		b1 = 0; b2 = 0;
		c1 = -1; c2 = 1;
		g_fSpeedControlOut = 0;
<<<<<<< HEAD
>>>>>>> parent of 31a416a... 常神代码移植
	}
	if (3 == mode)//速度+方向+直立
	{
<<<<<<< HEAD
		g_drive_left = RateOut_Stand - RateOut_Direct;
		g_drive_right = RateOut_Stand + RateOut_Direct;
=======
=======
	}
	if (3 == mode)//速度+方向+直立
	{
>>>>>>> parent of 31a416a... 常神代码移植
		a1 = 1; a2 = 1;
=======
		a1 = 1; a2 = 1;
		b1 = 0; b2 = 0;
		c1 = 0; c2 = 0;
	}
	if (2 == mode)//方向+直立
	{
		a1 = 1; a2 = 1;
		b1 = 0; b2 = 0;
		c1 = -1; c2 = 1;
		g_fSpeedControlOut = 0;
	}
	if (3 == mode)//速度+方向+直立
	{
		a1 = 1; a2 = 1;
>>>>>>> parent of 31a416a... 常神代码移植
		b1 = -1; b2 = -1;
		c1 = -1; c2 = 1;

	}
	if (Img_RampFlag != 1)
	{
		g_fleft = a1 * g_AngleControlOut + c1 * g_fDirectionControlOut + b1 * g_fSpeedControlOut;
		g_fright = a2 * g_AngleControlOut + c2 * g_fDirectionControlOut + b2 * g_fSpeedControlOut;
	}
	if (Img_RampFlag == 1)
	{
		;//避障函数执行
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> parent of 31a416a... 常神代码移植
=======
>>>>>>> parent of 31a416a... 常神代码移植
=======
>>>>>>> parent of 31a416a... 常神代码移植
	}

	if (4 == mode)//手调
	{
		g_fleft = g_duty_left;
		g_fright = g_duty_right;
	}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	if (5 == mode)//拍地模式2
=======
        if (5 == mode)//拍地模式
>>>>>>> parent of 31a416a... 常神代码移植
=======
        if (5 == mode)//拍地模式
>>>>>>> parent of 31a416a... 常神代码移植
=======
        if (5 == mode)//拍地模式
>>>>>>> parent of 31a416a... 常神代码移植
	{
		g_fleft = g_AngleControlOut;
		g_fright = g_AngleControlOut;
	}

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      占空比输出及限幅
//  @param      g_duty_PWMleft 	 输入左电机占空比	
//  @param      g_duty_PWMright	 输入右电机占空比		
//  @param     	 	
//  @return     			
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//读取20号扇区偏移0数据类型为uint32
//-------------------------------------------------------------------------------------------------------------------
void DrivePWM(float g_duty_PWMleft, float g_duty_PWMright)
{
	if (g_duty_PWMright > 8000)
	{
		g_duty_PWMright = 8000;
	}
	if (g_duty_PWMright < -8000)
	{
		g_duty_PWMright = -8000;
	}
	if (g_duty_PWMleft > 8000)
	{
		g_duty_PWMleft = 8000;
	}
	if (g_duty_PWMleft < -8000)
	{
		g_duty_PWMleft = -8000;
	}
	if (g_duty_PWMright >= 0 && g_duty_PWMleft >= 0)
	{
		ftm_pwm_duty(ftm0, RIGHT_GO, 0);
		ftm_pwm_duty(ftm0, LEFT_GO, 0);
		ftm_pwm_duty(ftm0, RIGHT_BACK, (int)g_duty_PWMright);
		ftm_pwm_duty(ftm0, LEFT_BACK, (int)g_duty_PWMleft);
	}
	else if (g_duty_PWMright >= 0 && g_duty_PWMleft <= 0)
	{
		ftm_pwm_duty(ftm0, RIGHT_GO, 0);
		ftm_pwm_duty(ftm0, RIGHT_BACK, (int)g_duty_PWMright);
		ftm_pwm_duty(ftm0, LEFT_GO, (int)(-g_duty_PWMleft));
		ftm_pwm_duty(ftm0, LEFT_BACK, 0);
	}
	else if (g_duty_PWMright <= 0 && g_duty_PWMleft >= 0)
	{
		ftm_pwm_duty(ftm0, RIGHT_GO, (int)(-g_duty_PWMright));
		ftm_pwm_duty(ftm0, RIGHT_BACK, 0);
		ftm_pwm_duty(ftm0, LEFT_GO, 0);
		ftm_pwm_duty(ftm0, LEFT_BACK, (int)g_duty_PWMleft);
	}
	else if (g_duty_PWMright <= 0 && g_duty_PWMleft <= 0)
	{
		ftm_pwm_duty(ftm0, RIGHT_GO, (int)(-g_duty_PWMright));
		ftm_pwm_duty(ftm0, RIGHT_BACK, 0);
		ftm_pwm_duty(ftm0, LEFT_GO, (int)(-g_duty_PWMleft));
		ftm_pwm_duty(ftm0, LEFT_BACK, 0);
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      开关电机标志函数
//  @param      		
//  @param      		
//  @param     	 	
//  @return     			
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//读取20号扇区偏移0数据类型为uint32
//-------------------------------------------------------------------------------------------------------------------
void power()
{
	if (1 == g_drive_flag)
	{
		motormode(g_mode);
		DrivePWM(g_fleft, g_fright);
	}
	else if (0 == g_drive_flag)
	{
		DrivePWM(0, 0);
	}
}
void on_off_flag()
{
	if (1 == g_drive_flag)
	{
		g_drive_flag = 0;
	}
	else if (0 == g_drive_flag)
	{
		g_drive_flag = 1;

	}
}
//================================================================//
//  @brief  :	电磁停车
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void StopCar()
{
	if (ind_left_line < 100 && ind_right_line < 100 && ind_mid < 100)
	{
		g_drive_flag = 0;
		g_MasterOutFlag = 1;
	}
}
================================================================//
  @brief  :	动态前瞻
  @param  :		
  @return :		
  @note   :      void
================================================================//
void DynamicProspect()
{
	int array[50] = { 54,54,54,54,54,53,53,53,52,52,52,51,51,51,50,50,49,49,48,
			48,48,47,47,45,45,45,44,44,44,43,43,43,43,42,42,42,41,40,35,
			35,35,35,35,34,34,34,34,34,34,33 };
	if (curSpeed < 10)
	{
		ProSpect = 55;
	}
	else if (curSpeed > 60)
	{
		ProSpect = 40;
	}
	else if (curSpeed >= 10 && curSpeed <= 60)
	{
		ProSpect = array[(int)curSpeed - 10] + pro;
	}
}
//================================================================//
//  @brief  :	拍地
//  @param  :		
//  @return :		
//  @note   :      void
//================================================================//
//void Ground()
//{
//  
//         static float angle_init = 0;
//         static float flag = 0;
//         static float time = 0;
//         if((BrokenFlag == 2 && g_StateSlave < 3 && g_SlaveOutFlag != 1 && flag == 0) || (StopLineFlag && flag == 0))
//         {
//           if(angle_init == 0)
//           {
//             angle_init = g_angle_set;
//           }
//          
//           g_mode = 5;
//           g_angle_set = 56;
//           time++;
//         }
//         else if(BrokenFlag != 2 || !StopLineFlag)
//         {
//           flag = 0;
//         }
//        
//           if(time >= 50)
//           {
//             flag = 1;
//             g_mode = 3;
//             time = 0;
//             g_drive_flag = 0;
//             g_angle_set = angle_init;
//             angle_init = 0;
//           }
//         
//}
//================================================================//
//  @brief  :	开机加速
//  @param  :		
//  @return :		
//  @note   :      void
//================================================================//
void StartSpeed()
{
	static int flag = 0;
	static float angle_init = 0;
	static int distance = 0;
	static int flag_2 = 0;
	if (flag == 0)
	{
		angle_init = g_angle_set;
		flag = 1;
	}
	if (g_drive_flag == 1 && ABS(distance) < 10000 && flag == 1)
	{

		g_angle_set = angle_init;
		distance += curSpeed;
		flag_2 = 1;
	}
	else if (!g_drive_flag)
	{
		distance = 0;
	}
	if ((ABS(distance) >= 10000 || g_drive_flag == 0) && flag_2 == 1 && flag == 1)
	{
		flag = 0;
		flag_2 = 0;
		g_angle_set = angle_init;
	}

}
////-------------------------------------------------------------------------------------------------------------------
////  @brief      停车函数
////  @param      	
////  @param      	
////  @param     	 	
////  @return     
////---------------------------------------------------------------------------------------------------------------------
//void StopCar(void)
//{
//  int sum = 0;
//  for (int i = 0; i < 188; i++)
//  {
//    if (image[DOWN_EAGE][i] < 50)sum++;
//  }
//
//    if (sum > 150)
//      g_drive_flag = 0;
//}