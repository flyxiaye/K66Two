#include "MK60_ftm.h"
#include "encoder.h"
#include "GlobalVar.h"

int Encoder_get(void)
{
//  int data = 0;
//  data = ftm_quad_get(Encoder_FTM);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
//  ftm_quad_clean(Encoder_FTM);                   //清 FTM 正交解码 的脉冲数
//  return (data);
  EncoderL = ftm_quad_get(ftm1);
  EncoderR = ftm_quad_get(ftm2);
  ftm_quad_clean(ftm1);
  ftm_quad_clean(ftm2);
  return 0;
}