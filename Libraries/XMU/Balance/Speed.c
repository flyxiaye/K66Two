#include "MK60_ftm.h"
#include <GlobalVar.h>
#include "Basic.h"


float g_CarSpeed = 0;
float g_fSpeedControlOut_before = 0;
float g_fSpeedControlOut_new = 0;
float g_CarSpeed_constant = 1;//8053;//转换单位常数
void ExpectSpeedGet()
{
  switch(speed_type)
  {
    case 0:
          spdExp = spdExp0;    
          //g_speedUpFlg = 0;
          break;
    case 1:
          spdExp = spdExp1;   
          //g_speedUpFlg = 1;
          break;
    case 2:
          spdExp = spdExp2;    
         // g_speedUpFlg = 2;
          break; 
    case 3:
          spdExp = spdExp3; 
          //g_speedUpFlg = 3;
          break;
    case 4:
          spdExp = spdExp4; 
          //g_speedUpFlg = 4;
          break;
    default:
          spdExp = spdExp0;      
         // g_speedUpFlg = 0;
  }
}
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
                		g_errorS = spdExp - curSpeed;
//		g_errorS = g_fSpeed_set - curSpeed;
              
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

void  SpeedControlOutput()
{
        static int get_flag = 0;
        static int StartCount = 0;
        //开机加速
//        if(g_drive_flag == 0 && !get_flag)
//        {
//           get_flag = 1;
//        }
//        else if(g_drive_flag && get_flag && StartCount < 1000)
//        {
//              g_fSpeedControlOut = 4000;
//              StartCount++;
//              if(StartCount >= 1000)
//              {
//                get_flag = 0;
//                StartCount = 0;
//              }
//        }
//        else if(g_drive_flag && !get_flag)
//        {
                
          g_nSpeedControlPeriod++;
          g_fSpeedControlOut = (g_fSpeedControlOut_new - g_fSpeedControlOut_before) * (g_nSpeedControlPeriod + 1) / g_SpeedPeriod + g_fSpeedControlOut_before;
          if (10 <= g_nSpeedControlPeriod)
          {
		g_nSpeedControlPeriod = 0;
          }
//        }
}

//void SpeedControlChangeAngle(void)//速度与角度串级暂时控不住
//{
//      //脉冲读取
//    rCurSpeed =-ftm_quad_get(ftm1);
//    lCurSpeed = ftm_quad_get(ftm2);
//    curSpeed = (lCurSpeed+rCurSpeed)>>1;
//    ftm_quad_clean(ftm1);
//    ftm_quad_clean(ftm2);
//    static float last_error = 0;
//    g_speed_error = (spdExp1 - curSpeed) / 1000.0f;
//    g_angle_set = g_angle_set_const -(g_speed_error_p * g_speed_error + g_speed_error_d * (g_speed_error - last_error));
//    last_error = g_speed_error;
//    g_angle_set = MAX(g_angle_set, -20);
//    g_angle_set = MIN(g_angle_set, 40);
//}

