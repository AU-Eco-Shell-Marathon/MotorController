/*******************************************************************************
* File Name: DAC_out.c  
* Version 2.0
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "DAC_out.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 DAC_out__PORT == 15 && ((DAC_out__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: DAC_out_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void DAC_out_Write(uint8 value) 
{
    uint8 staticBits = (DAC_out_DR & (uint8)(~DAC_out_MASK));
    DAC_out_DR = staticBits | ((uint8)(value << DAC_out_SHIFT) & DAC_out_MASK);
}


/*******************************************************************************
* Function Name: DAC_out_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  DAC_out_DM_STRONG     Strong Drive 
*  DAC_out_DM_OD_HI      Open Drain, Drives High 
*  DAC_out_DM_OD_LO      Open Drain, Drives Low 
*  DAC_out_DM_RES_UP     Resistive Pull Up 
*  DAC_out_DM_RES_DWN    Resistive Pull Down 
*  DAC_out_DM_RES_UPDWN  Resistive Pull Up/Down 
*  DAC_out_DM_DIG_HIZ    High Impedance Digital 
*  DAC_out_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void DAC_out_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(DAC_out_0, mode);
}


/*******************************************************************************
* Function Name: DAC_out_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro DAC_out_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 DAC_out_Read(void) 
{
    return (DAC_out_PS & DAC_out_MASK) >> DAC_out_SHIFT;
}


/*******************************************************************************
* Function Name: DAC_out_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 DAC_out_ReadDataReg(void) 
{
    return (DAC_out_DR & DAC_out_MASK) >> DAC_out_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(DAC_out_INTSTAT) 

    /*******************************************************************************
    * Function Name: DAC_out_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 DAC_out_ClearInterrupt(void) 
    {
        return (DAC_out_INTSTAT & DAC_out_MASK) >> DAC_out_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
