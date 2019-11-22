/*!
 * 	@file       ZS_DFlash.c
 * 	@brief      XMU SmartCar -> Group -> Camera
 * 	@author     WP
 * 	@version    New Car
 * 	@date       The Tenth in 2015
 */

#include<stdio.h>
#include<stdlib.h>

#include "DFlash.h"
#include "MK60_flash.h"
#include "MK60_savedata.h"
#include  "OLED.h"
#include <GlobalVar.h>
#include "common.h"

extern void OLED_Clear(void);

#define First_Time_Init 31


/********** 以下为存储的数据 **********/
#define DFLASH_PARAM1       g_angle_set           
#define DFLASH_PARAM2       g_angle_P  
#define DFLASH_PARAM3       g_RateP
#define DFLASH_PARAM4       g_RateD        
#define DFLASH_PARAM5       g_dire_P
#define DFLASH_PARAM6       g_dire_D    
#define DFLASH_PARAM7       g_fSpeed_set
#define DFLASH_PARAM8       g_Speed_P
#define DFLASH_PARAM9       g_Speed_I
#define DFLASH_PARAM10      gRateKd
#define DFLASH_PARAM11      gRateKp
#define DFLASH_PARAM12      MaxSpeed
#define DFLASH_PARAM13      ProSpect
#define DFLASH_PARAM14      max_duty
#define DFLASH_PARAM15      ramp_inf
#define DFLASH_PARAM16      sum_dist
#define DFLASH_PARAM17      Speed_MAX
#define DFLASH_PARAM18      exp_time
#define DFLASH_PARAM19      gRateKd_AD
#define DFLASH_PARAM20      gRateKp_AD
#define DFLASH_PARAM21      g_dire_P_AD
#define DFLASH_PARAM22      g_dire_D_AD
#define DFLASH_PARAM23		Img_CircleOpen
#define DFLASH_PARAM24		Img_StraightBrokenOpen
#define DFLASH_PARAM25		Img_CurveBrokenOpen
#define DFLASH_PARAM26		Img_StopOpen
#define DFLASH_PARAM27		Img_BlockOpen
#define DFLASH_PARAM28		Img_RampOpen
#define DFLASH_PARAM29		HighThreshold
#define DFLASH_PARAM30		LowThreshold
#define DFLASH_PARAM31		ControlMid
#define DFLASH_PARAM32		Rampangle
#define DFLASH_PARAM33		BrokenTurnTailPWM
#define DFLASH_PARAM34		BrokenTurnTailDistance
#define DFLASH_PARAM35		ind_left_line_max
#define DFLASH_PARAM36		ind_left_line_min
#define DFLASH_PARAM37		ind_left_column_max
#define DFLASH_PARAM38		ind_left_column_min
#define DFLASH_PARAM39		ind_right_line_min
#define DFLASH_PARAM40		ind_right_line_max
#define DFLASH_PARAM41		ind_right_column_max
#define DFLASH_PARAM42		ind_right_column_min
#define DFLASH_PARAM43		ind_mid_max
#define DFLASH_PARAM44		ind_mid_min
#define DFLASH_PARAM45		Circle_P
#define DFLASH_PARAM46		ST[0]
#define DFLASH_PARAM47		ST[1]
#define DFLASH_PARAM48		ST[2]
#define DFLASH_PARAM49		ST[3]
#define DFLASH_PARAM50          block_inf
#define DFLASH_PARAM51          Img_GrayJumpOpen
#define DFLASH_PARAM52          BrokenThreshold
#define DFLASH_PARAM53          Stopdistance
#define DFLASH_PARAM54          BootRacerOpen
#define DFLASH_PARAM55          AngleMutationOpenFlag
#define DFLASH_PARAM56          ADclearCircleFlag
#define DFLASH_PARAM57			Meeting13Flag

/********** 以上为存储的数据 **********/

