#include "headfile.h"
#include "Meeting.h"
#include "Obstacle.h"

#define GroundAngle 9.73
#define MEETING_SPEED 10
#define _ANGLE imu_data.yaw

int StartDistance = 0.5 * ONE_METER;

//================================================================//
//  @brief  :		��ͼ������ᳵ��������
//  @param  :		void
//  @return :		void
//  @note   :		���ڲ�ͼ����	
//================================================================//
void MeetingToImage(void)
{
	if (!g_single_open)			//�ǵ���ģʽ
	{
		if (0 == g_GetMeetingState && !g_GetMeetingFlag			//δ��ʼ�ᳵ
			&& 2 == Img_BrokenFlag && CarGo == g_StateMaster)
			g_GetMeetingFlag = 1;
		else if (g_GetMeetingState && !g_GetMeetingFlag			//�Ѿ���ɻᳵ������ʶ���·
			&& Img_BrokenFlag)
			Img_BrokenFlag = 0;

		if (g_GetMeetingState && 1 == Img_StopLineFlag && WaitingStop <= g_StateMaster)		//�Ѿ���ɻᳵ����ʼʶ��ͣ����
			;
		else Img_StopLineFlag = 0;
	}
}


//================================================================//
//  @brief  :		���ֳ��ᳵ����(��ͷ)
//  @param  :		void
//  @return :		void
//  @note   :		void	
//================================================================//
void MeetingTwo1(void)
{
	static int acc_speed = 0;
	static int sum_speed = 0.5 * ONE_METER;
	static float angle_init = 0;
	static int get_flag = 0;
	static int count = 0;
	switch (g_StateMaster)
	{
	case Ready:
		if (g_car_lanuch) g_StateMaster = WaitingBegin;
		break;
	case WaitingBegin:
		if (g_StateSlave > Ready)
		{
			g_StateMaster = CarGo;
			g_drive_flag = 1;
		}
		break;
	case CarGo:
		if (g_GetMeetingFlag) 			 //����ᳵ�� 
		{
			acc_speed += curSpeed;
			if (acc_speed > StartDistance)
			{
				g_StateMaster = StateOne;
				Img_BlockFlag = 0;
//				g_handle_open = 0;
				acc_speed = 0;
				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 1;
			}
		}
		else acc_speed = 0;
		break;
	case StateOne:		//�ᳵ�������Լ�״̬  
                if(!TurnTailFlag) 
                {
                    gpio_init(D2,GPO,0);
                    TurnTailFlag=1;
                 }
                  TurnTail();			//�����һ��������ͷ
		break;
	case StateTwo:
	//�ȴ������ź�
                if ((g_StateSlave > CarGo || g_SlaveOutFlag)&&!TurnTailGoFlag)		//���ڳ��ѹ���·
                {
			//GOGOGO!!!

			TurnTailGoFlag=1;
			g_handle_open=1;
                }
		TurnTail();
                break;
	case WaitingStop:		//�ȴ�ʶ��ͣ����
                TurnTailFlag=0;
		if (Img_StopLineFlag && !g_SlaveOutFlag)		//ʶ��ͣ���� �жϴӳ�״̬
		{
			if (g_StateSlave == StateStop) //�ӳ��ѵ� ������һ�ξ���ͣ��
				g_StateMaster = StateGo;
//			else                            //ƽ�⳵��ͣ��
//			{
//				g_StateMaster = StateStop; //ͣ���ȴ�
//			}
		}
		else if (Img_StopLineFlag && g_SlaveOutFlag)
			g_StateMaster = StateGo;
		break;
	case StateGo:
		acc_speed += curSpeed;
		if (acc_speed > sum_speed * 1.5)
		{
			acc_speed = 0;
			g_StateMaster = CarFinish;
		}
		break;
	case StateStop:
		if (g_StateSlave >= StateGo || g_SlaveOutFlag)		//�ӳ����� ���ߴӳ�����
		{
			acc_speed += curSpeed;
			if (acc_speed > sum_speed * 1.5)
			{
				g_StateMaster = CarFinish;
				acc_speed = 0;
			}
		}
		break;
	case CarFinish://�ĵ�
                {
                    if(count < 5)
                    {
                      angle_init = g_angle_set;
                    }
                    else if(count>= 5 && count < 50)
                    {
                      g_angle_set = GroundAngle;
                      g_mode = 5;
                      
                    }
                    else if(count >= 50)
                    {
                      g_drive_flag = 0;
                      g_angle_set = angle_init;
                      count = 0;
                    }
                    count++;
                 }
		break;
	default:
                
		break;
	}
}

