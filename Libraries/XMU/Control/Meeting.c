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
				//				g_handle_open = 0;
				acc_speed = 0;
				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 1;
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
		if (!TurnTailFlag)
		{
			gpio_init(D2, GPO, 0);
			TurnTailFlag = 1;
		}
		TurnTail();			//�����һ��������ͷ
		break;
	case StateTwo:
		//�ȴ������ź�
		if ((g_StateSlave > CarGo || g_SlaveOutFlag) && !TurnTailGoFlag)		//���ڳ��ѹ���·
		{
			//GOGOGO!!!

			TurnTailGoFlag = 1;
			g_handle_open = 1;
		}
		TurnTail();

		break;
	case WaitingStop:		//�ȴ�ʶ��ͣ����
		TurnTailFlag = 0;
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
				g_GetMeetingState = 1;
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
		if (!TurnNoTailFlag)
		{
			TurnNoTailFlag = 1;
		}
		TurnNoTail();			//�����һ��������ͷ
		break;
	case StateTwo:
		//�ȴ������ź�
		if (g_StateSlave > CarGo || g_SlaveOutFlag)		//���ڳ��ѹ���·
		{
			//GOGOGO!!!
			if (!TurnNoTailGoFlag)
			{
				TurnNoTailGoFlag = 1;

			}
		}
		TurnNoTail();
		break;
	case WaitingStop:		//�ȴ�ʶ��ͣ����
		gpio_init(A7, GPO, 0);
		TurnNoTailFlag = 0;
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

