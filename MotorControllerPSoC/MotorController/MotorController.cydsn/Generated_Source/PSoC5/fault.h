/*******************************************************************************
* File Name: fault.h  
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

#if !defined(CY_PINS_fault_H) /* Pins fault_H */
#define CY_PINS_fault_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "fault_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 fault__PORT == 15 && ((fault__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    fault_Write(uint8 value);
void    fault_SetDriveMode(uint8 mode);
uint8   fault_ReadDataReg(void);
uint8   fault_Read(void);
void    fault_SetInterruptMode(uint16 position, uint16 mode);
uint8   fault_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the fault_SetDriveMode() function.
     *  @{
     */
        #define fault_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define fault_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define fault_DM_RES_UP          PIN_DM_RES_UP
        #define fault_DM_RES_DWN         PIN_DM_RES_DWN
        #define fault_DM_OD_LO           PIN_DM_OD_LO
        #define fault_DM_OD_HI           PIN_DM_OD_HI
        #define fault_DM_STRONG          PIN_DM_STRONG
        #define fault_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define fault_MASK               fault__MASK
#define fault_SHIFT              fault__SHIFT
#define fault_WIDTH              1u

/* Interrupt constants */
#if defined(fault__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in fault_SetInterruptMode() function.
     *  @{
     */
        #define fault_INTR_NONE      (uint16)(0x0000u)
        #define fault_INTR_RISING    (uint16)(0x0001u)
        #define fault_INTR_FALLING   (uint16)(0x0002u)
        #define fault_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define fault_INTR_MASK      (0x01u) 
#endif /* (fault__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define fault_PS                     (* (reg8 *) fault__PS)
/* Data Register */
#define fault_DR                     (* (reg8 *) fault__DR)
/* Port Number */
#define fault_PRT_NUM                (* (reg8 *) fault__PRT) 
/* Connect to Analog Globals */                                                  
#define fault_AG                     (* (reg8 *) fault__AG)                       
/* Analog MUX bux enable */
#define fault_AMUX                   (* (reg8 *) fault__AMUX) 
/* Bidirectional Enable */                                                        
#define fault_BIE                    (* (reg8 *) fault__BIE)
/* Bit-mask for Aliased Register Access */
#define fault_BIT_MASK               (* (reg8 *) fault__BIT_MASK)
/* Bypass Enable */
#define fault_BYP                    (* (reg8 *) fault__BYP)
/* Port wide control signals */                                                   
#define fault_CTL                    (* (reg8 *) fault__CTL)
/* Drive Modes */
#define fault_DM0                    (* (reg8 *) fault__DM0) 
#define fault_DM1                    (* (reg8 *) fault__DM1)
#define fault_DM2                    (* (reg8 *) fault__DM2) 
/* Input Buffer Disable Override */
#define fault_INP_DIS                (* (reg8 *) fault__INP_DIS)
/* LCD Common or Segment Drive */
#define fault_LCD_COM_SEG            (* (reg8 *) fault__LCD_COM_SEG)
/* Enable Segment LCD */
#define fault_LCD_EN                 (* (reg8 *) fault__LCD_EN)
/* Slew Rate Control */
#define fault_SLW                    (* (reg8 *) fault__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define fault_PRTDSI__CAPS_SEL       (* (reg8 *) fault__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define fault_PRTDSI__DBL_SYNC_IN    (* (reg8 *) fault__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define fault_PRTDSI__OE_SEL0        (* (reg8 *) fault__PRTDSI__OE_SEL0) 
#define fault_PRTDSI__OE_SEL1        (* (reg8 *) fault__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define fault_PRTDSI__OUT_SEL0       (* (reg8 *) fault__PRTDSI__OUT_SEL0) 
#define fault_PRTDSI__OUT_SEL1       (* (reg8 *) fault__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define fault_PRTDSI__SYNC_OUT       (* (reg8 *) fault__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(fault__SIO_CFG)
    #define fault_SIO_HYST_EN        (* (reg8 *) fault__SIO_HYST_EN)
    #define fault_SIO_REG_HIFREQ     (* (reg8 *) fault__SIO_REG_HIFREQ)
    #define fault_SIO_CFG            (* (reg8 *) fault__SIO_CFG)
    #define fault_SIO_DIFF           (* (reg8 *) fault__SIO_DIFF)
#endif /* (fault__SIO_CFG) */

/* Interrupt Registers */
#if defined(fault__INTSTAT)
    #define fault_INTSTAT            (* (reg8 *) fault__INTSTAT)
    #define fault_SNAP               (* (reg8 *) fault__SNAP)
    
	#define fault_0_INTTYPE_REG 		(* (reg8 *) fault__0__INTTYPE)
#endif /* (fault__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_fault_H */


/* [] END OF FILE */
