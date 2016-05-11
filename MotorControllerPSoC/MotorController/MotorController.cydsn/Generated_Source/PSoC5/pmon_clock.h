/*******************************************************************************
* File Name: pmon_clock.h
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

#if !defined(CY_CLOCK_pmon_clock_H)
#define CY_CLOCK_pmon_clock_H

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

void pmon_clock_Start(void) ;
void pmon_clock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void pmon_clock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void pmon_clock_StandbyPower(uint8 state) ;
void pmon_clock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 pmon_clock_GetDividerRegister(void) ;
void pmon_clock_SetModeRegister(uint8 modeBitMask) ;
void pmon_clock_ClearModeRegister(uint8 modeBitMask) ;
uint8 pmon_clock_GetModeRegister(void) ;
void pmon_clock_SetSourceRegister(uint8 clkSource) ;
uint8 pmon_clock_GetSourceRegister(void) ;
#if defined(pmon_clock__CFG3)
void pmon_clock_SetPhaseRegister(uint8 clkPhase) ;
uint8 pmon_clock_GetPhaseRegister(void) ;
#endif /* defined(pmon_clock__CFG3) */

#define pmon_clock_Enable()                       pmon_clock_Start()
#define pmon_clock_Disable()                      pmon_clock_Stop()
#define pmon_clock_SetDivider(clkDivider)         pmon_clock_SetDividerRegister(clkDivider, 1u)
#define pmon_clock_SetDividerValue(clkDivider)    pmon_clock_SetDividerRegister((clkDivider) - 1u, 1u)
#define pmon_clock_SetMode(clkMode)               pmon_clock_SetModeRegister(clkMode)
#define pmon_clock_SetSource(clkSource)           pmon_clock_SetSourceRegister(clkSource)
#if defined(pmon_clock__CFG3)
#define pmon_clock_SetPhase(clkPhase)             pmon_clock_SetPhaseRegister(clkPhase)
#define pmon_clock_SetPhaseValue(clkPhase)        pmon_clock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(pmon_clock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define pmon_clock_CLKEN              (* (reg8 *) pmon_clock__PM_ACT_CFG)
#define pmon_clock_CLKEN_PTR          ((reg8 *) pmon_clock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define pmon_clock_CLKSTBY            (* (reg8 *) pmon_clock__PM_STBY_CFG)
#define pmon_clock_CLKSTBY_PTR        ((reg8 *) pmon_clock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define pmon_clock_DIV_LSB            (* (reg8 *) pmon_clock__CFG0)
#define pmon_clock_DIV_LSB_PTR        ((reg8 *) pmon_clock__CFG0)
#define pmon_clock_DIV_PTR            ((reg16 *) pmon_clock__CFG0)

/* Clock MSB divider configuration register. */
#define pmon_clock_DIV_MSB            (* (reg8 *) pmon_clock__CFG1)
#define pmon_clock_DIV_MSB_PTR        ((reg8 *) pmon_clock__CFG1)

/* Mode and source configuration register */
#define pmon_clock_MOD_SRC            (* (reg8 *) pmon_clock__CFG2)
#define pmon_clock_MOD_SRC_PTR        ((reg8 *) pmon_clock__CFG2)

#if defined(pmon_clock__CFG3)
/* Analog clock phase configuration register */
#define pmon_clock_PHASE              (* (reg8 *) pmon_clock__CFG3)
#define pmon_clock_PHASE_PTR          ((reg8 *) pmon_clock__CFG3)
#endif /* defined(pmon_clock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define pmon_clock_CLKEN_MASK         pmon_clock__PM_ACT_MSK
#define pmon_clock_CLKSTBY_MASK       pmon_clock__PM_STBY_MSK

/* CFG2 field masks */
#define pmon_clock_SRC_SEL_MSK        pmon_clock__CFG2_SRC_SEL_MASK
#define pmon_clock_MODE_MASK          (~(pmon_clock_SRC_SEL_MSK))

#if defined(pmon_clock__CFG3)
/* CFG3 phase mask */
#define pmon_clock_PHASE_MASK         pmon_clock__CFG3_PHASE_DLY_MASK
#endif /* defined(pmon_clock__CFG3) */

#endif /* CY_CLOCK_pmon_clock_H */


/* [] END OF FILE */