//enum MeetingFlag {
//	WaitingStop = 15,
//	IsStopLine,
//	StateGo,
//	StateStop,
//	CarFinish
//};
//int StartDistance = 500; //ʶ���·���Ӿ����
//int StartSum = 0;               //���������
//
////================================================================//
////  @brief  :		���ֳ��ᳵ����
////  @param  :		void
////  @return :		void
////  @note   :		void	
////================================================================//
//void MeetingFour(void)
//{
//	static int acc_speed = 0;
//	static int sum_speed = 2000;
//	static float yaw_init = 0;
//	static float yaw_init_1 = 0;
//	static float yaw_init_2 = 0;
//	static int get_flag = 0;
//	static int count = 0;
//#if 1 == MEETING_MODE	//��ͷ
//	switch (g_StartMaster)
//	{
//	case 0:
//		if (Img_BrokenFlag == 2 && !Img_RampFlag && !g_GetMeetingMaster)             //�����·�� 
//		{
//			acc_speed += curSpeed;
//			if (acc_speed > StartDistance)
//			{
//				g_GetMeetingMaster = 1;
//				g_handle_open = 0;
//				Img_BrokenFlag = 0;
//				BlockFlag = 0;
//				acc_speed = 0;
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 1;
//				speed_type = 3; //�ᳵ�ٶ�
//				spdExp3 = MEETING_SPEED;
//				yaw_init = _ANGLE;
//				yaw_init_1 = yaw_init + 90;
//				if (yaw_init_1 > 180) yaw_init_1 -= 360;
//			}
//		}
//		else acc_speed = 0;
//		break;
//	case 1:
//		ftm_pwm_duty(ftm3, ftm_ch5, InitSteer - 120);
//		if (_ANGLE > 0 && yaw_init_1 >= 0)
//		{
//			if (_ANGLE > yaw_init_1)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 3;
//				spdExp3 = 0;
//				acc_speed += curSpeed;
//			}
//		}
//		else if (_ANGLE <= 0 && yaw_init_1 < 0)
//		{
//			if (_ANGLE > yaw_init_1)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 3;
//				spdExp3 = 0;
//				acc_speed += curSpeed;
//			}
//		}
//		break;
//	case 3:	//��ʱһ��ʱ�� �ȴ�����ͣ��
//		count++;
//		if (count > 100)
//		{
//			count = 0;
//			//�ı�״̬ ������һ״̬��־
//			g_StateMaster = 4;
//			spdExp = -MEETING_SPEED;
//			acc_speed += curSpeed;
//		}
//		break;
//	case 4:
//		acc_speed += curSpeed;
//		if (acc_speed > 0)
//			ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//		else
//			ftm_pwm_duty(ftm3, ftm_ch5, InitSteer + 120);
//
//		if (_ANGLE > 0 && yaw_init_2 >= 0 && _ANGLE > yaw_init_2
//			|| _ANGLE <= 0 && yaw_init_2 < 0 && _ANGLE > yaw_init_2)
//		{
//			ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//			//�ı�״̬ ������һ״̬��־
//			g_StateMaster = 5;
//			spdExp3 = 0;
//		}
//		break;
//	case 5:		//�ȴ���һ����
//		if (g_StateMaster > 0)	//�ӳ�����
//		{
//			//�ı�״̬ ������һ״̬��־
//			g_StateMaster = 6;
//			spdExp3 = MEETING_SPEED;
//			g_handle_open = 1;
//			Img_BrokenFlag = 2;
//			BlockFlag = 1;
//		}
//		break;
//	case 6:
//		if (!Img_BrokenFlag)			//����· ������һ״̬
//		{
//			g_StateMaster = WaitingStop;
//			speed_type = 1;
//		}
//		break;
//	case WaitingStop:		//�ȴ�ʶ��ͣ����
//		if (Img_StopLineFlag)		//ʶ��ͣ���� �жϴӳ�״̬
//		{
//			if (g_StateSlave == StateStop) //�ӳ��ѵ� ������һ�ξ���ͣ��
//				g_StateMaster = StateGo;
//			else
//			{
//				g_StateMaster = StateStop; //ͣ���ȴ�
//				speed_type = 1;
//				spdExp1 = 0;
//			}
//		}
//		break;
//	case StateGo:
//		acc_speed += curSpeed;
//		if (acc_speed > sum_speed)
//		{
//			acc_speed = 0;
//			g_StateMaster = CarFinish;
//		}
//		break;
//	case StateStop:
//		if (g_StateSlave == StateGo || g_StateSlave == CarFinish)		//�ӳ�����
//		{
//			spdExp1 = 15;
//			acc_speed += curSpeed;
//			if (acc_speed > sum_speed)
//			{
//				g_StateMaster = CarFinish;
//				acc_speed = 0;
//			}
//		}
//		break;
//	case CarFinish:
//		speed_type = 1;
//		spdExp1 = 0;
//		break;
//	default:
//		break;
//	}
//
//#elif 2 == MEETING_MODE		//����ͷ
//	switch (g_StateMaster)
//	{
//	case 0:
//		if (Img_BrokenFlag == 2 && !Img_RampFlag && !g_GetMeetingMaster)             //�����·�� 
//		{
//			acc_speed += curSpeed;
//			if (acc_speed > StartDistance)
//			{
//				g_GetMeetingMaster = 1;
//				BlockFlag = 0;
//				acc_speed = 0;
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 1;
//				speed_type = 3; //�ᳵ�ٶ�
//				spdExp3 = MEETING_SPEED;
//				yaw_init = _ANGLE;
//				yaw_init_1 = yaw_init + 90;
//				if (yaw_init_1 > 180) yaw_init_1 -= 360;
//			}
//		}
//		else acc_speed = 0;
//		break;
//	case 1:
//		ftm_pwm_duty(ftm3, ftm_ch5, InitSteer - 120);
//		if (_ANGLE > 0 && yaw_init_1 >= 0)
//		{
//			if (_ANGLE > yaw_init_1)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 3;
//				spdExp3 = 0;
//				acc_speed += curSpeed;
//			}
//		}
//		else if (_ANGLE <= 0 && yaw_init_1 < 0)
//		{
//			if (_ANGLE > yaw_init_1)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 3;
//				spdExp3 = 0;
//				acc_speed += curSpeed;
//			}
//		}
//		break;
//	case 3:
//		//�ȴ������ź�
//		if (g_StateSlave > 1)		//�ӳ��ѹ���·
//		{
//			//�ı�״̬ ������һ״̬��־
//			g_StateMaster = 4;
//			spdExp3 = 0 - MEETING_SPEED;
//			yaw_init = _ANGLE;
//			yaw_init_1 = yaw_init - 90;
//			if (yaw_init_1 < -180)yaw_init_1 += 360;
//		}
//		break;
//	case 4:
//		//����
//		ftm_pwm_duty(ftm3, ftm_ch5, InitSteer - 120);
//		if (_ANGLE >= 0 && yaw_init_1 > 0)
//		{
//			if (_ANGLE < yaw_init_1)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 5;
//				spdExp3 = MEETING_SPEED;
//			}
//		}
//		else if (_ANGLE < 0 && yaw_init_1 <= 0)
//		{
//			if (_ANGLE < yaw_init_1)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 5;
//				spdExp3 = MEETING_SPEED;
//			}
//		}
//		break;
//	case 5:
//		//�ı�״̬ ������һ״̬��־
//		g_StateMaster = 6;
//		Img_BrokenFlag = 2;
//		g_handle_open = 1;
//		g_GetMeetingMaster = 0;
//		break;
//	case 6:
//		if (!Img_BrokenFlag)			//����· ������һ״̬
//		{
//			g_StateMaster = WaitingStop;
//			speed_type = 1;
//		}
//		break;
//	case WaitingStop://15		//�ȴ�ʶ��ͣ����
//		if (Img_StopLineFlag)		//ʶ��ͣ���� �жϴӳ�״̬
//		{
//			if (g_StateSlave == StateStop) //�ӳ��ѵ� ������һ�ξ���ͣ��
//				g_StateMaster = StateGo;
//			else
//			{
//				g_StateMaster = StateStop; //ͣ���ȴ�
//				speed_type = 1;
//				spdExp1 = 0;
//			}
//		}
//		break;
//		//case IsStopLine:
//		//	if (g_StateSlave == StateStop) //�ӳ��ѵ� ������һ�ξ���ͣ��
//		//		g_StateMaster = StateGo;
//		//	else
//		//	{
//		//		g_StateMaster = StateStop; //ͣ���ȴ�
//		//		speed_type = 1;
//		//		spdExp1 = 0;R44RRRRRRRR5RRRR4R
//		//	}
//		//	break;
//	case StateGo://16
//		acc_speed += curSpeed;
//		if (acc_speed > sum_speed)
//		{
//			acc_speed = 0;
//			g_StateMaster = CarFinish;
//		}
//		break;
//	case StateStop://18
//		if (g_StateSlave >= StateGo || g_StateSlave == CarFinish)		//�ӳ�����
//		{
//			spdExp1 = 15;
//			acc_speed += curSpeed;
//			if (acc_speed > sum_speed)
//			{
//				g_StateMaster = CarFinish;
//				acc_speed = 0;
//			}
//		}
//		break;
//	case CarFinish:
//		speed_type = 1;
//		spdExp1 = 0;
//		break;
//	default:
//		break;
//	}
//#endif
//}
//
////================================================================//
////  @brief  :		���ֳ��ᳵ����
////  @param  :		void
////  @return :		void
////  @note   :		void	
////================================================================//
//void MeetingTwo(void)
//{
//	static int acc_speed = 0;
//	static int sum_speed = 2500;
//	static float yaw_init = 0;
//	static float yaw_init_1 = 0;
//	static float yaw_init_2 = 0;
//	static int get_flag = 0;
//	static int count = 0;
//#if 1 == MEETING_MODE//��ͷ
//	switch (g_StateMaster)
//	{
//	case 0:
//		if (Img_BrokenFlag == 2 && !Img_RampFlag && !g_GetMeetingMaster)             //�����·�� 
//		{
//			BlockFlag = 0;//clear block 
//                        acc_speed+=curSpeed;
//			if (acc_speed > StartDistance)
//			{
//				g_GetMeetingMaster = 1;
//                                acc_speed = 0;
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 1;
//				g_drive_flag = 0;//ͣ��
//				Img_BrokenFlag = 0;
//
//			}
//		}
//		break;
//	case 1://�ȴ��ӳ������־
//		if (g_StateSlave > 0)
//		{
//			g_StartMaster = 2;
//		}
//		break;
//	case 2:
//		//ת��
//		//�˴�����ת�亯�� ��ֱ������ Ȼ������ת��
//                MeetingCtrlFun_1();
//		//������־λ�仯
////		g_StateMaster = 3;
////		Img_BrokenFlag = 2;
//
//		break;
//	case 3:
//		//����
//		if (0 == Img_BrokenFlag)
//		{
//			//����·�����ᳵ״̬���ͱ�־λ
//			g_StateMaster = WaitingStop;		//�����ᳵ
//			g_GetMeetingMaster = 0;
//		}
//		break;
//	case WaitingStop:		//����· �ȴ�ʶ��ͣ����
//		if (Img_StopLineFlag)	//ʶ��ͣ����
//		{
//			if (g_StateSlave == IsStopLine) //�ӳ��ѵ� ������һ�ξ���ͣ��
//				g_StateMaster = StateGo;
//			else
//			{
//				g_StateMaster = StateStop; //ͣ���ȴ�
//				g_drive_flag = 0;
//			}
//		}
//		break;
//	case StateGo:
//		acc_speed += curSpeed;
//		if (acc_speed > sum_speed)
//		{
//			acc_speed = 0;
//			g_StateMaster = CarFinish;
//		}
//		break;
//	case StateStop:
//		if (g_StateSlave == StateGo || g_StateSlave == CarFinish)		//�ӳ�����
//		{
//			g_drive_flag = 1;
//			acc_speed += curSpeed;
//			if (acc_speed > sum_speed)
//			{
//				g_StateMaster = CarFinish;
//				acc_speed = 0;
//			}
//		}
//		break;
//	case CarFinish:
//		g_drive_flag = 0;
//                break;
//	default:
//		break;
//	}
//#elif 2 == MEETING_MODE		//����ͷ
//	switch (g_StateMaster)
//	{
//	case 0:
//		if (Img_BrokenFlag == 2 && !Img_RampFlag && !g_GetMeetingMaster)             //�����·�� 
//		{
//			BlockFlag = 0;//clear block 
//                        acc_speed+=curSpeed;
//			if (acc_speed > StartDistance)
//			{
//                          acc_speed = 0;
//				g_GetMeetingMaster = 1;
//				//�ı�״̬ ������һ״̬��־
//				g_StateMaster = 1;
//			}
//		}
//		break;
//	case 1:		//����ܶ�·
////                if(Img_BrokenFlag == 2)
////                {
//////                  MeetingCtrlFun_2();
////                } 
//                if(g_StateSlave < 3 && g_SlaveOutFlag != 1)//�ӳ�δ���ﲢ��δ���磬�����ȴ�
//                {
////                  g_drive_flag = 0;
//                  Ground();
//                }
//                else if(g_StateSlave >= 3 || g_SlaveOutFlag == 1)
//                {
////                  g_StateMaster = WaitingStop;
//                  g_drive_flag = 1;
//                  CircleFlag = 0;
//                  CircleState = 0;
//                  BlockFlag = 0;
//                  Img_RampFlag = 0;
//                  g_handle_open = 1;
//                  CircleIsland_into_flag = 0;
//                  g_MasterOutFlag = 0;
//                  Img_BlockFlag = 1;
//                }
//		if (0 == Img_BrokenFlag)
//		{
//			//����·�����ᳵ״̬���ͱ�־λ
//			g_StateMaster = WaitingStop;		//�����ᳵ
//			g_GetMeetingMaster = 0;
//                        CircleFlag = 0;
//                        CircleState = 0;
//                        BlockFlag = 0;
//                        Img_RampFlag = 0;
//                        acc_speed = 0;
//		}
//		break;
//	case WaitingStop:		//����· �ȴ�ʶ��ͣ����
//		if (Img_StopLineFlag)	//ʶ��ͣ����
//		{
//			if (g_StateSlave == IsStopLine || g_SlaveOutFlag) //�ӳ��ѵ� ������һ�ξ���ͣ��
//				g_StateMaster = StateGo;
//			else
//			{
//				g_StateMaster = StateStop; //ͣ���ȴ�
//				
//			}
//		}
////                else if ((Img_BrokenFlag== 3 || Img_BrokenFlag == 1)&&4 ==  dialSwitchFlg4)
////                  Img_BrokenFlag = 0;
//		break;
//		//case IsStopLine:
//		//	if (g_StateSlave == IsStopLine) //�ӳ��ѵ� ������һ�ξ���ͣ��
//		//		g_StateMaster = StateGo;
//		//	else
//		//		g_StateMaster = StateStop; //ͣ���ȴ�
//		//	break;
//	case StateGo:
//		acc_speed += curSpeed;
//		if (acc_speed > sum_speed)
//		{
//			acc_speed = 0;
//			g_StateMaster = CarFinish;
//		}
//		break;
//                
//	case StateStop:
//                Ground();
//		if (g_StateSlave >= StateGo || g_StateSlave == CarFinish)		//�ӳ�����
//		{
//			g_drive_flag = 1;
//			acc_speed += curSpeed;
//			if (acc_speed > sum_speed + 200)
//			{
//				g_StateMaster = CarFinish;
//				acc_speed = 0;
//			}
//		}
//                if(g_SlaveOutFlag == 1)//�ӳ����磬����������
//                {
//                  g_drive_flag = 1;
//			acc_speed += curSpeed;
//			if (acc_speed > sum_speed + 200)
//			{ 
//				g_StateMaster = CarFinish;
//				acc_speed = 0; 
//			}
//                }
//		break;
//	case CarFinish:
//          
//		g_drive_flag = 0;
//                break;
//	default:
//		break;
//	}
//#endif
//}
//
////================================================================//
////  @brief  :		״̬ת��ƽ�����
////  @param  :		dst Ŀ����� srcԴ��� dutyĿ�����ռ�ձ�
////  @return :		�����
////  @note   :		Դ�����Ŀ���������
////================================================================//
//float LinearOut(float dst, float src, float duty)
//{
//	return (dst * duty + src * (1 - duty));
//}
//================================================================//
//  @brief  :		�ᳵ�����Ƴ������������֣�
//  @param  :		dst Ŀ����� srcԴ��� dutyĿ�����ռ�ձ�
//  @return :		�����
//  @note   :		Դ�����Ŀ���������
//================================================================//
//void MeetingCtrlFun_1(void) 
//{
//  gpio_init(D0,GPO,0);
//  static int stage = 0;
//  static float last_yaw = 0;
//  static int get_flag = 0;
//  static int delay = 0;
//  if(!get_flag)
//  {
//    g_mode = 1;
//    stage = 0;
//    get_flag = 1;
//    g_drive_flag = 0;
//    last_yaw = _ANGLE;
//  } 
//  switch(stage)
//  {
//    case 0:
//      if(delay <= 200)
//      {
//        delay++;
//      }
//      if(delay > 200)
//      {
//        g_drive_flag = 1;
//        stage = 1;
//        delay = 0;
//      }
//    case 1:
//      sum_left += lCurSpeed;
//      g_errorD = - meet_st;
//      if(_ANGLE * last_yaw < 0)
//      {
//        error_yaw += abs(abs(_ANGLE - last_yaw) - 360);
//      }
//      else
//      {
//        error_yaw += abs(_ANGLE - last_yaw);
//      }
//      last_yaw = _ANGLE;
//      if(sum_left > sum_meet_left && error_yaw > 45)
//      {
//        stage = 2;
//        last_yaw = _ANGLE;
//      }
//      break;
//    case 2:
//      sum_right += rCurSpeed;
//      g_errorD =  meet_st;
//      
//      if(_ANGLE * last_yaw <= 0)
//      {
//        error_yaw = abs(abs(_ANGLE - last_yaw) - 360);
//      }
//      else
//      {
//        error_yaw = abs(_ANGLE - last_yaw);
//      }
//      last_yaw = _ANGLE;
//      if(sum_right > sum_meet_right && error_yaw > 180)
//      {
//	g_StateMaster = 3;
//        Img_BrokenFlag = 2;
//        g_drive_flag = 0;
//        g_mode = 3;
////        error_yaw = 0;
//      }
//      break;
//    default:
//      break;
//  }
//    
//
//}


