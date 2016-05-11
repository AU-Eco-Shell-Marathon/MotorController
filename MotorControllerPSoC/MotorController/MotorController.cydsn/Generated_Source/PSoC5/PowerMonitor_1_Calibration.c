/*******************************************************************************
* File Name: PowerMonitor_1_Calibration.c
* Version 1.60
*
* Description:
*  This file provides the source code to the calibration API for the
*  Power Monitor Component.
*
* Note:
*
*******************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PowerMonitor_1.h"


/* Next Calibration type to run.*/
static uint8 PowerMonitor_1_calType;

/*******************************************************************************
* Configuration #1 : 2048 mV differential range
*      adcZeroCfg1  = units are ADC counts when both inputs are at Vssa
*      adcGainCfg1  = units are (1 mV) / (ADC count)
*      adcSeAdjCfg1 = units are mV from 2xVref PGA (added to all voltage rails)
*******************************************************************************/
int16 PowerMonitor_1_adcZeroCfg1;             /* nominally 0 counts       */
float PowerMonitor_1_adcGainCfg1;             /* nominally 1.0 mV/ct      */
int16 PowerMonitor_1_adcSeAdjCfg1;            /* nominally 2048 mV        */

/*******************************************************************************
* Configuration #2 : differential range:
*          adcZeroCfg2 = units are ADC counts when both inputs are at Vssa
*          adcGainCfg2 = units are (100 uV) / (ADC count)
*******************************************************************************/
int16 PowerMonitor_1_adcZeroCfg2;             /* nominally 0 counts         */
float PowerMonitor_1_adcGainCfg2;             /* nominally 0.03125 mV/ct @ 64 mV range */
                                                /*           0.06250 mV/ct @128 mV range */
float PowerMonitor_1_adcVoltCfg2;


/*******************************************************************************
* Configuration #3 : 1024 mV differential range
*          adcZeroCfg3 = units are ADC counts when both inputs are at Vssa
*          adcGainCfg3 = units are (1 mV) / (ADC count)
*******************************************************************************/
int16 PowerMonitor_1_adcZeroCfg3;             /* nominally 0 counts       */
float PowerMonitor_1_adcGainCfg3;             /* nominally 0.5 mV/ct      */
int16 PowerMonitor_1_adcSeAdjCfg3;            /* nominally 1024 mV        */

/* Variables to hold filtered ADC raw data */
CYBIT PowerMonitor_1_iirInit;


static int32 CYXDATA PowerMonitor_1_filtZeroRaw1;
static int32 CYXDATA PowerMonitor_1_filtGainRaw1;
static int32 CYXDATA PowerMonitor_1_filtSCAdjRaw1;

static int32 CYXDATA PowerMonitor_1_filtZeroRaw2;
static int32 CYXDATA PowerMonitor_1_filtGainRaw2a;
static int32 CYXDATA PowerMonitor_1_filtGainRaw2b;

static int32 CYXDATA PowerMonitor_1_filtZeroRaw3;
static int32 CYXDATA PowerMonitor_1_filtGainRaw3;
static int32 CYXDATA PowerMonitor_1_filtSCAdjRaw3;

/* Calibration related variables */
int16 PowerMonitor_1_cal2a_raw[PowerMonitor_1_CAL2A_FILT_SZ];
uint8 PowerMonitor_1_cal2a_idx = 0u;
int16 PowerMonitor_1_cal2a_tot;


/*******************************************************************************
* Local function prototypes
*******************************************************************************/
static void  PowerMonitor_1_RequestAdcCal(uint8 iirInit) ;
static float PowerMonitor_1_iirFiltAdc(uint8 iirInit, int16 newData, int32 CYXDATA *filtVal)
                                                           ;


