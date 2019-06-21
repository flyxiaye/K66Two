#include "headfile.h"
#include "Meeting.h"

/*直到收到从车进入会车区标志，结束主车会车状态(g_GetMeetingMaster = 0);

*/
int StartDistance = 1000; //识别断路后延距距离
int StartSum = 0;               //距离积累量


//识别断路后改变会车状态标志位
//放在pit中断里延距改变
void ChangeStartMeetingFlag(void)
{
  if (BrokenFlag==2)             //进入断路区 
  {
    StartSum += curSpeed;
    if (StartSum > StartDistance)
    {
      g_GetMeetingMaster = 1;
      g_handle_open = 0;
      BrokenFlag = 0;
      BlockFlag = 0;
      StartSum = 0;
    }
  }
  else StartSum = 0;
}

//会车区车辆控制函数
//放在pit中断里
void MeetingCarControl(void)
{
  //会车结束状态，未进入会车区，不进入函数控制函数
  if (g_MeetingCtrlEndFlag || !g_GetMeetingMaster) return;
  
  //会车控制函数
//  MeetingCtrlFun();
//  gpio_init(D0,GPO,0);
//  MeetingCtrlFun_2();
  g_drive_flag = 0;
  
  //控制结束，给出结束标志位
  //g_MeetingCtrlEndFlag = 1;
}
//会车结束全局变量控制
//放在main或pit中断函数
void ChangeEndMeetingFlag(void)
{
  if(g_MeetingCtrlEndFlag)              //主车完成会车，准备离开会车区
  {
    if (!g_GetMeetingSlave)         //从车未到达断路区
    {
      g_drive_flag = 0; //停车
    }
    else                                //从车到达断路区，主车启动
    {
      g_drive_flag = 1;
      g_GetMeetingMaster = 0;
      g_GetMeetingSlave = 0;
      BrokenFlag = 2;
      g_handle_open = 1;
      g_MeetingCtrlEndFlag = 0;
    }
  }
}




void MeetingCtrlFun(void) // 会车区控制车辆函数1
{
  static int state = 0;
  static int acc_speed = 0;
  speed_type = 3;
  switch(state)
  {
  case 0:
    ftm_pwm_duty(ftm3, ftm_ch5, InitSteer - 120);
    spdExp3 = 15;
    acc_speed += curSpeed;
    if (acc_speed > sum_distance_1)
      state = 1;
    break;
  case 1:
    ftm_pwm_duty(ftm3, ftm_ch5, InitSteer + 120);
    spdExp3 = -15;
    acc_speed += curSpeed;
    if (acc_speed < (sum_distance_1 - sum_distance_2))
      state = 2;
    break;
  case 2:
    spdExp3 = 15;
    acc_speed += curSpeed;
    SteerDuty = InitSteer + (int)(Steer_P * mul_ad_error* ad_error_1 + Steer_D * mul_ad_error * (ad_error_1 - ad_error_2) + 0.5);
    ad_error_2 = ad_error_1;
    if (20 == ind_left && 20 == ind_right)
      SteerDuty = InitSteer;
    else if (20 == ind_left && 20 != ind_right)              //右打死
      SteerDuty = InitSteer - 120;
    else if (20 == ind_right && 20 != ind_left)         //左打死
      SteerDuty = InitSteer + 120;
    else;
    SteerDuty = MAX(SteerDuty, InitSteer - 120);
    SteerDuty = MIN(SteerDuty, InitSteer + 120);
    ftm_pwm_duty(ftm3, ftm_ch5, SteerDuty);
    if (acc_speed > sum_distance_3)
    {
      state = 0;
      g_MeetingCtrlEndFlag = 1;
      acc_speed = 0;
    }
    break;
  default:
    break;
  }
}
#define _ANGLE imu_data.yaw
void MeetingCtrlFun_1(void) // 会车区控制车辆函数2
{
  static int state = 0;
  static int acc_speed = 0;
  static float yaw_init = 0;
  static float yaw_init_1 = 0;
  static float yaw_init_2 = 0;
  static int get_flag = 0;
  speed_type = 3;
  if (!get_flag)
  {
    yaw_init = _ANGLE;
    yaw_init_1 = yaw_init + obj_angle_1;
    if (yaw_init_1 > 180)
      yaw_init_1 -= 360;
    yaw_init_2 = yaw_init + obj_angle_2;
    if (yaw_init_2 < -180)
      yaw_init_2 += 360;
    get_flag = 1;
  }
  
  switch (state)
  {
  case 0:
    g_errorD = const_error_1;
    if (_ANGLE > 0 && yaw_init_1 >= 0)
    {
      if (_ANGLE > yaw_init_1)
      {
        state = 1;
      }
    }
    else if (_ANGLE <= 0 && yaw_init_1 < 0)
    {
      if (_ANGLE > yaw_init_1)
      {
        state = 1;
      }
    }
    break;
  case 1:
    g_errorD = const_error_2;
    if (_ANGLE >= 0 && yaw_init_2 > 0)
    {
      if (_ANGLE < yaw_init_2)
      {
        state = 2;
      }
    }
    else if (_ANGLE < 0 && yaw_init_2 <= 0)
    {
      if (_ANGLE < yaw_init_2)
      {
        state = 2;
      }
    }
    break;
  case 2:
    g_errorD = 0;
    acc_speed += curSpeed;
    if (acc_speed > 2000)
    {
      state = 0;
      g_MeetingCtrlEndFlag = 1;
      acc_speed = 0;
      get_flag = 0;
    }
    break;
  default:
    break;
  }
}
    
void MeetingCtrlFun_2(void) // 会车区控制车辆函数2
{
  static int state = 0;
  static int acc_speed = 0;
  static float yaw_init = 0;
  static float yaw_init_1 = 0;
  static float yaw_init_2 = 0;
  static int get_flag = 0;
  speed_type = 3;
  if (!get_flag)
  {
    yaw_init = _ANGLE;
    yaw_init_1 = yaw_init + 180;
    if (yaw_init_1 > 180)
      yaw_init_1 -= 360;
  }
  
  switch (state)
  {
  case 0:
    g_errorD = AngleError(_ANGLE,yaw_init_1);
    if (g_errorD < 5 && g_errorD > -5)
      state = 2;
    break;
  case 1:
    g_errorD = const_error_2;
    if (_ANGLE >= 0 && yaw_init_2 > 0)
    {
      if (_ANGLE < yaw_init_2)
      {
        state = 2;
      }
    }
    else if (_ANGLE < 0 && yaw_init_2 <= 0)
    {
      if (_ANGLE < yaw_init_2)
      {
        state = 2;
      }
    }
    break;
  case 2:
    g_errorD = 0;
    acc_speed += curSpeed;
    if (acc_speed > 0)
    {
      state = 0;
      g_MeetingCtrlEndFlag = 1;
      acc_speed = 0;
      get_flag = 0;
    }
    break;
  default:
    break;
  }
}   
#undef _ANGLE 
    
      