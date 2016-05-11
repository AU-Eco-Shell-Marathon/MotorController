/*******************************************************************************
* File Name: PowerMonitor_1.c
* Version 1.60
*
* Description:
*  This file provides the source code to the API for the Power Monitor
*  Component.
*
* Note:
*
*******************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "PowerMonitor_1.h"
#include "PowerMonitor_1_PGA.h"
#include "PowerMonitor_1_PM_AMux_Voltage.h"
#include "PowerMonitor_1_PM_AMux_Current.h"
#include "PowerMonitor_1_ADC.h"


/* To run the initialization block only at the start up */
uint8 PowerMonitor_1_initVar = 0u;

/* To run the threshold initialization only at the start up */
uint8 PowerMonitor_1_initThreshold = 0u;

/* last voltage, current annd auxiliary voltage channels */
extern uint8 CYDATA PowerMonitor_1_lastVoltageChan;
extern uint8 CYDATA PowerMonitor_1_lastCurrentChan;
extern uint8 CYDATA PowerMonitor_1_lastAuxVoltChan;

#if ((PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) || (PowerMonitor_1_NUM_AUX_INPUTS > 0u))
    extern uint8 CYDATA PowerMonitor_1_index;
#endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */

#if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)
    extern uint8 CYDATA PowerMonitor_1_auxIndex;
    extern int16 PowerMonitor_1_auxStore;
#endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0u) */


/* Fault and Warn Mask variables */
volatile uint32 CYDATA PowerMonitor_1_faultMask;
volatile uint32 CYDATA PowerMonitor_1_warnMask;

/* Enable fault and Enable warn variables */
CYBIT PowerMonitor_1_faultEnable;
CYBIT PowerMonitor_1_warnEnable;

/* Fault and Warn source variables */
volatile uint8 CYDATA PowerMonitor_1_warnSources;
volatile uint8 CYDATA PowerMonitor_1_faultSources;

/* Warn and Fault source status */
volatile uint8 CYDATA PowerMonitor_1_warnSourceStatus = 0u;
volatile uint8 CYDATA PowerMonitor_1_faultSourceStatus = 0u;

/* Warn and Fault status for power monitors */
volatile uint32 CYDATA PowerMonitor_1_OCWarnStatus = 0ul;
volatile uint32 CYDATA PowerMonitor_1_UVWarnStatus = 0ul;
volatile uint32 CYDATA PowerMonitor_1_OVWarnStatus = 0ul;
volatile uint32 CYDATA PowerMonitor_1_OCFaultStatus = 0ul;
volatile uint32 CYDATA PowerMonitor_1_UVFaultStatus = 0ul;
volatile uint32 CYDATA PowerMonitor_1_OVFaultStatus = 0ul;

/* Voltage tab related arrays as set in the customizer */
uint8 PowerMonitor_1_VoltageType[1] = { 0 };
uint16 PowerMonitor_1_UVWarnThreshold[1] = { 1700 };
uint16 PowerMonitor_1_OVWarnThreshold[1] = { 2825 };
uint16 PowerMonitor_1_UVFaultThreshold[1] = { 750 };
uint16 PowerMonitor_1_OVFaultThreshold[1] = { 3000 };
float PowerMonitor_1_VoltageScale[1] = { 1 };

/* Current tab related arrays as set in the customizer */
uint8 PowerMonitor_1_CurrentType[1] = { 1 };
float PowerMonitor_1_OCWarnThreshold[1] = { 9 };
float PowerMonitor_1_OCFaultThreshold[1] = { 12 };
float PowerMonitor_1_RShunt[1] = { 5 };
float PowerMonitor_1_CSAGain[1] = { 10 };
uint8 PowerMonitor_1_ActiveCurrentChan[1] = { 1 };

#if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)
    /* Auxiliary voltage table related arrays as set in the customizer */
    
    /* The enable mask for auxiliary channels which will be scanned in overall cycle */
    volatile uint8 PowerMonitor_1_auxEnableMask;
#endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0u) */

/* Local function protoptype */
static void PowerMonitor_1_StartupIsr(void)                                           \
                                                                ;
static int32 PowerMonitor_1_mVToCounts(uint16 mV, uint8 chan, uint8 * convSuccess)     \
                                                                ;
static int32 PowerMonitor_1_AToCounts(float current, uint8 chan, uint8 * convSuccess)  \
                                                                ;
static float PowerMonitor_1_GetMeasuredVal(uint8 chan, uint8 chanType)                \
                                                                ;
static float PowerMonitor_1_GetFiltRaw(uint8 chan, uint8 chanType)                    \
                                                                ;
static uint8 PowerMonitor_1_GetArrayIndex(uint8 chan)                                 \
                                                                ;


/******************************************************************************
* Function Name: PowerMonitor_1_Init
*******************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
* PowerMonitor_1_Start(). Also includes method for running self calibration.
*
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_Init(void) 
{
    /* Set the mode, and mask for warn and fault. */
    PowerMonitor_1_EnableFault();
    PowerMonitor_1_EnableWarn();
    PowerMonitor_1_SetFaultMode(PowerMonitor_1_DEFAULT_FAULT_MODE);
    PowerMonitor_1_SetWarnMode(PowerMonitor_1_DEFAULT_WARN_MODE);
    PowerMonitor_1_SetFaultMask(PowerMonitor_1_DEFAULT_FAULT_MASK);
    PowerMonitor_1_SetWarnMask(PowerMonitor_1_DEFAULT_WARN_MASK);
    #if (PowerMonitor_1_NUM_AUX_INPUTS > 0)
        /* Enable all auxiliary channels by default */
        PowerMonitor_1_auxEnableMask = PowerMonitor_1_DEFAULT_AUX_CHANNELS_MASK;
    #endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0) */
    PowerMonitor_1_maxChannels = PowerMonitor_1_MAX_CHANNELS;
}


/******************************************************************************
* Function Name: PowerMonitor_1_Enable
*******************************************************************************
*
* Summary:
*  Enables the power monitor component hardware blocks and starts scanning.
*
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_Enable(void) 
{
    uint8 index;
    /* Start the hardware blocks */
    PowerMonitor_1_PM_AMux_Voltage_Start();
    PowerMonitor_1_PM_AMux_Current_Start();
    PowerMonitor_1_PGA_Start();
    #if (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_2048MV)
        PowerMonitor_1_PGA_SetGain(PowerMonitor_1_PGA_GAIN_01);
    #else
        PowerMonitor_1_PGA_SetGain(PowerMonitor_1_PGA_GAIN_02);
    #endif /* (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_2048MV) */

    PowerMonitor_1_initThreshold = 0u;
    PowerMonitor_1_StartupIsr();
    /* Set warn and fault thresholds */
    for (index = 0u; index < PowerMonitor_1_NUMBER_OF_CONVERTERS; index++)
    {
        PowerMonitor_1_SetOVFaultThreshold((index + 1), PowerMonitor_1_OVFaultThreshold[index]);
        PowerMonitor_1_SetOVWarnThreshold((index + 1), PowerMonitor_1_OVWarnThreshold[index]);
        PowerMonitor_1_SetUVWarnThreshold((index + 1), PowerMonitor_1_UVWarnThreshold[index]);
        PowerMonitor_1_SetUVFaultThreshold((index + 1), PowerMonitor_1_UVFaultThreshold[index]);
        PowerMonitor_1_SetOCFaultThreshold((index + 1), PowerMonitor_1_OCFaultThreshold[index]);
        PowerMonitor_1_SetOCWarnThreshold((index + 1), PowerMonitor_1_OCWarnThreshold[index]);
    }
    PowerMonitor_1_initThreshold = 1u;
}


