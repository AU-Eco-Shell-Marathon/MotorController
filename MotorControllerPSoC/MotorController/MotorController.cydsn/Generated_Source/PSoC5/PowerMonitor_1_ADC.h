/*******************************************************************************
* File Name: PowerMonitor_1_ADC.h  
* Version 2.30
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Delta-Sigma ADC user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_ADC_DELSIG_PowerMonitor_1_ADC_H) 
#define CY_ADC_DELSIG_PowerMonitor_1_ADC_H 

#include "cytypes.h"
#include "cylib.h"
#include "cyfitter.h"
#include "cydevice_trm.h" 

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component ADC_DelSig_v2_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct PowerMonitor_1_ADC_backupStruct 
{
    uint8 enableState;
    uint8 deccr;
}   PowerMonitor_1_ADC_BACKUP_STRUCT;


/***************************************
*       Type defines
***************************************/

/* only valid for PSoCA */
#if (CY_PSOC5A)
    /* Power Mode register backup Support */
    typedef struct PowerMonitor_1_ADC_powerModeBackupStruct 
    {
        uint8 DSM_CR_14;
        uint8 DSM_CR_15;
        uint8 DSM_CR_16;
        uint8 DSM_CR_17;
        uint8 DSM_REF0;
        uint8 DSM_BUF0;
        uint8 DSM_BUF1;
        uint8 DSM_CLK;
        /* field to decide whether or not to save the registers 
           in the stop API*/
        uint8 bypassRestore;
    }   PowerMonitor_1_ADC_POWERMODE_BACKUP_STRUCT;
#endif /* CY_PSOC5A */


/***************************************
*        Function Prototypes 
***************************************/

void PowerMonitor_1_ADC_Start(void) ;
void PowerMonitor_1_ADC_Stop(void) ;
void PowerMonitor_1_ADC_Sleep(void) ;
void PowerMonitor_1_ADC_Wakeup(void) ;
void PowerMonitor_1_ADC_Init(void) ;   
void PowerMonitor_1_ADC_Enable(void) ;
void PowerMonitor_1_ADC_SaveConfig(void) ;
void PowerMonitor_1_ADC_RestoreConfig(void) ;
void PowerMonitor_1_ADC_SetBufferGain(uint8 gain) ;
void PowerMonitor_1_ADC_StartConvert(void) ;
void PowerMonitor_1_ADC_SetCoherency(uint8 coherency) ;
uint8 PowerMonitor_1_ADC_SetGCOR(float gainAdjust) ;
uint16 PowerMonitor_1_ADC_ReadGCOR(void) ;
void PowerMonitor_1_ADC_StopConvert(void) ;
void PowerMonitor_1_ADC_SetOffset(int32 offset) ; 
void PowerMonitor_1_ADC_SetGain(int32 adcGain) ; 
int8 PowerMonitor_1_ADC_GetResult8(void) ;
int16 PowerMonitor_1_ADC_GetResult16(void) ;
int32 PowerMonitor_1_ADC_GetResult32(void) ;
uint8 PowerMonitor_1_ADC_IsEndConversion(uint8 retMode) ;
int16 PowerMonitor_1_ADC_CountsTo_mVolts( int32 adcCounts) ; 
float PowerMonitor_1_ADC_CountsTo_Volts( int32 adcCounts) ; 
int32 PowerMonitor_1_ADC_CountsTo_uVolts( int32 adcCounts) ;
CY_ISR_PROTO(PowerMonitor_1_ADC_ISR1);
CY_ISR_PROTO(PowerMonitor_1_ADC_ISR2);
CY_ISR_PROTO(PowerMonitor_1_ADC_ISR3);
CY_ISR_PROTO(PowerMonitor_1_ADC_ISR4);
void PowerMonitor_1_ADC_SelectConfiguration(uint8 config, uint8 restart) \
                                           ;
void PowerMonitor_1_ADC_InitConfig(uint8 config) ;
void PowerMonitor_1_ADC_GainCompensation(uint8 inputRange, uint16 IdealDecGain, uint16 IdealOddDecGain, uint8 resolution)\
                                              ;
#if (CY_PSOC5A)
    void PowerMonitor_1_ADC_SavePowerRegisters(void) ;
    void PowerMonitor_1_ADC_RestorePowerRegisters(void) ;
    void PowerMonitor_1_ADC_SetLowPower(void) ;
#endif /* CY_PSOC5A */
uint16 PowerMonitor_1_ADC_RoundValue(uint32 busClockFreq, uint32 clockFreq) \
                                              ;
void PowerMonitor_1_ADC_SetDSMRef0Reg(uint8 value) ;

/* These functions may not be supported in the released version */
void    PowerMonitor_1_ADC_SetBufferChop(uint8 chopen, uint8 chopFreq) \
                                                  ;
#define PowerMonitor_1_ADC_IRQ_Enable() CyIntEnable(PowerMonitor_1_ADC_IRQ__INTC_NUMBER ); 
#define PowerMonitor_1_ADC_IRQ_Disable() CyIntDisable(PowerMonitor_1_ADC_IRQ__INTC_NUMBER );


/*******************************************************************************
* The following API?s are to support legacy code.  The IRQ_Start
* And IRQ_Stop routine have been replaced by the library source code
* for interrupts. IRQ_Start() and IRQ_Stop() should not be used in new
* designs
*******************************************************************************/
void PowerMonitor_1_ADC_IRQ_Start(void) ;
#define PowerMonitor_1_ADC_IRQ_Stop() CyIntDisable(PowerMonitor_1_ADC_IRQ__INTC_NUMBER )


/**************************************
*           Parameter Defaults        
**************************************/

/* Default config values from user parameters */

#define PowerMonitor_1_ADC_CFG1_RESOLUTION         (12u )
#define PowerMonitor_1_ADC_CFG1_RANGE              (5u)    
#define PowerMonitor_1_ADC_CFG1_SRATE              (10000u)         
#define PowerMonitor_1_ADC_CFG1_REFERENCE          (0u)       
#define PowerMonitor_1_ADC_CFG1_BUF_GAIN           (1u)   
#define PowerMonitor_1_ADC_CFG1_CONV_MODE          (0u)     
#define PowerMonitor_1_ADC_CFG1_RANGE_VALUE        (2.048)
#define PowerMonitor_1_ADC_DEFAULT_STROBE                     (0u) 
#define PowerMonitor_1_ADC_DEFAULT_INTERNAL_CLK               (1u)           
#define PowerMonitor_1_ADC_DEFAULT_NUM_CONFIGS                (4u)    
#define PowerMonitor_1_ADC_DEFAULT_CHARGE_PUMP_CLOCK          (1u)    
#define PowerMonitor_1_ADC_DEFAULT_INPUT_MODE                 (0u)

/* Config Constants which can be used as the "config" parameter of
   PowerMonitor_1_ADC_SelectConfiguration() API. */
#define PowerMonitor_1_ADC_CFG1        1
#define PowerMonitor_1_ADC_CFG2        2
#define PowerMonitor_1_ADC_CFG3        3
#define PowerMonitor_1_ADC_CFG4        4


/**************************************
*           API Constants        
**************************************/

/* Component's enable/disable state */
#define PowerMonitor_1_ADC_ENABLED                    (0x01u)
#define PowerMonitor_1_ADC_DISABLED                   (0x00u)

/* Defines for deciding whether or not to restore the 
   power registers in Enable API. Valid only for PSoC5A*/
#if (CY_PSOC5A)
    #define PowerMonitor_1_ADC_BYPASS_ENABLED                    (0x01u)
    #define PowerMonitor_1_ADC_BYPASS_DISABLED                   (0x00u)
#endif /* CY_PSOC5A */

/* Ideal Gain Constant */
#define PowerMonitor_1_ADC_IDEAL_GAIN_CONST           (0x8000u)

/* Gvalue constant */
#define PowerMonitor_1_ADC_MAX_GVAL                   (0x0Fu)

/* Gvalue constants based on resolution */
#define PowerMonitor_1_ADC_GVAL_RESOLUTIN_8           (0x09u)
#define PowerMonitor_1_ADC_GVAL_RESOLUTIN_9           (0x0Au)
#define PowerMonitor_1_ADC_GVAL_RESOLUTIN_10          (0x0Bu)
#define PowerMonitor_1_ADC_GVAL_RESOLUTIN_11          (0x0Cu)
#define PowerMonitor_1_ADC_GVAL_RESOLUTIN_12          (0x0Du)
#define PowerMonitor_1_ADC_GVAL_RESOLUTIN_13          (0x0Eu)

/* Constants for SetPower(), "power" paramter */
#define PowerMonitor_1_ADC_LOWPOWER                   (0x00u) 
#define PowerMonitor_1_ADC_MEDPOWER                   (0x01u) 
#define PowerMonitor_1_ADC_HIGHPOWER                  (0x02u) 
#define PowerMonitor_1_ADC_HIGHPOWER_1_5              (0x03u)  /* Reserved */
#define PowerMonitor_1_ADC_HIGHPOWER_2_0              (0x04u)  /* Reserved */
#define PowerMonitor_1_ADC_HIGHPOWER_2_5              (0x07u)  /* Reserved */
#define PowerMonitor_1_ADC_POWER_MASK                 (0x07u) 

/* Constants for SetBufferGain() "gain" paramter */
#define PowerMonitor_1_ADC_BUF_GAIN_1X                (0x00u) 
#define PowerMonitor_1_ADC_BUF_GAIN_2X                (0x01u) 
#define PowerMonitor_1_ADC_BUF_GAIN_4X                (0x02u) 
#define PowerMonitor_1_ADC_BUF_GAIN_8X                (0x03u) 
#define PowerMonitor_1_ADC_BUF_GAIN_MASK              (0x03u)

/* Constants for IsEndConversion() "retMode" parameter */
#define PowerMonitor_1_ADC_RETURN_STATUS              (0x01u) 
#define PowerMonitor_1_ADC_WAIT_FOR_RESULT            (0x00u)

/* Constants for input range parameter */

#define PowerMonitor_1_ADC_IR_VSSA_TO_VREF            (0x00u)
#define PowerMonitor_1_ADC_IR_VSSA_TO_2VREF           (0x01u)
#define PowerMonitor_1_ADC_IR_VSSA_TO_VDDA            (0x02u)
#define PowerMonitor_1_ADC_IR_VSSA_TO_6VREF           (0x03u)
#define PowerMonitor_1_ADC_IR_VNEG_VREF_DIFF          (0x04u)
#define PowerMonitor_1_ADC_IR_VNEG_2VREF_DIFF         (0x05u)
#define PowerMonitor_1_ADC_IR_VNEG_6VREF_DIFF         (0x06u)
#define PowerMonitor_1_ADC_IR_VNEG_VREF_2_DIFF        (0x07u)
#define PowerMonitor_1_ADC_IR_VNEG_VREF_4_DIFF        (0x08u)
#define PowerMonitor_1_ADC_IR_VNEG_VREF_8_DIFF        (0x09u)
#define PowerMonitor_1_ADC_IR_VNEG_VREF_16_DIFF       (0x0Au)

/* Constants for ADC reference parameter */

#define PowerMonitor_1_ADC_INT_REF_NOT_BYPASSED       (0x00u)
#define PowerMonitor_1_ADC_INT_REF_BYPASSED_ON_P03    (0x01u)
#define PowerMonitor_1_ADC_INT_REF_BYPASSED_ON_P32    (0x02u)
#define PowerMonitor_1_ADC_EXT_REF_ON_P03             (0x03u)
#define PowerMonitor_1_ADC_EXT_REF_ON_P32             (0x04u)
#define PowerMonitor_1_ADC_INT_REF_VDDA_4             (0x05u)
#define PowerMonitor_1_ADC_INT_REF_VDDA_3             (0x06u)

/* Priority of the ADC_IRQ interrupt. */
#define PowerMonitor_1_ADC_IRQ_INTC_PRIOR_NUMBER      PowerMonitor_1_ADC_IRQ__INTC_PRIOR_NUM

/* Constants for ADC coherency parameter */

#define PowerMonitor_1_ADC_COHER_LOW                  (0x01u) 
#define PowerMonitor_1_ADC_COHER_MID                  (0x02u) 
#define PowerMonitor_1_ADC_COHER_HIGH                 (0x03u)

/* Trim value defines */

#if (CY_PSOC3 || CY_PSOC5LP)
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_DIFF_8_15        (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__M5))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_DIFF_16_20       (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__M1))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_2_DIFF_8_15      (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__M6))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_2_DIFF_16_20     (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__M2))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_4_DIFF_8_15      (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__M7))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_4_DIFF_16_20     (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__M3))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_16_DIFF_8_15     (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__M8))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_16_DIFF_16_20    (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__M4))
#elif (CY_PSOC5A)
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_DIFF_8_15        (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__16L))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_DIFF_16_20       (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__1L))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_2_DIFF_8_15      (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__16H))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_2_DIFF_16_20     (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__1H))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_4_DIFF_8_15      (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__P25L))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_4_DIFF_16_20     (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__4L))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_16_DIFF_8_15     (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__P25H))
    #define PowerMonitor_1_ADC_DEC_TRIM_VREF_16_DIFF_16_20    (CY_GET_XTND_REG8(PowerMonitor_1_ADC_DEC__TRIM__4H))

#endif /* CY_PSOC5A */


/**************************************
*      Unsupported API Constants        
**************************************/

/* Constants for SetBuffer() "bufMode" parameter */   
#define PowerMonitor_1_ADC_BUF_DIFF_MODE              (0x00u)  /* Differential mode */
#define PowerMonitor_1_ADC_BUF_BYPASS_POS             (0x04u)  /* Bypass and power down positive channel */
#define PowerMonitor_1_ADC_BUF_BYPASS_NEG             (0x08u)  /* Bypass and power down negative channel */
#define PowerMonitor_1_ADC_BUF_RAIL_TO_RAIL           (0x10u)  /* Enables Rail-to-rail mode */
#define PowerMonitor_1_ADC_BUF_FILTER_EN              (0x20u)  /* Output filter enable */
#define PowerMonitor_1_ADC_BUF_LOW_PWR                (0x40u)  /* Enable  Low power mode */

