#include "headfile.h"
#include <GlobalVar.h>
#include "Basic.h"
#include "AD.h"
#include "Meeting.h"
int g_ErrorImageNumber = 0;
void AD_DirectionControl();
//================================================================//
//  @brief  :		�����㷨
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void Direction()
{
	//  Camera_DirectionControl();
	  //���뿪�����²�������ͷ�㷨
	//  if(dialSwitchFlg2&&balabaflag!=0)
	//  {
	//    
	//    if(Img_BrokenFlag == 2 && MEETING_MODE == 2 || Img_BrokenFlag == 3)
	//    {
	//      gpio_init(A7, GPO, 1);
	//      AD_DirectionControl();
	//    }
	//    else
	//    {
	//      gpio_init(A7, GPO, 0);
	//      Camera_DirectionControl();
	//    }
	//  }
	//  //���뿪�����ϲ�������㷨
	//  else if(!dialSwitchFlg2&&balabaflag!=0)
	//  {
	//
//  if((!dialSwitchFlg2||(2==Img_BrokenFlag||3==Img_BrokenFlag))&&!TurnTailFlag)
    if((!dialSwitchFlg2||Img_BrokenFlag)&&!TurnTailFlag)
  {
	AD_DirectionControl();
  }
  else if(dialSwitchFlg2&&!TurnTailFlag)
  {
    Camera_DirectionControl();
  }

        ////      AD_CircleIsland_Control();
	//    
	//  }
}

//================================================================//
//  @brief  :		����ƽ�����
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void DirectionControlOutput(void)//ƽ�����
{
	if (g_nDirectionControlPeriod < 5)
	{
		g_fDirectionControlOut = (g_fDirectionControlOut_new - g_fDirectionControlOut_before) * (g_nDirectionControlPeriod + 1) / g_DirectionPeriod + g_fDirectionControlOut_before;
	}
	g_nDirectionControlPeriod++;
	if (g_nDirectionControlPeriod == 5)
	{
		g_nDirectionControlPeriod = 0;
	}
	if (Img_BlockFlag || g_GetMeetingFlag || Img_BrokenFlag == 3) //�޷�
	{
		g_fDirectionControlOut = MAX(g_fDirectionControlOut, -max_duty);
		g_fDirectionControlOut = MIN(g_fDirectionControlOut, max_duty);
	}
	if (Img_BrokenFlag)
	{
		;
	}
	g_fDirectionControlOut_before = g_fDirectionControlOut_new;

}

//================================================================//
//  @brief  :		���ƫ���㷨
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void AD_DirectionControl()
{
	static int count = 0;
	static float g_fDirectionAngleControlOut = 0.0f;
	static float sensorGyroZLast = 0.0f;
	static float g_error_before = 0.0f;
	if (count >= 4)
	{
		count = 0;
		//    if ((Img_BlockFlag && !g_handle_open) || g_GetMeetingMaster)                //·�Ͽ���
		//    {
		//      ;
		//    }
		//    if(!CircleIsland_into_flag)//Բ��
		//    {
			if(!Img_BlockFlag&&circlelandflag!=2)
			{
		g_errorD = (left_line_norm - right_line_norm) / (right_line_norm + left_line_norm) * 100-g_errorCircleland;
			}
		//    }
		//    else if(CircleIsland_into_flag)
		//    {
		//      ;
		//    }
		//    if(!TurnTailFlag)
		//    {
		g_fDirectionAngleControlOut = g_errorD * g_dire_P_AD + (g_errorD - g_error_before) * g_dire_D_AD;
		g_fDirectionControlOut_new = (curSpeed/g_fSpeed_set+0.8)*(g_fDirectionAngleControlOut - sensor.Gyro_deg.z) * gRateKp_AD + (sensor.Gyro_deg.z - sensorGyroZLast) * gRateKd_AD;
		sensorGyroZLast = sensor.Gyro_deg.z;
		g_error_before = g_errorD;
		//    }
	}
	else count++;
}
//================================================================//
//  @brief  :		����ͷƫ���㷨
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
void Camera_DirectionControl()
{
	static int count = 0;
	static float g_fDirectionAngleControlOut = 0.0f;
	static float sensorGyroZLast = 0.0f;
	static float g_error_before = 0.0f;
	if (count >= 5)
	{
		count = 0;
//		if (Img_BlockFlag || g_GetMeetingFlag)                //·�Ͽ���
//		{
//			;
//		}
//		else
//		{	
          if(!Img_BlockFlag)
			{
			g_errorD = 94 - ML[ProSpect];
                          
			if (ErrorFlag)
			{
				g_ErrorImageNumber++;
				g_errorD = g_error_before;
			}
		}
		g_fDirectionAngleControlOut = g_errorD * g_dire_P *(curSpeed/g_fSpeed_set*0.8+0.2)+ (g_errorD - g_error_before) * g_dire_D;
		g_fDirectionControlOut_new = ((g_fDirectionAngleControlOut - sensor.Gyro_deg.z) * gRateKp + (sensor.Gyro_deg.z - sensorGyroZLast) * gRateKd);
		sensorGyroZLast = sensor.Gyro_deg.z;
		g_error_before = g_errorD;
	}
	else count++;
}