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
#include "PID.h"
#include <project.h>

struct PIDparameter parameter_;

int32 PIDval = 0;
int32 anti_windup_back_calc = 0;
int32 dt = dt_def;
int32 iState = 0;
int32 err = 0;
int32 pre_err = 0;

int32 Ki_dt = 0;
int32 Kd_dt = 0;
int32 Kp = 0;

void PID_init()
{
    parameter_.Kp = Kp_def;
    parameter_.Ki = Ki_def;
    parameter_.Kd = Kd_def;
    parameter_.MAX = MAX_def;
    parameter_.MIN = MIN_def;
    parameter_.preShift = preShift_def;
}



uint16 PID(const uint16 input, const uint16 plant)
{
    PIDval = 0;

    err = (((int32)input)<<parameter_.preShift) - (((int32)plant)<<parameter_.preShift);
    
    
	PIDval = Kp*err; //Proportional calc.
	
    if(Ki_dt != 0)
    {
	    iState += Ki_dt*err + anti_windup_back_calc; //intergral calc and anti windup
    }
	PIDval += iState; //intergral calc
	
	PIDval += Kd_dt*(err-pre_err); //differentiel calc
	
	pre_err = err;
	
    anti_windup_back_calc = PIDval;
    
	if(PIDval > parameter_.MAX<<parameter_.preShift)
		PIDval = parameter_.MAX<<parameter_.preShift;
    else if(PIDval < parameter_.MIN<<parameter_.preShift)
        PIDval = parameter_.MIN<<parameter_.preShift;
	
    anti_windup_back_calc = PIDval - anti_windup_back_calc;
    
    return ((uint16)(PIDval>>parameter_.preShift));
}

void setPID(const struct PIDparameter * parameter)
{
    parameter_ = *parameter;
    
    
    
    
    /*
    uint32 MSB_temp = (parameter_.Ki>>31);
    Ki_dt = ((parameter_.Ki<<(parameter_.preShift-parameter_.KShift))||(MSB_temp<<31))*dt;
    MSB_temp = (parameter_.Kd>>31);
    Kd_dt = ((parameter_.Kd<<(parameter_.preShift-parameter_.KShift))||(MSB_temp<<31))/dt;
    MSB_temp = (parameter_.Kp>>31);
    Kp = (parameter_.Kp<<(parameter_.preShift-parameter_.KShift))||(MSB_temp<<31);
    */
    
    Ki_dt = (parameter_.Ki<<(parameter_.preShift-parameter_.KShift))*dt;
    Kd_dt = (parameter_.Kd<<(parameter_.preShift-parameter_.KShift))/dt;
    Kp = parameter_.Kp<<(parameter_.preShift-parameter_.KShift);
    
}

/* [] END OF FILE */
