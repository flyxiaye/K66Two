#ifndef __GLOBALVAR_H__
#define __GLOBALVAR_H__


#include <math.h>


#define ROW             120             //ԭʼͼ��������������              
#define COL             188             //ԭʼͼ�������������� 

extern unsigned char image[ROW][COL];
extern unsigned char g_Img_All_120x188[ROW][COL];


extern const unsigned char g_Bit_Val_Up[8];
extern const unsigned char g_Bit_Val_Down[8];

//**********Angle(�Ƕȿ���)**************//
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
extern float g_acc_zy;//���ٶ�?
extern float g_acc_zx;
extern int g_count;
extern float g_angle_set_const;
//����
extern float g_AngleOut;
extern float g_RateP ;
extern float g_RateD;

//***********MOTOR�����ģʽ������������أ�***********//
extern float g_fleft;
extern float g_fright;
extern int g_mode;
extern float g_duty_left;
extern float g_duty_right;
//***********Speed(�ٶȿ���)**********//
extern float g_Speed_P;
extern float g_Speed_I;
extern float g_fSpeedControlOut;//����ٶȿ���
extern float g_fSpeed_set;//���õ��ٶ�ֵ
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
//**************************Direction���򻷣�����ͷ��**************//
extern float gRateKp;            //����p
extern float gRateKd;            //����d

extern int g_need ;//����ͷȡ������
extern float g_dire_P;
extern float g_dire_D;
extern float g_errorD;//��ֵ
extern float g_fDirectionControlOut;
extern float g_nDirectionControlPeriod;//���ƽ��
extern float g_DirectionPeriod;//�ֵĶ���
extern float AD_flag;






//==========================ͼ�����============================//
#define ROW             120             //ԭʼͼ��������������              
#define COL             188             //ԭʼͼ�������������� 
//#define F_INT(num)	(signed int)(num + 0.5f)
//#define Line_To_Dis(row) F_INT((((0.00004f * row - 0.0079f) * row + 0.5639f) * row - 18.833f) * row + 285.71f)

#define LEFT_EAGE 0			//ͼ����߽�
#define RIGHT_EAGE 187		//ͼ���ұ߽�
#define MIDDLE 94			//ͼ����ֵ
#define UP_EAGE 20			//ͼ���ϱ߽�
#define DOWN_EAGE 70		//ͼ���±߽�
#define IMG_ROW 120			//ͼ������
#define IMG_COL 188			//ͼ������
//#define FIND_LEFT 0
//#define FIND_RIGHT 1

#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#define MIN(x, y) ((x) < (y)) ? (x) : (y)

#define FINDLINE_TH 7		//������ֵ
#define IN_OFFSET 2 //����ƫ��
//#define OUT_OFFSET 6 //����ƫ��
#define CIRCLEUP_TH 15		//���뻷�������
#define CIRCLE 1			//��������
#define BROKEN 1			//��·����
#define BLOCK_OPEN 1                         //·�Ͽ���

typedef struct
{
	int Row;
	int Col;
}Point;

