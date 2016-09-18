/*
 * safety.c
 *
 * Created: 15-05-2014 15:57:56
 *  Author: Christoffer
 */ 

#include <asf.h>
#include "defines.h"
#include "protection.h"
#include "uart.h"

void Protection (StateType *state, volatile unsigned int *speed)
{
	//Check if stop is pressed (PD6)
	unsigned char tempPD = PIND & 0b01000000;
	if (tempPD==0b01000000)
	{
		SendString("Protection stop: Stop button is down!");
		*state = STATE_STOP;
	}
	
	//Check if PWM is >~80% and wheel speed is slow.
	if ((OCR0B >= 200) & (*speed > 30000))
	{
		SendString("Protection stop: PWM is 100% and wheel speed is slow");
		*state =  STATE_STOP;
	}

}