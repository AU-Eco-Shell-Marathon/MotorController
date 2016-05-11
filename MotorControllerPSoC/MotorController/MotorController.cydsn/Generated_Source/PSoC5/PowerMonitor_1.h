/*******************************************************************************
* File Name: PowerMonitor_1.h
* Version 1.60
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Power Monitor component.
*
* Note:
*
*******************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_POWERMONITOR_PowerMonitor_1_H)
#define CY_POWERMONITOR_PowerMonitor_1_H

#include "cytypes.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PowerMonitor_v1_60 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* To run the initialization block only at the start up */
extern uint8 PowerMonitor_1_initVar;

/* To run the threshold initialization only at the start up */
extern uint8 PowerMonitor_1_initThreshold;


/**************************************
*           Type defines
***************************************/
/*
* PowerMonitor_1_ADC_CTL_STRUCT - ADC filter reading structure (one struct
*                                   per channel)
*          idx = array index of oldest reading (next to be replaced)
*      filtval = Sum of last _ADC_FILT_SZ samples, divide to get Ave
*/
typedef struct
{
    volatile uint8 idx;                 /* Next sample to be replaced       */
    volatile int32 filtVal;             /* sample totalizer                 */
} PowerMonitor_1_ADC_CTL_STRUCT;


/*******************************************************************************
* PowerMonitor_1_WarnWin[] -> Structure to hold the warning thresholds
* PowerMonitor_1_FaultWin[] -> Structure to hold the fault thresholds
*******************************************************************************/
typedef struct
{
    uint8 enable;   /* Z suppresses warnings (e.g. when rail is OFF) */
    int32 OVWarnThrshldCounts;
    int32 UVWarnThrshldCounts;
    int32 OCWarnThrshldCounts;
    uint16 OVWarnThrshldVolts;
    uint16 UVWarnThrshldVolts;
    float OCWarnThrshldAmps;
} PowerMonitor_1_WARNWIN_STRUCT;

extern PowerMonitor_1_WARNWIN_STRUCT CYXDATA PowerMonitor_1_warnWin[];

typedef struct
{
    uint8 enable;   /* Z suppresses warnings (e.g. when rail is OFF) */
    int32 OVFaultThrshldCounts;
    int32 UVFaultThrshldCounts;
    int32 OCFaultThrshldCounts;
    uint16 OVFaultThrshldVolts;
    uint16 UVFaultThrshldVolts;
    float OCFaultThrshldAmps;
} PowerMonitor_1_FAULTWIN_STRUCT;

extern PowerMonitor_1_FAULTWIN_STRUCT CYXDATA PowerMonitor_1_faultWin[];


/* Variables to access at different source files */
extern volatile uint8 CYDATA PowerMonitor_1_adcConvNow;
extern volatile uint8 CYDATA PowerMonitor_1_adcConvNext;
extern volatile uint8 CYDATA PowerMonitor_1_adcConvNextPreCal;
extern volatile uint8 CYDATA PowerMonitor_1_adcConvCalType;
extern volatile uint8 CYDATA PowerMonitor_1_maxChannels;

/* Calibratin Variables */
/* ~ VALUE, RANGE Description */
extern CYBIT PowerMonitor_1_iirInit;
extern volatile int16 CYXDATA PowerMonitor_1_adcZeroCalRawCfg1;
extern volatile int16 CYXDATA PowerMonitor_1_adcGainCalRawCfg1;
extern volatile int16 CYXDATA PowerMonitor_1_adcSCCalRawCfg1;
extern volatile int16 CYXDATA PowerMonitor_1_adcZeroCalRawCfg2;
extern volatile int16 CYXDATA PowerMonitor_1_adcGainCalRawCfg2a;
extern volatile int16 CYXDATA PowerMonitor_1_adcGainCalRawCfg2b;
extern volatile int16 CYXDATA PowerMonitor_1_adcZeroCalRawCfg3;
extern volatile int16 CYXDATA PowerMonitor_1_adcGainCalRawCfg3;
extern volatile int16 CYXDATA PowerMonitor_1_adcSCCalRawCfg3;

/* Fault and Warn Mask variables */
extern volatile uint32 CYDATA PowerMonitor_1_faultMask;
extern volatile uint32 CYDATA PowerMonitor_1_warnMask;

/* Enable fault and Enable warn variables */
extern CYBIT PowerMonitor_1_faultEnable;
extern CYBIT PowerMonitor_1_warnEnable;

/* Fault and Warn source variables */
extern volatile uint8 CYDATA PowerMonitor_1_warnSources;
extern volatile uint8 CYDATA PowerMonitor_1_faultSources;

