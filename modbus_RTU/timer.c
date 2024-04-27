/*
 * timer.c
 *
 * Created: 24.02.2024 22:12:26
 *  Author: xpiroc03
 */ 

#include "timer.h"

static volatile uint16_t timer1overflowCount = 0;
uint32_t actual_time = 0;


ISR(TIMER1_OVF_vect)
{
	++timer1overflowCount;
}

void timerInit(void)
{
	TCNT1 = 0;
	TIFR1 = (1<<TOV1) | (1<<OCF1A);
	TCCR1B = (1<<CS11);
	TIMSK1 |= (1<<TOIE1);
	sei();
}

uint32_t getTime(void)
{
	cli();
	actual_time = ((uint32_t)timer1overflowCount << 15) | (TCNT1 >> 1);
	sei();
	
	return actual_time;
}

void delay(uint32_t us)
{
	us += getTime();
	
	while(getTime() < us);
}
