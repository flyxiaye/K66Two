#ifndef __GLOBALVAR_H__
#define __GLOBALVAR_H__


#include <math.h>


#define ROW             120             //原始图像总行数，即高              
#define COL             188             //原始图像总列数，即宽 

extern unsigned char image[ROW][COL];
extern unsigned char g_Img_All_120x188[ROW][COL];


extern const unsigned char g_Bit_Val_Up[8];
extern const unsigned char g_Bit_Val_Down[8];

//**********Angle(角度控制)**************//
extern float angle;
extern float g_constant1;
extern float g_gyro_ratio;
extern float g_AngleControlOut;
extern float g_angle_set;
extern float g_angle_P; 
extern float g_angle_D; 
extern float g_angle_x;
extern float g_angle_y;
extern float g_angle_z;
extern float g_anglerate_x;
extern float g_anglerate_y;
extern float g_anglerate_z;
extern float g_anglerate_P;
extern float g_anglerate_D;
extern float g_anglerate_I;
extern float g_rate_expect;
extern float g_rate_expect;
extern float g_rate_expect_old;
extern float g_rate_expect_integal;
extern float g_rate_expect_integal_old;
extern float g_angle_old;
extern float g_error;
extern float g_gyro_angle_x;
extern float g_angle;
extern float g_acc_zy;//加速度?
extern float g_acc_zx;
extern int g_count;
extern float g_angle_set_const;
//串级
extern float g_AngleOut;
extern float g_RateP ;
extern float g_RateD;

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
//extern int g_flag;
extern int MaxSpeed;
//**************************Direction方向环（摄像头）**************//
extern float gRateKp;            //串级p
extern float gRateKd;            //串级d

extern int g_need ;//摄像头取得行数
extern float g_dire_P;
extern float g_dire_D;
extern float g_errorD;//差值
extern float g_fDirectionControlOut;
extern float g_nDirectionControlPeriod;//输出平滑
extern float g_DirectionPeriod;//分的段数
extern float AD_flag;






//==========================图像变量============================//
#define ROW             120             //原始图像总行数，即高              
#define COL             188             //原始图像总列数，即宽 
//#define F_INT(num)	(signed int)(num + 0.5f)
//#define Line_To_Dis(row) F_INT((((0.00004f * row - 0.0079f) * row + 0.5639f) * row - 18.833f) * row + 285.71f)

#define LEFT_EAGE 0			//图像左边界
#define RIGHT_EAGE 187		//图像右边界
#define MIDDLE 94			//图像中值
#define UP_EAGE 20			//图像上边界
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
#define CIRCLE 1			//环岛开关
#define BROKEN 1			//断路开关
#define BLOCK_OPEN 1                         //路障开关

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
extern int CircleFlag;		//CN普通赛道 CL左环岛 CR右环岛
extern int CircleState;		//1远处识别环岛 4入环岛口 5环岛中 6出环岛
extern int BrokenFlag;		//断路标志
extern int BrokenLastAve;

extern int ML_Count;									//中线有效行
extern SpecialPoint LeftPnt, RightPnt;					//保存左右特殊点信息
extern int g_RoadType;
extern int ErrorFlag;								//错误标志位
extern int LastMiddleLine;							//保存上帧中线
extern int DiffThreshold;							//边缘检测阈值
extern int LightThreshold;						//去高光噪声阈值
extern int LightThreshold2;						//去高光噪声阈值2
extern int FindLineType;							//是否加入高光滤波标志

extern int ML[IMG_ROW], LL[IMG_ROW], RL[IMG_ROW];
extern unsigned char ImageEage[IMG_ROW][IMG_COL];
void VarInit(void);



extern unsigned char image[ROW][COL];
extern unsigned char g_Img_All_120x188[ROW][COL];


extern const unsigned char g_Bit_Val_Up[8];
extern const unsigned char g_Bit_Val_Down[8];



//extern int RoadType;
//extern int ErrorFlag;
//extern int MaxSpeedFlag;

//==========================摄像头参数==============================//
extern int exp_time;                           //摄像头曝光时间

//==========================菜单标志=================================//
//==========================菜单标志=================================//
#define InitSteer 1147               //舵机初始值 75Hz 1145 50Hz 766

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
extern int ind_left, ind_right, ind_mid;	//记录电感
extern int ind_leftmax, ind_leftmin, ind_rightmax, ind_rightmin, ind_midmax, ind_midmin;	//电感最值
extern float left_norm, right_norm, mid_norm;
extern float ad_error_1, ad_error_2;
extern int mul_ad_error;         //电磁error放大倍数

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
extern int dialSwitchcal;
//extern signed int DialSwitchFlag1;            //拨码开关1
//extern signed int DialSwitchFlag2;            //拨码开关2
//extern signed int DialSwitchFlag3;            //拨码开关3
//extern signed int DialSwitchFlag4;        
#define DIALSWITCH_PIN1 D8
#define DIALSWITCH_PIN2 D9
#define DIALSWITCH_PIN3 D10
#define DIALSWITCH_PIN4 D15
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
extern unsigned char g_StartMaster;                //主车发车准备
extern unsigned char g_StartSlave;                 //从车发车准备
extern unsigned char g_GetMeetingMaster;            //主车到达会车区标志
extern unsigned char g_GetMeetingSlave;             //从车到达会车区标志
extern unsigned char g_MeetingCtrlFlag;             //会车状态车辆控制标志
extern unsigned char g_MeetingCtrlEndFlag;             //会车状态车辆控制结束标志
extern unsigned char g_EndMeetingMaster;           //主车结束会车标志
extern unsigned char g_MasterOutFlag;              //主车出界
extern unsigned char g_SlaveOutFlag;               //从车出界
extern unsigned char g_StateMaster;
extern unsigned char g_StateSlave;

/*====================================会车区惯导变量=====================================*/
extern int sum_distance_1;
extern int sum_distance_2;
extern int sum_distance_3;
extern int obj_angle_1;
extern int obj_angle_2;
extern int const_error_1;
extern int const_error_2;
extern int max_duty;
/*====================================路障=====================================*/
extern int BlockFlag;                   //路障标志
extern int g_RB_Lduty,g_RB_Rduty;
extern float k1,k2,k3;//调整系数
extern int sum;
extern int sum_dist;
extern int inf_RB_flag;
extern int var;
extern float g_inf;
extern int s1;
extern int st;
extern int stop_inf;
extern unsigned char ObstacleEndFlag;
/*====================================坡道=====================================*/
extern int RampFlag;
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


#endif