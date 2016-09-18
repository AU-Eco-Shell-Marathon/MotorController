/*
 * io.c
 *
 * Created: 29-04-2014 19:26:29
 *  Author: Christoffer
 */ 

#include "io.h"

void initInput(void)
{
		//Enable interrupts
		sei();
		
		//Enable INT0, INT1 and 2.
		EIMSK =  (1<< INT0) | (1 << INT1) | (1 << INT2);

		//Set INT0, INT1 and INT2 to falling edge
		EICRA = (1<< ISC01) | (1<<ISC00) | (1 << ISC11) | (0 << ISC10) | (1 << ISC21) | (1 << ISC20) ;

		DDRD &= 0b10111111;	//INT0 - Sets PD6 as input. STOP
		DDRB &= 0b11111011;	//INT1 - Sets PB2 as input. SPEED
		DDRB &= 0b11011111;	//INT2 - Sets PB5 as input. START
}