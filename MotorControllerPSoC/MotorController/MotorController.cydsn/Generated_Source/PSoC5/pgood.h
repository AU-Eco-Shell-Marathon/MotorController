/*******************************************************************************
* File Name: pgood.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_pgood_H) /* Pins pgood_H */
#define CY_PINS_pgood_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "pgood_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 pgood__PORT == 15 && ((pgood__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    pgood_Write(uint8 value);
void    pgood_SetDriveMode(uint8 mode);
uint8   pgood_ReadDataReg(void);
uint8   pgood_Read(void);
void    pgood_SetInterruptMode(uint16 position, uint16 mode);
uint8   pgood_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the pgood_SetDriveMode() function.
     *  @{
     */
        #define pgood_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define pgood_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define pgood_DM_RES_UP          PIN_DM_RES_UP
        #define pgood_DM_RES_DWN         PIN_DM_RES_DWN
        #define pgood_DM_OD_LO           PIN_DM_OD_LO
        #define pgood_DM_OD_HI           PIN_DM_OD_HI
        #define pgood_DM_STRONG          PIN_DM_STRONG
        #define pgood_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define pgood_MASK               pgood__MASK
#define pgood_SHIFT              pgood__SHIFT
#define pgood_WIDTH              1u

/* Interrupt constants */
#if defined(pgood__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in pgood_SetInterruptMode() function.
     *  @{
     */
        #define pgood_INTR_NONE      (uint16)(0x0000u)
        #define pgood_INTR_RISING    (uint16)(0x0001u)
        #define pgood_INTR_FALLING   (uint16)(0x0002u)
        #define pgood_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define pgood_INTR_MASK      (0x01u) 
#endif /* (pgood__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define pgood_PS                     (* (reg8 *) pgood__PS)
/* Data Register */
#define pgood_DR                     (* (reg8 *) pgood__DR)
/* Port Number */
#define pgood_PRT_NUM                (* (reg8 *) pgood__PRT) 
/* Connect to Analog Globals */                                                  
#define pgood_AG                     (* (reg8 *) pgood__AG)                       
/* Analog MUX bux enable */
#define pgood_AMUX                   (* (reg8 *) pgood__AMUX) 
/* Bidirectional Enable */                                                        
#define pgood_BIE                    (* (reg8 *) pgood__BIE)
/* Bit-mask for Aliased Register Access */
#define pgood_BIT_MASK               (* (reg8 *) pgood__BIT_MASK)
/* Bypass Enable */
#define pgood_BYP                    (* (reg8 *) pgood__BYP)
/* Port wide control signals */                                                   
#define pgood_CTL                    (* (reg8 *) pgood__CTL)
/* Drive Modes */
#define pgood_DM0                    (* (reg8 *) pgood__DM0) 
#define pgood_DM1                    (* (reg8 *) pgood__DM1)
#define pgood_DM2                    (* (reg8 *) pgood__DM2) 
/* Input Buffer Disable Override */
#define pgood_INP_DIS                (* (reg8 *) pgood__INP_DIS)
/* LCD Common or Segment Drive */
#define pgood_LCD_COM_SEG            (* (reg8 *) pgood__LCD_COM_SEG)
/* Enable Segment LCD */
#define pgood_LCD_EN                 (* (reg8 *) pgood__LCD_EN)
/* Slew Rate Control */
#define pgood_SLW                    (* (reg8 *) pgood__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define pgood_PRTDSI__CAPS_SEL       (* (reg8 *) pgood__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define pgood_PRTDSI__DBL_SYNC_IN    (* (reg8 *) pgood__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define pgood_PRTDSI__OE_SEL0        (* (reg8 *) pgood__PRTDSI__OE_SEL0) 
#define pgood_PRTDSI__OE_SEL1        (* (reg8 *) pgood__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define pgood_PRTDSI__OUT_SEL0       (* (reg8 *) pgood__PRTDSI__OUT_SEL0) 
#define pgood_PRTDSI__OUT_SEL1       (* (reg8 *) pgood__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define pgood_PRTDSI__SYNC_OUT       (* (reg8 *) pgood__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(pgood__SIO_CFG)
    #define pgood_SIO_HYST_EN        (* (reg8 *) pgood__SIO_HYST_EN)
    #define pgood_SIO_REG_HIFREQ     (* (reg8 *) pgood__SIO_REG_HIFREQ)
    #define pgood_SIO_CFG            (* (reg8 *) pgood__SIO_CFG)
    #define pgood_SIO_DIFF           (* (reg8 *) pgood__SIO_DIFF)
#endif /* (pgood__SIO_CFG) */

/* Interrupt Registers */
#if defined(pgood__INTSTAT)
    #define pgood_INTSTAT            (* (reg8 *) pgood__INTSTAT)
    #define pgood_SNAP               (* (reg8 *) pgood__SNAP)
    
	#define pgood_0_INTTYPE_REG 		(* (reg8 *) pgood__0__INTTYPE)
#endif /* (pgood__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_pgood_H */


/* [] END OF FILE */