/* Warn and Fault source status */
extern volatile uint8 CYDATA PowerMonitor_1_warnSourceStatus;
extern volatile uint8 CYDATA PowerMonitor_1_faultSourceStatus;

/* Warn and Fault status for power monitors */
extern volatile uint32 CYDATA PowerMonitor_1_OCWarnStatus;
extern volatile uint32 CYDATA PowerMonitor_1_UVWarnStatus;
extern volatile uint32 CYDATA PowerMonitor_1_OVWarnStatus;
extern volatile uint32 CYDATA PowerMonitor_1_OCFaultStatus;
extern volatile uint32 CYDATA PowerMonitor_1_UVFaultStatus;
extern volatile uint32 CYDATA PowerMonitor_1_OVFaultStatus;


/*******************************************************************************
* Configuration #1 : 2048 mV differential range
*  adcZeroCfg1  = units are ADC counts when both inputs are at Vssa
*  adcGainCfg1  = units are (1 mV) / (ADC count)
*  adcSeAdjCfg1 = units are mV from 2xVref PGA (added to all voltage rails)
*******************************************************************************/
extern  int16 PowerMonitor_1_adcZeroCfg1;
extern  float PowerMonitor_1_adcGainCfg1;
extern  int16 PowerMonitor_1_adcSeAdjCfg1;

/*******************************************************************************
* Configuration #2 : low-voltage differential range:
*   adcZeroCfg2 = units are ADC counts when both inputs are at Vssa
*   adcGainCfg2 = units are (100 uV) / (ADC count)
*******************************************************************************/
extern int16 PowerMonitor_1_adcZeroCfg2;
extern float PowerMonitor_1_adcGainCfg2;


/*******************************************************************************
* Configuration #3 : 1024 mV differential range
*   adcZeroCfg3 = units are ADC counts when both inputs are at Vssa
*   adcGainCfg3 = units are (1 mV) / (ADC count)
*******************************************************************************/
extern int16 PowerMonitor_1_adcZeroCfg3;
extern float PowerMonitor_1_adcGainCfg3;
extern int16 PowerMonitor_1_adcSeAdjCfg3;


/**************************************
*           Parameter Defaults
**************************************/

/* Default config values from user parameters */
#define PowerMonitor_1_NUM_CONVERTERS                  (1u)
#define PowerMonitor_1_NUM_AUX_INPUTS                  (0u)
#define PowerMonitor_1_DEFAULT_OV_FAULT_MODE           (1u)
#define PowerMonitor_1_DEFAULT_OC_FAULT_MODE           (1u)
#define PowerMonitor_1_DEFAULT_UV_FAULT_MODE           (1u)
#define PowerMonitor_1_DEFAULT_OV_WARN_MODE            (0u)
#define PowerMonitor_1_DEFAULT_UV_WARN_MODE            (0u)
#define PowerMonitor_1_DEFAULT_OC_WARN_MODE            (0u)
#define PowerMonitor_1_DEFAULT_VFILTER_TYPE            (4u)
#define PowerMonitor_1_DEFAULT_CFILTER_TYPE            (4u)
#define PowerMonitor_1_DEFAULT_AUX_FILTER_TYPE         (4u)
#define PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE        (1u)
#define PowerMonitor_1_DEFAULT_DIFF_CURRENT_RANGE      (1u)
#define PowerMonitor_1_CAL_PIN_EXPOSED                 (0u)
#define PowerMonitor_1_DEFAULT_PGOOD_CONFIG            (0u)
#define PowerMonitor_1_EOC_CONFIG                      (1u)
#define PowerMonitor_1_SINGLE_ENDED_INPUT_MODE         (2u)


/***************************************
*        Function Prototypes
***************************************/

void PowerMonitor_1_Start(void) ;
void PowerMonitor_1_Stop (void) ;
void PowerMonitor_1_Init(void) ;
void PowerMonitor_1_Enable(void) ;
void PowerMonitor_1_EnableFault(void) ;
void PowerMonitor_1_DisableFault(void) ;
void PowerMonitor_1_SetFaultMode(uint8 faultMode) ;
uint8 PowerMonitor_1_GetFaultMode(void) ;
void PowerMonitor_1_SetFaultMask(uint32 faultMask) ;
uint32 PowerMonitor_1_GetFaultMask(void) ;
uint8 PowerMonitor_1_GetFaultSource(void) ;
uint32 PowerMonitor_1_GetOVFaultStatus(void) ;
uint32 PowerMonitor_1_GetUVFaultStatus(void) ;
uint32 PowerMonitor_1_GetOCFaultStatus(void) ;
void PowerMonitor_1_EnableWarn(void) ;
void PowerMonitor_1_DisableWarn(void) ;
void PowerMonitor_1_SetWarnMode(uint8 warnMode) ;
uint8 PowerMonitor_1_GetWarnMode(void) ;
void PowerMonitor_1_SetWarnMask(uint32 warnMask) ;
uint32 PowerMonitor_1_GetWarnMask(void) ;
uint8 PowerMonitor_1_GetWarnSource(void) ;
uint32 PowerMonitor_1_GetOVWarnStatus(void) ;
uint32 PowerMonitor_1_GetUVWarnStatus(void) ;
uint32 PowerMonitor_1_GetOCWarnStatus(void) ;
void PowerMonitor_1_SetUVWarnThreshold(uint8 converterNum, uint16 uvWarnThreshold) \
     ;
