#ifndef MEETING_H
#define MEETING_H
#define MEETING_MODE 2
#define MEETING_SPEED 10
#define _ANGLE imu_data.yaw
#define WaitingMeet 1

void ChangeStartMeetingFlag(void);
void MeetingCarControl(void);
void ChangeEndMeetingFlag(void);
void MeetingCtrlFun(void);
void MeetingCtrlFun_1(void);
void MeetingFour(void);
void MeetingTwo(void);

#endif //!MEETING_H