/*******************************************************************************
* File Name: PowerMonitor_1_PGA_PM.c  
* Version 2.0
*
* Description:
*  This file provides the power manager source code to the API for PGA 
*  Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "PowerMonitor_1_PGA.h"

static PowerMonitor_1_PGA_BACKUP_STRUCT  PowerMonitor_1_PGA_backup;


/*******************************************************************************
* Function Name: PowerMonitor_1_PGA_SaveConfig
********************************************************************************
*
* Summary:
*  Save the current user register configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PowerMonitor_1_PGA_SaveConfig(void) 
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: PowerMonitor_1_PGA_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore the register configurations.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PowerMonitor_1_PGA_RestoreConfig(void) 
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: PowerMonitor_1_PGA_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PowerMonitor_1_PGA_backup: The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void PowerMonitor_1_PGA_Sleep(void) 
{
    /* Save PGA enable state */
    if((PowerMonitor_1_PGA_PM_ACT_CFG_REG & PowerMonitor_1_PGA_ACT_PWR_EN) != 0u)
    {
        /* Component is enabled */
        PowerMonitor_1_PGA_backup.enableState = 1u;
        /* Stop the configuration */
        PowerMonitor_1_PGA_Stop();
    }
    else
    {
        /* Component is disabled */
        PowerMonitor_1_PGA_backup.enableState = 0u;
    }
    /* Save the configuration */
    PowerMonitor_1_PGA_SaveConfig();
}


/*******************************************************************************
* Function Name: PowerMonitor_1_PGA_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  void
*
* Global variables:
*  PowerMonitor_1_PGA_backup: The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
* 
*******************************************************************************/
void PowerMonitor_1_PGA_Wakeup(void) 
{
    /* Restore the configurations */
    PowerMonitor_1_PGA_RestoreConfig();
     /* Enables the component operation */
    if(PowerMonitor_1_PGA_backup.enableState == 1u)
    {
        PowerMonitor_1_PGA_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
