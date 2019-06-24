#include "headfile.h"
//

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
            g_AngleOut = (g_angle_set - imu_data.pit) * g_angle_P;//串级角度控制
            g_AngleControlOut = (g_AngleOut - (sensor.Gyro_deg.y)) * g_RateP - (sensor.Gyro_deg.y - g_gyro_y_last) * g_RateD;
            g_gyro_y_last = sensor.Gyro_deg.y;

//}
}