/* Constants for SetInputRange() "inputMode" parameter */
#define PowerMonitor_1_ADC_INMODE_VSSA_TO_VREF        (0x00u) 
#define PowerMonitor_1_ADC_INMODE_VSSA_TO_2VREF       (0x01u) 
#define PowerMonitor_1_ADC_INMODE_VSSA_TO_VDDA        (0x02u) 
#define PowerMonitor_1_ADC_INMODE_VNEG_PM_VREF_DIFF   (0x03u) 
#define PowerMonitor_1_ADC_INMODE_VNEG_PM_2VREF_DIFF  (0x04u)

/* Constants for SetBufferChop() "chopen" parameter */
#define PowerMonitor_1_ADC_BUF_CHOP_ENABLE            (0x01u)
#define PowerMonitor_1_ADC_BUF_CHOP_DISABLE           (0x00u)

/* Constants for SetBufferChop() "chopFreq" parameter */
#define PowerMonitor_1_ADC_BUF_CHOP_FREQ_FS2          (0x00u) 
#define PowerMonitor_1_ADC_BUF_CHOP_FREQ_FS4          (0x01u) 
#define PowerMonitor_1_ADC_BUF_CHOP_FREQ_FS8          (0x02u) 
#define PowerMonitor_1_ADC_BUF_CHOP_FREQ_FS16         (0x03u) 
#define PowerMonitor_1_ADC_BUF_CHOP_FREQ_FS32         (0x04u) 
#define PowerMonitor_1_ADC_BUF_CHOP_FREQ_FS64         (0x05u) 
#define PowerMonitor_1_ADC_BUF_CHOP_FREQ_FS128        (0x06u) 
#define PowerMonitor_1_ADC_BUF_CHOP_FREQ_FS256        (0x07u) 

/* Constants for custom reference mode settings */
#define PowerMonitor_1_ADC_REF_INTREF                 (0x80u) 
#define PowerMonitor_1_ADC_REF_REFMODE_MASK           (0x07u) 
#define PowerMonitor_1_ADC_INTREF_MASK                (0x07u)

#define PowerMonitor_1_ADC_MIN_RES                    (8u)
#define PowerMonitor_1_ADC_MAX_RES                    (20u) 


/**************************************
*       Register Constants        
**************************************/


/******************************************/
/* DSM.CR0 Control Register 0 definitions */
/******************************************/

/* Bit Field  DSM_NONOV */
#define PowerMonitor_1_ADC_DSM_NONOV_MASK             (0x0Cu)
#define PowerMonitor_1_ADC_DSM_NONOV_LOW              (0x00u)
#define PowerMonitor_1_ADC_DSM_NONOV_MED              (0x04u)
#define PowerMonitor_1_ADC_DSM_NONOV_HIGH             (0x08u)
#define PowerMonitor_1_ADC_DSM_NONOV_VHIGH            (0x0Cu)

/* Bit Field  DSM_QLEV */
#define PowerMonitor_1_ADC_DSM_QLEV_MASK              (0x03u)
#define PowerMonitor_1_ADC_DSM_QLEV_2                 (0x00u)
#define PowerMonitor_1_ADC_DSM_QLEV_3                 (0x01u)
#define PowerMonitor_1_ADC_DSM_QLEV_9                 (0x02u)
#define PowerMonitor_1_ADC_DSM_QLEV_9x                (0x03u)


/******************************************/
/* DSM.CR1 Control Register 1 definitions */
/******************************************/
#define PowerMonitor_1_ADC_DSM_ODET_TH_MASK           (0x1Fu)
#define PowerMonitor_1_ADC_DSM_ODEN                   (0x20u)
#define PowerMonitor_1_ADC_DSM_DPMODE                 (0x40u)


/******************************************/
/* DSM.CR2 Control Register 2 definitions */
/******************************************/

/* Bit Field  DSM_FCHOP */
#define PowerMonitor_1_ADC_DSM_FCHOP_MASK             (0x07u)
#define PowerMonitor_1_ADC_DSM_FCHOP_DIV2             (0x00u)
#define PowerMonitor_1_ADC_DSM_FCHOP_DIV4             (0x01u)
#define PowerMonitor_1_ADC_DSM_FCHOP_DIV8             (0x02u)
#define PowerMonitor_1_ADC_DSM_FCHOP_DIV16            (0x03u)
#define PowerMonitor_1_ADC_DSM_FCHOP_DIV32            (0x04u)
#define PowerMonitor_1_ADC_DSM_FCHOP_DIV64            (0x05u)
#define PowerMonitor_1_ADC_DSM_FCHOP_DIV128           (0x06u)
#define PowerMonitor_1_ADC_DSM_FCHOP_DIV256           (0x07u)

/* Bit Field  DSM_MOD_CHOP_EN */
#define PowerMonitor_1_ADC_DSM_MOD_CHOP_EN            (0x08u)

/* Bit Field  DSM_MX_RESET */
#define PowerMonitor_1_ADC_DSM_MX_RESET               (0x80u)

/* Bit Field  DSM_RESET1_EN */
#define PowerMonitor_1_ADC_DSM_RESET1_EN              (0x10u)

/* Bit Field  DSM_RESET2_EN */
#define PowerMonitor_1_ADC_DSM_RESET2_EN              (0x20u)

/* Bit Field  DSM_RESET3_EN */
#define PowerMonitor_1_ADC_DSM_RESET3_EN              (0x40u)


/******************************************/
/* DSM.CR3 Control Register 3 definitions */
/******************************************/

/* Bit Field  DSM_SELECT_MOD_BIT */
#define PowerMonitor_1_ADC_DSM_MODBITIN_MASK          (0x0Fu)
#define PowerMonitor_1_ADC_DSM_MODBITIN_LUT0          (0x00u)
#define PowerMonitor_1_ADC_DSM_MODBITIN_LUT1          (0x01u)
#define PowerMonitor_1_ADC_DSM_MODBITIN_LUT2          (0x02u)
#define PowerMonitor_1_ADC_DSM_MODBITIN_LUT3          (0x03u)
#define PowerMonitor_1_ADC_DSM_MODBITIN_LUT4          (0x04u)
#define PowerMonitor_1_ADC_DSM_MODBITIN_LUT5          (0x05u)
#define PowerMonitor_1_ADC_DSM_MODBITIN_LUT6          (0x06u)
#define PowerMonitor_1_ADC_DSM_MODBITIN_LUT7          (0x07u)
#define PowerMonitor_1_ADC_DSM_MODBITIN_UDB           (0x08u)

/* Bit Field  DSM_MODBIT_EN */
#define PowerMonitor_1_ADC_DSM_MODBIT_EN              (0x10u)

/* Bit Field  DSM_MX_DOUT */
#define PowerMonitor_1_ADC_DSM_MX_DOUT_8BIT           (0x00u)
#define PowerMonitor_1_ADC_DSM_MX_DOUT_2SCOMP         (0x20u)

/* Bit Field  DSM_MODBIT  TBD */
#define PowerMonitor_1_ADC_DSM_MODBIT                 (0x40u)

/* Bit Field  DSM_SIGN */
#define PowerMonitor_1_ADC_DSM_SIGN_NINV              (0x00u)
#define PowerMonitor_1_ADC_DSM_SIGN_INV               (0x80u)


/******************************************/
/* DSM.CR4 Control Register 4 definitions */
/******************************************/

/* Bit Field  DSM_SELECT_FCAP_EN */
#define PowerMonitor_1_ADC_DSM_FCAP1_EN               (0x80u)
#define PowerMonitor_1_ADC_DSM_FCAP1_DIS              (0x00u)

/* Bit Field  DSM_SELECT_FCAP1 */
#define PowerMonitor_1_ADC_DSM_FCAP1_MASK             (0x7Fu)
#define PowerMonitor_1_ADC_DSM_FCAP1_MIN              (0x00u)
#define PowerMonitor_1_ADC_DSM_FCAP1_MAX              (0x7Fu)
#define PowerMonitor_1_ADC_DSM_FCAP1_100FF            (0x01u)
#define PowerMonitor_1_ADC_DSM_FCAP1_200FF            (0x02u)
#define PowerMonitor_1_ADC_DSM_FCAP1_400FF            (0x04u)
#define PowerMonitor_1_ADC_DSM_FCAP1_800FF            (0x08u)
#define PowerMonitor_1_ADC_DSM_FCAP1_1600FF           (0x10u)
#define PowerMonitor_1_ADC_DSM_FCAP1_3200FF           (0x20u)
#define PowerMonitor_1_ADC_DSM_FCAP1_6400FF           (0x40u)


/******************************************/
/* DSM.CR5 Control Register 5 definitions */
/******************************************/

/* Bit Field  DSM_SELECT_IPCAP_EN */
#define PowerMonitor_1_ADC_DSM_IPCAP1_EN              (0x80u)
#define PowerMonitor_1_ADC_DSM_IPCAP1_DIS             (0x00u)

/* Bit Field  DSM_SELECT_IPCAP1 */
#define PowerMonitor_1_ADC_DSM_IPCAP1_MASK            (0x7Fu)
#define PowerMonitor_1_ADC_DSM_IPCAP1_MIN             (0x00u)
#define PowerMonitor_1_ADC_DSM_IPCAP1_MAX             (0x7Fu)
#define PowerMonitor_1_ADC_DSM_IPCAP1_96FF            (0x01u)
#define PowerMonitor_1_ADC_DSM_IPCAP1_192FF           (0x02u)
#define PowerMonitor_1_ADC_DSM_IPCAP1_400FF           (0x04u)
#define PowerMonitor_1_ADC_DSM_IPCAP1_800FF           (0x08u)
#define PowerMonitor_1_ADC_DSM_IPCAP1_1600FF          (0x10u)
#define PowerMonitor_1_ADC_DSM_IPCAP1_3200FF          (0x20u)
#define PowerMonitor_1_ADC_DSM_IPCAP1_6400FF          (0x40u)


/******************************************/
/* DSM.CR6 Control Register 6 definitions */
/******************************************/

/* Bit Field  DSM_SELECT_DACCAP_EN */
#define PowerMonitor_1_ADC_DSM_DACCAP_EN              (0x40u)
#define PowerMonitor_1_ADC_DSM_DACCAP_DIS             (0x00u)

/* Bit Field  DSM_SELECT_DACCAP */
#define PowerMonitor_1_ADC_DSM_DACCAP_MASK            (0x3Fu)
#define PowerMonitor_1_ADC_DSM_DACCAP_MIN             (0x00u)
#define PowerMonitor_1_ADC_DSM_DACCAP_MAX             (0x3Fu)
#define PowerMonitor_1_ADC_DSM_DACCAP_96FF            (0x01u)
#define PowerMonitor_1_ADC_DSM_DACCAP_192FF           (0x02u)
#define PowerMonitor_1_ADC_DSM_DACCAP_400FF           (0x04u)
#define PowerMonitor_1_ADC_DSM_DACCAP_800FF           (0x08u)
#define PowerMonitor_1_ADC_DSM_DACCAP_1600FF          (0x10u)
#define PowerMonitor_1_ADC_DSM_DACCAP_3200FF          (0x20u)


/******************************************/
/* DSM.CR7 Control Register 7 definitions */
/******************************************/

/* Bit Field  DSM_SELECT_RESCAP_EN */
#define PowerMonitor_1_ADC_DSM_RESCAP_EN              (0x08u)
#define PowerMonitor_1_ADC_DSM_RESCAP_DIS             (0x00u)

/* Bit Field  DSM_SELECT_RESCAP */
#define PowerMonitor_1_ADC_DSM_RESCAP_MASK            (0x07u)
#define PowerMonitor_1_ADC_DSM_RESCAP_MIN             (0x00u)
#define PowerMonitor_1_ADC_DSM_RESCAP_MAX             (0x07u)
#define PowerMonitor_1_ADC_DSM_RESCAP_12FF            (0x00u)
#define PowerMonitor_1_ADC_DSM_RESCAP_24FF            (0x01u)
#define PowerMonitor_1_ADC_DSM_RESCAP_36FF            (0x02u)
#define PowerMonitor_1_ADC_DSM_RESCAP_48FF            (0x03u)
#define PowerMonitor_1_ADC_DSM_RESCAP_60FF            (0x04u)
#define PowerMonitor_1_ADC_DSM_RESCAP_72FF            (0x05u)
#define PowerMonitor_1_ADC_DSM_RESCAP_84FF            (0x06u)
#define PowerMonitor_1_ADC_DSM_RESCAP_96FF            (0x07u)

#define PowerMonitor_1_ADC_DSM_FCAP2_DIS              (0x00u)
#define PowerMonitor_1_ADC_DSM_FCAP2_EN               (0x80u)

#define PowerMonitor_1_ADC_DSM_FCAP3_DIS              (0x00u)
#define PowerMonitor_1_ADC_DSM_FCAP3_EN               (0x40u)

#define PowerMonitor_1_ADC_DSM_IPCAP1OFFSET           (0x20u)
#define PowerMonitor_1_ADC_DSM_FPCAP1OFFSET           (0x10u)


/******************************************/
/* DSM.CR8 Control Register 8 definitions */
/******************************************/

#define PowerMonitor_1_ADC_DSM_IPCAP2_EN              (0x80u)

