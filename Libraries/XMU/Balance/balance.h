#ifndef  _balance_H_
#define  _balance_H_ 

float PIDControl(PID pid ,ERROR error, float now , float expect );
float Limit(float Out,float limit);
float DynamicPID(PID pid, ERROR error, float now, float expect);

#endif