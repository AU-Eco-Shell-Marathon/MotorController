/*******************************************************************************
* File Name: Padel_in.h  
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

#if !defined(CY_PINS_Padel_in_H) /* Pins Padel_in_H */
#define CY_PINS_Padel_in_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Padel_in_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Padel_in__PORT == 15 && ((Padel_in__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Padel_in_Write(uint8 value);
void    Padel_in_SetDriveMode(uint8 mode);
uint8   Padel_in_ReadDataReg(void);
uint8   Padel_in_Read(void);
void    Padel_in_SetInterruptMode(uint16 position, uint16 mode);
uint8   Padel_in_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Padel_in_SetDriveMode() function.
     *  @{
     */
        #define Padel_in_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Padel_in_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Padel_in_DM_RES_UP          PIN_DM_RES_UP
        #define Padel_in_DM_RES_DWN         PIN_DM_RES_DWN
        #define Padel_in_DM_OD_LO           PIN_DM_OD_LO
        #define Padel_in_DM_OD_HI           PIN_DM_OD_HI
        #define Padel_in_DM_STRONG          PIN_DM_STRONG
        #define Padel_in_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Padel_in_MASK               Padel_in__MASK
#define Padel_in_SHIFT              Padel_in__SHIFT
#define Padel_in_WIDTH              1u

/* Interrupt constants */
#if defined(Padel_in__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Padel_in_SetInterruptMode() function.
     *  @{
     */
        #define Padel_in_INTR_NONE      (uint16)(0x0000u)
        #define Padel_in_INTR_RISING    (uint16)(0x0001u)
        #define Padel_in_INTR_FALLING   (uint16)(0x0002u)
        #define Padel_in_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Padel_in_INTR_MASK      (0x01u) 
#endif /* (Padel_in__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Padel_in_PS                     (* (reg8 *) Padel_in__PS)
/* Data Register */
#define Padel_in_DR                     (* (reg8 *) Padel_in__DR)
/* Port Number */
#define Padel_in_PRT_NUM                (* (reg8 *) Padel_in__PRT) 
/* Connect to Analog Globals */                                                  
#define Padel_in_AG                     (* (reg8 *) Padel_in__AG)                       
/* Analog MUX bux enable */
#define Padel_in_AMUX                   (* (reg8 *) Padel_in__AMUX) 
/* Bidirectional Enable */                                                        
#define Padel_in_BIE                    (* (reg8 *) Padel_in__BIE)
/* Bit-mask for Aliased Register Access */
#define Padel_in_BIT_MASK               (* (reg8 *) Padel_in__BIT_MASK)
/* Bypass Enable */
#define Padel_in_BYP                    (* (reg8 *) Padel_in__BYP)
/* Port wide control signals */                                                   
#define Padel_in_CTL                    (* (reg8 *) Padel_in__CTL)
/* Drive Modes */
#define Padel_in_DM0                    (* (reg8 *) Padel_in__DM0) 
#define Padel_in_DM1                    (* (reg8 *) Padel_in__DM1)
#define Padel_in_DM2                    (* (reg8 *) Padel_in__DM2) 
/* Input Buffer Disable Override */
#define Padel_in_INP_DIS                (* (reg8 *) Padel_in__INP_DIS)
/* LCD Common or Segment Drive */
#define Padel_in_LCD_COM_SEG            (* (reg8 *) Padel_in__LCD_COM_SEG)
/* Enable Segment LCD */
#define Padel_in_LCD_EN                 (* (reg8 *) Padel_in__LCD_EN)
/* Slew Rate Control */
#define Padel_in_SLW                    (* (reg8 *) Padel_in__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Padel_in_PRTDSI__CAPS_SEL       (* (reg8 *) Padel_in__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Padel_in_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Padel_in__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Padel_in_PRTDSI__OE_SEL0        (* (reg8 *) Padel_in__PRTDSI__OE_SEL0) 
#define Padel_in_PRTDSI__OE_SEL1        (* (reg8 *) Padel_in__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Padel_in_PRTDSI__OUT_SEL0       (* (reg8 *) Padel_in__PRTDSI__OUT_SEL0) 
#define Padel_in_PRTDSI__OUT_SEL1       (* (reg8 *) Padel_in__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Padel_in_PRTDSI__SYNC_OUT       (* (reg8 *) Padel_in__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Padel_in__SIO_CFG)
    #define Padel_in_SIO_HYST_EN        (* (reg8 *) Padel_in__SIO_HYST_EN)
    #define Padel_in_SIO_REG_HIFREQ     (* (reg8 *) Padel_in__SIO_REG_HIFREQ)
    #define Padel_in_SIO_CFG            (* (reg8 *) Padel_in__SIO_CFG)
    #define Padel_in_SIO_DIFF           (* (reg8 *) Padel_in__SIO_DIFF)
#endif /* (Padel_in__SIO_CFG) */

/* Interrupt Registers */
#if defined(Padel_in__INTSTAT)
    #define Padel_in_INTSTAT            (* (reg8 *) Padel_in__INTSTAT)
    #define Padel_in_SNAP               (* (reg8 *) Padel_in__SNAP)
    
	#define Padel_in_0_INTTYPE_REG 		(* (reg8 *) Padel_in__0__INTTYPE)
#endif /* (Padel_in__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Padel_in_H */


/* [] END OF FILE */