/*******************************************************************************
* Function Name: PowerMonitor_1_Calibrate()
********************************************************************************
*
* Summary:
*  Initialize calibration after reset by running a quick series of
*  various calibration operations .
*
* Parameters:
*  None
*
* Return:
*  None
*
* Theory:
*  IDEAL CALIBRATION VALUES
*
* Ideal raw values from the ADC
*   PowerMonitor_1_adcZeroCalRawCfg1  = 0;
*   PowerMonitor_1_adcGainCalRawCfg1  = -1024;    Vref measured on +/- range
*   PowerMonitor_1_adcSCCalRawCfg1    = -2048;    Vref * 2
*   PowerMonitor_1_adcZeroCalRawCfg2  = 0;
*   PowerMonitor_1_adcZeroCalRawCfg3  = 0;
*   PowerMonitor_1_adcGainCalRawCfg3  = -2048;    Vref on +/-1024 mV range.
*   PowerMonitor_1_adcSCCalRawCfg3    = -2048;    Vref
*
* Ideal post-processed values:
*   PowerMonitor_1_adcZeroCfg1        = 0;        units are adc counts
*   PowerMonitor_1_adcGainCfg1        = 1.0;      units are 1 mV/count
*   PowerMonitor_1_adcSeAdjCfg1       = 2048;     units are mV
*   PowerMonitor_1_adcZeroCfg2        = 0;        units are adc counts
*   PowerMonitor_1_adcGainCfg2        = 0.03125;  1 mV/count at +/- 64 mV
*                                                   1 mV/count at +/-128 mV
*   PowerMonitor_1_adcZeroCfg3        = 0;        units are adc counts
*   PowerMonitor_1_adcGainCfg3        = 0.5;      units are 1 mV/count
*   PowerMonitor_1_adcSeAdjCfg3       = 1024;     units are mV
*
*******************************************************************************/
void PowerMonitor_1_Calibrate(void) 
{
    #if (PowerMonitor_1_CAL_PIN_EXPOSED)
        uint8 ivar;
    #endif /* PowerMonitor_1_CAL_PIN_EXPOSED */

    /***************************************************************************
    * Measure Config #1 (+/-2048 mV) Zero and Gain
    ***************************************************************************/
    PowerMonitor_1_calType = PowerMonitor_1_CAL_CFG1Z;
    PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);
    /* Measure Vref w/2048mV range  */
    PowerMonitor_1_calType = PowerMonitor_1_CAL_CFG1G;
    PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);

    /***************************************************************************
    * Measure 2xVref PGA voltage
    * Will be added to all voltage measurements as Config #1 offset)
    ***************************************************************************/
    PowerMonitor_1_calType = PowerMonitor_1_CAL_PGA1Z;
    PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);

    /***************************************************************************
    * Measure Config #3 (+/-1024 mV) Zero and Gain
    ***************************************************************************/
    PowerMonitor_1_calType = PowerMonitor_1_CAL_CFG3Z;
    PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);
    PowerMonitor_1_calType = PowerMonitor_1_CAL_CFG3G;  /* Measure Vref w/1024mV range */
    PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);
    PowerMonitor_1_calType = PowerMonitor_1_CAL_PGA3Z;
    PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);

    /***************************************************************************
    * Measure Config #2 (differential low-voltage range) Zero and Gain
    ***************************************************************************/
    PowerMonitor_1_calType = PowerMonitor_1_CAL_CFG2Z;
    PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);

    #if (PowerMonitor_1_CAL_PIN_EXPOSED)

        /* Measure user input on +/-1024 mV range */
        PowerMonitor_1_calType = PowerMonitor_1_CAL_CFG2Ga;
        PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);

           /***********************************************************************
            * Gather enough sample to render a complete oversampled value
            *  when mesuring ~50mV on +/-1024 mV range
            ***********************************************************************/

            for (ivar = 0u; ivar < (1u << PowerMonitor_1_CAL2A_OVER_EXP); ivar++)
            {
                PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);
            }

        PowerMonitor_1_calType = PowerMonitor_1_CAL_CFG2Gb; /* Measure cal input on low-voltage range */
        PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);

    #else
        /* adcGain for config 2 */
        PowerMonitor_1_calType = PowerMonitor_1_CAL_CFG2G;
        PowerMonitor_1_RequestAdcCal(PowerMonitor_1_iirInit);

    #endif /* PowerMonitor_1_CAL_PIN_EXPOSED */
}


