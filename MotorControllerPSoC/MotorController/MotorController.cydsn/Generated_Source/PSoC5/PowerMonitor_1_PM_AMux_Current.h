/*******************************************************************************
* File Name: PowerMonitor_1_PM_AMux_Current.h
* Version 1.50
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
*******************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_PowerMonitor_1_PM_AMux_Current_H)
#define CY_AMUX_PowerMonitor_1_PM_AMux_Current_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PM_AMux_v1_50 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void PowerMonitor_1_PM_AMux_Current_Start(void) ;
void PowerMonitor_1_PM_AMux_Current_Init(void) ;
void PowerMonitor_1_PM_AMux_Current_Stop(void) ;
void PowerMonitor_1_PM_AMux_Current_Select(uint8 channel) ;
void PowerMonitor_1_PM_AMux_Current_FastSelect(uint8 channel) ;
void PowerMonitor_1_PM_AMux_Current_DisconnectAll(void) ;


/***************************************
*     Initial Parameter Constants
***************************************/

#define PowerMonitor_1_PM_AMux_Current_CHANNELS  38u
#define PowerMonitor_1_PM_AMux_Current_MUXTYPE   1u


/***************************************
*             API Constants
***************************************/

#define PowerMonitor_1_PM_AMux_Current_NULL_CHANNEL  0xFFu
#define PowerMonitor_1_PM_AMux_Current_MUX_SINGLE   1u
#define PowerMonitor_1_PM_AMux_Current_MUX_DIFF     2u


/***************************************
*        Conditional Functions
***************************************/

#if (PowerMonitor_1_PM_AMux_Current_MUXTYPE == PowerMonitor_1_PM_AMux_Current_MUX_SINGLE)
    # define PowerMonitor_1_PM_AMux_Current_Connect(channel) PowerMonitor_1_PM_AMux_Current_Set(channel)
    # define PowerMonitor_1_PM_AMux_Current_Disconnect(channel) PowerMonitor_1_PM_AMux_Current_Unset(channel)
#else
    void PowerMonitor_1_PM_AMux_Current_Connect(uint8 channel) ;
    void PowerMonitor_1_PM_AMux_Current_Disconnect(uint8 channel) ;
#endif /* PowerMonitor_1_PM_AMux_Current_MUXTYPE == PowerMonitor_1_PM_AMux_Current_MUX_SINGLE */

#endif /* CY_AMUX_PowerMonitor_1_PM_AMux_Current_H */


/* [] END OF FILE */
