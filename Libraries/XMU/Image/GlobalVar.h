#ifndef __GLOBALVAR_H__
#define __GLOBALVAR_H__


#include <math.h>

extern int text_flag ;

#define ROW             120             //原始图像总行数，即高              
#define COL             188             //原始图像总列数，即宽 

extern unsigned char image[ROW][COL];
extern unsigned char g_Img_All_120x188[ROW][COL];


extern const unsigned char g_Bit_Val_Up[8];
extern const unsigned char g_Bit_Val_Down[8];

//**********Angle(角度控制)**************//
extern float angle;
extern float g_AngleControlOut;
extern float g_angle_set;


extern float g_error;
extern float g_angle;

extern int g_count;
//串级
extern float g_AngleOut;
extern float g_RateP ;
extern float g_RateD;
extern float g_angle_P; 
extern float g_angle_Psingle; 
extern float g_angle_D; 
//单级
extern float g_single_angle_P;
extern float g_single_angle_D;
extern int AngleMutationFlag;
//***********MOTOR（电机模式及其输出及开关）***********//
extern float g_fleft;
extern float g_fright;
extern int g_mode;
extern float g_duty_left;
extern float g_duty_right;
//***********Speed(速度控制)**********//
extern float g_Speed_P;
extern float g_Speed_I;
extern float g_fSpeedControlOut;//输出速度控制
extern float g_fSpeed_set;//设置的速度值
extern float g_nSpeedControlPeriod;
extern float g_SpeedPeriod;
extern float g_nLeftMotorPulseSigma;
extern float g_nRightMotorPulseSigma;
extern float g_s;
extern float g_errorS ;

extern float g_duty_PWMleft;
extern float g_duty_PWMright;
extern int g_mode;
extern int g_drive_flag;
extern int MaxSpeed;
extern int Speed_MAX ;
extern float g_fI ;//积分项暂存处

//**************************Direction方向环（摄像头）**************//
extern float gRateKp;            //串级p
extern float gRateKd;            //串级d

extern int g_need ;//摄像头取得行数
extern float g_dire_P;
extern float g_dire_D;
extern float g_errorD;//差值
extern float g_errorCircleland;
extern float g_fDirectionControlOut;
extern float g_nDirectionControlPeriod;//输出平滑
extern float g_DirectionPeriod;//分的段数
extern float AD_flag;

extern float g_dire_ad_P;
extern float g_dire_ad_D ;

extern float g_fDirectionControlOut_new;
extern float g_fDirectionControlOut_before;

extern float Circle_P;
//**************************Direction方向环（电感）**************//
extern float gRateKp_AD ;            //串级p
extern float gRateKd_AD ;            //串级d

extern float g_dire_P_AD;
extern float g_dire_D_AD;
//==========================元素计数变量============================//

extern unsigned char CircleDir[10];		//环岛计数
extern unsigned char Ind_CircleOpen;	//电磁判断环岛开关


//==========================图像变量============================//
#define LEFT_EAGE 0			//图像左边界
#define RIGHT_EAGE 187		//图像右边界
#define MIDDLE 94			//图像中值
#define UP_EAGE 25			//图像上边界
#define DOWN_EAGE 70		//图像下边界
#define IMG_ROW 120			//图像行数
#define IMG_COL 188			//图像列数
//#define FIND_LEFT 0
//#define FIND_RIGHT 1

#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#define MIN(x, y) ((x) < (y)) ? (x) : (y)

#define FINDLINE_TH 7		//搜线阈值
#define IN_OFFSET 2 //向内偏移
//#define OUT_OFFSET 6 //向外偏移
#define CIRCLEUP_TH 15		//进入环岛差距行

#define CIRCLE 2			//环岛开关
#define CURVE_BROKEN 1		//弯道断路
#define STRAIGHT_BROKEN 1	//直道断路
#define STOPLINE 1			//停车开关
#define CI_IND 0			//出环岛标志
#define INF 1				//红外识别

typedef struct
{
	int Row;
	int Col;
}Point;

typedef struct
{
	int Type;		//0普通情况 1内跳 2外跳 3拐点 4上边界 5左边界 6右边界 7首行丢边 8：4类十字补线 9结束循环
	int ErrRow;
	int ErrCol;
}SpecialPoint;

enum CircleType
{
	CN, CL, CR
};

enum EageType
{
	NO, HIGH_TH, LOW_TH
};

