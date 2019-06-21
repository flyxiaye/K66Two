#include  "headfile.h"
#include  "GlobalVar.h"
#include  "CameraDisplay.h"
#include  "TFCard.h"
#include  "Menu.h"
#include  "OLED.h"
#include  "keyboard.h"
#include "Bluetooth.h"
#include "Screen.h"
#include "PID.h"
#include "XMU_IMU.h"


//#include  "headfile.h"
//#include  "GlobalVar.h"
//#include  "CameraDisplay.h"
//#include  "TFCard.h"
//#include  "Menu.h"
//#include  "OLED.h"
//#include  "keyboard.h"
//#include "Bluetooth.h"
//#include "Screen.h"
//#include "program.h"

#if _USE_LCD
#define LINE_MAX 8
#elif _USE_IPS
#define LINE_MAX 8
#else
#define LINE_MAX 4                                           //һҳ����ʾ�������� OLEDΪ4 LCD����Ϊ8 ����Ϊ10
#endif

signed char g_pageNum = 1;                                   //��ǰ����ҳ
signed char g_lineNum = 1;                                   //��ǰ������
signed char g_pageCount = 0;
signed char g_lineCount = 0;
signed char g_pageMax = 0;								     //ҳ������
signed char g_lineMax = 0;                                   //��ǰҳ�����ĸ���

signed char g_lineLock = 0;                                  //������־

signed char g_plusFlg = 0;                                   //�ӷ���־
signed char g_minisFlg = 0;                                  //������־
signed char g_imgFlg = 0;

signed char g_Rate = 0;                                      //����ָ�� ����Ӽ�
float g_addValue = 1;                                        //����
signed char g_picture_show = 0;
signed char g_explore_page = 0;								//�����ع⹦��ҳ

unsigned char Cursor[][16] =

{
#if _USE_LCD || _USE_IPS
	{0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00},//>
	{0x00,0x00,0x00,0x05,0x0A,0x14,0x28,0x50,0xA0,0x50,0x28,0x14,0x0A,0x05,0x00,0x00}//��
#else
	{0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00},//>
	{0x10,0x20,0x40,0x90,0x20,0x40,0x80,0x00,0x10,0x08,0x04,0x12,0x09,0x04,0x02,0x01}//��
#endif
};

void FlagChange(int* flag)
{
	if (1 == *flag)
	{
		*flag = 0;
		return;
	}
	else if (0 == *flag)
	{
		*flag = 1;
		return;
	}
}

//*
//*  @brief:	���
//*  @param:	void
//*  @return:   void
//*
void Dis_Cursor(unsigned char Position, signed char Number)
{
#if _USE_LCD
	Lcd_SetRegion(0, 16 * Position, 7, 16 * Position + 15);
	uint8 i, j;
	uint8 temp;
	for (i = 0; i < 16; i++)
	{
		//		Lcd_SetRegion(x,y+i,x+7,y+i);
		temp = Cursor[Number][i];
		for (j = 0; j < 8; j++)
		{
			if (temp & 0x01)	LCD_WriteData_16Bit(PENCOLOR);
			else			LCD_WriteData_16Bit(BGCOLOR);
			temp >>= 1;
		}
	}
#elif _USE_IPS
	ips_set_region(0, 16 * Position, 7, 16 * Position + 15);
	uint8 i, j;
	uint8 temp;
	for (i = 0; i < 16; i++)
	{
		temp = Cursor[Number][i];
		for (j = 0; j < 8; j++)
		{
			if (temp & 0x01) ips_writedata_16bit(IPS_PENCOLOR);
			else ips_writedata_16bit(IPS_BGCOLOR);
			temp >>= 1;
		}
	}
#else
	OLED_Set_Pos(0, 2 * Position);
	for (unsigned char i = 0; i < 8; i++)
		OLED_Write_Data(Cursor[Number][i]);
	OLED_Set_Pos(0, 2 * Position + 1);
	for (unsigned char i = 0; i < 8; i++)
		OLED_Write_Data(Cursor[Number][i + 8]);
#endif
}

//*
//*  @brief:		����ҳ
//*  @param:		void
//*  @return:	    void
//*
void PageNumReduce(void)
{
#if _USE_LCD
	LCD_TFT_Clear(BGCOLOR);
#elif _USE_IPS
	ips_clear(IPS_BGCOLOR);
#else
	OLED_Clear();
#endif
	g_pageNum--;
	if (g_pageNum < 1) g_pageNum = g_pageMax;
	g_lineNum = 1;
}

//*
//*  @brief:		���ҷ�ҳ
//*  @param:		void
//*  @return:	    void
//*
void PageNumAdd(void)
{
#if _USE_LCD
	LCD_TFT_Clear(BGCOLOR);
#elif _USE_IPS
	ips_clear(IPS_BGCOLOR);
#else
	OLED_Clear();
#endif
	g_pageNum++;
	if (g_pageNum > g_pageMax) g_pageNum = 1;
	g_lineNum = 1;
}

