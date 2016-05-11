/*******************************************************************************
* File Name: ADC_V_PM.c
* Version 3.0
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_V.h"


/***************************************
* Local data allocation
***************************************/

static ADC_V_BACKUP_STRUCT  ADC_V_backup =
{
    ADC_V_DISABLED
};


/*******************************************************************************
* Function Name: ADC_V_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_V_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_V_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_V_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_V_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The ADC_V_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_V_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void ADC_V_Sleep(void)
{
    if((ADC_V_PWRMGR_SAR_REG  & ADC_V_ACT_PWR_SAR_EN) != 0u)
    {
        if((ADC_V_SAR_CSR0_REG & ADC_V_SAR_SOF_START_CONV) != 0u)
        {
            ADC_V_backup.enableState = ADC_V_ENABLED | ADC_V_STARTED;
        }
        else
        {
            ADC_V_backup.enableState = ADC_V_ENABLED;
        }
        ADC_V_Stop();
    }
    else
    {
        ADC_V_backup.enableState = ADC_V_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC_V_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  ADC_V_Sleep() was called. If the component was enabled before the
*  ADC_V_Sleep() function was called, the
*  ADC_V_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_V_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void ADC_V_Wakeup(void)
{
    if(ADC_V_backup.enableState != ADC_V_DISABLED)
    {
        ADC_V_Enable();
        #if(ADC_V_DEFAULT_CONV_MODE != ADC_V__HARDWARE_TRIGGER)
            if((ADC_V_backup.enableState & ADC_V_STARTED) != 0u)
            {
                ADC_V_StartConvert();
            }
        #endif /* End ADC_V_DEFAULT_CONV_MODE != ADC_V__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */
