/*******************************************************************************
* File Name: Control_Reg_motor_reset.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_Control_Reg_motor_reset_H) /* CY_CONTROL_REG_Control_Reg_motor_reset_H */
#define CY_CONTROL_REG_Control_Reg_motor_reset_H

#include "cytypes.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} Control_Reg_motor_reset_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    Control_Reg_motor_reset_Write(uint8 control) ;
uint8   Control_Reg_motor_reset_Read(void) ;

void Control_Reg_motor_reset_SaveConfig(void) ;
void Control_Reg_motor_reset_RestoreConfig(void) ;
void Control_Reg_motor_reset_Sleep(void) ; 
void Control_Reg_motor_reset_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define Control_Reg_motor_reset_Control        (* (reg8 *) Control_Reg_motor_reset_Sync_ctrl_reg__CONTROL_REG )
#define Control_Reg_motor_reset_Control_PTR    (  (reg8 *) Control_Reg_motor_reset_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_Control_Reg_motor_reset_H */


/* [] END OF FILE */
