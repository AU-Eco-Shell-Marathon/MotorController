#ifndef CAN_H
#define CAN_H

#include <project.h>

struct BMSData
{
    uint8 state;
    uint16 RunTime;
    uint16 battery_V;
    uint8 battery_cell_no_min;
    uint8 battery_cell_V_min;
    uint8 battery_cell_no_max;
    uint8 battery_cell_V_max;
    uint16 battery_A;
    uint8 SOC;
    uint16 DOD;
    uint16 capacity;
    uint8 Tbatt;
    uint8 IR_no_min;
    uint8 IR_min;
    uint8 IR_no_max;
    uint8 IR_max;
    uint8 NewData;
};

struct BMSData * CAN_init();

#endif