extern const int MidOffset[];
extern int ML_Count;									//中线有效行
extern SpecialPoint LeftPnt, RightPnt;					//保存左右特殊点信息
extern int g_RoadType;
extern int ErrorFlag;								//错误标志位
extern int LastMiddleLine;							//保存上帧中线
extern int DiffThreshold;							//边缘检测阈值
extern int DarkThreshold;                         //暗阈值（路障）
extern int BrightThreshold;                       //亮阈值（坡道）
extern int LightThreshold;						//去高光噪声阈值
extern int LightThreshold2;						//去高光噪声阈值2
extern int FindLineType;							//是否加入高光滤波标志

extern int SpeedRow;			//控速边界行数

extern int CircleFlag;
extern int CircleState;
extern unsigned char Dist_ClearSevenFlag;

extern int LeftIntLine;		//记录左内跳行数
extern int RightIntLine;		//记录右内跳行数
extern int Img_SpecialElemFlag;    //特殊元素标志
extern int Img_BrokenFlag;			//断路标志
extern int Img_BlockFlag;			//路障标志
extern int Img_StopLineFlag;		//停车线标志
extern int Img_RampFlag;			//坡道标志

extern int BrokenLastAve;
extern int StopLineDist;

extern unsigned char Img_CircleOpen;
extern unsigned char Img_StraightBrokenOpen;
extern unsigned char Img_CurveBrokenOpen;
extern unsigned char Img_StopOpen;
extern unsigned char Img_BlockOpen;
extern unsigned char Img_RampOpen;

extern int ML[IMG_ROW], LL[IMG_ROW], RL[IMG_ROW];
extern unsigned char ImageEage[IMG_ROW][IMG_COL];
void VarInit(void);
extern int pro;

//==========================摄像头参数=================================//
#define ONE_METER 5800
extern int exp_time;						//曝光时间
extern int HighThreshold;						//canny高阈值
extern int LowThreshold;						//canny低阈值
extern int ControlMid;						//图像控制中值


//==========================菜单标志=================================//
#define InitSteer 1147               //舵机初始值 75Hz 1145 50Hz 766

extern int g_car_lanuch;				//车辆启动标志
extern int g_drive_flag;                //电机开关
extern int g_ad_flag;                   //电感开关
extern int g_steer_open;                //舵机开关
extern int g_handle_open;               //补图开关
extern int g_ramp_open;            //坡道开关标志
extern int g_broken_open;          //断路开关标志
extern int g_block_open;           //路障开关标志
extern int g_single_open;          //单车模式开关标志
extern int Flag_max;
extern int g_camera_open;
extern int g_ind_open;
extern int g_camera_ctrl_flag;
extern int g_ind_ctrl_flag;
extern int g_broken_enable;
     
extern int BugFlag;

//=================================舵机控制======================================//
extern int SteerMode;      //舵机控制模式 0摄像头 1电磁
extern int Error_1;
extern int Error_2;
extern float Steer_P;
extern float Steer_P2;
extern float p_max;
extern float p_min;
extern float p_long;
extern float d_max;
extern float Steer_D;
extern float Steer_D1;                  //入弯
extern float Steer_D2;                  //出弯
extern int ProSpect;
extern int SteerDuty;
extern float long_steer_p;
extern float long_steer_d;

//=====================================电感===================================//
extern float ad_error_1, ad_error_2;
extern int mul_ad_error;         //电磁error放大倍数

extern int ind_left_line,//记录电感
            ind_left_column,
            ind_right_line,
            ind_right_column,
            ind_mid;
extern int ind_left_line_max,ind_left_line_min,  
    ind_left_column_max,ind_left_column_min ,
    ind_right_line_max,ind_right_line_min,
    ind_right_column_max ,ind_right_column_min,
    ind_mid_max,ind_mid_min;//电感最值
extern float  left_line_norm,left_column_norm,right_line_norm,right_column_norm,mid_norm ;//归一化
extern float ind_mid_flag ,ind_left_flag ,ind_right_flag ;
extern int CircleIsland_into_flag ;
/*=====================================增量式速度控制========================================*/
extern int KDet;

//电机输出限幅
#define SPEED_OUT_MAX  9999  //正转最大
#define SPEED_OUT_MIN -9999  //反转最大


extern int g_steer_error; //舵机偏差

typedef struct
{
    float error_1;//偏差
    float error_2;
    float error_3;
    int d_duty; //输出偏差项
     float p;
     float i;
     float d;
}SPEED;

