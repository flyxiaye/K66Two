#include "GlobalVar.h"
#include "headfile.h"
#include "AD.h"



void ind_acq(void)                         //电感采集
{
	//采集电感
        ind_left = collect(ADC1_SE9);
        ind_right = collect(ADC1_SE8);
//        ind_mid = adc_once(ADC0_SE6,ADC_16bit)
	//电感值限幅
	ind_left = MIN(ind_left, 3400);
	ind_left = MAX(ind_left, 20);
	ind_right = MIN(ind_right, 3400);
	ind_right = MAX(ind_right, 20);
//	ind_mid = MIN(ind_mid, 3400);
//	ind_mid = MAX(ind_mid, 20);
}

void ind_norm_maxmin(void)                //左右电感最大最小
{
	ind_leftmax = MAX(ind_leftmax, ind_left);
	ind_leftmin = MIN(ind_leftmin, ind_left);
	ind_rightmax = MAX(ind_rightmax, ind_right);
	ind_rightmin = MIN(ind_rightmin, ind_right);
//	ind_midmax = MAX(ind_midmax, ind_mid);
//	ind_midmin = MIN(ind_midmin, ind_mid);
}
void ind_norm(void)                           //得出归一值；
{
	left_norm = (float)((ind_left - ind_leftmin)) / (float)((ind_leftmax - ind_leftmin));
	right_norm = (float)((ind_right - ind_rightmin)) / (float)((ind_rightmax - ind_rightmin));
//	mid_norm = (float)((ind_mid - ind_midmin)) / (float)((ind_midmax - ind_midmin));
}

void get_ind_error(void)  //电感获取Error值 note:放在中断 如果电感开 则一直采集电感值 计算error；
{
	if (g_ad_flag == 1)
	{
                ind_acq();
                ind_norm();
		ad_error_1 = (left_norm - right_norm) / (left_norm + right_norm);
	}
        else if(0 == g_ad_flag)
        {
               ind_acq();
               ind_norm_maxmin();
               ind_norm();
        }
}

