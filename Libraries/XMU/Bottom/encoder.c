#include "MK60_ftm.h"
#include "encoder.h"
#include "GlobalVar.h"

int Encoder_get(void)
{
//  int data = 0;
//  data = ftm_quad_get(Encoder_FTM);          //��ȡFTM �������� ��������(������ʾ������)
//  ftm_quad_clean(Encoder_FTM);                   //�� FTM �������� ��������
//  return (data);
  EncoderL = ftm_quad_get(ftm1);
  EncoderR = ftm_quad_get(ftm2);
  ftm_quad_clean(ftm1);
  ftm_quad_clean(ftm2);
  return 0;
}