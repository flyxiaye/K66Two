#include "headfile.h"
//

float g_gyro_y_last = 0;
void AngleControl()//����
{
	
            g_AngleOut = (imu_data.pit - g_angle_set) * g_angle_P;//�����Ƕȿ���
            g_AngleControlOut = (g_AngleOut - (sensor.Gyro_deg.y)) * g_RateP - (sensor.Gyro_deg.y - g_gyro_y_last) * g_RateD;
            g_gyro_y_last = sensor.Gyro_deg.y;

}