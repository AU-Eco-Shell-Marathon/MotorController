/* ========================================
 *
 * adc_drv.h
 * Created: Spring 2014
 * Author: Simon M. Skov
 * 
 * SEM_IHA_MC @ Aarhus University
 *
 * Made for ATmega32M1
 * Source for: ADC driver
 *
 * Ver. 0.1
 *
 * ========================================
 */
 
#ifndef ADC_H_
#define ADC_H_

#include <stdbool.h>
#include <stdint.h>
#include "adcDef.h"
 

void adc_enable(_Bool status);
void adc_interrupt_enable (_Bool intEn);
void adc_left_adjust(_Bool adjust);
void adc_prescaler_selection(adc_prescaler_t prescaler);
uint16_t adc_read(void);
uint8_t adc_read_high(void);
uint8_t adc_read_low(void);
_Bool adcReadIntFlag(void);
_Bool adcIsEndConversion(void);
void adcClearIntFlag(void);
void adc_select_vref(adc_voltage_ref_t ref);
void adc_start_conversion(void);
void adc_digital_input_disable(adc_digital_channel_t chanList);
void adc_digital_input_enable(adc_digital_channel_t chanList);
void adc_trigger_enable(_Bool trigEn);
void adc_set_trigger_source (adc_trigger_source_t trigger);
void adc_select_input(adc_input_channel_t channel);
 
#endif /*ADC_H_*/