//================================================================//
//  @brief  :		���ֳ��ᳵ����(������·)
//  @param  :		void
//  @return :		void
//  @note   :		void	
//================================================================//
void MeetingTwo2(void)
{
	static int acc_speed = 0;
	static int sum_speed = 0.5 * ONE_METER;
	static float yaw_init = 0;
	static float yaw_init_1 = 0;
	static float yaw_init_2 = 0;
	static int get_flag = 0;
	static int count = 0;
	switch (g_StateMaster)
	{
	case Ready:
		if (g_car_lanuch) g_StateMaster = WaitingBegin;
		break;
	case WaitingBegin:
		if (g_StateSlave > Ready)
		{
			g_StateMaster = CarGo;
			g_drive_flag = 1;
		}
		break;
	case CarGo:
		if (g_GetMeetingFlag) 			 //����ᳵ�� 
		{
			acc_speed += curSpeed;
			if (acc_speed > StartDistance)
			{
				g_StateMaster = StateOne;
				Img_BlockFlag = 0;
				g_handle_open = 0;
				acc_speed = 0;
				//�ı�״̬ ������һ״̬��־
				speed_type = 3; //�ᳵ�ٶ�
				spdExp3 = MEETING_SPEED;
				yaw_init = _ANGLE;
				yaw_init_1 = yaw_init + 90;
				if (yaw_init_1 > 180) yaw_init_1 -= 360;
			}
		}
		else acc_speed = 0;
		break;
	case StateOne:		//�ᳵ�������Լ�״̬  
	if(!TurnNoTailFlag) 
	{
	TurnNoTailFlag=1;
	}
	TurnNoTail();			//�����һ��������ͷ
		break;
	case StateTwo:
	//�ȴ������ź�
	if (g_StateSlave > CarGo || g_SlaveOutFlag)		//���ڳ��ѹ���·
        {
			//GOGOGO!!!
			if(!TurnNoTailGoFlag)
			{
			TurnNoTailGoFlag=1;
                          
			}
        }
			TurnNoTail();
	break;
	case WaitingStop:		//�ȴ�ʶ��ͣ����
          gpio_init(A7, GPO, 0);
          TurnNoTailFlag=0;
		if (Img_StopLineFlag && !g_SlaveOutFlag)		//ʶ��ͣ���� �жϴӳ�״̬
		{
			if (g_StateSlave == StateStop) //�ӳ��ѵ� ������һ�ξ���ͣ��
				g_StateMaster = StateGo;
			else
			{
				g_StateMaster = StateStop; //ͣ���ȴ�
				speed_type = 0;
			}
		}
		else if (Img_StopLineFlag && g_SlaveOutFlag)
			g_StateMaster = StateGo;
		break;
	case StateGo:
		acc_speed += curSpeed;
		if (acc_speed > sum_speed * 1.5)
		{
			acc_speed = 0;
			g_StateMaster = CarFinish;
		}
		break;
	case StateStop:
		if (g_StateSlave >= StateGo || g_SlaveOutFlag)		//�ӳ����� ���ߴӳ�����
		{
			speed_type = 1;
			acc_speed += curSpeed;
			if (acc_speed > sum_speed * 1.5)
			{
				g_StateMaster = CarFinish;
				acc_speed = 0;
			}
		}
		break;
	case CarFinish:
		speed_type = 0;
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		���ֳ��ᳵ����
//  @param  :		void
//  @return :		void
//  @note   :		�����ж�	
//================================================================//
//void MeetingFour(void)
//{
//	static int acc_speed = 0;
//	static int sum_speed = 0.5 * ONE_METER;
//	static float yaw_init = 0;
//	static float yaw_init_1 = 0;
//	static float yaw_init_2 = 0;
//	static int get_flag = 0;
//	static int count = 0;
//	switch (g_StateMaster)
//	{
//	case Ready:
//		if (g_car_lanuch) g_StateMaster = WaitingBegin;
//		break;
//	case WaitingBegin:
//		if (g_StateSlave > Ready)
//		{
//			g_StateMaster = CarGo;
//			g_drive_flag = 1;
//		}
//		break;
//	case CarGo:				//�ȴ�����ᳵ��
//		if (g_GetMeetingFlag) 			 //����ᳵ�� 
//		{
//			acc_speed += curSpeed;
//			if (acc_speed > StartDistance)
//			{
//				Img_BlockFlag = 0;
//				g_handle_open = 0;
//				acc_speed = 0;
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = StateOne;
//				speed_type = 2; //�ᳵ�ٶ�
//				spdExp2 = spdBas2;
//				g_drive_flag = 0;									//debug
//				yaw_init = _ANGLE;
//				if (2 == g_MeetingDir)		//��ת
//				{
//					yaw_init_1 = yaw_init + 90;
//					if (yaw_init_1 > 180) yaw_init_1 -= 360;
//				}
//				else
//				{
//					yaw_init_1 = yaw_init - 90;
//					if (yaw_init_1 < -180) yaw_init_1 += 360;
//				}
//			}
//		}
//		else acc_speed = 0;
//		break;
//	case StateOne:		//�ᳵ�������Լ�״̬
//		if (2 == g_MeetingDir)
//		{
//			ftm_pwm_duty(ftm3, ftm_ch5, InitSteer - 120);
//			if (_ANGLE > 0 && yaw_init_1 >= 0)
//			{
//				if (_ANGLE > yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//�ı�״̬ ������һ״̬��־
//					g_StateMaster = StateTwo;
//					spdExp2 = 0;
//					acc_speed += curSpeed;
//				}
//			}
//			else if (_ANGLE <= 0 && yaw_init_1 < 0)
//			{
//				if (_ANGLE > yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//�ı�״̬ ������һ״̬��־
//					g_StateMaster = StateTwo;
//					spdExp2 = 0;
//					acc_speed += curSpeed;
//				}
//			}
//		}
//		else
//		{
//			ftm_pwm_duty(ftm3, ftm_ch5, InitSteer + 120);
//			if (_ANGLE >= 0 && yaw_init_1 > 0)
//			{
//				if (_ANGLE < yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//�ı�״̬ ������һ״̬��־
//					g_StateMaster = StateTwo;
//					spdExp2 = 0;
//					acc_speed += curSpeed;
//				}
//			}
//			else if (_ANGLE < 0 && yaw_init_1 <= 0)
//			{
//				if (_ANGLE < yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//�ı�״̬ ������һ״̬��־
//					g_StateMaster = StateTwo;
//					spdExp2 = 0;
//					acc_speed += curSpeed;
//				}
//			}
//		}
//		break;
//	case StateTwo:
//		//�ȴ������ź�
//		if (g_StateSlave > CarGo || g_SlaveOutFlag)		//�ӳ��ѹ���·
//		{
//			//�ı�״̬ ������һ״̬��־
//			g_StateMaster = StateThree;
//			speed_type = 2;
//			spdExp2 = 0 - spdBas2;
//			yaw_init = _ANGLE;
//			if (2 == g_MeetingDir)
//			{
//				yaw_init_1 = yaw_init - 90;
//				if (yaw_init_1 < -180)yaw_init_1 += 360;
//			}
//			else
//			{
//				yaw_init_1 = yaw_init + 90;
//				if (yaw_init_1 > 180)yaw_init_1 -= 360;
//			}
//		}
//		break;
//	case StateThree:
//		//����
//		acc_speed += curSpeed;
//		if (acc_speed > 0)
//			ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//		else if (2 == g_MeetingDir)
//			ftm_pwm_duty(ftm3, ftm_ch5, InitSteer - 120);
//		else ftm_pwm_duty(ftm3, ftm_ch5, InitSteer + 120);
//		if (2 == g_MeetingDir)
//		{
//			if (_ANGLE >= 0 && yaw_init_1 > 0)
//			{
//				if (_ANGLE < yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//�ı�״̬ ������һ״̬��־
//					g_StateMaster = StateFour;
//					spdExp2 = spdBas2;
//				}
//			}
//			else if (_ANGLE < 0 && yaw_init_1 <= 0)
//			{
//				if (_ANGLE < yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//�ı�״̬ ������һ״̬��־
//					g_StateMaster = StateFour;
//					spdExp2 = spdBas2;
//				}
//			}
//		}
//		else
//		{
//			if (_ANGLE > 0 && yaw_init_1 >= 0)
//			{
//				if (_ANGLE > yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//�ı�״̬ ������һ״̬��־
//					g_StateMaster = StateFour;
//					spdExp2 = spdBas2;
//				}
//			}
//			else if (_ANGLE <= 0 && yaw_init_1 < 0)
//			{
//				if (_ANGLE > yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//�ı�״̬ ������һ״̬��־
//					g_StateMaster = StateFour;
//					spdExp2 = spdBas2;
//				}
//			}
//		}
//		break;
//	case StateFour:
//		//�ı�״̬ ������һ״̬��־
//		g_StateMaster = StateFive;
//		Img_BrokenFlag = 2;
//		g_handle_open = 1;
//
//		//������
//		g_camera_ctrl_flag = 0;
//		g_ind_ctrl_flag = 1;
//		GetSteerOut();//���������
//		SteerControl();//�������
//		break;
//	case StateFive:
//		//������
//		g_camera_ctrl_flag = 0;
//		g_ind_ctrl_flag = 1;
//		GetSteerOut();//���������
//		SteerControl();//�������
//		if (!Img_BrokenFlag)			//����· ������һ״̬
//		{
//			g_GetMeetingFlag = 0;
//			acc_speed = 0;
//			g_StateMaster = WaitingStop;
//			speed_type = 1;
//		}
//		break;
//	case WaitingStop:		//�ȴ�ʶ��ͣ����
//		if (Img_StopLineFlag && !g_SlaveOutFlag)		//ʶ��ͣ���� �жϴӳ�״̬
//		{
//			if (g_StateSlave == StateStop) //�ӳ��ѵ� ������һ�ξ���ͣ��
//				g_StateMaster = StateGo;
//			else
//			{
//				g_StateMaster = StateStop; //ͣ���ȴ�
//				speed_type = 0;
//			}
//		}
//		else if (Img_StopLineFlag && g_SlaveOutFlag)
//			g_StateMaster = StateGo;
//		break;
//	case StateGo:
//		acc_speed += curSpeed;
//		if (acc_speed > sum_speed * 1.5)
//		{
//			acc_speed = 0;
//			g_StateMaster = CarFinish;
//		}
//		break;
//	case StateStop:
//		if (g_StateSlave >= StateGo || g_SlaveOutFlag)		//�ӳ����� ���ߴӳ�����
//		{
//			speed_type = 1;
//			acc_speed += curSpeed;
//			if (acc_speed > sum_speed * 1.5)
//			{
//				g_StateMaster = CarFinish;
//				acc_speed = 0;
//			}
//		}
//		break;
//	case CarFinish:
//		speed_type = 0;
//		break;
//	default:
//		break;
//	}
//}


#undef _ANGLE 

