#include "headfile.h"
#include "math.h"
#include <GlobalVar.h>
/***************环岛识别***************/
void circleland()
{
  static int LRflag = 0; //1左边大 2右边大
                         //          if(circlelandnumber==2)
                         //          {
                         //            return;
                         //          }
  if ((left_column_norm > 0.3 || right_column_norm > 0.3) && (abs(left_column_norm - right_column_norm) > 0.4 * MAX(left_column_norm, right_column_norm)) && (ABS(left_line_norm) > 0.3 || ABS(right_line_norm) > 0.3) && mid_norm > 0.8 && 0 == circlelandflag)
  {
    if (left_column_norm > right_column_norm)
    {
      LRflag = 1;
      circlelandflag = 1;
      circlelandSigma = 0;
    }
    if (left_column_norm < right_column_norm)
    {
      LRflag = 2;
      circlelandflag = 1;
      circlelandSigma = 0;
    }
  }
  if (1 == circlelandflag)
  {
    Circlelanderror = 0;
    circlelandtimecount++;

    if ((ABS(left_column_norm) > 0.3 || ABS(right_column_norm) > 0.3) && circlelandtimecount < 1000 && 1 == LRflag && circlelandSigma > 1000)
    {
      if (left_column_norm > right_column_norm)
      {
        LRflag = 0;
        circlelandflag = 2;
        circlelandSigma = 0;
        circlelandtimecount = 0;
        circlelandsymbol = 0;
      }
      else if ((ABS(left_column_norm) < 0.3 && ABS(right_column_norm) < 0.3) && circlelandtimecount > 1000 && 1 == LRflag && circlelandSigma > 1000)
      {
        LRflag = 0;
        circlelandflag = 0;
        circlelandSigma = 0;
        circlelandtimecount = 0;
        circlelandsymbol = 0;
      }
    }
    else if ((ABS(left_column_norm) > 0.3 || ABS(right_column_norm) > 0.3) && circlelandtimecount < 1000 && 2 == LRflag && circlelandSigma > 1000)
    {
      if (left_column_norm < right_column_norm)
      {
        LRflag = 0;
        circlelandflag = 2;
        circlelandSigma = 0;
        circlelandtimecount = 0;
        circlelandsymbol = 0;
      }
      else if ((ABS(left_column_norm) < 0.3 && ABS(right_column_norm) < 0.3) && circlelandtimecount > 1000 && 2 == LRflag && circlelandSigma > 1000)
      {
        LRflag = 0;
        circlelandflag = 0;
        circlelandSigma = 0;
        circlelandtimecount = 0;
        circlelandsymbol = 0;
      }
    }

    else if (circlelandtimecount >= 3000 || circlelandSigma >= 5000)
    {
      LRflag = 0;
      circlelandflag = 0;
      circlelandSigma = 0;
      circlelandtimecount = 0;
      circlelandsymbol = 0;
    }
  }
  //
  //		if(2==circlelandflag)
  //		{
  //
  //			circlelandtimecount++;
  //			if(circlelandSigma>2000)
  //			{
  //
  //
  //				circlelandflag=3;
  //                                circlelandSigma=0;
  //                                circlelandtimecount=0;
  //
  //			}
  //                }
  if (2 == circlelandflag)
  {
    if (ABS(left_column_norm) > 0.3 || ABS(right_column_norm) > 0.3)
    {
      if (circlelandSigma > 2000)
      {
        //                                  if(ABS(left_column_norm)<0.3&&ABS(right_column_norm)<0.3)
        //                          {
        //                            circlelandflag=0;
        //                                circlelandSigma=0;
        //                                circlelandtimecount=0;
        //                          }
        //                          else
        //                          {
        circlelandflag = 3;
        circlelandSigma = 0;
        circlelandtimecount = 0;
        LRflag = 0;
        //                          }
      }
    }
    else
    {
      if (circlelandSigma > 5000)
      {
        circlelandflag = 0;
        circlelandSigma = 0;
        circlelandtimecount = 0;
        LRflag = 0;
      }
    }
    circlelandtimecount++;
    if (circlelandtimecount >= 1000)
    {
      circlelandsymbol = 0;
      circlelandflag = 0;
      circlelandSigma = 0;
      circlelandtimecount = 0;
      LRflag = 0;
    }
  }
  if (3 == circlelandflag)
  {
    if (0 == circlelandsymbol)
    {
      if (circlelandSigma < 4500)
      {
        Circlelanderror = 0.8 * (((float)(left_column_norm) - (float)(right_column_norm)) / ((float)(left_column_norm) + (float)(right_column_norm)));
        //                  if(Circlelanderror<0.3)
        //                  {
        //                    Circlelanderror=Circlelanderror*1.5;
        //                  }
      }
      if ((ABS(left_line_norm) > 0.3 || ABS(right_line_norm) > 0.3) && circlelandSigma > 20000)
      {
        //                    if(circlelandSigma>15000)
        //                    {
        circlelandsymbol = 1;
        Circlelanderror = 0;
        //                    }
      }
      if ((ABS(left_line_norm) > 0.3 || ABS(right_line_norm) > 0.3) && circlelandSigma > 30000)
      {
        //                    if(circlelandSigma>15000)
        //                    {
        circlelandsymbol = 2;
        Circlelanderror = 0;
        //                    }
      }
      //                    if((ABS(left_line_norm)>0.3||ABS(right_line_norm)>0.3)&&circlelandSigma>9000)
      //                   {
      //                     Circlelanderror=0;
      //                   }
    }
    circlelandtimecount++;

    if (circlelandtimecount >= 20000)
    {
      circlelandsymbol = 0;
      circlelandflag = 0;
      circlelandSigma = 0;
      circlelandtimecount = 0;
    }
    if ((ABS(left_column_norm) > 0.3 || ABS(right_column_norm) > 0.3) && mid_norm > 0.8 && (abs(left_column_norm - right_column_norm) > 0.4 * MAX(left_column_norm, right_column_norm)))
    {
      if (circlelandSigma > 10000)
      {
        LRflag = 0;
        circlelandflag = 4;
        circlelandSigma = 0;
        circlelandtimecount = 0;
      }
    }
  }
  if (4 == circlelandflag)
  {
    if (left_column_norm > 0.3 || right_column_norm > 0.3)
    {
      if (circlelandSigma > 2000 && 1 == circlelandsymbol)
      {
        //                                            if(ABS(left_column_norm)<0.3&&ABS(right_column_norm)<0.3)
        //                          {
        //                            circlelandsymbol=0;
        //                            circlelandflag=0;
        //                                circlelandSigma=0;
        //                                circlelandtimecount=0;
        //                          }
        //                              else
        //                              {
        circlelandflag = 5;
        circlelandSigma = 0;
        circlelandtimecount = 0;
        circlelandsymbol = 0;
        //                              }
      }
      else if (circlelandSigma > 3000 && 2 == circlelandsymbol)
      {
        circlelandflag = 5;
        circlelandSigma = 0;
        circlelandtimecount = 0;
        circlelandsymbol = 0;
      }
    }
    circlelandtimecount++;
    if (circlelandtimecount >= 3000)
    {
      circlelandsymbol = 0;
      circlelandflag = 0;
      circlelandSigma = 0;
      circlelandtimecount = 0;
    }
  }
  if (5 == circlelandflag)
  {
    circlelandtimecount++;
    if (circlelandSigma < 7000)
    {
      //                  Circlelanderror=-0.7*(((float)(left_column_norm)-(float)(right_column_norm))/((float)(left_column_norm)+(float)(right_column_norm)));
      Circlelanderror = -0.9 * (((float)(left_line_norm) - (float)(right_line_norm)) / ((float)(left_line_norm) + (float)(right_line_norm)));
    }

    else if (ABS(left_column_norm) < 0.4 && ABS(right_column_norm) < 0.4 && circlelandtimecount > 1000)
    {
      if (circlelandSigma > 4000)
      {
        circlelandflag = 0;
        circlelandnumber++;
        Circlelanderror = 0;
        circlelandSigma = 0;
        circlelandtimecount = 0;
      }
    }
  }
}

