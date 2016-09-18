/*
 * motor.c
 *
 * Created: 29-04-2014 20:09:06
 *  Author: Christoffer
 */ 

#include <asf.h>
#include "defines.h"
#include "motor.h"
#include "adc_drv.h"
#include "uart.h"

//************************** Motor PWM *******************************
void initMotorPWM(void)
{
	DDRE |= 0b00000010; //Sets PE1 as output
	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0B1) | (0 << COM0B0); // 8-bit Fast PWM, clear OC0B on compare match.
	TCCR0B = (0 << CS02) | (0 << CS01) | (1 << CS00) | (0 << WGM02); // n = 1 fout = fclk/n*256 = 31,25kHz
	PORTE &= 0b11111101 ; //Sets value on PE1 to 0.
	
}

void enablePWM(unsigned char onoff)
{
	if (onoff == 1)
	{
			TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0B1) | (0 << COM0B0); //ON
			PORTE &= 0b11111101 ; //Sets value on PE1 to 0.
	}
	else
	{
			TCCR0A = (1 << WGM00) | (1 << WGM01) | (0 << COM0B1) | (0 << COM0B0); //Off
			PORTE &= 0b11111101 ; //Sets value on PE1 to 0.
	}
}


void setPWM(unsigned char pwm)
{ 
	OCR0B = pwm;
}

//************************** Motor RAMP *******************************

void startRamp(RampPar *motor, unsigned char startPwm, unsigned char finalPwm)
{
	TIMSK0 = (1 << TOIE0); //Enable overflow interupt
	motor->accelerationPwm = 1;
	motor->actualPwm = startPwm;
	motor->lastPwm = 0;
	motor->finalPwm = finalPwm;
	
}

void stopRamp(RampPar *motor)
{
	TIMSK0 = (0 << TOIE0); //Disable overflow interupt
	motor->accelerationPwm = 0;
	motor->actualPwm = 0;
	motor->lastPwm = 0;
	motor->finalPwm = 0;
	setPWM(0);
}


//**************************   Speed   *******************************
void initSpeedTimer(void)
{
	//Timer 1 (16bit normal mode)
	TCCR1A = (0 << WGM10) | (0 << WGM11) | (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0);
	TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10) | (0 << WGM12) | (0 << WGM13); //Prescaler 1024 | 1tick = 1000ms/(8Mhz/1024) = 0,128ms
}


unsigned int getSpeed(volatile unsigned int array[],int size)
{
	double total = 0;
	
	for (int i=0; i<size; i++)
	{
			total = total + array[i];
	}
	
	return (unsigned int)(total/size);
	
}

//************************** Current *******************************
void initCurrentADC(void)
{
	DDRC &= 0b11011111; //Sets PC5 as input.
	adc_select_vref(adc_voltage_ref_avcc_wcap); //Selects ADC reference voltage.
	adc_select_input(adc_input_channel_9); //Selects ADC9.
	adc_left_adjust(true); // Adjusts reading.
	adc_prescaler_selection(adc_prescaler_64); // ADC conversion at 8Mhz/64 = 125kHz.
	adc_trigger_enable(false); //Makes sure that ADC runs in single mode.
	adc_interrupt_enable(false); //Makes sure that theres is no interrupt at the end of conversion.
	adc_enable(true); //Enables ADC.
	adc_start_conversion(); // Initializes ADC.
}

void initCurrentRegulator(RegulatorPar *pReg)
{
	//INIT values for Pcurrent
	pReg->sp = 0; //71 = Around 3,3amp
	pReg->P_GAIN = 2;
	pReg->u_max = 255;
	pReg->u_min = 0;
}

int findAccRamp(int speed)
{
		if (speed<=32000 && speed>=977)	//Speed 0-1600rpm (0 - 7,54km/t)
		{
			return 0;
		}
		
		else if (speed<=976 && speed>=710)	//Speed 1600-2200rpm (7,54 - 10,37km/t)
		{
			return 1;
		}
		
		else if (speed<=709 && speed>=481)	//Speed 2200 - 3250rpm (10,37 - 15,32km/t)
		{
			return 2;
		}

		else if (speed<=480 && speed>=1)	//Speed 3250 - ....rpm (15,32 - ....km/t)
		{
			return 3;
		}
		else
		{
			return 0;
		}
}

void calcCurrent( RegulatorPar *pReg )
{
	int actualError;

	actualError = pReg->sp - pReg->pv;				//Find actual error	
	
	
	pReg->P = actualError * pReg->P_GAIN;			//Find P
	
	
	pReg->I +=  (pReg->I + pReg->previouserror)*pReg->I_GAIN;


	//Limit I
	if( pReg->I > pReg->I_max )
	pReg->I = pReg->I_max;
	
	if( pReg->I < pReg->I_min )
	pReg->I = pReg->I_min;
	
	
	pReg->u = pReg->P + pReg->I;								//Calc output
		
	//Limit output
	if( pReg->u > pReg->u_max )
		pReg->u = pReg->u_max;
	
	if( pReg->u < pReg->u_min )
		pReg->u = pReg->u_min;
	
	pReg->previouserror = actualError;
}

void setRegulatorSP( RegulatorPar *pReg, double sp)
{
	pReg->sp = sp;
}

unsigned char getCurrent(void)
{
	adc_start_conversion();
	while(!adcIsEndConversion());
	return adc_read_high();
}