extern int speed_type;

extern float curSpeed;
extern int lCurSpeed;
extern int rCurSpeed;
extern int sidetype;//赛道类型
extern SPEED leftDrv;
extern SPEED rightDrv;
//期望速度
extern int spdExp;
extern int spdExp0;
extern int spdExp1;
extern int spdExp2;
extern int spdExp3;
extern int spdExp4;
extern float leftExpect; //左轮期望速度
extern float rightExpect;//右轮期望速度

extern signed int leftSpeedOut; //左轮速度输出
extern signed int rightSpeedOut;//右轮速度输出
//速度控制pi
extern float g_speed_p;
extern float g_speed_i;
extern float g_speed_d;

//extern int s4;
//extern int s5;
//extern int s6;
//extern int s7;

extern int d_spd_max;
extern int d_spd_high;
extern int d_spd_mid;
extern int d_spd_min;

extern int s_width;
extern int m_width;
extern int l_width;

//==================================刹车=================//
extern int g_stop_line;
extern int g_stop_dis;
extern int times_go;
extern int times_stop;
extern int rount;          //圈数
extern int DistantStop;

//*****************TF卡变量*******************
#define GATHER_DATA_KIND    10                          //采集的变量个数

extern unsigned int g_SD_Gather_Data[GATHER_DATA_KIND]; //采集的数据
extern unsigned char g_Img_All_128x320[128*160];        //SD卡存摄像图数据
extern unsigned char g_Img_All_240x320[240*160];        //SD卡存摄像图数据
extern unsigned char g_Img_All_60x80[60*40]; 


/*=====================================拨码开关========================================*/
extern int dialSwitchFlg1;
extern int dialSwitchFlg2;
extern int dialSwitchFlg3;
extern int dialSwitchFlg4;
extern int dialSwitchFlg5;
extern int dialSwitchFlg6;
extern int dialSwitchFlg7;
extern int dialSwitchFlg8;
extern int dialSwitchcal;
//extern signed int DialSwitchFlag1;            //拨码开关1
//extern signed int DialSwitchFlag2;            //拨码开关2
//extern signed int DialSwitchFlag3;            //拨码开关3
//extern signed int DialSwitchFlag4;        
#define DIALSWITCH_PIN1 E8
#define DIALSWITCH_PIN2 E6
#define DIALSWITCH_PIN3 E7
#define DIALSWITCH_PIN4 E9
#define DIALSWITCH_PIN5 D9
#define DIALSWITCH_PIN6 D10
#define DIALSWITCH_PIN7 E12
#define DIALSWITCH_PIN8 E11
/*===================================双车通讯用=========================================*/
typedef struct 
{
  signed char infotype;// 1.      0传输距离 1传输标志位
  char begin;           // 4.      通信起跑
  char state;           // 5-8.      
}btInfo;

extern signed char btMessage ;

extern signed char btRcvMessage;

extern btInfo btSndInfo,btRcvInfo;
/*=====================================双车状态======================================*/
enum MeetingState {
	Ready, WaitingBegin, CarGo,
	StateOne = 4, StateTwo, StateThree, StateFour, StateFive, StateSix, StateSeven,
	WaitingStop = 11, IsStopLine, StateGo, StateStop, CarFinish
};

//extern unsigned char g_GetMeetingMaster;            //主车到达会车区标志
//extern unsigned char g_GetMeetingSlave;             //从车到达会车区标志
//extern unsigned char g_MeetingCtrlFlag;             //会车状态车辆控制标志
//extern unsigned char g_MeetingCtrlEndFlag;             //会车状态车辆控制结束标志
//extern unsigned char g_EndMeetingMaster;           //主车结束会车标志
extern unsigned char g_MasterOutFlag;              //主车出界
extern unsigned char g_SlaveOutFlag;               //从车出界
extern unsigned char g_StateMaster;
extern unsigned char g_StateSlave;
extern unsigned char g_MeetingMode;
extern unsigned char g_MeetingDir;			//转向方向 1为右转 0为左转
extern unsigned char g_GetMeetingFlag;
extern unsigned char g_GetMeetingState;