#define PowerMonitor_1_ADC_DSM_IPCAP2_MASK            (0x70u)
#define PowerMonitor_1_ADC_DSM_IPCAP2_0_FF            (0x00u)
#define PowerMonitor_1_ADC_DSM_IPCAP2_50_FF           (0x10u)
#define PowerMonitor_1_ADC_DSM_IPCAP2_100_FF          (0x20u)
#define PowerMonitor_1_ADC_DSM_IPCAP2_150_FF          (0x30u)
#define PowerMonitor_1_ADC_DSM_IPCAP2_200_FF          (0x40u)
#define PowerMonitor_1_ADC_DSM_IPCAP2_250_FF          (0x50u)
#define PowerMonitor_1_ADC_DSM_IPCAP2_300_FF          (0x60u)
#define PowerMonitor_1_ADC_DSM_IPCAP2_350_FF          (0x70u)

#define PowerMonitor_1_ADC_DSM_FCAP2_MASK             (0x0Fu)
#define PowerMonitor_1_ADC_DSM_FCAP2_0_FF             (0x00u)
#define PowerMonitor_1_ADC_DSM_FCAP2_50_FF            (0x01u)
#define PowerMonitor_1_ADC_DSM_FCAP2_100_FF           (0x02u)
#define PowerMonitor_1_ADC_DSM_FCAP2_150_FF           (0x03u)
#define PowerMonitor_1_ADC_DSM_FCAP2_200_FF           (0x04u)
#define PowerMonitor_1_ADC_DSM_FCAP2_250_FF           (0x05u)
#define PowerMonitor_1_ADC_DSM_FCAP2_300_FF           (0x06u)
#define PowerMonitor_1_ADC_DSM_FCAP2_350_FF           (0x07u)
#define PowerMonitor_1_ADC_DSM_FCAP2_400_FF           (0x08u)
#define PowerMonitor_1_ADC_DSM_FCAP2_450_FF           (0x09u)
#define PowerMonitor_1_ADC_DSM_FCAP2_500_FF           (0x0Au)
#define PowerMonitor_1_ADC_DSM_FCAP2_550_FF           (0x0Bu)
#define PowerMonitor_1_ADC_DSM_FCAP2_600_FF           (0x0Cu)
#define PowerMonitor_1_ADC_DSM_FCAP2_650_FF           (0x0Du)
#define PowerMonitor_1_ADC_DSM_FCAP2_700_FF           (0x0Eu)
#define PowerMonitor_1_ADC_DSM_FCAP2_750_FF           (0x0Fu)


/******************************************/
/* DSM.CR9 Control Register 9 definitions */
/******************************************/

#define PowerMonitor_1_ADC_DSM_IPCAP3_EN              (0x80u)

#define PowerMonitor_1_ADC_DSM_IPCAP3_MASK            (0x70u)
#define PowerMonitor_1_ADC_DSM_IPCAP3_0_FF            (0x00u)
#define PowerMonitor_1_ADC_DSM_IPCAP3_50_FF           (0x10u)
#define PowerMonitor_1_ADC_DSM_IPCAP3_100_FF          (0x20u)
#define PowerMonitor_1_ADC_DSM_IPCAP3_150_FF          (0x30u)
#define PowerMonitor_1_ADC_DSM_IPCAP3_200_FF          (0x40u)
#define PowerMonitor_1_ADC_DSM_IPCAP3_250_FF          (0x50u)
#define PowerMonitor_1_ADC_DSM_IPCAP3_300_FF          (0x60u)
#define PowerMonitor_1_ADC_DSM_IPCAP3_350_FF          (0x70u)

#define PowerMonitor_1_ADC_DSM_FCAP3_MASK             (0x0Fu)
#define PowerMonitor_1_ADC_DSM_FCAP3_0_FF             (0x00u)
#define PowerMonitor_1_ADC_DSM_FCAP3_50_FF            (0x01u)
#define PowerMonitor_1_ADC_DSM_FCAP3_100_FF           (0x02u)
#define PowerMonitor_1_ADC_DSM_FCAP3_150_FF           (0x03u)
#define PowerMonitor_1_ADC_DSM_FCAP3_200_FF           (0x04u)
#define PowerMonitor_1_ADC_DSM_FCAP3_250_FF           (0x05u)
#define PowerMonitor_1_ADC_DSM_FCAP3_300_FF           (0x06u)
#define PowerMonitor_1_ADC_DSM_FCAP3_350_FF           (0x07u)
#define PowerMonitor_1_ADC_DSM_FCAP3_400_FF           (0x08u)
#define PowerMonitor_1_ADC_DSM_FCAP3_450_FF           (0x09u)
#define PowerMonitor_1_ADC_DSM_FCAP3_500_FF           (0x0Au)
#define PowerMonitor_1_ADC_DSM_FCAP3_550_FF           (0x0Bu)
#define PowerMonitor_1_ADC_DSM_FCAP3_600_FF           (0x0Cu)
#define PowerMonitor_1_ADC_DSM_FCAP3_650_FF           (0x0Du)
#define PowerMonitor_1_ADC_DSM_FCAP3_700_FF           (0x0Eu)
#define PowerMonitor_1_ADC_DSM_FCAP3_750_FF           (0x0Fu)


/********************************************/
/* DSM.CR10 Control Register 10 definitions */
/********************************************/

#define PowerMonitor_1_ADC_DSM_SUMCAP1_EN             (0x80u)
#define PowerMonitor_1_ADC_DSM_SUMCAP2_EN             (0x08u)

#define PowerMonitor_1_ADC_DSM_SUMCAP1_MASK           (0x70u)
#define PowerMonitor_1_ADC_DSM_SUMCAP1_0_FF           (0x00u)
#define PowerMonitor_1_ADC_DSM_SUMCAP1_50_FF          (0x10u)
#define PowerMonitor_1_ADC_DSM_SUMCAP1_100_FF         (0x20u)
#define PowerMonitor_1_ADC_DSM_SUMCAP1_150_FF         (0x30u)
#define PowerMonitor_1_ADC_DSM_SUMCAP1_200_FF         (0x40u)
#define PowerMonitor_1_ADC_DSM_SUMCAP1_250_FF         (0x50u)
#define PowerMonitor_1_ADC_DSM_SUMCAP1_300_FF         (0x60u)
#define PowerMonitor_1_ADC_DSM_SUMCAP1_350_FF         (0x70u)

#define PowerMonitor_1_ADC_DSM_SUMCAP2_MASK           (0x07u)
#define PowerMonitor_1_ADC_DSM_SUMCAP2_0_FF           (0x00u)
#define PowerMonitor_1_ADC_DSM_SUMCAP2_50_FF          (0x01u)
#define PowerMonitor_1_ADC_DSM_SUMCAP2_100_FF         (0x02u)
#define PowerMonitor_1_ADC_DSM_SUMCAP2_150_FF         (0x03u)
#define PowerMonitor_1_ADC_DSM_SUMCAP2_200_FF         (0x04u)
#define PowerMonitor_1_ADC_DSM_SUMCAP2_250_FF         (0x05u)
#define PowerMonitor_1_ADC_DSM_SUMCAP2_300_FF         (0x06u)
#define PowerMonitor_1_ADC_DSM_SUMCAP2_350_FF         (0x07u)


/********************************************/
/* DSM.CR11 Control Register 11 definitions */
/********************************************/

#define PowerMonitor_1_ADC_DSM_SUMCAP3_EN             (0x08u)

#define PowerMonitor_1_ADC_DSM_SUMCAP3_MASK           (0x70u)
#define PowerMonitor_1_ADC_DSM_SUMCAP3_0_FF           (0x00u)
#define PowerMonitor_1_ADC_DSM_SUMCAP3_50_FF          (0x10u)
#define PowerMonitor_1_ADC_DSM_SUMCAP3_100_FF         (0x20u)
#define PowerMonitor_1_ADC_DSM_SUMCAP3_150_FF         (0x30u)
#define PowerMonitor_1_ADC_DSM_SUMCAP3_200_FF         (0x40u)
#define PowerMonitor_1_ADC_DSM_SUMCAP3_250_FF         (0x50u)
#define PowerMonitor_1_ADC_DSM_SUMCAP3_300_FF         (0x60u)
#define PowerMonitor_1_ADC_DSM_SUMCAP3_350_FF         (0x70u)

#define PowerMonitor_1_ADC_DSM_SUMCAPFB_MASK          (0x07u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_0_FF          (0x00u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_50_FF         (0x01u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_100_FF        (0x02u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_150_FF        (0x03u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_200_FF        (0x04u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_250_FF        (0x05u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_300_FF        (0x06u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_350_FF        (0x07u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_400_FF        (0x08u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_450_FF        (0x09u)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_500_FF        (0x0Au)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_550_FF        (0x0Bu)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_600_FF        (0x0Cu)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_650_FF        (0x0Du)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_700_FF        (0x0Eu)
#define PowerMonitor_1_ADC_DSM_SUMCAPFB_750_FF        (0x0Fu)


/********************************************/
/* DSM.CR12 Control Register 12 definitions */
/********************************************/

#define PowerMonitor_1_ADC_DSM_SUMCAPFB_EN            (0x40u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_EN            (0x20u)

#define PowerMonitor_1_ADC_DSM_SUMCAPIN_MASK          (0x1Fu)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_0_FF          (0x00u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_50_FF         (0x01u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_100_FF        (0x02u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_150_FF        (0x03u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_200_FF        (0x04u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_250_FF        (0x05u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_300_FF        (0x06u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_350_FF        (0x07u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_400_FF        (0x08u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_450_FF        (0x09u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_500_FF        (0x0Au)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_550_FF        (0x0Bu)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_600_FF        (0x0Cu)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_650_FF        (0x0Du)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_700_FF        (0x0Eu)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_750_FF        (0x0Fu)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_800_FF        (0x10u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_850_FF        (0x11u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_900_FF        (0x12u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_950_FF        (0x13u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1000_FF       (0x14u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1050_FF       (0x15u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1100_FF       (0x16u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1150_FF       (0x17u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1200_FF       (0x18u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1250_FF       (0x19u)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1300_FF       (0x1Au)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1350_FF       (0x1Bu)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1400_FF       (0x1Cu)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1450_FF       (0x1Du)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1500_FF       (0x1Eu)
#define PowerMonitor_1_ADC_DSM_SUMCAPIN_1550_FF       (0x15u)


/********************************************/
/* DSM.CR13 Control Register 13 definitions */
/********************************************/

#define PowerMonitor_1_ADC_DSM_CR13_RSVD              (0xFFu)


/********************************************/
/* DSM.CR14 Control Register 14 definitions */
/********************************************/

/* Bit Field  DSM_POWER1 */
#define PowerMonitor_1_ADC_DSM_POWER1_MASK            (0x07u)

#define PowerMonitor_1_ADC_DSM_POWER1_0               (0x00u)
#define PowerMonitor_1_ADC_DSM_POWER1_1               (0x01u)
#define PowerMonitor_1_ADC_DSM_POWER1_2               (0x02u)
#define PowerMonitor_1_ADC_DSM_POWER1_3               (0x03u)
#define PowerMonitor_1_ADC_DSM_POWER1_4               (0x04u)
#define PowerMonitor_1_ADC_DSM_POWER1_5               (0x05u)
#define PowerMonitor_1_ADC_DSM_POWER1_6               (0x06u)
#define PowerMonitor_1_ADC_DSM_POWER1_7               (0x07u)
  
#define PowerMonitor_1_ADC_DSM_POWER1_44UA            (0x00u)
#define PowerMonitor_1_ADC_DSM_POWER1_123UA           (0x01u)
#define PowerMonitor_1_ADC_DSM_POWER1_492UA           (0x02u)
#define PowerMonitor_1_ADC_DSM_POWER1_750UA           (0x03u)
#define PowerMonitor_1_ADC_DSM_POWER1_1MA             (0x04u)

