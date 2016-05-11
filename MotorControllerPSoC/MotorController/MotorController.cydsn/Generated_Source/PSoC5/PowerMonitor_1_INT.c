/*******************************************************************************
* File Name: PowerMonitor_1_INT.c
* Version 1.60
*
* Description:
*  This file contains the code that operates during the PowerMonitor interrupt
*  service routine.
*
* Note:
*
*******************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "PowerMonitor_1.h"
#include "PowerMonitor_1_PM_AMux_Voltage.h"
#include "PowerMonitor_1_PM_AMux_Current.h"
#include "PowerMonitor_1_ADC.h"
#include "cyapicallbacks.h"


/******************************************************************************
* Custom Declarations and Variables
* - add user inlcude files, prototypes and variables between the following
*   #START and #END tags
******************************************************************************/
/* `#START POWER_MONITOR_VAR`  */

/* `#END`  */


/*******************************************************************************
* Global variables
*******************************************************************************/

/*******************************************************************************
*   PowerMonitor_1_adcStartFalseCnt  - False adc count during filter init
*   PowerMonitor_1_PMoutEn           - Output signals enable
*   PowerMonitor_1_adcConvNow        - Conversion in progress
*   PowerMonitor_1_adcConvNext       - Next conversion
*   PowerMonitor_1_adcConvNextPreCal - Holds old Next before switching to
                                         calibration state
*   PowerMonitor_1_adcConvCalType    - Calibration type is in grogress
*   PowerMonitor_1_adcMaxChannels    - Calibration type is in grogress
*******************************************************************************/
volatile uint8 CYDATA PowerMonitor_1_adcStartFalseCnt;
volatile CYBIT PowerMonitor_1_PMoutEn;
volatile uint8 CYDATA PowerMonitor_1_adcConvNow;
volatile uint8 CYDATA PowerMonitor_1_adcConvNext;
volatile uint8 CYDATA PowerMonitor_1_adcConvNextPreCal;
volatile uint8 CYDATA PowerMonitor_1_adcConvCalType;
volatile uint8 CYDATA PowerMonitor_1_maxChannels;


/*******************************************************************************
*  Variables to hold the raw calibration values. The raw readings need to be
*  subsequently processed to yield the proper calibration adjustments.
*******************************************************************************/
/*                                                                 ~ VALUE, RANGE Description */
volatile int16 CYXDATA PowerMonitor_1_adcZeroCalRawCfg1;      /* ~     0, 2048 zero    */
volatile int16 CYXDATA PowerMonitor_1_adcGainCalRawCfg1;      /* ~ -1024, 2048 DSM Vref */
volatile int16 CYXDATA PowerMonitor_1_adcSCCalRawCfg1;        /* ~ -2048, 2048 PGA   */
volatile int16 CYXDATA PowerMonitor_1_adcZeroCalRawCfg2;      /* ~     0,   64 zero  */
volatile int16 CYXDATA PowerMonitor_1_adcGainCalRawCfg2a;
volatile int16 CYXDATA PowerMonitor_1_adcGainCalRawCfg2b;
volatile int16 CYXDATA PowerMonitor_1_adcZeroCalRawCfg3;      /* ~     0, 1024 zero    */
volatile int16 CYXDATA PowerMonitor_1_adcGainCalRawCfg3;      /* ~ -2048, 1024 DSM Vref */
volatile int16 CYXDATA PowerMonitor_1_adcSCCalRawCfg3;        /* ~ -2048, 1024 Vref   */
/*******************************************************************************
* PowerMonitor_1_ADC_RAWVOLTS_STRUCT - ADC votage reading structure
*                                    (one struct per channel)
*        rawVoltage[] = array of raw ADC readings. .
*******************************************************************************/
typedef struct
{
    /* Raw ADC samples      */
    volatile int16 rawVoltage[PowerMonitor_1_VOLTAGE_FILTER_SIZE];
} PowerMonitor_1_ADC_RAWVOLTS_STRUCT;

/* Array to hold ADC raw values of voltage channels of each converter */
PowerMonitor_1_ADC_RAWVOLTS_STRUCT CYXDATA PowerMonitor_1_voltFilt[PowerMonitor_1_NUMBER_OF_CONVERTERS];

/* Array to hold filtered ADC value for each voltage inputs */
PowerMonitor_1_ADC_CTL_STRUCT  CYXDATA PowerMonitor_1_voltCtl[PowerMonitor_1_NUMBER_OF_CONVERTERS];


#if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
/*******************************************************************************
* PowerMonitor_1_ADC_RAWCURRENT_STRUCT - ADC votage reading structure
*                                    (one struct per channel)
*        rawVoltage[] = array of raw ADC readings. .
*******************************************************************************/
typedef struct
{
    /* Raw ADC samples      */
    volatile int16 rawVoltage[PowerMonitor_1_CURRENT_FILTER_SIZE];
} PowerMonitor_1_ADC_RAWCURRENT_STRUCT;

/* Array to hold ADC raw values of current channels of each converter */
PowerMonitor_1_ADC_RAWCURRENT_STRUCT CYXDATA PowerMonitor_1_ampFilt[PowerMonitor_1_NUM_CURRENT_SOURCES];

/* Array to hold filtered ADC value for each current channels */
PowerMonitor_1_ADC_CTL_STRUCT  CYXDATA PowerMonitor_1_ampCtl[PowerMonitor_1_NUM_CURRENT_SOURCES];

#endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */


#if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)
/*******************************************************************************
* PowerMonitor_1_ADC_RAWAUXVOLTS_STRUCT - ADC votage reading structure
*                                    (one struct per channel)
*        rawVoltage[] = array of raw ADC readings. .
*******************************************************************************/
typedef struct
{
    /* Raw ADC samples      */
    volatile int16 rawVoltage[PowerMonitor_1_AUX_VOLTAGE_FILTER_SIZE];
} PowerMonitor_1_ADC_RAWAUXVOLTS_STRUCT;

/* Array to hold ADC raw values of aux voltage channels of each converter */
PowerMonitor_1_ADC_RAWAUXVOLTS_STRUCT CYXDATA PowerMonitor_1_auxVoltFilt[PowerMonitor_1_NUM_AUX_INPUTS];

/* Array to hold filtered ADC value for each aux inputs */
PowerMonitor_1_ADC_CTL_STRUCT  CYXDATA PowerMonitor_1_auxVoltCtl[PowerMonitor_1_NUM_AUX_INPUTS];
#endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0u) */

/* Supervisor_adcWarnWin[] = table of Min/Max values (scaled to filtered sum) */
PowerMonitor_1_WARNWIN_STRUCT CYXDATA PowerMonitor_1_warnWin [PowerMonitor_1_NUMBER_OF_CONVERTERS];
PowerMonitor_1_FAULTWIN_STRUCT CYXDATA PowerMonitor_1_faultWin [PowerMonitor_1_NUMBER_OF_CONVERTERS];

/* last voltage, current annd auxiliary voltage channels */
uint8 CYDATA PowerMonitor_1_lastVoltageChan;
uint8 CYDATA PowerMonitor_1_lastCurrentChan;
uint8 CYDATA PowerMonitor_1_lastAuxVoltChan;

/* Array to hold the pgood status */
#if (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_INDIVIDUAL)
    static uint8 CYPDATA PowerMonitor_1_faultStatus[PowerMonitor_1_NUMBER_OF_CONVERTERS];
#endif /* (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_INDIVIDUAL) */

#if ((PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) || (PowerMonitor_1_NUM_AUX_INPUTS > 0u))
    uint8 CYDATA PowerMonitor_1_index = 0u;
#endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */

#if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)
    uint8 CYDATA PowerMonitor_1_auxIndex = 0u;
    int16 PowerMonitor_1_auxStore;
#endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0u) */

/* variables to store the DSM routing registers */
static uint8 PowerMonitor_1_keep_CYREG_DSM0_SW0; /* vp_ag[7:0] */
static uint8 PowerMonitor_1_keep_CYREG_DSM0_SW2; /* vp_abus[2],[0] */
/* _[vn_vssa][vn_vref]_ _[vp_vssa]_[vp_amx] */
static uint8 PowerMonitor_1_keep_CYREG_DSM0_SW3;
static uint8 PowerMonitor_1_keep_CYREG_DSM0_SW4; /* vn_ag[7,5,3,1] */
static uint8 PowerMonitor_1_keep_CYREG_DSM0_SW6; /* vn_abus[3,1] */