/*====================================会车区惯导变量=====================================*/
extern int sum_distance_1;
extern int sum_distance_2;
extern int sum_distance_3;
extern int obj_angle_1;
extern int obj_angle_2;
extern int const_error_1;
extern int const_error_2;
extern int max_duty;
extern int OutMeetingDistance1;
extern int OutMeetingDistance2;
extern float GroundAngle;
/*====================================路障=====================================*/
extern int sum;
extern int sum_dist;
extern int g_inf;
extern int st;
extern int block_inf;
extern int ramp_inf;
/*=====================================陀螺仪变量======================================*/
extern signed int I2C_Wait_Times;        //i2c等待次数
extern signed int I2C_Wait_Err_Flg;      //i2c等待死循环错误标志

extern float g_gyroBalance;           //陀螺仪平衡值
extern float g_gyroIntegration;             //陀螺仪积分量

extern int g_gyroDimension;               //陀螺仪转换加速度计量纲

extern float g_angleResult;                 //角度计算结果
extern float g_angleResult_last;
extern float g_angle_temp;

extern int g_gyroGet;                     //陀螺仪读取值
extern signed int g_gyroGet_max;
extern signed int g_gyroGet_min;
extern int g_gyro_jolt;
extern int g_jolt_flag;

extern signed int gyroClearRamp;         //陀螺仪检测清坡道标志

extern int g_gyro_cnt;//坡道


extern int g_ramp_judge;

//==========================测试电机============================//
extern int LeftPWM;
extern int RightPWM;
extern int LeftPWM2;
extern int RightPWM2;
extern int EncoderL;
extern int EncoderR;

//**********speed(速度控制)**************//
extern float g_speed_error;
extern float g_speed_error_i;
extern float g_speed_error_p;
extern float g_speed_error_d;
///*=====================================速度控制========================================*/
//
//extern int opposite_ready,opposite_wait,opposite_done;
//extern int KDet;
//
////电机输出限幅
//#define SPEED_OUT_MAX  9999  //正转最大
//#define SPEED_OUT_MIN -9999  //反转最大
//#define IOUT_MAX       9500  //积分最大
//#define IOUT_MIN      -6000  //积分最小
//
//
//
//extern int g_steer_error; //舵机偏差
//
//typedef struct
//{
//    float error;//偏差
//    float iError;//偏差积分项
//    float iOut;//积分输出项
//    
//     float p;
//     float i;
//}SPEED;
//
//
//extern float curSpeed;
//extern int lCurSpeed;
//extern int rCurSpeed;
//extern int sidetype;//赛道类型
//extern SPEED leftDrv;
//extern SPEED rightDrv;
////期望速度
//extern int spdExp;
//extern int spdExp0;
//extern int spdExp1;
//extern int spdExp2;
//extern int spdExp3;
//extern int spdExp4;
//extern float leftExpect; //左轮期望速度
//extern float rightExpect;//右轮期望速度
//
//extern signed int leftSpeedOut; //左轮速度输出
//extern signed int rightSpeedOut;//右轮速度输出
////速度控制pi
//extern float g_speed_p;
//extern float g_speed_i;
//
//extern signed int lOverflowFlg;//左轮遇限方向积分溢出标志
//extern signed int rOverflowFlg;//右轮遇限方向积分溢出标志
////extern int s4;
////extern int s5;
////extern int s6;
////extern int s7;
//
//extern int d_spd_max;
//extern int d_spd_high;
//extern int d_spd_mid;
//extern int d_spd_low;
//
//extern int s_width;
//extern int m_width;
//extern int l_width;



//==========================硬件参数区============================//

extern signed int DialSwitchFlag1;            //拨码开关1
extern signed int DialSwitchFlag2;            //拨码开关2
extern signed int DialSwitchFlag3;            //拨码开关3
extern signed int DialSwitchFlag4;        
//#define DIALSWITCH_PIN1 E6
//#define DIALSWITCH_PIN2 E7
//#define DIALSWITCH_PIN3 E8
//#define DIALSWITCH_PIN4 E9
/**************电磁环岛*************/
extern float Circlelanderror;
extern int circlelandflag;
extern float circlelandSigma;
extern int circlelandtimecount;
extern int circlelandget; 
extern int circlelandnumber;
extern int circlelandsymbol;

//uint8 phototube;
/*************====================坡道路障相关可控制变量=======================*************/
extern int BrokenTurnTailPWM;
extern int BrokenTurnTailDistance;
extern float Rampangle;
/*************====================开机加速相关变量=======================*************/
extern int BootRacerFlag;
extern int BootRacerDistance;
extern int BootRacerAngle;



#endif