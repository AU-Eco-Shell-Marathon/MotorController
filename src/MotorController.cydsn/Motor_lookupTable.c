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

uint16 LUT_speed(const uint16 wanted_speed,const uint16 speed)
{
    uint16 point = (((speed)>>LUT_SHIFT) >= (LUT_SIZE-1) ? LUT_SIZE-1 : (speed)>>LUT_SHIFT);
    if(wanted_speed > speed)
    {
        return (uint16)RAMP_LUT[point];
    }
    else
    {
        return (uint16)CONST_LUT[point];
    }
}

/* [] END OF FILE */
