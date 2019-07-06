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
	//    if (dialSwitchFlg3) g_StartSlave = 1;
	dialSwitchFlg6 = gpio_get(DIALSWITCH_PIN6);//
	dialSwitchFlg7 = gpio_get(DIALSWITCH_PIN7);//
	dialSwitchFlg8 = gpio_get(DIALSWITCH_PIN8);//
	Parameter();
	while (1)
	{
		dialSwitchFlg1 = gpio_get(DIALSWITCH_PIN1);//开大图
		dialSwitchFlg2 = gpio_get(DIALSWITCH_PIN2);//电磁
		dialSwitchFlg3 = gpio_get(DIALSWITCH_PIN3);//蜂鸣器
		dialSwitchFlg4 = gpio_get(DIALSWITCH_PIN4);//会车改变  1为掉头
		dialSwitchFlg5 = gpio_get(DIALSWITCH_PIN5);//控制参数FLASH

		//    ftm_pwm_duty(ftm0,RIGHT_GO,0);
		//    ftm_pwm_duty(ftm0,RIGHT_BACK,8000);
		//    ftm_pwm_duty(ftm0,LEFT_GO,0);
		//    ftm_pwm_duty(ftm0,LEFT_BACK,8000);
		if (mt9v032_finish_flag)//图像接收完成标志位
		{

			mt9v032_finish_flag = 0;//标志位清零 
			if (1 == g_handle_open)
			{
				GetML();
				MeetingToImage();
			}
			else;


		}
		if (1 == dialSwitchFlg1 && g_drive_flag == 0)
		{
			displayimage032(image[0]);
		}
		else
		{
			Menu();
		}
		//Bluebooth_Push_Data();
                UserData();
//    SendAngle();
		//SendData();
		if (AngleMutationFlag)
			gpio_init(D1, GPO, 0);
		else gpio_init(D1, GPO, 1);
		//		if (CircleFlag)
		if ((Img_RampFlag || CircleFlag || Img_BrokenFlag) && dialSwitchFlg3)
			gpio_init(A7, GPO, 1);
		else gpio_init(A7, GPO, 0);

	}
}