uint16 PowerMonitor_1_GetUVWarnThreshold(uint8 converterNum) \
       ;
void PowerMonitor_1_SetOVWarnThreshold(uint8 converterNum, uint16 ovWarnThreshold) \
     ;
uint16 PowerMonitor_1_GetOVWarnThreshold(uint8 converterNum) \
       ;
void PowerMonitor_1_SetUVFaultThreshold(uint8 converterNum, uint16 uvFaultThreshold) \
     ;
uint16 PowerMonitor_1_GetUVFaultThreshold(uint8 converterNum) \
       ;
void PowerMonitor_1_SetOVFaultThreshold(uint8 converterNum, uint16 ovFaultThreshold) \
     ;
uint16 PowerMonitor_1_GetOVFaultThreshold(uint8 converterNum) \
       ;
void PowerMonitor_1_SetOCWarnThreshold(uint8 converterNum, float ocWarnThreshold) \
     ;
float PowerMonitor_1_GetOCWarnThreshold(uint8 converterNum) \
       ;
void PowerMonitor_1_SetOCFaultThreshold(uint8 converterNum, float ocFaultThreshold) \
     ;
float PowerMonitor_1_GetOCFaultThreshold(uint8 converterNum) \
       ;
uint16 PowerMonitor_1_GetConverterVoltage(uint8 converterNum) \
       ;
float PowerMonitor_1_GetConverterCurrent(uint8 converterNum) \
       ;
float PowerMonitor_1_GetAuxiliaryVoltage(uint8 auxNum) ;
void PowerMonitor_1_Calibrate(void) ;
void PowerMonitor_1_IsrStart(void) ;
CY_ISR_PROTO(PowerMonitor_1_ISR);
#if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)
    void PowerMonitor_1_SetAuxiliarySampleMode(uint8 auxNum, uint8 sampleMode)
                                            ;
    uint8 PowerMonitor_1_GetAuxiliarySampleMode(uint8 auxNum)
                                            ;
    float PowerMonitor_1_RequestAuxiliarySample(uint8 auxNum)
                                            ;
#endif /* PowerMonitor_1_NUM_AUX_INPUTS > 0u */


/* Total number of active current sources */
#define PowerMonitor_1_NUM_CURRENT_SOURCES             (1u)

/* structure variable declarations */
extern PowerMonitor_1_ADC_CTL_STRUCT CYXDATA PowerMonitor_1_voltCtl[];
#if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
    extern PowerMonitor_1_ADC_CTL_STRUCT CYXDATA PowerMonitor_1_ampCtl[];
#endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */

#if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)
    extern PowerMonitor_1_ADC_CTL_STRUCT CYXDATA PowerMonitor_1_auxVoltCtl[];
#endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0u) */

/* array declarations */
extern uint8 PowerMonitor_1_VoltageType[];

#if (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u)
    extern uint8 PowerMonitor_1_CurrentType[];
    extern uint8 PowerMonitor_1_ActiveCurrentChan[];
#endif /* (PowerMonitor_1_NUM_CURRENT_SOURCES > 0u) */

#if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)
    extern uint8 PowerMonitor_1_AuxVoltageType[];
#endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0u) */

#if (PowerMonitor_1_NUM_AUX_INPUTS > 0u)
    extern volatile uint8 PowerMonitor_1_auxEnableMask;
#endif /* (PowerMonitor_1_NUM_AUX_INPUTS > 0u) */


/**************************************
*           API Constants
**************************************/

#define PowerMonitor_1_MAX_CONVERTERS                  (32u)

/* default warn and fault modes */
#define PowerMonitor_1_DEFAULT_WARN_MODE               (PowerMonitor_1_DEFAULT_OV_WARN_MODE | \
                                                          (PowerMonitor_1_DEFAULT_UV_WARN_MODE << 1u) | \
                                                          (PowerMonitor_1_DEFAULT_OC_WARN_MODE << 2u))
