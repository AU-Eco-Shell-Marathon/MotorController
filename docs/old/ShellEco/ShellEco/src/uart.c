

/* ----------------------------------------------------------------------- 
 * Title:    Configuration USART Rx (8 bit + 1 stop bit 19200) 
 * Hardware: ATmega64M1 
 * Software: AtmelStudio
 -----------------------------------------------------------------------*/ 
#define F_CPU 8000000UL                  // Clock frequency 
#define BAUD 9600                       // Baud rate 

#include <avr/io.h> 
#include <util/delay.h>
#include <stdlib.h>
//#include <interrupt.h>
#include <stdio.h>
#include "uart.h"




// ********************************* Initialisation USART ********************************* 

void InitUart( void) 
{ 
DDRD |= 0b00001000;	//Sets PD3 as output.
DDRD &= 0b11101111; //Sets PD4 as input.			
LINCR = (1 << LSWRES); 
LINBRRH = (((F_CPU/BAUD)/16)-1)>>8; 
LINBRRL = (((F_CPU/BAUD)/16)-1); 
LINBTR = (1 << LDISR) | (16 << LBT0); 
LINCR = (1<<LENA)|(1<<LCMD2)|(1<<LCMD1)|(1<<LCMD0); 
} 

 
int SendChar (unsigned char byte_data) { 
    while (LINSIR & (1 << LBUSY));          // Wait while the UART is busy. 
    LINDAT = byte_data; 
    return 0; 
} 

char ReadChar (void) { 
     while (LINSIR & (1 << LBUSY));          // Wait while the UART is busy. 

     return LINDAT; 
 } 

 
int SendString(char* String)
{
  // Repeat untill zero-termination
  while (*String != 0)
  {
    // Send the character pointed to by "String"
    SendChar(*String);
    // Advance the pointer one step
    String++;
  }
  return 0;
}

int SendInteger(int number)
{
char array[7];
  // Convert the integer to an ASCII string (array), radix = 10 
  itoa(number, array, 10);
  // - then send the string
  SendString(array);
  return 0;
}


int SendUnsignedInteger(unsigned int number)
{
	char array[7];
	// Convert the unsigned integer to an ASCII string (array), radix = 10
	utoa(number, array, 10);
	// - then send the string
	SendString(array);
	return 0;
}
