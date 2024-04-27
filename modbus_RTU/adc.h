/*
 * timer.c
 *
 * Created: 25.02.2024 13:21:35
 *  Author: xpiroc03
 */ 

#pragma once

#include <avr/io.h>
#include <math.h>

#define B 3895.0
#define T0 298.15
#define R0 10000.0
#define R1 20000.0
#define R2 1500.0
#define Ucc 5.0


void ADC_init(void);
uint16_t ADC_read(uint8_t channel);
float ADC_read_voltage(uint8_t channel);
float get_temperature(float voltage);