//*
//*  @brief:		��������
//*  @param:		void
//*  @return:	    void
//*
void LineNumReduce(void)
{
	g_lineNum--;
	if (g_lineNum < 1) g_lineNum = g_lineMax;
}

//*
//*  @brief:		��������
//*  @param:		void
//*  @return:	    void
//*
void LineNumAdd(void)
{
	g_lineNum++;
	if (g_lineNum > g_lineMax) g_lineNum = 1;
}


void GotoGivenPage(void)
{
	OLED_Clear();

	g_pageNum = GIVEN_PAGE;
	g_lineLock = 0;
	g_lineNum = 1;
}

//*
//*  @brief:		����һ��ҳ��
//*  @param:		��Ҫ��ʾ�ı��⣬������Ҫ��ʾ����ʱ�ַ���д"\0"
//*  @return:	    void
//*  @note:     
//*
void Insert_Page(signed char String[])
{
	g_pageCount++;

	if (g_pageMax < g_pageCount) g_pageMax = g_pageCount;

	if (g_pageNum == g_pageCount)
	{
		g_imgFlg = 0;
		if (String != "\0")
		{
			Dis_Head(String);
		}
		g_lineMax = 0;
	}
	g_lineCount = 0;
}

//*
//*  @brief:		����һ��ͼ��
//*  @param:		�ڲ˵��е�λ�� ��Ҫ��ʾ�ı����Լ�ͼ����ʾ�ĺ�����������Ҫ��ʾ����ʱ�ַ���д"\0"
//*  @return:	    void
//*  @note:     
//*
void Insert_Img(signed char String[], void pfunc_show(void))
{
	g_pageCount++;

	if (g_pageMax < g_pageCount) g_pageMax = g_pageCount;

	if (g_pageNum == g_pageCount)
	{
		g_imgFlg = 1;
		if (String != "\0")
		{
			Dis_Head(String);
		}
		g_lineMax = 0;
	}
	g_lineCount = 0;

	if (g_pageNum == g_pageCount)pfunc_show();
}

//*
//*  @brief:		�����ַ�����
//*  @param:		�ڲ˵��е�λ�� ��Ҫ����ı������Լ������ĵ�ַ
//*  @return:	    void
//*  @note:     
//*
void Insert_Char(signed char String[], signed char* Number)
{
	char tmp_line;
	if (g_pageNum == g_pageCount)
	{
		g_lineCount++;
		if (g_lineMax < g_lineCount) g_lineMax = g_lineCount;

		if (g_lineNum < LINE_MAX)
		{
			tmp_line = g_lineCount;
			Dis_String(2 * tmp_line - 1, String);
			Dis_Int(2 * tmp_line, *Number);
		}
		else
		{
			if (g_lineNum >= g_lineCount)
			{
				tmp_line = LINE_MAX - 1 + g_lineCount - g_lineNum;
				Dis_String(2 * tmp_line - 1, String);
				Dis_Int(2 * tmp_line, *Number);
			}
		}

		if (g_lineNum == g_lineCount)
		{
			if (g_plusFlg)
			{
				*Number += (int)g_addValue;
				g_plusFlg = 0;
			}
			else if (g_minisFlg)
			{
				*Number -= (int)g_addValue;
				g_minisFlg = 0;
			}
		}
	}
}

//*
//*  @brief:		�������ͱ���
//*  @param:		�ڲ˵��е�λ�� ��Ҫ����ı������Լ������ĵ�ַ
//*  @return:	    void
//*  @note:     
//*
void Insert_Int(signed char String[], signed int* Number)
{
	char tmp_line;
	if (g_pageNum == g_pageCount)
	{
		g_lineCount++;
		if (g_lineMax < g_lineCount) g_lineMax = g_lineCount;

		if (g_lineNum < LINE_MAX)
		{
			tmp_line = g_lineCount;
			Dis_String(2 * tmp_line - 1, String);
			Dis_Int(2 * tmp_line, *Number);
		}
		else
		{
			if (g_lineNum >= g_lineCount)
			{
				tmp_line = LINE_MAX - 1 + g_lineCount - g_lineNum;
				Dis_String(2 * tmp_line - 1, String);
				Dis_Int(2 * tmp_line, *Number);
			}
		}

		if (g_lineNum == g_lineCount)
		{
			if (g_plusFlg)
			{
				*Number += (int)g_addValue;
				g_plusFlg = 0;
			}
			else if (g_minisFlg)
			{
				*Number -= (int)g_addValue;
				g_minisFlg = 0;
			}
		}
	}
}

