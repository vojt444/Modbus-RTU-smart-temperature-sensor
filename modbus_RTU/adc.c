/*
 * timer.c
 *
 * Created: 25.02.2024 13:21:25
 *  Author: xpiroc03
 */ 

#include "adc.h"


void ADC_init(void)
{
	DIDR0 |= (1<<ADC1D);
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
	ADMUX |= (1 << REFS0); 
}

uint16_t ADC_read(uint8_t channel)
{
	ADMUX &= ~(MUX3 | MUX2 | MUX1 | MUX0);
	ADMUX |= channel;
	
	ADCSRA |= (1<<ADSC); 
	
	while(ADCSRA & (1<<ADSC)); 
	
	return ADC; 
}

float ADC_read_voltage(uint8_t channel)
{
	ADMUX &= ~(MUX3 | MUX2 | MUX1 | MUX0);
	ADMUX |= channel;
	
	ADCSRA |= (1<<ADSC); 
	
	while(ADCSRA & (1<<ADSC)); 
	
	return (float)ADC*5.0/1024; 
}

float get_temperature(float voltage)
{
	//float voltage = ADC_to_voltage(); 
	float Rth = (R1*voltage)/(Ucc-voltage) - R2;
	float temperature = B / (log(Rth/R0)+(B/T0));
	return temperature - 273.15; 
}