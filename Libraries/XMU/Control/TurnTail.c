#include "GlobalVar.h"
#include "headfile.h"
#include "TurnTail.h"
#define _ANGLE imu_data.yaw
int StayDistance = 3000, TurntailDistance = 9000;
int balabaflag = 0;
float flipgyro = 0;
void TurnTail()
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
  static float angle_init = 0;
  static int step = 0;
  static int integral = 0;
  
   switch(step)
   {
   case 0:
     {
       if(Img_BrokenFlag == 1 )
       {
         angle_init = g_angle_set;
         g_angle_set = angle_init + 3;
         step = 1;
       }
       break;
     }
   case 1:
     {
       if(Img_BrokenFlag == 2)
       {
         g_mode = 2;
         step = 2;
       }
       break;
     }
   case 2://�˴���Ҫ�رշ��򻷵�ƫ����㣬�������Ե���
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
       if(ABS(integral) >= 45 || !Img_BrokenFlag )
       {
         step = 0;
         integral = 0;
         g_angle_set = angle_init;
         g_mode = 5;
       }
     }
   }
=======
=======
>>>>>>> parent of 31a416a... 常神代码移植
=======
>>>>>>> parent of 31a416a... 常神代码移植
//  flipgyro+= sensor.Gyro_deg.z * 0.002;
    static float lastangle, lastspeed, first, initangleset, initmode;
    static int TurnTail = 0, acc_speed = 0, count = 0, initcameraopen, initindopen;
    if (balabaflag == 1)
    {
        switch (TurnTail)
        {
        case 0:
        {
            acc_speed += curSpeed;
            if (acc_speed >= 2000)
            {
                initcameraopen = g_camera_open;
                initindopen = g_ind_open;
                initmode = g_mode;
                g_camera_open = 0;
                g_ind_open = 0;
                lastspeed = curSpeed;
                lastangle = _ANGLE;
                initangleset = g_angle_set;
                TurnTail = 1;
                acc_speed = 0;
                flipgyro = 0;
            }
            break;
        }
        case 1:
        {
            //        if(2==BrokenFlag)
            //        {
            acc_speed += curSpeed;
            if (flipgyro < 180)
            {
                //  rightExpect=(1-(AngleError(_ANGLE,lastangle+90))/90)*lastangle;
                //  leftExpect=lastangle;
                // leftExpect=40;
                // rightExpect=10;
                //	        g_fDirectionControlOut_new+=AngleError(_ANGLE,lastangle);
                flipgyro += sensor.Gyro_deg.z * 0.002;
                g_fDirectionControlOut_new = 8000;
                lastangle = _ANGLE;
            }
            else if (flipgyro >= 180)
            {
                acc_speed = 0;
                TurnTail = 2;
                g_fDirectionControlOut_new = 0;
                lastangle = _ANGLE;
                initangleset = _ANGLE;
                flipgyro = 0;
            }
            break;
            //        }
        }
        case 2:
        {
            if (count <= 500)
            {
                g_mode = 1;
                g_angle_set = 30;
            }
            count++;
            if (count > 500 && imu_data.pit > 28)
            {
                g_mode = 3;
                TurnTail = 3;
                count = 0;
                g_angle_set = initangleset;
            }
            break;
        }
        case 3:
        {
            acc_speed += curSpeed;
            if (flipgyro <= 0)
            {
                flipgyro += sensor.Gyro_deg.z * 0.002;
                g_fDirectionControlOut_new = 8000;
                lastangle = _ANGLE;
            }
            else if (flipgyro > 0)
            {
                acc_speed = 0;
                TurnTail = 4;
                g_fDirectionControlOut_new = 0;
                lastangle = _ANGLE;
            }
            break;
        }
        case 4:
        {
            if (count <= 500)
            {
                g_mode = 1;
                g_angle_set = 30;
            }
            count++;
            if (count > 500 && imu_data.pit > 28)
            {
                g_drive_flag = 0;
                g_mode = 3;
                TurnTail = 3;
                count = 0;
                g_angle_set = initangleset;
            }
            break;
        }
        }
    }
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> parent of 31a416a... 常神代码移植
=======
>>>>>>> parent of 31a416a... 常神代码移植
=======
>>>>>>> parent of 31a416a... 常神代码移植
}