/*******************************************************************************
* File Name: PowerMonitor_1_ADC_Ext_CP_Clk.h
* Version 1.70
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_CLOCK_PowerMonitor_1_ADC_Ext_CP_Clk_H)
#define CY_CLOCK_PowerMonitor_1_ADC_Ext_CP_Clk_H

#include <cytypes.h>
#include <cyfitter.h>

/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v1_70 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

/***************************************
*        Function Prototypes
***************************************/

void PowerMonitor_1_ADC_Ext_CP_Clk_Start(void) ;
void PowerMonitor_1_ADC_Ext_CP_Clk_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void PowerMonitor_1_ADC_Ext_CP_Clk_StopBlock(void) ;
#endif

void PowerMonitor_1_ADC_Ext_CP_Clk_StandbyPower(uint8 state) ;
void PowerMonitor_1_ADC_Ext_CP_Clk_SetDividerRegister(uint16 clkDivider, uint8 reset) ;
uint16 PowerMonitor_1_ADC_Ext_CP_Clk_GetDividerRegister(void) ;
void PowerMonitor_1_ADC_Ext_CP_Clk_SetModeRegister(uint8 modeBitMask) ;
void PowerMonitor_1_ADC_Ext_CP_Clk_ClearModeRegister(uint8 modeBitMask) ;
uint8 PowerMonitor_1_ADC_Ext_CP_Clk_GetModeRegister(void) ;
void PowerMonitor_1_ADC_Ext_CP_Clk_SetSourceRegister(uint8 clkSource) ;
uint8 PowerMonitor_1_ADC_Ext_CP_Clk_GetSourceRegister(void) ;
#if defined(PowerMonitor_1_ADC_Ext_CP_Clk__CFG3)
void PowerMonitor_1_ADC_Ext_CP_Clk_SetPhaseRegister(uint8 clkPhase) ;
uint8 PowerMonitor_1_ADC_Ext_CP_Clk_GetPhaseRegister(void) ;
#endif

#define PowerMonitor_1_ADC_Ext_CP_Clk_Enable()                       PowerMonitor_1_ADC_Ext_CP_Clk_Start()
#define PowerMonitor_1_ADC_Ext_CP_Clk_Disable()                      PowerMonitor_1_ADC_Ext_CP_Clk_Stop()
#define PowerMonitor_1_ADC_Ext_CP_Clk_SetDivider(clkDivider)         PowerMonitor_1_ADC_Ext_CP_Clk_SetDividerRegister(clkDivider, 1)
#define PowerMonitor_1_ADC_Ext_CP_Clk_SetDividerValue(clkDivider)    PowerMonitor_1_ADC_Ext_CP_Clk_SetDividerRegister((clkDivider) - 1, 1)
#define PowerMonitor_1_ADC_Ext_CP_Clk_SetMode(clkMode)               PowerMonitor_1_ADC_Ext_CP_Clk_SetModeRegister(clkMode)
#define PowerMonitor_1_ADC_Ext_CP_Clk_SetSource(clkSource)           PowerMonitor_1_ADC_Ext_CP_Clk_SetSourceRegister(clkSource)
#if defined(PowerMonitor_1_ADC_Ext_CP_Clk__CFG3)
#define PowerMonitor_1_ADC_Ext_CP_Clk_SetPhase(clkPhase)             PowerMonitor_1_ADC_Ext_CP_Clk_SetPhaseRegister(clkPhase)
#define PowerMonitor_1_ADC_Ext_CP_Clk_SetPhaseValue(clkPhase)        PowerMonitor_1_ADC_Ext_CP_Clk_SetPhaseRegister((clkPhase) + 1)
#endif


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define PowerMonitor_1_ADC_Ext_CP_Clk_CLKEN              (* (reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__PM_ACT_CFG)
#define PowerMonitor_1_ADC_Ext_CP_Clk_CLKEN_PTR          ((reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define PowerMonitor_1_ADC_Ext_CP_Clk_CLKSTBY            (* (reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__PM_STBY_CFG)
#define PowerMonitor_1_ADC_Ext_CP_Clk_CLKSTBY_PTR        ((reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define PowerMonitor_1_ADC_Ext_CP_Clk_DIV_LSB            (* (reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__CFG0)
#define PowerMonitor_1_ADC_Ext_CP_Clk_DIV_LSB_PTR        ((reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__CFG0)
#define PowerMonitor_1_ADC_Ext_CP_Clk_DIV_PTR            ((reg16 *) PowerMonitor_1_ADC_Ext_CP_Clk__CFG0)

/* Clock MSB divider configuration register. */
#define PowerMonitor_1_ADC_Ext_CP_Clk_DIV_MSB            (* (reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__CFG1)
#define PowerMonitor_1_ADC_Ext_CP_Clk_DIV_MSB_PTR        ((reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__CFG1)

/* Mode and source configuration register */
#define PowerMonitor_1_ADC_Ext_CP_Clk_MOD_SRC            (* (reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__CFG2)
#define PowerMonitor_1_ADC_Ext_CP_Clk_MOD_SRC_PTR        ((reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__CFG2)

#if defined(PowerMonitor_1_ADC_Ext_CP_Clk__CFG3)
/* Analog clock phase configuration register */
#define PowerMonitor_1_ADC_Ext_CP_Clk_PHASE              (* (reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__CFG3)
#define PowerMonitor_1_ADC_Ext_CP_Clk_PHASE_PTR          ((reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__CFG3)
#endif


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define PowerMonitor_1_ADC_Ext_CP_Clk_CLKEN_MASK         PowerMonitor_1_ADC_Ext_CP_Clk__PM_ACT_MSK
#define PowerMonitor_1_ADC_Ext_CP_Clk_CLKSTBY_MASK       PowerMonitor_1_ADC_Ext_CP_Clk__PM_STBY_MSK

/* CFG2 field masks */
#define PowerMonitor_1_ADC_Ext_CP_Clk_SRC_SEL_MSK        PowerMonitor_1_ADC_Ext_CP_Clk__CFG2_SRC_SEL_MASK
#define PowerMonitor_1_ADC_Ext_CP_Clk_MODE_MASK          (~(PowerMonitor_1_ADC_Ext_CP_Clk_SRC_SEL_MSK))

#if defined(PowerMonitor_1_ADC_Ext_CP_Clk__CFG3)
/* CFG3 phase mask */
#define PowerMonitor_1_ADC_Ext_CP_Clk_PHASE_MASK         PowerMonitor_1_ADC_Ext_CP_Clk__CFG3_PHASE_DLY_MASK
#endif

#endif /* CY_CLOCK_PowerMonitor_1_ADC_Ext_CP_Clk_H */


/* [] END OF FILE */
