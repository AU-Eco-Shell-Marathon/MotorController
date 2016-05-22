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
#include "powerSensor.h"
#define effect_sensor_OFFSET 5000 //mV
int16   Current_temp = 0,
        Volt_temp = 0;

int16   Volt_offset = 0,
        Current_offset = 0;


int32   y_Current = 0,
        y_Volt = 0;

uint8 a = 3; // fra 0 til 255! er lig med 0-1.


void DMA_ADC_A_V_init();

CY_ISR_PROTO(ISR_AMP);

CY_ISR_PROTO(ISR_VOLT);

void VDACSetOffset(int16 Current);


CY_ISR(ISR_AMP)
{
    y_Current = (((int32)a*(int32)Current_temp)<<8) + (int32)(((int64)(256u-a)*(int64)y_Current)>>8) + (int32)1;
}


CY_ISR(ISR_VOLT)
{
    y_Volt = (((int32)a*(int32)Volt_temp)<<8) + (int32)(((int64)(256u-a)*(int32)y_Volt)>>8) + (int32)1;
}

void powerSensor_init(int16 Volt, int16 Current)
{
    Volt_offset = Volt;
    Current_offset = Current;
    
    
    PGA_1_Start();
    PGA_2_Start();
   // Opamp_1_Start();
    
    ADC_A_Start();
    ADC_V_Start();
    ADC_A_StartConvert();
    ADC_V_StartConvert();
    
    isr_A_StartEx(ISR_AMP);
    isr_V_StartEx(ISR_VOLT);
    
    DMA_ADC_A_V_init();
    
    //VDACSetOffset(Current_offset);
    VDAC8_1_Start();
    CyDelay(100);
    Comp_1_Start();
}

void powerSensor_calibrate(int16 * Volt, int16 * Current)
{
    uint8 a_tmp = a;
    a = 1;
    CyDelay(10000);
    Volt_offset = (int16)(y_Volt>>16);
    Current_offset = (int16)(y_Current>>16);
    
    a = a_tmp;
    
    *Volt = Volt_offset;
    *Current = Current_offset;
    //VDACSetOffset(ADC_A_CountsTo_mVolts(Current_offset)-effect_sensor_OFFSET);
}

/*

void VDACSetOffset(int16 Current)
{
    uint8 calc = (uint8)(200+(int8)(Current>>4)); // 200 => 3200mV
    //VDAC8_1_SetValue(calc); 
    Control_Reg_motor_reset_Write(1u);
}
*/



uint16 powerSensor_getValue()
{
    int16 rawVolt = ADC_V_CountsTo_mVolts((int16)(y_Volt>>16)-Volt_offset); // mangler validering
    int16 rawCurrent = ADC_A_CountsTo_mVolts((int16)(y_Current>>16)-Current_offset);
    
    /*
    uint32 volt = (rawVolt <= (int16)effect_sensor_OFFSET ? 0u : rawVolt - (uint32)effect_sensor_OFFSET)*15.625;// *15.625)/1000; skal trække minus 5 volt fra!!!
    uint32 current = (rawCurrent <= (int16)effect_sensor_OFFSET ? 0u : rawCurrent - (uint32)effect_sensor_OFFSET)*5;// *3)/1000;
    */
    uint32 volt = (rawVolt <= 0 ? 0u : rawVolt)*15.625;
    uint32 current = (rawCurrent <= 0 ? 0u : rawCurrent)*5.21;
    
    return ((uint16)(((uint64)volt*(uint64)current)/100000));
}



void DMA_ADC_A_V_init()
{
    
    
    /* Defines for DMA_A */
    #define DMA_A_BYTES_PER_BURST 2
    #define DMA_A_REQUEST_PER_BURST 1
    #define DMA_A_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_A_DST_BASE (CYDEV_SRAM_BASE)

    /* Variable declarations for DMA_A */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_A_Chan;
    uint8 DMA_A_TD[1];

    /* DMA Configuration for DMA_A */
    DMA_A_Chan = DMA_A_DmaInitialize(DMA_A_BYTES_PER_BURST, DMA_A_REQUEST_PER_BURST, 
        HI16(DMA_A_SRC_BASE), HI16(DMA_A_DST_BASE));
    DMA_A_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_A_TD[0], 2, DMA_INVALID_TD, DMA_A__TD_TERMOUT_EN | TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_A_TD[0], LO16((uint32)ADC_A_SAR_WRK0_PTR), LO16((uint32)&Current_temp));
    CyDmaChSetInitialTd(DMA_A_Chan, DMA_A_TD[0]);
    CyDmaChEnable(DMA_A_Chan, 1);

    
    // ------------------------------------------------------------------------------------------------------ //
    
    /* Defines for DMA_V */
    #define DMA_V_BYTES_PER_BURST 2
    #define DMA_V_REQUEST_PER_BURST 1
    #define DMA_V_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_V_DST_BASE (CYDEV_SRAM_BASE)

    /* Variable declarations for DMA_V */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_V_Chan;
    uint8 DMA_V_TD[1];

    /* DMA Configuration for DMA_V */
    DMA_V_Chan = DMA_V_DmaInitialize(DMA_V_BYTES_PER_BURST, DMA_V_REQUEST_PER_BURST, 
        HI16(DMA_V_SRC_BASE), HI16(DMA_V_DST_BASE));
    DMA_V_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_V_TD[0], 2, DMA_INVALID_TD, DMA_V__TD_TERMOUT_EN | TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_V_TD[0], LO16((uint32)ADC_V_SAR_WRK0_PTR), LO16((uint32)&Volt_temp));
    CyDmaChSetInitialTd(DMA_V_Chan, DMA_V_TD[0]);
    CyDmaChEnable(DMA_V_Chan, 1);
}



/* [] END OF FILE */
