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
#include "ControllerClass.h"
#include "Logger.h"


int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    Logger_Init();
    Logger_Write("Ready");
    
    init();            /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        run();         /* Place your application code here. */
    }
}

/* [] END OF FILE */
