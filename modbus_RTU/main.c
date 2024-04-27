/*
 * modbus_RTU.c
 *
 * Created: 27.04.2024 19:02:16
 * Author : xpiroc03
 */ 

#include <avr/io.h>
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "modbus.h"


int main(void)
{
	timerInit();
	ADC_init();
	USART_Init();

    while (1) 
    {
		
    }
}

