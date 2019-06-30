#include "headfile.h"
#include "Meeting.h"
#include "Obstacle.h"

#define GroundAngle 9.73
#define MEETING_SPEED 10
#define _ANGLE imu_data.yaw

int StartDistance = 0.5 * ONE_METER;

//================================================================//
//  @brief  :		补图变量与会车变量调整
//  @param  :		void
//  @return :		void
//  @note   :		放在补图后面	
//================================================================//
void MeetingToImage(void)
{
	if (!g_single_open)			//非单车模式
	{
		if (0 == g_GetMeetingState && 2 == Img_BrokenFlag && CarGo == g_StateMaster)		//未开始会车
			g_GetMeetingFlag = 1;
		else if (g_GetMeetingState && Img_BrokenFlag)			//已经完成会车，不再识别断路
			Img_BrokenFlag = 0;

		if (g_GetMeetingState && 1 == Img_StopLineFlag && WaitingStop <= g_StateMaster)		//已经完成会车，开始识别停车线
			;
		else Img_StopLineFlag = 0;
	}
}


//================================================================//
//  @brief  :		二轮车会车函数(掉头)
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
		if (g_GetMeetingFlag) 			 //进入会车区 
		{
			acc_speed += curSpeed;
			if (acc_speed > StartDistance)
			{
				g_StateMaster = StateOne;
				Img_BlockFlag = 0;
//				g_handle_open = 0;
				acc_speed = 0;
				//改变状态 调整下一状态标志
//				g_StateMaster = 1;
			}
		}
		else acc_speed = 0;
		break;
	case StateOne:		//会车区动作以及状态  
                if(!TurnTailFlag) 
                {
                    gpio_init(D2,GPO,0);
                    TurnTailFlag=1;
                 }
                  TurnTail();			//进入后一波操作掉头
		break;
	case StateTwo:
	//等待接收信号
                if ((g_StateSlave > CarGo || g_SlaveOutFlag)&&!TurnTailGoFlag)		//隔壁车已过断路
                {
			//GOGOGO!!!

			TurnTailGoFlag=1;
			g_handle_open=1;
                }
		TurnTail();
                break;
	case WaitingStop:		//等待识别停车线
                TurnTailFlag=0;
		if (Img_StopLineFlag && !g_SlaveOutFlag)		//识别停车线 判断从车状态
		{
			if (g_StateSlave == StateStop) //从车已到 继续跑一段距离停下
				g_StateMaster = StateGo;
//			else                            //平衡车不停留
//			{
//				g_StateMaster = StateStop; //停留等待
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
		if (g_StateSlave >= StateGo || g_SlaveOutFlag)		//从车到达 或者从车出界
		{
			acc_speed += curSpeed;
			if (acc_speed > sum_speed * 1.5)
			{
				g_StateMaster = CarFinish;
				acc_speed = 0;
			}
		}
		break;
	case CarFinish://拍地
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
//  @brief  :		二轮车会车函数(继续跑路)
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
		if (g_GetMeetingFlag) 			 //进入会车区 
		{
			acc_speed += curSpeed;
			if (acc_speed > StartDistance)
			{
				g_StateMaster = StateOne;
				Img_BlockFlag = 0;
				g_handle_open = 0;
				acc_speed = 0;
				//改变状态 调整下一状态标志
				speed_type = 3; //会车速度
				spdExp3 = MEETING_SPEED;
				yaw_init = _ANGLE;
				yaw_init_1 = yaw_init + 90;
				if (yaw_init_1 > 180) yaw_init_1 -= 360;
			}
		}
		else acc_speed = 0;
		break;
	case StateOne:		//会车区动作以及状态  
	if(!TurnNoTailFlag) 
	{
	TurnNoTailFlag=1;
	}
	TurnNoTail();			//进入后一波操作掉头
		break;
	case StateTwo:
	//等待接收信号
	if (g_StateSlave > CarGo || g_SlaveOutFlag)		//隔壁车已过断路
        {
			//GOGOGO!!!
			if(!TurnNoTailGoFlag)
			{
			TurnNoTailGoFlag=1;
                          
			}
        }
			TurnNoTail();
	break;
	case WaitingStop:		//等待识别停车线
          gpio_init(A7, GPO, 0);
          TurnNoTailFlag=0;
		if (Img_StopLineFlag && !g_SlaveOutFlag)		//识别停车线 判断从车状态
		{
			if (g_StateSlave == StateStop) //从车已到 继续跑一段距离停下
				g_StateMaster = StateGo;
			else
			{
				g_StateMaster = StateStop; //停留等待
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
		if (g_StateSlave >= StateGo || g_SlaveOutFlag)		//从车到达 或者从车出界
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
//  @brief  :		四轮车会车函数
//  @param  :		void
//  @return :		void
//  @note   :		放在中断	
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
//	case CarGo:				//等待进入会车区
//		if (g_GetMeetingFlag) 			 //进入会车区 
//		{
//			acc_speed += curSpeed;
//			if (acc_speed > StartDistance)
//			{
//				Img_BlockFlag = 0;
//				g_handle_open = 0;
//				acc_speed = 0;
//				//改变状态 调整下一状态标志
//				g_StateMaster = StateOne;
//				speed_type = 2; //会车速度
//				spdExp2 = spdBas2;
//				g_drive_flag = 0;									//debug
//				yaw_init = _ANGLE;
//				if (2 == g_MeetingDir)		//右转
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
//	case StateOne:		//会车区动作以及状态
//		if (2 == g_MeetingDir)
//		{
//			ftm_pwm_duty(ftm3, ftm_ch5, InitSteer - 120);
//			if (_ANGLE > 0 && yaw_init_1 >= 0)
//			{
//				if (_ANGLE > yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//改变状态 调整下一状态标志
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
//					//改变状态 调整下一状态标志
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
//					//改变状态 调整下一状态标志
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
//					//改变状态 调整下一状态标志
//					g_StateMaster = StateTwo;
//					spdExp2 = 0;
//					acc_speed += curSpeed;
//				}
//			}
//		}
//		break;
//	case StateTwo:
//		//等待接收信号
//		if (g_StateSlave > CarGo || g_SlaveOutFlag)		//从车已过断路
//		{
//			//改变状态 调整下一状态标志
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
//		//倒车
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
//					//改变状态 调整下一状态标志
//					g_StateMaster = StateFour;
//					spdExp2 = spdBas2;
//				}
//			}
//			else if (_ANGLE < 0 && yaw_init_1 <= 0)
//			{
//				if (_ANGLE < yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//改变状态 调整下一状态标志
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
//					//改变状态 调整下一状态标志
//					g_StateMaster = StateFour;
//					spdExp2 = spdBas2;
//				}
//			}
//			else if (_ANGLE <= 0 && yaw_init_1 < 0)
//			{
//				if (_ANGLE > yaw_init_1)
//				{
//					ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//					//改变状态 调整下一状态标志
//					g_StateMaster = StateFour;
//					spdExp2 = spdBas2;
//				}
//			}
//		}
//		break;
//	case StateFour:
//		//改变状态 调整下一状态标志
//		g_StateMaster = StateFive;
//		Img_BrokenFlag = 2;
//		g_handle_open = 1;
//
//		//舵机输出
//		g_camera_ctrl_flag = 0;
//		g_ind_ctrl_flag = 1;
//		GetSteerOut();//计算舵机输出
//		SteerControl();//舵机控制
//		break;
//	case StateFive:
//		//舵机输出
//		g_camera_ctrl_flag = 0;
//		g_ind_ctrl_flag = 1;
//		GetSteerOut();//计算舵机输出
//		SteerControl();//舵机控制
//		if (!Img_BrokenFlag)			//出断路 进入下一状态
//		{
//			g_GetMeetingFlag = 0;
//			acc_speed = 0;
//			g_StateMaster = WaitingStop;
//			speed_type = 1;
//		}
//		break;
//	case WaitingStop:		//等待识别停车线
//		if (Img_StopLineFlag && !g_SlaveOutFlag)		//识别停车线 判断从车状态
//		{
//			if (g_StateSlave == StateStop) //从车已到 继续跑一段距离停下
//				g_StateMaster = StateGo;
//			else
//			{
//				g_StateMaster = StateStop; //停留等待
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
//		if (g_StateSlave >= StateGo || g_SlaveOutFlag)		//从车到达 或者从车出界
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



//enum MeetingFlag {
//	WaitingStop = 15,
//	IsStopLine,
//	StateGo,
//	StateStop,
//	CarFinish
//};
//int StartDistance = 500; //识别断路后延距距离
//int StartSum = 0;               //距离积累量
//
////================================================================//
////  @brief  :		四轮车会车函数
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
//#if 1 == MEETING_MODE	//掉头
//	switch (g_StartMaster)
//	{
//	case 0:
//		if (Img_BrokenFlag == 2 && !Img_RampFlag && !g_GetMeetingMaster)             //进入断路区 
//		{
//			acc_speed += curSpeed;
//			if (acc_speed > StartDistance)
//			{
//				g_GetMeetingMaster = 1;
//				g_handle_open = 0;
//				Img_BrokenFlag = 0;
//				BlockFlag = 0;
//				acc_speed = 0;
//				//改变状态 调整下一状态标志
//				g_StateMaster = 1;
//				speed_type = 3; //会车速度
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
//				//改变状态 调整下一状态标志
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
//				//改变状态 调整下一状态标志
//				g_StateMaster = 3;
//				spdExp3 = 0;
//				acc_speed += curSpeed;
//			}
//		}
//		break;
//	case 3:	//延时一段时间 等待车辆停稳
//		count++;
//		if (count > 100)
//		{
//			count = 0;
//			//改变状态 调整下一状态标志
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
//			//改变状态 调整下一状态标志
//			g_StateMaster = 5;
//			spdExp3 = 0;
//		}
//		break;
//	case 5:		//等待另一辆车
//		if (g_StateMaster > 0)	//从车到达
//		{
//			//改变状态 调整下一状态标志
//			g_StateMaster = 6;
//			spdExp3 = MEETING_SPEED;
//			g_handle_open = 1;
//			Img_BrokenFlag = 2;
//			BlockFlag = 1;
//		}
//		break;
//	case 6:
//		if (!Img_BrokenFlag)			//出断路 进入下一状态
//		{
//			g_StateMaster = WaitingStop;
//			speed_type = 1;
//		}
//		break;
//	case WaitingStop:		//等待识别停车线
//		if (Img_StopLineFlag)		//识别停车线 判断从车状态
//		{
//			if (g_StateSlave == StateStop) //从车已到 继续跑一段距离停下
//				g_StateMaster = StateGo;
//			else
//			{
//				g_StateMaster = StateStop; //停留等待
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
//		if (g_StateSlave == StateGo || g_StateSlave == CarFinish)		//从车到达
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
//#elif 2 == MEETING_MODE		//不掉头
//	switch (g_StateMaster)
//	{
//	case 0:
//		if (Img_BrokenFlag == 2 && !Img_RampFlag && !g_GetMeetingMaster)             //进入断路区 
//		{
//			acc_speed += curSpeed;
//			if (acc_speed > StartDistance)
//			{
//				g_GetMeetingMaster = 1;
//				BlockFlag = 0;
//				acc_speed = 0;
//				//改变状态 调整下一状态标志
//				g_StateMaster = 1;
//				speed_type = 3; //会车速度
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
//				//改变状态 调整下一状态标志
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
//				//改变状态 调整下一状态标志
//				g_StateMaster = 3;
//				spdExp3 = 0;
//				acc_speed += curSpeed;
//			}
//		}
//		break;
//	case 3:
//		//等待接收信号
//		if (g_StateSlave > 1)		//从车已过断路
//		{
//			//改变状态 调整下一状态标志
//			g_StateMaster = 4;
//			spdExp3 = 0 - MEETING_SPEED;
//			yaw_init = _ANGLE;
//			yaw_init_1 = yaw_init - 90;
//			if (yaw_init_1 < -180)yaw_init_1 += 360;
//		}
//		break;
//	case 4:
//		//倒车
//		ftm_pwm_duty(ftm3, ftm_ch5, InitSteer - 120);
//		if (_ANGLE >= 0 && yaw_init_1 > 0)
//		{
//			if (_ANGLE < yaw_init_1)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				//改变状态 调整下一状态标志
//				g_StateMaster = 5;
//				spdExp3 = MEETING_SPEED;
//			}
//		}
//		else if (_ANGLE < 0 && yaw_init_1 <= 0)
//		{
//			if (_ANGLE < yaw_init_1)
//			{
//				ftm_pwm_duty(ftm3, ftm_ch5, InitSteer);
//				//改变状态 调整下一状态标志
//				g_StateMaster = 5;
//				spdExp3 = MEETING_SPEED;
//			}
//		}
//		break;
//	case 5:
//		//改变状态 调整下一状态标志
//		g_StateMaster = 6;
//		Img_BrokenFlag = 2;
//		g_handle_open = 1;
//		g_GetMeetingMaster = 0;
//		break;
//	case 6:
//		if (!Img_BrokenFlag)			//出断路 进入下一状态
//		{
//			g_StateMaster = WaitingStop;
//			speed_type = 1;
//		}
//		break;
//	case WaitingStop://15		//等待识别停车线
//		if (Img_StopLineFlag)		//识别停车线 判断从车状态
//		{
//			if (g_StateSlave == StateStop) //从车已到 继续跑一段距离停下
//				g_StateMaster = StateGo;
//			else
//			{
//				g_StateMaster = StateStop; //停留等待
//				speed_type = 1;
//				spdExp1 = 0;
//			}
//		}
//		break;
//		//case IsStopLine:
//		//	if (g_StateSlave == StateStop) //从车已到 继续跑一段距离停下
//		//		g_StateMaster = StateGo;
//		//	else
//		//	{
//		//		g_StateMaster = StateStop; //停留等待
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
//		if (g_StateSlave >= StateGo || g_StateSlave == CarFinish)		//从车到达
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
////  @brief  :		二轮车会车函数
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
//#if 1 == MEETING_MODE//掉头
//	switch (g_StateMaster)
//	{
//	case 0:
//		if (Img_BrokenFlag == 2 && !Img_RampFlag && !g_GetMeetingMaster)             //进入断路区 
//		{
//			BlockFlag = 0;//clear block 
//                        acc_speed+=curSpeed;
//			if (acc_speed > StartDistance)
//			{
//				g_GetMeetingMaster = 1;
//                                acc_speed = 0;
//				//改变状态 调整下一状态标志
//				g_StateMaster = 1;
//				g_drive_flag = 0;//停车
//				Img_BrokenFlag = 0;
//
//			}
//		}
//		break;
//	case 1://等待从车到达标志
//		if (g_StateSlave > 0)
//		{
//			g_StartMaster = 2;
//		}
//		break;
//	case 2:
//		//转弯
//		//此处添加转弯函数 车直立起来 然后慢慢转弯
//                MeetingCtrlFun_1();
//		//结束标志位变化
////		g_StateMaster = 3;
////		Img_BrokenFlag = 2;
//
//		break;
//	case 3:
//		//启动
//		if (0 == Img_BrokenFlag)
//		{
//			//出断路结束会车状态发送标志位
//			g_StateMaster = WaitingStop;		//结束会车
//			g_GetMeetingMaster = 0;
//		}
//		break;
//	case WaitingStop:		//出断路 等待识别停车线
//		if (Img_StopLineFlag)	//识别到停车线
//		{
//			if (g_StateSlave == IsStopLine) //从车已到 继续跑一段距离停下
//				g_StateMaster = StateGo;
//			else
//			{
//				g_StateMaster = StateStop; //停留等待
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
//		if (g_StateSlave == StateGo || g_StateSlave == CarFinish)		//从车到达
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
//#elif 2 == MEETING_MODE		//不掉头
//	switch (g_StateMaster)
//	{
//	case 0:
//		if (Img_BrokenFlag == 2 && !Img_RampFlag && !g_GetMeetingMaster)             //进入断路区 
//		{
//			BlockFlag = 0;//clear block 
//                        acc_speed+=curSpeed;
//			if (acc_speed > StartDistance)
//			{
//                          acc_speed = 0;
//				g_GetMeetingMaster = 1;
//				//改变状态 调整下一状态标志
//				g_StateMaster = 1;
//			}
//		}
//		break;
//	case 1:		//电磁跑断路
////                if(Img_BrokenFlag == 2)
////                {
//////                  MeetingCtrlFun_2();
////                } 
//                if(g_StateSlave < 3 && g_SlaveOutFlag != 1)//从车未到达并且未出界，主车等待
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
//			//出断路结束会车状态发送标志位
//			g_StateMaster = WaitingStop;		//结束会车
//			g_GetMeetingMaster = 0;
//                        CircleFlag = 0;
//                        CircleState = 0;
//                        BlockFlag = 0;
//                        Img_RampFlag = 0;
//                        acc_speed = 0;
//		}
//		break;
//	case WaitingStop:		//出断路 等待识别停车线
//		if (Img_StopLineFlag)	//识别到停车线
//		{
//			if (g_StateSlave == IsStopLine || g_SlaveOutFlag) //从车已到 继续跑一段距离停下
//				g_StateMaster = StateGo;
//			else
//			{
//				g_StateMaster = StateStop; //停留等待
//				
//			}
//		}
////                else if ((Img_BrokenFlag== 3 || Img_BrokenFlag == 1)&&4 ==  dialSwitchFlg4)
////                  Img_BrokenFlag = 0;
//		break;
//		//case IsStopLine:
//		//	if (g_StateSlave == IsStopLine) //从车已到 继续跑一段距离停下
//		//		g_StateMaster = StateGo;
//		//	else
//		//		g_StateMaster = StateStop; //停留等待
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
//		if (g_StateSlave >= StateGo || g_StateSlave == CarFinish)		//从车到达
//		{
//			g_drive_flag = 1;
//			acc_speed += curSpeed;
//			if (acc_speed > sum_speed + 200)
//			{
//				g_StateMaster = CarFinish;
//				acc_speed = 0;
//			}
//		}
//                if(g_SlaveOutFlag == 1)//从车出界，主车继续动
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
////  @brief  :		状态转化平滑输出
////  @param  :		dst 目标输出 src源输出 duty目标输出占空比
////  @return :		输出量
////  @note   :		源输出向目标输出过渡
////================================================================//
//float LinearOut(float dst, float src, float duty)
//{
//	return (dst * duty + src * (1 - duty));
//}
//================================================================//
//  @brief  :		会车区控制车辆函数（两轮）
//  @param  :		dst 目标输出 src源输出 duty目标输出占空比
//  @return :		输出量
//  @note   :		源输出向目标输出过渡
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
//  @brief  :		电感会车算法,不掉头（两轮）
//  @param  :		
//  @return :		
//  @note   :		void
//================================================================//
//void MeetingCtrlFun_2()//现用
//{
//  static int count = 0;
//
//  if(count >= 5 )
//  {
//    count = 0;
//
////改变方向环差值
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
//  //用方向环函数继续进行平滑输出
//}
////会车区车辆控制函数
////放在pit中断里
//void MeetingCarControl(void)
//{
//	//会车结束状态，未进入会车区，不进入函数控制函数
//	if (g_MeetingCtrlEndFlag || !g_GetMeetingMaster) return;
//
//	//会车控制函数
//  //  MeetingCtrlFun();
//	MeetingCtrlFun_1();
//
//	//控制结束，给出结束标志位
//	//g_MeetingCtrlEndFlag = 1;
//}
////会车结束全局变量控制
////放在main或pit中断函数
//void ChangeEndMeetingFlag(void)
//{
//#if 1 == MEETING_MODE
//	if (g_MeetingCtrlEndFlag)              //主车完成会车，准备离开会车区
//	{
//		if (!g_GetMeetingSlave)         //从车未到达断路区
//		{
//			speed_type = 4; //停车
//		}
//		else                                //从车到达断路区，主车启动
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




//void MeetingCtrlFun(void) // 会车区控制车辆函数1
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
//		else if (20 == ind_left && 20 != ind_right)              //右打死
//			SteerDuty = InitSteer - 120;
//		else if (20 == ind_right && 20 != ind_left)         //左打死
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
//void MeetingCtrlFun_1(void) // 会车区控制车辆函数2
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

