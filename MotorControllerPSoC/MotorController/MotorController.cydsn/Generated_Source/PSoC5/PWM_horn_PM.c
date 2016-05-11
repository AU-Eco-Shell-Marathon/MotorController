/*******************************************************************************
* File Name: PWM_horn_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_horn.h"

static PWM_horn_backupStruct PWM_horn_backup;


/*******************************************************************************
* Function Name: PWM_horn_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_horn_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_horn_SaveConfig(void) 
{

    #if(!PWM_horn_UsingFixedFunction)
        #if(!PWM_horn_PWMModeIsCenterAligned)
            PWM_horn_backup.PWMPeriod = PWM_horn_ReadPeriod();
        #endif /* (!PWM_horn_PWMModeIsCenterAligned) */
        PWM_horn_backup.PWMUdb = PWM_horn_ReadCounter();
        #if (PWM_horn_UseStatus)
            PWM_horn_backup.InterruptMaskValue = PWM_horn_STATUS_MASK;
        #endif /* (PWM_horn_UseStatus) */

        #if(PWM_horn_DeadBandMode == PWM_horn__B_PWM__DBM_256_CLOCKS || \
            PWM_horn_DeadBandMode == PWM_horn__B_PWM__DBM_2_4_CLOCKS)
            PWM_horn_backup.PWMdeadBandValue = PWM_horn_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_horn_KillModeMinTime)
             PWM_horn_backup.PWMKillCounterPeriod = PWM_horn_ReadKillTime();
        #endif /* (PWM_horn_KillModeMinTime) */

        #if(PWM_horn_UseControl)
            PWM_horn_backup.PWMControlRegister = PWM_horn_ReadControlRegister();
        #endif /* (PWM_horn_UseControl) */
    #endif  /* (!PWM_horn_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_horn_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_horn_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_horn_RestoreConfig(void) 
{
        #if(!PWM_horn_UsingFixedFunction)
            #if(!PWM_horn_PWMModeIsCenterAligned)
                PWM_horn_WritePeriod(PWM_horn_backup.PWMPeriod);
            #endif /* (!PWM_horn_PWMModeIsCenterAligned) */

            PWM_horn_WriteCounter(PWM_horn_backup.PWMUdb);

            #if (PWM_horn_UseStatus)
                PWM_horn_STATUS_MASK = PWM_horn_backup.InterruptMaskValue;
            #endif /* (PWM_horn_UseStatus) */

            #if(PWM_horn_DeadBandMode == PWM_horn__B_PWM__DBM_256_CLOCKS || \
                PWM_horn_DeadBandMode == PWM_horn__B_PWM__DBM_2_4_CLOCKS)
                PWM_horn_WriteDeadTime(PWM_horn_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_horn_KillModeMinTime)
                PWM_horn_WriteKillTime(PWM_horn_backup.PWMKillCounterPeriod);
            #endif /* (PWM_horn_KillModeMinTime) */

            #if(PWM_horn_UseControl)
                PWM_horn_WriteControlRegister(PWM_horn_backup.PWMControlRegister);
            #endif /* (PWM_horn_UseControl) */
        #endif  /* (!PWM_horn_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_horn_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_horn_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_horn_Sleep(void) 
{
    #if(PWM_horn_UseControl)
        if(PWM_horn_CTRL_ENABLE == (PWM_horn_CONTROL & PWM_horn_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_horn_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_horn_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_horn_UseControl) */

    /* Stop component */
    PWM_horn_Stop();

    /* Save registers configuration */
    PWM_horn_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_horn_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_horn_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_horn_Wakeup(void) 
{
     /* Restore registers values */
    PWM_horn_RestoreConfig();

    if(PWM_horn_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_horn_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
