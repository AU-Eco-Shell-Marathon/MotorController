/* ========================================
 *
 * adc_drv.c
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
 
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>

#include "adcDef.h"

#define ADC_REFS_MASK (_BV(REFS0) | _BV(REFS1))
#define ADC_REFS0_IDX (REFS0)


void adc_enable(_Bool status)
{
    if (status != false)
    {
        ADCSRA |= _BV(ADEN);
    }
    else
    {
        ADCSRA &= ~_BV(ADEN);
    }
}

void adc_interrupt_enable(_Bool intEn)
{
    if (intEn != false)
    {
        ADCSRA |= _BV(ADIE);
    }
    else
    {
        ADCSRA &= ~_BV(ADIE);
    }
}

void adc_left_adjust(_Bool adjust)
{
    if (adjust != false)
    {
        ADMUX |= _BV(ADLAR);
    }
    else
    {
        ADMUX &= ~_BV(ADLAR);
    }
}

void adc_prescaler_selection(adc_prescaler_t prescaler)
{
    ADCSRA &= ~(_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));
    ADCSRA |= prescaler << ADPS0;
}

uint16_t adc_read(void)
{
    return ADCW;
}

uint8_t adc_read_high(void)
{
    return ADCH;
}

uint8_t adc_read_low(void)
{
    return ADCL;
}

_Bool adcReadIntFlag(void)
{
    return (ADCSRA & _BV(ADIF)) == 0 ? false : true;
}

_Bool adcIsEndConversion(void)
{
	return(ADCSRA & _BV(ADSC)) == 0 ? true : false;
}

void adcClearIntFlag(void)
{
    //Write a logical 1 to clear the flag
    ADCSRA |= _BV(ADIF);
}

void adc_select_vref(adc_voltage_ref_t ref)
{
    ADMUX &= ~(ADC_REFS_MASK);
    ADMUX |= (ref & 0x03) << ADC_REFS0_IDX;
    ADCSRB &= ~_BV(ISRCEN);
    if(ref & 0x04)
    {
        ADCSRB |= _BV(AREFEN);
    }
    else
    {
        ADCSRB &= ~_BV(AREFEN);
    }
}

void adc_start_conversion(void)
{
    ADCSRA |= _BV(ADSC);
}

void adc_digital_input_disable(adc_digital_channel_t chanList)
{
    DIDR0 |= (uint8_t)chanList;
    DIDR1 |= (uint8_t)(chanList>>8);
}

void adc_digital_input_enable(adc_digital_channel_t chanList)
{
    DIDR0 &= ~((uint8_t)chanList);
    DIDR1 &= ~((uint8_t)(chanList>>8));
}

void adc_trigger_enable(_Bool trigEn)
{
    if(trigEn != false)
    {
        ADCSRA |= _BV(ADATE);
    }
    else
    {
        ADCSRA &= ~_BV(ADATE);
    }
}

void adc_set_trigger_source(adc_trigger_source_t trigger)
{
	ADCSRB &= ~(_BV(ADTS3) | _BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0));
    ADCSRB |= trigger;
}

void adc_select_input(adc_input_channel_t channel)
{
	ADMUX &= ~(_BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0));
	ADMUX |= channel;
}
	
	
	