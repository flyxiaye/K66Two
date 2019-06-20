#include "GlobalVar.h"
#include "headfile.h"
#include "SteerControl.h"

void CarControl(void)
{
  if (BlockFlag==2)        //·�Ͽ��Ƴ���
  {
    return;
  }
  else if (g_GetMeetingMaster)       //�ᳵ��
  {     
    return;
  }
  else
  {
    Ind_StopCar();//���ͣ��
    GetError();//��ȡ���
    CheckError();//����ͷ����У��
    SteerChangeFlag();//��־λУ�飬ȷ���������ģʽ
    ChangeSpeedFlag();//��־λУ�飬ȷ���ٶ�ģʽ
    GetSteerOut();//���������
    SteerControl();//�������
  }
}
//���ͣ��
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
//����ͷ����У��
void CheckError(void)
{
  if (1 == g_camera_open) //����ͷ����
  {
    if (3 == ErrorFlag)   //��Ұ��ʧ
    {
      if (1 == g_ind_open)  //��ſ���
      {
        if (30 > ind_left && 30 > ind_right)      //����
          g_drive_flag = 0;       //ͣ��
        else 
        {
          g_camera_ctrl_flag = 0;   //��������Ȩ
          g_ind_ctrl_flag = 1;
        }
      }
      else
      {
//        g_drive_flag = 0;       //ͣ��
        Error_1 = Error_2;
      }
    }
    else if (1 == ErrorFlag || 2 == ErrorFlag || 4 == ErrorFlag)
    {
//      if (1 == g_ind_open)  //��ſ���
//      {
//        if (30 > ind_left && 30 > ind_right)      //����
//          g_drive_flag = 0;       //ͣ��
//        else 
//        {
//          g_camera_ctrl_flag = 0;   //��������Ȩ
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
          g_camera_ctrl_flag = 1;   //��������Ȩ
          g_ind_ctrl_flag = 0;
      }
    }
  }
}
//ƫ��ֵ��ȡ
void GetError(void)
{
  if (1 == g_camera_open)   //����ͷ����
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
  if (1 == g_ind_open)    //��ſ���
  {
    ;
  }
}

//��־����
void SteerChangeFlag(void)
{
//  g_broken_enable = BrokenFlag_1 || BrokenFlag_2;
//  g_broken_enable = BrokenFlag_2;
  if (g_camera_open && g_ind_open)    //����ͷ���ͬʱ����
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
  else if (g_camera_open)   //������ͷ����
  {
    g_camera_ctrl_flag = 1;
    g_ind_ctrl_flag = 0;
  }
  else if (g_ind_open)      //����ſ���
  {
    g_camera_ctrl_flag = 0;
    g_ind_ctrl_flag = 1;
  }
  else          //��δ����
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

//���������
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
    if (20 == ind_left && 20 != ind_right)              //�Ҵ���
      SteerDuty = InitSteer - 120;
    else if (20 == ind_right && 20 != ind_left)         //�����
      SteerDuty = InitSteer + 120;
    else;
  }
  else;
  
  SteerDuty = MAX(SteerDuty, InitSteer - 120);
  SteerDuty = MIN(SteerDuty, InitSteer + 120);
}

//�������
void SteerControl(void)
{
  if (g_steer_open)
    ftm_pwm_duty(ftm3, ftm_ch5, SteerDuty);
}


