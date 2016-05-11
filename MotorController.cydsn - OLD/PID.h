#ifndef PID_H
#define PID_H
#include <project.h>

#define Kp_def 1000;
#define Ki_def 10;
#define Kd_def 0;
#define MAX_def 255;
#define MIN_def 0;
#define preShift_def 3;
#define dt_def 10;

struct PIDparameter
{
    int32 Kp;
    int32 Ki;
    int32 Kd;
    int32 KShift;
    int32 MAX;
    int32 MIN;
    int32 preShift;
    char valid;
};

void PID_init();
uint16 PID(const uint16 input, const uint16 plant);
void setPID(const struct PIDparameter * parameter);
#endif