//================================================================//
//  @brief  :		��лᳵ�㷨,����ͷ�����֣�
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
//void MeetingCtrlFun_2()//����
//{
//  static int count = 0;
//
//  if(count >= 5 )
//  {
//    count = 0;
//
////�ı䷽�򻷲�ֵ
//    g_errorD = (ind_right / ave_right - ind_left / ave_left) / (ind_right / ave_right + ind_left / ave_left) /0.01;
//    g_error_before = g_errorD;
//
//    g_fDirectionControlOut_before = g_fDirectionControlOut_new;
//    if(0 == g_errorD)
//    {
//      g_fDirectionControlOut_new = 0;
//    }
//    else
//    {
//      g_fDirectionControlOut_new = g_errorD * g_dire_ad_P + ( g_errorD - g_error_before ) * g_dire_ad_D;
//    }
//  }
//  else
//    count++;
//  //�÷��򻷺�����������ƽ�����
//}
////�ᳵ���������ƺ���
////����pit�ж���
//void MeetingCarControl(void)
//{
//	//�ᳵ����״̬��δ����ᳵ���������뺯�����ƺ���
//	if (g_MeetingCtrlEndFlag || !g_GetMeetingMaster) return;
//
//	//�ᳵ���ƺ���
//  //  MeetingCtrlFun();
//	MeetingCtrlFun_1();
//
//	//���ƽ���������������־λ
//	//g_MeetingCtrlEndFlag = 1;
//}
////�ᳵ����ȫ�ֱ�������
////����main��pit�жϺ���
//void ChangeEndMeetingFlag(void)
//{
//#if 1 == MEETING_MODE
//	if (g_MeetingCtrlEndFlag)              //������ɻᳵ��׼���뿪�ᳵ��
//	{
//		if (!g_GetMeetingSlave)         //�ӳ�δ�����·��
//		{
//			speed_type = 4; //ͣ��
//		}
//		else                                //�ӳ������·������������
//		{
//			g_GetMeetingMaster = 0;
//			g_GetMeetingSlave = 0;
//			speed_type = 1;
//			Img_BrokenFlag = 2;
//			g_handle_open = 1;
//			g_MeetingCtrlEndFlag = 0;
//		}
//	}
//#elif 2 == MEETING_MODE
//	if (g_MeetingCtrlEndFlag);
//#endif
//
//}




