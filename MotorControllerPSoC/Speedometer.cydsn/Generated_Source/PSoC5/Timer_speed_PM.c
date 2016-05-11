/*******************************************************************************
* File Name: Timer_speed_PM.c
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

#include "Timer_speed.h"

static Timer_speed_backupStruct Timer_speed_backup;


/*******************************************************************************
* Function Name: Timer_speed_SaveConfig
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
*  Timer_speed_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_speed_SaveConfig(void) 
{
    #if (!Timer_speed_UsingFixedFunction)
        Timer_speed_backup.TimerUdb = Timer_speed_ReadCounter();
        Timer_speed_backup.InterruptMaskValue = Timer_speed_STATUS_MASK;
        #if (Timer_speed_UsingHWCaptureCounter)
            Timer_speed_backup.TimerCaptureCounter = Timer_speed_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_speed_UDB_CONTROL_REG_REMOVED)
            Timer_speed_backup.TimerControlRegister = Timer_speed_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_speed_RestoreConfig
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
*  Timer_speed_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_speed_RestoreConfig(void) 
{   
    #if (!Timer_speed_UsingFixedFunction)

        Timer_speed_WriteCounter(Timer_speed_backup.TimerUdb);
        Timer_speed_STATUS_MASK =Timer_speed_backup.InterruptMaskValue;
        #if (Timer_speed_UsingHWCaptureCounter)
            Timer_speed_SetCaptureCount(Timer_speed_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_speed_UDB_CONTROL_REG_REMOVED)
            Timer_speed_WriteControlRegister(Timer_speed_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_speed_Sleep
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
*  Timer_speed_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_speed_Sleep(void) 
{
    #if(!Timer_speed_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_speed_CTRL_ENABLE == (Timer_speed_CONTROL & Timer_speed_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_speed_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_speed_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_speed_Stop();
    Timer_speed_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_speed_Wakeup
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
*  Timer_speed_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_speed_Wakeup(void) 
{
    Timer_speed_RestoreConfig();
    #if(!Timer_speed_UDB_CONTROL_REG_REMOVED)
        if(Timer_speed_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_speed_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