void circleland2() //摄像头识别  电磁冲锋
{
  static int Lflag = 0, Rflag = 0, entercircleland = 0, Iscirclelandflag,bigflag=0;smallflag=0;
  static int acc_speed = 0;
  if (CL == ImgJudgeCircle())
  {
    Lflag = 1;
    Rflag = 0;
    circlelandflag = 1;
  }
  else if (CR == ImgJudgeCircle())
  {
    Lflag = 0;
    Rflag = 1;
    circlelandflag = 1;
  }
  else if (CN == ImgJudeCircle)
  {
    Lflag = 0;
    Rflag = 0;
    circlelandflag = 0;
  }
  if ((Lflag || Rflag) && mid_norm > 1.3 && !Iscirclelandflag && circlelandflag)
  {
    Iscirclelandflag = 1;
    circlelandflag = 2;
  }
  else if ((Lflag || Rflag) && mid_norm < 1.3 && !Iscirclelandflag && circlelandflag)
  {
    acc_speed += curSpeed;
    if (acc_speed > 4000)
    {
      circlelandflag = 0;
      Iscirclelandflag = 0;
      Lflag = 0;
      Rflag = 0;
      acc_speed = 0;
    }
  }
  if (!dialSwitchFlg2 && 2 == circlelandflag && Iscirclelandflag && Lflag && left_column_norm > 0.1 && left_line_norm > right_line_norm)
  {
    acc_speed += curSpeed;
    g_errorD = (left_line_norm + left_column_norm - right_line_norm) / (right_line_norm + left_line_norm) * 100;
    if (acc_speed > 3000)
    {
      circlelandflag = 3;
    }
  }
  else if (!dialSwitchFlg2 && 2 = circlelandflag && Iscirclelandflag && Rflag && right_column_norm > 0.1 && right_line_norm > left_line_norm)
  {
    acc_speed += curSpeed;
    g_errorD = (left_line_norm - right_column_norm - right_line_norm) / (right_line_norm + left_line_norm) * 100;
  }
  if (acc_speed>3000)
  {
      circlelandflag = 3;
  }
  if(3==circlelandflag)
  {
    
  }
}