#define PowerMonitor_1_DEFAULT_FAULT_MODE              (PowerMonitor_1_DEFAULT_OV_FAULT_MODE | \
                                                          (PowerMonitor_1_DEFAULT_UV_FAULT_MODE << 1u) | \
                                                          (PowerMonitor_1_DEFAULT_OC_FAULT_MODE << 2u))
#define PowerMonitor_1_WARN_MODE_MASK                  (0x07u)
#define PowerMonitor_1_FAULT_MODE_MASK                 (0x07u)

#define PowerMonitor_1_CYTRUE                          (1u)
#define PowerMonitor_1_CYFALSE                         (0u)

#define PowerMonitor_1_MAX_VOLT_CHAN                   (32u)
#define PowerMonitor_1_MAX_CURRENT_CHAN                (32u)

/* Maximum voltage for ADC range */
#define PowerMonitor_1_VOLTAGE_2048MV                  (2048u)
#define PowerMonitor_1_VOLTAGE_4096MV                  (4096u)
#define PowerMonitor_1_VOLTAGE_64MV                    (64u)
#define PowerMonitor_1_VOLTAGE_128MV                   (128u)

/* Single ended voltage range defines */
#define PowerMonitor_1_SE_VOLTAGE_RANGE_2048MV         (0u)
#define PowerMonitor_1_SE_VOLTAGE_RANGE_4096MV         (1u)

/* Differential current range defines */
#define PowerMonitor_1_DIFF_CURRENT_RANGE_64MV         (0u)
#define PowerMonitor_1_DIFF_CURRENT_RANGE_128MV        (1u)

/* ADC configuration defines */
/* ADC Configuration #1 : +/-2048 mV range      */
#define PowerMonitor_1_RANGE_2048                      (1u)

/* ADC Configuration #2 or #3 : low-voltage range   */
#if (PowerMonitor_1_DEFAULT_DIFF_CURRENT_RANGE == PowerMonitor_1_DIFF_CURRENT_RANGE_64MV)
    #define PowerMonitor_1_RANGE_LOW                   (2u)
#else
    #define PowerMonitor_1_RANGE_LOW                   (3u)
#endif /* (PowerMonitor_1_DEFAULT_DIFF_CURRENT_RANGE == PowerMonitor_1_DIFF_CURRENT_RANGE_64MV) */

/* ADC Configuration #1 or #4 : single ended voltage range   */
#if (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_2048MV)
    #define PowerMonitor_1_SE_VOLTAGE_RANGE            (4u)
#else
    #define PowerMonitor_1_SE_VOLTAGE_RANGE            (1u)
#endif /* (PowerMonitor_1_DEFAULT_SE_VOLTAGE_RANGE == PowerMonitor_1_SE_VOLTAGE_RANGE_2048MV) */

/* ADC Configuration #4 : +/-1024 mV range      */
#define PowerMonitor_1_RANGE_1024                      (4u)

/* 1st ADC State, reads 1st voltage */
#define PowerMonitor_1_ISR_STATE_0                     (0u)

/* Priority of the ADC_IRQ interrupt. */
#define PowerMonitor_1_IRQ_INTC_PRIOR_NUMBER           PowerMonitor_1_ADC_IRQ__INTC_PRIOR_NUM
#define PowerMonitor_1_IRQ__INTC_NUMBER                PowerMonitor_1_ADC_IRQ__INTC_NUMBER

/* Default warn and fault masks */
#if (PowerMonitor_1_NUM_CONVERTERS == PowerMonitor_1_MAX_CONVERTERS)
    #define PowerMonitor_1_DEFAULT_WARN_MASK           (0xfffffffful)
    #define PowerMonitor_1_DEFAULT_FAULT_MASK          (0xfffffffful)
#else
    #define PowerMonitor_1_DEFAULT_WARN_MASK           ((1ul << PowerMonitor_1_NUM_CONVERTERS) - 1)
    #define PowerMonitor_1_DEFAULT_FAULT_MASK          ((1ul << PowerMonitor_1_NUM_CONVERTERS) - 1)
#endif /* (PowerMonitor_1_NUM_CONVERTERS == PowerMonitor_1_MAX_CONVERTERS) */

/* Defaultauxiliary channels masks */
#define PowerMonitor_1_DEFAULT_AUX_CHANNELS_MASK       (0xffu >> (8 - PowerMonitor_1_NUM_AUX_INPUTS))

/* Filter type defines */
#define PowerMonitor_1_FILTER_TYPE_NONE                (0u)
#define PowerMonitor_1_FILTER_TYPE_4                   (1u)
#define PowerMonitor_1_FILTER_TYPE_8                   (2u)
#define PowerMonitor_1_FILTER_TYPE_16                  (3u)
#define PowerMonitor_1_FILTER_TYPE_32                  (4u)

