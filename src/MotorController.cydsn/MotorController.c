/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "MotorController.h"

/*
const uint8 * speed_ = NULL;
const uint16 * rpm_ = NULL;
const uint16 * volt_ = NULL;
const uint16 * current_ = NULL;
*/
uint16 Threshold_ = 57; // i RPM
enum STATES {TEST, ACC, CAB, STOP}; //CAB = cost and burn

enum STATES state = STOP;

CY_ISR_PROTO(MOTOR_tick);
CY_ISR_PROTO(OVERCURRENT);

CY_ISR(MOTOR_tick)
{

    uint16 output;
    uint16 power = effectSensor_getValue();
    uint16 rpm = RPMSensor_getValue();
    uint16 WantedSpeed = Pedal_getValue();
    uint16 Speed_LUT = LUT_speed(WantedSpeed, rpm, power);
    
    switch(state){
    case TEST:
        //til test og andet.
        break;
    case ACC:
        
        output = PID(Speed_LUT, rpm);
        PWM_motor_WriteCompare((uint8)output);
        if(rpm >= WantedSpeed) state = CAB;
        break;
    case CAB:
        if(rpm <= (WantedSpeed - Threshold_)) state = ACC;
    case STOP:
    default:
        PWM_motor_WriteCompare(0x00);
        break;
    }

}


CY_ISR(OVERCURRENT)
{
    MC_stop();
}

void MC_start()
{
    state = ACC;
}

void MC_stop()
{
    state = STOP;
}

void MC_init(const struct PIDparameter * pidval)
{
    PID_init();
    setPID(pidval);
    
    isr_overCurrent_StartEx(OVERCURRENT);
    
    /*
    speed_ = speed;
    rpm_ = rpm;
    current_ = current;
    volt_ = volt;
    */
    
    Clock_motor_Start();
    PWM_motor_Start();
    MC_start();
    
    isr_motor_StartEx(MOTOR_tick);
}

void MC_ChangePID(const struct PIDparameter * pidval)
{
    setPID(pidval);
}

void MC_setThreshold(uint16 Threshold)
{
       Threshold_ = Threshold;
}




/* [] END OF FILE */
