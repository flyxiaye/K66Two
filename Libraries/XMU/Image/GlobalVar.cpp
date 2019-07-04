 #include <GlobalVar.h>
#include "headfile.h"

int text_flag = 0;
const unsigned char g_Bit_Val_Up[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//Һ��ͼ��ѹ��;
const unsigned char g_Bit_Val_Down[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

//**********Angle(�Ƕȿ���)**************//
float g_angle_set=18.09;//��ƽ��λ�ýǶ�
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


//����
float g_AngleOut = 0.0f;
float g_RateP = 39.09;
float g_RateD = 61.69;
float g_angle_P=19.3; 
float g_angle_Psingle=850; 
float g_angle_D=100; 
//����
float g_single_angle_P = 1;
float g_single_angle_D = 0;
int AngleMutationFlag=1;     //������
//***********MOTOR�����ģʽ������������أ�***********//
float g_fleft;
float g_fright;
int g_mode=3;
float g_duty_left=0;
float g_duty_right=0;
//***********Speed(�ٶȿ���)**********//
int g_flag=0;  //�Ƿ񷢳�
float g_Speed_P = 240;
float g_Speed_I = 0.97;
float g_fSpeedControlOut = 0;//����ٶȿ���
float g_fSpeed_set = 20;//���õ��ٶ�ֵ
float g_nSpeedControlPeriod=-1;
float g_SpeedPeriod=10;
float g_nLeftMotorPulseSigma = 0;//�������ɼ�������ֵ
float g_nRightMotorPulseSigma = 0;//�������ɼ�������ֵ
float g_errorS = 0;
int MaxSpeed = 3000;
float g_fI = 1000;//�������ݴ洦
int Speed_MAX = 3800;//�����޷�



//**************************Direction���򻷣�����ͷ��**************//
float gRateKp = 8.41;            //����p
float gRateKd = 10.1;            //����d

float g_dire_P=9;
float g_dire_D=9.2;
float g_errorD=0;//��ֵ
float g_errorCircleland=0;
float g_fDirectionControlOut;
float g_nDirectionControlPeriod=0;//���ƽ��
float g_DirectionPeriod=5;//�ֵĶ���
float AD_flag = 0;
float g_fDirectionControlOut_new=0;
float g_fDirectionControlOut_before=0;

float Circle_P = 11;//����P
//**************************Direction���򻷣���У�**************//
float gRateKp_AD = 15;            //����p
float gRateKd_AD = 10;            //����d

float g_dire_P_AD=6;
float g_dire_D_AD=10;
int protect_flag=0;
//==========================Ԫ�ؼ�������============================//

unsigned char CircleDir[10];		//��������
unsigned char Ind_CircleOpen = 0;	//����жϻ�������
//==========================ͼ�����============================//
//#include "GlobalVar.h"
const int MidOffset[] = {
	 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 11, 12, 13, 14, 14, 15, 16, 17, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 52, 53, 54, 55, 56, 57, 58, 59, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93,
};
unsigned char ImageEage[IMG_ROW][IMG_COL];
int ML[IMG_ROW], LL[IMG_ROW], RL[IMG_ROW];   //�����Ե����Ϣ����
int ML_Count;									//������Ч��
SpecialPoint LeftPnt, RightPnt;					//���������������Ϣ
int DiffThreshold = 25;							//��Ե�����ֵ
int DarkThreshold = 55;                         //����ֵ��·�ϣ�
int BrightThreshold = 15;                      //����ֵ���µ���
int LightThreshold = 80;						//ȥ�߹�������ֵ
int LightThreshold2 = 150;						//ȥ�߹�������ֵ2
int FindLineType = 0;							//�Ƿ����߹��˲���־
int g_RoadType = 0;
int ErrorFlag = 0;								//�����־λ
//1˫��ʮ�ִ���  2����Ϊбʮ��  3��Ұ��ʧ����  4���ߴ���  5˫��ʮ��������  6ƽʮ��������
int LastMiddleLine = 0;

int SpeedRow = 0;			//���ٱ߽�����

int CircleFlag = 0;
int CircleState = 0;
unsigned char Dist_ClearSevenFlag = 0;		//�Ӿ��廷��7��־λ

int LeftIntLine = 0;		//��¼����������
int RightIntLine = 0;		//��¼����������

int Img_SpecialElemFlag = 0;    //����Ԫ�ر�־
int Img_BrokenFlag = 0;			//��·��־
int Img_BlockFlag = 0;			//·�ϱ�־
int Img_StopLineFlag = 0;		//ͣ���߱�־
int Img_RampFlag = 0;			//�µ���־

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
	//����ĳЩ�ط����˹ؼ�����
	LeftPnt.ErrCol = RightPnt.ErrCol = 0;
	LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
	for (int i = 0; i < IMG_ROW; i++)
	{
		LL[i] = LEFT_EAGE;
		RL[i] = RIGHT_EAGE;
		ML[i] = MIDDLE;
	}

}


int StraightFlag = 0;//ֱ����־λ 1Ϊֱ��
//==========================����ͷ����==============================//
unsigned char image[ROW][COL];        //����ͷ���ݽ���
int exp_time = 300;                           //����ͷ�ع�ʱ��
int HighThreshold = 50;						//canny����ֵ
int LowThreshold = 20;						//canny����ֵ
int ControlMid = 90;						//ͼ�������ֵ

//==========================�˵���־==============================//

int	g_car_lanuch = 0;			//����������־
int g_drive_flag = 0;           //������ر�־
int g_ad_flag = 1;              //��вɼ���־
int g_steer_open = 1;           //������ر�־
int g_handle_open = 0;          //��ͼ���ر�־
int g_ramp_open = 0;            //�µ����ر�־
int g_broken_open = 1;          //��·���ر�־
int g_block_open = 1;           //·�Ͽ��ر�־
int g_single_open = 0;          //����ģʽ���ر�־
int Flag_max;
int g_camera_open = 1;
int g_ind_open = 1;
int g_camera_ctrl_flag = 0;
int g_ind_ctrl_flag = 0;
int g_broken_enable = 0;
int BugFlag = 0;

int speed_type = 1;
//int RoadType = 0;
//int MaxSpeedFlag = 0;     //���ٱ�־
//
//float Swidth = 0.5;

//====================================�������================================//
int SteerMode = 0;      //�������ģʽ 0����ͷ 1���
int Error_1 = 0;
int Error_2 = 0;
float Steer_P = 2;
float Steer_P2 = 0;
float p_max = 0;
float p_min = 0;
float p_long = 0;
float d_max = 0;
float Steer_D = 0;
float Steer_D1 = 0;     //����
float Steer_D2 = 0;     //����
int ProSpect= 55;
int SteerDuty = InitSteer;
float long_steer_p = 0.5;
float long_steer_d = 0.5;

//=====================================���===================================//
float ad_error_1 = 0, ad_error_2 = 0;
int mul_ad_error = 100;         //���error�Ŵ���

int ind_left_line,//��¼���
    ind_left_column,
    ind_right_line,
    ind_right_column,
    ind_mid;

int ind_left_line_max,ind_left_line_min,  
    ind_left_column_max,ind_left_column_min ,
    ind_right_line_max,ind_right_line_min,
    ind_right_column_max ,ind_right_column_min,
    ind_mid_max,ind_mid_min;//�����ֵ
float  left_line_norm,left_column_norm,right_line_norm,right_column_norm,mid_norm ;//��һ��
float ind_mid_flag = 0,ind_left_flag = 0,ind_right_flag = 0;
int CircleIsland_into_flag = 0;
/*=====================================����ʽ�ٶȿ���========================================*/
int KDet = 10;

float curSpeed=0;
int lCurSpeed=0;
int rCurSpeed=0;
int sidetype=1;
SPEED leftDrv = {0, 0, 0, 0, 0};
SPEED rightDrv = {0, 0, 0, 0, 0};

int g_steer_error = 0;

//�����ٶ�
int spdExp=0;
int spdExp0=10;
int spdExp1 = 0;
int spdExp2=22;
int spdExp3=5;
int spdExp4=0;

float leftExpect; //���������ٶ�
float rightExpect;//���������ٶ�
signed int leftSpeedOut = 0; //�����ٶ����
signed int rightSpeedOut = 0;//�����ٶ����
//�ٶȿ���pi
float g_speed_p=280;
float g_speed_i=7;
float g_speed_d = 0;

signed int lOverflowFlg=0;//�������޷�����������־
signed int rOverflowFlg=0;//�������޷�����������־
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

//==================================ɲ��=================//
int g_stop_line = 0;
int g_stop_dis = 0;
int times_go = 0;
int times_stop = 0;
int rount = 1;          //Ȧ��
int DistantStop = 0;		//�ۼƱ���������

//==========================TF��============================//
unsigned char g_Img_All_120x188[ROW][COL];


//==========================Ӳ��������============================//
unsigned char image[ROW][COL];        //����ͷ���ݽ���
/*=====================================˫��ͨѶ======================================*/

signed char btMessage = 49;
signed char btRcvMessage =0;
btInfo btSndInfo={0};
btInfo btRcvInfo={0};
/*===================================���뿪��===========================================*/
int dialSwitchFlg1=0;
int dialSwitchFlg2=0;
int dialSwitchFlg3=0;
int dialSwitchFlg4=0;
int dialSwitchFlg5=0;
int dialSwitchFlg6=0;
int dialSwitchFlg7=0;
int dialSwitchFlg8=0;
int dialSwitchcal=0;

/*=====================================˫��״̬======================================*/
unsigned char g_MasterOutFlag = 0;              //��������
unsigned char g_SlaveOutFlag = 0;               //�ӳ�����
unsigned char g_StateMaster = 0;		//����״̬
unsigned char g_StateSlave = 0;			//�ӳ�״̬
unsigned char g_MeetingMode = 2;
unsigned char g_MeetingDir = 2;			//ת���� 2Ϊ��ת 1Ϊ��ת
unsigned char g_GetMeetingFlag = 0;	//����ᳵ��־ ͨ�����������ı�
unsigned char g_GetMeetingState = 0;	//�Ѿ��ᳵ 0δ�ᳵ 1�Ѿ��ᳵ ʶ��ᳵ�����Ϊ1

/*====================================�ᳵ���ߵ�����=====================================*/
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
/*====================================·��=====================================*/
int sum = 0;
int sum_dist = 4500;
int g_inf = 0;
int st = -40;
int stop_inf = 900;

/**************��Ż���*************/
float Circlelanderror=0;
int circlelandflag=0;
float circlelandSigma=0;
int circlelandtimecount=0;
int circlelandget=0; 
int circlelandnumber=0;
int circlelandsymbol=0;

uint8 phototube=0;
/*************====================�µ�·����ؿɿ��Ʊ���=======================*************/
int BrokenTurnTailPWM=2900;
int BrokenTurnTailDistance=500;
float Rampangle=12;

/*************====================����������ر���=======================*************/
int BootRacerFlag=0;
int BootRacerDistance=5000;
int BootRacerAngle=-11;