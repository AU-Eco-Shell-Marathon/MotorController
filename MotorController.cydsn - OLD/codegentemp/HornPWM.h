/*******************************************************************************
* File Name: HornPWM.h  
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

#if !defined(CY_PINS_HornPWM_H) /* Pins HornPWM_H */
#define CY_PINS_HornPWM_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "HornPWM_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 HornPWM__PORT == 15 && ((HornPWM__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    HornPWM_Write(uint8 value);
void    HornPWM_SetDriveMode(uint8 mode);
uint8   HornPWM_ReadDataReg(void);
uint8   HornPWM_Read(void);
void    HornPWM_SetInterruptMode(uint16 position, uint16 mode);
uint8   HornPWM_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the HornPWM_SetDriveMode() function.
     *  @{
     */
        #define HornPWM_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define HornPWM_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define HornPWM_DM_RES_UP          PIN_DM_RES_UP
        #define HornPWM_DM_RES_DWN         PIN_DM_RES_DWN
        #define HornPWM_DM_OD_LO           PIN_DM_OD_LO
        #define HornPWM_DM_OD_HI           PIN_DM_OD_HI
        #define HornPWM_DM_STRONG          PIN_DM_STRONG
        #define HornPWM_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define HornPWM_MASK               HornPWM__MASK
#define HornPWM_SHIFT              HornPWM__SHIFT
#define HornPWM_WIDTH              1u

/* Interrupt constants */
#if defined(HornPWM__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in HornPWM_SetInterruptMode() function.
     *  @{
     */
        #define HornPWM_INTR_NONE      (uint16)(0x0000u)
        #define HornPWM_INTR_RISING    (uint16)(0x0001u)
        #define HornPWM_INTR_FALLING   (uint16)(0x0002u)
        #define HornPWM_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define HornPWM_INTR_MASK      (0x01u) 
#endif /* (HornPWM__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define HornPWM_PS                     (* (reg8 *) HornPWM__PS)
/* Data Register */
#define HornPWM_DR                     (* (reg8 *) HornPWM__DR)
/* Port Number */
#define HornPWM_PRT_NUM                (* (reg8 *) HornPWM__PRT) 
/* Connect to Analog Globals */                                                  
#define HornPWM_AG                     (* (reg8 *) HornPWM__AG)                       
/* Analog MUX bux enable */
#define HornPWM_AMUX                   (* (reg8 *) HornPWM__AMUX) 
/* Bidirectional Enable */                                                        
#define HornPWM_BIE                    (* (reg8 *) HornPWM__BIE)
/* Bit-mask for Aliased Register Access */
#define HornPWM_BIT_MASK               (* (reg8 *) HornPWM__BIT_MASK)
/* Bypass Enable */
#define HornPWM_BYP                    (* (reg8 *) HornPWM__BYP)
/* Port wide control signals */                                                   
#define HornPWM_CTL                    (* (reg8 *) HornPWM__CTL)
/* Drive Modes */
#define HornPWM_DM0                    (* (reg8 *) HornPWM__DM0) 
#define HornPWM_DM1                    (* (reg8 *) HornPWM__DM1)
#define HornPWM_DM2                    (* (reg8 *) HornPWM__DM2) 
/* Input Buffer Disable Override */
#define HornPWM_INP_DIS                (* (reg8 *) HornPWM__INP_DIS)
/* LCD Common or Segment Drive */
#define HornPWM_LCD_COM_SEG            (* (reg8 *) HornPWM__LCD_COM_SEG)
/* Enable Segment LCD */
#define HornPWM_LCD_EN                 (* (reg8 *) HornPWM__LCD_EN)
/* Slew Rate Control */
#define HornPWM_SLW                    (* (reg8 *) HornPWM__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define HornPWM_PRTDSI__CAPS_SEL       (* (reg8 *) HornPWM__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define HornPWM_PRTDSI__DBL_SYNC_IN    (* (reg8 *) HornPWM__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define HornPWM_PRTDSI__OE_SEL0        (* (reg8 *) HornPWM__PRTDSI__OE_SEL0) 
#define HornPWM_PRTDSI__OE_SEL1        (* (reg8 *) HornPWM__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define HornPWM_PRTDSI__OUT_SEL0       (* (reg8 *) HornPWM__PRTDSI__OUT_SEL0) 
#define HornPWM_PRTDSI__OUT_SEL1       (* (reg8 *) HornPWM__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define HornPWM_PRTDSI__SYNC_OUT       (* (reg8 *) HornPWM__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(HornPWM__SIO_CFG)
    #define HornPWM_SIO_HYST_EN        (* (reg8 *) HornPWM__SIO_HYST_EN)
    #define HornPWM_SIO_REG_HIFREQ     (* (reg8 *) HornPWM__SIO_REG_HIFREQ)
    #define HornPWM_SIO_CFG            (* (reg8 *) HornPWM__SIO_CFG)
    #define HornPWM_SIO_DIFF           (* (reg8 *) HornPWM__SIO_DIFF)
#endif /* (HornPWM__SIO_CFG) */

/* Interrupt Registers */
#if defined(HornPWM__INTSTAT)
    #define HornPWM_INTSTAT            (* (reg8 *) HornPWM__INTSTAT)
    #define HornPWM_SNAP               (* (reg8 *) HornPWM__SNAP)
    
	#define HornPWM_0_INTTYPE_REG 		(* (reg8 *) HornPWM__0__INTTYPE)
#endif /* (HornPWM__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_HornPWM_H */


/* [] END OF FILE */
