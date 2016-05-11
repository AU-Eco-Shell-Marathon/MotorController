/*******************************************************************************
* File Name: Print.h  
* Version 2.0
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Print_H) /* Pins Print_H */
#define CY_PINS_Print_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Print_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Print__PORT == 15 && ((Print__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Print_Write(uint8 value) ;
void    Print_SetDriveMode(uint8 mode) ;
uint8   Print_ReadDataReg(void) ;
uint8   Print_Read(void) ;
uint8   Print_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Print_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Print_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Print_DM_RES_UP          PIN_DM_RES_UP
#define Print_DM_RES_DWN         PIN_DM_RES_DWN
#define Print_DM_OD_LO           PIN_DM_OD_LO
#define Print_DM_OD_HI           PIN_DM_OD_HI
#define Print_DM_STRONG          PIN_DM_STRONG
#define Print_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Print_MASK               Print__MASK
#define Print_SHIFT              Print__SHIFT
#define Print_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Print_PS                     (* (reg8 *) Print__PS)
/* Data Register */
#define Print_DR                     (* (reg8 *) Print__DR)
/* Port Number */
#define Print_PRT_NUM                (* (reg8 *) Print__PRT) 
/* Connect to Analog Globals */                                                  
#define Print_AG                     (* (reg8 *) Print__AG)                       
/* Analog MUX bux enable */
#define Print_AMUX                   (* (reg8 *) Print__AMUX) 
/* Bidirectional Enable */                                                        
#define Print_BIE                    (* (reg8 *) Print__BIE)
/* Bit-mask for Aliased Register Access */
#define Print_BIT_MASK               (* (reg8 *) Print__BIT_MASK)
/* Bypass Enable */
#define Print_BYP                    (* (reg8 *) Print__BYP)
/* Port wide control signals */                                                   
#define Print_CTL                    (* (reg8 *) Print__CTL)
/* Drive Modes */
#define Print_DM0                    (* (reg8 *) Print__DM0) 
#define Print_DM1                    (* (reg8 *) Print__DM1)
#define Print_DM2                    (* (reg8 *) Print__DM2) 
/* Input Buffer Disable Override */
#define Print_INP_DIS                (* (reg8 *) Print__INP_DIS)
/* LCD Common or Segment Drive */
#define Print_LCD_COM_SEG            (* (reg8 *) Print__LCD_COM_SEG)
/* Enable Segment LCD */
#define Print_LCD_EN                 (* (reg8 *) Print__LCD_EN)
/* Slew Rate Control */
#define Print_SLW                    (* (reg8 *) Print__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Print_PRTDSI__CAPS_SEL       (* (reg8 *) Print__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Print_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Print__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Print_PRTDSI__OE_SEL0        (* (reg8 *) Print__PRTDSI__OE_SEL0) 
#define Print_PRTDSI__OE_SEL1        (* (reg8 *) Print__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Print_PRTDSI__OUT_SEL0       (* (reg8 *) Print__PRTDSI__OUT_SEL0) 
#define Print_PRTDSI__OUT_SEL1       (* (reg8 *) Print__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Print_PRTDSI__SYNC_OUT       (* (reg8 *) Print__PRTDSI__SYNC_OUT) 


#if defined(Print__INTSTAT)  /* Interrupt Registers */

    #define Print_INTSTAT                (* (reg8 *) Print__INTSTAT)
    #define Print_SNAP                   (* (reg8 *) Print__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Print_H */


/* [] END OF FILE */
