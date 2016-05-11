/*******************************************************************************
* File Name: clock_Speedometer.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_clock_Speedometer_H)
#define CY_CLOCK_clock_Speedometer_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void clock_Speedometer_Start(void) ;
void clock_Speedometer_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void clock_Speedometer_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void clock_Speedometer_StandbyPower(uint8 state) ;
void clock_Speedometer_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 clock_Speedometer_GetDividerRegister(void) ;
void clock_Speedometer_SetModeRegister(uint8 modeBitMask) ;
void clock_Speedometer_ClearModeRegister(uint8 modeBitMask) ;
uint8 clock_Speedometer_GetModeRegister(void) ;
void clock_Speedometer_SetSourceRegister(uint8 clkSource) ;
uint8 clock_Speedometer_GetSourceRegister(void) ;
#if defined(clock_Speedometer__CFG3)
void clock_Speedometer_SetPhaseRegister(uint8 clkPhase) ;
uint8 clock_Speedometer_GetPhaseRegister(void) ;
#endif /* defined(clock_Speedometer__CFG3) */

#define clock_Speedometer_Enable()                       clock_Speedometer_Start()
#define clock_Speedometer_Disable()                      clock_Speedometer_Stop()
#define clock_Speedometer_SetDivider(clkDivider)         clock_Speedometer_SetDividerRegister(clkDivider, 1u)
#define clock_Speedometer_SetDividerValue(clkDivider)    clock_Speedometer_SetDividerRegister((clkDivider) - 1u, 1u)
#define clock_Speedometer_SetMode(clkMode)               clock_Speedometer_SetModeRegister(clkMode)
#define clock_Speedometer_SetSource(clkSource)           clock_Speedometer_SetSourceRegister(clkSource)
#if defined(clock_Speedometer__CFG3)
#define clock_Speedometer_SetPhase(clkPhase)             clock_Speedometer_SetPhaseRegister(clkPhase)
#define clock_Speedometer_SetPhaseValue(clkPhase)        clock_Speedometer_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(clock_Speedometer__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define clock_Speedometer_CLKEN              (* (reg8 *) clock_Speedometer__PM_ACT_CFG)
#define clock_Speedometer_CLKEN_PTR          ((reg8 *) clock_Speedometer__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define clock_Speedometer_CLKSTBY            (* (reg8 *) clock_Speedometer__PM_STBY_CFG)
#define clock_Speedometer_CLKSTBY_PTR        ((reg8 *) clock_Speedometer__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define clock_Speedometer_DIV_LSB            (* (reg8 *) clock_Speedometer__CFG0)
#define clock_Speedometer_DIV_LSB_PTR        ((reg8 *) clock_Speedometer__CFG0)
#define clock_Speedometer_DIV_PTR            ((reg16 *) clock_Speedometer__CFG0)

/* Clock MSB divider configuration register. */
#define clock_Speedometer_DIV_MSB            (* (reg8 *) clock_Speedometer__CFG1)
#define clock_Speedometer_DIV_MSB_PTR        ((reg8 *) clock_Speedometer__CFG1)

/* Mode and source configuration register */
#define clock_Speedometer_MOD_SRC            (* (reg8 *) clock_Speedometer__CFG2)
#define clock_Speedometer_MOD_SRC_PTR        ((reg8 *) clock_Speedometer__CFG2)

#if defined(clock_Speedometer__CFG3)
/* Analog clock phase configuration register */
#define clock_Speedometer_PHASE              (* (reg8 *) clock_Speedometer__CFG3)
#define clock_Speedometer_PHASE_PTR          ((reg8 *) clock_Speedometer__CFG3)
#endif /* defined(clock_Speedometer__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define clock_Speedometer_CLKEN_MASK         clock_Speedometer__PM_ACT_MSK
#define clock_Speedometer_CLKSTBY_MASK       clock_Speedometer__PM_STBY_MSK

/* CFG2 field masks */
#define clock_Speedometer_SRC_SEL_MSK        clock_Speedometer__CFG2_SRC_SEL_MASK
#define clock_Speedometer_MODE_MASK          (~(clock_Speedometer_SRC_SEL_MSK))

#if defined(clock_Speedometer__CFG3)
/* CFG3 phase mask */
#define clock_Speedometer_PHASE_MASK         clock_Speedometer__CFG3_PHASE_DLY_MASK
#endif /* defined(clock_Speedometer__CFG3) */

#endif /* CY_CLOCK_clock_Speedometer_H */


/* [] END OF FILE */
