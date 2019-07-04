#include "headfile.h"
#include <GlobalVar.h>

float g_gyro_y_last = 0;
void AngleControl()//串级
{
	
//         if(!BrokenFlag && !get_flag)
//         {
//           g_angle_set_init = g_angle_set;
//         }
//        if(BrokenFlag == 1 && g_StateSlave < 3 && g_SlaveOutFlag != 1 && !get_flag)
//        {
//          get_flag = 1;
//          g_angle_set = g_angle_set_init - 3;
//        }
//        else if(BrokenFlag == 2 && get_flag)
//        {
//          g_angle_set = g_angle_set_init;
//          get_flag = 0;
//        }
            if(BootRacerFlag)
            {
            g_AngleOut = (imu_data.pit-g_angle_set) * g_angle_P;//串级角度控制
            }
            g_AngleControlOut = (g_AngleOut - (sensor.Gyro_deg.y)) * g_RateP - (sensor.Gyro_deg.y - g_gyro_y_last) * g_RateD;
            g_gyro_y_last = sensor.Gyro_deg.y;
            
                 if(7==g_mode)
                 {
                   g_AngleControlOut=(imu_data.pit-g_angle_set  )*g_angle_Psingle - sensor.Gyro_deg.y* g_angle_D;
                 }
                  g_AngleControlOut = MAX(g_AngleControlOut, -9000);
		g_AngleControlOut = MIN(g_AngleControlOut, 9000);

}
//================================================================//
//  @brief  :		角度跳变检测
//  @param  :		void
//  @return :		
//  @note   :		AngleMutationFlag为1正常，为0异常
//================================================================//
void AngleMutation()
{
  static int count=0,number=0,EveryMutationFlag=1;
  static float angle_pitbefore[9]={0},angle_pitnow[9]={0};
  count++;
  if(count>5&&number<=9)
  {
    angle_pitbefore[number]=imu_data.pit;
    number++;
    count=0;
  }
  else if(count>5&&number<=19)
  {
    angle_pitnow[9]=imu_data.pit;
    number++;
    count=0;
  }
  else if(20==number)
  {
    for (int i = 0; i < 10; i++)
    {
      if(angle_pitbefore[i]>angle_pitnow[i+10])
      {
        if(angle_pitbefore[i]-angle_pitnow[i+10]<=3)
        {
          EveryMutationFlag=1;
        }
        else
        {
          EveryMutationFlag=0;
          AngleMutationFlag=0;
        }
        
      }
      else if(angle_pitbefore[i]<angle_pitnow[i+10])
      {
        if(angle_pitnow[i+10]-angle_pitbefore[i]<=3)
        {
          EveryMutationFlag=1;
        }
        else
        {
          EveryMutationFlag=0;
          AngleMutationFlag=0;
        }
      }
    }
  }
  if(EveryMutationFlag&&20==number)
  {
    AngleMutationFlag=1;
  }
}