#include "GlobalVar.h"
#include "headfile.h"
#include "TurnTail.h"
#define _ANGLE imu_data.yaw
int StayDistance = 3000, TurntailDistance = 9000;
int TurnTailFlag = 0;
float flipgyro = 0,acc_Xpeed = 0;
void TurnTail()
{
//  flipgyro+= sensor.Gyro_deg.z * 0.002;
    static float lastangle,acc_Speed = 0, lastspeed, first, initangleset,number=0, initmode;
    static int TurnTail = 0, count = 0, initcameraopen, initindopen;
        if(2==Img_BrokenFlag&&g_mode==3&&g_drive_flag&&!number)
        {
          TurnTailFlag=1;
          number=1008611;
        }
    if (TurnTailFlag)
    {
      
        switch (TurnTail)
        {
        case 0:
          {
            acc_Speed+=curSpeed;
            
            AD_DirectionControl();
            if(acc_Speed>2000)
            {
              gpio_init(D2,GPO,0);
              acc_Xpeed=acc_Speed;
              initmode = g_mode;
                initcameraopen = g_camera_open;
                initindopen = g_ind_open;
                g_camera_open = 0;
                g_ind_open = 0;
                lastspeed = curSpeed;
                lastangle = _ANGLE;
                initangleset = g_angle_set;
                TurnTail = 1;
                acc_Speed = 0;
                flipgyro = 0;
            }
            break;
          }
        case 1:
          {
            count++;
            if(count<200)
            {
            g_mode=1;
            g_angle_set=14;
            }
            else if(count>=200&&count<1000)
            {
              g_drive_flag=0;
            }
            else if(count>=2000)
            {
              g_drive_flag=1;
//              g_mode=2;
              TurnTail=2;
              g_angle_set=-13;
//              AD_DirectionControl();
              count=0;
              flipgyro=0;
            }
            break;
          }
//        case 2:
//        {
//          g_drive_flag=1;
//            acc_Speed += curSpeed;
//            AD_DirectionControl();
//            gpio_init(D1, GPO, 0);
//            if (acc_Speed >= 500) 
//            {
//              initmode = g_mode;
//                initcameraopen = g_camera_open;
//                initindopen = g_ind_open;
//                g_camera_open = 0;
//                g_ind_open = 0;
//                lastspeed = curSpeed;
//                lastangle = _ANGLE;
//                initangleset = g_angle_set;
//                TurnTail = 2;
//                acc_Speed = 0;
//                flipgyro = 0;
//            }
//            break;
//        }
//        case 2:
//        {
////            //        if(2==BrokenFlag)
////            //        {
////            acc_Speed += curSpeed;
////            if (flipgyro >= -45)
////            {
////              
////                //  rightExpect=(1-(AngleError(_ANGLE,lastangle+90))/90)*lastangle;
////                //  leftExpect=lastangle;
////                // leftExpect=40;
////                // rightExpect=10;
////                //	        g_fDirectionControlOut_new+=AngleError(_ANGLE,lastangle)
////                g_mode=2;
////                g_angle_set=-13;
////                AngleControl();
////                flipgyro += sensor.Gyro_deg.z * 0.002;
////                g_fDirectionControlOut_new = -2500;
////                lastangle = _ANGLE;
////            }
////            else 
////              if (flipgyro <-45)
////            {
////                acc_Speed = 0;
////                g_mode=1;
////                TurnTail = 2;
////                g_fDirectionControlOut_new = 0;
////                lastangle = _ANGLE;
////                initangleset = _ANGLE;
////                count=0;
////                flipgyro = 0;
////            }
////            break;
//        }
        case 2:
        {
            //        if(2==BrokenFlag)
            //        {
            acc_Speed += curSpeed;
            if (flipgyro <= 90)
            {
              
                //  rightExpect=(1-(AngleError(_ANGLE,lastangle+90))/90)*lastangle;
                //  leftExpect=lastangle;
                // leftExpect=40;
                // rightExpect=10;
                //	        g_fDirectionControlOut_new+=AngleError(_ANGLE,lastangle)
                g_mode=2;
                g_angle_set=-13;
                AngleControl();
                flipgyro += sensor.Gyro_deg.z * 0.002;
                g_fDirectionControlOut_new = 2500;
                lastangle = _ANGLE;
            }
            else if (flipgyro > 90)
            {
                acc_Speed = 0;
                g_mode=1;
                TurnTail = 3;
                g_fDirectionControlOut_new = 0;
                lastangle = _ANGLE;
                initangleset = _ANGLE;
                count=0;
                flipgyro = 0;
            }
            break;
        }
        case 3:
          {
//            
            count++;
            if(count<50)
            {
              g_angle_set=14;
            }
            else if(count>=50)
            {
              g_drive_flag=0;
              TurnTail=4;
              count=0;
            }
            break;
          }
        case 4:
        {
            count++;
            if (count <= 300)
            {
                  g_drive_flag=0;
            }
            else if (count >300&&count<=1000)
            {
              g_angle_set=-6;
              g_mode=3;
              g_drive_flag=1;
              AngleControl();
              AD_DirectionControl();
              TurnTail=0;
              TurnTailFlag=0;
              Img_BrokenFlag=0;
            }
            else if(count>1000)
            {
              ;
            }
            break;
        }
//              g_mode=2;
//                 g_fDirectionControlOut=0;
//            }
//            else if(count>1000)
//            {
//
//                g_angle_set = -13;
//                TurnTail=3;
//                g_drive_flag=1;
//                flipgyro=0;
//            }
////            
//            break;
//        }
////         case 3:
////         {
////             flipgyro += sensor.Gyro_deg.z * 0.002;
//////           if(ind_mid!=4000)
//////           {
//////             gpio_init(D0,GPO,0);
////                g_fDirectionControlOut =-2000;
//////           }
////          if(ind_mid>=1000)
//////                   &&((ind_left_column-ind_right_column)<50||(ind_right_column-ind_left_column)<50))
////           {
////               g_fDirectionControlOut=0;
////               g_drive_flag=0;
////               TurnTailFlag=0;
////               TurnTail=0;
////           }  
////           else if(flipgyro<-80)
////           {
////               g_fDirectionControlOut=0;
////               g_drive_flag=0;
////               g_mode=3;
////               TurnTail=4;
////           }
////         }
////          case 4:
////          {
////              g_drive_flag=1;
////              acc_Speed+=curSpeed;
////            if(Img_BrokenFlag)
////            {
////              AD_DirectionControl();
////            }
////            else if(!Img_BlockFlag)
////            {
////                if(dialSwitchFlg2)
////                {
////                Camera_DirectionControl();
////                }
////                else
////                {
////                    AD_DirectionControl();
////                }
////                TurnTailFlag=0;
////                TurnTail=0;
////            }
////            if(acc_Speed>3000)
////            {
////              TurnTailFlag=0;
////                TurnTail=-1;
////            }
////              
////          }
//
//        //  }
//        //     acc_Speed += curSpeed;
//        //     if (flipgyro <= 0)
//        //     {
//        //         flipgyro += sensor.Gyro_deg.z * 0.002;
//        //         g_fDirectionControlOut_new = 8000;
//        //         lastangle = _ANGLE;
//        //     }
//        //     else if (flipgyro > 0)
//        //     {
//        //         acc_Speed = 0;
//        //         TurnTail = 4;
//        //         g_fDirectionControlOut_new = 0;
//        //         lastangle = _ANGLE;
//        //     }
//        //     break;
//        // }
////        case 3:
////        {
////            if (count <= 100)
////            {
////                g_mode = 1;
////                g_angle_set = 14;
////            }
////            count++;
////            if (count > 100 )
////            {
////                g_drive_flag = 0;
////                g_mode = 0;
////                TurnTail = 3;
////                count = 0;
////                g_angle_set = initangleset;
////                TurnTailFlag=0;
////                g_drive_flag=0;
////            }
////            break;
//        }
        }
    }
}