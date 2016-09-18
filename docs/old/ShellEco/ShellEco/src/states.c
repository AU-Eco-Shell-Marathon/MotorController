#define F_CPU 8000000UL
#include <asf.h>
#include <util/delay.h>
#include "defines.h"
#include "states.h"
#include "motor.h"
#include "uart.h"

//Globals					P	I	P_GAIN	I_GAIN	previousErr	sp	pv	u	umax	umin	imax	imin	
RegulatorPar pCurrent = {	0,	0,	1.3,	2,		0,			0,	0,	0,	255,	0,		50,		0};

//Current function f(x) = a*x + b
AccPar accPar = {
//	0-1600		1600-2200	2200-3250	3250-7000rpm
	{0,			-0.02621,	-0.2489,		-0.3852},	//a part
	{65,		90.614,		248.7249,	314.2879}	//b part
};

//24km/t = 283 ticks/puls
//25km/t = 295 ticks/puls
//28km/t = 252 ticks/puls
//29km/t = 243 ticks/puls
//30km/t = 235 ticks/puls
//31km/t = 227 ticks/puls

						//	LowerLimit	UpperLimit	
BurnCoastPar burnCoast = {	295,		243};
//BurnCoastPar burnCoast = {	283,		235};
//BurnCoastPar burnCoast = {	307,		263};
//BurnCoastPar burnCoast = {	800,		300};

void Stop(RampPar *motor)
{
	if (TEST_MODE)
	{
		//SendString("Stop: \n\r ");
		SendString("1,2,3,4,5\n\r");
	 	//SendString("0,0,");	
	}
	
	
	enablePWM(0); //Disable PWM
	
	setPWM(0);
	
	motor->actualPwm = 0;
	motor->finalPwm = 0;
	motor->lastPwm = 0;
	motor->accelerationPwm = 0;
	
	_delay_ms(100);
}

void RampInit(StateType *state, RampPar *motor )
{
	if (TEST_MODE)
	{
		//SendString("RampInit state");	
		SendString("1,2,3,4,5\n\r");
	}
		
	RampPar tempMotor;
	
	//Enable PWM
	enablePWM(1); 
	
	//Start ramp from 
	startRamp(&tempMotor, 0, 64);
	
	//Copy tempvalues to global motorpar
	motor->actualPwm = tempMotor.actualPwm;
	motor->lastPwm = tempMotor.lastPwm;
	motor->finalPwm = tempMotor.finalPwm;
	motor->accelerationPwm = tempMotor.accelerationPwm;

	*state = STATE_RAMP_SEARCH;
}

void RampSearch(StateType *state, RampPar *motor, volatile unsigned int *speed, volatile unsigned int *lastspeed)
{
	if (TEST_MODE)
	{
		//SendString("RampSearch state ");
		SendString("1,2,3,4,5\n\r");
	}
/*
	SendInteger(*speed);
	SendString(", ");
	SendInteger((int)getCurrent());
	SendString(", ");
	SendInteger(motor->finalPwm);
	SendString(", ");
	SendInteger(motor->actualPwm);
	SendString("\n\r");
*/
	//RampPar tempMotor;
	
	// Check if speed is ascending	
	if (*speed < *lastspeed)
		{
			
			//stopRamp(&tempMotor);
			//Copy tempvalues to global motorpar
			//motor->actualPwm = tempMotor.actualPwm;
			//motor->finalPwm = tempMotor.finalPwm;
			//motor->lastPwm = tempMotor.lastPwm;
			//motor->accelerationPwm = tempMotor.accelerationPwm;
			
			*state = STATE_ACC;
		}

	//unsigned char current = getCurrent();
	
	//Check if current is ascending
	//if ( )

}

void Acc(StateType *state, RampPar *motor, volatile unsigned int speedArray[])
{
	if (TEST_MODE)
	{
		 SendString("Acc state ");	
	}
	
	//Calc avg speed from array
	double total = 0;
		
	for (int i=0; i<SPEED_ARRAY_SIZE; i++)
	{
		total = total + speedArray[i];
	}
	unsigned int avgSpeed = (unsigned int)total/SPEED_ARRAY_SIZE;
	
	//Find witch acceleration ramp to use
	unsigned char accRampMode = findAccRamp(avgSpeed);

	//Calc current regulator setpoint f(x)=a*x + b
	double tempSp = accPar.funkA[accRampMode]*avgSpeed+accPar.funkB[accRampMode]*1.2; //multiply with 1.2 to raise the setpoint
	
	
	//Limit calc to 0-255
	if (tempSp>255)
		tempSp = 255;
	else if (tempSp<0)
		tempSp = 0;
	
	//Typecast result to "unsigned char" to get rid of decimals
	unsigned char finalSp = (unsigned char)tempSp;
	
	
	setRegulatorSP(&pCurrent, finalSp );
	
	pCurrent.pv = (double)getCurrent();
	calcCurrent(&pCurrent);
	
	motor->finalPwm = (unsigned char)pCurrent.u;
	//setPWM((unsigned char)pCurrent.u);
	

	SendInteger((int)pCurrent.sp);
	SendString(", ");
	//SendInteger((int)pCurrent.pv);
	//SendString(", ");
	//SendInteger((int)pCurrent.u);
	//SendString(", ");
	SendInteger(avgSpeed);
	SendString(", ");
	SendInteger((int)getCurrent());
	//SendChar(getCurrent());
	//SendString(", ");	
	//SendInteger(burnCoast.upperSpeedLimit);
	//SendString(", ");
	//SendInteger(burnCoast.lowerSpeedLimit);
	SendString(", ");
	SendInteger(motor->finalPwm);
	SendString(", ");
	SendInteger(motor->actualPwm);
	SendString("\n\r");
	_delay_ms(100);
	
	
	//If start is pressed(PB5), skip speed check (Keep acc)
	unsigned char tempPB = PINB & 0b00100000;
	if (tempPB == 0b00100000)
	{
		//Force motor to coast at 30km/t
		SendString("Green is down\n\r");
		if (avgSpeed < 235 )
		{
			*state = STATE_COAST;
		}
	}
	else
	{
		//Coast when speed is above upper speed limit
		if (avgSpeed < burnCoast.upperSpeedLimit)
		{
			*state = STATE_COAST;
		}
	}

}

void Coast(StateType *state, RampPar *motor, volatile unsigned int *speed)
{
	if (TEST_MODE)
	{
		SendString("Coast state ,");	
	}
		
	//Set PWM to 0;
	setPWM(0);
	
	//Disable PWM
	enablePWM(0);
	
	motor->actualPwm = 0;
	motor->finalPwm = 0;
	motor->lastPwm = 0;
	motor->accelerationPwm = 0;
	
	_delay_ms(500);
	
	if (*speed > burnCoast.lowerSpeedLimit)
	{
		*state = STATE_RAMP_INIT;
	}	
}