typedef struct
{
	int Type;		//0��ͨ��� 1���� 2���� 3�յ� 4�ϱ߽� 5��߽� 6�ұ߽� 7���ж��� 8��4��ʮ�ֲ��� 9����ѭ��
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
extern int CircleFlag;		//CN��ͨ���� CL�󻷵� CR�һ���
extern int CircleState;		//1Զ��ʶ�𻷵� 4�뻷���� 5������ 6������
extern int BrokenFlag;		//��·��־
extern int BrokenLastAve;

extern int ML_Count;									//������Ч��
extern SpecialPoint LeftPnt, RightPnt;					//���������������Ϣ
extern int g_RoadType;
extern int ErrorFlag;								//�����־λ
extern int LastMiddleLine;							//������֡����
extern int DiffThreshold;							//��Ե�����ֵ
extern int LightThreshold;						//ȥ�߹�������ֵ
extern int LightThreshold2;						//ȥ�߹�������ֵ2
extern int FindLineType;							//�Ƿ����߹��˲���־

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

//==========================����ͷ����==============================//
extern int exp_time;                           //����ͷ�ع�ʱ��

//==========================�˵���־=================================//
//==========================�˵���־=================================//
#define InitSteer 1147               //�����ʼֵ 75Hz 1145 50Hz 766

extern int g_drive_flag;                //�������
extern int g_ad_flag;                   //��п���
extern int g_steer_open;                //�������
extern int g_handle_open;               //��ͼ����
extern int g_ramp_open;            //�µ����ر�־
extern int g_broken_open;          //��·���ر�־
extern int g_block_open;           //·�Ͽ��ر�־
extern int g_single_open;          //����ģʽ���ر�־
extern int Flag_max;
extern int g_camera_open;
extern int g_ind_open;
extern int g_camera_ctrl_flag;
extern int g_ind_ctrl_flag;
extern int g_broken_enable;
     
extern int BugFlag;

//=================================�������======================================//
extern int SteerMode;      //�������ģʽ 0����ͷ 1���
extern int Error_1;
extern int Error_2;
extern float Steer_P;
extern float Steer_P2;
extern float p_max;
extern float p_min;
extern float p_long;
extern float d_max;
extern float Steer_D;
extern float Steer_D1;                  //����
extern float Steer_D2;                  //����
extern int ProSpect;
extern int SteerDuty;
extern float long_steer_p;
extern float long_steer_d;

//=====================================���===================================//
extern int ind_left, ind_right, ind_mid;	//��¼���
extern int ind_leftmax, ind_leftmin, ind_rightmax, ind_rightmin, ind_midmax, ind_midmin;	//�����ֵ
extern float left_norm, right_norm, mid_norm;
extern float ad_error_1, ad_error_2;
extern int mul_ad_error;         //���error�Ŵ���

/*=====================================����ʽ�ٶȿ���========================================*/
extern int KDet;

//�������޷�
#define SPEED_OUT_MAX  9999  //��ת���
#define SPEED_OUT_MIN -9999  //��ת���


extern int g_steer_error; //���ƫ��

typedef struct
{
    float error_1;//ƫ��
    float error_2;
    float error_3;
    int d_duty; //���ƫ����
     float p;
     float i;
     float d;
}SPEED;

extern int speed_type;

extern float curSpeed;
extern int lCurSpeed;
extern int rCurSpeed;
extern int sidetype;//��������
extern SPEED leftDrv;
extern SPEED rightDrv;
//�����ٶ�
extern int spdExp;
extern int spdExp0;
extern int spdExp1;
extern int spdExp2;
extern int spdExp3;
extern int spdExp4;
extern float leftExpect; //���������ٶ�
extern float rightExpect;//���������ٶ�

extern signed int leftSpeedOut; //�����ٶ����
extern signed int rightSpeedOut;//�����ٶ����
//�ٶȿ���pi
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

//==================================ɲ��=================//
extern int g_stop_line;
extern int g_stop_dis;
extern int times_go;
extern int times_stop;
extern int rount;          //Ȧ��
extern int DistantStop;

//*****************TF������*******************
#define GATHER_DATA_KIND    10                          //�ɼ��ı�������

extern unsigned int g_SD_Gather_Data[GATHER_DATA_KIND]; //�ɼ�������
extern unsigned char g_Img_All_128x320[128*160];        //SD��������ͼ����
extern unsigned char g_Img_All_240x320[240*160];        //SD��������ͼ����
extern unsigned char g_Img_All_60x80[60*40]; 


/*=====================================���뿪��========================================*/
extern int dialSwitchFlg1;
extern int dialSwitchFlg2;
extern int dialSwitchFlg3;
extern int dialSwitchFlg4;
extern int dialSwitchcal;
//extern signed int DialSwitchFlag1;            //���뿪��1
//extern signed int DialSwitchFlag2;            //���뿪��2
//extern signed int DialSwitchFlag3;            //���뿪��3
//extern signed int DialSwitchFlag4;        
#define DIALSWITCH_PIN1 D8
#define DIALSWITCH_PIN2 D9
#define DIALSWITCH_PIN3 D10
#define DIALSWITCH_PIN4 D15
/*===================================˫��ͨѶ��=========================================*/
typedef struct 
{
  signed char infotype;// 1.      0������� 1�����־λ
  char begin;           // 4.      ͨ������
  char state;           // 5-8.      
}btInfo;

extern signed char btMessage ;

extern signed char btRcvMessage;

extern btInfo btSndInfo,btRcvInfo;
/*=====================================˫��״̬======================================*/
extern unsigned char g_StartMaster;                //��������׼��
extern unsigned char g_StartSlave;                 //�ӳ�����׼��
extern unsigned char g_GetMeetingMaster;            //��������ᳵ����־
extern unsigned char g_GetMeetingSlave;             //�ӳ�����ᳵ����־
extern unsigned char g_MeetingCtrlFlag;             //�ᳵ״̬�������Ʊ�־
extern unsigned char g_MeetingCtrlEndFlag;             //�ᳵ״̬�������ƽ�����־
extern unsigned char g_EndMeetingMaster;           //���������ᳵ��־
extern unsigned char g_MasterOutFlag;              //��������
extern unsigned char g_SlaveOutFlag;               //�ӳ�����
extern unsigned char g_StateMaster;
extern unsigned char g_StateSlave;

/*====================================�ᳵ���ߵ�����=====================================*/
extern int sum_distance_1;
extern int sum_distance_2;
extern int sum_distance_3;
extern int obj_angle_1;
extern int obj_angle_2;
extern int const_error_1;
extern int const_error_2;
extern int max_duty;
/*====================================·��=====================================*/
extern int BlockFlag;                   //·�ϱ�־
extern int g_RB_Lduty,g_RB_Rduty;
extern float k1,k2,k3;//����ϵ��
extern int sum;
extern int sum_dist;
extern int inf_RB_flag;
extern int var;
extern float g_inf;
extern int s1;
extern int st;
extern int stop_inf;
extern unsigned char ObstacleEndFlag;
/*====================================�µ�=====================================*/
extern int RampFlag;
/*=====================================�����Ǳ���======================================*/
extern signed int I2C_Wait_Times;        //i2c�ȴ�����
extern signed int I2C_Wait_Err_Flg;      //i2c�ȴ���ѭ�������־

extern float g_gyroBalance;           //������ƽ��ֵ
extern float g_gyroIntegration;             //�����ǻ�����

extern int g_gyroDimension;               //������ת�����ٶȼ�����

extern float g_angleResult;                 //�Ƕȼ�����
extern float g_angleResult_last;
extern float g_angle_temp;

extern int g_gyroGet;                     //�����Ƕ�ȡֵ
extern signed int g_gyroGet_max;
extern signed int g_gyroGet_min;
extern int g_gyro_jolt;
extern int g_jolt_flag;

extern signed int gyroClearRamp;         //�����Ǽ�����µ���־

extern int g_gyro_cnt;//�µ�


extern int g_ramp_judge;

//==========================���Ե��============================//
extern int LeftPWM;
extern int RightPWM;
extern int LeftPWM2;
extern int RightPWM2;
extern int EncoderL;
extern int EncoderR;

//**********speed(�ٶȿ���)**************//
extern float g_speed_error;
extern float g_speed_error_i;
extern float g_speed_error_p;
extern float g_speed_error_d;
///*=====================================�ٶȿ���========================================*/
//
//extern int opposite_ready,opposite_wait,opposite_done;
//extern int KDet;
//
////�������޷�
//#define SPEED_OUT_MAX  9999  //��ת���
//#define SPEED_OUT_MIN -9999  //��ת���
//#define IOUT_MAX       9500  //�������
//#define IOUT_MIN      -6000  //������С
//
//
//
//extern int g_steer_error; //���ƫ��
//
//typedef struct
//{
//    float error;//ƫ��
//    float iError;//ƫ�������
//    float iOut;//���������
//    
//     float p;
//     float i;
//}SPEED;
//
//
//extern float curSpeed;
//extern int lCurSpeed;
//extern int rCurSpeed;
//extern int sidetype;//��������
//extern SPEED leftDrv;
//extern SPEED rightDrv;
////�����ٶ�
//extern int spdExp;
//extern int spdExp0;
//extern int spdExp1;
//extern int spdExp2;
//extern int spdExp3;
//extern int spdExp4;
//extern float leftExpect; //���������ٶ�
//extern float rightExpect;//���������ٶ�
//
//extern signed int leftSpeedOut; //�����ٶ����
//extern signed int rightSpeedOut;//�����ٶ����
////�ٶȿ���pi
//extern float g_speed_p;
//extern float g_speed_i;
//
//extern signed int lOverflowFlg;//�������޷�����������־
//extern signed int rOverflowFlg;//�������޷�����������־
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



//==========================Ӳ��������============================//

extern signed int DialSwitchFlag1;            //���뿪��1
extern signed int DialSwitchFlag2;            //���뿪��2
extern signed int DialSwitchFlag3;            //���뿪��3
extern signed int DialSwitchFlag4;        
//#define DIALSWITCH_PIN1 E6
//#define DIALSWITCH_PIN2 E7
//#define DIALSWITCH_PIN3 E8
//#define DIALSWITCH_PIN4 E9


#endif