/*******************************************************************************
* ADC_CONFIGURATION macro - Ensures the desired ADC Configuration is selected
*
* ADC_CONFIG_PUSH_NEW(newConfig) - Set new config and copy as being pushed
* ADC_CONFIG_POP     ()          - Pop prior pushed config
* ADC_CONFIG_PUSH_OLD(newConfig) - Push old config and set a new config
*
*******************************************************************************/
static uint8 CYDATA PowerMonitor_1_adcConfig = 0xffu;
static uint8 CYDATA PowerMonitor_1_adcConvDoneConfig = 0xffu;
static uint8 CYDATA PowerMonitor_1_adcConfigCalPush;

#if(PowerMonitor_1_SINGLE_ENDED_INPUT_MODE == PowerMonitor_1__ADC_DELSIG__BYPASS_BUFFER)
    #define PowerMonitor_1_ADC_CONFIG_SET(newConfig)                                     \
        do {                                                                               \
            if (PowerMonitor_1_adcConfig != newConfig)                                   \
            {                                                                              \
                PowerMonitor_1_adcConfig = newConfig;                                    \
                PowerMonitor_1_ADC_SelectConfiguration(newConfig, 0u);                   \
                if( (newConfig == PowerMonitor_1_SE_VOLTAGE_RANGE) || (newConfig == PowerMonitor_1_RANGE_2048) ) \
                {                                                                                                    \
                    PowerMonitor_1_ADC_DSM_BUF0_REG    = PowerMonitor_1_ADC_DSM_BYPASS_P;                        \
                    PowerMonitor_1_ADC_DSM_BUF1_REG    = PowerMonitor_1_ADC_DSM_BYPASS_N;                        \
                    PowerMonitor_1_ADC_DSM_BUF2_REG    = 0u;                                                       \
                    PowerMonitor_1_ADC_DSM_BUF3_REG    = 0u;                                                       \
                }                                                                                                    \
                CyIntSetVector(PowerMonitor_1_IRQ__INTC_NUMBER, PowerMonitor_1_ISR );  \
                PowerMonitor_1_ADC_Start();                                              \
            }                                                                              \
        } while(0)
#else  /* PowerMonitor_1_SINGLE_ENDED_INPUT_MODE == PowerMonitor_1__ADC_DELSIG__LEVEL_SHIFT */
    #define PowerMonitor_1_ADC_CONFIG_SET(newConfig)                                     \
        do {                                                                               \
            if (PowerMonitor_1_adcConfig != newConfig)                                   \
            {                                                                              \
                PowerMonitor_1_adcConfig = newConfig;                                    \
                PowerMonitor_1_ADC_SelectConfiguration(newConfig, 0u);                   \
                CyIntSetVector(PowerMonitor_1_IRQ__INTC_NUMBER, PowerMonitor_1_ISR );  \
                PowerMonitor_1_ADC_Start();                                              \
            }                                                                              \
        } while(0)
#endif /* PowerMonitor_1_SINGLE_ENDED_INPUT_MODE == PowerMonitor_1__ADC_DELSIG__BYPASS_BUFFER */
#define PowerMonitor_1_ADC_CONFIG_PUSH_NEW(newConfig)                  \
    do {                                                                 \
        PowerMonitor_1_ADC_CONFIG_SET(newConfig);                      \
        PowerMonitor_1_adcConfigCalPush = PowerMonitor_1_adcConfig;  \
        PowerMonitor_1_adcCfgPopPend = PowerMonitor_1_CYTRUE;        \
    } while (0)

#define PowerMonitor_1_ADC_CONFIG_POP()                                   \
    do {                                                                    \
        PowerMonitor_1_ADC_CONFIG_SET(PowerMonitor_1_adcConfigCalPush); \
        PowerMonitor_1_adcCfgPopPend = PowerMonitor_1_CYFALSE;          \
    } while(0)

#define PowerMonitor_1_ADC_CONFIG_PUSH_OLD(newConfig)                  \
    do {                                                                 \
        PowerMonitor_1_adcConfigCalPush = PowerMonitor_1_adcConfig;  \
        PowerMonitor_1_ADC_CONFIG_SET(newConfig);                      \
        PowerMonitor_1_adcCfgPopPend = PowerMonitor_1_CYTRUE;        \
    } while(0)


/*******************************************************************************
* Used when punctuated "calibration" didn't really mess with the analog
* switch routing (like when it's the first IRQ after reset).
*******************************************************************************/
static CYBIT PowerMonitor_1_justReset;
/* AMUX Channel select variable */
static uint8 CYDATA PowerMonitor_1_amuxChan;
/* Bit variable to decide whether to restore the DSM registers or not */
static CYBIT PowerMonitor_1_dsmRegRestorePend = PowerMonitor_1_CYFALSE;
static CYBIT PowerMonitor_1_adcCfgPopPend     = PowerMonitor_1_CYFALSE;

#if (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_GLOBAL)
    static CYBIT PowerMonitor_1_faultDetected;
#endif /* (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_GLOBAL) */
volatile int16 CYXDATA * CYDATA PowerMonitor_1_pCalStore = 0u;


/******************************************************************************
*    Local APIs
******************************************************************************/
static void PowerMonitor_1_SaveDsmRegsAndIsolate   (void) \
                           ;
static void PowerMonitor_1_RestoreDsmRegs          (void) \
                           ;
static uint8 PowerMonitor_1_GetMaxFilterOrder      (void) \
                           ;

/*******************************************************************************
* Function Name: Supervisor_AdcIsrStart()
********************************************************************************
*
* Summary:
*  Set-up and begin the autonomous ADC ISR thread. Starts the ADC operation by
*  initially setting the software interrupt and excecuting the ISR routine.
*
* Parameters:
*  None
*
* Return:
*  None.
*
*******************************************************************************/
void PowerMonitor_1_IsrStart(void) 
{
    /* Disable and set the interrupt priority */
    PowerMonitor_1_IntDisable      (PowerMonitor_1_IRQ__INTC_NUMBER);
    CyIntSetPriority  (PowerMonitor_1_IRQ__INTC_NUMBER, PowerMonitor_1_IRQ_INTC_PRIOR_NUMBER);

    /* Initialize the variables with values */
    PowerMonitor_1_adcStartFalseCnt   = PowerMonitor_1_GetMaxFilterOrder();
    PowerMonitor_1_PMoutEn            = PowerMonitor_1_CYFALSE;
    PowerMonitor_1_adcConvNow         = PowerMonitor_1_STATE_CAL;
    PowerMonitor_1_adcConvNext        = PowerMonitor_1_STATE_CAL;
    PowerMonitor_1_adcConvCalType     = PowerMonitor_1_CAL_START;

    /* Setting the interrupt vecotr for software interrupt and enabling it */
    CyIntSetVector(PowerMonitor_1_IRQ__INTC_NUMBER, PowerMonitor_1_ISR);
    CyIntSetPending   (PowerMonitor_1_IRQ__INTC_NUMBER);
    PowerMonitor_1_IntEnable       (PowerMonitor_1_IRQ__INTC_NUMBER);
}


/*******************************************************************************
* Function Name: Set_First_Channel()
********************************************************************************
*
* Summary:
*  Set analog multiplexer and configure ADC for the first channel.
*
* Parameters:
*  None
*
* Return:
*  None.
*
*******************************************************************************/
static void PowerMonitor_1_Set_First_Channel(void) 
{
    PowerMonitor_1_adcConvNow = PowerMonitor_1_ISR_STATE_0;
    PowerMonitor_1_adcConvNext = PowerMonitor_1_ISR_STATE_0 + 1;
    /* Update AmuxChan flag */
    PowerMonitor_1_amuxChan = PowerMonitor_1_adcConvNow;
    /* Select the proper AMux Channel */
    PowerMonitor_1_PM_AMux_Voltage_FastSelect(PowerMonitor_1_amuxChan);

    if (PowerMonitor_1_VoltageType[0] == PowerMonitor_1_VOLTAGE_TYPE_SINGLE)
    {
        PowerMonitor_1_PM_AMux_Current_FastSelect(PowerMonitor_1_PGA_OUT_CHAN);
        PowerMonitor_1_ADC_CONFIG_PUSH_NEW(PowerMonitor_1_SE_VOLTAGE_RANGE);
    }
    else
    {
        PowerMonitor_1_PM_AMux_Current_FastSelect(PowerMonitor_1_amuxChan);
        PowerMonitor_1_ADC_CONFIG_PUSH_NEW(PowerMonitor_1_RANGE_2048);
    }
}


