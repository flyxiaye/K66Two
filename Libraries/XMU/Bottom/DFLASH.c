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
#define DFLASH_PARAM1       CNST           //阈值                  unsigned char

#define DFLASH_PARAM2       KDp  // 舵机
#define DFLASH_PARAM3       Kd

#define DFLASH_PARAM4       KIp         //电机
#define DFLASH_PARAM5       Ki

#define DFLASH_PARAM6       Con_line    // 前瞻

#define DFLASH_PARAM7       st   //速度
#define DFLASH_PARAM8       sum_dist

#define DFLASH_PARAM9      // Circle_path

#define DFLASH_PARAM10      Circle_path
   
#define DFLASH_PARAM11      outBendSpeed
#define DFLASH_PARAM12      smallStrightSpeed
#define DFLASH_PARAM13      longStrightSpeed
#define DFLASH_PARAM14      st
#define DFLASH_PARAM15      sum_dist
#define DFLASH_PARAM16      circleSpeed

#define DFLASH_PARAM17      timeLimit

#define DFLASH_PARAM18      closeLoop.p
#define DFLASH_PARAM19      closeLoop.i
#define DFLASH_PARAM20      closeLoop.expectDistance
#define DFLASH_PARAM21      img.bendErrorEnlarge
#define DFLASH_PARAM22      prospect

/********** 以上为存储的数据 **********/

//*************************************************************************//
//--------------------------------存储数据---------------------------------//
//*************************************************************************//
void MyFlash_Write(signed int flashnum)
{
    DisableInterrupts;
    FLASH_EraseSector(SECTOR_NUM + flashnum);
    DFlash_Write_Int(SECTOR_NUM + flashnum,0,First_Time_Init);
    


       /********** 以下为存储的数据 **********/
    DFlash_Write_Float(SECTOR_NUM + flashnum, 1, g_angle_set);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 2, g_angle_P);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 3, g_RateP);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 4, g_RateD);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 5, g_dire_P);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 6, g_dire_D);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 7, g_fSpeed_set);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 8, g_Speed_P);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 9, g_Speed_I);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 10, gRateKd);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 11, gRateKp);
    DFlash_Write_Int(SECTOR_NUM + flashnum, 12, MaxSpeed);
    DFlash_Write_Int(SECTOR_NUM + flashnum, 13, ProSpect);
    DFlash_Write_Int(SECTOR_NUM + flashnum, 14, DFLASH_PARAM14);
    DFlash_Write_Int(SECTOR_NUM + flashnum, 15, DFLASH_PARAM15);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 16, SteeringCtrlDi_universe);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 17, SteeringCtrlDo_universe);  
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 18, SteerEndDist_circle);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 19, SteerEndDist);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 20, SteeringMid );
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 21, SteeringMin );
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 22, SteeringMax );
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 23, DiffThreshold);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 24, NoiseThreshold);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 25, FrontThreshold);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 26, EngineMax);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 27, EngineMin);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 28, g_Angle_threshold);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 29, Jolt_delta);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 30, Jolt_Down_Speed);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 31, Jolt_time);
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 32, Jolt_Open);

    /********** 以上为存储的数据 **********/
OLED_Clear();
}
void MyFlash_Read(signed int flashnum)
{
    int FirstStart = 0;
    FirstStart = DFlash_Read_Int(SECTOR_NUM + flashnum,0);
    if(FirstStart == First_Time_Init)
    {
        /********** 以下为存储的数据 **********/
      

     
      
        g_angle_set = DFlash_Read_Float(SECTOR_NUM + flashnum, 1);
        g_angle_P = DFlash_Read_Float(SECTOR_NUM + flashnum, 2);
        g_RateP = DFlash_Read_Float(SECTOR_NUM + flashnum, 3);
        g_RateD = DFlash_Read_Float(SECTOR_NUM + flashnum, 4);
        g_dire_P = DFlash_Read_Float(SECTOR_NUM + flashnum, 5);
        g_dire_D = DFlash_Read_Float(SECTOR_NUM + flashnum, 6);
        g_fSpeed_set = DFlash_Read_Float(SECTOR_NUM + flashnum, 7);
        g_Speed_P= DFlash_Read_Float(SECTOR_NUM + flashnum, 8);    
        g_Speed_I = DFlash_Read_Float(SECTOR_NUM + flashnum, 9);
        gRateKd = DFlash_Read_Float(SECTOR_NUM + flashnum, 10);
        gRateKp = DFlash_Read_Float(SECTOR_NUM + flashnum, 11);
        MaxSpeed= DFlash_Read_Int(SECTOR_NUM + flashnum, 12);
        ProSpect = DFlash_Read_Int(SECTOR_NUM + flashnum,13);
        DFLASH_PARAM14 = DFlash_Read_Int(SECTOR_NUM + flashnum,14);
        DFLASH_PARAM15 = DFlash_Read_Int(SECTOR_NUM + flashnum,15);
//        SteeringCtrlDi_universe = DFlash_Read_Float(SECTOR_NUM + flashnum,16);
//        SteeringCtrlDo_universe = DFlash_Read_Float(SECTOR_NUM + flashnum,17);
//        SteerEndDist_circle = DFlash_Read_Float(SECTOR_NUM + flashnum,18);       
//        SteerEndDist  = DFlash_Read_Float(SECTOR_NUM + flashnum,19);
//        SteeringMid = DFlash_Read_Float(SECTOR_NUM + flashnum,20);
//        SteeringMin = DFlash_Read_Float(SECTOR_NUM + flashnum,21);
//        SteeringMax = DFlash_Read_Float(SECTOR_NUM + flashnum,22);
//        
//        
//        
//        DiffThreshold = DFlash_Read_Float(SECTOR_NUM + flashnum,23);
//        NoiseThreshold = DFlash_Read_Float(SECTOR_NUM + flashnum,24);
//        FrontThreshold = DFlash_Read_Float(SECTOR_NUM + flashnum,25);
//        EngineMax = DFlash_Read_Float(SECTOR_NUM + flashnum,26);
//        EngineMin = DFlash_Read_Float(SECTOR_NUM + flashnum,27);
//  
//        
//        g_Angle_threshold = DFlash_Read_Float(SECTOR_NUM + flashnum,28);
//        Jolt_delta = DFlash_Read_Float(SECTOR_NUM + flashnum,29);
//        Jolt_Down_Speed = DFlash_Read_Float(SECTOR_NUM + flashnum,30);
//        Jolt_time = DFlash_Read_Float(SECTOR_NUM + flashnum,31);
//        Jolt_Open = DFlash_Read_Float(SECTOR_NUM + flashnum,32);

         
 
        /********** 以上为存储的数据 **********/
      
     }
    
}