#include "GlobalVar.h"
#include "headfile.h"
#include "TurnTail.h"
#define _ANGLE imu_data.yaw
int StayDistance = 3000, TurntailDistance = 9000;
int TurnTailFlag = 0;
float flipgyro = 0;
void TurnTail()
{
//  flipgyro+= sensor.Gyro_deg.z * 0.002;
    static float lastangle, lastspeed, first, initangleset, initmode;
    static int TurnTail = 0, acc_speed = 0, count = 0, initcameraopen, initindopen;
    if (TurnTailFlag == 1)
    {
        switch (TurnTail)
        {
        case 0:
        {
            acc_speed += curSpeed;
            if (acc_speed >= 2000)
            {
              initmode = g_mode;
                g_mode=1;
                initcameraopen = g_camera_open;
                initindopen = g_ind_open;
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
            if (flipgyro <= 180)
            {
              
                //  rightExpect=(1-(AngleError(_ANGLE,lastangle+90))/90)*lastangle;
                //  leftExpect=lastangle;
                // leftExpect=40;
                // rightExpect=10;
                //	        g_fDirectionControlOut_new+=AngleError(_ANGLE,lastangle)
                g_mode=2;
                flipgyro += sensor.Gyro_deg.z * 0.002;
                g_fDirectionControlOut_new = 3000;
                lastangle = _ANGLE;
            }
            else if (flipgyro > 180)
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
            count++;
            if (count <= 100)
            {
                g_mode = 1;
                g_angle_set = 14;
            }
            else if (count >100&&count<=200)
            {
              g_mode=6;
                 g_fDirectionControlOut=0;
            }
            else if(count>1000)
            {

                g_angle_set = initangleset;
                TurnTail=3;
                g_drive_flag=1;
                flipgyro=0;
            }
            
            break;
        }
         case 3:
         {
             flipgyro += sensor.Gyro_deg.z * 0.002;
//           if(ind_mid!=4000)
//           {
//             gpio_init(D0,GPO,0);
                g_fDirectionControlOut =-2000;
//           }
          if(ind_mid==4000)
//                   &&((ind_left_column-ind_right_column)<50||(ind_right_column-ind_left_column)<50))
           {
               g_fDirectionControlOut=0;
               g_drive_flag=0;
               TurnTailFlag=0;
               TurnTail=0;
           }  
           else if(flipgyro<-80)
           {
               g_fDirectionControlOut=0;
               g_drive_flag=0;
               g_mode=1;
               TurnTailFlag=0;
               TurnTail=0;
           }
         }
        //  case 4:
        //  {

        //  }
        //     acc_speed += curSpeed;
        //     if (flipgyro <= 0)
        //     {
        //         flipgyro += sensor.Gyro_deg.z * 0.002;
        //         g_fDirectionControlOut_new = 8000;
        //         lastangle = _ANGLE;
        //     }
        //     else if (flipgyro > 0)
        //     {
        //         acc_speed = 0;
        //         TurnTail = 4;
        //         g_fDirectionControlOut_new = 0;
        //         lastangle = _ANGLE;
        //     }
        //     break;
        // }
//        case 3:
//        {
//            if (count <= 100)
//            {
//                g_mode = 1;
//                g_angle_set = 14;
//            }
//            count++;
//            if (count > 100 )
//            {
//                g_drive_flag = 0;
//                g_mode = 0;
//                TurnTail = 3;
//                count = 0;
//                g_angle_set = initangleset;
//                TurnTailFlag=0;
//                g_drive_flag=0;
//            }
//            break;
//        }
        }
    }
}