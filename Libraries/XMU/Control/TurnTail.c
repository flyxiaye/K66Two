#include "GlobalVar.h"
#include "TurnTail.h"
#include "mpu_prepare.h"

#define _ANGLE_YAW imu_data.yaw
#define _RATE_YAW sensor.Gyro_deg.z


int deviation = 0;//断路掉头固定偏差（第一段）
//================================================================//
//  @brief  :	平衡车掉头
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void TurnAround()
{
  static float angle_init = 0;
  static int step = 0;
  static int integral = 0;
  
   switch(step)
   {
   case 0:
     {
       if(BrokenFlag == 1 )
       {
         angle_init = g_angle_set;
         g_angle_set = angle_init + 3;
         step = 1;
       }
       break;
     }
   case 1:
     {
       if(BrokenFlag == 2)
       {
         g_mode = 2;
         step = 2;
       }
       break;
     }
   case 2://此处需要关闭方向环的偏差计算，开启惯性导航
     {
       if(ABS(integral) < 270)
       {
         g_errorD = deviation;
          integral += ABS(_RATE_YAW * 0.002);
       }
       else if(ABS(integral) >= 270)
       {
         step = 3;
         integral = 0;
       }
       break;
     }
   case 3:
     {
       g_errorD = -deviation;
        integral += ABS(_RATE_YAW * 0.002);
       if(ABS(integral) >= 180 || !BrokenFlag )
       {
         step = 0;
         integral = 0;
         g_angle_set = angle_init;
         g_mode = 5;
       }
     }
   }
}