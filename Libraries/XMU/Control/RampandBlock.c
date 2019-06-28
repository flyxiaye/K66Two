#include "GlobalVar.h"
#include "headfile.h"
#include "RampandBlock.h"
void Ramp()
{
  static int acc_speed=0,rampflag=0,initangleset;
  
  if(gpio_get(C8)==0&&g_drive_flag&&!rampflag)
  {
    initangleset=g_angle_set;
    g_angle_set=initangleset-13;
    g_mode=7;
    rampflag=1;
  }
  if(rampflag)
  {
    acc_speed+=curSpeed;
    if(acc_speed>4000&&gpio_get(C8)==1)
    {
      rampflag=0;
      g_mode=3;
      g_angle_set=initangleset;
    }
  }
}