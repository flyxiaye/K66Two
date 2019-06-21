#include "MK60_ftm.h"
#include <GlobalVar.h>
#include "Basic.h"


float g_CarSpeed = 0;
float g_fSpeedControlOut_before = 0;
float g_fSpeedControlOut_new = 0;
float g_CarSpeed_constant = 1;//8053;//转换单位常数
float g_fI = 0;//积分项暂存处

void SpeedControl()
{
  rCurSpeed = -ftm_quad_get(ftm1);
  lCurSpeed = ftm_quad_get(ftm2);
  curSpeed = (lCurSpeed + rCurSpeed) >> 1;
  g_nLeftMotorPulseSigma+=lCurSpeed;
  g_nRightMotorPulseSigma+=rCurSpeed;
  ftm_quad_clean(ftm1);
  ftm_quad_clean(ftm2);
  
  static unsigned char count = 0;
  count++;
  if (count >= 10)
  {
    count = 0;
//    g_nLeftMotorPulseSigma = ftm_quad_get(ftm1);
//    g_nRightMotorPulseSigma = ftm_quad_get(ftm2);
//    ftm_quad_clean(ftm1);
//    ftm_quad_clean(ftm2);
    g_CarSpeed =  0.9*(g_nLeftMotorPulseSigma + g_nRightMotorPulseSigma)/2+0.1*g_CarSpeed;
    g_CarSpeed *= g_CarSpeed_constant;
    g_fSpeedControlOut_before = g_fSpeedControlOut_new;
    g_errorS = g_fSpeed_set - g_CarSpeed;
    g_fI += g_errorS * g_Speed_I;
    g_fSpeedControlOut_new = g_errorS * g_Speed_P + g_fI;
    g_fSpeedControlOut_new = MAX(g_fSpeedControlOut_new, -500);
    g_fSpeedControlOut_new = MIN(g_fSpeedControlOut_new, 500);
    g_nLeftMotorPulseSigma = 0;
    g_nRightMotorPulseSigma = 0;
  }
}

void  SpeedControlOutput()
{
  g_nSpeedControlPeriod++;
  g_fSpeedControlOut=(g_fSpeedControlOut_new-g_fSpeedControlOut_before)*(g_nSpeedControlPeriod + 1)/g_SpeedPeriod+g_fSpeedControlOut_before;
  if (10 <= g_nSpeedControlPeriod)   
  {
    g_nSpeedControlPeriod = 0;
  }
}

void SpeedControlChangeAngle(void)
{
      //脉冲读取
    rCurSpeed =-ftm_quad_get(ftm1);
    lCurSpeed = ftm_quad_get(ftm2);
    curSpeed = (lCurSpeed+rCurSpeed)>>1;
    ftm_quad_clean(ftm1);
    ftm_quad_clean(ftm2);
    static float last_error = 0;
    g_speed_error = (spdExp1 - curSpeed) / 1000.0f;
    g_angle_set = g_angle_set_const -(g_speed_error_p * g_speed_error + g_speed_error_d * (g_speed_error - last_error));
    last_error = g_speed_error;
    g_angle_set = MAX(g_angle_set, -20);
    g_angle_set = MIN(g_angle_set, 40);
}

void StopCar(void)
{
  int sum = 0;
  for (int i = 0; i < 188; i++)
  {
    if (image[DOWN_EAGE][i] < 50)sum++;
  }
  if (sum > 150)
    g_drive_flag = 0;
}