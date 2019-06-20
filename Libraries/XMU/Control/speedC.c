#include "GlobalVar.h"
#include "headfile.h"
#include "speedC.h"

int speed_sum = 0;
void SpeedGet(void)
{
      //�����ȡ
    rCurSpeed =-ftm_quad_get(ftm1);
    lCurSpeed = ftm_quad_get(ftm2);
    curSpeed = (lCurSpeed+rCurSpeed)>>1;
    ftm_quad_clean(ftm1);
    ftm_quad_clean(ftm2);
    speed_sum += curSpeed;
}

//=================================================================//
//  @brief:	���������ٶ�
//  @param: 	
//  @retirn: 
//  @note: 
//=================================================================//
void ExpectSpeedGet()
{
  switch(speed_type)
  {
    case 0:
          spdExp = spdExp0;    
          //g_speedUpFlg = 0;
          break;
    case 1:
          spdExp = spdExp1;   
          //g_speedUpFlg = 1;
          break;
    case 2:
          spdExp = spdExp2;    
         // g_speedUpFlg = 2;
          break; 
    case 3:
          spdExp = spdExp3; 
          //g_speedUpFlg = 3;
          break;
    case 4:
          spdExp = spdExp4; 
          //g_speedUpFlg = 4;
          break;
    default:
          spdExp = spdExp0;      
         // g_speedUpFlg = 0;
  }
}
//=================================================================//
//  @brief:	����pwm�������
//  @param: 	
//  @retirn: 
//  @note: 
//=================================================================//
void LeftPiCalc()
{
  float kdet =(float) KDet / 10000.0f ;
   leftExpect = (1- kdet*g_steer_error)*spdExp;  
  
  leftDrv.error_1 = leftExpect - lCurSpeed;

    leftDrv.p = g_speed_p;
    leftDrv.i = g_speed_i ;
    leftDrv.d = g_speed_d;
    
    leftDrv.d_duty = (int)(0.5 + leftDrv.p * (leftDrv.error_1 - leftDrv.error_2) + leftDrv.i * leftDrv.error_1 + leftDrv.d * (leftDrv.error_1 - 2 * leftDrv.error_2 + leftDrv.error_3));
    leftSpeedOut += leftDrv.d_duty;
    
    leftDrv.error_3 = leftDrv.error_2;
    leftDrv.error_2 = leftDrv.error_1;
    
    if (leftSpeedOut > SPEED_OUT_MAX)
      leftSpeedOut = SPEED_OUT_MAX;
    else if (leftSpeedOut < SPEED_OUT_MIN)
      leftSpeedOut = SPEED_OUT_MIN;
}
//=================================================================//
//  @brief:	����pwm�������
//  @param: 	
//  @retirn: 
//  @note: 
//=================================================================//
void RightPiCalc()
{
     float kdet =(float)KDet / 10000.0f;
   rightExpect = (1+kdet*g_steer_error)*spdExp;  
  
   rightDrv.error_1 = rightExpect - rCurSpeed;

    rightDrv.p = g_speed_p;
    rightDrv.i = g_speed_i ;
    rightDrv.d = g_speed_d;
    
    rightDrv.d_duty = (int)(0.5 + rightDrv.p * (rightDrv.error_1 - rightDrv.error_2) + rightDrv.i * rightDrv.error_1 + rightDrv.d * (rightDrv.error_1 - 2 * rightDrv.error_2 + rightDrv.error_3));
    rightSpeedOut += rightDrv.d_duty;
    
    rightDrv.error_3 = rightDrv.error_2;
    rightDrv.error_2 = rightDrv.error_1;
    
    if (rightSpeedOut > SPEED_OUT_MAX)
      rightSpeedOut = SPEED_OUT_MAX;
    else if (rightSpeedOut < SPEED_OUT_MIN)
      rightSpeedOut = SPEED_OUT_MIN;
  
}
//=================================================================//
// @brief:    ���pwm���
// @param: 	
//@retirn: 
// @note: 
//=================================================================//
void GiveDriveDuty(void)
{
    //����
    if(rightSpeedOut>=0)
    {   ftm_pwm_duty(ftm0, ftm_ch4,rightSpeedOut); //����
        ftm_pwm_duty(ftm0, ftm_ch5,0);
    }
    else
    {
       ftm_pwm_duty(ftm0, ftm_ch4,0);
       ftm_pwm_duty(ftm0, ftm_ch5,-rightSpeedOut);
    }
   //����
    if(leftSpeedOut>=0)
    {
       ftm_pwm_duty(ftm0, ftm_ch7,0);
       ftm_pwm_duty(ftm0, ftm_ch6,leftSpeedOut);//����
    }
    else 
    {
       ftm_pwm_duty(ftm0, ftm_ch7,-leftSpeedOut);//��
       ftm_pwm_duty(ftm0, ftm_ch6,0);
    }
}

void CleanTmp(void)
{
    leftSpeedOut = 0;
    rightSpeedOut = 0;
    leftDrv.error_1 = 0;
    leftDrv.error_2 = 0;
    leftDrv.error_3 = 0;
    leftDrv.d_duty = 0;
    rightDrv.error_1 = 0;
    rightDrv.error_2 = 0;
    rightDrv.error_3 = 0;
    rightDrv.d_duty = 0;
}


void SpeedControl(void)
{
  if(1==g_drive_flag)
  {
        SpeedGet();
        ExpectSpeedGet();
        LeftPiCalc();
        RightPiCalc();
        GiveDriveDuty();
 // last_speed = speed_type;
  }
  else if(0==g_drive_flag)
  {
    SpeedGet();
    CleanTmp();
    ftm_pwm_duty(ftm0, ftm_ch6,0);
    ftm_pwm_duty(ftm0, ftm_ch4,0);//��
    ftm_pwm_duty(ftm0, ftm_ch7,0);//����
    ftm_pwm_duty(ftm0, ftm_ch5,0);
//    DistantStop += curSpeed;
  }
}

//=================================================================//
//  @brief:	ͣ��������
//  @param: 	
//  @retirn: 
//  @note: 
//=================================================================//
void go_stop(void)
{
  static int nowrount = 0;                          //��ǰȦ��
  static int ntime = 0;
  if (g_drive_flag)
  {
    if (g_stop_line)              //��ʼͣ��
    {
      if (nowrount >= rount)
      {
//        g_handle_open = 0;
        speed_type = 4;
        nowrount = 0;
      }
    }
  
    if (g_stop_line)
      ntime++;
    else ntime = 0;
    if (ntime > 1000)
    {
      g_stop_line = 0;
      ntime = 0;
      ++nowrount;
    }
  }
  else
  {
    nowrount = 0;
    ntime = 0;
    g_stop_line = 0;
  }
}

//�Ӿ�ɲ��
void DelayDistantStop(void)
{
	static int GiveDistant = 2500;		//��Ϊ�趨�ӳپ���

	if (g_stop_dis)		//���յ��Ӿ�ɲ����־ ��ʼ�ۼ���ʻ����
		DistantStop += curSpeed;
	else DistantStop = 0;

	if (DistantStop >= GiveDistant)
	{
		g_stop_line = 1;	//ִ��ɲ��
		g_stop_dis = 0;
	}
	else;
}
  