//void MeetingCtrlFun(void) // �ᳵ�����Ƴ�������1
//{
//	static int state = 0;
//	static int acc_speed = 0;
//	speed_type = 3;
//	switch (state)
//	{
//	case 0:
//		ftm_pwm_duty(ftm3, ftm_ch5, InitSteer - 120);
//		spdExp3 = 15;
//		acc_speed += curSpeed;
//		if (acc_speed > sum_distance_1)
//			state = 1;
//		break;
//	case 1:
//		ftm_pwm_duty(ftm3, ftm_ch5, InitSteer + 120);
//		spdExp3 = -15;
//		acc_speed += curSpeed;
//		if (acc_speed < (sum_distance_1 - sum_distance_2))
//			state = 2;
//		break;
//	case 2:
//		spdExp3 = 15;
//		acc_speed += curSpeed;
//		SteerDuty = InitSteer + (int)(Steer_P * mul_ad_error * ad_error_1 + Steer_D * mul_ad_error * (ad_error_1 - ad_error_2) + 0.5);
//		ad_error_2 = ad_error_1;
//		if (20 == ind_left && 20 == ind_right)
//			SteerDuty = InitSteer;
//		else if (20 == ind_left && 20 != ind_right)              //�Ҵ���
//			SteerDuty = InitSteer - 120;
//		else if (20 == ind_right && 20 != ind_left)         //�����
//			SteerDuty = InitSteer + 120;
//		else;
//		SteerDuty = MAX(SteerDuty, InitSteer - 120);
//		SteerDuty = MIN(SteerDuty, InitSteer + 120);
//		ftm_pwm_duty(ftm3, ftm_ch5, SteerDuty);
//		if (acc_speed > sum_distance_3)
//		{
//			state = 0;
//			g_MeetingCtrlEndFlag = 1;
//			acc_speed = 0;
//		}
//		break;
//	default:
//		break;
//	}
//}
//
//void MeetingCtrlFun_1(void) // �ᳵ�����Ƴ�������2
//{
//	static int state = 0;
//	static int acc_speed = 0;
//	static float yaw_init = 0;
//	static float yaw_init_1 = 0;
//	static float yaw_init_2 = 0;
//	static int get_flag = 0;
//	speed_type = 3;
//	if (!get_flag)
//	{
//		yaw_init = _ANGLE;
//		yaw_init_1 = yaw_init + 90;
//		if (yaw_init_1 > 180)
//			yaw_init_1 -= 360;
//		yaw_init_2 = yaw_init + 160;
//		if (yaw_init_2 > 180)
//			yaw_init_2 -= 360;
//		get_flag = 1;
//	}
//
//	switch (state)
//	{
//	case 0:
//		ftm_pwm_duty(ftm3, ftm_ch5, InitSteer - 120);
//		if (_ANGLE > 0 && yaw_init_1 >= 0)
//		{
//			if (_ANGLE > yaw_init_1)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				state = 1;
//				spdExp3 = -spdExp3;
//				acc_speed += curSpeed;
//			}
//		}
//		else if (_ANGLE <= 0 && yaw_init_1 < 0)
//		{
//			if (_ANGLE > yaw_init_1)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				state = 1;
//				spdExp3 = -spdExp3;
//				acc_speed += curSpeed;
//			}
//		}
//		break;
//	case 1:
//		acc_speed += curSpeed;
//		if (acc_speed > 0)
//			ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//		else
//			ftm_pwm_duty(ftm3, ftm_ch5, InitSteer + 120);
//
//		if (_ANGLE > 0 && yaw_init_2 >= 0)
//		{
//			if (_ANGLE > yaw_init_2)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				state = 2;
//				spdExp3 = -spdExp3;
//			}
//		}
//		else if (_ANGLE <= 0 && yaw_init_2 < 0)
//		{
//			if (_ANGLE > yaw_init_2)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				state = 2;
//				spdExp3 = -spdExp3;
//			}
//		}
//		break;
//	case 2:
//		acc_speed += curSpeed;
//		ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//		if (acc_speed > -4000)
//		{
//			state = 0;
//			g_MeetingCtrlEndFlag = 1;
//			acc_speed = 0;
//			get_flag = 0;
//		}
//		break;
//	default:
//		break;
//	}
//}


#undef _ANGLE 

