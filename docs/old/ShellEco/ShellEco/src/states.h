/*
 * states.h
 *
 * Created: 23-04-2014 11:42:22
 *  Author: Christoffer
 */ 


#ifndef STATES_H_
#define STATES_H_


//State machine function prototypes
void Stop(RampPar *motor);
void RampInit(StateType *state, RampPar *motor);
void RampSearch(StateType *state, RampPar *motor, volatile unsigned int *speed , volatile unsigned int *lastspeed);
void Acc(StateType *state, RampPar *motor, volatile unsigned int speedArray[]);
void Coast(StateType *state,RampPar *motor, volatile unsigned int *speed);

#endif /* STATES_H_ */