#if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)

    /*******************************************************************************
    * Function Name: Set_Aux_Channel()
    ********************************************************************************
    *
    * Summary:
    *  Set analog multiplexer and configure ADC for the auxiliary channel.
    *
    * Parameters:
    *  sampleMode: Specifies the sample mode
    *              0 - Continuous, 1 - On Demand.
    *      For the On Demand mode, PowerMonitor_1_auxIndex variable should be
    *      configured with auxilary channel number.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    static void PowerMonitor_1_Set_Aux_Channel(uint8 sampleMode) 
    {
        if(sampleMode == PowerMonitor_1_CONTINUOUS)
        {
            /* Check to see whether auxiliary channel is enabled or not. If it is
            *  disabled skip auxiliary channels.
            */
            while (((PowerMonitor_1_auxEnableMask >> PowerMonitor_1_index) & 1u) == 0u)
            {
                PowerMonitor_1_index++;
                PowerMonitor_1_adcConvNow++;
                PowerMonitor_1_adcConvNext++;
                if(PowerMonitor_1_index >= PowerMonitor_1_NUM_AUX_INPUTS)
                {
                    break;
                }
            }
            PowerMonitor_1_auxIndex = PowerMonitor_1_index;
        }
        if(PowerMonitor_1_auxIndex < PowerMonitor_1_NUM_AUX_INPUTS)
        {
            /* Update AmuxChan flag */
            PowerMonitor_1_amuxChan = PowerMonitor_1_AUX_IN_CHAN + PowerMonitor_1_auxIndex;

            PowerMonitor_1_PM_AMux_Voltage_FastSelect(PowerMonitor_1_amuxChan);

            if (PowerMonitor_1_AuxVoltageType[PowerMonitor_1_auxIndex] == PowerMonitor_1_AUX_VOLTAGE_SINGLE)
            {
                PowerMonitor_1_PM_AMux_Current_FastSelect(PowerMonitor_1_PGA_OUT_CHAN);
            }
            else
            {
                PowerMonitor_1_PM_AMux_Current_FastSelect(PowerMonitor_1_amuxChan);
            }
            /* Set the proper ADC configuration */
            if((PowerMonitor_1_AuxVoltageType[PowerMonitor_1_auxIndex] == PowerMonitor_1_AUX_VOLTAGE_64MV_DIFF) ||
               (PowerMonitor_1_AuxVoltageType[PowerMonitor_1_auxIndex] == PowerMonitor_1_AUX_VOLTAGE_128MV_DIFF))
            {
                if(sampleMode == PowerMonitor_1_CONTINUOUS)
                {
                    PowerMonitor_1_ADC_CONFIG_PUSH_NEW(PowerMonitor_1_RANGE_LOW);
                }
                else /* need to restore configuration for On Demand mode */
                {
                    PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_LOW);
                }
            }
            else
            {
                if (PowerMonitor_1_AuxVoltageType[PowerMonitor_1_auxIndex] == PowerMonitor_1_AUX_VOLTAGE_SINGLE)
                {
                    if(sampleMode == PowerMonitor_1_CONTINUOUS)
                    {
                        PowerMonitor_1_ADC_CONFIG_PUSH_NEW(PowerMonitor_1_SE_VOLTAGE_RANGE);
                    }
                    else /* need to restore configuration for On Demand mode */
                    {
                        PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_SE_VOLTAGE_RANGE);
                    }
                }
                else
                {
                    if(sampleMode == PowerMonitor_1_CONTINUOUS)
                    {
                        PowerMonitor_1_ADC_CONFIG_PUSH_NEW(PowerMonitor_1_RANGE_2048);
                    }
                    else /* need to restore configuration for On Demand mode */
                    {
                        PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_2048);
                    }
                }
            }
        }
        else /* set firts channel if there are no more axiliary channel available */
        {
            PowerMonitor_1_Set_First_Channel();
        }
    }
#endif  /* PowerMonitor_1_NUM_AUX_INPUTS > 0u */

#if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0)
    /*******************************************************************************
    * Function Name: Set_Current_Channel()
    ********************************************************************************
    *
    * Summary:
    *  Set analog multiplexer and configure ADC for the current channel.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    static void PowerMonitor_1_Set_Current_Channel(void) 
    {
        /* Check to see whether current type is differential or not. If it is
        *  differential skip amux channels.
        */
        while (PowerMonitor_1_CurrentType[PowerMonitor_1_index] == PowerMonitor_1_CURRENT_TYPE_NA)
        {
            PowerMonitor_1_index++;
        }

        /* Set the proper ADC cofiguration */
        if (PowerMonitor_1_CurrentType[PowerMonitor_1_index] == PowerMonitor_1_CURRENT_TYPE_DIRECT)
        {
            /* Update AmuxChan flag */
            PowerMonitor_1_amuxChan = PowerMonitor_1_index;
            /* Select the proper AMux channels */
            PowerMonitor_1_PM_AMux_Voltage_FastSelect(PowerMonitor_1_amuxChan);
            PowerMonitor_1_PM_AMux_Current_FastSelect(PowerMonitor_1_amuxChan);
            /* Change the ADC configuration */
            PowerMonitor_1_ADC_CONFIG_PUSH_NEW(PowerMonitor_1_RANGE_LOW);
        }
        else /* CSA type */
        {
            /* Update AmuxChan flag */
            PowerMonitor_1_amuxChan = PowerMonitor_1_CSA_IN_CHAN + PowerMonitor_1_index;
            /* Select the proper AMux channels */
            PowerMonitor_1_PM_AMux_Voltage_FastSelect(PowerMonitor_1_amuxChan);
            PowerMonitor_1_PM_AMux_Current_FastSelect(PowerMonitor_1_PGA_OUT_CHAN);
            /* Change the ADC configuration */
            PowerMonitor_1_ADC_CONFIG_PUSH_NEW(PowerMonitor_1_SE_VOLTAGE_RANGE);
        }
    }
#endif /* PowerMonitor_1_NUM_CURRENT_SOURCES > 0 */


