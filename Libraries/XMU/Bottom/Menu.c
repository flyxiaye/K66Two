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
#include "AD.h"

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
#define LINE_MAX 4                                           //一页能显示的总行数 OLED为4 LCD横向为8 竖向为10
#endif

signed char g_pageNum = 1;                                   //当前所在页
signed char g_lineNum = 1;                                   //当前所在行
signed char g_pageCount = 0;
signed char g_lineCount = 0;
signed char g_pageMax = 0;								     //页面总数
signed char g_lineMax = 0;                                   //当前页变量的个数

signed char g_lineLock = 0;                                  //锁定标志

signed char g_plusFlg = 0;                                   //加法标志
signed char g_minisFlg = 0;                                  //减法标志
signed char g_imgFlg = 0;

signed char g_Rate = 0;                                      //倍率指数 方便加减
float g_addValue = 1;                                        //倍率
signed char g_picture_show = 0;
signed char g_explore_page = 0;								//调节曝光功能页

unsigned char Cursor[][16] =

{
#if _USE_LCD || _USE_IPS
	{0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00},//>
	{0x00,0x00,0x00,0x05,0x0A,0x14,0x28,0x50,0xA0,0x50,0x28,0x14,0x0A,0x05,0x00,0x00}//》
#else
	{0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00},//>
	{0x10,0x20,0x40,0x90,0x20,0x40,0x80,0x00,0x10,0x08,0x04,0x12,0x09,0x04,0x02,0x01}//》
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
//*  @brief:	光标
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
//*  @brief:		向左翻页
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
//*  @brief:		向右翻页
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
//*  @brief:		行数上移
//*  @param:		void
//*  @return:	    void
//*
void LineNumReduce(void)
{
	g_lineNum--;
	if (g_lineNum < 1) g_lineNum = g_lineMax;
}

//*
//*  @brief:		行数下移
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
//*  @brief:		插入一个页面
//*  @param:		需要显示的标题，当不需要显示标题时字符串写"\0"
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
//*  @brief:		插入一幅图像
//*  @param:		在菜单中的位置 需要显示的标题以及图像显示的函数，当不需要显示标题时字符串写"\0"
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
//*  @brief:		插入字符变量
//*  @param:		在菜单中的位置 需要插入的变量名以及变量的地址
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
//*  @brief:		插入整型变量
//*  @param:		在菜单中的位置 需要插入的变量名以及变量的地址
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
//*  @brief:		插入浮点型变量
//*  @param:		在菜单中的位置(0~127) 需要插入的变量名以及变量的地址
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
//*  @brief:		插入volatile浮点型变量
//*  @param:		在菜单中的位置(0~127) 需要插入的变量名以及变量的地址
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
//*  @brief:		主菜单
//*  @param:		void
//*  @return:	    void
//*
void Menu(void)
{
	// g_Key = Key_Check();
	Key_Function();
	if (!g_drive_flag)
	{
		if (g_explore_page)
			ExploreTime();
		else
		{
			Extra_Show();
			Main_Show();
			OLED_Write_Int(0, 0, g_mode);
		}
	}
}

//*
//*  @brief:		按键功能
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
		case 1:           //启动
		{
			FlagChange(&g_drive_flag);//开电机
			if (g_drive_flag)
			{
//				g_StateMaster = 2;
				g_MasterOutFlag = 0;;
				CircleFlag = 0;
				CircleState = 0;
				Img_BrokenFlag = 0;
				Img_BlockFlag = 0;
				if (1 == g_camera_open)
					g_handle_open = 1;
				g_ad_flag = 1;
				Img_RampFlag = 0;
			}
		}
		break;
		case 2:
			if (g_pageNum == 1)
				g_steer_open = !g_steer_open;   //开舵机
			else
				LineNumReduce();                //行数上移
			break;
		case 3:
#if _USE_LCD
			lcd_init();
#elif _USE_IPS
			ips_init();
#else
			OLED_Init();               //刷新屏幕
			lcd_init();
#endif
			set_exposure_time(exp_time);  //曝光时间

			break;
		case 4:PageNumReduce();
			break;//向上翻页
		case 5:
			if (g_pageNum == 1)
				g_handle_open = !g_handle_open; //开补图
			else
				g_lineLock = 1;                 //进入数值修改
			break;
		case 6:  PageNumAdd();       break;//向下翻页
		case 7:
			if (12 == g_pageNum)
				g_ad_flag = !g_ad_flag;
			else changemode();
			//				SD_Gather_Camera_Picture_120x188();        break; //采图
			break;
		case 8:
			if (1 == g_pageNum)
			{
				g_picture_show++;
				OLED_Clear();
			}
			else
				LineNumAdd();           //行数下移
			break;
		case 9:

			//			FlagChange(&g_drive_flag);//开电机
			//                        		if (g_drive_flag)
			//			{
			//				g_StateMaster = 0;
			//				g_StartMaster = 0;
			//				g_MasterOutFlag = 0;
			//				CircleFlag = 0;
			//				CircleState = 0;
			//				Img_BrokenFlag = 0;
			//				Img_BlockFlag = 0;
			//				//				speed_type = 1;
			//				if (1 == g_camera_open)
			//					g_handle_open = 1;
			//				g_ad_flag = 1;
			//				Img_RampFlag = 0;
			//			}
			//                        FlagChange(&TurnTailFlag);
			//			if (g_drive_flag)
			//			{
			//				g_StateMaster = 0;
			//				g_StartMaster = 0;
			//				g_MasterOutFlag = 0;
			//				CircleFlag = 0;
			//				CircleState = 0;
			//				Img_BrokenFlag = 0;
			//				Img_BlockFlag = 0;
			//				//				speed_type = 1;

			//				if (1 == g_camera_open)
			//					g_handle_open = 1;
			//				g_ad_flag = 1;
			//				Img_RampFlag = 0;
			//			}
						// g_ramp_enable = 1;

			//			g_drive_flag = !g_drive_flag;
			//			TurnTailFlag=!TurnTailFlag;
//			SD_Gather_Gray_Picture120x188();

//			FlagChange(&g_drive_flag);//开电机
//			if (g_drive_flag)
//			{
				g_StateMaster = 0;
                                g_car_lanuch=1;
				g_MasterOutFlag = 0;;
				CircleFlag = 0;
				CircleState = 0;
				Img_BrokenFlag = 0;
				Img_BlockFlag = 0;
				if (1 == g_camera_open)
					g_handle_open = 1;
				g_ad_flag = 1;
				Img_RampFlag = 0;
//			}
			break; //采图
		default:                     break;
		}
	}
	else
	{
		switch (g_Key)
		{
		case 1: MyFlash_Write(0);    break;    //写入FLASH
		case 2: LineNumReduce();    break;    //行数上移
		case 3: g_plusFlg = 1;      break;    //数值加
		case 4: g_Rate--;           break;    //减少倍率
		case 5: g_lineLock = 0;     break;    //退出数值修改
		case 6: g_Rate++;           break;    //增加倍率
		case 7:                     break;
		case 8: LineNumAdd();       break;    //行数下移
		case 9: g_minisFlg = 1;     break;    //数值减
		default:                    break;
		}
	}
	INTR_Keyboard_Init();
}