//*************************************************************************//
//--------------------------------存储数据---------------------------------//
//*************************************************************************//
void MyFlash_Write(signed int flashnum)
{
	DisableInterrupts;
	FLASH_EraseSector(SECTOR_NUM + flashnum);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 0, First_Time_Init);



	/********** 以下为存储的数据 **********/
	DFlash_Write_Float(SECTOR_NUM + flashnum, 1, DFLASH_PARAM1);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 2, DFLASH_PARAM2);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 3, DFLASH_PARAM3);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 4, DFLASH_PARAM4);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 5, DFLASH_PARAM5);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 6, DFLASH_PARAM6);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 7, DFLASH_PARAM7);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 8, DFLASH_PARAM8);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 9, DFLASH_PARAM9);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 10, DFLASH_PARAM10);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 11, DFLASH_PARAM11);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 12, DFLASH_PARAM12);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 13, DFLASH_PARAM13);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 14, DFLASH_PARAM14);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 15, DFLASH_PARAM15);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 16, DFLASH_PARAM16);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 17, DFLASH_PARAM17);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 18, DFLASH_PARAM18);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 19, DFLASH_PARAM19);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 20, DFLASH_PARAM20);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 21, DFLASH_PARAM21);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 22, DFLASH_PARAM22);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 23, DFLASH_PARAM23);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 24, DFLASH_PARAM24);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 25, DFLASH_PARAM25);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 26, DFLASH_PARAM26);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 27, DFLASH_PARAM27);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 28, DFLASH_PARAM28);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 29, DFLASH_PARAM29);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 30, DFLASH_PARAM30);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 31, DFLASH_PARAM31);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 32, DFLASH_PARAM32);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 33, DFLASH_PARAM33);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 34, DFLASH_PARAM34);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 35, DFLASH_PARAM35);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 36, DFLASH_PARAM36);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 37, DFLASH_PARAM37);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 38, DFLASH_PARAM38);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 39, DFLASH_PARAM39);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 40, DFLASH_PARAM40);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 41, DFLASH_PARAM41);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 42, DFLASH_PARAM42);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 43, DFLASH_PARAM43);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 44, DFLASH_PARAM44);
	DFlash_Write_Float(SECTOR_NUM + flashnum, 45, DFLASH_PARAM45);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 46, DFLASH_PARAM46);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 47, DFLASH_PARAM47);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 49, DFLASH_PARAM49);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 50, DFLASH_PARAM50);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 51, DFLASH_PARAM51);
	DFlash_Write_Int(SECTOR_NUM + flashnum, 52, DFLASH_PARAM52);
        DFlash_Write_Int(SECTOR_NUM + flashnum, 53, DFLASH_PARAM53);
        DFlash_Write_Int(SECTOR_NUM + flashnum, 54, DFLASH_PARAM54);
        DFlash_Write_Int(SECTOR_NUM + flashnum, 55, DFLASH_PARAM55);
        DFlash_Write_Int(SECTOR_NUM + flashnum, 56, DFLASH_PARAM56);
		DFlash_Write_Int(SECTOR_NUM + flashnum, 57, DFLASH_PARAM57);

	/********** 以上为存储的数据 **********/
#ifdef _USE_LCD
	LCD_DispChar(0, 0, '*');
#else
	OLED_Write_Char(0, 0, '*');
