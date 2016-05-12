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
#include "Motor_LookupTable.h"
#include "LUT_data.h"

uint16 LUT_speed(const uint16 wanted_speed,const uint16 speed,const uint16 power)
{
    volatile uint16 calc_speed = 0;
    volatile uint16 point = (((power/10)>>LUT_SHIFT) >= (LUT_SIZE-1) ? LUT_SIZE-1 : (power/10)>>LUT_SHIFT);
    if(wanted_speed > speed)
    {
        calc_speed = RAMP_LUT[point];
        return calc_speed;
    }
    else
    {
        return ((uint16)CONST_LUT[((power/10)>>LUT_SHIFT >= LUT_SIZE ? LUT_SIZE : (power/10)>>LUT_SHIFT)]);
    }
}

/* [] END OF FILE */
