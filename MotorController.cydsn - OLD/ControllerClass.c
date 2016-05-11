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
#include "ControllerClass.h"

struct BMSData * BMSdata; //Ik' tænk over hvordan det virker, det gøre det bare ;)


void init()
{
    struct PIDparameter PID;
    size_t sizes[] = {sizeof(struct PIDparameter), sizeof(int16[2])};
    EEPROM_init(sizes, 2);
    
    if(EEPROM_read(0, (uint8 *)&PID) == 0)
    {
        PID.Kp = 1;
        PID.Ki = 0;
        PID.Kd = 0;
        PID.KShift = 0;
        PID.MAX = 255;
        PID.MIN = 0;
        PID.preShift = 0;
        PID.valid = 1;
        EEPROM_write(0, (const uint8 *)&PID);
    }
    
    int16 offset[2];
    if(EEPROM_read(1, (uint8 *)offset) == 0)
    {
        effectSensor_init(0, 0);
        effectSensor_calibrate(&offset[0], &offset[1]);
        EEPROM_write(1, (uint8 *)offset);
    }
    else
    {
        effectSensor_init(offset[0], offset[1]); 
    }

    RPMSensor_init();
    Pedal_init();
    
    MC_init((struct PIDparameter *)&PID);
    Logger_Init();
    BMSdata = CAN_init();
    Clock_3_Start();
}


struct dataSample
{
    int16 MC_volt;
    int16 MC_current;
};

void run()
{
    Pedal_update();
    if(BMSdata->NewData == 0b00111111)
    {
        Logger_LogData(1,
            BMSdata->battery_V,
            BMSdata->battery_cell_no_max,
            BMSdata->battery_cell_V_max,
            BMSdata->battery_cell_no_min,
            BMSdata->battery_cell_V_min,
            BMSdata->battery_A,
            BMSdata->IR_no_max,
            BMSdata->IR_max,
            BMSdata->IR_no_min,
            BMSdata->IR_min,
            BMSdata->Tbatt,
            BMSdata->state,
            BMSdata->RunTime,
            RPMSensor_getSpeed(),
            effectSensor_getValue()
        );
        
        BMSdata->NewData = 0x00;
    }
}

/* [] END OF FILE */
