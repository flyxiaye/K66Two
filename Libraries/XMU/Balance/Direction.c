#include "headfile.h"
#include <GlobalVar.h>
#include "Basic.h"
#include "AD.h"
#include "Meeting.h"
float g_fDirectionControlOut_new;
float g_fDirectionControlOut_before;


//================================================================//
//  @brief  :		方向环算法
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void Direction()
{
  void AD_DirectionControl();
  //拨码开关向下拨，摄像头算法
  if(dialSwitchFlg2)
  {
    
    if(BrokenFlag == 2 && MEETING_MODE == 2 || BrokenFlag == 3)
    {
      gpio_init(A7, GPO, 1);
      AD_DirectionControl();
    }
    else
    {
      gpio_init(A7, GPO, 0);
      Camera_DirectionControl();
    }
  }
  //拨码开关向上拨，电磁算法
  else if(!dialSwitchFlg2)
  {
    
      AD_DirectionControl();
//      AD_CircleIsland_Control();
    
  }
}

//================================================================//
//  @brief  :		方向环平滑输出
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void DirectionControlOutput(void)//平滑输出
{
	if (g_nDirectionControlPeriod < 5)
	{
		g_fDirectionControlOut = (g_fDirectionControlOut_new - g_fDirectionControlOut_before) * (g_nDirectionControlPeriod + 1) / g_DirectionPeriod + g_fDirectionControlOut_before;
	}
	g_nDirectionControlPeriod++;
	if (g_nDirectionControlPeriod == 5)
	{
		g_nDirectionControlPeriod = 0;
	}
	if (BlockFlag || g_GetMeetingMaster || BrokenFlag == 3) //限幅
	{
		g_fDirectionControlOut = MAX(g_fDirectionControlOut, -max_duty);
		g_fDirectionControlOut = MIN(g_fDirectionControlOut, max_duty);
	}
	if (BrokenFlag)
	{
            ;
	}
        g_fDirectionControlOut_before = g_fDirectionControlOut_new;

}

//================================================================//
//  @brief  :		电磁偏差算法
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void AD_DirectionControl()
{
  static int count = 0;
  static float g_fDirectionAngleControlOut = 0.0f;
  static float sensorGyroZLast = 0.0f;
  static float g_error_before = 0.0f;
  if(count >= 5)
  {
    count = 0;
    if ((BlockFlag && !g_handle_open) || g_GetMeetingMaster)                //路障控制
    {
      ;
    }
//    if(!CircleIsland_into_flag)//圆环
//    {
      g_errorD = (right_line_norm - left_line_norm) / (right_line_norm + left_line_norm) * 100;
//    }
//    else if(CircleIsland_into_flag)
//    {
//      ;
//    }
    g_fDirectionAngleControlOut = g_errorD * g_dire_P_AD + (g_errorD - g_error_before) * g_dire_D_AD;
    g_fDirectionControlOut_new = (g_fDirectionAngleControlOut - sensor.Gyro_deg.z) * gRateKp_AD + (sensor.Gyro_deg.z -  sensorGyroZLast) * gRateKd_AD;
    sensorGyroZLast = sensor.Gyro_deg.z;
    g_error_before = g_errorD;
  }
  else count++;
}
//================================================================//
//  @brief  :		摄像头偏差算法
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void Camera_DirectionControl()
{
  static int count = 0;
  static float g_fDirectionAngleControlOut = 0.0f;
  static float sensorGyroZLast = 0.0f;
  static float g_error_before = 0.0f;
  if(count >= 5)
  {
    count = 0;
    if (BlockFlag|| g_GetMeetingMaster)                //路障控制
    {
      ;
    }
    else
    {
      g_errorD = 94 - ML[ProSpect];
    
	if (ErrorFlag)
        {
          g_errorD = g_error_before;
        }
    }
    g_fDirectionAngleControlOut = g_errorD * g_dire_P + (g_errorD - g_error_before) * g_dire_D;
    g_fDirectionControlOut_new = (g_fDirectionAngleControlOut - sensor.Gyro_deg.z) * gRateKp + (sensor.Gyro_deg.z - sensorGyroZLast) * gRateKd;
    sensorGyroZLast = sensor.Gyro_deg.z;
    g_error_before = g_errorD;
  }
  else count++;
}