#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

void JudgeRamp(void);
void roadblock1();
void roadblock2();
void roadblock3(void);
int JudgeMaxAngle(float real_angle, float obj_angle);
float AngleError(float real_angle, float obj_angle);
void inf();
extern float k1;
extern float k2;
extern float k3;
extern float k4;//调整系数
extern float g_inf;
extern int inf_RB_flag;
extern int s1;
extern int st;
extern int sum;
extern int stop_inf;

#endif