/*******************************************************************************
* Function Name: PowerMonitor_1_Start
********************************************************************************
*
* Summary:
*  The start function initializes the power monitor with the default
*  values by calling Init() API if component has been not initialized before.
*  Also calls Enable() API.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PowerMonitor_1_initVar:  Used to check the initial configuration,
*  modified when this function is called for the first time.
*
*******************************************************************************/
void PowerMonitor_1_Start() 
{
    if(PowerMonitor_1_initVar == 0u)
    {
       PowerMonitor_1_Init();
       PowerMonitor_1_initVar = 1u;
    }

    /* Enable the component */
    PowerMonitor_1_Enable();
}


/*******************************************************************************
* Function Name: PowerMonitor_1_Stop
********************************************************************************
*
* Summary:
*  Stops the power monitor block operation which includes stopping of ADC
*  sampling.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  pgood, warn, fault and eoc outputs are de-asserted.
*
*******************************************************************************/
void PowerMonitor_1_Stop(void) 
{
    /* Disable the interrupt */
    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);

    /* Stop the hardware */
    PowerMonitor_1_PM_AMux_Voltage_Stop();
    PowerMonitor_1_PM_AMux_Current_Stop();
    PowerMonitor_1_PGA_Stop();
    PowerMonitor_1_ADC_Stop();

    /* De-assert the output signals */
    #if !defined(PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
        PowerMonitor_1_CONTROL1_REG &= ~(PowerMonitor_1_EOC_MASK | PowerMonitor_1_WARN_MASK |
                                           PowerMonitor_1_FAULT_MASK);
    #endif /* (PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED) */

    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED)
         PowerMonitor_1_PGOOD_CONTROL1_REG &= ~PowerMonitor_1_PGOOD_CTRL_MASK;
    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED) */

    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED)
        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8)
            PowerMonitor_1_PGOOD_CONTROL2_REG &= ~PowerMonitor_1_PGOOD_CTRL_MASK;
        #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8) */
    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED) */

    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED)
        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16)
            PowerMonitor_1_PGOOD_CONTROL3_REG &= ~PowerMonitor_1_PGOOD_CTRL_MASK;
        #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16) */
    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED) */

    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED)
        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24)
            PowerMonitor_1_PGOOD_CONTROL4_REG &= ~PowerMonitor_1_PGOOD_CTRL_MASK;
        #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24)*/
    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED) */
}


/*******************************************************************************
* Function Name: PowerMonitor_1_EnableFault
********************************************************************************
*
* Summary:
*  This API enables the generation of Fault signal. Enabling of fault sources
*  is configured using the PowerMonitor_1_SetFaultMode() and the
*  PowerMonitor_1_SetFaultMask() APIs. Fault signal generation is
*  automatically enabled by Init() API.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_EnableFault(void) 
{
    /* Enable fault */
    PowerMonitor_1_faultEnable = PowerMonitor_1_CYTRUE;
}


/*******************************************************************************
* Function Name: PowerMonitor_1_DisableFault
********************************************************************************
*
* Summary:
*  Disables the genration of Fault signal.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Fault output is de-asserted.
*
*******************************************************************************/
void PowerMonitor_1_DisableFault(void) 
{
    /* Disable the fault */
    PowerMonitor_1_faultEnable = PowerMonitor_1_CYFALSE;
    /* De-assert the fault output signal */
    #if !defined(PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
        /* Disable the interrupt */
        PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
        PowerMonitor_1_CONTROL1_REG &= ~PowerMonitor_1_FAULT_MASK;
        /* Enable the interrupt */
        PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
    #endif /* (PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED) */
}


