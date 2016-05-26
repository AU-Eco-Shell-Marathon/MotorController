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
    uint16 power = powerSensor_getValue();
    uint16 rpm = RPMSensor_getValue();
    uint16 WantedSpeed = Pedal_getValue();
    
    uint16 Power_LUT = LUT_speed(WantedSpeed, rpm)*10;
    
    switch(state){
    case TEST:
        //til test og andet.
        break;
    case ACC:
        
        output = PID(Power_LUT, power);
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


CY_ISR(OVERCURRENT) //tjekker om det er nødvendigt at lukke for systemet eller ej.
{
    CyGlobalIntDisable;
    MC_stop();
    PWM_motor_WriteCompare(0x00);
    CyDelay(1000);
    PWM_motor_WriteCompare(0x01);
    Control_Reg_motor_reset_Write(1u);
    CyDelay(100);
    if((Status_Reg_1_Read()&1u) == 1u)
    {
        PWM_motor_WriteCompare(0x00);
        while(1); // der er for stor belastning på motoren og MCU går i lukke tilstand og kræver en genstart af systemet før bilen kan køre videre.
    }
    
    
    MC_start(); //Det er vurderet at det er iorden at forsætte og MCU går tilbage til normal tilstand og tillader interupt igen.
    CyGlobalIntEnable;
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
    //initialiser PID regulatoren
    PID_init();
    setPID(pidval);
    
    //opsætter interrupt rutine til tilfælde af overbelastning.
    isr_overCurrent_StartEx(OVERCURRENT);
    
    //Starter de hardware blokke der skal bruges af MotorController.
    Clock_motor_Start();
    PWM_motor_Start();
    
    //Sætter bilen i start tilstand og opsætter interrupt rutine til MotorControlleren.
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