/*****************************************************************************
* Function Name: PowerMonitor_1_ISR
******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Reentrant:
*  No
*
*****************************************************************************/
CY_ISR(PowerMonitor_1_ISR)
{
    #if(CY_PSOC3)
        uint8 int_en = EA;
    #endif /* (CY_PSOC3) */

    int16 CYDATA rawVal;
    uint8 CYDATA convDone;
    int32 rawFiltVal = 0;
    #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
        uint8 CYDATA iChanNum;
    #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */

    #if(CY_PSOC3)
        /* Enable the global interrupts */
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

    #ifdef PowerMonitor_1_ISR_ENTRY_CALLBACK
        PowerMonitor_1_ISR_EntryCallback();
    #endif /* PowerMonitor_1_ISR_ENTRY_CALLBACK */


    /************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    *************************************************************************/
    /* `#START BEGIN_POWER_MONITOR_ISR`  */

    /* `#END`  */

    /* Collect the raw reading */
    rawVal = PowerMonitor_1_ADC_GetResult16();

    /* Update the state of flags */
    convDone = PowerMonitor_1_adcConvNow;
    PowerMonitor_1_adcConvNow = PowerMonitor_1_adcConvNext;
    PowerMonitor_1_adcConvNext++;
    PowerMonitor_1_adcConvDoneConfig = PowerMonitor_1_adcConfig;

    /* If DSM registers need restoring, do it BEFORE any FastSelects() */
    if (PowerMonitor_1_dsmRegRestorePend == PowerMonitor_1_CYTRUE)
    {
        PowerMonitor_1_RestoreDsmRegs();
    }

    /* If ADC configuration needs restoring, do it before proceeding */
    if (PowerMonitor_1_adcCfgPopPend == PowerMonitor_1_CYTRUE)
    {
        PowerMonitor_1_ADC_CONFIG_POP();
    }

    /* Based on the new adc reading, setup either special adc
    *  configuation/routing for next reading or just take the next
    *  sequential measurement
    */
    if (PowerMonitor_1_adcConvNow < PowerMonitor_1_lastVoltageChan)
    {
        PowerMonitor_1_PM_AMux_Voltage_FastSelect(PowerMonitor_1_adcConvNow);
        /* Update AmuxChan flag */
        if (PowerMonitor_1_VoltageType[PowerMonitor_1_adcConvNow] == PowerMonitor_1_VOLTAGE_TYPE_DIFF)
        {
            PowerMonitor_1_PM_AMux_Current_FastSelect(PowerMonitor_1_adcConvNow);
            PowerMonitor_1_ADC_CONFIG_PUSH_NEW(PowerMonitor_1_RANGE_2048);
        }
        else
        {
            PowerMonitor_1_PM_AMux_Current_FastSelect(PowerMonitor_1_PGA_OUT_CHAN);
            PowerMonitor_1_ADC_CONFIG_PUSH_NEW(PowerMonitor_1_SE_VOLTAGE_RANGE);
        }
    }
    else if (PowerMonitor_1_adcConvNow == PowerMonitor_1_lastVoltageChan)
    {
        #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0)
             /* --- Set first current channel after last voltage channel --- */
            PowerMonitor_1_index = 0u;
            PowerMonitor_1_Set_Current_Channel();
        #elif (PowerMonitor_1_NUM_AUX_INPUTS > 0)
             /* --- Set first auxiliary channel after last voltage channel --- */
            PowerMonitor_1_index = 0u;
            PowerMonitor_1_Set_Aux_Channel(PowerMonitor_1_CONTINUOUS);
        #else
            /* --- Set first voltage channel after last voltage channel  --- */
            PowerMonitor_1_Set_First_Channel();
        #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0) */
    }
    #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0)
        else if (PowerMonitor_1_adcConvNow == PowerMonitor_1_lastCurrentChan)
        {
            #if (PowerMonitor_1_NUM_AUX_INPUTS > 0)
                 /* --- Set auxiliary channel after last current channel --- */
                PowerMonitor_1_index = 0u;
                PowerMonitor_1_Set_Aux_Channel(PowerMonitor_1_CONTINUOUS);
            #else
                /* --- Set first voltage channel after last current channel  --- */
                PowerMonitor_1_Set_First_Channel();
            #endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0) */
        }
        else if ((PowerMonitor_1_adcConvNow > PowerMonitor_1_lastVoltageChan) &&
                 (PowerMonitor_1_adcConvNow < PowerMonitor_1_lastCurrentChan))
        {
            PowerMonitor_1_index++;
            PowerMonitor_1_Set_Current_Channel();
        }
    #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCE > 0) */
    #if (PowerMonitor_1_NUM_AUX_INPUTS > 0)
        else if (PowerMonitor_1_adcConvNow == PowerMonitor_1_lastAuxVoltChan)
        {
             /* --- Set first voltage channel after last auxiliary channel  --- */
            PowerMonitor_1_Set_First_Channel();
        }

        else if ((PowerMonitor_1_adcConvNow > PowerMonitor_1_lastCurrentChan) &&
                 (PowerMonitor_1_adcConvNow < PowerMonitor_1_lastAuxVoltChan))
        {
            PowerMonitor_1_index++;
            PowerMonitor_1_Set_Aux_Channel(PowerMonitor_1_CONTINUOUS);
        }
        else if (PowerMonitor_1_adcConvNow == PowerMonitor_1_STATE_MEASURE_AUX)
        {
            /*  Next state will be pre on demand next state */
            PowerMonitor_1_adcConvNext = PowerMonitor_1_adcConvNextPreCal;
            /* Out of order auxiliary channel measurement */
            PowerMonitor_1_Set_Aux_Channel(PowerMonitor_1_ON_DEMAND);
        }
    #endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0) */
    else if (PowerMonitor_1_adcConvNow == PowerMonitor_1_STATE_CAL)
    {
        /***********************************************************************
        * Going to a TBD Calibration state
        *
        * Skip AMux, routes will be manually set
        *  Next state will be pre-calibration next state
        ***********************************************************************/
        PowerMonitor_1_adcConvNext = PowerMonitor_1_adcConvNextPreCal;

        /***********************************************************************
        * Configure for the specific calibration operation.
        * NOTE: at entry DSM regs MUST BE setup as normal AMux sampling
        ***********************************************************************/
        switch (PowerMonitor_1_adcConvCalType)
        {
            case PowerMonitor_1_CAL_START:
                /*******************************************************************
                * Startup ADC, only after firmware reset
                *******************************************************************/
                PowerMonitor_1_ADC_Start();
                PowerMonitor_1_Set_First_Channel();
                 /* Initialize local selection memory    */
                PowerMonitor_1_adcConvDoneConfig =PowerMonitor_1_adcConfig;

                /* Don't "restore" DSM regs, not real CAL */
                PowerMonitor_1_justReset = PowerMonitor_1_CYTRUE;
                #if (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_GLOBAL)
                    PowerMonitor_1_faultDetected = PowerMonitor_1_CYFALSE;
                #endif /* (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_GLOBAL) */
                break;

            case PowerMonitor_1_CAL_CFG1Z:
                /*******************************************************************
                * +/- 2048mV : Calibrate ZERO, VP = VN = Vssa
                *******************************************************************/
                PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_2048);
                PowerMonitor_1_SaveDsmRegsAndIsolate();
                /* vn_vssa & vp_vssa    */
                PowerMonitor_1_ADC_DSM_SW3_REG = PowerMonitor_1_ADC_VN_VSSA_VP_VSSA;
                PowerMonitor_1_pCalStore = &PowerMonitor_1_adcZeroCalRawCfg1;
                break;

            case PowerMonitor_1_CAL_CFG1G:
                /*******************************************************************
                * +/- 2048mV : Calibrate GAIN, VP = Vssa, VN = Vref
                *******************************************************************/
                PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_2048);
                PowerMonitor_1_SaveDsmRegsAndIsolate();
                /* vn_vref & vp_vssa  */
                PowerMonitor_1_ADC_DSM_SW3_REG = PowerMonitor_1_ADC_VN_VREF_VP_VSSA;
                /*******************************************************************
                * Either use REFBUF1 by enabling it or bypass by using SW12
                *   cf. "Reference Options" in TRM.
                *  SW12 avoids introducing potential REFBUF1 offset error.
                *******************************************************************/
                /* S12 ON so can read Vref */
                PowerMonitor_1_ADC_DSM_REF3_REG |= PowerMonitor_1_ADC_S12_ON;
                PowerMonitor_1_pCalStore = &PowerMonitor_1_adcGainCalRawCfg1;
                break;

            case PowerMonitor_1_CAL_PGA1Z:
                /*******************************************************************
                * PGAx2 : Calibrate PGA offset, VP = Vssa, VN = PGA
                *******************************************************************/
                PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_2048);
                PowerMonitor_1_SaveDsmRegsAndIsolate();
                /* vp_vssa              */
                PowerMonitor_1_ADC_DSM_SW3_REG = PowerMonitor_1_ADC_VP_VSSA;
                /* VN = PGA             */
                /* adc vn to abus1      */
                PowerMonitor_1_ADC_DSM_SW6_REG = PowerMonitor_1_ADC_VN_ABUS1;
                /* pga out to abus1     */
                PowerMonitor_1_PGA_SW10_REG = PowerMonitor_1_PGA_OUT_ABUS1;
                PowerMonitor_1_pCalStore = &PowerMonitor_1_adcSCCalRawCfg1;
                break;

            case PowerMonitor_1_CAL_CFG2Z:
                /***************************************************************
                * +/- LOW mV : Calibrate ZERO, VP = Vssa, VN = Vssa
                ***************************************************************/
                PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_LOW);
                PowerMonitor_1_SaveDsmRegsAndIsolate();
                /* vn_vssa + vp_vssa    */
                PowerMonitor_1_ADC_DSM_SW3_REG = PowerMonitor_1_ADC_VN_VSSA_VP_VSSA;
                PowerMonitor_1_pCalStore = &PowerMonitor_1_adcZeroCalRawCfg2;
                break;

            #if (PowerMonitor_1_CAL_PIN_EXPOSED)
            case PowerMonitor_1_CAL_CFG2Ga:
                /*******************************************************************
                * Measure cal input using DelSig +/- 1024 mV range
                *   Route: cal input to +ve of ADC and connect GND to -ve input
                *******************************************************************/
                PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_1024);
                PowerMonitor_1_PM_AMux_Voltage_FastSelect(PowerMonitor_1_CAL_IN_CHAN);
                PowerMonitor_1_PM_AMux_Current_FastSelect(PowerMonitor_1_CAL_IN_CHAN);
                PowerMonitor_1_pCalStore = &PowerMonitor_1_adcGainCalRawCfg2a;
                break;

            case PowerMonitor_1_CAL_CFG2Gb:
                /*******************************************************************
                * Measure cal input using DelSig +/- LOW mV range
                *   Route: cal input to +ve of ADC, vssa to -ve of ADC
                *******************************************************************/
                PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_LOW);
                PowerMonitor_1_PM_AMux_Voltage_FastSelect(PowerMonitor_1_CAL_IN_CHAN);
                PowerMonitor_1_PM_AMux_Current_FastSelect(PowerMonitor_1_CAL_IN_CHAN);
                PowerMonitor_1_pCalStore = &PowerMonitor_1_adcGainCalRawCfg2b;
                break;
            #endif /* PowerMonitor_1_CAL_PIN_EXPOSED */

            case PowerMonitor_1_CAL_CFG3Z:
                /*******************************************************************
                * +/- 1024mV : Calibrate ZERO, VP = Vssa, VN = Vssa
                *******************************************************************/
                PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_1024);
                PowerMonitor_1_SaveDsmRegsAndIsolate();
                /* vn_vssa + vp_vssa    */
                PowerMonitor_1_ADC_DSM_SW3_REG = PowerMonitor_1_ADC_VN_VSSA_VP_VSSA;
                PowerMonitor_1_pCalStore = &PowerMonitor_1_adcZeroCalRawCfg3;
                break;

            case PowerMonitor_1_CAL_CFG3G:
                /*******************************************************************
                * +/- 1024mV : Calibrate GAIN, VP = Vssa, VN = Vref
                *******************************************************************/
                PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_1024);
                PowerMonitor_1_SaveDsmRegsAndIsolate();
                /* vn_vref + vp_vssa  */
                PowerMonitor_1_ADC_DSM_SW3_REG = PowerMonitor_1_ADC_VN_VREF_VP_VSSA;
                /* S12 ON so can read Vref */
                PowerMonitor_1_ADC_DSM_REF3_REG |= PowerMonitor_1_ADC_S12_ON;
                PowerMonitor_1_pCalStore = &PowerMonitor_1_adcGainCalRawCfg3;
                break;

            case PowerMonitor_1_CAL_PGA3Z:
                /*******************************************************************
                * Ref : Calibrate ref offset, VP = Vssa, VN = ABUSL_1
                *******************************************************************/
                PowerMonitor_1_ADC_CONFIG_PUSH_OLD(PowerMonitor_1_RANGE_1024);
                PowerMonitor_1_SaveDsmRegsAndIsolate();
                /* vp_vssa              */
                PowerMonitor_1_ADC_DSM_SW3_REG = PowerMonitor_1_ADC_VP_VSSA;
                /* VN = PGA             */
                /* adc vn to abus1      */
                PowerMonitor_1_ADC_DSM_SW6_REG = PowerMonitor_1_ADC_VN_ABUS1;
                /* pga out to abus1     */
                PowerMonitor_1_PGA_SW10_REG = PowerMonitor_1_PGA_OUT_ABUS1;
                PowerMonitor_1_pCalStore = &PowerMonitor_1_adcSCCalRawCfg3;
                break;
            default:
                break;
        }
    }
    else
    {
        /* Do nothing */
    }

    /* Start the ADC conversion */
    PowerMonitor_1_ADC_StartConvert();

    /* Save the raw ADC readings in its proper destination */
    if (convDone != PowerMonitor_1_STATE_CAL)
    {
        /***********************************************************************
        * "Normal measurements" are adjusted for "zero" here.
        * Config 2 measurements correct for ROUTE INVERSION here.
        *       These adjustments are applied here because these values are
        *       used in this ISR for trimming and OV/UV/OC/UC testing.
        ***********************************************************************/
        if (PowerMonitor_1_adcConvDoneConfig == PowerMonitor_1_RANGE_LOW)
        {
            if (convDone < PowerMonitor_1_lastCurrentChan)
            {
                rawVal  = -rawVal;                  /* Fix route inversion  */
            }
            rawVal -= PowerMonitor_1_adcZeroCfg2;   /* Fix zero offset      */
        }
        else
        {
            /* Fix any zero offset */
            if (PowerMonitor_1_adcConvDoneConfig == PowerMonitor_1_RANGE_2048)
            {
                rawVal -= PowerMonitor_1_adcZeroCfg1;
            }
            else
            {
                rawVal -= PowerMonitor_1_adcZeroCfg3;
            }
        }

        if(convDone < PowerMonitor_1_MAX_CHANNELS)
        {
            /* ADC reading is a normal voltage or current sample */
            uint8 CYDATA Idx = 0u;
            volatile int16 CYXDATA * CYDATA pAdcFilt;
            PowerMonitor_1_ADC_CTL_STRUCT CYXDATA * CYDATA psCtl;

            /* initialize the pointers */
            pAdcFilt = &PowerMonitor_1_voltFilt[0u].rawVoltage[Idx];
            psCtl = &PowerMonitor_1_voltCtl[0u];

            if (convDone < PowerMonitor_1_lastVoltageChan)
            {
                psCtl = &PowerMonitor_1_voltCtl[convDone];
                Idx = psCtl->idx;
                pAdcFilt = &PowerMonitor_1_voltFilt[convDone].rawVoltage[Idx];
                #if (PowerMonitor_1_DEFAULT_VFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE)
                    psCtl->idx = 0u;
                #else
                    psCtl->idx = (Idx < PowerMonitor_1_VOLTAGE_FILTER_SIZE-1u) ? (Idx+1u) : 0u;
                #endif /* (PowerMonitor_1_DEFAULT_VFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE) */
            }
            #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
                else if ((convDone >= PowerMonitor_1_lastVoltageChan) &&
                         (convDone < PowerMonitor_1_lastCurrentChan))
                {
                    psCtl = &PowerMonitor_1_ampCtl[convDone - PowerMonitor_1_lastVoltageChan];
                    Idx = psCtl->idx;
                    pAdcFilt = &PowerMonitor_1_ampFilt[convDone - PowerMonitor_1_lastVoltageChan].rawVoltage[Idx];
                    #if (PowerMonitor_1_DEFAULT_CFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE)
                        psCtl->idx = 0u;
                    #else
                        psCtl->idx = (Idx < PowerMonitor_1_CURRENT_FILTER_SIZE-1u) ? (Idx+1u) : 0u;
                    #endif /* (PowerMonitor_1_DEFAULT_CFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE) */
                }
            #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */
            #if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)
                else
                {
                    psCtl = &PowerMonitor_1_auxVoltCtl[convDone - PowerMonitor_1_lastCurrentChan];
                    Idx = psCtl->idx;
                    pAdcFilt = &PowerMonitor_1_auxVoltFilt[convDone-PowerMonitor_1_lastCurrentChan].rawVoltage[Idx];
                    #if (PowerMonitor_1_DEFAULT_AUX_FILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE)
                        psCtl->idx = 0u;
                    #else
                        psCtl->idx = (Idx < PowerMonitor_1_AUX_VOLTAGE_FILTER_SIZE - 1u) ? (Idx + 1u) : 0u;
                    #endif /* (PowerMonitor_1_DEFAULT_AUX_FILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE) */
                }
            #endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0u) */

            /***********************************************************************
            * Update filter summation (subtract old value and add new value).
            ***********************************************************************/
            rawFiltVal  = psCtl->filtVal;
            rawFiltVal -= *pAdcFilt;
            rawFiltVal += rawVal;
            psCtl->filtVal = rawFiltVal;

            /***********************************************************************
            * Finally, store new value in history buffer
            ***********************************************************************/
            *pAdcFilt = rawVal;
        }
        #if (PowerMonitor_1_NUM_AUX_INPUTS > 0)
            else if(convDone == PowerMonitor_1_STATE_MEASURE_AUX)
            {
                /* Store raw DelSig reading with zero calibration in appropriate Auxiliary location */
                PowerMonitor_1_auxStore = rawVal;
            }
        #endif /* PowerMonitor_1_NUM_AUX_INPUTS > 0 */
        else
        {
            /* No action */
        }
    }
    else  /* convDone == PowerMonitor_1_STATE_CAL */
    {
        /*
        * Calibration measurement (these DO NOT have any corrections applied)
        *  so the non-ISR calibration process sees pure raw readings.
        */
        if (!PowerMonitor_1_justReset)
        {
            /* Store raw DelSig reading in appropriate Calibration location */
            *PowerMonitor_1_pCalStore = rawVal;
        }
        else
        {
            PowerMonitor_1_justReset = PowerMonitor_1_CYFALSE;
        }
    }

    /***********************************************************************
        * Generate the Fault, Warn and Pgood and EOC output signals
        * Also, update the OV, OC and UV status bits and OV, UV and OC source
        * status bits
    ************************************************************************/

    /* Generate pgood, warn and fault signals */
    /* Conv done is for current and voltage channels */
    if(PowerMonitor_1_PMoutEn == PowerMonitor_1_CYTRUE)
    {
        if (convDone < PowerMonitor_1_lastCurrentChan)
        {
            if (PowerMonitor_1_warnEnable == PowerMonitor_1_CYTRUE)
            {
                if (convDone < PowerMonitor_1_lastVoltageChan) /* for voltage channel */
                {
                    if (PowerMonitor_1_warnMask & (1ul << convDone))
                    {
                        if (PowerMonitor_1_warnSources & PowerMonitor_1_OV_WARN_SOURCE_MASK)
                        {
                              if (rawFiltVal > PowerMonitor_1_warnWin[convDone].OVWarnThrshldCounts )
                            {
                                /* Write the status to status bits */
                                PowerMonitor_1_warnSourceStatus |= PowerMonitor_1_ENABLE_OV_WARN_SOURCE;
                                PowerMonitor_1_OVWarnStatus |= (1ul << convDone);
                                #if !defined( \
                                PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
                                    /* Write the hardware signal */
                                    PowerMonitor_1_CONTROL1_REG |= PowerMonitor_1_WARN_MASK;
                                #endif /* B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED */
                            }
                        }

                        if (PowerMonitor_1_warnSources & PowerMonitor_1_UV_WARN_SOURCE_MASK)
                        {
                            if (rawFiltVal < PowerMonitor_1_warnWin[convDone].UVWarnThrshldCounts )
                            {
                                /* Write the status to status bits */
                                PowerMonitor_1_warnSourceStatus |= PowerMonitor_1_ENABLE_UV_WARN_SOURCE;
                                PowerMonitor_1_UVWarnStatus |= (1ul << convDone);
                                #if !defined( \
                                PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
                                    /* Write the hardware signal */
                                    PowerMonitor_1_CONTROL1_REG |= PowerMonitor_1_WARN_MASK;
                                #endif /* B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED */
                            }
                        }
                    }
                }
                #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
                    else
                    {
                        /* Get the index */
                        iChanNum = convDone - PowerMonitor_1_lastVoltageChan;
                        /* Get the actual current channel number */
                        iChanNum = PowerMonitor_1_ActiveCurrentChan[iChanNum];
                        /* Converter number is not zero based. So left shift count should
                           be converter number - 1 */
                        if (PowerMonitor_1_warnMask & (1ul << (iChanNum - 1u)))
                        {
                            if (PowerMonitor_1_warnSources & PowerMonitor_1_OC_WARN_SOURCE_MASK)
                            {
                                if (rawFiltVal > PowerMonitor_1_warnWin[iChanNum - 1u].OCWarnThrshldCounts )
                                {
                                    /* Write the status to status bits */
                                    PowerMonitor_1_warnSourceStatus |= PowerMonitor_1_ENABLE_OC_WARN_SOURCE;
                                    PowerMonitor_1_OCWarnStatus |= (1ul << (iChanNum - 1u));
                                    #if !defined(\
                                    PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
                                        /* Write the hardware signal */
                                        PowerMonitor_1_CONTROL1_REG |= PowerMonitor_1_WARN_MASK;
                                    #endif /* B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED */
                                }
                            }
                        }
                    }
                #endif /* (PowerMonitor_1_CURRENT_SOURCES > 0u) */
            }

            if (PowerMonitor_1_faultEnable == PowerMonitor_1_CYTRUE)
            {
                if (convDone < PowerMonitor_1_lastVoltageChan)
                {
                    if (PowerMonitor_1_faultMask & (1ul << convDone))
                    {
                        if (PowerMonitor_1_faultSources & PowerMonitor_1_OV_FAULT_SOURCE_MASK)
                        {
                            if (rawFiltVal > PowerMonitor_1_faultWin[convDone].OVFaultThrshldCounts )
                            {
                                /* Write the status to status bits */
                                PowerMonitor_1_faultSourceStatus |= PowerMonitor_1_ENABLE_OV_FAULT_SOURCE;
                                PowerMonitor_1_OVFaultStatus |= (1ul << convDone);
                                #if !defined(\
                                PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
                                    /* Write the hardware signal */
                                    PowerMonitor_1_CONTROL1_REG |= PowerMonitor_1_FAULT_MASK;
                                #endif /* B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED */
                                /* Set the fault detected flag */
                                #if (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_GLOBAL)
                                    PowerMonitor_1_faultDetected = PowerMonitor_1_CYTRUE;
                                #else
                                    PowerMonitor_1_faultStatus[convDone] = PowerMonitor_1_CYTRUE;
                                #endif /* (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_GLOBAL) */
                            }
                        }

                        if (PowerMonitor_1_faultSources & PowerMonitor_1_UV_FAULT_SOURCE_MASK)
                        {
                            if (rawFiltVal < PowerMonitor_1_faultWin[convDone].UVFaultThrshldCounts )
                            {
                                /* Write the status to status bits */
                                PowerMonitor_1_faultSourceStatus |= PowerMonitor_1_ENABLE_UV_FAULT_SOURCE;
                                PowerMonitor_1_UVFaultStatus |= (1ul << convDone);
                                #if !defined(\
                                PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
                                    /* Write the hardware signal */
                                    PowerMonitor_1_CONTROL1_REG |= PowerMonitor_1_FAULT_MASK;
                                #endif /* B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED */
                                /* Set the fault detected flag */
                                #if (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_GLOBAL)
                                    PowerMonitor_1_faultDetected = PowerMonitor_1_CYTRUE;
                                #else
                                    PowerMonitor_1_faultStatus[convDone] = PowerMonitor_1_CYTRUE;
                                #endif /* (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_GLOBAL) */
                            }
                        }
                    }
                }
                #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
                    else
                    {
                        /* Get the index */
                        iChanNum = convDone - PowerMonitor_1_lastVoltageChan;
                        /* Get the actual current channel number */
                        iChanNum = PowerMonitor_1_ActiveCurrentChan[iChanNum];
                        /* Converter number is not zero based. So left shitcount should be converter number - 1 */
                        if (PowerMonitor_1_faultMask & (1ul << (iChanNum - 1u)))
                        {
                            if (PowerMonitor_1_faultSources & PowerMonitor_1_OC_FAULT_SOURCE_MASK)
                            {
                                if (rawFiltVal > PowerMonitor_1_faultWin[iChanNum - 1u].OCFaultThrshldCounts )
                                {
                                    /* Write the status to status bits */
                                    PowerMonitor_1_faultSourceStatus |= PowerMonitor_1_ENABLE_OC_FAULT_SOURCE;
                                    PowerMonitor_1_OCFaultStatus |= (1ul << (iChanNum - 1u));
                                    #if !defined(\
                                    PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
                                        /* Write the hardware signal */
                                        PowerMonitor_1_CONTROL1_REG |= PowerMonitor_1_FAULT_MASK;
                                    #endif /* B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED */
                                    /* Set the fault detected flag */
                                    #if (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_GLOBAL)
                                        PowerMonitor_1_faultDetected = PowerMonitor_1_CYTRUE;
                                    #else
                                        PowerMonitor_1_faultStatus[iChanNum - 1u] = PowerMonitor_1_CYTRUE;
                                    #endif /* PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_GLOBAL */
                                }
                            }
                        }
                    }
                #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */
            }
        }

        /* Generate the PGOOD signal based on the pgood terminal configuration */
        #if (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_INDIVIDUAL)
            if (convDone != PowerMonitor_1_STATE_CAL)
            {
                if (convDone < PowerMonitor_1_lastVoltageChan)
                {
                    #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
                    if (!((PowerMonitor_1_CurrentType[convDone] != PowerMonitor_1_CURRENT_TYPE_NA) &&
                        (PowerMonitor_1_faultSources & PowerMonitor_1_OC_FAULT_SOURCE_MASK)))
                    {
                    #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */
                        if (!(PowerMonitor_1_faultStatus[convDone]))
                        {
                            #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED)
                                /* publish pgood */
                                if (convDone < PowerMonitor_1_CONVERTERS_8)
                                {
                                    PowerMonitor_1_PGOOD_CONTROL1_REG |= (1u << convDone);
                                }
                            #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED) */
                            #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED)
                                #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8)
                                    if ((convDone >= PowerMonitor_1_CONVERTERS_8) &&
                                             (convDone < PowerMonitor_1_CONVERTERS_16))
                                    {
                                        PowerMonitor_1_PGOOD_CONTROL2_REG |=
                                        (1u << (convDone - PowerMonitor_1_CONVERTERS_8));
                                    }
                                #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8) */
                            #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED) */
                            #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED)
                                #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16)
                                    if ((convDone >= PowerMonitor_1_CONVERTERS_16) &&
                                             (convDone < PowerMonitor_1_CONVERTERS_24))
                                    {
                                        PowerMonitor_1_PGOOD_CONTROL3_REG |=
                                        (1u << (convDone - PowerMonitor_1_CONVERTERS_16));
                                    }
                                #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16) */
                            #endif /* PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED) */
                            #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED)
                                #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24)
                                    if (convDone >= PowerMonitor_1_CONVERTERS_24)
                                    {
                                        PowerMonitor_1_PGOOD_CONTROL4_REG |=
                                        (1u << (convDone - PowerMonitor_1_CONVERTERS_24));
                                    }
                                #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24) */
                            #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED) */
                        }
                        else
                        {
                            /* Clear the status */
                            PowerMonitor_1_faultStatus[convDone] = PowerMonitor_1_CYFALSE;
                            #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED)
                                /* Clear pgood */
                                if (convDone < PowerMonitor_1_CONVERTERS_8)
                                {
                                    PowerMonitor_1_PGOOD_CONTROL1_REG &= ~(1u << convDone);
                                }
                            #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED) */
                            #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED)
                                #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8)
                                    if ((convDone >= PowerMonitor_1_CONVERTERS_8) &&
                                             (convDone < PowerMonitor_1_CONVERTERS_16))
                                    {
                                        PowerMonitor_1_PGOOD_CONTROL2_REG &=
                                        ~(1u << (convDone - PowerMonitor_1_CONVERTERS_8));
                                    }
                                #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8) */
                            #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED) */
                            #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED)
                                #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16)
                                    if ((convDone >= PowerMonitor_1_CONVERTERS_16) &&
                                             (convDone < PowerMonitor_1_CONVERTERS_24))
                                    {
                                        PowerMonitor_1_PGOOD_CONTROL3_REG &=
                                        ~(1u << (convDone - PowerMonitor_1_CONVERTERS_16));
                                    }
                                #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16) */
                            #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED) */
                            #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED)
                                #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24)
                                    if (convDone >= PowerMonitor_1_CONVERTERS_24)
                                    {
                                        PowerMonitor_1_PGOOD_CONTROL4_REG &=
                                        ~(1u << (convDone - PowerMonitor_1_CONVERTERS_24));
                                    }
                                #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24) */
                            #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED) */
                        }
                    #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
                    }
                    #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */
                }
                #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
                    else
                    {
                        if (convDone < PowerMonitor_1_lastCurrentChan)
                        {
                            /* Get the index */
                            iChanNum = convDone - PowerMonitor_1_lastVoltageChan;
                            /* Get the actual current channel number */
                            iChanNum = (PowerMonitor_1_ActiveCurrentChan[iChanNum]);
                            if ((PowerMonitor_1_CurrentType[iChanNum - 1u] != PowerMonitor_1_CURRENT_TYPE_NA) &&
                                (PowerMonitor_1_faultSources & PowerMonitor_1_OC_FAULT_SOURCE_MASK))
                            {

                                if (!(PowerMonitor_1_faultStatus[iChanNum - 1u]))
                                {
                                    #if !defined(\
                                        PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED)
                                        /* publish pgood */
                                        if (iChanNum <= PowerMonitor_1_CONVERTERS_8)
                                        {
                                            PowerMonitor_1_PGOOD_CONTROL1_REG |= (1u << (iChanNum - 1u));
                                        }
                                    #endif /* Sync_PM_1_8_Ctrl1__REMOVED */
                                    #if !defined(\
                                        PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED)
                                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8)
                                            if ((iChanNum > PowerMonitor_1_CONVERTERS_8) &&
                                                     (iChanNum <= PowerMonitor_1_CONVERTERS_16))
                                            {
                                                PowerMonitor_1_PGOOD_CONTROL2_REG |=
                                                (1u << (iChanNum - PowerMonitor_1_CONVERTERS_8 - 1u));
                                            }
                                        #endif /* PowerMonitor_1_CONVERTERS_8 */
                                    #endif /* Sync_PM_9_16_Ctrl2__REMOVED */
                                    #if !defined(\
                                        PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED)
                                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16)
                                            if ((iChanNum > PowerMonitor_1_CONVERTERS_16) &&
                                                     (iChanNum <= PowerMonitor_1_CONVERTERS_24))
                                            {
                                                PowerMonitor_1_PGOOD_CONTROL3_REG |=
                                                (1u << (iChanNum - PowerMonitor_1_CONVERTERS_16 - 1u));
                                            }
                                        #endif /* PowerMonitor_1_CONVERTERS_16 */
                                    #endif /* Sync_PM_17_24_Ctrl3__REMOVED */
                                    #if !defined(\
                                        PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED)
                                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24)
                                            if (iChanNum > PowerMonitor_1_CONVERTERS_24)
                                            {
                                                PowerMonitor_1_PGOOD_CONTROL4_REG |=
                                                (1u << (iChanNum - PowerMonitor_1_CONVERTERS_24 - 1u));
                                            }
                                        #endif /* PowerMonitor_1_CONVERTERS_24 */
                                    #endif /* Sync_PM_25_32_Ctrl4__REMOVED */
                                }
                                else
                                {
                                    /* Clear the status */
                                    PowerMonitor_1_faultStatus[iChanNum - 1u] = PowerMonitor_1_CYFALSE;
                                    #if !defined(\
                                        PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED)
                                        /* Clear pgood */
                                        if (iChanNum <= PowerMonitor_1_CONVERTERS_8)
                                        {
                                            PowerMonitor_1_PGOOD_CONTROL1_REG &= ~(1u << (iChanNum - 1u));
                                        }
                                    #endif /* Sync_PM_1_8_Ctrl1__REMOVED */
                                    #if !defined(\
                                        PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED)
                                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8)
                                            if ((iChanNum > PowerMonitor_1_CONVERTERS_8) &&
                                                     (iChanNum <= PowerMonitor_1_CONVERTERS_16))
                                            {
                                                PowerMonitor_1_PGOOD_CONTROL2_REG &=
                                                ~(1u << (iChanNum - PowerMonitor_1_CONVERTERS_8 - 1u));
                                            }
                                        #endif /* PowerMonitor_1_CONVERTERS_) */
                                    #endif /* Sync_PM_9_16_Ctrl2__REMOVED */
                                    #if !defined(\
                                        PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED)
                                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16)
                                            if ((iChanNum > PowerMonitor_1_CONVERTERS_16) &&
                                                     (iChanNum <= PowerMonitor_1_CONVERTERS_24))
                                            {
                                                PowerMonitor_1_PGOOD_CONTROL3_REG &=
                                                ~(1u << (iChanNum - PowerMonitor_1_CONVERTERS_16 - 1u));
                                            }
                                        #endif /* PowerMonitor_1_CONVERTERS_16 */
                                    #endif /* $CtrlReg_RplcString`_PM_17_24_Ctrl3__REMOVED */
                                    #if !defined(\
                                        PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED)
                                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24)
                                            if (iChanNum > PowerMonitor_1_CONVERTERS_24)
                                            {
                                                PowerMonitor_1_PGOOD_CONTROL4_REG &= \
                                                ~(1u << (iChanNum - PowerMonitor_1_CONVERTERS_24 - 1u));
                                            }
                                        #endif /* PowerMonitor_1_CONVERTERS_24 */
                                    #endif /* Sync_PM_25_32_Ctrl4__REMOVED */
                                }
                            }
                        }
                    }
                #endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */
            }
        #else
            /* Generate the PGOOD signal at the end of voltage and current measurements */
            if (convDone == (PowerMonitor_1_TOTAL_V_I_MEASUREMENTS - 1u))
            {
                if (!PowerMonitor_1_faultDetected)
                {
                    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED)
                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24)
                            PowerMonitor_1_PGOOD_CONTROL4_REG |= PowerMonitor_1_PGOOD_CTRL_25_32_MASK;
                        #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24) */
                    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED) */

                    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED)
                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16)
                            PowerMonitor_1_PGOOD_CONTROL3_REG |= PowerMonitor_1_PGOOD_CTRL_17_24_MASK;
                        #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16) */
                    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED) */

                    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED)
                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8)
                            PowerMonitor_1_PGOOD_CONTROL2_REG |= PowerMonitor_1_PGOOD_CTRL_9_16_MASK;
                        #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8) */
                    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED) */

                    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED)
                        PowerMonitor_1_PGOOD_CONTROL1_REG |= PowerMonitor_1_PGOOD_CTRL_1_8_MASK;
                    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED) */
                }
                else
                {
                    PowerMonitor_1_faultDetected = PowerMonitor_1_CYFALSE;
                    /* Clear Pgood Signal */
                    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED)
                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24)
                            PowerMonitor_1_PGOOD_CONTROL4_REG &= ~PowerMonitor_1_PGOOD_CTRL_25_32_MASK;
                        #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_24) */
                    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__REMOVED) */

                    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED)
                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16)
                            PowerMonitor_1_PGOOD_CONTROL3_REG &= ~PowerMonitor_1_PGOOD_CTRL_17_24_MASK;
                        #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_16) */
                    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__REMOVED) */

                    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED)
                        #if (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8)
                            PowerMonitor_1_PGOOD_CONTROL2_REG &= ~PowerMonitor_1_PGOOD_CTRL_9_16_MASK;
                        #endif /* (PowerMonitor_1_NUMBER_OF_CONVERTERS > PowerMonitor_1_CONVERTERS_8) */
                    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__REMOVED) */

                    #if !defined(PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED)
                        PowerMonitor_1_PGOOD_CONTROL1_REG &= ~PowerMonitor_1_PGOOD_CTRL_1_8_MASK;
                    #endif /* (PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__REMOVED) */
                }
            }
        #endif /* (PowerMonitor_1_DEFAULT_PGOOD_CONFIG == PowerMonitor_1_PGOOD_INDIVIDUAL) */

        /* Generate EOC output */
        #if !defined(PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED)
            #if(PowerMonitor_1_EOC_CONFIG == PowerMonitor_1__PER_SAMPLE)
                PowerMonitor_1_CONTROL1_REG = (PowerMonitor_1_CONTROL1_REG ^ PowerMonitor_1_EOC_MASK);
            #else
                if (convDone == (PowerMonitor_1_maxChannels - 1u))
                {
                    PowerMonitor_1_CONTROL1_REG = (PowerMonitor_1_CONTROL1_REG ^ PowerMonitor_1_EOC_MASK);
                }
            #endif /* PowerMonitor_1_SINGLE_ENDED_INPUT_MODE == PowerMonitor_1__PER_SAMPLE */
        #endif /* (PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__REMOVED) */
    }
    else
    {
        if (convDone == (PowerMonitor_1_maxChannels - 1u))
        {
            if(PowerMonitor_1_adcStartFalseCnt != 0u)
            {
                PowerMonitor_1_adcStartFalseCnt --;
            }
            if((PowerMonitor_1_initThreshold == 1u)&&(PowerMonitor_1_adcStartFalseCnt == 0u))
            {
                PowerMonitor_1_PMoutEn = PowerMonitor_1_CYTRUE;
            }
        }
    }

    /************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    *************************************************************************/
    /* `#START END_POWER_MONITOR_ISR`  */

    /* `#END`  */

    #ifdef PowerMonitor_1_ISR_EXIT_CALLBACK
        PowerMonitor_1_ISR_ExitCallback();
    #endif /* PowerMonitor_1_ISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        /* Restore the global interrupts */
        EA = int_en;
    #endif /* (CY_PSOC3) */
}