/*******************************************************************************
* Function Name: PowerMonitor_1_SetFaultMode
********************************************************************************
*
* Summary:
*  This API configures Fault sources for the component. Three Fault sources
*  available are: OV, UV, and OC.
*
* Parameters:
*  uint8 faultMode: Value to set Fault sources.
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_SetFaultMode(uint8 faultMode) 
{
    /* Set the fault mode */
    PowerMonitor_1_faultSources = faultMode & PowerMonitor_1_FAULT_MODE_MASK;
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetFaultMode
********************************************************************************
*
* Summary:
*  This API returns the enabled Fault sources for the component.
*
* Parameters:
*  None
*
* Return:
*  uint8: The value corresponding to the enabled fault sources.
*
*******************************************************************************/
uint8 PowerMonitor_1_GetFaultMode(void) 
{
    return (PowerMonitor_1_faultSources);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_SetFaultMask
********************************************************************************
*
* Summary:
*  This API enables or disables faults from each power converters through a
*  mask. Masking applies to all fault sources. By default all power converters
*  have their fault masks enabled.
*
* Parameters:
*  uint32 faultMask: 32-bit mask value
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_SetFaultMask(uint32 faultMask) 
{
    /* Disable the interrupt */
    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
    /* Set the fault mask */
    PowerMonitor_1_faultMask = faultMask & PowerMonitor_1_DEFAULT_FAULT_MASK;
    /* Enable the interrupt */
    PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetFaultMask
********************************************************************************
*
* Summary:
*  This API returns fault mask status of each power converter. Masking applies
*  to all sources.
*
* Parameters:
*  None
*
* Return:
*  uint32: 32-bit mask value set for each of the sources.
*
*******************************************************************************/
uint32 PowerMonitor_1_GetFaultMask(void) 
{
    return (PowerMonitor_1_faultMask);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetFaultSource
********************************************************************************
*
* Summary:
*  This API returns the pending fault source. This API can
*  be used to poll the fault status of the component.
*
* Parameters:
*  None
*
* Return:
*  uint8: 8-bit value indicating pending fault sources form the component.
*
*******************************************************************************/
uint8 PowerMonitor_1_GetFaultSource(void) 
{
    /* Get the status */
    return (PowerMonitor_1_faultSourceStatus);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetOVFaultStatus
********************************************************************************
*
* Summary:
*  This API returns over voltage fault status of each of the power converters.
*  Status is reported regardless of the Fault Mask.
*
* Parameters:
*  None
*
* Return:
*  uint32: 32-bit value indicating OV status of power converters
*
* Side Effects:
*  Calling this API clears the Fault condition source sticky bits.
*
*******************************************************************************/
uint32 PowerMonitor_1_GetOVFaultStatus(void) 
{
    uint32 OVFaultStatus;
    /* Disable the interrupt */
    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
    /* Read the over voltage fault status */
    OVFaultStatus = PowerMonitor_1_OVFaultStatus;
    /* Clear OV fault source status bit */
    PowerMonitor_1_faultSourceStatus &= ~PowerMonitor_1_ENABLE_OV_FAULT_SOURCE;
    /* Clear OV fault status */
    PowerMonitor_1_OVFaultStatus = PowerMonitor_1_RESET_OV_FAULT_STATUS;
    /* Check for other possible fault and then clear the hardware */
    #if !defined(PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
        if((PowerMonitor_1_UVFaultStatus == PowerMonitor_1_RESET_UV_FAULT_STATUS) &&
           (PowerMonitor_1_OCFaultStatus == PowerMonitor_1_RESET_OC_FAULT_STATUS))
        {
            PowerMonitor_1_CONTROL1_REG &= ~PowerMonitor_1_FAULT_MASK;
        }
    #endif /* (PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED) */

    /* Enable the interrupt */
    PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);

    return (OVFaultStatus);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetUVFaultStatus
********************************************************************************
*
* Summary:
*  This API returns under voltage fault status of each of the power converters.
*
* Parameters:
*  None
*
* Return:
*  uint32: 32-bit value indicating under voltage status of power converters.
*
* Side Effects:
*  Calling this API clears the fault condition source sticky bits.
*
*******************************************************************************/
uint32 PowerMonitor_1_GetUVFaultStatus(void) 
{
    uint32 UVFaultStatus;

    /* Disable the interrupt */
    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
    /* Read the under voltage fault status */
    UVFaultStatus = PowerMonitor_1_UVFaultStatus;
    /* Clear UV fault source status bit */
    PowerMonitor_1_faultSourceStatus &= ~PowerMonitor_1_ENABLE_UV_FAULT_SOURCE;
    /* Clear UV fault status */
    PowerMonitor_1_UVFaultStatus = PowerMonitor_1_RESET_UV_FAULT_STATUS;
    /* Check for other possible fault and then clear the hardware */
    #if !defined(PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
        if((PowerMonitor_1_OVFaultStatus == PowerMonitor_1_RESET_OV_FAULT_STATUS) &&
           (PowerMonitor_1_OCFaultStatus == PowerMonitor_1_RESET_OC_FAULT_STATUS))
        {
            PowerMonitor_1_CONTROL1_REG &= ~PowerMonitor_1_FAULT_MASK;
        }
    #endif /* (PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED) */

    /* Enable the interrupt */
    PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);

    return (UVFaultStatus);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetOCFaultStatus
********************************************************************************
*
* Summary:
*  This API returns Over current fault status of each of the power converters.
*  The status is regardless of fault mask.
*
* Parameters:
*  None
*
* Return:
*  uint32: 32-bit valud indicating Over current status of power converters.
*
* Side Effects:
*  Calling this API clears the fault condition source sticky bits.
*
*******************************************************************************/
uint32 PowerMonitor_1_GetOCFaultStatus(void) 
{
    uint32 OCFaultStatus;
    /* Disable the interrupt */
    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
    /* Read the over current fault status */
    OCFaultStatus = PowerMonitor_1_OCFaultStatus;
    /* Clear OC fault source status bit */
    PowerMonitor_1_faultSourceStatus &= ~PowerMonitor_1_ENABLE_OC_FAULT_SOURCE;
    /* Clear OC fault status */
    PowerMonitor_1_OCFaultStatus = PowerMonitor_1_RESET_OC_FAULT_STATUS;
    /* Check for other possible fault and then clear the hardware */
    #if !defined(PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
        if((PowerMonitor_1_UVFaultStatus == PowerMonitor_1_RESET_UV_FAULT_STATUS) &&
           (PowerMonitor_1_OVFaultStatus == PowerMonitor_1_RESET_OV_FAULT_STATUS))
        {
            PowerMonitor_1_CONTROL1_REG &= ~PowerMonitor_1_FAULT_MASK;
        }
    #endif /* (PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED) */

    /* Enable the interrupt */
    PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);

    return (OCFaultStatus);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_EnableWarn
********************************************************************************
*
* Summary:
*  This API enables the generation of warning signal. Enabling of warning
*  sources is configured using PowerMonitor_1_SetWarnMode() and
*  PowerMonitor_1_SetWarnMask() APIs. Warning signal generation is
*  automatically enabled by the Init() API.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_EnableWarn(void) 
{
    /* Enable the warn */
    PowerMonitor_1_warnEnable = PowerMonitor_1_CYTRUE;
}


/*******************************************************************************
* Function Name: PowerMonitor_1_DisableWarn
********************************************************************************
*
* Summary:
*  Disables the generation of warning signal.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Warning output is de-asserted.
*
*******************************************************************************/
void PowerMonitor_1_DisableWarn(void) 
{
    /* Disable the warn */
    PowerMonitor_1_warnEnable = PowerMonitor_1_CYFALSE;
    /* De-assert the warn signal */
    #if !defined(PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
        /* Disable the interrupt */
        PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
        PowerMonitor_1_CONTROL1_REG &= ~PowerMonitor_1_WARN_MASK;
        /* Enable the interrupt */
        PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
    #endif /* (PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED) */
}


/*******************************************************************************
* Function Name: PowerMonitor_1_SetWarnMode
********************************************************************************
*
* Summary:
*  This API sets the warning sources for the component. Available three
*  warning sources are OV, UV, OC.
*
* Parameters:
*  uint8 warnMode: 8-bit value indicating the warn source to set.
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_SetWarnMode(uint8 warnMode) 
{
    /* Set the warn mode */
    PowerMonitor_1_warnSources = (warnMode & PowerMonitor_1_WARN_MODE_MASK);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetWarnMode
********************************************************************************
*
* Summary:
*  This API retruns the enabled warning sources for the component.
*
* Parameters:
*  None
*
* Return:
*  uint8: 8-bit value indicating the enabled warning sources
*
*******************************************************************************/
uint8 PowerMonitor_1_GetWarnMode(void) 
{
    return (PowerMonitor_1_warnSources);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_SetWarnMask
********************************************************************************
*
* Summary:
*  Calling this API enables or disables warnings from each power converter
*  through a mask. Masking applies to all warning sources. By default
*  warning masks are enabled for each of the power converter.
*
* Parameters:
*  uint32 warnMask: 32-bit warning mask value.
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_SetWarnMask(uint32 warnMask) 
{
    /* Disable the interrupt */
    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
    /* Set the warn mask */
    PowerMonitor_1_warnMask = (warnMask & PowerMonitor_1_DEFAULT_WARN_MASK);
    /* Enable the interrupt */
    PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetWarnMask
********************************************************************************
*
* Summary:
*  This API returns the warning mask status of each of the power converter.
*
* Parameters:
*  None
*
* Return:
*  uint32: 32-bit value indicating warning mask status.
*
*******************************************************************************/
uint32 PowerMonitor_1_GetWarnMask(void) 
{
    return (PowerMonitor_1_warnMask);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetWarnSource
********************************************************************************
*
* Summary:
*  This API returns pending warning source. This API can be
*  used to poll the warning status of the component.
*
* Parameters:
*  None
*
* Return:
*  uint8: 8-bit value indicating the pending warning sources.
*
*******************************************************************************/
uint8 PowerMonitor_1_GetWarnSource(void) 
{
    /* Get the status */
    return (PowerMonitor_1_warnSourceStatus);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetOVWarnStatus
********************************************************************************
*
* Summary:
*  This API can be used to get the over voltage warning status of each power
*  converter. The status is regardless of Warning Mask set.
*
* Parameters:
*  None
*
* Return:
*  uint32: 32-bit status indicating the over voltage warning status of each
*  power converter.
*
*******************************************************************************/
uint32 PowerMonitor_1_GetOVWarnStatus(void) 
{
    uint32 OVWarnStatus;

    /* Disable the interrupt */
    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
    /* Read the over voltage warn status */
    OVWarnStatus = PowerMonitor_1_OVWarnStatus;
    /* Clear OV warn status bit */
    PowerMonitor_1_warnSourceStatus &= ~PowerMonitor_1_ENABLE_OV_WARN_SOURCE;
    /* Clear OV warn status */
    PowerMonitor_1_OVWarnStatus = PowerMonitor_1_RESET_OV_WARN_STATUS;
    /* Check for other possible warn and then clear the hardware */
    #if !defined(PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
        if((PowerMonitor_1_UVWarnStatus == PowerMonitor_1_RESET_UV_WARN_STATUS) &&
           (PowerMonitor_1_OCWarnStatus == PowerMonitor_1_RESET_OC_WARN_STATUS))
        {
            PowerMonitor_1_CONTROL1_REG &= ~PowerMonitor_1_WARN_MASK;
        }
    #endif /* (PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED) */

    /* Enable the interrupt */
    PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);

    return (OVWarnStatus);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetUVWarnStatus
********************************************************************************
*
* Summary:
*  This API returns the under voltage warning status of each power converter.
*  The status is regardless of Warning mask.
*
* Parameters:
*  None
*
* Return:
*  uint32: 32-bit value indicating under voltage warning status for each power
*  converter.
*
*******************************************************************************/
uint32 PowerMonitor_1_GetUVWarnStatus(void) 
{
    uint32 UVWarnStatus;

    /* Disable the interrupt */
    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
    /* Read the under voltage warn status */
    UVWarnStatus = PowerMonitor_1_UVWarnStatus;
    /* Clear UV warn source status bit */
    PowerMonitor_1_warnSourceStatus &= ~PowerMonitor_1_ENABLE_UV_WARN_SOURCE;
    /* Clear UV warn status */
    PowerMonitor_1_UVWarnStatus = PowerMonitor_1_RESET_UV_WARN_STATUS;
    /* Check for other possible warn and then clear the hardware */
    #if !defined(PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
        if((PowerMonitor_1_OVWarnStatus == PowerMonitor_1_RESET_OV_WARN_STATUS) &&
           (PowerMonitor_1_OCWarnStatus == PowerMonitor_1_RESET_OC_WARN_STATUS))
        {
            PowerMonitor_1_CONTROL1_REG &= ~PowerMonitor_1_WARN_MASK;
        }
    #endif /* (PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED) */

    /* Enable the interrupt */
    PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);

    return (UVWarnStatus);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetOCWarnStatus
********************************************************************************
*
* Summary:
*  This API returns over current warning status for each power converter. The
*  status is regardless of Warning mask.
*
* Parameters:
*  None
*
* Return:
*  uint32: 32-bit value indicating the over current warning status of each
*  power converter.
*
*******************************************************************************/
uint32 PowerMonitor_1_GetOCWarnStatus(void) 
{
    uint32 OCWarnStatus;

    /* Disable the interrupt */
    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
    /* Read the over current warn status */
    OCWarnStatus = PowerMonitor_1_OCWarnStatus;
    /* Clear OC warn source status bit */
    PowerMonitor_1_warnSourceStatus &= ~PowerMonitor_1_ENABLE_OC_WARN_SOURCE;
    /* Clear OC warn status */
    PowerMonitor_1_OCWarnStatus = PowerMonitor_1_RESET_OC_WARN_STATUS;
    /* Check for other possible warn and then clear the hardware */
    #if !defined(PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
        if((PowerMonitor_1_UVWarnStatus == PowerMonitor_1_RESET_UV_WARN_STATUS) &&
           (PowerMonitor_1_OVWarnStatus == PowerMonitor_1_RESET_OV_WARN_STATUS))
        {
            PowerMonitor_1_CONTROL1_REG &= ~PowerMonitor_1_WARN_MASK;
        }
    #endif /* (PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED) */

    /* Enable the interrupt */
    PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);

    return (OCWarnStatus);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_SetUVWarnThreshold
********************************************************************************
*
* Summary:
*  This API can be used to set the under voltage warning threshold for the
*  specified power converter.
*
* Parameters:
*  uint8 converterNum: Converter number for which the threshold to be set.
*                      Valid range is 1-32.
*  uint16 uvWarnThreshold: Under voltage warning threshold in mv. Valid range
*                          is 1-65,535.
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_SetUVWarnThreshold(uint8 converterNum, uint16 uvWarnThreshold) \
     
{
    int32 uvWarnCounts;
    uint8 convRes = PowerMonitor_1_CYFALSE;

    uvWarnCounts = PowerMonitor_1_mVToCounts(uvWarnThreshold, converterNum, &convRes);
    if(convRes == PowerMonitor_1_CYTRUE)
    {
        /* Disable the interrupt */
        PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
        /* Set the under voltage warn threshold */
        PowerMonitor_1_warnWin[converterNum - 1u].UVWarnThrshldCounts = uvWarnCounts;
        PowerMonitor_1_warnWin[converterNum - 1u].UVWarnThrshldVolts = uvWarnThreshold;
        /* Enable the interrupt */
        PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
    }
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetUVWarnThreshold
********************************************************************************
*
* Summary:
*  This API returns the power converter under voltage warning threshold for
*  the specified power converter.
*
* Parameters:
*  uint8 converterNum: Converter number for which to get the UV threshold
*                      value. Valid range is 1-32.
*
* Return:
*  uint16: 16-bit value indicating the under voltage threshold in mV.
*
*******************************************************************************/
uint16 PowerMonitor_1_GetUVWarnThreshold(uint8 converterNum) \
       
{
    uint16 uvWarnThreshold = 0u;

    if ((converterNum > 0u) && (converterNum <= PowerMonitor_1_NUMBER_OF_CONVERTERS))
    {
        uvWarnThreshold = PowerMonitor_1_warnWin[converterNum - 1u].UVWarnThrshldVolts;
    }

    return (uvWarnThreshold);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_SetOVWarnThreshold
********************************************************************************
*
* Summary:
*  This API can be used to set the over voltage warning threshold for the
*  specified.power converter.
*
* Parameters:
*  uint8 converterNum: 8-bit value indicating the converter number for which
                       OV warning threshold to be set.
*  uint16 ovWarnThreshold: Over voltage warning threshold in mV. Valid range
                           is 1 - 65,535.
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_SetOVWarnThreshold(uint8 converterNum, uint16 ovWarnThreshold) \
     
{
    int32 ovWarnCounts;
    uint8 convRes = PowerMonitor_1_CYFALSE;

    ovWarnCounts = PowerMonitor_1_mVToCounts(ovWarnThreshold, converterNum, &convRes);
    if(convRes == PowerMonitor_1_CYTRUE)
    {
        /* Disable the interrupt */
        PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
        /* Set the over voltage warn threshold for warn checking */
        PowerMonitor_1_warnWin[converterNum - 1u].OVWarnThrshldCounts = ovWarnCounts;
        PowerMonitor_1_warnWin[converterNum - 1u].OVWarnThrshldVolts = ovWarnThreshold;
        /* Enable the interrupt */
        PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
    }
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetOVWarnThreshold
********************************************************************************
*
* Summary:
*  This API returns the Over voltage warning threshold set for specified.power
*  converter.
*
* Parameters:
*  uint8 converterNum: Converter number for which to get the OV threshold
*                      value. Valid range is 1-32.
*
* Return:
*  uint16: 16-bit value indicating the over voltage threshold in mV.
*
*******************************************************************************/
uint16 PowerMonitor_1_GetOVWarnThreshold(uint8 converterNum) \
       
{
    uint16 ovWarnThreshold = 0u;

    if ((converterNum > 0u) && (converterNum <= PowerMonitor_1_NUMBER_OF_CONVERTERS))
    {
        ovWarnThreshold = PowerMonitor_1_warnWin[converterNum - 1u].OVWarnThrshldVolts;
    }

    return (ovWarnThreshold);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_SetUVFaultThreshold
********************************************************************************
*
* Summary:
*  This API can be used to set the under voltage fault threshold for the
*  specified power converter.
*
* Parameters:
*  uint8 converterNum: 8-bit value indicating the converter number for which
                       UV fault threshold to be set.
*  uint16 UVFaultThreshold: Under voltage fault threshold in mV. Valid range
                           is 1 - 65,535.
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_SetUVFaultThreshold(uint8 converterNum, uint16 uvFaultThreshold) \
     
{
    int32 uvFaultCounts;
    uint8 convRes = PowerMonitor_1_CYFALSE;

    uvFaultCounts = PowerMonitor_1_mVToCounts(uvFaultThreshold, converterNum, &convRes);
    if(convRes == PowerMonitor_1_CYTRUE)
    {
        /* Disable the interrupt */
        PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
        /* Set the threshold */
        PowerMonitor_1_faultWin[converterNum - 1u].UVFaultThrshldCounts = uvFaultCounts;
        PowerMonitor_1_faultWin[converterNum - 1u].UVFaultThrshldVolts = uvFaultThreshold;
        /* Enable the interrupt */
        PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
    }
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetUVFaultThreshold
********************************************************************************
*
* Summary:
*  This API can be used to get the under voltage fault threshold set for the
*  specified power converter..
*
* Parameters:
*  uint8 converterNum: Converter number for which to get the UV fault threshold
*                      value. Valid range is 1-32.
*
* Return:
*  uint16: 16-bit value indicating the under voltage fault threshold in mV.
*
*******************************************************************************/
uint16 PowerMonitor_1_GetUVFaultThreshold(uint8 converterNum) \
       
{
    uint16 uvFaultThreshold = 0u;

    if ((converterNum > 0u) && (converterNum <= PowerMonitor_1_NUMBER_OF_CONVERTERS))
    {
        uvFaultThreshold = PowerMonitor_1_faultWin[converterNum - 1u].UVFaultThrshldVolts;
    }

    return (uvFaultThreshold);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_SetOVFaultThreshold
********************************************************************************
*
* Summary:
*  This API can be used to set the over voltage fault threshold for the
*  specified power converter.
*
* Parameters:
*  uint8 converterNum: 8-bit value indicating the converter number for which
                       OV fault threshold to be set.
*  uint16 OVFaultThreshold: Over voltage fault threshold in mV. Valid range
                           is 1 - 65,535.
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_SetOVFaultThreshold(uint8 converterNum, uint16 ovFaultThreshold) \
     
{
    int32 ovFaultCounts;
    uint8 convRes = PowerMonitor_1_CYFALSE;

    ovFaultCounts = PowerMonitor_1_mVToCounts(ovFaultThreshold, converterNum, &convRes);
    if(convRes == PowerMonitor_1_CYTRUE)
    {
        /* Disable the interrupt */
        PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
        /* Set the over voltage fault threshold */
        PowerMonitor_1_faultWin[converterNum - 1u].OVFaultThrshldCounts = ovFaultCounts;
        PowerMonitor_1_faultWin[converterNum - 1u].OVFaultThrshldVolts = ovFaultThreshold;
        PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
    }
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetOVFaultThreshold
********************************************************************************
*
* Summary:
*  This API can be used to get the over voltage fault threshold for the
*  specified power converter.
*
* Parameters:
*  uint8 converterNum: Converter number for which to get the OV fault threshold
*                      value. Valid range is 1-32.
*
* Return:
*  uint16: 16-bit value indicating the over voltage fault threshold in mV.
*
*******************************************************************************/
uint16 PowerMonitor_1_GetOVFaultThreshold(uint8 converterNum) \
       
{
    uint16 ovFaultThreshold = 0u;

    if ((converterNum > 0u) && (converterNum <= PowerMonitor_1_NUMBER_OF_CONVERTERS))
    {
       ovFaultThreshold = PowerMonitor_1_faultWin[converterNum - 1u].OVFaultThrshldVolts;
    }

    return (ovFaultThreshold);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_SetOCWarnThreshold
********************************************************************************
*
* Summary:
*  This API can be used to set the over current warning threshold for the
*  specified power converter.
*
* Parameters:
*  uint8 converterNum: 8-bit value indicating the converter number for which
                       over current warning threshold to be set.
*  float OCWarnThreshold: Over Current warning threshold in A.
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_SetOCWarnThreshold(uint8 converterNum, float ocWarnThreshold) \
     
{
    int32 ocWarnCounts;
    uint8 convRes = PowerMonitor_1_CYFALSE;

    ocWarnCounts = PowerMonitor_1_AToCounts(ocWarnThreshold, converterNum, &convRes);
    if(convRes == PowerMonitor_1_CYTRUE)
    {
        /* Disable the interrupt */
        PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
        PowerMonitor_1_warnWin[converterNum - 1u].OCWarnThrshldCounts = ocWarnCounts;
        PowerMonitor_1_warnWin[converterNum - 1u].OCWarnThrshldAmps = ocWarnThreshold;
        PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
    }
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetOCWarnThreshold
********************************************************************************
*
* Summary:
*  This API returns the over current warning theshold set for specified power
*  converter.
*
* Parameters:
*  uint8 converterNum: Converter number for which to get the over current
*                      warning threshold value. Valid range is 1-32.
*
* Return:
*  float: value indicating the over current warning threshold in A.
*
*******************************************************************************/
float PowerMonitor_1_GetOCWarnThreshold(uint8 converterNum) \
       
{
    float ocWarnThreshold = 0.0f;

    if ((converterNum > 0u) && (converterNum <= PowerMonitor_1_NUMBER_OF_CONVERTERS))
    {
        if(PowerMonitor_1_CurrentType[converterNum - 1u] != PowerMonitor_1_CURRENT_TYPE_NA)
        {
            ocWarnThreshold = PowerMonitor_1_warnWin[converterNum - 1u].OCWarnThrshldAmps;
        }
    }

    return (ocWarnThreshold);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_SetOCFaultThreshold
********************************************************************************
*
* Summary:
*  This API can be used to set the over current fault threshold for the
*  specified power converter.
*
* Parameters:
*  uint8 converterNum: 8-bit value indicating the converter number for which
                       over current fault threshold to be set.
*  float OCFaultThreshold: Over Current fault threshold in A.
*
* Return:
*  None
*
*******************************************************************************/
void PowerMonitor_1_SetOCFaultThreshold(uint8 converterNum, float ocFaultThreshold) \
     
{
    int32 ocFaultCounts;
    uint8 convRes = PowerMonitor_1_CYFALSE;

    ocFaultCounts = PowerMonitor_1_AToCounts(ocFaultThreshold, converterNum, &convRes);
    if(convRes == PowerMonitor_1_CYTRUE)
    {
        /* Disable the interrupt */
        PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
        PowerMonitor_1_faultWin[converterNum - 1u].OCFaultThrshldCounts = ocFaultCounts;
        PowerMonitor_1_faultWin[converterNum - 1u].OCFaultThrshldAmps = ocFaultThreshold;
        PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
    }
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetOCFaultThreshold
********************************************************************************
*
* Summary:
*  This API returns the over current fault theshold set for specified power
*  converter.
*
* Parameters:
*  uint8 converterNum: Converter number for which to get the over current
*                      fault threshold value. Valid range is 1-32.
*
* Return:
*  float: value indicating the over current fault threshold in A.
*
*******************************************************************************/
float PowerMonitor_1_GetOCFaultThreshold(uint8 converterNum) \
       
{
    float ocFaultThreshold = 0.0f;


    if ((converterNum > 0u) && (converterNum <= PowerMonitor_1_NUMBER_OF_CONVERTERS))
    {
        if(PowerMonitor_1_CurrentType[converterNum - 1u] != PowerMonitor_1_CURRENT_TYPE_NA)
        {
            ocFaultThreshold = PowerMonitor_1_faultWin[converterNum - 1u].OCFaultThrshldAmps;
        }
    }

    return (ocFaultThreshold);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetConverterVoltage
********************************************************************************
*
* Summary:
*  This API returns the power converter output voltage for the specified power
*  converter. If averaging is enabled, then value returned is the averaged
*  value.
*
* Parameters:
*  uint8 converterNum: Converter number for which to read the voltage. Valid
*                      Range is 1 - 32.
*
* Return:
*  uint16: Output voltage for the specified converter in mV Valid range
*          is 1 - 65535.
*
*******************************************************************************/
uint16 PowerMonitor_1_GetConverterVoltage(uint8 converterNum) \
       
{
    float retval = 0.0f;

    if ((converterNum <= PowerMonitor_1_NUMBER_OF_CONVERTERS) && (converterNum > 0u))
    {
        /* raw pin voltage, in mV   */
        retval  = PowerMonitor_1_GetMeasuredVal((converterNum - 1), PowerMonitor_1_VOLTAGE);
        retval /= PowerMonitor_1_VoltageScale[converterNum - 1];
        retval += 0.5f;                                      /* Ret rounded-off integer  */
    }
    if(retval < 0u)
    {
        retval = 0u;
    }
    return ((uint16)retval);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetConverterCurrent
********************************************************************************
*
* Summary:
*  This API returns the power converter load current for the specified power
*  converter. If averaging is enabled, then value returned is the averaged
*  value.
*
* Parameters:
*  uint8 converterNum: Converter number for which to read the current. Valid
*                      Range is 1 - 32.
*
* Return:
*  float: value indicating the output current for the specified converter in A.
*
*******************************************************************************/
float PowerMonitor_1_GetConverterCurrent(uint8 converterNum) \
       
{
    float retval = 0.0f;

    if ((converterNum <= PowerMonitor_1_NUMBER_OF_CONVERTERS) && (converterNum > 0u))
    {
        #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0)
            /* raw pin voltage, in mV   */
            retval  = PowerMonitor_1_GetMeasuredVal((converterNum - 1), PowerMonitor_1_CURRENT);
            /* Convert pin voltage to current */
            if (PowerMonitor_1_CurrentType[converterNum - 1] == PowerMonitor_1_CURRENT_TYPE_DIRECT)
            {
                retval /= PowerMonitor_1_RShunt[converterNum - 1];
            }
            else if (PowerMonitor_1_CurrentType[converterNum - 1] == PowerMonitor_1_CURRENT_TYPE_CSA)
            {
                retval /= (PowerMonitor_1_CSAGain[converterNum - 1] * PowerMonitor_1_RShunt[converterNum - 1]);
            }
            else
            {
                /* No action */
            }
        #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0 */
    }
    return (retval);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetAuxiliaryVoltage
********************************************************************************
*
* Summary:
*  This API returns the output voltage for one of the four auxiliary inputs.
*
* Parameters:
*  uint8 auxNum: Auxiliary input number. Valid range is 1 - 4.
*
* Return:
*  float value indicating the output voltage for one of the four
*  auxiliary inputs.
*
*******************************************************************************/
float PowerMonitor_1_GetAuxiliaryVoltage(uint8 auxNum) 
{
    float retval = 0.0f;

    if ((auxNum <= PowerMonitor_1_NUM_AUX_INPUTS) && (auxNum > 0u))
    {
        #if (PowerMonitor_1_NUM_AUX_INPUTS > 0)
            /* Disable the interrupt before fetching the raw ADC data */
            PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
            retval  = PowerMonitor_1_auxVoltCtl[auxNum - 1].filtVal;
            /* Re-enable the interrupt */
            PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);

            retval /= PowerMonitor_1_AUX_VOLTAGE_FILTER_SIZE;

            if ((PowerMonitor_1_AuxVoltageType[auxNum - 1] == PowerMonitor_1_AUX_VOLTAGE_64MV_DIFF) ||
                (PowerMonitor_1_AuxVoltageType[auxNum - 1] == PowerMonitor_1_AUX_VOLTAGE_128MV_DIFF))
            {
                /* Use config-2 correction value */
                retval *= PowerMonitor_1_adcGainCfg2;
            }
            else
            {
                /* Use config-1 or 4 correction value */
                if (PowerMonitor_1_AuxVoltageType[auxNum - 1] != PowerMonitor_1_AUX_VOLTAGE_SINGLE)
                {
                    retval *= PowerMonitor_1_adcGainCfg1;    /* Convert counts to 1mV units   */
                }
                else
                {
                    #if (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV)
                        retval *= PowerMonitor_1_adcGainCfg1;    /* Convert counts to 1mV units   */
                        retval += PowerMonitor_1_adcSeAdjCfg1;
                    #else
                        retval *= PowerMonitor_1_adcGainCfg3;    /* Convert counts to 1mV units   */
                        retval += PowerMonitor_1_adcSeAdjCfg3;
                    #endif /* (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV) */
                }
            }
            retval = retval / PowerMonitor_1_VOLTAGE_SCALE;
        #endif /* PowerMonitor_1_NUM_AUX_INPUTS > 0 */
    }
    return (retval);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_StartupISR
********************************************************************************
*
* Summary:
*  This is an internal API. This starts the ISR routine manually by setting the
*  interrupt pending register. This also sets the initial state for ADC to start
*  after software reset happens..
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
static void PowerMonitor_1_StartupIsr(void) 
{
     /* last voltage, current annd auxiliary voltage channels */
    PowerMonitor_1_lastVoltageChan = PowerMonitor_1_NUMBER_OF_CONVERTERS;
    PowerMonitor_1_lastCurrentChan = PowerMonitor_1_NUM_CURRENT_SOURCES + PowerMonitor_1_NUMBER_OF_CONVERTERS;
    PowerMonitor_1_lastAuxVoltChan = PowerMonitor_1_NUM_AUX_INPUTS + PowerMonitor_1_lastCurrentChan;
    PowerMonitor_1_iirInit = PowerMonitor_1_INITIALIZE_IIR_FILTER;

    /* Manually start the first ADC ISR */
    PowerMonitor_1_IsrStart();

    /* Start the calibration process */
    PowerMonitor_1_Calibrate();
    PowerMonitor_1_iirInit = PowerMonitor_1_NEXTSAMPLE_IIR_FILTER;
}

/*******************************************************************************
* Function Name: PowerMonitor_1_mVToCounts()
********************************************************************************
*
* Summary:
*  Convert mV to Counts for DelSig Config #1 +/- 2048 mV range or 
*                           DelSig Config #4 +/- 1024 mV
*
* Parameters:
*  mV = millivolt value to convert to counts
*  chan = channel id
*  convSuccess = pointer to conversion result returns: 1 - success, 0 - failed
*
* Return:
*  counts = DelSig raw counts corresponding to desired mV value
*
*******************************************************************************/
static int32 PowerMonitor_1_mVToCounts(uint16 mV, uint8 chan, uint8 * convSuccess)     \
                                                                
{
    float pinV = mV;
    float counts = 0.0f;

    *convSuccess = PowerMonitor_1_CYFALSE;
    if ((chan > 0u) && (chan <= PowerMonitor_1_NUMBER_OF_CONVERTERS))
    {
        pinV *= PowerMonitor_1_VoltageScale[chan - 1u];

        if (PowerMonitor_1_VoltageType[chan - 1] == PowerMonitor_1_VOLTAGE_TYPE_SINGLE)
        {
            #if (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV)
                if(pinV <= PowerMonitor_1_VOLTAGE_4096MV)
                {
                    *convSuccess = PowerMonitor_1_CYTRUE;
                    pinV -= PowerMonitor_1_adcSeAdjCfg1;
                    pinV /= PowerMonitor_1_adcGainCfg1;
                }
            #else
                if(pinV <= PowerMonitor_1_VOLTAGE_2048MV)
                {
                    *convSuccess = PowerMonitor_1_CYTRUE;
                    pinV -= PowerMonitor_1_adcSeAdjCfg3;
                    pinV /= PowerMonitor_1_adcGainCfg3;
                }
            #endif /* (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV) */
        }
        else
        {
            if(pinV <= PowerMonitor_1_VOLTAGE_2048MV)
            {
                *convSuccess = PowerMonitor_1_CYTRUE;
                pinV  /= PowerMonitor_1_adcGainCfg1;
            }
        }
        if(*convSuccess == PowerMonitor_1_CYTRUE)
        {
            counts = pinV * PowerMonitor_1_VOLTAGE_FILTER_SIZE + 0.5f;   /* runing total */
        }
    }
    return (int32)(counts);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_AToCounts()
********************************************************************************
*
* Summary:
*  Converts current to Counts for DelSig Config #1 +/- 2048 mV range
*  and Config #2 +/- 64mV range
*
* Parameters:
*  current = Amp value to convert to counts
*  chan = channel id
*  convSuccess = pointer to conversion result returns: 1 - success, 0 - failed
*
* Return:
*  counts = DelSig raw counts corresponding to desired A value
*
*******************************************************************************/
static int32 PowerMonitor_1_AToCounts(float current, uint8 chan, uint8 * convSuccess)  \
                                                                
{
    float pinV = current;
    float counts = 0.0f;

    *convSuccess = PowerMonitor_1_CYFALSE;
    if ((chan > 0u) && (chan <= PowerMonitor_1_NUMBER_OF_CONVERTERS))
    {
        if(PowerMonitor_1_CurrentType[chan - 1u] != PowerMonitor_1_CURRENT_TYPE_NA)
        {
            if (PowerMonitor_1_CurrentType[chan - 1u] == PowerMonitor_1_CURRENT_TYPE_CSA)
            {
                /* Convert current to mV */
                pinV *= (PowerMonitor_1_CSAGain[chan - 1u] * PowerMonitor_1_RShunt[chan - 1u]);
                #if (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV)
                    if(pinV <= PowerMonitor_1_VOLTAGE_4096MV)
                    {
                        *convSuccess = PowerMonitor_1_CYTRUE;
                        pinV -= PowerMonitor_1_adcSeAdjCfg1;     /* Apply PGA 2048 mV offset   */
                        pinV /= PowerMonitor_1_adcGainCfg1;      /* counts = mV / mV/count     */
                    }
                #else
                    if(pinV <= PowerMonitor_1_VOLTAGE_2048MV)
                    {
                        *convSuccess = PowerMonitor_1_CYTRUE;
                        pinV -= PowerMonitor_1_adcSeAdjCfg3;     /* Apply Ref 1024 mV offset   */
                        pinV /= PowerMonitor_1_adcGainCfg3;      /* counts = mV / mV/count     */
                    }
                #endif /* PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV */
            }
            else
            {
                /* Convert current to mV */
                pinV *= PowerMonitor_1_RShunt[chan - 1u];
                #if (PowerMonitor_1_DEFAULT_DIFF_CURRENT_RANGE == PowerMonitor_1_DIFF_CURRENT_RANGE_64MV)
                    if(pinV <= PowerMonitor_1_VOLTAGE_64MV)
                    {
                        *convSuccess = PowerMonitor_1_CYTRUE;
                        pinV /= PowerMonitor_1_adcGainCfg2;   /* divide by mV/count         */
                    }
                #else
                    if(pinV <= PowerMonitor_1_VOLTAGE_128MV)
                    {
                        *convSuccess = PowerMonitor_1_CYTRUE;
                        pinV /= PowerMonitor_1_adcGainCfg2;   /* divide by mV/count         */
                    }
                #endif /* PowerMonitor_1_DEFAULT_DIFF_CURRENT_RANGE == PowerMonitor_1_DIFF_CURRENT_RANGE_64MV */
            }
            if(*convSuccess == PowerMonitor_1_CYTRUE)
            {
                counts = pinV * PowerMonitor_1_CURRENT_FILTER_SIZE + 0.5f;   /* runing total */
            }
        }
    }
    return (int32)(counts);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetMeasuredVal()
********************************************************************************
*
* Summary:
*  Fetch specified channel measurement value.
*
* Parameters:
*  chan = channel number
*
* Return:
*  floating point are milli-volts
*
*******************************************************************************/
static float PowerMonitor_1_GetMeasuredVal(uint8 chan, uint8 chanType)
                                                            
{
    float retval = 0.0f;
    uint8 index;

    /* Get filtered ADC mV reading */
    if (chanType == PowerMonitor_1_CURRENT)
    {
        /* Check and select valid array index to fetch filtered raw current value */
        index = PowerMonitor_1_GetArrayIndex(chan);
        if (index != 0)
        {
            index = index - 1; /* zero based index for array */
            retval = (float)PowerMonitor_1_GetFiltRaw(index, chanType);
        }
        if (PowerMonitor_1_CurrentType[chan] == PowerMonitor_1_CURRENT_TYPE_CSA)
        {
            /* Convert counts to 1mV units   */
            #if (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV)
                retval *= PowerMonitor_1_adcGainCfg1;
                retval += PowerMonitor_1_adcSeAdjCfg1;
            #else
                retval *= PowerMonitor_1_adcGainCfg3;
                retval += PowerMonitor_1_adcSeAdjCfg3;
            #endif /* (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV) */
        }
        else if (PowerMonitor_1_CurrentType[chan] == PowerMonitor_1_CURRENT_TYPE_DIRECT)
        {
            /* Convert mV units  */
            retval *= PowerMonitor_1_adcGainCfg2;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        index = chan;
        retval = (float)PowerMonitor_1_GetFiltRaw(index, chanType);
        /* Convert counts to 1mV units   */
        if (PowerMonitor_1_VoltageType[chan] == PowerMonitor_1_VOLTAGE_TYPE_SINGLE)
        {
            #if (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV)
                retval *= PowerMonitor_1_adcGainCfg1;
                retval += PowerMonitor_1_adcSeAdjCfg1;   /* fix PGA 2048mV measure ref    */
            #else
                retval *= PowerMonitor_1_adcGainCfg3;
                retval += PowerMonitor_1_adcSeAdjCfg3;   /* fix Ref 1024mV measure ref    */
            #endif /* (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV) */
        }
        else
        {
            retval *= PowerMonitor_1_adcGainCfg1;
        }
    }
    return(retval);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetFiltRaw()
********************************************************************************
*
* Summary:
*  Fetch raw filtered ADC value in counts for specified channel.
*
* Parameters:
*  chan = channel number
*  chanType = Voltage or Current or Auxiliary channel
*
* Return:
*  floating point are milli-volts
*
*******************************************************************************/
static float PowerMonitor_1_GetFiltRaw(uint8 chan, uint8 chanType) 
{
    float retval = 0;

    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);
    if (chanType == PowerMonitor_1_VOLTAGE)
    {
        retval = (float)PowerMonitor_1_voltCtl[chan].filtVal;
    }
    #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0)
        else if (chanType == PowerMonitor_1_CURRENT)
        {
            retval = (float)PowerMonitor_1_ampCtl[chan].filtVal;
        }
        else
        {
            /* No action */
        }
    #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0) */

    PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);

    if (chanType == PowerMonitor_1_VOLTAGE)
    {
        retval /= PowerMonitor_1_VOLTAGE_FILTER_SIZE;
    }
    #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0)
        else
        {
            retval /= PowerMonitor_1_CURRENT_FILTER_SIZE;
        }
    #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0) */
    return (retval);
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetArrayIndex()
********************************************************************************
*
* Summary:
*  Fetchs the array index for Current raw value array.
*
* Parameters:
*  chan = channel number
*
* Return:
*  valid array index.
*
*******************************************************************************/
static uint8 PowerMonitor_1_GetArrayIndex(uint8 chan) 
{
    uint8 chanNum = 0u;
    uint8 i;
    for (i = 0u; i <= chan; i++)
    {
        if (PowerMonitor_1_CurrentType[i] != PowerMonitor_1_CURRENT_TYPE_NA)
        {
            chanNum++;
        }
    }
    return (chanNum);
}


#if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)


    /*******************************************************************************
    * Function Name: PowerMonitor_1_SetAuxiliarySampleMode()
    ********************************************************************************
    *
    * Summary:
    *  Sets the ADC sample mode for the selected auxiliary input.
    *  Note: all auxiliary inputs are set to continuous sampling mode by default.
    *
    * Parameters:
    *  auxNum: Auxiliary input number. Valid range is 1 - 4.
    *  sampleMode: Specifies the sample mode
    *              0 - Continuous, 1 - On Demand.
    *
    * Return:
    *  None
    *
    * Side Effect:
    *  When On Demand sampling is selected, auxiliary input filtering is disabled
    *  Changing the auxiliary input sample mode impacts the overall sample rate
    *  for the power converters
    *
    *******************************************************************************/
    void PowerMonitor_1_SetAuxiliarySampleMode(uint8 auxNum, uint8 sampleMode)
                                            
    {
        uint8 auxMask;

        if ((auxNum > 0u) && (auxNum <= PowerMonitor_1_NUM_AUX_INPUTS))
        {
            auxMask = (uint8)(0x01u << (auxNum - 1u));

            /* Change auxiliary channels enable mask if it is required */
            if( ((sampleMode == PowerMonitor_1_CONTINUOUS) && ((PowerMonitor_1_auxEnableMask & auxMask) == 0u)) ||
                ((sampleMode != PowerMonitor_1_CONTINUOUS) && ((PowerMonitor_1_auxEnableMask & auxMask) != 0u)) )
            {
                /* Disable interrupt and reconfigure number of channels */
                PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);

                if(sampleMode == PowerMonitor_1_CONTINUOUS)
                {
                    PowerMonitor_1_auxEnableMask |= auxMask;
                    /* Update total channels number */
                    PowerMonitor_1_maxChannels++;
                }
                else /* PowerMonitor_1_ON_DEMAND */
                {
                    PowerMonitor_1_auxEnableMask &= (uint8)~auxMask;
                    /* Update total channels number */
                    PowerMonitor_1_maxChannels--;
                }
                /* Enable interrupt */
                PowerMonitor_1_IntEnable(PowerMonitor_1_IRQ__INTC_NUMBER);
            }
        }
        else
        {
            /* Halt CPU in debug mode if auxNum is out of valid range */
            CYASSERT(0u != 0u);
        }
    }


    /*******************************************************************************
    * Function Name: PowerMonitor_1_GetAuxiliarySampleMode()
    ********************************************************************************
    *
    * Summary:
    *  Returns the ADC sample mode for the selected auxiliary input.
    *
    * Parameters:
    *  auxNum: Auxiliary input number. Valid range is 1 - 4.
    *
    * Return:
    *  Sample mode: 0 - Continuous, 1 - On Demand.
    *  None
    *
    *******************************************************************************/
    uint8 PowerMonitor_1_GetAuxiliarySampleMode(uint8 auxNum)
                                            
    {
        uint8 auxMask;
        uint8 status = PowerMonitor_1_ON_DEMAND;

        if ((auxNum > 0u) && (auxNum <= PowerMonitor_1_NUM_AUX_INPUTS))
        {
            auxMask = (uint8)(0x01u << (auxNum - 1u));

            if((PowerMonitor_1_auxEnableMask & auxMask) != 0u)
            {
                status = PowerMonitor_1_CONTINUOUS;
            }
        }
        else
        {
            /* Halt CPU in debug mode if auxNum is out of valid range */
            CYASSERT(0u != 0u);
        }

        return (status);
    }


    /*******************************************************************************
    * Function Name: PowerMonitor_1_RequestAuxiliarySample()
    ********************************************************************************
    *
    * Summary:
    *  Requests and returns a single unfiltered on-demand sample result of the 
    *  specified auxiliary input. Calling this API will cause the normal ADC 
    *  conversion sequence to be interrupted in order to obtain the requested sample
    *  as soon as possible. The API may also be called when the auxiliary input 
    *  sample mode is set to continuous. It doesn't affect on continuous auxiliary
    *  measurements. 
    *
    * Parameters:
    *  auxNum: Auxiliary input number. Valid range is 1 - 4.
    *
    * Return:
    *  float value indicating the unfiltered output voltage for one of the four
    *  auxiliary inputs.
    *
    * Side Effect:
    *  Calling this API impacts the overall sample rate for the power converters
    *
    *******************************************************************************/
    float PowerMonitor_1_RequestAuxiliarySample(uint8 auxNum)
                                            
    {
        float retval = 0.0f;

        if ((auxNum > 0u) && (auxNum <= PowerMonitor_1_NUM_AUX_INPUTS))
        {
            /* Save which sample was next in sequence before calibration interruption */
            PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);

            PowerMonitor_1_adcConvNextPreCal = PowerMonitor_1_adcConvNext;
            PowerMonitor_1_adcConvNext       = PowerMonitor_1_STATE_MEASURE_AUX;
            PowerMonitor_1_auxIndex          = auxNum - 1;

            PowerMonitor_1_IntEnable (PowerMonitor_1_IRQ__INTC_NUMBER);

            /***************************************************************************
            * WAIT FOR ADC ISR TO FINISH GETTING RAW VALUE.
            * (ADC has cleared calibration process and resumed normal sampling
            ***************************************************************************/
            while ((PowerMonitor_1_adcConvNext == PowerMonitor_1_STATE_MEASURE_AUX) ||   /* wait start */
                   (PowerMonitor_1_adcConvNow  == PowerMonitor_1_STATE_MEASURE_AUX));    /* wait finish*/
            /* read mesured value */
            retval  = (float)PowerMonitor_1_auxStore;

            if ((PowerMonitor_1_AuxVoltageType[auxNum - 1] == PowerMonitor_1_AUX_VOLTAGE_64MV_DIFF) ||
                (PowerMonitor_1_AuxVoltageType[auxNum - 1] == PowerMonitor_1_AUX_VOLTAGE_128MV_DIFF))
            {
                /* Use config-2 correction value */
                retval *= PowerMonitor_1_adcGainCfg2;
            }
            else
            {
                /* Use config-1 or 4 correction value */
                if (PowerMonitor_1_AuxVoltageType[auxNum - 1] != PowerMonitor_1_AUX_VOLTAGE_SINGLE)
                {
                    retval *= PowerMonitor_1_adcGainCfg1;    /* Convert counts to 1mV units   */
                }
                else
                {
                    #if (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV)
                        retval *= PowerMonitor_1_adcGainCfg1;    /* Convert counts to 1mV units   */
                        retval += PowerMonitor_1_adcSeAdjCfg1;
                    #else
                        retval *= PowerMonitor_1_adcGainCfg3;    /* Convert counts to 1mV units   */
                        retval += PowerMonitor_1_adcSeAdjCfg3;
                    #endif /* (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV) */
                }
            }
            retval = retval / PowerMonitor_1_VOLTAGE_SCALE;
        }
        else
        {
            /* Halt CPU in debug mode if auxNum is out of valid range */
            CYASSERT(0u != 0u);
        }
        return (retval);
    }


#endif /* PowerMonitor_1_NUM_AUX_INPUTS > 0 */

/* [] END OF FILE */
