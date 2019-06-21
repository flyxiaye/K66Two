#include "GlobalVar.h"
#include "headfile.h"
#include "AD.h"



void ind_acq(void)                         //电感采集
{
	//采集电感

  ind_left_line = ad_ave(ADC1_SE8, ADC_12bit, 6);
  ind_left_column = ad_ave(ADC1_SE9, ADC_12bit, 6);
  ind_right_line = ad_ave(ADC0_SE13, ADC_12bit, 6);
  ind_right_column = ad_ave(ADC0_SE12, ADC_12bit, 6);
  ind_mid = ad_ave(ADC1_SE10, ADC_12bit, 6);

//        ind_mid = adc_once(ADC0_SE6,ADC_16bit)
	//电感值限幅
	ind_left_line = MIN(ind_left_line, 4000);
	ind_left_line = MAX(ind_left_line, 20);
        
        ind_left_column = MIN(ind_left_column, 4000);
	ind_left_column = MAX(ind_left_column, 20);
        
	ind_right_line = MIN(ind_right_line, 4000);
	ind_right_line = MAX(ind_right_line, 20);
        
        ind_right_column = MIN(ind_right_column, 4000);
	ind_right_column = MAX(ind_right_column, 20);
        
	ind_mid = MIN(ind_mid, 4000);
	ind_mid = MAX(ind_mid, 20);
}

void ind_norm_maxmin(void)                //左右电感最大最小
{
	ind_left_line_max = MAX(ind_left_line_max, ind_left_line);
	ind_left_line_min = MIN(ind_left_line_min, ind_left_line);
        
        ind_left_column_max = MAX(ind_left_column_max, ind_left_column);
	ind_left_column_min = MIN(ind_left_column_min, ind_left_column);
        
	ind_right_line_max = MAX(ind_right_line_max, ind_right_line);
	ind_right_line_min = MIN(ind_right_line_min, ind_right_line);
        
        ind_right_column_max = MAX(ind_right_column_max, ind_right_column);
	ind_right_column_min = MIN(ind_right_column_min, ind_right_column);
        
	ind_mid_max = MAX(ind_mid_max, ind_mid);
	ind_mid_min = MIN(ind_mid_min, ind_mid);
}
void ind_norm(void)                           //得出归一值；
{
  left_line_norm = (float)((ind_left_line - ind_left_line_min)) / (float)((ind_left_line_max - ind_left_line_min));
  left_column_norm = (float)((ind_left_column - ind_left_column_min)) / (float)((ind_left_column_max - ind_left_column_min));
  right_line_norm = (float)((ind_right_line - ind_right_line_min)) / (float)((ind_right_line_max - ind_right_line_min));
  right_column_norm = (float)((ind_right_column - ind_right_column_min)) / (float)((ind_right_column_max - ind_right_column_min));
  mid_norm = (float)((ind_mid - ind_mid_min)) / (float)((ind_mid_max - ind_mid_min));
}

void get_ind_error(void)  //电感获取Error值 note:放在中断 如果电感开 则一直采集电感值 计算error；
{
	if (g_ad_flag == 1)
	{
                ind_acq();
                ind_norm();
		ad_error_1 = (left_line_norm - right_line_norm) / (left_line_norm + right_line_norm);
	}
        else if(0 == g_ad_flag)
        {
               ind_acq();
               ind_norm_maxmin();
               ind_norm();
        }
}

unsigned char IndJudgeCircle(unsigned char type)
{
  return 1;
}