/*******************************************************************************
* Function Name: PowerMonitor_1_RequestAdcCal()
********************************************************************************
*
* Summary:
*  Punctuate normal ADC sampling process for selected calibration operation.
*
* Parameters:
*  iirInit = initialize the filter with this reading
*
* Return:
*  Appropriate IIR filter
*
* Side Effects:
*  Waits for ADC interrupt to complete the requested raw calibration operation.
*  THIS IS A CANDIATE FOR A TWO-STATE PROCESS to return to main() with minimal
*  delay.
*
*******************************************************************************/
static void PowerMonitor_1_RequestAdcCal(uint8 iirInit) 
{
    int16 tmpInt;
    float nominalGain;

    /* Save which sample was next in sequence before calibration interruption */
    PowerMonitor_1_IntDisable(PowerMonitor_1_IRQ__INTC_NUMBER);

    PowerMonitor_1_adcConvNextPreCal = PowerMonitor_1_adcConvNext;
    PowerMonitor_1_adcConvNext       = PowerMonitor_1_STATE_CAL;
    PowerMonitor_1_adcConvCalType    = PowerMonitor_1_calType;

    PowerMonitor_1_IntEnable (PowerMonitor_1_IRQ__INTC_NUMBER);

    /***************************************************************************
    * WAIT FOR ADC ISR TO FINISH GETTING RAW CALIBRATION VALUE.
    * (ADC has cleared calibration process and resumed normal sampling
    ***************************************************************************/
    while ((PowerMonitor_1_adcConvNext == PowerMonitor_1_STATE_CAL) ||   /* wait start */
           (PowerMonitor_1_adcConvNow  == PowerMonitor_1_STATE_CAL));    /* wait finish*/

    /***************************************************************************
    * Process "raw" ADC value from the ISR to something useful.
    ***************************************************************************/
    switch (PowerMonitor_1_calType)
    {
        /* Calibrate ADC Configuration #1 (+/-2048 mV range) */
        case PowerMonitor_1_CAL_CFG1Z:
            PowerMonitor_1_adcZeroCfg1 = (int16)PowerMonitor_1_iirFiltAdc(iirInit,
                                                            PowerMonitor_1_adcZeroCalRawCfg1,
                                                            &PowerMonitor_1_filtZeroRaw1);
            break;

        case PowerMonitor_1_CAL_CFG1G:
            /***********************************************************************
            * Correct for:   a) Route inversion
            *                b) Cfg #1 zero offset
            ***********************************************************************/
            tmpInt  = (int16)PowerMonitor_1_iirFiltAdc(iirInit,PowerMonitor_1_adcGainCalRawCfg1,
                                                         &PowerMonitor_1_filtGainRaw1);
            tmpInt  = -tmpInt;
            tmpInt -= PowerMonitor_1_adcZeroCfg1;
            PowerMonitor_1_adcGainCfg1 = 1024.0 / tmpInt;
            break;

        case PowerMonitor_1_CAL_PGA1Z:
            /***********************************************************************
            * Calibrate 2048 mV PGA voltage (added to all voltage measurements)
            * Ideal = -2048 counts raw
            ***********************************************************************/
            /***********************************************************************
            * Correct for:   a) Route inversion
            *                b) Cfg #1 zero offset
            ***********************************************************************/
            tmpInt  = (int16)PowerMonitor_1_iirFiltAdc(iirInit, PowerMonitor_1_adcSCCalRawCfg1,
                                                         &PowerMonitor_1_filtSCAdjRaw1);
            tmpInt  = -tmpInt;
            tmpInt -= PowerMonitor_1_adcZeroCfg1;
            PowerMonitor_1_adcSeAdjCfg1 = (int16)(tmpInt * PowerMonitor_1_adcGainCfg1);
            break;

        /***************************************************************************
        * Calibrate Config #2 (low-voltage range)
        * Use VDAC to generate calibration signal in the proper range, then
        *   A) Measure VDAC on   calibrated 1024 mV range (ideal  120 counts raw)
        *   B) Measure VDAC on uncalibrated  low mV range (ideal 1920 counts raw)
        ***************************************************************************/
        case PowerMonitor_1_CAL_CFG2Z:
            PowerMonitor_1_adcZeroCfg2 = (int16)PowerMonitor_1_iirFiltAdc(iirInit,
                                                                      PowerMonitor_1_adcZeroCalRawCfg2,
                                                                       &PowerMonitor_1_filtZeroRaw2);
            break;

        case PowerMonitor_1_CAL_CFG2Ga:
            /***********************************************************************
            * The input cal signal is measured on Config #4 range and the
             * result is result is subsequently used when measuring on
            * low-voltage range (not now) Oversample 12-bit +/-1024 mV range to
            * 14-bit resolution to avoid introducing error when applying to
            * low voltage range.
            *******************************************************************/
            PowerMonitor_1_cal2a_tot -= PowerMonitor_1_cal2a_raw[PowerMonitor_1_cal2a_idx];
            PowerMonitor_1_cal2a_tot += PowerMonitor_1_adcGainCalRawCfg2a;
            PowerMonitor_1_cal2a_raw[PowerMonitor_1_cal2a_idx] = PowerMonitor_1_adcGainCalRawCfg2a;
            if (++PowerMonitor_1_cal2a_idx >= PowerMonitor_1_CAL2A_FILT_SZ)
            {
                /***************************************************************
                * Store new adcGainRaw2a (14-bit resolution, ~125 uV/ct)
                ***************************************************************/
                PowerMonitor_1_filtGainRaw2a = PowerMonitor_1_cal2a_tot / 4;
                PowerMonitor_1_cal2a_idx = 0u;
            }
            break;

        case PowerMonitor_1_CAL_CFG2Gb:
            /* #if OVERSAMPLE_CAL2a */
            PowerMonitor_1_adcVoltCfg2  = (float)PowerMonitor_1_filtGainRaw2a / 4;
            PowerMonitor_1_adcVoltCfg2 -= PowerMonitor_1_adcZeroCfg3;
            PowerMonitor_1_adcVoltCfg2 *= PowerMonitor_1_adcGainCfg3;   /* mV VDAC */


            /***********************************************************************
            * Supervisor_adcVdacCfg2 is the measured "test" VDAC output level in mV
            *  (as measured with the 1024 mV ADC range)
            *
            *                 tmpInt = filtered counts of VDAC on low-voltage range
            * Supervisor_adcGainCfg2 = floating point mV/count on low-voltage range
            ***********************************************************************/
            tmpInt  = (int16)PowerMonitor_1_iirFiltAdc(iirInit, PowerMonitor_1_adcGainCalRawCfg2b,
                                                  &PowerMonitor_1_filtGainRaw2b);
            tmpInt -= PowerMonitor_1_adcZeroCfg2;
            PowerMonitor_1_adcGainCfg2 = PowerMonitor_1_adcVoltCfg2 / tmpInt;
            break;

        case PowerMonitor_1_CAL_CFG2G:
            #if (PowerMonitor_1_DEFAULT_DIFF_CURRENT_RANGE == PowerMonitor_1_DIFF_CURRENT_RANGE_64MV)
                nominalGain = 0.03125; /* Nominal gain for 64mV ADC range */
            #else
                nominalGain = 0.06250; /* Nominal gain for 128mV ADC range */
            #endif /* (PowerMonitor_1_DEFAULT_DIFF_CURRENT_RANGE == PowerMonitor_1_DIFF_CURRENT_RANGE_64MV) */
                PowerMonitor_1_adcGainCfg2 = nominalGain; /* PowerMonitor_1_adcZeroCfg2; */
            break;


        /***************************************************************************
        * Calibrate Config #3 (+/-1024 mV range)  Ideal = -2048 counts raw
        ***************************************************************************/
        case PowerMonitor_1_CAL_CFG3Z:
            PowerMonitor_1_adcZeroCfg3 = (int16)PowerMonitor_1_iirFiltAdc(iirInit,
                                                            PowerMonitor_1_adcZeroCalRawCfg3,
                                                            &PowerMonitor_1_filtZeroRaw3);
            break;

        case PowerMonitor_1_CAL_CFG3G:
            /***********************************************************************
            * Correct for:   a) Route inversion
            *                b) Cfg #3 zero offset
            ***********************************************************************/
            tmpInt  = (int16)PowerMonitor_1_iirFiltAdc(iirInit, PowerMonitor_1_adcGainCalRawCfg3,
                                                  &PowerMonitor_1_filtGainRaw3);
            tmpInt  = -tmpInt;
            tmpInt -= PowerMonitor_1_adcZeroCfg3;
            /* Correct for floating mV/count (tmpInt reads perfect 1024.0 mV)     */
            PowerMonitor_1_adcGainCfg3 = 1024.0 / tmpInt;   /* floating mV/count      */
            break;

        case PowerMonitor_1_CAL_PGA3Z:
            /***********************************************************************
            * Calibrate 1024 mV ref voltage (added to all voltage measurements)
            * Ideal = -2048 counts raw
            ***********************************************************************/
            /***********************************************************************
            * Correct for:   a) Route inversion
            *                b) Cfg #1 zero offset
            ***********************************************************************/
            tmpInt  = (int16)PowerMonitor_1_iirFiltAdc(iirInit, PowerMonitor_1_adcSCCalRawCfg3,
                                                  &PowerMonitor_1_filtSCAdjRaw3);
            tmpInt  = -tmpInt;
            tmpInt -= PowerMonitor_1_adcZeroCfg3;
            PowerMonitor_1_adcSeAdjCfg3 = (int16)(tmpInt * PowerMonitor_1_adcGainCfg3);
            break;
        default:
            break;
    }
}


