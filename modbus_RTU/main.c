/*
 * modbus_RTU.c
 *
 * Created: 27.04.2024 19:02:16
 * Author : xpiroc03
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "display.h"
#include "relay.h"
#include "modbus.h"

FILE lcd_out = FDEV_SETUP_STREAM(LCD_write_char, NULL, _FDEV_SETUP_WRITE);

#define LED_MASK (1<<5)

uint8_t request[MODBUS_BUFFER_SIZE] = {0};
uint8_t received_value = 0;
uint8_t received_index = 0;
bool received_flag = false;

uint8_t modbus_status_register = 0;
uint16_t upper_limit = 25;
uint16_t lower_limit = 15;

ISR(USART_RX_vect)
{
	received_value = UDR0;
	if(received_index < MODBUS_BUFFER_SIZE)
	{
		request[received_index] = received_value;
		received_index++;
		TCNT0 = 0;
	}
}

ISR(TIMER0_OVF_vect)
{
	if(received_index > 0)
	{
		received_flag = true;
		TIMSK0 &= ~(1 << TOIE0);
	}
}


int main(void)
{
	timerInit();
	ADC_init();
	USART_Init();
	LCD_init();
	RELAY_INIT();
	
	TCCR0B |= (1 << CS02);
	TCNT0 = 0;
	TIMSK0 |= (1 << TOIE0);
		
    while(1)
    {
		if(received_flag == true)
		{
			modbus_process_request(request);
			LCD_clear();
			LCD_set_cursor(0,0);
			fprintf(&lcd_out, "%.1f C", get_temperature(ADC_read_voltage(1)));
			LCD_set_cursor(1,0);
			fprintf(&lcd_out, "%d %d", lower_limit, upper_limit);
		}
    }
}

