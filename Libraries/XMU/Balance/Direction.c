#include "headfile.h"
#include <GlobalVar.h>
#include "Basic.h"
#include "AD.h"


//================================================================//
//  @brief  :		方向环基本摄像头算法
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//

float g_error_before=0;
float g_fDirectionControlOut_new;
float g_fDirectionControlOut_before;


//void Camera_DirectionControl(void) 
//{
//  g_error_before = g_errorD;
//  g_errorD = 94 - ML[65];
//  g_fDirectionControlOut_before = g_fDirectionControlOut_new;
//  if(0 == g_errorD)
//  {
//    g_fDirectionControlOut_new = 0;
//  }
//  else
//  {
//    g_fDirectionControlOut_new = g_errorD * g_dire_P + ( g_errorD - g_error_before ) * g_dire_D;
//  }
//  
//}

void Camera_DirectionControl(void) 
{
  static int count = 0;
  if (count >= 5)
  {
    count = 0;
    static float g_fDirectionAngleControlOut = 0.0f;
    static float sensorGyroZLast = 0.0f;
    if (BlockFlag || g_GetMeetingMaster)                //路障控制
    {
      ;
    }
    else
    {
      g_errorD = 94 - ML[ProSpect];
      if (ErrorFlag)
        g_errorD = g_error_before;
    }
    g_fDirectionControlOut_before = g_fDirectionControlOut_new;
    if(0 == g_errorD)
    {
      g_fDirectionControlOut_new = 0;
    }
    else
    {
      g_fDirectionAngleControlOut = g_errorD * g_dire_P + ( g_errorD - g_error_before ) * g_dire_D;
      g_fDirectionControlOut_new = (g_fDirectionAngleControlOut - sensor.Gyro_deg.z) * gRateKp + (sensor.Gyro_deg.z - sensorGyroZLast) * gRateKd;
    }
    sensorGyroZLast = sensor.Gyro_deg.z;
    g_error_before = g_errorD;
  }
  else count++;
}


//================================================================//
//  @brief  :		方向环基本电感算法
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void AD_DirectionControl()
{
  ind_acq();
//  ind_error();
  g_errorD = (ind_right - ind_left) / (ind_right + ind_left) / 0.01;
  g_error_before = g_errorD;

  g_fDirectionControlOut_before = g_fDirectionControlOut_new;
  if(0 == g_errorD)
  {
    g_fDirectionControlOut_new = 0;
  }
  else
  {
    g_fDirectionControlOut_new = g_errorD * g_dire_P + ( g_errorD - g_error_before ) * g_dire_D;
  }
  
  
}

void AD_Direction(void)
{
  get_ind_error();
  g_fDirectionControlOut = g_dire_P * ad_error_1 + g_dire_D * (ad_error_1 - ad_error_2);
  ad_error_2 = ad_error_1;
  g_fDirectionControlOut = -g_fDirectionControlOut;
}

//================================================================//
//  @brief  :		方向环基本摄像头算法
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void DirectionControlOutput(void)//平滑输出
{
//   Camera_DirectionControl();
//  g_fDirectionControlOut=((g_fDirectionControlOut_new-g_fDirectionControlOut_before)*(g_nDirectionControlPeriod + 1)/g_DirectionPeriod+g_fDirectionControlOut_before);
  
    if(g_nDirectionControlPeriod <5)
   {
      g_fDirectionControlOut = (g_fDirectionControlOut_new-g_fDirectionControlOut_before)*(g_nDirectionControlPeriod + 1)/g_DirectionPeriod+g_fDirectionControlOut_before;
    }
   g_nDirectionControlPeriod++;
   if(g_nDirectionControlPeriod == 5)
   {
     g_nDirectionControlPeriod = 0;
   }
   if (BlockFlag || g_GetMeetingMaster) //限幅
   {
     g_fDirectionControlOut = MAX(g_fDirectionControlOut,-max_duty);
     g_fDirectionControlOut = MIN(g_fDirectionControlOut,max_duty);
   }

}