//*
//*  @brief:		���븡���ͱ���
//*  @param:		�ڲ˵��е�λ��(0~127) ��Ҫ����ı������Լ������ĵ�ַ
//*  @return:	  void
//*
void Insert_Float(signed char String[], float* Number)
{
	char tmp_line;

	if (g_pageNum == g_pageCount)
	{
		g_lineCount++;
		if (g_lineMax < g_lineCount) g_lineMax = g_lineCount;

		if (g_lineNum < LINE_MAX)
		{
			tmp_line = g_lineCount;
			Dis_String(2 * tmp_line - 1, String);
			Dis_Float(2 * tmp_line, *Number);
		}
		else
		{
			if (g_lineNum >= g_lineCount)
			{
				tmp_line = LINE_MAX - 1 + g_lineCount - g_lineNum;
				Dis_String(2 * tmp_line - 1, String);
				Dis_Float(2 * tmp_line, *Number);
			}
		}
		if (g_lineNum == g_lineCount)
		{
			if (g_plusFlg)
			{
				*Number += g_addValue;
				g_plusFlg = 0;
			}
			else if (g_minisFlg)
			{
				*Number -= g_addValue;
				g_minisFlg = 0;
			}
		}
	}
}



//*
//*  @brief:		����volatile�����ͱ���
//*  @param:		�ڲ˵��е�λ��(0~127) ��Ҫ����ı������Լ������ĵ�ַ
//*  @return:	  void
//*
void Insert_VFloat(signed char String[], volatile float* Number)
{
	char tmp_line;

	if (g_pageNum == g_pageCount)
	{
		g_lineCount++;
		if (g_lineMax < g_lineCount) g_lineMax = g_lineCount;

		if (g_lineNum < LINE_MAX)
		{
			tmp_line = g_lineCount;
			Dis_String(2 * tmp_line - 1, String);
			Dis_Float(2 * tmp_line, *Number);
		}
		else
		{
			if (g_lineNum >= g_lineCount)
			{
				tmp_line = LINE_MAX - 1 + g_lineCount - g_lineNum;
				Dis_String(2 * tmp_line - 1, String);
				Dis_Float(2 * tmp_line, *Number);
			}
		}
		if (g_lineNum == g_lineCount)
		{
			if (g_plusFlg)
			{
				*Number += g_addValue;
				g_plusFlg = 0;
			}
			else if (g_minisFlg)
			{
				*Number -= g_addValue;
				g_minisFlg = 0;
			}
		}
	}
}

//*
//*  @brief:		���˵�
//*  @param:		void
//*  @return:	    void
//*
void Menu(void)
{
	g_Key = Key_Check();
	Key_Function();
	if (!g_drive_flag)
	{
		Extra_Show();
		Main_Show();
                OLED_Write_Int(0,0,g_mode);
	}
}

//*
//*  @brief:		��������
//*  @param:		void
//*  @return:	    void
//*  @note:    
//* 
void Key_Function(void)
{
	if (!g_lineLock)
	{
		switch (g_Key)
		{
		case 1: if (g_steer_open)           //����
		{
			FlagChange(&g_drive_flag);//�����
			if (g_drive_flag)
			{
				g_StateMaster = 0;
				g_StartMaster = 0;
				g_MasterOutFlag = 0;
				CircleFlag = 0;
				CircleState = 0;
				BrokenFlag = 0;
				BlockFlag = 0;
				speed_type = 1;
				if (1 == g_camera_open)
					g_handle_open = 1;
				g_steer_open = 1;
				g_ad_flag = 1;
				RampFlag = 0;
			}
			// g_ramp_enable = 1;
		}
				break;
		case 2:
			if (g_pageNum == 1)
				g_steer_open = !g_steer_open;   //�����
			else
				LineNumReduce();                //��������
			break;
		case 3:
#if _USE_LCD
			lcd_init();
#elif _USE_IPS
			ips_init();
#else
			OLED_Init();               //ˢ����Ļ
			lcd_init();
#endif
			set_exposure_time(exp_time);  //�ع�ʱ��

			break;
		case 4:PageNumReduce();
			break;//���Ϸ�ҳ
		case 5:
			if (g_pageNum == 1)
				g_handle_open = !g_handle_open; //����ͼ
			else
				g_lineLock = 1;                 //������ֵ�޸�
			break;
		case 6:  PageNumAdd();       break;//���·�ҳ
		case 7:
                  if (2 == g_pageNum)
				g_ad_flag = !g_ad_flag;
			else changemode();
//				SD_Gather_Camera_Picture_120x188();        break; //��ͼ
                        break;
		case 8:
			if (1 == g_pageNum)
			{
				g_picture_show++;
				OLED_Clear();
			}
			else
				LineNumAdd();           //��������
			break;
		case 9:
			SD_Gather_Gray_Picture120x188();         break; //��ͼ
		default:                     break;
		}
	}
	else
	{
		switch (g_Key)
		{
		case 1: MyFlash_Write(0);    break;    //д��FLASH
		case 2: LineNumReduce();    break;    //��������
		case 3: g_plusFlg = 1;      break;    //��ֵ��
		case 4: g_Rate--;           break;    //���ٱ���
		case 5: g_lineLock = 0;     break;    //�˳���ֵ�޸�
		case 6: g_Rate++;           break;    //���ӱ���
		case 7:                     break;
		case 8: LineNumAdd();       break;    //��������
		case 9: g_minisFlg = 1;     break;    //��ֵ��
		default:                    break;
		}
	}
}