/******************************************************************************
* Function Name: PowerMonitor_1_SaveDsmRegsAndIsolate()
********************************************************************************
*
* Summary:
*  Save DeltaSigma DSM ADC connections to fully isolate before manual routing
*  (bypassing the AMuxSeq Component) for calibration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*  Note the AMuxSeq routing is logical and predictable, EXCEPT that the fitter
*   currently disconnects the 2xVref reference at the PGA, not at the DSM(-)
*   as the schematic leads you to conclude.  This is handled here
*
* Calibration originally used the AMuxSeq component, but uses 3 AMuxSeq
*  input channels.  The AMuxSeq has a max of 32 channels and we want up to
*  16 rails, so do calibration routing outside the AMuxSeq.
*
*  This section MIGHT need occasional tweaking because it relies upon
*  knowledge of how CY Fitter works.  Although CY Fitter should be
*  constrained to a good rendering, it may still do unexpected things.
*  For example, it attaches 2048mV to DelSig(-) by leaving ABUSL1 on the
*  DelSig and controlling the PGA output switch.
*
*******************************************************************************/
static void PowerMonitor_1_SaveDsmRegsAndIsolate(void) 
{
    PowerMonitor_1_keep_CYREG_DSM0_SW0 = PowerMonitor_1_ADC_DSM_SW0_REG;
    PowerMonitor_1_keep_CYREG_DSM0_SW2 = PowerMonitor_1_ADC_DSM_SW2_REG;
    PowerMonitor_1_keep_CYREG_DSM0_SW3 = PowerMonitor_1_ADC_DSM_SW3_REG;
    PowerMonitor_1_keep_CYREG_DSM0_SW4 = PowerMonitor_1_ADC_DSM_SW4_REG;
    PowerMonitor_1_keep_CYREG_DSM0_SW6 = PowerMonitor_1_ADC_DSM_SW6_REG;
    /***************************************************************************
    * Isolate/disconnect DSM inputs
    ***************************************************************************/
    PowerMonitor_1_ADC_DSM_SW0_REG = 0x00u;
    PowerMonitor_1_ADC_DSM_SW2_REG = 0x00u;
    PowerMonitor_1_ADC_DSM_SW3_REG = 0x00u;
    PowerMonitor_1_ADC_DSM_SW4_REG = 0x00u;
    PowerMonitor_1_ADC_DSM_SW6_REG = 0x00u;

    /* Set the flag after writing ADC switching registers with new values for
       calibration. This will be used to restore the DSM switching registers
       soon after calibration */
    PowerMonitor_1_dsmRegRestorePend = PowerMonitor_1_CYTRUE;
}


