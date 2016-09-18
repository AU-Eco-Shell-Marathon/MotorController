/*
 * defines.h
 *
 * Created: 29-04-2014 19:07:26
 *  Author: Christoffer
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

//If 1 then uart is broardcasted!
#define TEST_MODE 1

//Size of array to average speed pulses
#define SPEED_ARRAY_SIZE 6

//Type defines for state machine
typedef volatile enum
{
	STATE_STOP,
	STATE_RAMP_INIT,
	STATE_RAMP_SEARCH,
	STATE_ACC,
	STATE_COAST,
	NUM_STATES
}StateType;


//Type defines for motor and regulator
typedef struct
{
	double P;
	double I;

	double P_GAIN;
	double I_GAIN;
	
	double previouserror;
	double sp, pv, u, u_max, u_min, I_max, I_min;
}RegulatorPar;


typedef volatile struct
{
	unsigned char accelerationPwm;
	unsigned char actualPwm;
	unsigned char lastPwm;
	unsigned char finalPwm;
	unsigned char actualCurrent;
	unsigned char lastCurrent;
	unsigned char gapCurrent;
	unsigned char maxCurrent;
}RampPar;

typedef volatile struct
{
	double funkA[4];
	double funkB[4];
	
}AccPar;

typedef volatile struct
{
	int lowerSpeedLimit;
	int upperSpeedLimit;
	
}BurnCoastPar;

#endif /* DEFINES_H_ */