//*
//*  @brief:		��ʾ���ʺ͹��
//*  @param:		void
//*  @return:	  void
//*
void Extra_Show(void)
{
	if (!g_imgFlg)
	{
		switch (g_lineNum)
		{
#if _USE_LCD
		case 1:
			Dis_Cursor(1, g_lineLock);
			LCD_DispString(0, 4, " ");
			LCD_DispString(0, 6, " ");
			LCD_DispString(0, 8, " ");
			LCD_DispString(0, 10, " ");
			LCD_DispString(0, 12, " ");
			LCD_DispString(0, 14, " ");
			break;
		case 2:
			Dis_Cursor(2, g_lineLock);
			LCD_DispString(0, 2, " ");
			LCD_DispString(0, 6, " ");
			LCD_DispString(0, 8, " ");
			LCD_DispString(0, 10, " ");
			LCD_DispString(0, 12, " ");
			LCD_DispString(0, 14, " ");
			break;
		case 3:
			Dis_Cursor(3, g_lineLock);
			LCD_DispString(0, 4, " ");
			LCD_DispString(0, 2, " ");
			LCD_DispString(0, 8, " ");
			LCD_DispString(0, 10, " ");
			LCD_DispString(0, 12, " ");
			LCD_DispString(0, 14, " ");
			break;
		case 4:
			Dis_Cursor(4, g_lineLock);
			LCD_DispString(0, 4, " ");
			LCD_DispString(0, 6, " ");
			LCD_DispString(0, 2, " ");
			LCD_DispString(0, 10, " ");
			LCD_DispString(0, 12, " ");
			LCD_DispString(0, 14, " ");
			break;
		case 5:
			Dis_Cursor(5, g_lineLock);
			LCD_DispString(0, 4, " ");
			LCD_DispString(0, 6, " ");
			LCD_DispString(0, 8, " ");
			LCD_DispString(0, 2, " ");
			LCD_DispString(0, 12, " ");
			LCD_DispString(0, 14, " ");
			break;
		case 6:
			Dis_Cursor(6, g_lineLock);
			LCD_DispString(0, 4, " ");
			LCD_DispString(0, 6, " ");
			LCD_DispString(0, 8, " ");
			LCD_DispString(0, 10, " ");
			LCD_DispString(0, 2, " ");
			LCD_DispString(0, 14, " ");
			break;
		default:
			Dis_Cursor(7, g_lineLock);
			LCD_DispString(0, 4, " ");
			LCD_DispString(0, 6, " ");
			LCD_DispString(0, 8, " ");
			LCD_DispString(0, 10, " ");
			LCD_DispString(0, 12, " ");
			LCD_DispString(0, 2, " ");
			break;
#elif _USE_IPS
		case 1:
			Dis_Cursor(1, g_lineLock);
			IPS_DispString(0, 4, " ");
			IPS_DispString(0, 6, " ");
			IPS_DispString(0, 8, " ");
			IPS_DispString(0, 10, " ");
			IPS_DispString(0, 12, " ");
			IPS_DispString(0, 14, " ");
			break;
		case 2:
			Dis_Cursor(2, g_lineLock);
			IPS_DispString(0, 2, " ");
			IPS_DispString(0, 6, " ");
			IPS_DispString(0, 8, " ");
			IPS_DispString(0, 10, " ");
			IPS_DispString(0, 12, " ");
			IPS_DispString(0, 14, " ");
			break;
		case 3:
			Dis_Cursor(3, g_lineLock);
			IPS_DispString(0, 4, " ");
			IPS_DispString(0, 2, " ");
			IPS_DispString(0, 8, " ");
			IPS_DispString(0, 10, " ");
			IPS_DispString(0, 12, " ");
			IPS_DispString(0, 14, " ");
			break;
		case 4:
			Dis_Cursor(4, g_lineLock);
			IPS_DispString(0, 4, " ");
			IPS_DispString(0, 6, " ");
			IPS_DispString(0, 2, " ");
			IPS_DispString(0, 10, " ");
			IPS_DispString(0, 12, " ");
			IPS_DispString(0, 14, " ");
			break;
		case 5:
			Dis_Cursor(5, g_lineLock);
			IPS_DispString(0, 4, " ");
			IPS_DispString(0, 6, " ");
			IPS_DispString(0, 8, " ");
			IPS_DispString(0, 2, " ");
			IPS_DispString(0, 12, " ");
			IPS_DispString(0, 14, " ");
			break;
		case 6:
			Dis_Cursor(6, g_lineLock);
			IPS_DispString(0, 4, " ");
			IPS_DispString(0, 6, " ");
			IPS_DispString(0, 8, " ");
			IPS_DispString(0, 10, " ");
			IPS_DispString(0, 2, " ");
			IPS_DispString(0, 14, " ");
			break;
		default:
			Dis_Cursor(7, g_lineLock);
			IPS_DispString(0, 4, " ");
			IPS_DispString(0, 6, " ");
			IPS_DispString(0, 8, " ");
			IPS_DispString(0, 10, " ");
			IPS_DispString(0, 12, " ");
			IPS_DispString(0, 2, " ");
			break;
#else
		case 1:
			Dis_Cursor(1, g_lineLock);
			OLED_Write_String(4, 0, " ");
			OLED_Write_String(6, 0, " ");
			break;
		case 2:
			OLED_Write_String(2, 0, " ");
			Dis_Cursor(2, g_lineLock);
			OLED_Write_String(6, 0, " ");
			break;
		default:
			OLED_Write_String(2, 0, " ");
			OLED_Write_String(4, 0, " ");
			Dis_Cursor(3, g_lineLock);
			break;
#endif
		}
	}
	switch (g_Rate)
	{
	case -4: g_Rate = 2;                         break;
	case -3: g_addValue = 0.001;                 break;
	case -2: g_addValue = 0.01;                  break;
	case -1: g_addValue = 0.1;                   break;
	case 0: g_addValue = 1;                      break;
	case 1: g_addValue = 10;                     break;
	case 2: g_addValue = 100;                    break;
	case 3: g_addValue = 1000;                  break;
	case 4: g_Rate = -3;                         break;
	default: g_Rate = 0;                       break;
	}

#if _USE_LCD
	if (g_Rate >= 0)                                      //��ʾ����
	{
		LCD_DispChar(144, 0, ' ');
		OLED_Write_Int(0, 152, g_Rate);
	}
	else OLED_Write_Int(0, 144, g_Rate);
#elif _USE_IPS
	if (g_Rate >= 0)
	{
		IPS_DispChar(224, 0, ' ');
		OLED_Write_Int(0, 232, g_Rate);
	}
	else OLED_Write_Int(0, 224, g_Rate);
#else
	if (g_Rate >= 0)                                      //��ʾ����
	{
		OLED_Write_String(0, 112, " ");
		OLED_Write_Int(0, 120, g_Rate);
	}
	else OLED_Write_Int(0, 112, g_Rate);
#endif  
}


