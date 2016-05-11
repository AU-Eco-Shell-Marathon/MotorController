#include "Speed_O_meter.h"

// The following constant is the distance between 
// the spokes of the wheel (measured in mm)
#define DISTANCE 1000

float speed = 0;

// A 32-bit timer measures the amount of ticks since the
// last rising-edge from the sensor (1 tick = 1 ms)


void Speedo_Init()
{
    Timer_Speedometer_Start();
    Speedometer_isr_Enable();
}

float Speedo_GetSpeed()
{
    return speed;   
}

CY_ISR(RPM_isr)
{
    uint16 time;
    
    time = Timer_Speedometer_ReadCapture();
    speed = DISTANCE/time;
    
    Timer_Speedometer_WriteCounter(0);
    Timer_Speedometer_ClearFIFO();
}