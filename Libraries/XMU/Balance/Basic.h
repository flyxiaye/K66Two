#ifndef  _Basic_H_
#define  _Basic_H_ 

#define RIGHT_GO ftm_ch4
#define RIGHT_BACK ftm_ch5
#define LEFT_GO ftm_ch6
#define LEFT_BACK  ftm_ch7

void changemode();
void flag();
void DrivePWM(float g_duty_PWMleft, float g_duty_PWMright);
#endif