//*
//*  @brief:		�˵�������ʾ ����������ʾ��ͼ����ʾ
//*  @param:		void
//*  @return:	    void
//*
extern int speed_sum;
void Main_Show(void)
{
	g_pageCount = 0;

	//ͼ��ҳ
//	 Insert_Img("\0", DisPlayImage);
	Insert_Page("Display");
	if (g_pageNum == 1)
	{
		displayimage032_zoom(image[0], 64, 10, 70,1);
		switch (g_picture_show)
		{
		case 0:
			g_picture_show = 1;
			break;
		case 1:        //Բ��
			OLED_Write_Int(2, 8, RampFlag);
			OLED_Write_Int(4, 8, BrokenFlag);            //��·��־
			OLED_Write_Int(6, 8, BlockFlag);			//·�ϱ�־
			OLED_Write_Int(2, 96, g_StateMaster);         //�ᳵ״̬
			OLED_Write_Int(4, 96, g_StateSlave);            //���һ�����־
			OLED_Write_Int(6, 96, CircleState);           //����״̬��־

			break;
		case 2:
			Dis_Float(2, imu_data.pit);
			Dis_Float(4, imu_data.rol);
			Dis_Float(6, imu_data.yaw);
			Dis_Float(1, sensor.Gyro_deg.x);
			Dis_Float(3, sensor.Gyro_deg.y);
			Dis_Float(5, sensor.Gyro_deg.z);
			break;
			//           case 2:        //������Ϣҳ
			//                 OLED_Write_Int(0,80,g_steer_type);
			//           OLED_Write_Int(0,80,Steer_P);
			//                 OLED_Write_Int(0,88,spdExp);
			////                 OLED_Write_Int(2,96,SearchUp(180,160));
			//                 OLED_Write_Int(2,80,IdentifyRamp());
			//                 OLED_Write_Int(4,80,g_ramp);
			//                 OLED_Write_Int(4,96,g_angleResult);
			//                 break;
			//           case 3:        //˫����Ϣҳ
			//                 OLED_Write_Int(0,80,g_steer_type);
			//                 OLED_Write_Int(0,88,spdExp);
			//                 OLED_Write_Int(2,96,g_state);
			//                 OLED_Write_Int(2,112,g_state_opposite);
			//  //               OLED_Write_Int(4,96,g_delay_count);
			//                 OLED_Write_Int(4,96,g_distance_count);
			//                 OLED_Write_Int(6,96,IdentifyMeetingArea());
			//                 OLED_Write_Int(6,120,g_score);
			//                 break;
		case 3:      //����ͷ��Ϣҳ
			Dis_Int(1, ML[70]);
			Dis_Int(2, ML[60]);
			Dis_Int(3, ML[50]);
			Dis_Int(4, ML[40]);
			break;
		case 4:
			Dis_Int(1, ML[ProSpect]);
			Dis_Int(3, Error_1);
			Dis_Int(5, ErrorFlag);
			break;
		default:
			g_picture_show = 0;
			break;
		}
	}

         Insert_Page("Angle"); //��Ԫ��//SPI
         Insert_Float("pit",&imu_data.pit);
         Insert_Float("rol",&imu_data.rol);
         Insert_Float("yaw",&imu_data.yaw);
         Insert_Float("x",&sensor.Gyro_deg.x);
         Insert_Float("y",&sensor.Gyro_deg.y);
         Insert_Float("z",&sensor.Gyro_deg.z);

         
         
//         Insert_Page("Angle");//�Ƕ�
//         Insert_Float("X",&Angle.X);
//         Insert_Float("Y",&Angle.Y);
//         Insert_Float("Z",&Angle.Z);
//         Insert_Float("Rate_X",&ICM_Gyro.X);
//         Insert_Float("Rate_Y",&ICM_Gyro.Y);
//         Insert_Float("Rate_Z",&ICM_Gyro.Z);
//         
//         Insert_Page("Angle");//�Ƕ�
//         Insert_Float("ANGLE",&g_angle);
//         Insert_Float("GYRO",&g_gyro_angle_x);
//         Insert_Float("ACC",&g_acc_zy);
//         Insert_Float("RATIO",&g_gyro_ratio);

         Insert_Page("PID_A");//�Ƕ�
         Insert_Float("set",&g_angle_set);
         Insert_Float("conset",&g_angle_set_const);
         Insert_Float("out",&g_AngleOut);
         Insert_Float("P1",&g_angle_P);
         Insert_Float("D1",&g_angle_D);
         Insert_Float("P2",&g_RateP);
         Insert_Float("D2",&g_RateD);
//         Insert_Float("P2",&g_anglerate_P);
//         Insert_Float("D2",&g_anglerate_D);
//         Insert_Float("I2",&g_anglerate_I);
         
         
         
//         Insert_Page("PID_A");//�Ƕ�//SPI
//         Insert_Float("set",&g_angle_set);
//         Insert_Float("P_A",&pidAnglePitch.kp);
//         Insert_Float("I_A",&pidAnglePitch.ki);
//         Insert_Float("D_A",&pidAnglePitch.kd);
//         Insert_Float("P_D",&pidRatePitch.kp);
//         Insert_Float("I_D",&pidRatePitch.ki);
//         Insert_Float("D_D",&pidRatePitch.kd);

         Insert_Page("Obstacle");
         Insert_Float("g_inf",&g_inf);
         Insert_Int("stop_inf",&stop_inf);
         Insert_Int("s1",&s1);
         Insert_Int("st",&st);
         Insert_Int("sum_dist",&sum_dist);
         
         Insert_Page("Flag");
//         Insert_Int("camera_open",&g_camera_open);      //1��ʹ������ͷ 0����ʹ������ͷ
//         Insert_Int("ind_open",&g_ind_open);            //1��ʹ�õ��   0����ʹ�õ��
         Insert_Int("single_open",&g_single_open);
//         Insert_Int("ramp_open",&g_ramp_open);
         Insert_Int("broken_open",&g_broken_open);
         Insert_Int("block_open",&g_block_open);
         
         Insert_Page("PID_D");//����
         Insert_Float("error",&g_errorD);
         Insert_Float("Out" ,&g_fDirectionControlOut);
         Insert_Float("KP",&gRateKp);
         Insert_Float("KD",&gRateKd);
         Insert_Float("P",&g_dire_P);
         Insert_Float("D",&g_dire_D);
         Insert_Int("Pro",&ProSpect);
         
         Insert_Page("MeetFlag");
         Insert_Char("MasterM",&g_GetMeetingMaster);
         Insert_Char("SlaveM",&g_GetMeetingSlave);
         Insert_Char("MasterS",&g_StartMaster);
         Insert_Char("SlaveS",&g_StartSlave);
         Insert_Char("MasterO",&g_MasterOutFlag);
         Insert_Char("SlaveO",&g_SlaveOutFlag);
         
         Insert_Page("AD"); 
         Insert_Int("g_ad_flag",&g_ad_flag);            //1��������ƫ��  0���ɼ����ֵ  
         Insert_Float("ad_error_1",&ad_error_1);        //���ƫ��ֵ
         Insert_Int("left",&ind_left);                  //��Ųɼ�ֵ ��
         Insert_Int("right",&ind_right);                //��Ųɼ�ֵ ��
         Insert_Int("lmax",&ind_leftmax);               //��������ֵ
         Insert_Int("rmax",&ind_rightmax);              //��������ֵ
         Insert_Int("lmin",&ind_leftmin);               //�������Сֵ
         Insert_Int("rmin",&ind_rightmin);              //�������Сֵ
         Insert_Float("lnorm",&left_norm);              //������һ��ֵ
         Insert_Float("rnorm",&right_norm);             //����ҹ�һ��ֵ
         Insert_Int("mul_ad_error",&mul_ad_error);      //���error�Ŵ���
        
         //         //�����ٶȣ�����PID
         Insert_Page("SPEED");
         Insert_Int("SpdExp",&spdExp0);
         Insert_Int("SpdRel",&spdExp1);                 //ʵ���ٶ�
         Insert_Int("SpdInd",&spdExp2);
//         Insert_Int("SpdHigh",&d_spd_high);
//         Insert_Int("SpdMid",&d_spd_mid);
//         Insert_Int("SpdMax",&d_spd_max);
//         Insert_Int("SpdMin",&d_spd_min);
         Insert_Int("KDet",&KDet);
         Insert_Float("SpeedP",&g_speed_p);             //�ٿ�P
         Insert_Float("SpeedI",&g_speed_i);             //�ٿ�I
         Insert_Float("SpeedD",&g_speed_d);             //�ٿ�D
         
         
         Insert_Page("PID_S");//�ٶ�
//         Insert_Float("I", &g_speed_error_i);
//         Insert_Float("P", &g_speed_error_p);
//         Insert_Float("D", &g_speed_error_d);
//         Insert_Float("E",&g_speed_error);
         Insert_Float("SpdSet",&g_fSpeed_set);
         Insert_Float("error",&g_errorS);
         Insert_Float("P",&g_Speed_P);
         Insert_Float("I",&g_Speed_I);
         Insert_Int("MaxSpeed",&MaxSpeed);
         
         Insert_Page("PWM"); //���
         Insert_Float("left",&g_nLeftMotorPulseSigma);
         Insert_Float("right",&g_nRightMotorPulseSigma);
         Insert_Float("out_L",&g_fleft);
         Insert_Float("out_R",&g_fright);
         Insert_Float("duty_L",&g_duty_left);
         Insert_Float("duty_R",&g_duty_right);
         
         Insert_Page("AD"); //���
         Insert_Int("left",&ind_left);
         Insert_Int("right",&ind_right);
         Insert_Float("error",&g_errorD);
         
//          //���
//          Insert_Page("AD"); 
//         Insert_Int("g_ad_flag",&g_ad_flag);
//         Insert_Float("ad_error_1",&ad_error_1);
//         Insert_Int("mul_ad_error",&mul_ad_error);
//         Insert_Int("left",&ind_left);
//         Insert_Int("right",&ind_right);
//         Insert_Float("lnorm",&left_norm);
//         Insert_Float("rnorm",&right_norm);

//         Insert_Float("Pmax",&p_max);
//         Insert_Float("Pmin",&p_min);
//         Insert_Float("D1",&Steer_D1);
//         Insert_Float("D2",&Steer_D2);
//         Insert_Float("long_p",&long_steer_p);
//         Insert_Float("long_d",&long_steer_d);
//         Insert_Float("Plong",&p_long);
//         Insert_Float("Dmax",&d_max);
//         Insert_Int("E0",&E0);
//         Insert_Float("P2",&Steer_P2);
         
//         Insert_Float("P2",&Steer_P2);
//         Insert_Float("D",&Steer_D);
//         Insert_Float("D1",&Steer_D1);
//         Insert_Float("D2",&Steer_D2);
//         Insert_Int("steerduty",&SteerDuty);
         
//         Insert_Float("P_lose",&g_steer_p_straight);
//         Insert_Float("D_in",&g_steer_d_in);
//         Insert_Float("cir_p",&g_steer_p_circle);
//         Insert_Float("cir_p2",&g_steer_p2_circle);
//         Insert_Float("cir_d",&g_steer_d_circle);
//         Insert_Int("steer",&g_steer_duty);
//         Insert_Float("P_lit",&g_steer_p_little);
//         Insert_Float("D_out",&g_steer_d_out);
//         
//         //�����ٶȣ�����PID
//         Insert_Page("SPEED");
//         Insert_Int("SpdExp",&spdExp0);
//         Insert_Int("SpdRel",&spdExp1);
//         Insert_Int("SpdHigh",&d_spd_high);
//         Insert_Int("SpdMid",&d_spd_mid);
//         Insert_Int("SpdMax",&d_spd_max);
//         Insert_Int("SpdMin",&d_spd_min);
//         Insert_Int("KDet",&KDet);
//         Insert_Int("Small",&s_width);
//         Insert_Int("Middle",&m_width);
//         Insert_Int("Large",&l_width);
         
         
//         Insert_Int("SpdMin",&spdExp2);
//         Insert_Int("SpdCir",&spdExp3);
//         Insert_Int("SpdMeet",&spdExp4);
//         Insert_Int("SpdRamp",&spdExp5);
//         Insert_Int("SpdBig",&spdExp6);         
//         Insert_Int("SpdBack",&spdExpb);
//         Insert_Float("SpeedP",&g_speed_p);
//         Insert_Float("SpeedI",&g_speed_i);
//         Insert_Float("SpeedD",&g_speed_d);
//         Insert_Int("Lcur",&lCurSpeed);
//         Insert_Int("Rcur",&rCurSpeed);
//         Insert_Int("drif",&g_drive_flag);
         
//         
//         Insert_Page("STOP");
//         Insert_Int("rount",&rount);
         
//         Insert_Page("Camera");
//         Insert_Int("exp_time",&exp_time);
//         //�ᳵ
//         Insert_Page("meeting"); 
//         Insert_Int("identify",&g_meeting_threshold);
//         Insert_Int("offset",&g_meeting_offset);
//         Insert_Float("dis_k",&g_distance_k);
//         Insert_Int("protect",&g_protect_enable);
//         
//         //���        
//         Insert_Page("INDUC");
//         Insert_Int("middle",&ind_middle);
//         Insert_Float("MN",&middle_norm);
//         Insert_Int("MH",&mid_max);
//         Insert_Int("ML",&mid_min);
//         
//         /*����*/
////         Insert_Int("left",&ind_left);
////         Insert_Int("right",&ind_right);
////         Insert_Float("LN",&left_norm);
////         Insert_Float("RN",&right_norm);
//         
//         
//         //������
//         Insert_Page("Angle");
//         Insert_Int("jolt",&g_gyro_jolt);
//         Insert_Float("Res",&g_angleResult);
//         Insert_Int("get",&g_gyroGet);
//         Insert_Float("gb",&g_gyroBalance);
//         Insert_Int("Judge",&g_ramp_judge);
//           
//         //��ʱͣ��
//         Insert_Page("stop") ;
//         Insert_Int("sec",&g_stop_time);
//         Insert_Int("pic240",&g_pic240);
//         Insert_Int("cir_able",&g_circle_open);
//         Insert_Int("pic_show",&g_picture_show);
//          //����
//         Insert_Page("0000") ;
//         Insert_Int("CON1",&condition[1]);
//         Insert_Int("CON2",&condition[2]);
//         Insert_Int("CON3",&condition[3]);
//         Insert_Int("BIG",&big_circle);        
//         Insert_Int("meet",&circle_meet_cnt);
//         Insert_Int("MAX",&circle_max);
//         Insert_Int("1or0",&g_JudgeValue);  
//         
         
         

}


