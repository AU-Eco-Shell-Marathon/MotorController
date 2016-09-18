/* ========================================
 *
 * adc_def.h
 * Created: Spring 2014
 * Author: Simon M. Skoc
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
 
#ifndef ADCDEF_H_
#define ADCDEF_H_
 

//List of available prescaler.
typedef enum adc_prescaler_e
{
    adc_prescaler_2 = 0,	// Oscillator clock divided by 2.
    adc_prescaler_2_,		// Oscillator clock divided by 2. Alternate setting.
    adc_prescaler_4, 		// Oscillator clock divided by 4.
    adc_prescaler_8,		// Oscillator clock divided by 8.
    adc_prescaler_16, 		// Oscillator clock divided by 16.
    adc_prescaler_32,		// Oscillator clock divided by 32.
    adc_prescaler_64,		// Oscillator clock divided by 64.
    adc_prescaler_128,		// Oscillator clock divided by 128.
}adc_prescaler_t;


// List of available voltage reference typedef.
typedef enum adc_voltage_ref_e
{
    adc_voltage_ref_avcc_wocap = 1,
    adc_voltage_ref_internal_2_56_wocap = 3,
    adc_voltage_ref_aref,
    adc_voltage_ref_avcc_wcap,
    adc_voltage_ref_internal_2_56_wcap = 7,
}adc_voltage_ref_t;

//List of available trigger sources typedef.
typedef enum adc_trigger_source_e
{
    adc_trigger_source_free_running = 0,
    adc_trigger_source_ext_int_request0,
    adc_trigger_source_timer_0_compare_match,
    adc_trigger_source_timer_0_overflow,
    adc_trigger_source_timer_1_compare_match_b,
    adc_trigger_source_timer_1_overflow,
    adc_trigger_source_timer_1_capture_event,
    adc_trigger_source_psc0asy_event,
    adc_trigger_source_psc1asy_event,
    adc_trigger_source_psc2asy_event,
    adc_trigger_source_analog_comparator_0,
    adc_trigger_source_analog_comparator_1,
    adc_trigger_source_analog_comparator_2,
    adc_trigger_source_analog_comparator_3
}adc_trigger_source_t;


//List of available input for ADC typedef.
typedef enum adc_input_channel_e
{
    adc_input_channel_0 = 0,
    adc_input_channel_1,
    adc_input_channel_2,
    adc_input_channel_3,
    adc_input_channel_4,
    adc_input_channel_5,
    adc_input_channel_6,
    adc_input_channel_7,
    adc_input_channel_8,
    adc_input_channel_9,
    adc_input_channel_10,
    adc_input_channel_temp_sensor,
    adc_input_channel_vcc_div_4,
    adc_input_channel_isrc,
    adc_input_channel_amp0,
    adc_input_channel_amp1,
    adc_input_channel_amp2,
    adc_input_channel_vbg,
    adc_input_channel_gnd
}adc_input_channel_t;

//List of digital input channel typedef.
typedef enum adc_digital_channel_e
{
    adc_digital_channel_0 = 0x0001,
    adc_digital_channel_1 = 0x0002,
    adc_digital_channel_2 = 0x0004,
    adc_digital_channel_3 = 0x0008,
    adc_digital_channel_4 = 0x0010,
    adc_digital_channel_5 = 0x0020,
    adc_digital_channel_6 = 0x0040,
    adc_digital_channel_amp2nd = 0x0040,
    adc_digital_channel_7 = 0x0080,
    adc_digital_channel_8 = 0x0100,
    adc_digital_channel_amp1nd = 0x0100,
    adc_digital_channel_9 = 0x0200,
    adc_digital_channel_amp1pd = 0x0200,
    adc_digital_channel_10 = 0x0400,
    adc_digital_channel_amp0nd = 0x0800,
    adc_digital_channel_amp0pd = 0x1000,
    adc_digital_channel_amp2pd = 0x4000
}adc_digital_channel_t;

#endif /*ADCDEF_H_*/