#endif
}
void MyFlash_Read(signed int flashnum)
{
	int FirstStart = 0;
	FirstStart = DFlash_Read_Int(SECTOR_NUM + flashnum, 0);
	if (FirstStart == First_Time_Init)
	{
		/********** 以下为存储的数据 **********/




		DFLASH_PARAM1 = DFlash_Read_Float(SECTOR_NUM + flashnum, 1);
		DFLASH_PARAM2 = DFlash_Read_Float(SECTOR_NUM + flashnum, 2);
		DFLASH_PARAM3 = DFlash_Read_Float(SECTOR_NUM + flashnum, 3);
		DFLASH_PARAM4 = DFlash_Read_Float(SECTOR_NUM + flashnum, 4);
		DFLASH_PARAM5 = DFlash_Read_Float(SECTOR_NUM + flashnum, 5);
		DFLASH_PARAM6 = DFlash_Read_Float(SECTOR_NUM + flashnum, 6);
		DFLASH_PARAM7 = DFlash_Read_Float(SECTOR_NUM + flashnum, 7);
		DFLASH_PARAM8 = DFlash_Read_Float(SECTOR_NUM + flashnum, 8);
		DFLASH_PARAM9 = DFlash_Read_Float(SECTOR_NUM + flashnum, 9);
		DFLASH_PARAM10 = DFlash_Read_Float(SECTOR_NUM + flashnum, 10);
		DFLASH_PARAM11 = DFlash_Read_Float(SECTOR_NUM + flashnum, 11);
		DFLASH_PARAM12 = DFlash_Read_Int(SECTOR_NUM + flashnum, 12);
		DFLASH_PARAM13 = DFlash_Read_Int(SECTOR_NUM + flashnum, 13);
		DFLASH_PARAM14 = DFlash_Read_Int(SECTOR_NUM + flashnum, 14);
		DFLASH_PARAM15 = DFlash_Read_Int(SECTOR_NUM + flashnum, 15);
		DFLASH_PARAM16 = DFlash_Read_Int(SECTOR_NUM + flashnum, 16);
		DFLASH_PARAM17 = DFlash_Read_Int(SECTOR_NUM + flashnum, 17);
		DFLASH_PARAM18 = DFlash_Read_Int(SECTOR_NUM + flashnum, 18);
		DFLASH_PARAM19 = DFlash_Read_Float(SECTOR_NUM + flashnum, 19);
		DFLASH_PARAM20 = DFlash_Read_Float(SECTOR_NUM + flashnum, 20);
		DFLASH_PARAM21 = DFlash_Read_Float(SECTOR_NUM + flashnum, 21);
		DFLASH_PARAM22 = DFlash_Read_Float(SECTOR_NUM + flashnum, 22);
		DFLASH_PARAM23 = DFlash_Read_Int(SECTOR_NUM + flashnum, 23);
		DFLASH_PARAM24 = DFlash_Read_Int(SECTOR_NUM + flashnum, 24);
		DFLASH_PARAM25 = DFlash_Read_Int(SECTOR_NUM + flashnum, 25);
		DFLASH_PARAM26 = DFlash_Read_Int(SECTOR_NUM + flashnum, 26);
		DFLASH_PARAM27 = DFlash_Read_Int(SECTOR_NUM + flashnum, 27);
		DFLASH_PARAM28 = DFlash_Read_Int(SECTOR_NUM + flashnum, 28);
		DFLASH_PARAM29 = DFlash_Read_Int(SECTOR_NUM + flashnum, 29);
		DFLASH_PARAM30 = DFlash_Read_Int(SECTOR_NUM + flashnum, 30);
		DFLASH_PARAM31 = DFlash_Read_Int(SECTOR_NUM + flashnum, 31);
		DFLASH_PARAM32 = DFlash_Read_Int(SECTOR_NUM + flashnum, 32);
		DFLASH_PARAM33 = DFlash_Read_Int(SECTOR_NUM + flashnum, 33);
		DFLASH_PARAM34 = DFlash_Read_Int(SECTOR_NUM + flashnum, 34);
		DFLASH_PARAM35 = DFlash_Read_Int(SECTOR_NUM + flashnum, 35);
		DFLASH_PARAM36 = DFlash_Read_Int(SECTOR_NUM + flashnum, 36);
		DFLASH_PARAM37 = DFlash_Read_Int(SECTOR_NUM + flashnum, 37);
		DFLASH_PARAM38 = DFlash_Read_Int(SECTOR_NUM + flashnum, 38);
		DFLASH_PARAM39 = DFlash_Read_Int(SECTOR_NUM + flashnum, 39);
		DFLASH_PARAM40 = DFlash_Read_Int(SECTOR_NUM + flashnum, 40);
		DFLASH_PARAM41 = DFlash_Read_Int(SECTOR_NUM + flashnum, 41);
		DFLASH_PARAM42 = DFlash_Read_Int(SECTOR_NUM + flashnum, 42);
		DFLASH_PARAM43 = DFlash_Read_Int(SECTOR_NUM + flashnum, 43);
		DFLASH_PARAM44 = DFlash_Read_Int(SECTOR_NUM + flashnum, 44);
		DFLASH_PARAM45 = DFlash_Read_Float(SECTOR_NUM + flashnum, 45);
		DFLASH_PARAM46 = DFlash_Read_Int(SECTOR_NUM + flashnum, 46);
		DFLASH_PARAM47 = DFlash_Read_Int(SECTOR_NUM + flashnum, 47);
		DFLASH_PARAM48 = DFlash_Read_Int(SECTOR_NUM + flashnum, 48);
		DFLASH_PARAM49 = DFlash_Read_Int(SECTOR_NUM + flashnum, 49);
		DFLASH_PARAM50 = DFlash_Read_Int(SECTOR_NUM + flashnum, 50);
                DFLASH_PARAM51 = DFlash_Read_Int(SECTOR_NUM + flashnum, 51);
                DFLASH_PARAM52 = DFlash_Read_Int(SECTOR_NUM + flashnum, 52);
                DFLASH_PARAM53 = DFlash_Read_Int(SECTOR_NUM + flashnum, 53);
                DFLASH_PARAM54 = DFlash_Read_Int(SECTOR_NUM + flashnum, 54);
                DFLASH_PARAM55 = DFlash_Read_Int(SECTOR_NUM + flashnum, 55);
                DFLASH_PARAM56 = DFlash_Read_Int(SECTOR_NUM + flashnum, 56);
				DFLASH_PARAM57 = DFlash_Read_Int(SECTOR_NUM + flashnum, 57);


		/********** 以上为存储的数据 **********/

	}

}