#include "GlobalVar.h"
#include "headfile.h"
#include "TurnTail.h"
#define _ANGLE imu_data.yaw
#define _RATE_YAW sensor.Gyro_deg.y
void TurnTail()
{
  static float angle_init = 0;
  static int step = 0;
  static int error = 0;
  static int distance = 0;
  static int piece = 0;
  static float integral = 0;
  switch(step)
   {
   case 0:
     {
       if(Img_BrokenFlag)//关补图
       {
         angle_init = _ANGLE;
         stop_flag = 1;
         step = 1;
        
       }
       break;
     }
   case 1:
     {
       integral += _RATE_YAW * 0.002;
       if(Img_BrokenFlag == 2) 
       {
         distance += curSpeed;
         if(distance > 100 && deviation > piece)
         { 
           piece += 5;
           distance = 0;
//           g_errorD = piece;
         }
         if(deviation < piece)
         {
            piece = deviation;
            step = 2;
         }
         g_errorD = piece;

       }
       break;
     }
   case 2://此处需要关闭方向环的偏差计算，开启惯性导航
     {
       integral += _RATE_YAW * 0.002;
       error = 220 - integral;
//       error = AngleError(_ANGLE, angle_init + 220);
       if(error > 0 )
       {
         g_errorD = deviation;  
       } 
       else if(error <= 0 )
       {
         step = 3;
         error = 0;
//         piece = deviation;
       }
       break;
     }
   case 3:
     {
       integral += _RATE_YAW * 0.002;
       error = 260 - integral;
//       error = AngleError(_ANGLE, angle_init + 260);
       if(error > 0 && piece > 0)
       {
         piece -= 5;
       }
       if(error <= 10 || piece < 0)
       {
         piece = 0;
         step = 4;
         
       }
         g_errorD = piece;
       break;
     }
   case 4:
     {
       integral += _RATE_YAW * 0.002;
       error = 380 - integral;
//       error = AngleError(_ANGLE, angle_init + 380);
       if( -deviation < piece && Img_BrokenFlag )
       {
         piece -= 8;
       }
       if(-deviation > piece)
       {
         piece += 8;
       }
       g_errorD  = piece;
       if(error <= 0 || Img_BrokenFlag)
       {
         step = 0;
         error = 0;
         piece = 0; 
         integral = 0;
         angle_init = 0;
       }
        
     }
   }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      返回陀螺仪差值
//  @param      real_angle    //实际角度
//  @param      obj_angle  //目标角度
//  Sample usage:				
//------------------------------------------------------------------------------------------------------------------- 
float AngleError(float real_angle, float obj_angle)
{
  if (obj_angle > 180) obj_angle -= 360;
  else if (obj_angle < -180) obj_angle += 360;
  if (obj_angle - real_angle > 180)
  {
    return (obj_angle - 360 - real_angle);
  }
  else if (real_angle - obj_angle > 180)
  {
    return (obj_angle + 360 - real_angle);
  }
  else
  {
    return (obj_angle - real_angle);
  }
}