/* Voltage Filter type define */
#if (PowerMonitor_1_DEFAULT_VFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE)
    #define PowerMonitor_1_VOLTAGE_FILTER_SIZE         (1u)
#elif (PowerMonitor_1_DEFAULT_VFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_4)
    #define PowerMonitor_1_VOLTAGE_FILTER_SIZE         (4u)
#elif (PowerMonitor_1_DEFAULT_VFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_8)
    #define PowerMonitor_1_VOLTAGE_FILTER_SIZE         (8u)
#elif (PowerMonitor_1_DEFAULT_VFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_16)
    #define PowerMonitor_1_VOLTAGE_FILTER_SIZE         (16u)
#else
    #define PowerMonitor_1_VOLTAGE_FILTER_SIZE         (32u)
#endif /* (PowerMonitor_1_DEFAULT_VFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE) */

/* Current Filter type define */
#if (PowerMonitor_1_DEFAULT_CFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE)
    #define PowerMonitor_1_CURRENT_FILTER_SIZE         (1u)
#elif (PowerMonitor_1_DEFAULT_CFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_4)
    #define PowerMonitor_1_CURRENT_FILTER_SIZE         (4u)
#elif (PowerMonitor_1_DEFAULT_CFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_8)
    #define PowerMonitor_1_CURRENT_FILTER_SIZE         (8u)
#elif (PowerMonitor_1_DEFAULT_CFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_16)
    #define PowerMonitor_1_CURRENT_FILTER_SIZE         (16u)
#else
    #define PowerMonitor_1_CURRENT_FILTER_SIZE         (32u)
#endif /* (PowerMonitor_1_DEFAULT_CFILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE) */

/* Auxiliary voltage Filter type define */
#if (PowerMonitor_1_DEFAULT_AUX_FILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE)
    #define PowerMonitor_1_AUX_VOLTAGE_FILTER_SIZE     (1u)
#elif (PowerMonitor_1_DEFAULT_AUX_FILTER_TYPE == PowerMonitor_1_FILTER_TYPE_4)
    #define PowerMonitor_1_AUX_VOLTAGE_FILTER_SIZE     (4u)
#elif (PowerMonitor_1_DEFAULT_AUX_FILTER_TYPE == PowerMonitor_1_FILTER_TYPE_8)
    #define PowerMonitor_1_AUX_VOLTAGE_FILTER_SIZE     (8u)
#elif (PowerMonitor_1_DEFAULT_AUX_FILTER_TYPE == PowerMonitor_1_FILTER_TYPE_16)
    #define PowerMonitor_1_AUX_VOLTAGE_FILTER_SIZE     (16u)
#else
    #define PowerMonitor_1_AUX_VOLTAGE_FILTER_SIZE     (32u)
#endif /* (PowerMonitor_1_DEFAULT_AUX_FILTER_TYPE == PowerMonitor_1_FILTER_TYPE_NONE) */

/* Define for possible max number of combinations */
#define PowerMonitor_1_MAX_CHANNELS                    (PowerMonitor_1_NUM_CONVERTERS + \
                                                         PowerMonitor_1_NUM_CURRENT_SOURCES + \
                                                         PowerMonitor_1_NUM_AUX_INPUTS)

/* No Warning check for this rail */
#define PowerMonitor_1_WARN_DISABLED                   (0u)
/* Start delay until Warning checks begin */
#define PowerMonitor_1_WARN_TIME_0                     (1u)
/* Check Warning checks for this ADC channel */
#define PowerMonitor_1_WARN_FAULT_ENABLED              (20u)
/* AUX channels mode*/
#define PowerMonitor_1_CONTINUOUS                      (0u)
#define PowerMonitor_1_ON_DEMAND                       (1u)


/**************************************
*    Enumerated Types and Parameters
**************************************/

/*  Reference constants */
#define PowerMonitor_1__ADC_DELSIG__SOFTWARE 0
#define PowerMonitor_1__ADC_DELSIG__HARDWARE 1

/*  Input Buffer Mode constants */
#define PowerMonitor_1__ADC_DELSIG__BYPASS_BUFFER 0
#define PowerMonitor_1__ADC_DELSIG__RAIL_TO_RAIL 1
#define PowerMonitor_1__ADC_DELSIG__LEVEL_SHIFT 2

/*  EOC pulse is asserted after every measurement(Per Sample)
*   or just once when one sample has been obtained from all specified
*   inputs(Entire Set).
*/
#define PowerMonitor_1__PER_SAMPLE 0
#define PowerMonitor_1__COMPLETE_CYCLE 1




