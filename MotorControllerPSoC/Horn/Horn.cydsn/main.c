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
#include "Horn.h"

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
  
    PWM_horn_Start();
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        hornFunc();
    }
}

/* [] END OF FILE */
