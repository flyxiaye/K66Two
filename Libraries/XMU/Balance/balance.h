#ifndef  _BALANCE_H_
#define  _BALANCE_H_ 

extern float Limit(float Out,float limit);
extern void OutControl();
extern float DynamicPID(PID pid, ERROR error, float now, float expect);

extern float DynamicPID(PID pid, ERROR error, float now, float expect);

#endif