/* Bit Field  DSM_OPAMP1_BW */
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_MASK         (0xF0u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_0            (0x00u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_1            (0x10u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_2            (0x20u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_3            (0x30u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_4            (0x40u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_5            (0x50u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_6            (0x60u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_7            (0x70u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_8            (0x80u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_9            (0x90u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_A            (0xA0u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_B            (0xB0u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_C            (0xC0u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_D            (0xD0u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_E            (0xE0u)
#define PowerMonitor_1_ADC_DSM_OPAMP1_BW_F            (0xF0u)


/********************************************/
/* DSM.CR15 Control Register 15 definitions */
/********************************************/

/* Bit Field  DSM_POWER2_3 */
#define PowerMonitor_1_ADC_DSM_POWER2_3_MASK          (0x07u)

#define PowerMonitor_1_ADC_DSM_POWER2_3_LOW           (0x00u)
#define PowerMonitor_1_ADC_DSM_POWER2_3_MED           (0x01u)
#define PowerMonitor_1_ADC_DSM_POWER2_3_HIGH          (0x02u)
#define PowerMonitor_1_ADC_DSM_POWER2_3_IP5X          (0x03u)
#define PowerMonitor_1_ADC_DSM_POWER2_3_2X            (0x04u)
#define PowerMonitor_1_ADC_DSM_POWER2_3_HIGH_5        (0x05u)
#define PowerMonitor_1_ADC_DSM_POWER2_3_HIGH_6        (0x06u)
#define PowerMonitor_1_ADC_DSM_POWER2_3_HIGH_7        (0x07u)

/* Bit Field  DSM_POWER_COMP */
#define PowerMonitor_1_ADC_DSM_POWER_COMP_MASK        (0x30u)

#define PowerMonitor_1_ADC_DSM_POWER_VERYLOW          (0x00u)
#define PowerMonitor_1_ADC_DSM_POWER_NORMAL           (0x10u)
#define PowerMonitor_1_ADC_DSM_POWER_6MHZ             (0x20u)
#define PowerMonitor_1_ADC_DSM_POWER_12MHZ            (0x30u)


/********************************************/
/* DSM.CR16 Control Register 16 definitions */
/********************************************/

/* Bit Field  DSM_POWER_SUM */
#define PowerMonitor_1_ADC_DSM_POWER_SUM_MASK         (0x70u)

#define PowerMonitor_1_ADC_DSM_POWER_SUM_LOW          (0x00u)
#define PowerMonitor_1_ADC_DSM_POWER_SUM_MED          (0x10u)
#define PowerMonitor_1_ADC_DSM_POWER_SUM_HIGH         (0x20u)
#define PowerMonitor_1_ADC_DSM_POWER_SUM_IP5X         (0x30u)
#define PowerMonitor_1_ADC_DSM_POWER_SUM_2X           (0x40u)
#define PowerMonitor_1_ADC_DSM_POWER_SUM_HIGH_5       (0x50u)
#define PowerMonitor_1_ADC_DSM_POWER_SUM_HIGH_6       (0x60u)
#define PowerMonitor_1_ADC_DSM_POWER_SUM_HIGH_7       (0x70u)

#define PowerMonitor_1_ADC_DSM_CP_ENABLE              (0x08u)
#define PowerMonitor_1_ADC_DSM_CP_PWRCTL_MASK         (0x07u)
#define PowerMonitor_1_ADC_DSM_CP_PWRCTL_DEFAULT      (0x0Au)
#define PowerMonitor_1_ADC_DSM_CP_PWRCTL_2X           (0x09u)
#define PowerMonitor_1_ADC_DSM_CP_PWRCTL_LOW          (0x04u)


/********************************************/
/* DSM.CR17 Control Register 17 definitions */
/********************************************/

/* Bit Field  DSM_EN_BUF */
#define PowerMonitor_1_ADC_DSM_EN_BUF_VREF            (0x01u)
#define PowerMonitor_1_ADC_DSM_EN_BUF_VCM             (0x02u)

#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VREF_MASK     (0x0Cu)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VREF_0        (0x00u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VREF_1        (0x04u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VREF_2        (0x08u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VREF_3        (0x0Cu)

#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VCM_MASK      (0x30u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VCM_0         (0x00u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VCM_1         (0x10u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VCM_2         (0x20u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VCM_3         (0x30u)

#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VREF_INN_MASK (0xC0u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VREF_INN_0    (0x00u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VREF_INN_1    (0x40u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VREF_INN_2    (0x80u)
#define PowerMonitor_1_ADC_DSM_PWR_CTRL_VREF_INN_3    (0xC0u)


/*********************************************/
/* DSM.REF0 Reference Register 0 definitions */
/*********************************************/

#define PowerMonitor_1_ADC_DSM_REFMUX_MASK            (0x07u)
#define PowerMonitor_1_ADC_DSM_REFMUX_NONE            (0x00u)
#define PowerMonitor_1_ADC_DSM_REFMUX_UVCM            (0x01u)
#define PowerMonitor_1_ADC_DSM_REFMUX_VDA_4           (0x02u)
#define PowerMonitor_1_ADC_DSM_REFMUX_VDAC0           (0x03u)
#define PowerMonitor_1_ADC_DSM_REFMUX_1_024V          (0x04u)
#define PowerMonitor_1_ADC_DSM_REFMUX_1_20V           (0x05u)

#define PowerMonitor_1_ADC_DSM_EN_BUF_VREF_INN        (0x08u)
#define PowerMonitor_1_ADC_DSM_VREF_RES_DIV_EN        (0x10u)
#define PowerMonitor_1_ADC_DSM_VCM_RES_DIV_EN         (0x20u)
#define PowerMonitor_1_ADC_DSM_VCMSEL_MASK            (0xC0u)
#define PowerMonitor_1_ADC_DSM_VCMSEL_NOSEL           (0x00u)
#define PowerMonitor_1_ADC_DSM_VCMSEL_0_8V            (0x40u)
#define PowerMonitor_1_ADC_DSM_VCMSEL_0_7V            (0x80u)
#define PowerMonitor_1_ADC_DSM_VCMSEL_VPWRA_2         (0xC0u)


/*********************************************/
/* DSM.REF1 Reference Register 1 definitions */
/*********************************************/

#define PowerMonitor_1_ADC_DSM_REF1_MASK              (0xFFu)


/*********************************************/
/* DSM.REF2 Reference Register 2 definitions */
/*********************************************/

#define PowerMonitor_1_ADC_DSM_REF2_MASK              (0xFFu)
#define PowerMonitor_1_ADC_DSM_REF2_S0_EN             (0x01u)
#define PowerMonitor_1_ADC_DSM_REF2_S1_EN             (0x02u)
#define PowerMonitor_1_ADC_DSM_REF2_S2_EN             (0x04u)
#define PowerMonitor_1_ADC_DSM_REF2_S3_EN             (0x08u)
#define PowerMonitor_1_ADC_DSM_REF2_S4_EN             (0x10u)
#define PowerMonitor_1_ADC_DSM_REF2_S5_EN             (0x20u)
#define PowerMonitor_1_ADC_DSM_REF2_S6_EN             (0x40u)
#define PowerMonitor_1_ADC_DSM_REF2_S7_EN             (0x80u)


/*********************************************/
/* DSM.REF3 Reference Register 3 definitions */
/*********************************************/

#define PowerMonitor_1_ADC_DSM_REF3_MASK              (0xFFu)
#define PowerMonitor_1_ADC_DSM_REF2_S8_EN             (0x01u)
#define PowerMonitor_1_ADC_DSM_REF2_S9_EN             (0x02u)
#define PowerMonitor_1_ADC_DSM_REF2_S10_EN            (0x04u)
#define PowerMonitor_1_ADC_DSM_REF2_S11_EN            (0x08u)
#define PowerMonitor_1_ADC_DSM_REF2_S12_EN            (0x10u)
#define PowerMonitor_1_ADC_DSM_REF2_S13_EN            (0x20u)


/************************************************/
/* DSM.DEM0 Dynamic Element Matching Register 0 */
/************************************************/

#define PowerMonitor_1_ADC_DSM_EN_SCRAMBLER0          (0x01u)
#define PowerMonitor_1_ADC_DSM_EN_SCRAMBLER1          (0x02u)
#define PowerMonitor_1_ADC_DSM_EN_DEM                 (0x04u)
#define PowerMonitor_1_ADC_DSM_ADC_TEST_EN            (0x08u)
#define PowerMonitor_1_ADC_DSM_DEMTEST_SRC            (0x10u)


/************************************************/
/* DSM.DEM1 Dynamic Element Matching Register 1 */
/************************************************/

#define PowerMonitor_1_ADC_DSM_DEM1_MASK              (0xFF)


/***********************************************/
/* DSM.BUF0 Buffer Register 0                  */
/***********************************************/

#define PowerMonitor_1_ADC_DSM_ENABLE_P               (0x01u)
#define PowerMonitor_1_ADC_DSM_BYPASS_P               (0x02u)
#define PowerMonitor_1_ADC_DSM_RAIL_RAIL_EN           (0x04u)

#define PowerMonitor_1_ADC_DSM_BUFSEL                 (0x08u)
#define PowerMonitor_1_ADC_DSM_BUFSEL_A               (0x00u)
#define PowerMonitor_1_ADC_DSM_BUFSEL_B               (0x08u)


/***********************************************/
/* DSM.BUF1 Buffer Register 1                  */
/***********************************************/

#define PowerMonitor_1_ADC_DSM_ENABLE_N               (0x01u)
#define PowerMonitor_1_ADC_DSM_BYPASS_N               (0x02u)
#define PowerMonitor_1_ADC_DSM_GAIN_MASK              (0x0Cu)


/***********************************************/
/* DSM.BUF2 Buffer Register 2                  */
/***********************************************/

#define PowerMonitor_1_ADC_DSM_LOWPOWER_EN            (0x01u)
#define PowerMonitor_1_ADC_DSM_ADD_EXTRA_RC           (0x02u)


/***********************************************/
/* DSM.BUF3 Buffer Register 3                  */
/***********************************************/

#define PowerMonitor_1_ADC_DSM_BUF_CHOP_EN            (0x08u)

#define PowerMonitor_1_ADC_DSM_BUF_FCHOP_MASK         (0x07u)
#define PowerMonitor_1_ADC_DSM_BUF_FCHOP_FS2          (0x00u)
#define PowerMonitor_1_ADC_DSM_BUF_FCHOP_FS4          (0x01u)
#define PowerMonitor_1_ADC_DSM_BUF_FCHOP_FS8          (0x02u)
#define PowerMonitor_1_ADC_DSM_BUF_FCHOP_FS16         (0x03u)
#define PowerMonitor_1_ADC_DSM_BUF_FCHOP_FS32         (0x04u)
#define PowerMonitor_1_ADC_DSM_BUF_FCHOP_FS64         (0x05u)
#define PowerMonitor_1_ADC_DSM_BUF_FCHOP_FS128        (0x06u)
#define PowerMonitor_1_ADC_DSM_BUF_FCHOP_FS256        (0x07u)


/***********************************************/
/* DSM.MISC Delta Sigma Misc Register          */
/***********************************************/

#define PowerMonitor_1_ADC_DSM_SEL_ICLK_CP            (0x01u)


/************************************************/
/* DSM.CLK Delta Sigma Clock selection Register */
/************************************************/

#define PowerMonitor_1_ADC_DSM_CLK_MX_CLK_MSK         (0x07u)
#define PowerMonitor_1_ADC_DSM_CLK_CLK_EN             (0x08u)
#define PowerMonitor_1_ADC_DSM_CLK_BYPASS_SYNC        (0x10u)


/***********************************************/
/* DSM.OUT0 Output Register 0                  */
/***********************************************/
#define PowerMonitor_1_ADC_DSM_DOUT0                  (0xFFu)


/***********************************************/
/* DSM.OUT1 Output Register 1                  */
/***********************************************/
#define PowerMonitor_1_ADC_DSM_DOUT2SCOMP_MASK        (0x0Fu)
#define PowerMonitor_1_ADC_DSM_OVDFLAG                (0x10u)
#define PowerMonitor_1_ADC_DSM_OVDCAUSE               (0x20u)


/***********************************************/
/*          Decimator                          */
/***********************************************/


/***********************************************/
/* DEC.CR Control Register 0                   */
/***********************************************/

#define PowerMonitor_1_ADC_DEC_SAT_EN                 (0x80u) /* Enable post process offset correction */
#define PowerMonitor_1_ADC_DEC_FIR_EN                 (0x40u) /* Post process FIR enable  */
#define PowerMonitor_1_ADC_DEC_OCOR_EN                (0x20u) /* Offest correction enable */
#define PowerMonitor_1_ADC_DEC_GCOR_EN                (0x10u) /* Enable gain correction feature */

#define PowerMonitor_1_ADC_MODE_MASK                  (0x0Cu) /* Conversion Mode */
#define PowerMonitor_1_ADC_MODE_SINGLE_SAMPLE         (0x00u)
#define PowerMonitor_1_ADC_MODE_FAST_FILTER           (0x04u)
#define PowerMonitor_1_ADC_MODE_CONTINUOUS            (0x08u)
#define PowerMonitor_1_ADC_MODE_FAST_FIR              (0x0Cu)

#define PowerMonitor_1_ADC_DEC_XSTART_EN              (0x02u) /* Enables external start signal */
#define PowerMonitor_1_ADC_DEC_START_CONV             (0x01u) /* A write to this bit starts a conversion */


/***********************************************/
/* DEC.SR Status and Control Register 0        */
/***********************************************/

#define PowerMonitor_1_ADC_DEC_CORECLK_DISABLE        (0x20u) /* Controls gating of the Core clock */
#define PowerMonitor_1_ADC_DEC_INTR_PULSE             (0x10u) /* Controls interrupt mode */ 
#define PowerMonitor_1_ADC_DEC_OUT_ALIGN              (0x08u) /* Controls 8-bit shift of SAMP registers */
#define PowerMonitor_1_ADC_DEC_INTR_CLEAR             (0x04u) /* A write to this bit clears bit0 */
#define PowerMonitor_1_ADC_DEC_INTR_MASK              (0x02u) /* Controls the generation of the conversion comp. INTR */
#define PowerMonitor_1_ADC_DEC_CONV_DONE              (0x01u) /* Bit set if conversion has completed  */


/***********************************************/
/* DEC.SHIFT1 Decimator Input Shift Register 1 */
/***********************************************/

#define PowerMonitor_1_ADC_DEC_SHIFT1_MASK            (0x1Fu) /* Input shift1 control */


/***********************************************/
/* DEC.SHIFT2 Decimator Input Shift Register 2 */
/***********************************************/

#define PowerMonitor_1_ADC_DEC_SHIFT2_MASK            (0x0Fu) /* Input shift2 control */


/****************************************************************/
/* DEC.DR2 Decimator Decimation Rate of FIR Filter Low Register */
/****************************************************************/

#define PowerMonitor_1_ADC_DEC_DR2_MASK               (0xFFu)


/******************************************************************/
/* DEC.DR2H Decimator Decimation Rate of FIR Filter High Register */
/******************************************************************/

#define PowerMonitor_1_ADC_DEC_DR2H_MASK              (0x03u)


/***********************************************/
/* DEC.COHR Decimator Coherency Register       */
/***********************************************/

#define PowerMonitor_1_ADC_DEC_GCOR_KEY_MASK          (0x30u) 
#define PowerMonitor_1_ADC_DEC_GCOR_KEY_OFF           (0x00u) 
#define PowerMonitor_1_ADC_DEC_GCOR_KEY_LOW           (0x10u) 
#define PowerMonitor_1_ADC_DEC_GCOR_KEY_MID           (0x20u) 
#define PowerMonitor_1_ADC_DEC_GCOR_KEY_HIGH          (0x30u) 

#define PowerMonitor_1_ADC_DEC_OCOR_KEY_MASK          (0x0Cu)
#define PowerMonitor_1_ADC_DEC_OCOR_KEY_OFF           (0x00u) 
#define PowerMonitor_1_ADC_DEC_OCOR_KEY_LOW           (0x04u) 
#define PowerMonitor_1_ADC_DEC_OCOR_KEY_MID           (0x08u) 
#define PowerMonitor_1_ADC_DEC_OCOR_KEY_HIGH          (0x0Cu) 

#define PowerMonitor_1_ADC_DEC_SAMP_KEY_MASK          (0x03u) 
#define PowerMonitor_1_ADC_DEC_SAMP_KEY_OFF           (0x00u) 
#define PowerMonitor_1_ADC_DEC_SAMP_KEY_LOW           (0x01u) 
#define PowerMonitor_1_ADC_DEC_SAMP_KEY_MID           (0x02u) 
#define PowerMonitor_1_ADC_DEC_SAMP_KEY_HIGH          (0x03u)

/* PM_ACT_CFG (Active Power Mode CFG Register) mask */ 
#define PowerMonitor_1_ADC_ACT_PWR_DEC_EN             PowerMonitor_1_ADC_DEC__PM_ACT_MSK /* Power enable mask */
#define PowerMonitor_1_ADC_STBY_PWR_DEC_EN            PowerMonitor_1_ADC_DEC__PM_STBY_MSK /* Power enable mask */

/* Active and Alternative active power masks for DSM */

#if (CY_PSOC3 || CY_PSOC5LP)
    #define PowerMonitor_1_ADC_ACT_PWR_DSM_EN         (0x10u) /* Active Power enable mask */
    #define PowerMonitor_1_ADC_STBY_PWR_DSM_EN        (0x10u) /* Alternate active Power enable mask */
#elif (CY_PSOC5A)
    #define PowerMonitor_1_ADC_ACT_PWR_DSM_EN         (0x01u) /* Power enable mask */
#endif /* CY_PSOC5A */


/* Internal and charge pump clock power enable masks */
#if(PowerMonitor_1_ADC_DEFAULT_INTERNAL_CLK)
    #define PowerMonitor_1_ADC_ACT_PWR_CLK_EN              PowerMonitor_1_ADC_theACLK__PM_ACT_MSK 
    #define PowerMonitor_1_ADC_STBY_PWR_CLK_EN             PowerMonitor_1_ADC_theACLK__PM_STBY_MSK 
#endif /* PowerMonitor_1_ADC_DEFAULT_INTERNAL_CLK */

#define PowerMonitor_1_ADC_ACT_PWR_CHARGE_PUMP_CLK_EN  PowerMonitor_1_ADC_Ext_CP_Clk__PM_ACT_MSK 
#define PowerMonitor_1_ADC_STBY_PWR_CHARGE_PUMP_CLK_EN PowerMonitor_1_ADC_Ext_CP_Clk__PM_STBY_MSK 


/***********************************************/
/* DSM.SW3 DSM Analog Routing Register 3       */
/***********************************************/
#define PowerMonitor_1_ADC_DSM_VP_VSSA                (0x04u) 

/* To connect negative input to analog mux Bus. This is valid only
   for PSoC5A */
#if(CY_PSOC5A)
    #define PowerMonitor_1_ADC_DSM_VN_AMX             (0x10u) 
#endif /* CY_PSOC5A */

/* To connect positive input to analog mux Bus. This is valid only
   for PSoC3 or PSoC5LP silicon */
#if(CY_PSOC3 || CY_PSOC5LP)
    #define PowerMonitor_1_ADC_DSM_VP_AMX             (0x01u) 
#endif /* CY_PSOC3 || CY_PSOC5LP */

#define PowerMonitor_1_ADC_DSM_VN_VREF                (0x20u) 
#define PowerMonitor_1_ADC_DSM_VN_VSSA                (0x40u) 
#define PowerMonitor_1_ADC_DSM_VN_REF_MASK            (0x60u) 


/***********************************************/
/* ANIF.PUMP.CR1 Pump Configuration Register 1 */
/***********************************************/

#define PowerMonitor_1_ADC_PUMP_CR1_CLKSEL            (0x04u)
#define PowerMonitor_1_ADC_PUMP_CR1_FORCE             (0x02u)
#define PowerMonitor_1_ADC_PUMP_CR1_AUTO              (0x01u)

/***********************************************/
/* RESET_CR4 Reset Ignore Control Register     */
/***********************************************/ 

#define PowerMonitor_1_ADC_IGNORE_PRESD1              (0x01u) 
#define PowerMonitor_1_ADC_IGNORE_PRESA1              (0x02u)
#define PowerMonitor_1_ADC_IGNORE_LPCOMPD1            (0x04u)
#define PowerMonitor_1_ADC_IGNORE_LPCOMPA1            (0x08u)
#define PowerMonitor_1_ADC_IGNORE_HBR1                (0x10u)


/***********************************************/
/* RESET_CR5 Reset Ignore Control Register     */
/***********************************************/ 

#define PowerMonitor_1_ADC_IGNORE_PRESD2              (0x01u) 
#define PowerMonitor_1_ADC_IGNORE_PRESA2              (0x02u)
#define PowerMonitor_1_ADC_IGNORE_LPCOMPD2            (0x04u)
#define PowerMonitor_1_ADC_IGNORE_LPCOMPA2            (0x08u)
#define PowerMonitor_1_ADC_IGNORE_HBR2                (0x10u)


/***********************************************/
/* RESET_CR1 Reset Ignore Control Register     */
/***********************************************/ 

#define PowerMonitor_1_ADC_DIS_HBR1                   (0x20u) 
#define PowerMonitor_1_ADC_DIS_PRES1                  (0x10u)
#define PowerMonitor_1_ADC_VMON_HVI_SEL               (0x08u)
#define PowerMonitor_1_ADC_EN_HVI_A                   (0x04u)
#define PowerMonitor_1_ADC_EN_LVI_A                   (0x02u)
#define PowerMonitor_1_ADC_EN_LVI_D                   (0x01u)


/***********************************************/
/* RESET_CR3 Reset Ignore Control Register     */
/***********************************************/ 

#define PowerMonitor_1_ADC_DIS_PRES2                  (0x08u) 
#define PowerMonitor_1_ADC_DIS_HBR2                   (0x10u)


/***************************************
*              Registers        
***************************************/

/* Decimator (DEC) Registers */
#define PowerMonitor_1_ADC_DEC_CR_REG               (* (reg8 *) PowerMonitor_1_ADC_DEC__CR )
#define PowerMonitor_1_ADC_DEC_CR_PTR               (  (reg8 *) PowerMonitor_1_ADC_DEC__CR )

#define PowerMonitor_1_ADC_DEC_SR_REG               (* (reg8 *) PowerMonitor_1_ADC_DEC__SR )
#define PowerMonitor_1_ADC_DEC_SR_PTR               (  (reg8 *) PowerMonitor_1_ADC_DEC__SR )

#define PowerMonitor_1_ADC_DEC_SHIFT1_REG           (* (reg8 *) PowerMonitor_1_ADC_DEC__SHIFT1 )
#define PowerMonitor_1_ADC_DEC_SHIFT1_PTR           (  (reg8 *) PowerMonitor_1_ADC_DEC__SHIFT1 )

#define PowerMonitor_1_ADC_DEC_SHIFT2_REG           (* (reg8 *) PowerMonitor_1_ADC_DEC__SHIFT2 )
#define PowerMonitor_1_ADC_DEC_SHIFT2_PTR           ( (reg8 *) PowerMonitor_1_ADC_DEC__SHIFT2 )

#define PowerMonitor_1_ADC_DEC_DR2_REG              (* (reg8 *) PowerMonitor_1_ADC_DEC__DR2 )
#define PowerMonitor_1_ADC_DEC_DR2_PTR              (  (reg8 *) PowerMonitor_1_ADC_DEC__DR2 )

#define PowerMonitor_1_ADC_DEC_DR2H_REG             (* (reg8 *) PowerMonitor_1_ADC_DEC__DR2H )
#define PowerMonitor_1_ADC_DEC_DR2H_PTR             (  (reg8 *) PowerMonitor_1_ADC_DEC__DR2H )

#define PowerMonitor_1_ADC_DEC_DR1_REG              (* (reg8 *) PowerMonitor_1_ADC_DEC__DR1 )
#define PowerMonitor_1_ADC_DEC_DR1_PTR              (  (reg8 *) PowerMonitor_1_ADC_DEC__DR1 )

#define PowerMonitor_1_ADC_DEC_OCOR_REG             (* (reg8 *) PowerMonitor_1_ADC_DEC__OCOR )
#define PowerMonitor_1_ADC_DEC_OCOR_PTR             (  (reg8 *) PowerMonitor_1_ADC_DEC__OCOR )

#define PowerMonitor_1_ADC_DEC_OCORM_REG            (* (reg8 *) PowerMonitor_1_ADC_DEC__OCORM )
#define PowerMonitor_1_ADC_DEC_OCORM_PTR            (  (reg8 *) PowerMonitor_1_ADC_DEC__OCORM )

#define PowerMonitor_1_ADC_DEC_OCORH_REG            (* (reg8 *) PowerMonitor_1_ADC_DEC__OCORH )
#define PowerMonitor_1_ADC_DEC_OCORH_PTR            (  (reg8 *) PowerMonitor_1_ADC_DEC__OCORH )

#define PowerMonitor_1_ADC_DEC_GVAL_REG             (* (reg8 *) PowerMonitor_1_ADC_DEC__GVAL )
#define PowerMonitor_1_ADC_DEC_GVAL_PTR             (  (reg8 *) PowerMonitor_1_ADC_DEC__GVAL )

#define PowerMonitor_1_ADC_DEC_GCOR_REG             (* (reg8 *) PowerMonitor_1_ADC_DEC__GCOR )
#define PowerMonitor_1_ADC_DEC_GCOR_PTR             (  (reg8 *) PowerMonitor_1_ADC_DEC__GCOR )

#define PowerMonitor_1_ADC_DEC_GCORH_REG            (* (reg8 *) PowerMonitor_1_ADC_DEC__GCORH )
#define PowerMonitor_1_ADC_DEC_GCORH_PTR            (  (reg8 *) PowerMonitor_1_ADC_DEC__GCORH )

#define PowerMonitor_1_ADC_DEC_SAMP_REG             (* (reg8 *) PowerMonitor_1_ADC_DEC__OUTSAMP )
#define PowerMonitor_1_ADC_DEC_SAMP_PTR             (  (reg8 *) PowerMonitor_1_ADC_DEC__OUTSAMP )

#define PowerMonitor_1_ADC_DEC_SAMPM_REG            (* (reg8 *) PowerMonitor_1_ADC_DEC__OUTSAMPM )
#define PowerMonitor_1_ADC_DEC_SAMPM_PTR            (  (reg8 *) PowerMonitor_1_ADC_DEC__OUTSAMPM )

#define PowerMonitor_1_ADC_DEC_SAMPH_REG            (* (reg8 *) PowerMonitor_1_ADC_DEC__OUTSAMPH )
#define PowerMonitor_1_ADC_DEC_SAMPH_PTR            (  (reg8 *) PowerMonitor_1_ADC_DEC__OUTSAMPH )

#define PowerMonitor_1_ADC_DEC_COHER_REG            (* (reg8 *) PowerMonitor_1_ADC_DEC__COHER )
#define PowerMonitor_1_ADC_DEC_COHER_PTR            (  (reg8 *) PowerMonitor_1_ADC_DEC__COHER )

#define PowerMonitor_1_ADC_PWRMGR_DEC_REG           (* (reg8 *) PowerMonitor_1_ADC_DEC__PM_ACT_CFG )    
#define PowerMonitor_1_ADC_PWRMGR_DEC_PTR           (  (reg8 *) PowerMonitor_1_ADC_DEC__PM_ACT_CFG )

#define PowerMonitor_1_ADC_STBY_PWRMGR_DEC_REG      (* (reg8 *) PowerMonitor_1_ADC_DEC__PM_STBY_CFG )     
#define PowerMonitor_1_ADC_STBY_PWRMGR_DEC_PTR      (  (reg8 *) PowerMonitor_1_ADC_DEC__PM_STBY_CFG )

/* Delta Sigma Modulator (DSM) Registers */  
#define PowerMonitor_1_ADC_DSM_CR0_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR0 )
#define PowerMonitor_1_ADC_DSM_CR0_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR0 )

#define PowerMonitor_1_ADC_DSM_CR1_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR1 )
#define PowerMonitor_1_ADC_DSM_CR1_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR1 )

#define PowerMonitor_1_ADC_DSM_CR2_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR2 )
#define PowerMonitor_1_ADC_DSM_CR2_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR2 )

#define PowerMonitor_1_ADC_DSM_CR3_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR3 )
#define PowerMonitor_1_ADC_DSM_CR3_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR3 )

#define PowerMonitor_1_ADC_DSM_CR4_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR4 )
#define PowerMonitor_1_ADC_DSM_CR4_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR4 )

#define PowerMonitor_1_ADC_DSM_CR5_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR5 )
#define PowerMonitor_1_ADC_DSM_CR5_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR5 )

#define PowerMonitor_1_ADC_DSM_CR6_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR6 )
#define PowerMonitor_1_ADC_DSM_CR6_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR6 )

#define PowerMonitor_1_ADC_DSM_CR7_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR7 )
#define PowerMonitor_1_ADC_DSM_CR7_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR7 )

#define PowerMonitor_1_ADC_DSM_CR8_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR8 )
#define PowerMonitor_1_ADC_DSM_CR8_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR8 )

#define PowerMonitor_1_ADC_DSM_CR9_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR9 )
#define PowerMonitor_1_ADC_DSM_CR9_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR9 )

#define PowerMonitor_1_ADC_DSM_CR10_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR10 )
#define PowerMonitor_1_ADC_DSM_CR10_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR10 )

#define PowerMonitor_1_ADC_DSM_CR11_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR11 )
#define PowerMonitor_1_ADC_DSM_CR11_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR11 )

#define PowerMonitor_1_ADC_DSM_CR12_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR12 )
#define PowerMonitor_1_ADC_DSM_CR12_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR12 )

#define PowerMonitor_1_ADC_DSM_CR13_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR13 )
#define PowerMonitor_1_ADC_DSM_CR13_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR13 )

#define PowerMonitor_1_ADC_DSM_CR14_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR14 )
#define PowerMonitor_1_ADC_DSM_CR14_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR14 )

#define PowerMonitor_1_ADC_DSM_CR15_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR15 )
#define PowerMonitor_1_ADC_DSM_CR15_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR15 )

#define PowerMonitor_1_ADC_DSM_CR16_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR16 )
#define PowerMonitor_1_ADC_DSM_CR16_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR16 )

#define PowerMonitor_1_ADC_DSM_CR17_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__CR17 )
#define PowerMonitor_1_ADC_DSM_CR17_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__CR17 )

#define PowerMonitor_1_ADC_DSM_REF0_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__REF0 )
#define PowerMonitor_1_ADC_DSM_REF0_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__REF0 )

#define PowerMonitor_1_ADC_DSM_REF1_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__REF1 )
#define PowerMonitor_1_ADC_DSM_REF1_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__REF1 )

#define PowerMonitor_1_ADC_DSM_REF2_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__REF2 )
#define PowerMonitor_1_ADC_DSM_REF2_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__REF2 )

#define PowerMonitor_1_ADC_DSM_REF3_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__REF3 )
#define PowerMonitor_1_ADC_DSM_REF3_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__REF3 )

#define PowerMonitor_1_ADC_DSM_DEM0_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__DEM0 )
#define PowerMonitor_1_ADC_DSM_DEM0_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__DEM0 )

#define PowerMonitor_1_ADC_DSM_DEM1_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__DEM1 )
#define PowerMonitor_1_ADC_DSM_DEM1_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__DEM1 )

#define PowerMonitor_1_ADC_DSM_MISC_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__MISC )
#define PowerMonitor_1_ADC_DSM_MISC_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__MISC )

#define PowerMonitor_1_ADC_DSM_CLK_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__CLK )
#define PowerMonitor_1_ADC_DSM_CLK_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__CLK )

#define PowerMonitor_1_ADC_DSM_TST0_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__TST0 )
#define PowerMonitor_1_ADC_DSM_TST0_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__TST0 )

#define PowerMonitor_1_ADC_DSM_TST1_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__TST1 )
#define PowerMonitor_1_ADC_DSM_TST1_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__TST1 )

#define PowerMonitor_1_ADC_DSM_BUF0_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__BUF0 )
#define PowerMonitor_1_ADC_DSM_BUF0_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__BUF0 )

#define PowerMonitor_1_ADC_DSM_BUF1_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__BUF1 )
#define PowerMonitor_1_ADC_DSM_BUF1_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__BUF1 )

#define PowerMonitor_1_ADC_DSM_BUF2_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__BUF2 )
#define PowerMonitor_1_ADC_DSM_BUF2_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__BUF2 )

#define PowerMonitor_1_ADC_DSM_BUF3_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__BUF3 )
#define PowerMonitor_1_ADC_DSM_BUF3_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__BUF3 )

#define PowerMonitor_1_ADC_DSM_OUT0_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__OUT0 )
#define PowerMonitor_1_ADC_DSM_OUT0_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__OUT0 )

#define PowerMonitor_1_ADC_DSM_OUT1_REG             (* (reg8 *) PowerMonitor_1_ADC_DSM4__OUT1 )
#define PowerMonitor_1_ADC_DSM_OUT1_PTR             (  (reg8 *) PowerMonitor_1_ADC_DSM4__OUT1 )

#define PowerMonitor_1_ADC_DSM_SW0_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__SW0 )
#define PowerMonitor_1_ADC_DSM_SW0_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__SW0 )

#define PowerMonitor_1_ADC_DSM_SW2_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__SW2 )
#define PowerMonitor_1_ADC_DSM_SW2_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__SW2 )

#define PowerMonitor_1_ADC_DSM_SW3_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__SW3 )
#define PowerMonitor_1_ADC_DSM_SW3_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__SW3 )

#define PowerMonitor_1_ADC_DSM_SW4_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__SW4 )
#define PowerMonitor_1_ADC_DSM_SW4_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__SW4 )

#define PowerMonitor_1_ADC_DSM_SW6_REG              (* (reg8 *) PowerMonitor_1_ADC_DSM4__SW6 )
#define PowerMonitor_1_ADC_DSM_SW6_PTR              (  (reg8 *) PowerMonitor_1_ADC_DSM4__SW6 )

/* Charge pump configuration register */
#define PowerMonitor_1_ADC_PUMP_CR1_REG             (* (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)
#define PowerMonitor_1_ADC_PUMP_CR1_PTR             (  (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)

/* DSM Active Power manager register */

#if(CY_PSOC3 || CY_PSOC5LP)
    #define PowerMonitor_1_ADC_PWRMGR_DSM_REG       (* (reg8 *) CYDEV_PM_ACT_CFG10 )  /* Modulator Power manager */
    #define PowerMonitor_1_ADC_PWRMGR_DSM_PTR       (  (reg8 *) CYDEV_PM_ACT_CFG10 )  /* Modulator Power manager */
#elif (CY_PSOC5A)
    #define PowerMonitor_1_ADC_PWRMGR_DSM_REG       (* (reg8 *) CYDEV_PM_AVAIL_CR5 )  /* Modulator Power manager */
    #define PowerMonitor_1_ADC_PWRMGR_DSM_PTR       (  (reg8 *) CYDEV_PM_AVAIL_CR5 )  /* Modulator Power manager */
#endif /* CY_PSOC5A */

/* DSM Alternative Active Power manager register */

#if(CY_PSOC3 || CY_PSOC5LP)
    #define PowerMonitor_1_ADC_STBY_PWRMGR_DSM_REG  (* (reg8 *) CYDEV_PM_STBY_CFG10 )  
    #define PowerMonitor_1_ADC_STBY_PWRMGR_DSM_PTR  (  (reg8 *) CYDEV_PM_STBY_CFG10 )  
#endif /* CY_PSOC3 || CY_PSOC5LP */

/* This is only valid if there is an internal clock */
#if(PowerMonitor_1_ADC_DEFAULT_INTERNAL_CLK)
    #define PowerMonitor_1_ADC_PWRMGR_CLK_REG       (* (reg8 *) PowerMonitor_1_ADC_theACLK__PM_ACT_CFG ) 
    #define PowerMonitor_1_ADC_PWRMGR_CLK_PTR       (  (reg8 *) PowerMonitor_1_ADC_theACLK__PM_ACT_CFG ) 

    #define PowerMonitor_1_ADC_STBY_PWRMGR_CLK_REG  (* (reg8 *) PowerMonitor_1_ADC_theACLK__PM_STBY_CFG )
    #define PowerMonitor_1_ADC_STBY_PWRMGR_CLK_PTR  (  (reg8 *) PowerMonitor_1_ADC_theACLK__PM_STBY_CFG )
#endif /* PowerMonitor_1_ADC_DEFAULT_INTERNAL_CLK */

/* This is only valid if there is an external charge clock */

/* Clock power manager register */
#define PowerMonitor_1_ADC_PWRMGR_CHARGE_PUMP_CLK_REG       (* (reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__PM_ACT_CFG )
#define PowerMonitor_1_ADC_PWRMGR_CHARGE_PUMP_CLK_PTR       (  (reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__PM_ACT_CFG )

#define PowerMonitor_1_ADC_STBY_PWRMGR_CHARGE_PUMP_CLK_REG  (* (reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__PM_STBY_CFG ) 
#define PowerMonitor_1_ADC_STBY_PWRMGR_CHARGE_PUMP_CLK_PTR  (  (reg8 *) PowerMonitor_1_ADC_Ext_CP_Clk__PM_STBY_CFG ) 

/* PRES-A related registers */
#if (CY_PSOC3 || CY_PSOC5LP)

    #define PowerMonitor_1_ADC_RESET_CR4_REG                    (* (reg8 *) CYDEV_RESET_CR4)
    #define PowerMonitor_1_ADC_RESET_CR4_PTR                    (  (reg8 *) CYDEV_RESET_CR4)

    #define PowerMonitor_1_ADC_RESET_CR5_REG                    (* (reg8 *) CYDEV_RESET_CR5)
    #define PowerMonitor_1_ADC_RESET_CR5_PTR                    (  (reg8 *) CYDEV_RESET_CR5)
    
#elif (CY_PSOC5A)
    #define PowerMonitor_1_ADC_RESET_CR1_REG                    (* (reg8 *) CYDEV_RESET_CR1)
    #define PowerMonitor_1_ADC_RESET_CR1_PTR                    (  (reg8 *) CYDEV_RESET_CR1)

    #define PowerMonitor_1_ADC_RESET_CR3_REG                    (* (reg8 *) CYDEV_RESET_CR3)
    #define PowerMonitor_1_ADC_RESET_CR3_PTR                    (  (reg8 *) CYDEV_RESET_CR3)
    
#endif /* CY_PSOC5A */
    

/* Default register settings for the Config 1 */
/* Default Config */
/* PowerMonitor_1_ADC_CFG1 Sample Rate: 10000 Samples per Second */
/* PowerMonitor_1_ADC_CFG1 Conversion Mode: 0 */
/* PowerMonitor_1_ADC_CFG1 Input Buffer Gain: 1   */
/* PowerMonitor_1_ADC_CFG1 Reference: 0 */
/* PowerMonitor_1_ADC_CFG1 Input Range: 5  */
/* PowerMonitor_1_ADC_CFG1 Resolution: 12 bits */
/* PowerMonitor_1_ADC_CFG1 Clock: 1310000 Hz */

#define PowerMonitor_1_ADC_CFG1_DEC_CR             (0X30u)
#define PowerMonitor_1_ADC_CFG1_DEC_SR             (0X14u)
#define PowerMonitor_1_ADC_CFG1_DEC_SHIFT1         (0X06u)
#define PowerMonitor_1_ADC_CFG1_DEC_SHIFT2         (0X09u)
#define PowerMonitor_1_ADC_CFG1_DEC_DR2            (0X00u)
#define PowerMonitor_1_ADC_CFG1_DEC_DR2H           (0X00u)
#define PowerMonitor_1_ADC_CFG1_DEC_DR1            (0X1Fu)
#define PowerMonitor_1_ADC_CFG1_DEC_OCOR           (0X00u)
#define PowerMonitor_1_ADC_CFG1_DEC_OCORM          (0X00u)
#define PowerMonitor_1_ADC_CFG1_DEC_OCORH          (0X00u)
#define PowerMonitor_1_ADC_CFG1_DEC_GVAL           (0X0Du)
#define PowerMonitor_1_ADC_CFG1_DEC_GCOR           (0X00u)
#define PowerMonitor_1_ADC_CFG1_DEC_GCORH          (0X00u)
#define PowerMonitor_1_ADC_CFG1_DEC_COHER          (0X01u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR0            (0X0Au)
#define PowerMonitor_1_ADC_CFG1_DSM_CR1            (0X00u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR2            (0X7Au)
#define PowerMonitor_1_ADC_CFG1_DSM_CR3            (0X00u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR4            (0X14u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR5            (0X0Bu)
#define PowerMonitor_1_ADC_CFG1_DSM_CR6            (0X19u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR7            (0X00u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR8            (0X12u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR9            (0X13u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR10           (0X44u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR11           (0X48u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR12           (0X02u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR13           (0X00u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR14           (0X01u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR15           (0X11u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR16           (0X19u)
#define PowerMonitor_1_ADC_CFG1_DSM_CR17           (0X97u)
#define PowerMonitor_1_ADC_CFG1_DSM_REF0           (0X44u)
#define PowerMonitor_1_ADC_CFG1_DSM_REF1           (0X00u)
#define PowerMonitor_1_ADC_CFG1_DSM_REF2           (0X58u)
#define PowerMonitor_1_ADC_CFG1_DSM_REF3           (0X00u)
#define PowerMonitor_1_ADC_CFG1_DSM_DEM0           (0X07u)
#define PowerMonitor_1_ADC_CFG1_DSM_DEM1           (0X00u)
#define PowerMonitor_1_ADC_CFG1_DSM_MISC           (0X00u)
#define PowerMonitor_1_ADC_CFG1_DSM_CLK            (0X18u)
#define PowerMonitor_1_ADC_CFG1_DSM_BUF0           (0X01u)
#define PowerMonitor_1_ADC_CFG1_DSM_BUF1           (0X01u)
#define PowerMonitor_1_ADC_CFG1_DSM_BUF2           (0X01u)
#define PowerMonitor_1_ADC_CFG1_DSM_BUF3           (0X0Au)
#define PowerMonitor_1_ADC_CFG1_DSM_OUT0           (0X00u)
#define PowerMonitor_1_ADC_CFG1_DSM_OUT1           (0X00u)
#define PowerMonitor_1_ADC_CFG1_DSM_SW3            (0X00u)
#define PowerMonitor_1_ADC_CFG1_CLOCKS_PER_SAMPLE  (0X0083u)
#define PowerMonitor_1_ADC_CFG1_CLOCK_FREQ         (1310000u)
#define PowerMonitor_1_ADC_CFG1_CP_CLOCK_FREQ      (5240000u)
#define PowerMonitor_1_ADC_CFG1_REFERENCE_VOLTAGE  (1.0240)
#define PowerMonitor_1_ADC_CFG1_COUNTS_PER_VOLT    (1000u)
#define PowerMonitor_1_ADC_CFG1_IDEAL_DEC_GAIN     (0X92D2u)

#define PowerMonitor_1_ADC_CFG1_IDEAL_ODDDEC_GAIN  (0X8000u)

/* Input and DAC Cap values in pF */
#define PowerMonitor_1_ADC_CFG1_IPCAP1VALUE        (0X0440u)
#define PowerMonitor_1_ADC_CFG1_DACAPVALUE         (0X09C0u)

/* Below defines are depricated and should not be used. These are provided 
   just to keep the component backward compatible */
#define PowerMonitor_1_ADC_DEFAULT_SRATE                      PowerMonitor_1_ADC_CFG1_SRATE              
#define PowerMonitor_1_ADC_CLOCKS_PER_SAMPLE                  PowerMonitor_1_ADC_CFG1_CLOCKS_PER_SAMPLE  
#define PowerMonitor_1_ADC_DFLT_CLOCK_FREQ                    PowerMonitor_1_ADC_CFG1_CLOCK_FREQ         
#define PowerMonitor_1_ADC_REFERENCE_VOLTAGE                  PowerMonitor_1_ADC_CFG1_REFERENCE_VOLTAGE  
#define PowerMonitor_1_ADC_DFLT_COUNTS_PER_VOLT               PowerMonitor_1_ADC_CFG1_COUNTS_PER_VOLT    
#define PowerMonitor_1_ADC_DEFAULT_RESOLUTION                 PowerMonitor_1_ADC_CFG1_RESOLUTION         
#define PowerMonitor_1_ADC_DEFAULT_CONV_MODE                  PowerMonitor_1_ADC_CFG1_CONV_MODE           
#define PowerMonitor_1_ADC_DEFAULT_IDEAL_DEC_GAIN             PowerMonitor_1_ADC_CFG1_IDEAL_DEC_GAIN     
#define PowerMonitor_1_ADC_DEFAULT_RANGE                      PowerMonitor_1_ADC_CFG1_RANGE        
#define PowerMonitor_1_ADC_DEFAULT_RANGE_VALUE                PowerMonitor_1_ADC_CFG1_RANGE_VALUE  
#define PowerMonitor_1_ADC_DEFAULT_REFERENCE                  PowerMonitor_1_ADC_CFG1_REFERENCE         
#define PowerMonitor_1_ADC_DEFAULT_BUF_GAIN                   PowerMonitor_1_ADC_CFG1_BUF_GAIN       

/* Input and DAC Cap values in pF */
#define PowerMonitor_1_ADC_DEFAULT_IPCAP1VALUE                PowerMonitor_1_ADC_CFG1_IPCAP1VALUE       
#define PowerMonitor_1_ADC_DEFAULT_DACAPVALUE                 PowerMonitor_1_ADC_CFG1_DACAPVALUE        
#define PowerMonitor_1_ADC_DEFAULT_IDEAL_ODDDEC_GAIN          PowerMonitor_1_ADC_CFG1_IDEAL_ODDDEC_GAIN 


/* Default register settings for the Config 2 */
/* Second Config */
/* PowerMonitor_1_ADC_CFG2 Sample Rate: 10000 Samples per Second */
/* PowerMonitor_1_ADC_CFG2 Conversion Mode: 0 */
/* PowerMonitor_1_ADC_CFG2 Input Buffer Gain: 1   */
/* PowerMonitor_1_ADC_CFG2 Reference: 0 */
/* PowerMonitor_1_ADC_CFG2 Input Range: 10  */
/* PowerMonitor_1_ADC_CFG2 Resolution: 12 bits */
/* PowerMonitor_1_ADC_CFG2 Clock: 1310000 Hz */

#define PowerMonitor_1_ADC_CFG2_DEC_CR             (0X30u)
#define PowerMonitor_1_ADC_CFG2_DEC_SR             (0X14u)
#define PowerMonitor_1_ADC_CFG2_DEC_SHIFT1         (0X06u)
#define PowerMonitor_1_ADC_CFG2_DEC_SHIFT2         (0X09u)
#define PowerMonitor_1_ADC_CFG2_DEC_DR2            (0X00u)
#define PowerMonitor_1_ADC_CFG2_DEC_DR2H           (0X00u)
#define PowerMonitor_1_ADC_CFG2_DEC_DR1            (0X1Fu)
#define PowerMonitor_1_ADC_CFG2_DEC_OCOR           (0X00u)
#define PowerMonitor_1_ADC_CFG2_DEC_OCORM          (0X00u)
#define PowerMonitor_1_ADC_CFG2_DEC_OCORH          (0X00u)
#define PowerMonitor_1_ADC_CFG2_DEC_GVAL           (0X0Du)
#define PowerMonitor_1_ADC_CFG2_DEC_GCOR           (0X00u)
#define PowerMonitor_1_ADC_CFG2_DEC_GCORH          (0X00u)
#define PowerMonitor_1_ADC_CFG2_DEC_COHER          (0X01u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR0            (0X0Au)
#define PowerMonitor_1_ADC_CFG2_DSM_CR1            (0X00u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR2            (0X7Au)
#define PowerMonitor_1_ADC_CFG2_DSM_CR3            (0X00u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR4            (0X14u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR5            (0X39u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR6            (0X04u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR7            (0X00u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR8            (0X12u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR9            (0X13u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR10           (0X44u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR11           (0X42u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR12           (0X10u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR13           (0X00u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR14           (0X01u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR15           (0X11u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR16           (0X19u)
#define PowerMonitor_1_ADC_CFG2_DSM_CR17           (0X97u)
#define PowerMonitor_1_ADC_CFG2_DSM_REF0           (0X44u)
#define PowerMonitor_1_ADC_CFG2_DSM_REF2           (0X58u)
#define PowerMonitor_1_ADC_CFG2_DSM_REF3           (0X00u)
#define PowerMonitor_1_ADC_CFG2_DSM_DEM0           (0X07u)
#define PowerMonitor_1_ADC_CFG2_DSM_DEM1           (0X00u)
#define PowerMonitor_1_ADC_CFG2_DSM_BUF0           (0X01u)
#define PowerMonitor_1_ADC_CFG2_DSM_BUF1           (0X01u)
#define PowerMonitor_1_ADC_CFG2_DSM_BUF2           (0X01u)
#define PowerMonitor_1_ADC_CFG2_DSM_BUF3           (0X0Au)
#define PowerMonitor_1_ADC_CFG2_DSM_OUT0           (0X00u)
#define PowerMonitor_1_ADC_CFG2_DSM_OUT1           (0X00u)
#define PowerMonitor_1_ADC_CFG2_DSM_SW3            (0X00u)
#define PowerMonitor_1_ADC_CFG2_SRATE              (10000u )
#define PowerMonitor_1_ADC_CFG2_CLOCKS_PER_SAMPLE  (0X0083u)
#define PowerMonitor_1_ADC_CFG2_CLOCK_FREQ         (1310000u)
#define PowerMonitor_1_ADC_CFG2_CP_CLOCK_FREQ      (5240000u)
#define PowerMonitor_1_ADC_CFG2_REFERENCE_VOLTAGE  (1.0240)
#define PowerMonitor_1_ADC_CFG2_COUNTS_PER_VOLT    (32000u)
#define PowerMonitor_1_ADC_CFG2_RESOLUTION         (12u)
#define PowerMonitor_1_ADC_CFG2_CONVMODE           (0u)
#define PowerMonitor_1_ADC_CFG2_IDEAL_DEC_GAIN     (0X8FD2u)
#define PowerMonitor_1_ADC_CFG2_INPUT_RANGE        (10u)
#define PowerMonitor_1_ADC_CFG2_INPUT_RANGE_VALUE  (0.064)

#define PowerMonitor_1_ADC_CFG2_INPUT_MODE         (2u)
#define PowerMonitor_1_ADC_CFG2_REFERENCE          (0u)
#define PowerMonitor_1_ADC_CFG2_BUFFER_GAIN        (1u)

/* Input and DAC Cap values in pF */
#define PowerMonitor_1_ADC_CFG2_IPCAP1VALUE        (0X1640u)
#define PowerMonitor_1_ADC_CFG2_DACAPVALUE         (0X0190u)
#define PowerMonitor_1_ADC_CFG2_IDEAL_ODDDEC_GAIN  (0X8000u)

/* Below defines are depricated and should not be used. These are provided 
   just to keep the component backward compatible */
#define PowerMonitor_1_ADC_DEFAULT_SRATE_CFG2                 PowerMonitor_1_ADC_CFG2_SRATE              
#define PowerMonitor_1_ADC_CLOCKS_PER_SAMPLE_CFG2             PowerMonitor_1_ADC_CFG2_CLOCKS_PER_SAMPLE  
#define PowerMonitor_1_ADC_DFLT_CLOCK_FREQ_CFG2               PowerMonitor_1_ADC_CFG2_CLOCK_FREQ         
#define PowerMonitor_1_ADC_REFERENCE_VOLTAGE_CFG2             PowerMonitor_1_ADC_CFG2_REFERENCE_VOLTAGE  
#define PowerMonitor_1_ADC_DFLT_COUNTS_PER_VOLT_CFG2          PowerMonitor_1_ADC_CFG2_COUNTS_PER_VOLT    
#define PowerMonitor_1_ADC_DEFAULT_RESOLUTION_CFG2            PowerMonitor_1_ADC_CFG2_RESOLUTION         
#define PowerMonitor_1_ADC_CONVMODE_CFG2                      PowerMonitor_1_ADC_CFG2_CONVMODE           
#define PowerMonitor_1_ADC_DEFAULT_IDEAL_DEC_GAIN_CFG2        PowerMonitor_1_ADC_CFG2_IDEAL_DEC_GAIN     
#define PowerMonitor_1_ADC_DEFAULT_INPUT_RANGE_CFG2           PowerMonitor_1_ADC_CFG2_INPUT_RANGE        
#define PowerMonitor_1_ADC_DEFAULT_INPUT_RANGE_VALUE_CFG2     PowerMonitor_1_ADC_CFG2_INPUT_RANGE_VALUE  
#define PowerMonitor_1_ADC_DEFAULT_INPUT_MODE_CFG2            PowerMonitor_1_ADC_CFG2_INPUT_MODE         
#define PowerMonitor_1_ADC_DEFAULT_REFERENCE_CFG2             PowerMonitor_1_ADC_CFG2_REFERENCE         
#define PowerMonitor_1_ADC_DEFAULT_BUFFER_GAIN_CONFIG2        PowerMonitor_1_ADC_CFG2_BUFFER_GAIN       

/* Input and DAC Cap values in pF */
#define PowerMonitor_1_ADC_DEFAULT_IPCAP1VALUE_CFG2           PowerMonitor_1_ADC_CFG2_IPCAP1VALUE       
#define PowerMonitor_1_ADC_DEFAULT_DACAPVALUE_CFG2            PowerMonitor_1_ADC_CFG2_DACAPVALUE        
#define PowerMonitor_1_ADC_DEFAULT_IDEAL_ODDDEC_GAIN_CFG2     PowerMonitor_1_ADC_CFG2_IDEAL_ODDDEC_GAIN 


/* Default register settings for the following configuration */
/* Third Config */
/* PowerMonitor_1_ADC_CFG3 Sample Rate: 10000 Samples per Second */
/* PowerMonitor_1_ADC_CFG3 Conversion Mode: 0 */
/* PowerMonitor_1_ADC_CFG3 Input Buffer Gain: 1   */
/* PowerMonitor_1_ADC_CFG3 Reference: 0 */
/* PowerMonitor_1_ADC_CFG3 Input Range: 9  */
/* PowerMonitor_1_ADC_CFG3 Resolution: 12 bits */
/* PowerMonitor_1_ADC_CFG3 Clock: 1310000 Hz */

#define PowerMonitor_1_ADC_CFG3_DEC_CR             (0X30u)
#define PowerMonitor_1_ADC_CFG3_DEC_SR             (0X14u)
#define PowerMonitor_1_ADC_CFG3_DEC_SHIFT1         (0X06u)
#define PowerMonitor_1_ADC_CFG3_DEC_SHIFT2         (0X09u)
#define PowerMonitor_1_ADC_CFG3_DEC_DR2            (0X00u)
#define PowerMonitor_1_ADC_CFG3_DEC_DR2H           (0X00u)
#define PowerMonitor_1_ADC_CFG3_DEC_DR1            (0X1Fu)
#define PowerMonitor_1_ADC_CFG3_DEC_OCOR           (0X00u)
#define PowerMonitor_1_ADC_CFG3_DEC_OCORM          (0X00u)
#define PowerMonitor_1_ADC_CFG3_DEC_OCORH          (0X00u)
#define PowerMonitor_1_ADC_CFG3_DEC_GVAL           (0X0Du)
#define PowerMonitor_1_ADC_CFG3_DEC_GCOR           (0X00u)
#define PowerMonitor_1_ADC_CFG3_DEC_GCORH          (0X00u)
#define PowerMonitor_1_ADC_CFG3_DEC_COHER          (0X01u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR0            (0X0Au)
#define PowerMonitor_1_ADC_CFG3_DSM_CR1            (0X00u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR2            (0X7Au)
#define PowerMonitor_1_ADC_CFG3_DSM_CR3            (0X00u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR4            (0X14u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR5            (0X23u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR6            (0X05u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR7            (0X00u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR8            (0X12u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR9            (0X13u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR10           (0X44u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR11           (0X44u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR12           (0X10u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR13           (0X00u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR14           (0X01u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR15           (0X11u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR16           (0X19u)
#define PowerMonitor_1_ADC_CFG3_DSM_CR17           (0X97u)
#define PowerMonitor_1_ADC_CFG3_DSM_REF0           (0X44u)
#define PowerMonitor_1_ADC_CFG3_DSM_REF2           (0X58u)
#define PowerMonitor_1_ADC_CFG3_DSM_REF3           (0X00u)
#define PowerMonitor_1_ADC_CFG3_DSM_DEM0           (0X07u)
#define PowerMonitor_1_ADC_CFG3_DSM_DEM1           (0X00u)
#define PowerMonitor_1_ADC_CFG3_DSM_BUF0           (0X01u)
#define PowerMonitor_1_ADC_CFG3_DSM_BUF1           (0X01u)
#define PowerMonitor_1_ADC_CFG3_DSM_BUF2           (0X01u)
#define PowerMonitor_1_ADC_CFG3_DSM_BUF3           (0X0Au)
#define PowerMonitor_1_ADC_CFG3_DSM_OUT0           (0X00u)
#define PowerMonitor_1_ADC_CFG3_DSM_OUT1           (0X00u)
#define PowerMonitor_1_ADC_CFG3_DSM_SW3            (0X00u)
#define PowerMonitor_1_ADC_CFG3_SRATE              (10000u)
#define PowerMonitor_1_ADC_CFG3_CLOCKS_PER_SAMPLE  (0X0083u)
#define PowerMonitor_1_ADC_CFG3_CLOCK_FREQ         (1310000u)
#define PowerMonitor_1_ADC_CFG3_CP_CLOCK_FREQ      (5240000u)
#define PowerMonitor_1_ADC_CFG3_REFERENCE_VOLTAGE  (1.0240)
#define PowerMonitor_1_ADC_CFG3_COUNTS_PER_VOLT    (16000u)
#define PowerMonitor_1_ADC_CFG3_RESOLUTION         (12u)
#define PowerMonitor_1_ADC_CFG3_CONVMODE           (0u)
#define PowerMonitor_1_ADC_CFG3_IDEAL_DEC_GAIN     (0X919Du)
#define PowerMonitor_1_ADC_CFG3_INPUT_RANGE        (9u)
#define PowerMonitor_1_ADC_CFG3_INPUT_MODE         (2u)
#define PowerMonitor_1_ADC_CFG3_REFERENCE          (0u)
#define PowerMonitor_1_ADC_CFG3_BUFFER_GAIN        (1u)
#define PowerMonitor_1_ADC_CFG3_INPUT_RANGE_VALUE  (0.128)

/* Input and DAC Cap values in pF */
#define PowerMonitor_1_ADC_CFG3_IPCAP1VALUE        (0X0DA0u)
#define PowerMonitor_1_ADC_CFG3_DACAPVALUE         (0X01F0u)
#define PowerMonitor_1_ADC_CFG3_IDEAL_ODDDEC_GAIN  (0X8000u)

/* Below defines are depricated and should not be used. These are provided 
   just to keep the component backward compatible */
#define PowerMonitor_1_ADC_DEFAULT_SRATE_CFG3                 PowerMonitor_1_ADC_CFG3_SRATE              
#define PowerMonitor_1_ADC_CLOCKS_PER_SAMPLE_CFG3             PowerMonitor_1_ADC_CFG3_CLOCKS_PER_SAMPLE  
#define PowerMonitor_1_ADC_DFLT_CLOCK_FREQ_CFG3               PowerMonitor_1_ADC_CFG3_CLOCK_FREQ         
#define PowerMonitor_1_ADC_REFERENCE_VOLTAGE_CFG3             PowerMonitor_1_ADC_CFG3_REFERENCE_VOLTAGE  
#define PowerMonitor_1_ADC_DFLT_COUNTS_PER_VOLT_CFG3          PowerMonitor_1_ADC_CFG3_COUNTS_PER_VOLT    
#define PowerMonitor_1_ADC_DEFAULT_RESOLUTION_CFG3            PowerMonitor_1_ADC_CFG3_RESOLUTION         
#define PowerMonitor_1_ADC_CONVMODE_CFG3                      PowerMonitor_1_ADC_CFG3_CONVMODE           
#define PowerMonitor_1_ADC_DEFAULT_IDEAL_DEC_GAIN_CFG3        PowerMonitor_1_ADC_CFG3_IDEAL_DEC_GAIN     
#define PowerMonitor_1_ADC_DEFAULT_INPUT_RANGE_CFG3           PowerMonitor_1_ADC_CFG3_INPUT_RANGE        
#define PowerMonitor_1_ADC_DEFAULT_INPUT_RANGE_VALUE_CFG3     PowerMonitor_1_ADC_CFG3_INPUT_RANGE_VALUE  
#define PowerMonitor_1_ADC_DEFAULT_INPUT_MODE_CFG3            PowerMonitor_1_ADC_CFG3_INPUT_MODE         
#define PowerMonitor_1_ADC_DEFAULT_REFERENCE_CFG3             PowerMonitor_1_ADC_CFG3_REFERENCE         
#define PowerMonitor_1_ADC_DEFAULT_BUFFER_GAIN_CONFIG3        PowerMonitor_1_ADC_CFG3_BUFFER_GAIN       

/* Input and DAC Cap values in pF */
#define PowerMonitor_1_ADC_DEFAULT_IPCAP1VALUE_CFG3           PowerMonitor_1_ADC_CFG3_IPCAP1VALUE       
#define PowerMonitor_1_ADC_DEFAULT_DACAPVALUE_CFG3            PowerMonitor_1_ADC_CFG3_DACAPVALUE        
#define PowerMonitor_1_ADC_DEFAULT_IDEAL_ODDDEC_GAIN_CFG3     PowerMonitor_1_ADC_CFG3_IDEAL_ODDDEC_GAIN 


/* Default register settings for the following configuration */
/* Fourth Config */
/* PowerMonitor_1_ADC_CFG4 Sample Rate: 10000 Samples per Second */
/* PowerMonitor_1_ADC_CFG4 Conversion Mode: 0 */
/* PowerMonitor_1_ADC_CFG4 Input Buffer Gain: 1   */
/* PowerMonitor_1_ADC_CFG4 Reference: 0 */
/* PowerMonitor_1_ADC_CFG4 Input Range: 4  */
/* PowerMonitor_1_ADC_CFG4 Resolution: 12 bits */
/* PowerMonitor_1_ADC_CFG4 Clock: 1310000 Hz */

#define PowerMonitor_1_ADC_CFG4_DEC_CR             (0X30u)
#define PowerMonitor_1_ADC_CFG4_DEC_SR             (0X14u)
#define PowerMonitor_1_ADC_CFG4_DEC_SHIFT1         (0X06u)
#define PowerMonitor_1_ADC_CFG4_DEC_SHIFT2         (0X09u)
#define PowerMonitor_1_ADC_CFG4_DEC_DR2            (0X00u)
#define PowerMonitor_1_ADC_CFG4_DEC_DR2H           (0X00u)
#define PowerMonitor_1_ADC_CFG4_DEC_DR1            (0X1Fu)
#define PowerMonitor_1_ADC_CFG4_DEC_OCOR           (0X00u)
#define PowerMonitor_1_ADC_CFG4_DEC_OCORM          (0X00u)
#define PowerMonitor_1_ADC_CFG4_DEC_OCORH          (0X00u)
#define PowerMonitor_1_ADC_CFG4_DEC_GVAL           (0X0Du)
#define PowerMonitor_1_ADC_CFG4_DEC_GCOR           (0X00u)
#define PowerMonitor_1_ADC_CFG4_DEC_GCORH          (0X00u)
#define PowerMonitor_1_ADC_CFG4_DEC_COHER          (0X01u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR0            (0X0Au)
#define PowerMonitor_1_ADC_CFG4_DSM_CR1            (0X00u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR2            (0X7Au)
#define PowerMonitor_1_ADC_CFG4_DSM_CR3            (0X00u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR4            (0X14u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR5            (0X09u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR6            (0X0Au)
#define PowerMonitor_1_ADC_CFG4_DSM_CR7            (0X00u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR8            (0X12u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR9            (0X13u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR10           (0X55u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR11           (0X5Au)
#define PowerMonitor_1_ADC_CFG4_DSM_CR12           (0X05u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR13           (0X00u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR14           (0X01u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR15           (0X11u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR16           (0X19u)
#define PowerMonitor_1_ADC_CFG4_DSM_CR17           (0X97u)
#define PowerMonitor_1_ADC_CFG4_DSM_REF0           (0X44u)
#define PowerMonitor_1_ADC_CFG4_DSM_REF2           (0X58u)
#define PowerMonitor_1_ADC_CFG4_DSM_REF3           (0X00u)
#define PowerMonitor_1_ADC_CFG4_DSM_DEM0           (0X07u)
#define PowerMonitor_1_ADC_CFG4_DSM_DEM1           (0X00u)
#define PowerMonitor_1_ADC_CFG4_DSM_BUF0           (0X01u)
#define PowerMonitor_1_ADC_CFG4_DSM_BUF1           (0X01u)
#define PowerMonitor_1_ADC_CFG4_DSM_BUF2           (0X01u)
#define PowerMonitor_1_ADC_CFG4_DSM_BUF3           (0X0Au)
#define PowerMonitor_1_ADC_CFG4_DSM_OUT0           (0X00u)
#define PowerMonitor_1_ADC_CFG4_DSM_OUT1           (0X00u)
#define PowerMonitor_1_ADC_CFG4_DSM_SW3            (0X00u)
#define PowerMonitor_1_ADC_CFG4_SRATE              (10000u)         
#define PowerMonitor_1_ADC_CFG4_CLOCKS_PER_SAMPLE  (0X0083u)
#define PowerMonitor_1_ADC_CFG4_CLOCK_FREQ         (1310000u)
#define PowerMonitor_1_ADC_CFG4_CP_CLOCK_FREQ      (5240000u)
#define PowerMonitor_1_ADC_CFG4_REFERENCE_VOLTAGE  (1.0240)
#define PowerMonitor_1_ADC_CFG4_COUNTS_PER_VOLT    (2000u)
#define PowerMonitor_1_ADC_CFG4_RESOLUTION         (12u)
#define PowerMonitor_1_ADC_CFG4_CONVMODE           (0u)
#define PowerMonitor_1_ADC_CFG4_IDEAL_DEC_GAIN     (0X8DB6u)
#define PowerMonitor_1_ADC_CFG4_INPUT_RANGE        (4u)
#define PowerMonitor_1_ADC_CFG4_INPUT_MODE         (2u)
#define PowerMonitor_1_ADC_CFG4_REFERENCE          (0u)
#define PowerMonitor_1_ADC_CFG4_BUFFER_GAIN        (1u)
#define PowerMonitor_1_ADC_CFG4_INPUT_RANGE_VALUE  (1.024)

/* Input cap and DAC Cap values in pF */
#define PowerMonitor_1_ADC_CFG4_IPCAP1VALUE        (0X0380u)
#define PowerMonitor_1_ADC_CFG4_DACAPVALUE         (0X03E0u)
#define PowerMonitor_1_ADC_CFG4_IDEAL_ODDDEC_GAIN  (0X8000u)

/* Below defines are depricated and should not be used. These are provided 
   just to keep the component backward compatible */
#define PowerMonitor_1_ADC_DEFAULT_SRATE_CFG4                 PowerMonitor_1_ADC_CFG4_SRATE              
#define PowerMonitor_1_ADC_CLOCKS_PER_SAMPLE_CFG4             PowerMonitor_1_ADC_CFG4_CLOCKS_PER_SAMPLE  
#define PowerMonitor_1_ADC_DFLT_CLOCK_FREQ_CFG4               PowerMonitor_1_ADC_CFG4_CLOCK_FREQ         
#define PowerMonitor_1_ADC_REFERENCE_VOLTAGE_CFG4             PowerMonitor_1_ADC_CFG4_REFERENCE_VOLTAGE  
#define PowerMonitor_1_ADC_DFLT_COUNTS_PER_VOLT_CFG4          PowerMonitor_1_ADC_CFG4_COUNTS_PER_VOLT    
#define PowerMonitor_1_ADC_DEFAULT_RESOLUTION_CFG4            PowerMonitor_1_ADC_CFG4_RESOLUTION         
#define PowerMonitor_1_ADC_CONVMODE_CFG4                      PowerMonitor_1_ADC_CFG4_CONVMODE           
#define PowerMonitor_1_ADC_DEFAULT_IDEAL_DEC_GAIN_CFG4        PowerMonitor_1_ADC_CFG4_IDEAL_DEC_GAIN     
#define PowerMonitor_1_ADC_DEFAULT_INPUT_RANGE_CFG4           PowerMonitor_1_ADC_CFG4_INPUT_RANGE        
#define PowerMonitor_1_ADC_DEFAULT_INPUT_RANGE_VALUE_CFG4     PowerMonitor_1_ADC_CFG4_INPUT_RANGE_VALUE  
#define PowerMonitor_1_ADC_DEFAULT_INPUT_MODE_CFG4            PowerMonitor_1_ADC_CFG4_INPUT_MODE         
#define PowerMonitor_1_ADC_DEFAULT_REFERENCE_CFG4             PowerMonitor_1_ADC_CFG4_REFERENCE         
#define PowerMonitor_1_ADC_DEFAULT_BUFFER_GAIN_CONFIG4        PowerMonitor_1_ADC_CFG4_BUFFER_GAIN       

/* Input and DAC Cap values in pF */
#define PowerMonitor_1_ADC_DEFAULT_IPCAP1VALUE_CFG4           PowerMonitor_1_ADC_CFG4_IPCAP1VALUE       
#define PowerMonitor_1_ADC_DEFAULT_DACAPVALUE_CFG4            PowerMonitor_1_ADC_CFG4_DACAPVALUE        
#define PowerMonitor_1_ADC_DEFAULT_IDEAL_ODDDEC_GAIN_CFG4     PowerMonitor_1_ADC_CFG4_IDEAL_ODDDEC_GAIN 

#endif /* CY_ADC_DELSIG_PowerMonitor_1_ADC_H */


/* [] END OF FILE */
