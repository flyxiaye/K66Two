#include "GlobalVar.h"
#include "mpu_prepare.h"
#include "balance.h"



//------------------------------------------------------------------
//  @brief  :		PID
//  @param  :		
//  @return :		
//  @note   :		void
//-------------------------------------------------------------------
float PIDControl(PID pid ,ERROR error, float now , float expect )
{
  float out = 0;
  error.ERROR = expect - now;
  error.ERROR_SUM += error.ERROR * pid.KI;
  
  out = error.ERROR * pid.KP + (expect - now - error.ERROR_LAST) * pid.KD + error.ERROR_SUM;
    
  error.ERROR_LAST = expect - now;
  return out;
  
}

//------------------------------------------------------------------
//  @brief  :		限幅
//  @param  :		
//  @return :		
//  @note   :		void
//-------------------------------------------------------------------
float Limit(float Out,float limit)
{
  Out = MAX(Out , -limit);
  Out = MIN(Out , limit);
  return Out;
}

//------------------------------------------------------------------
//  @brief  :		输出控制
//  @param  :		
//  @return :		
//  @note   :		void
//-------------------------------------------------------------------
void OutControl()
{
  static int count_10ms = 0;
  static int count_100ms = 0;
  
  //角速度环控制直立
  RateOut_Stand = PIDControl(RATE_STAND_PID, RATE_STAND_ERROR, sensor.Gyro_deg.y, AngleOut);
  RateOut_Stand  = Limit(RateOut_Stand, 6000);
  
  //角速度环控制转向
  if(dialSwitchFlg2)//摄像头算法
  {
    g_errorD = 94 - ML[ProSpect];
  }
  else if(!dialSwitchFlg2)//电磁算法
  {
    g_errorD = (right_line_norm - left_line_norm) / (right_line_norm + left_line_norm + mid_norm) * 100;
  }
  radius = DynamicPID(RADIUS_PID, RADIUS_ERROR, g_errorD, 0);
  RateOut_Direct = PIDControl(RATE_DIRECT_PID, RATE_DIRECT_ERROR, sensor.Gyro_deg.z, radius * SpeedOut);
  RateOut_Direct  = Limit(RateOut_Direct, 6000);
  
  //角度环
  if(count_10ms >= 5)
  {
    AngleOut = PIDControl(ANGLE_PID, ANGLE_ERROR, imu_data.pit, g_angle_set); 
    AngleOut  = Limit(AngleOut, 5000);
    count_10ms = 0;
  }
  
  //速度环
  if(count_100ms >= 50)
  {
    rCurSpeed = -ftm_quad_get(ftm1);
    lCurSpeed = ftm_quad_get(ftm2);
    curSpeed = (lCurSpeed + rCurSpeed) >> 1;
    SpeedOut = PIDControl(SPEED_PID, SPEED_ERROR, curSpeed, g_speed_set); 
    SpeedOut  = Limit(SpeedOut , 5000);
    count_100ms = 0;
  }
  
  count_10ms ++;
  count_100ms ++;
    
}
//------------------------------------------------------------------
//  @brief  :		计算转弯半径(动态PID)
//  @param  :		
//  @return :		
//  @note   :		void
//-------------------------------------------------------------------
float DynamicPID(PID pid, ERROR error, float now, float expect)
{
   float out = 0;
   error.ERROR = expect - now;
   pid.KP = error.ERROR * error.ERROR / pid.KD + pid.KI;
   error.ERROR_SUM = error.ERROR * pid.KI;
   out = error.ERROR * pid.KP + (error.ERROR - error.ERROR_LAST) * pid.KD + error.ERROR_SUM;
   error.ERROR_LAST = error.ERROR;
   return out;
}