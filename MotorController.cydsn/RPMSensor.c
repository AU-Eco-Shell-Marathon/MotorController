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
#include "RPMSensor.h"

uint16 RPM_temp = 0;
char RPM_reset=0;

CY_ISR_PROTO(RPM);


CY_ISR(RPM)
{

    uint8 STATUS = Timer_RPM_STATUS;
    
    if((STATUS&(1<<1)))//capture
    {
        uint16 temp = Timer_RPM_ReadCapture();
        uint16 temp2 = 0;
        if(Timer_RPM_STATUS&(1<<3))
            temp2 = Timer_RPM_ReadCapture();
            
        
        RPM_reset=0;
        
        if(temp2 != 0)
        {
            RPM_temp = ((uint16)(60000/(temp-temp2)));
        }
        else
        {
            //RPM_temp=0;
        }
        
    }
    else if((STATUS&(1<<0)))//TC
    {
        
        if(RPM_reset)
        {
            RPM_temp=0;
        }
        RPM_reset=1;
        
    }
    Timer_RPM_WriteCounter(6000u);
    Timer_RPM_ClearFIFO();
}


void RPMSensor_init()
{
    RPM_isr_StartEx(RPM);
    Timer_RPM_Start();
    Clock_4_Start();
}

uint16 RPMSensor_getValue()
{
    return RPM_temp;   
}

uint16 RPMSensor_getSpeed()
{
    return RPMSensor_getValue();
}

/* [] END OF FILE */
