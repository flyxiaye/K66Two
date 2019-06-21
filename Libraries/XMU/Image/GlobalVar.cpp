#include <GlobalVar.h>
#include "headfile.h"

const unsigned char g_Bit_Val_Up[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//Һ��ͼ��ѹ��;
const unsigned char g_Bit_Val_Down[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

//**********Angle(�Ƕȿ���)**************//
float g_angle_set=21;//��ƽ��λ�ýǶ�
float g_angle_set_const = 26;
float g_angle_P=280; 
float g_angle_D=30; 
float g_angle_I=0; 
float g_gyro_ratio = 4.8;
float g_AngleControlOut = 0;
float g_angle_x=0;
float g_angle_y=0;
float g_angle_z=0;
float g_anglerate_x=0;
float g_anglerate_y=0;
float g_anglerate_z=0;
float g_anglerate_P=0;
float g_anglerate_D=0;
float g_anglerate_I=0;
float g_rate_expect = 0;
float g_rate_expect_old = 0;
float g_rate_expect_integal = 0;
float g_rate_expect_integal_old = 0;
float g_angle_old = 0;
float g_error = 0;
float g_gyro_angle_x = 0;
float g_angle = 0;
float g_acc_zy = 0;//���ٶ�?
float g_acc_zx = 0;
int g_count = 0;


//����
float g_AngleOut = 0.0f;
float g_RateP = 60.0;
float g_RateD = 10.0;
//***********MOTOR�����ģʽ������������أ�***********//
float g_fleft;
float g_fright;
int g_mode=1;
float g_duty_left=0;
float g_duty_right=0;
//***********Speed(�ٶȿ���)**********//
//int g_flag=0;  //�Ƿ񷢳�
float g_Speed_P = 0;
float g_Speed_I = 0.0;
float g_fSpeedControlOut = 0;//����ٶȿ���
float g_fSpeed_set = 0.0;//���õ��ٶ�ֵ
float g_nSpeedControlPeriod=-1;
float g_SpeedPeriod=10;
float g_nLeftMotorPulseSigma = 0;//�������ɼ�������ֵ
float g_nRightMotorPulseSigma = 0;//�������ɼ�������ֵ
float g_errorS = 0;
int MaxSpeed = 6000;


//**********speed(�ٶȿ���)**************//
float g_speed_error = 0;
float g_speed_error_i = 0;
float g_speed_error_p = 0;
float g_speed_error_d = 0;

//**************************Direction���򻷣�����ͷ��**************//
float gRateKp = 0.0;            //����p
float gRateKd = 0.0;            //����d

float g_dire_P=0;
float g_dire_D=-0;
float g_errorD=0;//��ֵ
float g_fDirectionControlOut;
float g_nDirectionControlPeriod=0;//���ƽ��
float g_DirectionPeriod=5;//�ֵĶ���
float AD_flag = 0;
//==========================ͼ�����============================//
#include "GlobalVar.h"
unsigned char ImageEage[IMG_ROW][IMG_COL];
int ML[IMG_ROW], LL[IMG_ROW], RL[IMG_ROW];   //�����Ե����Ϣ����
int ML_Count;									//������Ч��
SpecialPoint LeftPnt, RightPnt;					//���������������Ϣ
int DiffThreshold = 25;							//��Ե�����ֵ
int LightThreshold = 80;						//ȥ�߹�������ֵ
int LightThreshold2 = 150;						//ȥ�߹�������ֵ2
int FindLineType = 0;							//�Ƿ����߹��˲���־
int g_RoadType = 0;
int ErrorFlag = 0;								//�����־λ
//1˫��ʮ�ִ���  2����Ϊбʮ��  3��Ұ��ʧ����
int LastMiddleLine = 0;

int CircleFlag = 0;             //������־
int CircleState = 0;
int BrokenFlag = 0;           //��·��־
int BrokenLastAve = 0;


void VarInit(void)
{
        ML_Count = 0;
	ErrorFlag = 0;
        g_RoadType = 0;
        
        LeftPnt.ErrCol = RightPnt.ErrCol = 0;
        LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
        for (int i = 0; i < IMG_ROW; i++)
        {
          LL[i] = LEFT_EAGE;
          RL[i] = RIGHT_EAGE;
          ML[i] = MIDDLE;
        }
}
//==========================����ͷ����==============================//
int exp_time = 300;                           //����ͷ�ع�ʱ��

//==========================�˵���־==============================//


int g_drive_flag = 0;           //������ر�־
int g_ad_flag = 0;              //��вɼ���־
int g_steer_open = 1;           //������ر�־
int g_handle_open = 0;          //��ͼ���ر�־
int g_ramp_open = 1;            //�µ����ر�־
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

//int ErrorFlag = 0;
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
int ind_left, ind_right, ind_mid;	//��¼���
int ind_leftmax, ind_leftmin, ind_rightmax, ind_rightmin, ind_midmax, ind_midmin;	//�����ֵ
float left_norm, right_norm, mid_norm;
float ad_error_1 = 0, ad_error_2 = 0;
int mul_ad_error = 100;         //���error�Ŵ���

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
int dialSwitchcal=0;

/*=====================================˫��״̬======================================*/
unsigned char g_StartMaster = 0;                //��������׼��
unsigned char g_StartSlave = 1;                 //�ӳ�����׼��
unsigned char g_GetMeetingMaster = 0;            //��������ᳵ����־
unsigned char g_GetMeetingSlave = 0;             //�ӳ�����ᳵ����־
unsigned char g_MeetingCtrlFlag = 0;             //�ᳵ״̬�������Ʊ�־
unsigned char g_MeetingCtrlEndFlag = 0;             //�ᳵ״̬�������ƽ�����־
unsigned char g_EndMeetingMaster = 0;           //���������ᳵ��־
unsigned char g_MasterOutFlag = 0;              //��������
unsigned char g_SlaveOutFlag = 0;               //�ӳ�����
unsigned char g_StateMaster = 0;
unsigned char g_StateSlave = 0;

/*====================================�ᳵ���ߵ�����=====================================*/
int sum_distance_1 = 3400;
int sum_distance_2 = 2000;
int sum_distance_3 = 2000;
int obj_angle_1 = 30;
int obj_angle_2 = -180;
int const_error_1 = -20;
int const_error_2 = 40;
int max_duty = 1300;
/*====================================·��=====================================*/
int BlockFlag = 0;              //·�ϱ�־
int g_RB_Lduty = 0,g_RB_Rduty = 0;
float k1 = 30,k2 = 54,k3 = 63;//����ϵ��
int sum = 0;
int sum_dist = 4500;
int inf_RB_flag = 0;
int var = 0;
float g_inf = 0;
int s1 = 15;
int st = -40;
int stop_inf = 90;
unsigned char ObstacleEndFlag = 0;
/*====================================�µ�=====================================*/
int RampFlag = 0;

