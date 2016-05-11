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
#include "Sensor.h"
uint16 RPM_temp = 0;
uint16 Current_temp = 0;
uint16 Volt_temp = 0;

CY_ISR_PROTO(RPM);
char RPM_reset=0;

void DMA_ADC_A_V_init();

CY_ISR(RPM)
{

    uint8 STATUS = Timer_RPM_STATUS;
    
    if((STATUS&(1<<1)))//capture
    {
        uint16 temp = Timer_RPM_ReadCapture();
        uint16 temp2 = 0;
        if(Timer_RPM_STATUS&(1<<3))
            temp2 = Timer_RPM_ReadCapture();
            
        
        RPM_reset=0;
        
        if(temp2 != 0)
        {
            RPM_temp = ((uint16)(1000000/(temp-temp2)));
        }
        else
        {
            RPM_temp=0;
        }
        
    }
    else if((STATUS&(1<<0)))//TC
    {
        
        if(RPM_reset)
        {
            RPM_temp=0;
        }
        RPM_reset=1;
        
    }
    Timer_RPM_WriteCounter(2000u);
    Timer_RPM_ClearFIFO();
}


void S_init()
{
    RPM_isr_StartEx(RPM);
    Timer_RPM_Start();
    DMA_ADC_A_V_init();
    Clock_4_Start();
}

void DMA_ADC_A_V_init()
{
    PGA_1_Start();
    
    ADC_A_Start();
    ADC_V_Start();
    ADC_A_StartConvert();
    ADC_V_StartConvert();
    
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
    CyDmaTdSetConfiguration(DMA_A_TD[0], 2, DMA_INVALID_TD, TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_A_TD[0], LO16((uint32)ADC_A_SAR_WRK0_PTR), LO16((uint32)&Current_temp));
    CyDmaChSetInitialTd(DMA_A_Chan, DMA_A_TD[0]);
    CyDmaChEnable(DMA_A_Chan, 1);

    
    // ------------------------------------------------------------------------------------------------------
    
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
    CyDmaTdSetConfiguration(DMA_V_TD[0], 2, DMA_INVALID_TD, TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_V_TD[0], LO16((uint32)ADC_V_SAR_WRK0_PTR), LO16((uint32)&Volt_temp));
    CyDmaChSetInitialTd(DMA_V_Chan, DMA_V_TD[0]);
    CyDmaChEnable(DMA_V_Chan, 1);
}

const uint16 * S_RPM_ptr()
{
    return &RPM_temp;  
}

const uint16 * S_Current_ptr()
{
    return &Current_temp;  
}
const uint16 * S_Volt_ptr()
{
    return &Volt_temp;  
}

/* [] END OF FILE */
