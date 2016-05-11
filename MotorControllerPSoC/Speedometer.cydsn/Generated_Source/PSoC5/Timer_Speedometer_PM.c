/*******************************************************************************
* File Name: Timer_Speedometer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Timer_Speedometer.h"

static Timer_Speedometer_backupStruct Timer_Speedometer_backup;


/*******************************************************************************
* Function Name: Timer_Speedometer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_Speedometer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_Speedometer_SaveConfig(void) 
{
    #if (!Timer_Speedometer_UsingFixedFunction)
        Timer_Speedometer_backup.TimerUdb = Timer_Speedometer_ReadCounter();
        Timer_Speedometer_backup.InterruptMaskValue = Timer_Speedometer_STATUS_MASK;
        #if (Timer_Speedometer_UsingHWCaptureCounter)
            Timer_Speedometer_backup.TimerCaptureCounter = Timer_Speedometer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_Speedometer_UDB_CONTROL_REG_REMOVED)
            Timer_Speedometer_backup.TimerControlRegister = Timer_Speedometer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_Speedometer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_Speedometer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_Speedometer_RestoreConfig(void) 
{   
    #if (!Timer_Speedometer_UsingFixedFunction)

        Timer_Speedometer_WriteCounter(Timer_Speedometer_backup.TimerUdb);
        Timer_Speedometer_STATUS_MASK =Timer_Speedometer_backup.InterruptMaskValue;
        #if (Timer_Speedometer_UsingHWCaptureCounter)
            Timer_Speedometer_SetCaptureCount(Timer_Speedometer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_Speedometer_UDB_CONTROL_REG_REMOVED)
            Timer_Speedometer_WriteControlRegister(Timer_Speedometer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_Speedometer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_Speedometer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_Speedometer_Sleep(void) 
{
    #if(!Timer_Speedometer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_Speedometer_CTRL_ENABLE == (Timer_Speedometer_CONTROL & Timer_Speedometer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_Speedometer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_Speedometer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_Speedometer_Stop();
    Timer_Speedometer_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_Speedometer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_Speedometer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_Speedometer_Wakeup(void) 
{
    Timer_Speedometer_RestoreConfig();
    #if(!Timer_Speedometer_UDB_CONTROL_REG_REMOVED)
        if(Timer_Speedometer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_Speedometer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
