/*******************************************************************************
* File Name: Print.c  
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
#include "Print.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Print__PORT == 15 && ((Print__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Print_Write
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
void Print_Write(uint8 value) 
{
    uint8 staticBits = (Print_DR & (uint8)(~Print_MASK));
    Print_DR = staticBits | ((uint8)(value << Print_SHIFT) & Print_MASK);
}


/*******************************************************************************
* Function Name: Print_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Print_DM_STRONG     Strong Drive 
*  Print_DM_OD_HI      Open Drain, Drives High 
*  Print_DM_OD_LO      Open Drain, Drives Low 
*  Print_DM_RES_UP     Resistive Pull Up 
*  Print_DM_RES_DWN    Resistive Pull Down 
*  Print_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Print_DM_DIG_HIZ    High Impedance Digital 
*  Print_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Print_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Print_0, mode);
}


/*******************************************************************************
* Function Name: Print_Read
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
*  Macro Print_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Print_Read(void) 
{
    return (Print_PS & Print_MASK) >> Print_SHIFT;
}


/*******************************************************************************
* Function Name: Print_ReadDataReg
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
uint8 Print_ReadDataReg(void) 
{
    return (Print_DR & Print_MASK) >> Print_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Print_INTSTAT) 

    /*******************************************************************************
    * Function Name: Print_ClearInterrupt
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
    uint8 Print_ClearInterrupt(void) 
    {
        return (Print_INTSTAT & Print_MASK) >> Print_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
