/*
 * timer.h
 *
 * Created: 24.02.2024 22:08:52
 *  Author: xpiroc03
 */ 


#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

void timerInit(void);
uint32_t getTime(void);
void delay(uint32_t us);




