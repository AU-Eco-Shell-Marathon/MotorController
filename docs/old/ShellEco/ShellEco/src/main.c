#define F_CPU 8000000UL
#include <asf.h>
#include <util/delay.h>
#include <stdio.h>
#include "defines.h"
#include "motor.h"
#include "states.h"
#include "io.h"
#include "uart.h"
#include "protection.h"

//Globals...
volatile StateType Sm_State = STATE_STOP;
volatile unsigned int speed = 65535;
volatile unsigned int lastSpeed = 65535;
volatile unsigned int speedArray[SPEED_ARRAY_SIZE] = { 0 };
volatile RampPar motor;
volatile int Timer0OverflowCounter = 0;
unsigned char NeedToRelease = 0;


int main (void)
{
	InitUart();	//Init uart
	initInput(); //Init interrupt to start button, stop button and speed sensor	
	initMotorPWM(); //Init Fast PWM on Timer 0 (Pin PE1)
	initSpeedTimer(); //Init Timer 1 (16bit normal mode)
	initCurrentADC(); //Init ADC on PC5
	
	
	//Init speed array
	for (int i = 0; i<(SPEED_ARRAY_SIZE); i++)
	{
		speedArray[i]=65535;
	}
	
	while (1)
	{
		//Switch to stop state if something is rotten
		Protection(&Sm_State, &speed);
		
		switch (Sm_State)
		{
			case STATE_STOP:
			Stop(&motor);
			break;
				
			case STATE_RAMP_INIT:
			RampInit(&Sm_State, &motor);
			break;
				
			case STATE_RAMP_SEARCH:
			RampSearch(&Sm_State, &motor, &speed, &lastSpeed);
			break;
				
			case STATE_ACC:
			Acc(&Sm_State,&motor, speedArray);
			break;
				
			case STATE_COAST:
			Coast(&Sm_State, &motor, &speed);
			break;
				
			default:
			Sm_State = STATE_STOP;
			break;
		}
		
	}

}


//Stop button
ISR(INT0_vect)
{
	Sm_State = STATE_STOP;
}

//Speed count
ISR(INT1_vect)
{	
	//Zeroset timer 1 overflow flag
	if (TIFR1 & (0x1<<TOV1))
	{
		TIFR1 |= (1<<TOV1);
		
		speed = 65535;
		
		for (int i = 0; i<(SPEED_ARRAY_SIZE); i++)
		{
			speedArray[i]=65535;
		}
	}
	else
	{
		int i = (SPEED_ARRAY_SIZE-1);
		
		//Shift speed arrays
		while (i > 0)
		{
			speedArray[i] = speedArray[i-1];		
			i--;
		}
		
		speedArray[0] = TCNT1;			
		
		//Save last speed
		lastSpeed = speed;
		speed = TCNT1;
	}
	
	//Zeroset timer value;
	TCNT1 = 0;

	if (TEST_MODE == 1)
	{	
	//	SendString("Speed:");
	//	SendInteger(speed);
	//	SendString("\n\r");
	}
	
}

//Start
ISR(INT2_vect)
{
	//Change to "STATE_RAMP_INIT" if state is different from "STATE_RAMP_SEARCH"
	if ((Sm_State != STATE_RAMP_SEARCH) & (Sm_State != STATE_ACC))
		{
			Sm_State = STATE_RAMP_INIT;
		}

}


//Ramp motor1
ISR(TIMER0_OVF_vect)
{
	if (Timer0OverflowCounter >= 750)
	{
		Timer0OverflowCounter = 0;
	
		motor.lastPwm = motor.actualPwm;
	
		if (motor.finalPwm != motor.actualPwm)
			{
				if (motor.actualPwm > motor.finalPwm)
					{
						motor.actualPwm = motor.lastPwm - motor.accelerationPwm;		
					}
				else
					{
						motor.actualPwm = motor.lastPwm + motor.accelerationPwm;
					}
				OCR0B = motor.actualPwm;
			}
	}

	Timer0OverflowCounter++;	
}
