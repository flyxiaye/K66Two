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


/********** ����Ϊ�洢������ **********/
#define DFLASH_PARAM1       CNST           //��ֵ                  unsigned char

#define DFLASH_PARAM2       KDp  // ���
#define DFLASH_PARAM3       Kd

#define DFLASH_PARAM4       KIp         //���
#define DFLASH_PARAM5       Ki

#define DFLASH_PARAM6       Con_line    // ǰհ

#define DFLASH_PARAM7       st   //�ٶ�
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

/********** ����Ϊ�洢������ **********/

//*************************************************************************//
//--------------------------------�洢����---------------------------------//
//*************************************************************************//
void MyFlash_Write(signed int flashnum)
{
    DisableInterrupts;
    FLASH_EraseSector(SECTOR_NUM + flashnum);
    DFlash_Write_Int(SECTOR_NUM + flashnum,0,First_Time_Init);
    


       /********** ����Ϊ�洢������ **********/
    DFlash_Write_Float(SECTOR_NUM + flashnum, 1, g_angle_set);//�Ƕ�����
    DFlash_Write_Float(SECTOR_NUM + flashnum, 2, g_angle_P);//�ǶȻ�
    DFlash_Write_Float(SECTOR_NUM + flashnum, 3, g_RateP);//�ڻ����ٶȻ�
    DFlash_Write_Float(SECTOR_NUM + flashnum, 4, g_RateD);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 5, g_dire_P);//����
    DFlash_Write_Float(SECTOR_NUM + flashnum, 6, g_dire_D);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 7, g_fSpeed_set);//�ٶȻ�����
    DFlash_Write_Float(SECTOR_NUM + flashnum, 8, g_Speed_P);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 9, g_Speed_I);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 10, gRateKd);//���򴮼�
    DFlash_Write_Float(SECTOR_NUM + flashnum, 11, gRateKp);
    DFlash_Write_Int(SECTOR_NUM + flashnum, 12, MaxSpeed);//�ٶȻ��޷�
    DFlash_Write_Int(SECTOR_NUM + flashnum, 13, ProSpect);//ǰհ
//    DFlash_Write_Int(SECTOR_NUM + flashnum, 14, meet_st);//�ᳵ���Ĺ̶�ƫ��
    DFlash_Write_Int(SECTOR_NUM + flashnum, 15, max_duty);//�ٶȻ�����޷�
    DFlash_Write_Int(SECTOR_NUM + flashnum, 16, st);//V���α���ƫ��
    DFlash_Write_Int(SECTOR_NUM + flashnum, 17, stop_inf); //·�Ϲ������
    DFlash_Write_Int(SECTOR_NUM + flashnum, 18, sum_dist);//V���α���·�̻���
//    DFlash_Write_Int(SECTOR_NUM + flashnum, 19, ave_left_cross);//��й�һ��
//    DFlash_Write_Int(SECTOR_NUM + flashnum, 20,  ave_right_cross);//
//    DFlash_Write_Int(SECTOR_NUM + flashnum, 21,  ave_left_column);//
//    DFlash_Write_Int(SECTOR_NUM + flashnum, 22,  ave_right_column);//
//        DFlash_Write_Float(SECTOR_NUM + flashnum, 23, ave_mid);//
//    DFlash_Write_Int(SECTOR_NUM + flashnum, 24, sum_meet_left);//�ᳵ��ͷ
//    DFlash_Write_Int(SECTOR_NUM + flashnum, 25, sum_meet_right);
    DFlash_Write_Int(SECTOR_NUM + flashnum, 26, Speed_MAX);//�ٶȻ����޷�
//    DFlash_Write_Float(SECTOR_NUM + flashnum, 27, Img_BlockOpen);//ǰհ

    DFlash_Write_Int(SECTOR_NUM + flashnum, 28, exp_time);//�������Ʋ���
    DFlash_Write_Float(SECTOR_NUM + flashnum, 29, gRateKd_AD);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 30, gRateKp_AD);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 31, g_dire_P_AD);
    DFlash_Write_Float(SECTOR_NUM + flashnum, 32, g_dire_D_AD);

    /********** ����Ϊ�洢������ **********/
OLED_Clear();
}
void MyFlash_Read(signed int flashnum)
{
    int FirstStart = 0;
    FirstStart = DFlash_Read_Int(SECTOR_NUM + flashnum,0);
    if(FirstStart == First_Time_Init)
    {
        /********** ����Ϊ�洢������ **********/
      

     
      
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
//        meet_st = DFlash_Read_Int(SECTOR_NUM + flashnum,14);
        max_duty = DFlash_Read_Int(SECTOR_NUM + flashnum,15);
        st = DFlash_Read_Int(SECTOR_NUM + flashnum,16);
        stop_inf = DFlash_Read_Int(SECTOR_NUM + flashnum,17);
        sum_dist = DFlash_Read_Int(SECTOR_NUM + flashnum,18);       
//         ave_left_cross = DFlash_Read_Int(SECTOR_NUM + flashnum,19);
//        ave_right_cross = DFlash_Read_Int(SECTOR_NUM + flashnum,20);
//        ave_left_column = DFlash_Read_Int(SECTOR_NUM + flashnum,21);
//        ave_right_column = DFlash_Read_Int(SECTOR_NUM + flashnum,22);
//                ave_mid = DFlash_Read_Float(SECTOR_NUM + flashnum,23);

        
        
        
//        sum_meet_left = DFlash_Read_Int(SECTOR_NUM + flashnum,24);
//        sum_meet_right = DFlash_Read_Int(SECTOR_NUM + flashnum,25);
        Speed_MAX = DFlash_Read_Int(SECTOR_NUM + flashnum,26);
//        Img_BlockOpen = DFlash_Read_Float(SECTOR_NUM + flashnum,27);
//  
//        
        exp_time = DFlash_Read_Int(SECTOR_NUM + flashnum,28);
        gRateKd_AD = DFlash_Read_Float(SECTOR_NUM + flashnum,29);
        gRateKp_AD = DFlash_Read_Float(SECTOR_NUM + flashnum,30);
        g_dire_P_AD = DFlash_Read_Float(SECTOR_NUM + flashnum,31);
        g_dire_D_AD = DFlash_Read_Float(SECTOR_NUM + flashnum,32);

         
 
        /********** ����Ϊ�洢������ **********/
      
     }
    
}