/*******************************************************************************
* Function Name: PowerMonitor_1_RestoreDsmRegs()
********************************************************************************
*
* Summary:
*  Replace DSM default switches as expected by AMuxSeq.  Does the inverse
*  of sameDsmRegsAndIsolate() when leaving calibration and resuming sampling.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*  Note the AMuxSeq routing is logical and predictable, EXCEPT that the fitter
*   currently disconnects the 2xVref reference at the PGA, not at the DSM(-)
*   as the schematic leads you to conclude.  This is handled here
*
*******************************************************************************/
static void PowerMonitor_1_RestoreDsmRegs(void) 
{
    /* Isolate/disconnect DSM inputs BEFORE restoring connections */
    PowerMonitor_1_ADC_DSM_SW0_REG = 0x00u;
    PowerMonitor_1_ADC_DSM_SW2_REG = 0x00u;
    PowerMonitor_1_ADC_DSM_SW3_REG = 0x00u;
    PowerMonitor_1_ADC_DSM_SW4_REG = 0x00u;
    PowerMonitor_1_ADC_DSM_SW6_REG = 0x00u;

    /* Restore original DSM connnections */
    PowerMonitor_1_ADC_DSM_SW0_REG = PowerMonitor_1_keep_CYREG_DSM0_SW0;
    PowerMonitor_1_ADC_DSM_SW2_REG = PowerMonitor_1_keep_CYREG_DSM0_SW2;
    PowerMonitor_1_ADC_DSM_SW3_REG = PowerMonitor_1_keep_CYREG_DSM0_SW3;
    PowerMonitor_1_ADC_DSM_SW4_REG = PowerMonitor_1_keep_CYREG_DSM0_SW4;
    PowerMonitor_1_ADC_DSM_SW6_REG = PowerMonitor_1_keep_CYREG_DSM0_SW6;

    /* Clear the flag after restoring ADC switching registers */
    PowerMonitor_1_dsmRegRestorePend = PowerMonitor_1_CYFALSE;
}


/*******************************************************************************
* Function Name: PowerMonitor_1_GetMaxFilterOrder()
********************************************************************************
*
* Summary:
*  Return maximun filter order (voltage, current, auxiliary)
*
* Parameters:
*  void
*
* Return:
*  maximun filter order
*
*******************************************************************************/
static uint8 PowerMonitor_1_GetMaxFilterOrder(void) 
{
    uint8 ret = PowerMonitor_1_VOLTAGE_FILTER_SIZE;

    #if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
        if(PowerMonitor_1_CURRENT_FILTER_SIZE > ret)
        {
            ret = PowerMonitor_1_CURRENT_FILTER_SIZE;
        }
    #endif /* PowerMonitor_1_NUM_CURRENT_SOURCES > 0u */

    #if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)
        if(PowerMonitor_1_AUX_VOLTAGE_FILTER_SIZE > ret)
        {
            ret = PowerMonitor_1_AUX_VOLTAGE_FILTER_SIZE;
        }
    #endif /* PowerMonitor_1_NUM_AUX_INPUTS > 0u */
    return ret;
}


/* [] END OF FILE */
