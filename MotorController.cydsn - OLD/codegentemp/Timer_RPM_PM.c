/*******************************************************************************
* File Name: Timer_RPM_PM.c
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

#include "Timer_RPM.h"

static Timer_RPM_backupStruct Timer_RPM_backup;


/*******************************************************************************
* Function Name: Timer_RPM_SaveConfig
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
*  Timer_RPM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_RPM_SaveConfig(void) 
{
    #if (!Timer_RPM_UsingFixedFunction)
        Timer_RPM_backup.TimerUdb = Timer_RPM_ReadCounter();
        Timer_RPM_backup.InterruptMaskValue = Timer_RPM_STATUS_MASK;
        #if (Timer_RPM_UsingHWCaptureCounter)
            Timer_RPM_backup.TimerCaptureCounter = Timer_RPM_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_RPM_UDB_CONTROL_REG_REMOVED)
            Timer_RPM_backup.TimerControlRegister = Timer_RPM_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_RPM_RestoreConfig
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
*  Timer_RPM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_RPM_RestoreConfig(void) 
{   
    #if (!Timer_RPM_UsingFixedFunction)

        Timer_RPM_WriteCounter(Timer_RPM_backup.TimerUdb);
        Timer_RPM_STATUS_MASK =Timer_RPM_backup.InterruptMaskValue;
        #if (Timer_RPM_UsingHWCaptureCounter)
            Timer_RPM_SetCaptureCount(Timer_RPM_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_RPM_UDB_CONTROL_REG_REMOVED)
            Timer_RPM_WriteControlRegister(Timer_RPM_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_RPM_Sleep
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
*  Timer_RPM_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_RPM_Sleep(void) 
{
    #if(!Timer_RPM_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_RPM_CTRL_ENABLE == (Timer_RPM_CONTROL & Timer_RPM_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_RPM_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_RPM_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_RPM_Stop();
    Timer_RPM_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_RPM_Wakeup
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
*  Timer_RPM_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_RPM_Wakeup(void) 
{
    Timer_RPM_RestoreConfig();
    #if(!Timer_RPM_UDB_CONTROL_REG_REMOVED)
        if(Timer_RPM_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_RPM_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