//*
//*  @brief:		����ͷ�����ع�ʱ��
//*  @param:		void
//*  @return:	    void
//*
void ExploreTime(void)
{
	static unsigned char DispPicture = 0;	//��ʾ����ͼ��
	static unsigned char DispEage = 1;		//��ʾ��Ե
	static signed char AddMul = 0;
	static unsigned char AddValue = 1;
	CannyEage();
	//displayimage032(image[0]);
	displayimage032_zoom(image[0], 64, 10, 70, 0);
	if (DispEage)
	{
		unsigned char* p = ImageEage[0];
		for (int i = 10; i <= 70; i++)
		{
			for (int j = 0; j < 160; j++)
			{
				unsigned char temp = *(p + i * ROW / 120 * COL + j * (COL - 1) / (160 - 1));//��ȡ���ص�
				if (HIGH_TH == temp)
				{
					Lcd_SetRegion(j, i + 54, j, i + 54);		//��������
					LCD_WriteData_16Bit(BLUE);
				}
			}
		}
	}
	//display
	Dis_String(1, "exp_time"); Dis_Int(2, exp_time);
	Dis_String(3, "ave_gray"); Dis_Int(4, LightThreshold);
	OLED_Write_Int(0, 152, AddValue);
	if (0 == AddMul) AddValue = 1;
	else if (1 == AddMul) AddValue = 10;
	else if (2 == AddMul) AddValue = 100;
	switch (g_Key)
	{
	case 1:
		MyFlash_Write(0);
		break;
	case 3:
		exp_time += AddValue;
		break;
	case 4:
		AddMul--;
		if (AddMul < 0)AddMul = 2;
		break;
	case 6:
		AddMul++;
		if (AddMul > 2) AddMul = 0;
		break;
	case 5:
		set_exposure_time(exp_time);
		break;
	case 8:
		g_explore_page = 0;
		g_pageNum = 1;
		g_lineNum = 1;
		OLED_Clear();
		break;
	case 9:
		exp_time -= AddValue;
		break;
	default:
		break;
	}
	INTR_Keyboard_Init();
}