/********************************************
* Calibration state defines
********************************************/

/* defines to interrupt normal ADC reading and to start calibration */
#define PowerMonitor_1_STATE_CAL                       (255u)
#define PowerMonitor_1_STATE_MEASURE_AUX               (254u)
/* one-time ADC ISR setup at reset */
#define PowerMonitor_1_CAL_START                       (0u)
/* +/-2048 ZERO */
#define PowerMonitor_1_CAL_CFG1Z                       (1u)
/* +/-2048 read of -Vref */
#define PowerMonitor_1_CAL_CFG1G                       (2u)
/* +/-2048 read of 2 x PGA x Vref */
#define PowerMonitor_1_CAL_PGA1Z                       (3u)
/* +/-256  ZERO */
#define PowerMonitor_1_CAL_CFG2Z                       (4u)
/* +/-1024 read of cal input      */
#define PowerMonitor_1_CAL_CFG2Ga                      (5u)
/* +/-64/128mv  read of cal input */
#define PowerMonitor_1_CAL_CFG2Gb                      (6u)
#define PowerMonitor_1_CAL_CFG2G                       (7u)
/* +/-1024 ZERO */
#define PowerMonitor_1_CAL_CFG3Z                       (8u)
/* +/-1024 read of -Vref */
#define PowerMonitor_1_CAL_CFG3G                       (9u)
/* +/-1024 read of  Vref */
#define PowerMonitor_1_CAL_PGA3Z                       (10u)
/* Calibration oversample related defines */
#define PowerMonitor_1_CAL2A_OVER_EXP                  (4u)
/* _FILT_SZ  - number of samples to oversample */
#define PowerMonitor_1_CAL2A_FILT_SZ                   (1u << PowerMonitor_1_CAL2A_OVER_EXP)

/*******************************************************************************
* Filtered raw data from the ADC
*  filter = filter - (filter / SCALE_VAL) + newData;
*  output = filter / SCALE_VAL;
*******************************************************************************/
#define PowerMonitor_1_FILTER_SHIFT                    (4u)
#define PowerMonitor_1_SCALE_VAL                       (uint16)(1u << PowerMonitor_1_FILTER_SHIFT)
#define PowerMonitor_1_INITIALIZE_IIR_FILTER           (1u)
#define PowerMonitor_1_NEXTSAMPLE_IIR_FILTER           (0u)

/* Defines for voltage types */
#define PowerMonitor_1_VOLTAGE_TYPE_SINGLE             (0u)
#define PowerMonitor_1_VOLTAGE_TYPE_DIFF               (01u)
/* Defines for current types */
#define PowerMonitor_1_CURRENT_TYPE_NA                 (0u)
#define PowerMonitor_1_CURRENT_TYPE_DIRECT             (01u)
#define PowerMonitor_1_CURRENT_TYPE_CSA                (02u)

/* Defines for Auxiliary voltage types */
#define PowerMonitor_1_AUX_VOLTAGE_SINGLE              (0u)
#define PowerMonitor_1_AUX_VOLTAGE_64MV_DIFF           (1u)
#define PowerMonitor_1_AUX_VOLTAGE_2048MV_DIFF         (2u)
#define PowerMonitor_1_AUX_VOLTAGE_128MV_DIFF          (3u)

/* Defines for Register masks */
#define PowerMonitor_1_EOC_MASK                        (0x04u)
#define PowerMonitor_1_FAULT_MASK                      (0x02u)
#define PowerMonitor_1_WARN_MASK                       (0x01u)

/* Defines for reseting fault status */
#define PowerMonitor_1_RESET_OV_FAULT_STATUS           (0x00000000ul)
#define PowerMonitor_1_RESET_UV_FAULT_STATUS           (0x00000000ul)
#define PowerMonitor_1_RESET_OC_FAULT_STATUS           (0x00000000ul)

/* Defines for resetting warn status */
#define PowerMonitor_1_RESET_OV_WARN_STATUS            (0x00000000ul)
#define PowerMonitor_1_RESET_UV_WARN_STATUS            (0x00000000ul)
#define PowerMonitor_1_RESET_OC_WARN_STATUS            (0x00000000ul)

/* Channel Type defines */
#define PowerMonitor_1_VOLTAGE                         (0u)
#define PowerMonitor_1_CURRENT                         (1u)
#define PowerMonitor_1_AUXVOLTAGE                      (2u)

/* Number of voltage and current measurements */
#define PowerMonitor_1_TOTAL_V_I_MEASUREMENTS          (PowerMonitor_1_NUM_CONVERTERS + \
                                                          PowerMonitor_1_NUM_CURRENT_SOURCES)