/*******************************************************************************
* Function Name: PowerMonitor_1_iirFiltAdc()
********************************************************************************
*
* Summary:
*  Recursive Infinite Impulse Response (IIR) filter for raw calibration
*  readings.
*  Post new data to filter and return filtered result
*
* Parameters:
*  *filtVal = ptr to desired IIR filter variable
*  newData = new reading to be added to filter
*
* Return:
*  filtered result
*
* Theory:
*  The values in each IIR filter register is the (average * SCALE_VAL).
*
*  FILTER_SHIFT  RiseTime Samples     FILTER_SHIFT  RiseTime Samples
*             1 -> 3                              4  -> 34
*             2 -> 8                              5  -> 69
*             3 -> 16                             6  -> 140
*
*******************************************************************************/
static float PowerMonitor_1_iirFiltAdc(uint8 iirInit, int16 newData, int32 CYXDATA *filtVal)
                                                            
{
    int32 tmpFilt;
    int32 junk;
    /* If filter should be initialized to newData */
    if (iirInit != 0u)
    {
        /* start filter with newData */
        tmpFilt = (int32)newData * PowerMonitor_1_SCALE_VAL;
    }
    /* Else filter should incorporate newData */
    else
    {
          tmpFilt = *filtVal;
          junk = (tmpFilt + PowerMonitor_1_SCALE_VAL / 2);
          junk /= PowerMonitor_1_SCALE_VAL;
          tmpFilt = tmpFilt - junk + newData;
    }
    *filtVal = tmpFilt;
    return ((float)tmpFilt / PowerMonitor_1_SCALE_VAL);
}


/* [] END OF FILE */
