#include "GlobalVar.h"
#include "headfile.h"
#include "MK60_ftm.h"
#include "PID.h"
//------------电机管脚-------------------------------------------------









//-------------------------------------------------------------------------------------------------------------------
//  @brief      转换模式函数
//  @param      		
//  @param    		
//  @param     		 	
//  @return     			
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//读取20号扇区偏移0数据类型为uint32
//-------------------------------------------------------------------------------------------------------------------
void changemode()
{
  g_mode++; 
  if(g_mode>4) g_mode=1;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      模式函数
//  @param      mode  模式		
//  @param    		
//  @param     		 	
//  @return     			
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//读取20号扇区偏移0数据类型为uint32
//-------------------------------------------------------------------------------------------------------------------
void motormode(float mode)
{
  int a1=0,b1=0,c1=0,a2=0,b2=0,c2=0;
  if(1==mode)//直立
  {
    a1=1; a2=1;
    b1=0; b2=0;
    c1=0; c2=0;
  }
  if(2==mode)//方向+直立
  {
    a1=1; a2=1;
    b1=0; b2=0;
    c1=1; c2=-1;
    g_fSpeedControlOut = 0;
  }
  if(3==mode)//速度+方向+直立
  {
    a1=1; a2=1;
    b1=-1; b2=-1;
    c1=1; c2=-1;
    
  }

  g_fleft = a1 * g_AngleControlOut + c1 * g_fDirectionControlOut + b1 * g_fSpeedControlOut;
  g_fright = a2 * g_AngleControlOut + c2 * g_fDirectionControlOut + b2 * g_fSpeedControlOut;

  if(4==mode)//手调
  {
    g_fleft=g_duty_left;
    g_fright=g_duty_right; 
  }
 

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      占空比输出及限幅
//  @param      g_duty_PWMleft 	 输入左电机占空比	
//  @param      g_duty_PWMright	 输入右电机占空比		
//  @param     	 	
//  @return     			
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//读取20号扇区偏移0数据类型为uint32
//-------------------------------------------------------------------------------------------------------------------
void DrivePWM(float g_duty_PWMleft, float g_duty_PWMright)
{
  if(g_duty_PWMright>9999) 
  {
    g_duty_PWMright=9999;
  }
  if(g_duty_PWMright<-9999)
  {
    g_duty_PWMright=-9999;
  }
  if(g_duty_PWMleft>9999)
  {
    g_duty_PWMleft=9999;
  }
  if(g_duty_PWMleft<-9999)
  {
    g_duty_PWMleft=-9999;
  }
  if(g_duty_PWMright>=0&&g_duty_PWMleft>=0)
  {
      ftm_pwm_duty(ftm3, RIGHT_GO, 0);
      ftm_pwm_duty(ftm3, LEFT_GO, 0);
      ftm_pwm_duty(ftm3, RIGHT_BACK, (int)g_duty_PWMright);
      ftm_pwm_duty(ftm3, LEFT_BACK, (int)g_duty_PWMleft);
  }
  else if(g_duty_PWMright>=0&&g_duty_PWMleft<=0)
  {
    ftm_pwm_duty(ftm3,RIGHT_GO,0);
    ftm_pwm_duty(ftm3,RIGHT_BACK,(int)g_duty_PWMright);
    ftm_pwm_duty(ftm3,LEFT_GO,(int)(-g_duty_PWMleft));
    ftm_pwm_duty(ftm3,LEFT_BACK,0);
  }
  else if(g_duty_PWMright<=0&&g_duty_PWMleft>=0)
  {
    ftm_pwm_duty(ftm3,RIGHT_GO,(int)(-g_duty_PWMright));
    ftm_pwm_duty(ftm3,RIGHT_BACK,0);
    ftm_pwm_duty(ftm3,LEFT_GO,0);
    ftm_pwm_duty(ftm3,LEFT_BACK,(int)g_duty_PWMleft);
  }
  else if(g_duty_PWMright<=0&&g_duty_PWMleft<=0)
  {
    ftm_pwm_duty(ftm3,RIGHT_GO,(int)(-g_duty_PWMright));
    ftm_pwm_duty(ftm3,RIGHT_BACK,0);
    ftm_pwm_duty(ftm3,LEFT_GO,(int)(-g_duty_PWMleft));
    ftm_pwm_duty(ftm3,LEFT_BACK,0);
  }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      开关电机标志函数
//  @param      		
//  @param      		
//  @param     	 	
//  @return     			
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//读取20号扇区偏移0数据类型为uint32
//-------------------------------------------------------------------------------------------------------------------
void power()
{
  if(1==g_drive_flag) 
  {
    motormode(g_mode);
   DrivePWM(g_fleft,g_fright);
  }
  else if(0==g_drive_flag) 
  {
    DrivePWM(0,0);
  }
}
void on_off_flag()
{
  if(1==g_drive_flag) 
  {
   g_drive_flag = 0;
  }
  else if(0==g_drive_flag) 
  {
    g_drive_flag = 1;
    
  }
}

