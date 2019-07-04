 #include <GlobalVar.h>
#include "headfile.h"

int text_flag = 0;
const unsigned char g_Bit_Val_Up[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//液晶图像压缩;
const unsigned char g_Bit_Val_Down[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

//**********Angle(角度控制)**************//
float g_angle_set=18.09;//车平衡位置角度
float g_angle_set_const = 26;

float g_gyro_ratio = 4.8;
float g_AngleControlOut = 0;


float g_rate_expect = 0;
float g_rate_expect_old = 0;
float g_rate_expect_integal = 0;
float g_rate_expect_integal_old = 0;
float g_angle_old = 0;
float g_error = 0;
float g_gyro_angle_x = 0;
float g_angle = 0;

int g_count = 0;


//串级
float g_AngleOut = 0.0f;
float g_RateP = 39.09;
float g_RateD = 61.69;
float g_angle_P=19.3; 
float g_angle_Psingle=850; 
float g_angle_D=100; 
//单级
float g_single_angle_P = 1;
float g_single_angle_D = 0;
int AngleMutationFlag=1;     //跳变检测
//***********MOTOR（电机模式及其输出及开关）***********//
float g_fleft;
float g_fright;
int g_mode=3;
float g_duty_left=0;
float g_duty_right=0;
//***********Speed(速度控制)**********//
int g_flag=0;  //是否发车
float g_Speed_P = 240;
float g_Speed_I = 0.97;
float g_fSpeedControlOut = 0;//输出速度控制
float g_fSpeed_set = 20;//设置的速度值
float g_nSpeedControlPeriod=-1;
float g_SpeedPeriod=10;
float g_nLeftMotorPulseSigma = 0;//编码器采集的脉冲值
float g_nRightMotorPulseSigma = 0;//编码器采集的脉冲值
float g_errorS = 0;
int MaxSpeed = 3000;
float g_fI = 1000;//积分项暂存处
int Speed_MAX = 3800;//积分限幅



//**************************Direction方向环（摄像头）**************//
float gRateKp = 8.41;            //串级p
float gRateKd = 10.1;            //串级d

float g_dire_P=9;
float g_dire_D=9.2;
float g_errorD=0;//差值
float g_errorCircleland=0;
float g_fDirectionControlOut;
float g_nDirectionControlPeriod=0;//输出平滑
float g_DirectionPeriod=5;//分的段数
float AD_flag = 0;
float g_fDirectionControlOut_new=0;
float g_fDirectionControlOut_before=0;

float Circle_P = 11;//环岛P
//**************************Direction方向环（电感）**************//
float gRateKp_AD = 15;            //串级p
float gRateKd_AD = 10;            //串级d

float g_dire_P_AD=6;
float g_dire_D_AD=10;
int protect_flag=0;
//==========================元素计数变量============================//

unsigned char CircleDir[10];		//环岛计数
unsigned char Ind_CircleOpen = 0;	//电磁判断环岛开关
//==========================图像变量============================//
//#include "GlobalVar.h"
const int MidOffset[] = {
	 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 11, 12, 13, 14, 14, 15, 16, 17, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 52, 53, 54, 55, 56, 57, 58, 59, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93,
};
unsigned char ImageEage[IMG_ROW][IMG_COL];
int ML[IMG_ROW], LL[IMG_ROW], RL[IMG_ROW];   //保存边缘线信息数组
int ML_Count;									//中线有效行
SpecialPoint LeftPnt, RightPnt;					//保存左右特殊点信息
int DiffThreshold = 25;							//边缘检测阈值
int DarkThreshold = 55;                         //暗阈值（路障）
int BrightThreshold = 15;                      //亮阈值（坡道）
int LightThreshold = 80;						//去高光噪声阈值
int LightThreshold2 = 150;						//去高光噪声阈值2
int FindLineType = 0;							//是否加入高光滤波标志
int g_RoadType = 0;
int ErrorFlag = 0;								//错误标志位
//1双侧十字错误  2误判为斜十字  3视野丢失错误  4中线错误  5双侧十字噪点错误  6平十字噪点错误
int LastMiddleLine = 0;

int SpeedRow = 0;			//控速边界行数

int CircleFlag = 0;
int CircleState = 0;
unsigned char Dist_ClearSevenFlag = 0;		//延距清环岛7标志位

int LeftIntLine = 0;		//记录左内跳行数
int RightIntLine = 0;		//记录右内跳行数

int Img_SpecialElemFlag = 0;    //特殊元素标志
int Img_BrokenFlag = 0;			//断路标志
int Img_BlockFlag = 0;			//路障标志
int Img_StopLineFlag = 0;		//停车线标志
int Img_RampFlag = 0;			//坡道标志

int BrokenLastAve = 0;
int StopLineDist = 0;

unsigned char Img_CircleOpen = 1;
unsigned char Img_StraightBrokenOpen = 1;
unsigned char Img_CurveBrokenOpen = 1;
unsigned char Img_StopOpen = 1;
unsigned char Img_BlockOpen = 1;
unsigned char Img_RampOpen = 1;
void VarInit(void)
{
	ML_Count = 0;
	ErrorFlag = 0;
	g_RoadType = 0;
	LeftIntLine = 0;
	RightIntLine = 0;
	//下面某些地方起了关键作用
	LeftPnt.ErrCol = RightPnt.ErrCol = 0;
	LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
	for (int i = 0; i < IMG_ROW; i++)
	{
		LL[i] = LEFT_EAGE;
		RL[i] = RIGHT_EAGE;
		ML[i] = MIDDLE;
	}

}


int StraightFlag = 0;//直道标志位 1为直道
//==========================摄像头参数==============================//
unsigned char image[ROW][COL];        //摄像头数据接收
int exp_time = 300;                           //摄像头曝光时间
int HighThreshold = 50;						//canny高阈值
int LowThreshold = 20;						//canny低阈值
int ControlMid = 90;						//图像控制中值

//==========================菜单标志==============================//

int	g_car_lanuch = 0;			//车辆启动标志
int g_drive_flag = 0;           //电机开关标志
int g_ad_flag = 1;              //电感采集标志
int g_steer_open = 1;           //舵机开关标志
int g_handle_open = 0;          //补图开关标志
int g_ramp_open = 0;            //坡道开关标志
int g_broken_open = 1;          //断路开关标志
int g_block_open = 1;           //路障开关标志
int g_single_open = 0;          //单车模式开关标志
int Flag_max;
int g_camera_open = 1;
int g_ind_open = 1;
int g_camera_ctrl_flag = 0;
int g_ind_ctrl_flag = 0;
int g_broken_enable = 0;
int BugFlag = 0;

int speed_type = 1;
//int RoadType = 0;
//int MaxSpeedFlag = 0;     //高速标志
//
//float Swidth = 0.5;

//====================================舵机控制================================//
int SteerMode = 0;      //舵机控制模式 0摄像头 1电磁
int Error_1 = 0;
int Error_2 = 0;
float Steer_P = 2;
float Steer_P2 = 0;
float p_max = 0;
float p_min = 0;
float p_long = 0;
float d_max = 0;
float Steer_D = 0;
float Steer_D1 = 0;     //入弯
float Steer_D2 = 0;     //出弯
int ProSpect= 55;
int SteerDuty = InitSteer;
float long_steer_p = 0.5;
float long_steer_d = 0.5;

//=====================================电感===================================//
float ad_error_1 = 0, ad_error_2 = 0;
int mul_ad_error = 100;         //电磁error放大倍数

int ind_left_line,//记录电感
    ind_left_column,
    ind_right_line,
    ind_right_column,
    ind_mid;

int ind_left_line_max,ind_left_line_min,  
    ind_left_column_max,ind_left_column_min ,
    ind_right_line_max,ind_right_line_min,
    ind_right_column_max ,ind_right_column_min,
    ind_mid_max,ind_mid_min;//电感最值
float  left_line_norm,left_column_norm,right_line_norm,right_column_norm,mid_norm ;//归一化
float ind_mid_flag = 0,ind_left_flag = 0,ind_right_flag = 0;
int CircleIsland_into_flag = 0;
/*=====================================增量式速度控制========================================*/
int KDet = 10;

float curSpeed=0;
int lCurSpeed=0;
int rCurSpeed=0;
int sidetype=1;
SPEED leftDrv = {0, 0, 0, 0, 0};
SPEED rightDrv = {0, 0, 0, 0, 0};

int g_steer_error = 0;

//期望速度
int spdExp=0;
int spdExp0=10;
int spdExp1 = 0;
int spdExp2=22;
int spdExp3=5;
int spdExp4=0;

float leftExpect; //左轮期望速度
float rightExpect;//右轮期望速度
signed int leftSpeedOut = 0; //左轮速度输出
signed int rightSpeedOut = 0;//右轮速度输出
//速度控制pi
float g_speed_p=280;
float g_speed_i=7;
float g_speed_d = 0;

signed int lOverflowFlg=0;//左轮遇限方向积分溢出标志
signed int rOverflowFlg=0;//右轮遇限方向积分溢出标志
//int s4=0;
//int s5 =0;
//int s6=0;
//int s7=0;

int d_spd_max = 0;
int d_spd_high = 50;
int d_spd_mid = 0;
int d_spd_min = 15;

int s_width = 15;
int m_width = 50;
int l_width = 100;

//==================================刹车=================//
int g_stop_line = 0;
int g_stop_dis = 0;
int times_go = 0;
int times_stop = 0;
int rount = 1;          //圈数
int DistantStop = 0;		//累计编码器脉冲

//==========================TF卡============================//
unsigned char g_Img_All_120x188[ROW][COL];


//==========================硬件参数区============================//
unsigned char image[ROW][COL];        //摄像头数据接收
/*=====================================双车通讯======================================*/

signed char btMessage = 49;
signed char btRcvMessage =0;
btInfo btSndInfo={0};
btInfo btRcvInfo={0};
/*===================================拨码开关===========================================*/
int dialSwitchFlg1=0;
int dialSwitchFlg2=0;
int dialSwitchFlg3=0;
int dialSwitchFlg4=0;
int dialSwitchFlg5=0;
int dialSwitchFlg6=0;
int dialSwitchFlg7=0;
int dialSwitchFlg8=0;
int dialSwitchcal=0;

/*=====================================双车状态======================================*/
unsigned char g_MasterOutFlag = 0;              //主车出界
unsigned char g_SlaveOutFlag = 0;               //从车出界
unsigned char g_StateMaster = 0;		//主车状态
unsigned char g_StateSlave = 0;			//从车状态
unsigned char g_MeetingMode = 2;
unsigned char g_MeetingDir = 2;			//转向方向 2为右转 1为左转
unsigned char g_GetMeetingFlag = 0;	//进入会车标志 通过其他动作改变
unsigned char g_GetMeetingState = 0;	//已经会车 0未会车 1已经会车 识别会车区后变为1

/*====================================会车区惯导变量=====================================*/
int sum_distance_1 = 3400;
int sum_distance_2 = 2000;
int sum_distance_3 = 2000;
int obj_angle_1 = 30;
int obj_angle_2 = -180;
int const_error_1 = -20;
int const_error_2 = 40;
int max_duty = 8000;
int OutMeetingDistance = 7000;
float GroundAngle = 9.73;
/*====================================路障=====================================*/
int sum = 0;
int sum_dist = 4500;
int g_inf = 0;
int st = -40;
int stop_inf = 900;

/**************电磁环岛*************/
float Circlelanderror=0;
int circlelandflag=0;
float circlelandSigma=0;
int circlelandtimecount=0;
int circlelandget=0; 
int circlelandnumber=0;
int circlelandsymbol=0;

uint8 phototube=0;
/*************====================坡道路障相关可控制变量=======================*************/
int BrokenTurnTailPWM=2900;
int BrokenTurnTailDistance=500;
float Rampangle=12;

/*************====================开机加速相关变量=======================*************/
int BootRacerFlag=0;
int BootRacerDistance=5000;
int BootRacerAngle=-11;