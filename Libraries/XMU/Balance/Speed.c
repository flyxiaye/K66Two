#include "MK60_ftm.h"
#include <GlobalVar.h>
#include "Basic.h"


float g_CarSpeed = 0;
float g_fSpeedControlOut_before = 0;
float g_fSpeedControlOut_new = 0;
float g_CarSpeed_constant = 1;//8053;//转换单位常数
//-------------------------------------------------------------------------------------------------------------------
//  @brief      速度计算函数
//  @param      	
//  @param      	
//  @param     	 	
//  @return     
//---------------------------------------------------------------------------------------------------------------------
void SpeedControl()
{
	rCurSpeed = -ftm_quad_get(ftm1);
	lCurSpeed = ftm_quad_get(ftm2);
	curSpeed = (lCurSpeed + rCurSpeed) >> 1;
	g_nLeftMotorPulseSigma += lCurSpeed;
	g_nRightMotorPulseSigma += rCurSpeed;
	ftm_quad_clean(ftm1);
	ftm_quad_clean(ftm2);

	static unsigned char count = 0;
        
	count++;
	if (count >= 10)
	{
		count = 0;
                
		g_fSpeedControlOut_before = g_fSpeedControlOut_new;
		g_errorS = g_fSpeed_set - curSpeed;
              
//                if(curSpeed < 0.2)
                g_fI += g_errorS * g_Speed_I;
		if (g_drive_flag == 0)
		{
			g_fI = 0;
		}
		if (g_fI > Speed_MAX) g_fI = Speed_MAX;
		if (g_fI < -Speed_MAX) g_fI = -Speed_MAX;
	
		g_fSpeedControlOut_new = g_errorS * g_Speed_P + g_fI;

		g_nLeftMotorPulseSigma = 0;
		g_nRightMotorPulseSigma = 0;
	
		g_fSpeedControlOut_new = MAX(g_fSpeedControlOut_new, -MaxSpeed);
		g_fSpeedControlOut_new = MIN(g_fSpeedControlOut_new, MaxSpeed);
	}
       
}
////================================================================//
////  @brief  :		速度环平滑输出
////  @param  :		
////  @return :		
////  @note   :		void
////================================================================//
//void  SpeedControlOutput()
//{
//        static int get_flag = 0;
//        static int StartCount = 0;
//          g_nSpeedControlPeriod++;
//          g_fSpeedControlOut = (g_fSpeedControlOut_new - g_fSpeedControlOut_before) * (g_nSpeedControlPeriod + 1) / g_SpeedPeriod + g_fSpeedControlOut_before;
//          if (10 <= g_nSpeedControlPeriod)
//          {
//		g_nSpeedControlPeriod = 0;
//          }
//}
//================================================================//
//  @brief  :		角度环速度环串级
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void SpeedAngle()
{
  static float Angle_init = 0;
  static int flag = 0;
  if(flag == 0 && g_drive_flag == 1)
  {
    Angle_init = g_angle_set;
    flag = 1;
  }  
  if(flag == 1)
  {
    g_expect_angle = g_angle_set + g_fSpeedControlOut_new * 0.001;
  }
}