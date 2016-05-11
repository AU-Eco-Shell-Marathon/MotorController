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

#include "Horn.h"

void hornFunc()
{
    if(Status_Reg_HORN_Read())
    {
        PWM_horn_WriteCompare(255);
    }
    else
        PWM_horn_WriteCompare(0);
}


/* [] END OF FILE */