//float g_fL_add;
//float g_fL_sub;
//float g_error_before=0;
//float g_fDirectionControlOut_new;
//float g_fDirectionControlOut_before;
//float g_ADmax = 1600;
//float g_ADmin = 680;
////**********归一化***********
//float g_fL_left_normal_sum = 0;
//float g_fL_right_normal_sum = 0;
//float g_fL_mid_normal_sum = 0;
////*********滤波**************
//  int g_value_buf[3][10] = {0};//存放电感值
//
//float filter(int x);
//
//void Adc_DirectionControl(void) //电感采集（左+中+右）
//{
//  g_fL_left = ad_ave(ADC0_SE6,ADC_12bit,10);
//  g_fL_right = ad_ave(ADC0_SE7,ADC_12bit,10);
//  g_fL_mid = ad_ave(ADC0_SE8,ADC_12bit,10);
//  
//  
////  g_fL_left_normal = g_fL_left / g_fL_left_normal_avg;
////  g_fL_right_normal = g_fL_right / g_fL_right_normal_avg;
////  g_fL_mid_normal = g_fL_mid / g_fL_mid_normal_avg; 
//  
//  g_fL_left_normal = filter(0)/ g_fL_left_normal_avg;
//  g_fL_right_normal = filter(1) / g_fL_right_normal_avg;
//  g_fL_mid_normal = filter(2) / g_fL_mid_normal_avg;
//  
//  g_fL_add = (g_fL_left_normal + g_fL_right_normal) + g_fL_mid_normal;
//  g_fL_sub = (g_fL_left_normal - g_fL_right_normal);
//  if (g_fL_add < 0.1) g_fL_add = 0.1;
//  g_error_before = g_errorD;
//  g_errorD = g_fL_sub / g_fL_add * 100;
//  g_fDirectionControlOut_before = g_fDirectionControlOut_new;
//  if(0 == g_errorD)
//  {
//    g_fDirectionControlOut_new = 0;
//  }
//  else
//  {
//    g_fDirectionControlOut_new = g_errorD * g_dire_P + ( g_errorD - g_error_before ) * g_dire_D;
//  }
//}
//
//void DirectionControlOutput(void)//平滑输出
//{
//   DirectionControl();
//  g_fDirectionControlOut=(g_fDirectionControlOut_new-g_fDirectionControlOut_before)*(g_nDirectionControlPeriod + 1)/g_DirectionPeriod+g_fDirectionControlOut_before;
//}
//
//
//void normalization()//归一化
//{
//  if(g_normal_count <= 200)
//  {
//    g_fL_left_normal_sum += ad_ave(ADC0_SE6,ADC_12bit,10) / 200;
//    g_fL_right_normal_sum += ad_ave(ADC0_SE7,ADC_12bit,10) / 200;
//    g_fL_mid_normal_sum += ad_ave(ADC0_SE8,ADC_12bit,10) / 200;
//    g_normal_count ++;
//  }
//  if(201 == g_normal_count)
//  {
//    g_fL_left_normal_avg = g_fL_left_normal_sum;
//    g_fL_right_normal_avg = g_fL_right_normal_sum;
//    g_fL_mid_normal_avg = g_fL_mid_normal_sum;
//    g_normal_count ++;
//  }
//  if(202 == g_normal_count)
//  {
//    g_fL_left_normal_sum = 0;
//    g_fL_right_normal_sum = 0;
//    g_fL_mid_normal_sum = 0;
//
//  }
//}
//
//float filter(int x)//平均递推滤波
//{
//
//  int g_value_avg = 0;
//
//  int g_value_sum = 0;
// 
//  int i;
//    for(i = 0;i < 9;i++)
//    {
//      g_value_buf[x][i] = g_value_buf[x][i + 1];
//    }
//    if (x == 0)
//    {
//      g_value_buf[x][i] = ad_ave(ADC0_SE6,ADC_12bit,10);
//    }
//    if (x == 1)
//    {
//      g_value_buf[x][i] = ad_ave(ADC0_SE7,ADC_12bit,10);
//    }
//    if (x == 2)
//    {
//      g_value_buf[x][i] = ad_ave(ADC0_SE8,ADC_12bit,10);
//    }
//    for(i = 0;i < 9;i++)
//    {
//      g_value_sum += g_value_buf[x][i];
//    }
//    g_value_avg = 0.9 * g_value_buf[x][i] + 0.1 * g_value_sum;
//    return g_value_avg;
//
//}












