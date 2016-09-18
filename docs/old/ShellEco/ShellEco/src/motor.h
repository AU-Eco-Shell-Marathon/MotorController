/*
 * motor.h
 *
 * Created: 29-04-2014 20:08:52
 *  Author: Christoffer
 */ 



#ifndef MOTOR_H_
#define MOTOR_H_




//Motor PWM
void initMotorPWM (void);
void enablePWM(unsigned char onoff);
void setPWM(unsigned char pwm);

//Motor ramp
void startRamp(RampPar *motor, unsigned char startPwm, unsigned char finalPwm);
void stopRamp(RampPar *motor);

//Speed measurement
void initSpeedTimer(void);
unsigned int getSpeed(volatile unsigned int array[], int size);

//Current regulator
void initCurrentADC(void);
void initCurrentRegulator( RegulatorPar *pReg );
int findAccRamp(int speed);
void calcCurrent( RegulatorPar *pReg );
void setRegulatorSP( RegulatorPar *pReg, double sp);
unsigned char getCurrent(void);

#endif /* MOTOR_H_ */