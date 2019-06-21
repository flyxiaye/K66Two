//#include "Obstacle.h"
//#include <GlobalVar.h>
//#include "headfile.h"
//#include "PID.h"
//
////坡道识别
//void JudgeRamp(void)
//{
//  static int angle_init = 0;
//  if (0 == angle_init)
//    angle_init = imu_data.pit;
//  if (RampFlag == 2)
//  {
//    if (imu_data.pit - angle_init < 5 
//        && angle_init - imu_data.pit < 5)
//    {
//      RampFlag = 0;
//      CircleFlag = 0;
//      BrokenFlag = 0;
//      BlockFlag = 0;
//    }
//  }
//  else if (RampFlag == 1)
//  {
//    if (angle_init - imu_data.pit > 12)
//      RampFlag = 2;
//  }
//  else
//  {
//    if (imu_data.pit - angle_init > 12)
//    {
//      RampFlag = 1;
//      CircleFlag = 0;
//      BrokenFlag = 0;
//      BlockFlag = 0;
//    }
//  }
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      横断路障1 三拐
////  @param      camera_RB_flag    //摄像头识别横断路障标志位
////  @param      inf_RB_flag  //红外标志位
////  Sample usage:				
////-------------------------------------------------------------------------------------------------------------------
//void roadblock1()//int camera_RB_flag, int inf_RB_flag)
//{
////  spdExp0 = 10;
////  if(camera_RB_flag && infrared_RB_flag)
////  {
////    g_camera_open = 0;
////    g_ind_open = 0; 
////  }
////  SteerDuty = 1147;
//
//  sum += curSpeed;
//
//  if(sum < 100 * k1)
//  {
//      SteerDuty = 1147 + 120;
////      rightDrv.d_duty = rightDrv.d_duty * 1.1;
//
//  }
//  if(sum > 100 * k1 && sum < 100 * k2)
//  {
//      SteerDuty = 1147 - 120;
////      leftDrv.d_duty = leftDrv.d_duty * 1.1;
//
//  }
//  if(100 * k2 < sum && sum < 100 * k3)
//  {
//      SteerDuty = 1147;
//
//  }
//  if(100 * k3 < sum  && sum < 20000)
//  {
//    SteerDuty = 1147 - 120;
////    leftDrv.d_duty = leftDrv.d_duty * 1.1;
//  }
//  if(sum > 20000)
//  {
//    g_drive_flag = 0;
//    inf_RB_flag = 0;
//  }
////  if(100 * k4 < sum && sum < 100 * k5)
////  {
////    SteerDuty = 1147 + 120;  
////  }
////  if(100 * k5 < sum )
////  {
////    SteerDuty = 1147;  
////  }
//  ftm_pwm_duty(ftm3, ftm_ch5, SteerDuty);
//}
////-------------------------------------------------------------------------------------------------------------------
////  @brief      红外
////  @param      camera_RB_flag    //摄像头识别横断路障标志位
////  @param      inf_RB_flag  //红外标志位
////  @return     0 //处于横断路障；1//出横断路障 
////  Sample usage:				
////-------------------------------------------------------------------------------------------------------------------
//void inf(void)
//{
//  if (!BlockFlag && !g_GetMeetingMaster && !RampFlag && BrokenFlag != 2 && g_drive_flag && !CircleFlag)
//  {
//    g_inf = collect(ADC1_SE17);
//    if(g_inf > stop_inf * 10)
//    {
//      BlockFlag = 1;
//    }
//  }
//  if (BlockFlag==1)
//  {
//      BlockFlag = 2;
//      g_handle_open = 0;
//      BrokenFlag = 0;
//  }
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      横断路障2 两拐
////  @param      camera_RB_flag    //摄像头识别横断路障标志位
////  @param      inf_RB_flag  //红外标志位
////  Sample usage:				
////-------------------------------------------------------------------------------------------------------------------
//void roadblock2(void)
//{
//  if (BlockFlag==2)
//  {
//    speed_type = 3;
//    sum += curSpeed;
//    if(sum < 100 * s1)
//    {
//        SteerDuty = InitSteer + 120;
//    }
//    else if(sum < sum_dist * 10)
//    {     
//        SteerDuty = InitSteer - st;
//        if (ind_left > 100 && ind_right > 100)
//          sum = sum_dist*10;
//    }
//    else
//    {
//        SteerDuty = InitSteer + st;
//        ObstacleEndFlag = 1;
//        sum = 0;
//    }
//    ftm_pwm_duty(ftm3, ftm_ch5, SteerDuty);
//  }
//  if (ObstacleEndFlag)
//  {
//    speed_type = 1;
//    BlockFlag = 0;
//    ObstacleEndFlag = 0;
//    
//    CircleFlag = 0;
//    CircleState = 0;
//    BrokenFlag = 0;
//    if(1 == g_camera_open)
//      g_handle_open = 1;
//    g_steer_open = 1;
//  }
//}
//
//#define _ANGLE imu_data.yaw
////-------------------------------------------------------------------------------------------------------------------
////  @brief      横断路障2 两拐 陀螺仪
////  @param      camera_RB_flag    //摄像头识别横断路障标志位
////  @param      inf_RB_flag  //红外标志位
////  Sample usage:				
////-------------------------------------------------------------------------------------------------------------------
//void roadblock3(void)
//{
//  if (BlockFlag==2)
//  {
//    static pidInit_t SteerInit = {2,0,0};
//    static PidObject Steer;
//    static int state = 0;
//    static int yaw_init = 0;
//    static int yaw_obj = 0;
//    static int get_flag = 0;
////    static int sum_speed = sum_dist;
//    if (!get_flag)
//    {
//      yaw_init = _ANGLE;
//      get_flag = 1;
//      yaw_obj = yaw_init - st;
//      speed_type = 3;
//    }
//    sum += curSpeed;
//    switch (state)
//    {
//    case 0:
//      g_errorD = AngleError(_ANGLE, yaw_obj);
//      if (sum > sum_dist)
//      {
//        state = 1;
//        yaw_obj = yaw_init + st;
//      }
//      break;
//    case 1:
//      g_errorD = AngleError(_ANGLE, yaw_obj);
//      if (sum > (sum_dist<<1))
//      {
//        state = 0;
//        sum = 0;
//        get_flag = 0;
//        BlockFlag = 0;
//        CircleFlag = 0;
//        CircleState = 0;
//        BrokenFlag = 0;
//        if(1 == g_camera_open)
//          g_handle_open = 1;
//      }
//      break;
//    default:
//      break;
//    } 
//  }
//}
//      
//#undef _ANGLE  
////-------------------------------------------------------------------------------------------------------------------
////  @brief      陀螺仪比较大小(非大即小)
////  @param      real_angle    //实际角度
////  @param      obj_angle  //目标角度
////  Sample usage:				
////------------------------------------------------------------------------------------------------------------------- 
//int JudgeMaxAngle(float real_angle, float obj_angle)
//{
//  if (obj_angle > 180) obj_angle -= 360;
//  else if (obj_angle < -180) obj_angle += 360;
//  if (real_angle > 0 && obj_angle >= 0 && real_angle > obj_angle)
//    return 1;
//  else if (real_angle <= 0 && obj_angle < 0 && real_angle > obj_angle)
//    return 1;
//  else return 0;
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      返回陀螺仪差值
////  @param      real_angle    //实际角度
////  @param      obj_angle  //目标角度
////  Sample usage:				
////------------------------------------------------------------------------------------------------------------------- 
//float AngleError(float real_angle, float obj_angle)
//{
//  if (obj_angle > 180) obj_angle -= 360;
//  else if (obj_angle < -180) obj_angle += 360;
//  if (obj_angle - real_angle > 180)
//  {
//    return (obj_angle - 360 - real_angle);
//  }
//  else if (real_angle - obj_angle > 180)
//  {
//    return (obj_angle + 360 - real_angle);
//  }
//  else
//  {
//    return (obj_angle - real_angle);
//  }
//}