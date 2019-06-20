#include "GlobalVar.h"
#include "headfile.h"
#include "SteerControl.h"

void CarControl(void)
{
  if (BlockFlag==2)        //路障控制车辆
  {
    return;
  }
  else if (g_GetMeetingMaster)       //会车区
  {     
    return;
  }
  else
  {
    Ind_StopCar();//电磁停车
    GetError();//获取误差
    CheckError();//摄像头错误校验
    SteerChangeFlag();//标志位校验，确定舵机控制模式
    ChangeSpeedFlag();//标志位校验，确定速度模式
    GetSteerOut();//计算舵机输出
    SteerControl();//舵机控制
  }
}
//电磁停车
void Ind_StopCar(void)
{
  static int count = 0;
  if(1 == g_ind_open)
  {
    if (20 == ind_left && 20 == ind_right)
      count++;
    else count = 0;
    if (count > 20)
    {
      g_drive_flag = 0;
      count = 0;
    }
  }
}
//摄像头错误校验
void CheckError(void)
{
  if (1 == g_camera_open) //摄像头开启
  {
    if (3 == ErrorFlag)   //视野丢失
    {
      if (1 == g_ind_open)  //电磁开启
      {
        if (30 > ind_left && 30 > ind_right)      //出界
          g_drive_flag = 0;       //停车
        else 
        {
          g_camera_ctrl_flag = 0;   //交换控制权
          g_ind_ctrl_flag = 1;
        }
      }
      else
      {
//        g_drive_flag = 0;       //停车
        Error_1 = Error_2;
      }
    }
    else if (1 == ErrorFlag || 2 == ErrorFlag || 4 == ErrorFlag)
    {
//      if (1 == g_ind_open)  //电磁开启
//      {
//        if (30 > ind_left && 30 > ind_right)      //出界
//          g_drive_flag = 0;       //停车
//        else 
//        {
//          g_camera_ctrl_flag = 0;   //交换控制权
//          g_ind_ctrl_flag = 1;
//        }
//      }
//      else
      {
        Error_1 = Error_2;
      }
    }
    else 
    {
      if (1 == g_ind_open)
      {
          g_camera_ctrl_flag = 1;   //交换控制权
          g_ind_ctrl_flag = 0;
      }
    }
  }
}
//偏差值获取
void GetError(void)
{
  if (1 == g_camera_open)   //摄像头开启
  {
    if (ProSpect < ML_Count)
    {
      Error_1 = 94 - ML[ML_Count];
    }
    else 
    {
      Error_1 = 94 - ML[ProSpect];
    }
  }
  if (1 == g_ind_open)    //电磁开启
  {
    ;
  }
}

//标志调整
void SteerChangeFlag(void)
{
//  g_broken_enable = BrokenFlag_1 || BrokenFlag_2;
//  g_broken_enable = BrokenFlag_2;
  if (g_camera_open && g_ind_open)    //摄像头电磁同时开启
  {
    if (BrokenFlag==2)
    {
      g_camera_ctrl_flag = 0;
      g_ind_ctrl_flag = 1;
    }
    else if (BrokenFlag==1)
    {
      if (ML_Count > 50)
      {
        g_camera_ctrl_flag = 0;
        g_ind_ctrl_flag = 1;
      }
      else
      {
        g_camera_ctrl_flag = 1;
        g_ind_ctrl_flag = 0;
      }
    }
    else
    {
      g_camera_ctrl_flag = 1;
      g_ind_ctrl_flag = 0;
    }
  }
  else if (g_camera_open)   //仅摄像头开启
  {
    g_camera_ctrl_flag = 1;
    g_ind_ctrl_flag = 0;
  }
  else if (g_ind_open)      //仅电磁开启
  {
    g_camera_ctrl_flag = 0;
    g_ind_ctrl_flag = 1;
  }
  else          //均未开启
    g_drive_flag = 0;
}

void ChangeSpeedFlag(void)
{
  if (g_camera_ctrl_flag)
    speed_type = 1;
  else if (g_broken_enable || g_ind_ctrl_flag)
    speed_type = 2;
  else;
}

//计算舵机输出
void GetSteerOut(void)
{
  if (g_camera_ctrl_flag && g_handle_open)
  {
    SteerDuty = InitSteer + (int)(Steer_P * Error_1 + Steer_D * (Error_1 - Error_2) + 0.5);
    Error_2 = Error_1;
  }
  else if (g_ind_ctrl_flag)
  {
    SteerDuty = InitSteer + (int)(Steer_P * mul_ad_error* ad_error_1 + Steer_D * mul_ad_error * (ad_error_1 - ad_error_2) + 0.5);
    ad_error_2 = ad_error_1;
    if (20 == ind_left && 20 != ind_right)              //右打死
      SteerDuty = InitSteer - 120;
    else if (20 == ind_right && 20 != ind_left)         //左打死
      SteerDuty = InitSteer + 120;
    else;
  }
  else;
  
  SteerDuty = MAX(SteerDuty, InitSteer - 120);
  SteerDuty = MIN(SteerDuty, InitSteer + 120);
}

//舵机控制
void SteerControl(void)
{
  if (g_steer_open)
    ftm_pwm_duty(ftm3, ftm_ch5, SteerDuty);
}


