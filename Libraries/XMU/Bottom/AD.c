#include "GlobalVar.h"
#include "headfile.h"
#include "AD.h"



void ind_acq(void)                         //��вɼ�
{
	//�ɼ����
        ind_left = collect(ADC1_SE9);
        ind_right = collect(ADC1_SE8);
//        ind_mid = adc_once(ADC0_SE6,ADC_16bit)
	//���ֵ�޷�
	ind_left = MIN(ind_left, 3400);
	ind_left = MAX(ind_left, 20);
	ind_right = MIN(ind_right, 3400);
	ind_right = MAX(ind_right, 20);
//	ind_mid = MIN(ind_mid, 3400);
//	ind_mid = MAX(ind_mid, 20);
}

void ind_norm_maxmin(void)                //���ҵ�������С
{
	ind_leftmax = MAX(ind_leftmax, ind_left);
	ind_leftmin = MIN(ind_leftmin, ind_left);
	ind_rightmax = MAX(ind_rightmax, ind_right);
	ind_rightmin = MIN(ind_rightmin, ind_right);
//	ind_midmax = MAX(ind_midmax, ind_mid);
//	ind_midmin = MIN(ind_midmin, ind_mid);
}
void ind_norm(void)                           //�ó���һֵ��
{
	left_norm = (float)((ind_left - ind_leftmin)) / (float)((ind_leftmax - ind_leftmin));
	right_norm = (float)((ind_right - ind_rightmin)) / (float)((ind_rightmax - ind_rightmin));
//	mid_norm = (float)((ind_mid - ind_midmin)) / (float)((ind_midmax - ind_midmin));
}

void get_ind_error(void)  //��л�ȡErrorֵ note:�����ж� �����п� ��һֱ�ɼ����ֵ ����error��
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

