#include "headfile.h"
//
//#define PI 3.1415926
//#define error_ratio 5
//#define angle_ratio 500
//#define T 0.002
//
//
//float g_gyro_sum_x_0 = 0,
//      g_gyro_sum_y_0 = 0,
//      g_gyro_sum_z_0 = 0;//À€÷√?
//float g_gyro_x_0 = 0,
//      g_gyro_y_0 = 0,
//      g_gyro_z_0 = 0;//ˆÌ’¢?
//float g_gyro_x = 0,
//      g_gyro_y = 0,
//      g_gyro_z = 0;//ˆÌ’¢?
//
//float g_acc_xy = 0;
////*********************OLD   Angle fusion***********
//void count_500()//À€÷√?
//{
//  if(g_count < 500)
//  {
//    g_gyro_sum_x_0 += icm_gyro_x;
//    g_gyro_sum_y_0 += icm_gyro_y;
//    g_gyro_sum_z_0 += icm_gyro_z;
//    g_count++;
//  }
//  else if (500 == g_count)
//  {
//    g_gyro_x_0 = g_gyro_sum_x_0 / g_count;
//    g_gyro_y_0 = g_gyro_sum_y_0 / g_count;
//    g_gyro_z_0 = g_gyro_sum_z_0 / g_count;
//    g_gyro_angle_x = g_acc_zy;
//    g_angle = g_acc_zy;
//
//    g_count++;
//  }
//}
//
//void Acc_AngleCalculate()// •·‹”¯
//{
//  if(0 == icm_acc_x)//
//  {
//    g_acc_zy = 90;
//  }
//  else
//  {
//    g_acc_zy =  atan2(icm_acc_y,icm_acc_z) * 180 / PI;
//    g_acc_zx =  atan2(icm_acc_x,icm_acc_z) * 180 / PI;
//    g_acc_xy =  atan2(icm_acc_x,icm_acc_y) * 180 / PI;
//  }
//}
//
//void Gyro_AngleCalculate()
//{
//  if(g_count > 500)
//  {
//    g_gyro_x = (icm_gyro_x - g_gyro_x_0)*250/4095*g_gyro_ratio;
//    g_gyro_angle_x += g_gyro_x * T;
//  }
//}
//
//void AngleCalculate()// «”¯Î◊˘Í
//{
//  count_500();
//  Acc_AngleCalculate();
//  Gyro_AngleCalculate();
//  g_angle_old = g_angle;
//  g_error = (g_acc_zy - g_angle_old) / error_ratio;
//  g_angle += (g_gyro_x + g_error) / angle_ratio;
//  
//}


//***********************PUTOUT**********/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*********
//void AngleControl()
//{
////  g_fleft = (g_angle_set - imu_data.pit) * g_angle_P + sensor.Gyro_deg.y*g_angle_D;
////  g_fright = (g_angle_set - imu_data.pit) * g_angle_P + sensor.Gyro_deg.y*g_angle_D;
//  g_AngleControlOut = (g_angle_set - imu_data.pit) * g_angle_P + sensor.Gyro_deg.y*g_angle_D;
//}
void DutyOut(void)
{
  if(g_drive_flag)
  {
    g_AngleControlOut = -g_AngleControlOut;
    if (g_AngleControlOut > 0)
    {
      ftm_pwm_duty(ftm3, RIGHT_GO, g_AngleControlOut);
      ftm_pwm_duty(ftm3, LEFT_GO, g_AngleControlOut);
      ftm_pwm_duty(ftm3, RIGHT_BACK, 0);
      ftm_pwm_duty(ftm3, LEFT_BACK, 0);
    }
    else 
    {
      ftm_pwm_duty(ftm3, RIGHT_GO, 0);
      ftm_pwm_duty(ftm3, LEFT_GO, 0);
      ftm_pwm_duty(ftm3, RIGHT_BACK, -g_AngleControlOut);
      ftm_pwm_duty(ftm3, LEFT_BACK, -g_AngleControlOut);
    }
  }
  else
  {
    ftm_pwm_duty(ftm3, RIGHT_GO, 0);
    ftm_pwm_duty(ftm3, LEFT_GO, 0);
    ftm_pwm_duty(ftm3, RIGHT_BACK, 0);
    ftm_pwm_duty(ftm3, LEFT_BACK, 0);
  }
}
    
//#define RIGHT_GO ftm_ch5  
//#define RIGHT_BACK ftm_ch4
//#define LEFT_GO ftm_ch6
//#define LEFT_BACK  ftm_ch7

float g_gyro_y_last = 0;
void AngleControl()//¥Æº∂
{
   g_AngleOut = (imu_data.pit - g_angle_set ) * g_angle_P;
   g_AngleControlOut = (g_AngleOut - (sensor.Gyro_deg.y)) * g_RateP + (sensor.Gyro_deg.y - g_gyro_y_last) * g_RateD;
   g_gyro_y_last = sensor.Gyro_deg.y;
//  g_AngleControlOut = -g_AngleControlOut;
}