/* Warn Source Status bits related defines */
#define PowerMonitor_1_ENABLE_OV_WARN_SOURCE           (0x01u)
#define PowerMonitor_1_ENABLE_UV_WARN_SOURCE           (0x02u)
#define PowerMonitor_1_ENABLE_OC_WARN_SOURCE           (0x04u)

/* Fault Source Status bits related defines */
#define PowerMonitor_1_ENABLE_OV_FAULT_SOURCE          (0x01u)
#define PowerMonitor_1_ENABLE_UV_FAULT_SOURCE          (0x02u)
#define PowerMonitor_1_ENABLE_OC_FAULT_SOURCE          (0x04u)

/* Warn Source Mask */
#define PowerMonitor_1_OV_WARN_SOURCE_MASK             (0x01u)
#define PowerMonitor_1_UV_WARN_SOURCE_MASK             (0x02u)
#define PowerMonitor_1_OC_WARN_SOURCE_MASK             (0x04u)

/* Fault Source Mask */
#define PowerMonitor_1_OV_FAULT_SOURCE_MASK            (0x01u)
#define PowerMonitor_1_UV_FAULT_SOURCE_MASK            (0x02u)
#define PowerMonitor_1_OC_FAULT_SOURCE_MASK            (0x04u)

/* Number of converters related defines */
#define PowerMonitor_1_CONVERTERS_8                    (8u)
#define PowerMonitor_1_CONVERTERS_16                   (16u)
#define PowerMonitor_1_CONVERTERS_24                   (24u)

/* Pgood control register mask */
#define PowerMonitor_1_PGOOD_CTRL_MASK                 (0xFFu)
#if (PowerMonitor_1_NUM_CONVERTERS > PowerMonitor_1_CONVERTERS_24)
    #define PowerMonitor_1_PGOOD_CTRL_25_32_MASK      ((1u <<(PowerMonitor_1_NUM_CONVERTERS - \
                                                          PowerMonitor_1_CONVERTERS_24)) - 1)
    #define PowerMonitor_1_PGOOD_CTRL_17_24_MASK      (0xFFu)
    #define PowerMonitor_1_PGOOD_CTRL_9_16_MASK       (0xFFu)
    #define PowerMonitor_1_PGOOD_CTRL_1_8_MASK        (0xFFu)
#elif (PowerMonitor_1_NUM_CONVERTERS > PowerMonitor_1_CONVERTERS_16)
    #define PowerMonitor_1_PGOOD_CTRL_25_32_MASK      (0x00u)
    #define PowerMonitor_1_PGOOD_CTRL_17_24_MASK      ((1u <<(PowerMonitor_1_NUM_CONVERTERS - \
                                                          PowerMonitor_1_CONVERTERS_16)) - 1)
    #define PowerMonitor_1_PGOOD_CTRL_9_16_MASK       (0xFFu)
    #define PowerMonitor_1_PGOOD_CTRL_1_8_MASK        (0xFFu)
#elif (PowerMonitor_1_NUM_CONVERTERS > PowerMonitor_1_CONVERTERS_8)
    #define PowerMonitor_1_PGOOD_CTRL_25_32_MASK      (0x00u)
    #define PowerMonitor_1_PGOOD_CTRL_17_24_MASK      (0x00u)
    #define PowerMonitor_1_PGOOD_CTRL_9_16_MASK       ((1u <<(PowerMonitor_1_NUM_CONVERTERS - \
                                                          PowerMonitor_1_CONVERTERS_8)) - 1)
    #define PowerMonitor_1_PGOOD_CTRL_1_8_MASK        (0xFFu)
#else
    #define PowerMonitor_1_PGOOD_CTRL_25_32_MASK      (0x00u)
    #define PowerMonitor_1_PGOOD_CTRL_17_24_MASK      (0x00u)
    #define PowerMonitor_1_PGOOD_CTRL_9_16_MASK       (0x00u)
    #define PowerMonitor_1_PGOOD_CTRL_1_8_MASK        ((1u <<PowerMonitor_1_NUM_CONVERTERS) - 1)
#endif /* (PowerMonitor_1_NUM_CONVERTERS > PowerMonitor_1_CONVERTERS_24) */

/* PGOOD configuration related defines */
#define PowerMonitor_1_PGOOD_GLOBAL                  (0u)
#define PowerMonitor_1_PGOOD_INDIVIDUAL              (1u)

/* Defines for AMux Current channel numbers */
#define PowerMonitor_1_PGA_OUT_CHAN                  (37u)
#define PowerMonitor_1_VN_VSS_CHAN                   (36u)
#define PowerMonitor_1_AUX_IN_CHAN                   (32u)

