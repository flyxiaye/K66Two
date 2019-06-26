#include  "headfile.h"
#include "math.h"
#include <GlobalVar.h>
/***************环岛识别***************/
void circleland()
	{
        static int LRflag=0;//1左边大 2右边大
//          if(circlelandnumber==2)
//          {
//            return;
//          }
                if((leftcol_norm>0.3||rightcol_norm>0.3)&&(abs(leftcol_norm-rightcol_norm)>0.4*MAX(leftcol_norm,rightcol_norm))&&(ABS(left_norm)>0.3||ABS(right_norm)>0.3)&&mid_norm>0.8&&0==circlelandflag)
          {
              if(leftcol_norm>rightcol_norm)
                  {
                      LRflag=1;
                       circlelandflag=1;
                       circlelandSigma=0;
                  }
               if(leftcol_norm<rightcol_norm)
                  {
                      LRflag=2;
            circlelandflag=1;
            circlelandSigma=0;
          }
          }
		if(1==circlelandflag)
		{
                  Circlelanderror=0;
                  circlelandtimecount++;
                  
		if((ABS(leftcol_norm)>0.3||ABS(rightcol_norm)>0.3)&&circlelandtimecount<1000&&1==LRflag&&circlelandSigma>1000)
		{
                  if(leftcol_norm>rightcol_norm)
                  {
                      LRflag=0;
			circlelandflag=2;
                           circlelandSigma=0;
                                circlelandtimecount=0;
                                circlelandsymbol=0;
                  }
                  else if((ABS(leftcol_norm)<0.3&&ABS(rightcol_norm)<0.3)&&circlelandtimecount>1000&&1==LRflag&&circlelandSigma>1000)
                  {
                     LRflag=0;
			circlelandflag=0;
                    circlelandSigma=0;
                                circlelandtimecount=0;
                                circlelandsymbol=0;
                  }
                }
                	else	if((ABS(leftcol_norm)>0.3||ABS(rightcol_norm)>0.3)&&circlelandtimecount<1000&&2==LRflag&&circlelandSigma>1000)
		{
                  if(leftcol_norm<rightcol_norm)
                  {
                      LRflag=0;
			circlelandflag=2;
                           circlelandSigma=0;
                                circlelandtimecount=0;
                                circlelandsymbol=0;
                  }
            else if((ABS(leftcol_norm)<0.3&&ABS(rightcol_norm)<0.3)&&circlelandtimecount>1000&&2==LRflag&&circlelandSigma>1000)
                  {
                     LRflag=0;
			circlelandflag=0;
                    circlelandSigma=0;
                                circlelandtimecount=0;
                                circlelandsymbol=0;
                  }
                }
    
                  
                else if(circlelandtimecount>=3000||circlelandSigma>=5000)
                {
                               LRflag=0;
			circlelandflag=0;
                    circlelandSigma=0;
                                circlelandtimecount=0;
                                circlelandsymbol=0;

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
		if(2==circlelandflag)
		{
			if(ABS(leftcol_norm)>0.3||ABS(rightcol_norm)>0.3)
			{
                          if(circlelandSigma>2000)
                          {
//                                  if(ABS(leftcol_norm)<0.3&&ABS(rightcol_norm)<0.3)
//                          {
//                            circlelandflag=0;
//                                circlelandSigma=0;
//                                circlelandtimecount=0;
//                          }
//                          else
//                          {
				circlelandflag=3;
                                 circlelandSigma=0;
                                circlelandtimecount=0;
                                LRflag=0;
//                          }
                          }
			}
                        else
                        {
                           if(circlelandSigma>5000)
                          {
                            circlelandflag=0;
                                 circlelandSigma=0;
                                circlelandtimecount=0;
                                LRflag=0;
                          }
                        }
                        circlelandtimecount++;
                             if(circlelandtimecount>=1000)
                   {
                     circlelandsymbol=0;
                     circlelandflag=0;
                     circlelandSigma=0;
                     circlelandtimecount=0;
                     LRflag=0;
                   }
		}
		 if(3==circlelandflag)
		{
                  if(0==circlelandsymbol)
                  {
                   if(circlelandSigma<4500)
                      {
                  Circlelanderror=0.8*(((float)(leftcol_norm)-(float)(rightcol_norm))/((float)(leftcol_norm)+(float)(rightcol_norm)));
//                  if(Circlelanderror<0.3)
//                  {
//                    Circlelanderror=Circlelanderror*1.5;
//                  }

                      }
                  if((ABS(left_norm)>0.3||ABS(right_norm)>0.3)&&circlelandSigma>20000)
                  {
//                    if(circlelandSigma>15000)
//                    {
                    circlelandsymbol=1;
                    Circlelanderror=0;
//                    }
                    }
                                   if((ABS(left_norm)>0.3||ABS(right_norm)>0.3)&&circlelandSigma>30000)
                  {
//                    if(circlelandSigma>15000)
//                    {
                    circlelandsymbol=2;
                    Circlelanderror=0;
//                    }
                    }
//                    if((ABS(left_norm)>0.3||ABS(right_norm)>0.3)&&circlelandSigma>9000)
//                   {
//                     Circlelanderror=0;
//                   }
                  }
			circlelandtimecount++;
                   
                   if(circlelandtimecount>=20000)
                   {
                     circlelandsymbol=0;
                     circlelandflag=0;
                     circlelandSigma=0;
                     circlelandtimecount=0;
                   }
                         if((ABS(leftcol_norm)>0.3||ABS(rightcol_norm)>0.3)&&mid_norm>0.8&&(abs(leftcol_norm-rightcol_norm)>0.4*MAX(leftcol_norm,rightcol_norm)))
                  {
                      if(circlelandSigma>10000)
                      {
                                   LRflag=0;
			circlelandflag=4;
                    circlelandSigma=0;
                                circlelandtimecount=0;
                              
                      }
                  }
		}
               if(4==circlelandflag)
                {
                  if(leftcol_norm>0.3||rightcol_norm>0.3)
                  {
                            if(circlelandSigma>2000&&1==circlelandsymbol)
                      {
//                                            if(ABS(leftcol_norm)<0.3&&ABS(rightcol_norm)<0.3)
//                          {
//                            circlelandsymbol=0;
//                            circlelandflag=0;
//                                circlelandSigma=0;
//                                circlelandtimecount=0;
//                          }
//                              else
//                              {
				circlelandflag=5;
                                circlelandSigma=0;
                                circlelandtimecount=0;
                                circlelandsymbol=0;
//                              }
                
                      }
                               else    if(circlelandSigma>3000&&2==circlelandsymbol)
                      {
                        		circlelandflag=5;
                                circlelandSigma=0;
                                circlelandtimecount=0;
                                circlelandsymbol=0;

                
                      }
                  }
                  circlelandtimecount++;
                          if(circlelandtimecount>=3000)
                   {
                     circlelandsymbol=0;
                     circlelandflag=0;
                     circlelandSigma=0;
                     circlelandtimecount=0;
                   }
                }
                if(5==circlelandflag)
                {
                  circlelandtimecount++;
                  if(circlelandSigma<7000)
                      {
//                  Circlelanderror=-0.7*(((float)(leftcol_norm)-(float)(rightcol_norm))/((float)(leftcol_norm)+(float)(rightcol_norm)));
                         Circlelanderror=-0.9*(((float)(left_norm)-(float)(right_norm))/((float)(left_norm)+(float)(right_norm)));
                      }
			
                 else if(ABS(leftcol_norm)<0.4&&ABS(rightcol_norm)<0.4&&circlelandtimecount>1000)
                  {
                       if(circlelandSigma>4000)
                      {
                    circlelandflag=0;
                    circlelandnumber++;
                    Circlelanderror=0;
                                circlelandSigma=0;
                                circlelandtimecount=0;
                      }
                  }
                }

		
	}   
