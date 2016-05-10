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
#include "Pedal.h"

uint16 set_ = 0;

void Pedal_init()
{
    //return &set_;
}

uint16 Pedal_getValue()
{
    return set_;
}

void Pedal_update()
{
    set_ = (uint16)275u * (uint16)(Status_Reg_1_Read()&0b1);
}

/* [] END OF FILE */
