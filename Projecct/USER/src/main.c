#include "headfile.h"
#include "MK60_uart.h"
#include "DFlash.h"
#include "Bluetooth.h"
#include  "isr.h"
#include  "Init.h"
#include  "Basic.h"
#include "DFLASH.h"
#include <GlobalVar.h>
#include  "keyboard.h"


int main(void)
{
    get_clk();     
    MyFlash_Read(0);
    system_Init();
    systick_delay_ms(1000);
//    dialSwitchFlg3 = gpio_get(DIALSWITCH_PIN3);
//    if (dialSwitchFlg3) g_StartSlave = 1;
    while(1)
  {
    dialSwitchFlg1 = gpio_get(DIALSWITCH_PIN1);
    dialSwitchFlg2 = gpio_get(DIALSWITCH_PIN2);
//    ftm_pwm_duty(ftm0,RIGHT_GO,0);
//    ftm_pwm_duty(ftm0,RIGHT_BACK,8000);
//    ftm_pwm_duty(ftm0,LEFT_GO,0);
//    ftm_pwm_duty(ftm0,LEFT_BACK,8000);
     if(mt9v032_finish_flag)//图像接收完成标志位
        {
            
            mt9v032_finish_flag = 0;//标志位清零 
            //Blackboader_Compensate();//黑边处理  
            //Start_Process(); 
          if (1 == g_handle_open)
          {
            GetML();
          }
          else;
          
            
	}
     //发车
//     if (g_StartMaster)
//     {
//       if (g_StartSlave)
//       {
//         g_flag = 1;
//         g_StartMaster = 0;
//          CircleFlag = 0;
//          CircleState = 0;
//          BrokenFlag = 0;
//          BlockFlag = 0;
//          RampFlag = 0;
//          g_handle_open = 1;
//       }
//     }
    if (1 == dialSwitchFlg1)
    {
//      DisplayBinary(ImageData[0]);
      
      displayimage032(image[0]);
      ShowEage();
    }
    else
         //Bluebooth_Push_Data();
//    SendAngle();
//    SendData();
//    if (!BlockFlag)
//      StopCar();
      Menu();
//         OLED_Write_Int(0,0,Key_Scan());
//      lcd_showint16(0,0,Key_Scan());
//    text_flag = Key_Check();
     StopCar();
  }
  
}
