/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>
#include <math.h>

// The following constant is the distance between 
// the spokes of the wheel (measured in mm)
#define DISTANCE 1000

// A 16-bit measures the amount of ticks since the
// last rising-edge from the sensor (1 tick = 1 ms)

int main()
{
	/* Start the components */
    Timer_Speedometer_Start();
    
    /* The time-variable */
    uint16 time;
    float speed;
    
    while(1)
    {
        time = Timer_Speedometer_ReadCapture();
        speed = DISTANCE/time;
    }
}

/* [] END OF FILE */