//*
//*  @brief:		显示倍率和光标
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
	if (g_Rate >= 0)                                      //显示倍率
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
	if (g_Rate >= 0)                                      //显示倍率
	{
		OLED_Write_String(0, 112, " ");
		OLED_Write_Int(0, 120, g_Rate);
	}
	else OLED_Write_Int(0, 112, g_Rate);
#endif  
}


//*
//*  @brief:		菜单主体显示 包括变量显示与图像显示
//*  @param:		void
//*  @return:	    void
//*
extern int speed_sum;
void Main_Show(void)
{
	g_pageCount = 0;

	//图像页
//	 Insert_Img("\0", DisPlayImage);
	Insert_Page("Display");
	if (g_pageNum == 1)
	{
		displayimage032_zoom(image[0], 64, 10, 70, 1);
		switch (g_picture_show)
		{
		case 0:
			g_picture_show = 1;
			break;
		case 1:        //圆环
			OLED_Write_Int(2, 8, Img_RampFlag);
			OLED_Write_Int(4, 8, Img_BrokenFlag);            //断路标志
			OLED_Write_Int(6, 8, Img_BlockFlag);			//路障标志
			OLED_Write_Int(2, 96, g_StateMaster);         //会车状态
			OLED_Write_Int(4, 96, g_StateSlave);            \
			OLED_Write_Int(6, 96, CircleState);           //环岛状态标志

			break;
		case 2:
			Dis_Float(2, imu_data.pit);
			Dis_Float(4, imu_data.rol);
			Dis_Float(6, imu_data.yaw);
			Dis_Float(1, sensor.Gyro_deg.x);
			Dis_Float(3, sensor.Gyro_deg.y);
			Dis_Float(5, sensor.Gyro_deg.z);
			break;
			//           case 2:        //单车信息页
			//                 OLED_Write_Int(0,80,g_steer_type);
			//           OLED_Write_Int(0,80,Steer_P);
			//                 OLED_Write_Int(0,88,spdExp);
			////                 OLED_Write_Int(2,96,SearchUp(180,160));
			//                 OLED_Write_Int(2,80,IdentifyRamp());
			//                 OLED_Write_Int(4,80,g_ramp);
			//                 OLED_Write_Int(4,96,g_angleResult);
			//                 break;
			//           case 3:        //双车信息页
			//                 OLED_Write_Int(0,80,g_steer_type);
			//                 OLED_Write_Int(0,88,spdExp);
			//                 OLED_Write_Int(2,96,g_state);
			//                 OLED_Write_Int(2,112,g_state_opposite);
			//  //               OLED_Write_Int(4,96,g_delay_count);
			//                 OLED_Write_Int(4,96,g_distance_count);
			//                 OLED_Write_Int(6,96,IdentifyMeetingArea());
			//                 OLED_Write_Int(6,120,g_score);
			//                 break;
		case 3:      //调镜头信息页
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

	Insert_Page("Angle"); //四元素//SPI
	Insert_Float("pit", &imu_data.pit);
	Insert_Float("rol", &imu_data.rol);
	Insert_Float("yaw", &imu_data.yaw);
	Insert_Float("x", &sensor.Gyro_deg.x);
	Insert_Float("y", &sensor.Gyro_deg.y);
	Insert_Float("z", &sensor.Gyro_deg.z);

	Insert_Page("PID_A");//角度
	Insert_Float("set", &g_angle_set);
	Insert_Float("out", &g_AngleOut);
	Insert_Float("PA", &g_angle_P);
	Insert_Float("PR", &g_RateP);
	Insert_Float("DR", &g_RateD);
        Insert_Float("SP", &g_angle_Psingle);
         Insert_Float("SD", &g_angle_D);
	// Insert_Float("flygyro",)




	Insert_Page("PID_D/C");//方向
	Insert_Float("error", &g_errorD);
	Insert_Float("Out", &g_fDirectionControlOut);
	Insert_Float("KP", &gRateKp);
	Insert_Float("KD", &gRateKd);
	Insert_Float("P", &g_dire_P);
	Insert_Float("D", &g_dire_D);
	Insert_Int("MAX", &max_duty);
	Insert_Int("ProOUT", &ProSpect);
	Insert_Float("xxx", &acc_Xpeed);

	Insert_Page("PID_D/AD");//方向
	Insert_Float("error", &g_errorD);
	Insert_Float("Out", &g_fDirectionControlOut);
	Insert_Float("KP", &gRateKp_AD);
	Insert_Float("KD", &gRateKd_AD);
	Insert_Float("P", &g_dire_P_AD);
	Insert_Float("D", &g_dire_D_AD);


	Insert_Page("PID_S");//速度
	Insert_Float("SpdSet", &g_fSpeed_set);
	Insert_Float("error", &g_errorS);
	Insert_Float("P", &g_Speed_P);
	Insert_Float("I", &g_Speed_I);
	Insert_Int("MaxSpeed", &MaxSpeed);
	Insert_Int("MaxI", &Speed_MAX);
	Insert_Float("g_fI", &g_fI);

	Insert_Page("Obstacle");
	Insert_Int("g_inf", &g_inf);
	Insert_Int("stop_inf", &stop_inf);
	Insert_Int("angle", &st);
	Insert_Int("sum_dist", &sum_dist);
	//	Insert_Int("delay_dist", &delay_dist);

	Insert_Page("ImgFlag");
	Insert_Char("ImgCircle", &Img_CircleOpen);
	Insert_Char("ImgStraiBroken", &Img_StraightBrokenOpen);
	Insert_Char("ImgCurveBroken", &Img_CurveBrokenOpen);
	Insert_Char("ImgBlock", &Img_BlockOpen);
	Insert_Char("ImgRamp", &Img_RampOpen);
	Insert_Char("ImgStop", &Img_StopOpen);

	Insert_Page("MeetFlag");
	Insert_Char("Mode", &g_MeetingMode);
	Insert_Char("Dir", &g_MeetingDir);
	Insert_Char("MasterS", &g_StateMaster);
	Insert_Char("SlaveS", &g_StateSlave);
	Insert_Char("MasterO", &g_MasterOutFlag);
	Insert_Char("SlaveO", &g_SlaveOutFlag);
        Insert_Int("TurnTailPWM", &BrokenTurnTailPWM);
        

	Insert_Page("PWM"); //电机
	Insert_Float("left", &g_nLeftMotorPulseSigma);
	Insert_Float("right", &g_nRightMotorPulseSigma);
	Insert_Float("out_L", &g_fleft);
	Insert_Float("out_R", &g_fright);
	Insert_Float("duty_L", &g_duty_left);
	Insert_Float("duty_R", &g_duty_right);

	Insert_Page("camera");//摄像头曝光时间
	Insert_Int("exp", &exp_time);
	Insert_Int("HighTH", &HighThreshold);
	Insert_Int("LowTH", &LowThreshold);
	Insert_Int("ConMid", &ControlMid);
	Insert_Char("exp_page", &g_explore_page);
	Insert_Int("Diff", &DiffThreshold);
	Insert_Int("Light", &LightThreshold);


	Insert_Page("AD"); //电感
	Insert_Int("L-", &ind_left_line);
	Insert_Int("R-", &ind_right_line);
	Insert_Int("mid", &ind_mid);
	Insert_Int("L|", &ind_left_column);
	Insert_Int("R|", &ind_right_column);
	Insert_Int("L-Max", &ind_left_line_max);
	Insert_Int("R-Max", &ind_right_line_max);
	Insert_Int("L|Max", &ind_left_column_max);
	Insert_Int("R|Max", &ind_right_column_max);
	Insert_Int("mid_Max", &ind_mid_max);
	Insert_Float("norm_L-", &left_line_norm);
	Insert_Float("norm_R-", &right_line_norm);
	Insert_Float("norm_mid", &mid_norm);
	Insert_Float("norm_L|", &left_column_norm);
	Insert_Float("noem_R|", &right_column_norm);
	Insert_Int("Flag", &g_ad_flag);

}

//*
//*  @brief:		摄像头调节曝光时间
//*  @param:		void
//*  @return:	    void
//*
void ExploreTime(void)
{
	static unsigned char DispPicture = 0;	//显示整幅图像
	static unsigned char DispEage = 1;		//显示边缘
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
				unsigned char temp = *(p + i * ROW / 120 * COL + j * (COL - 1) / (160 - 1));//读取像素点
				if (HIGH_TH == temp)
				{
					Lcd_SetRegion(j, i + 54, j, i + 54);		//坐标设置
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