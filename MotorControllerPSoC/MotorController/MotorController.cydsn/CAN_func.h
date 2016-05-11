
#ifndef CAN_FUNC_H
#define CAN_FUNC_H
    
#include <project.h>
enum CAN_flag {CAN_STATUS, BATTERY_V, BATTERY_A, DOD, T_BATTERY, IR};
extern void CAN_func(uint8 mailbox,enum CAN_flag);

#endif