/* Defines for AMux Voltage channel numbers */
#define PowerMonitor_1_CAL_IN_CHAN                   (36u)
#define PowerMonitor_1_CSA_IN_CHAN                   (37u)

/* ADC register related defines */
#define PowerMonitor_1_ADC_VN_VSSA                   (0x40u)
#define PowerMonitor_1_ADC_VN_VSSA_VP_VSSA           (0x44u)
#define PowerMonitor_1_ADC_VN_VREF_VP_VSSA           (0x24u)
#define PowerMonitor_1_ADC_S12_ON                    (0x10u)
#define PowerMonitor_1_ADC_VP_VSSA                   (0x04u)
#define PowerMonitor_1_ADC_VN_ABUS1                  (0x02u)

/* PGA register related defines */
#define PowerMonitor_1_PGA_OUT_ABUS1                 (0x02u)

/* Voltage scale for auxiliary voltage measurement */
#define PowerMonitor_1_VOLTAGE_SCALE                 (1000u)


/******************************************************************************
*        Registers
******************************************************************************/
/* Control register to generate warn, fault and eoc signal */
#define PowerMonitor_1_CONTROL1_REG        \
                    (* (reg8 *) PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__CONTROL_REG)
#define PowerMonitor_1_CONTROL1_PTR        \
                    ( (reg8 *) PowerMonitor_1_B_PowerMonitor_SyncCtl_ctrlreg1__CONTROL_REG)
/* Control register to generate pgood signal for converters between 1 and 8 */
#define PowerMonitor_1_PGOOD_CONTROL1_REG  \
                    (* (reg8 *) PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__CONTROL_REG)
#define PowerMonitor_1_PGOOD_CONTROL1_PTR  \
                    ( (reg8 *) PowerMonitor_1_B_PowerMonitor_Sync_PM_1_8_Ctrl1__CONTROL_REG)
/* Control register to generate pgood signal for cnverters between 9 and 16 */
#define PowerMonitor_1_PGOOD_CONTROL2_REG  \
                    (* (reg8 *) PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__CONTROL_REG)
#define PowerMonitor_1_PGOOD_CONTROL2_PTR  \
                    ( (reg8 *) PowerMonitor_1_B_PowerMonitor_Sync_PM_9_16_Ctrl2__CONTROL_REG)
/* Control register to generate pgood signal for converters between 17 and 24 */
#define PowerMonitor_1_PGOOD_CONTROL3_REG  \
                    (* (reg8 *) PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__CONTROL_REG)
#define PowerMonitor_1_PGOOD_CONTROL3_PTR  \
                    ( (reg8 *) PowerMonitor_1_B_PowerMonitor_Sync_PM_17_24_Ctrl3__CONTROL_REG)
/* Control register to generate pgood signal for converters between 15 and 32 */
#define PowerMonitor_1_PGOOD_CONTROL4_REG  \
                    (* (reg8 *) PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__CONTROL_REG)
#define PowerMonitor_1_PGOOD_CONTROL4_PTR  \
                    ( (reg8 *) PowerMonitor_1_B_PowerMonitor_Sync_PM_25_32_Ctrl4__CONTROL_REG)

/* Interrupt macroses */
#ifdef __C51__
    #define PowerMonitor_1_IntDisable(irqNum) \
       ((reg8*)CY_INT_CLEAR_PTR)[((CY_INT_NUMBER_MASK & irqNum) >> 3)] = \
                   (uint8)1u << (0x07 & irqNum)
#else
    #define PowerMonitor_1_IntDisable(irqNum) \
         *(uint32*)CY_INT_CLEAR_PTR = \
                         (uint32)1u << (CY_INT_NUMBER_MASK & irqNum);
#endif /* __C51__ */

#ifdef __C51__
   #define PowerMonitor_1_IntEnable(irqNum) \
      ((reg8*)CY_INT_ENABLE_PTR)[((CY_INT_NUMBER_MASK & irqNum) >> 3)] = \
                   (uint8)1u << (0x07 & irqNum);
#else
   #define PowerMonitor_1_IntEnable(irqNum) \
         *(uint32*)CY_INT_ENABLE_PTR = \
                         (uint32)1u << (CY_INT_NUMBER_MASK & irqNum);
#endif /* __C51__ */

#define PowerMonitor_1_PGA_SW10_REG                  (* (reg8 *) PowerMonitor_1_PGA_SC__SW10 )


/* Obsolete defines for backward compatibility.
*  Should not be used in new designs.
*/
#define PowerMonitor_1_NUMBER_OF_CONVERTERS        PowerMonitor_1_NUM_CONVERTERS

#endif /* CY_POWERMONITOR_PowerMonitor_1_H */


/* [] END OF FILE */
