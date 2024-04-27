/*
 * usart.c
 *
 * Created: 25.02.2024 4:05:02
 *  Author: xpiroc03
 */ 

#include "uart.h"

void USART_Init(void)
{
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8); 
	UBRR0L = (uint8_t)(UBRR_VALUE); 
	
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0); 
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); 
}


void USART_Transmit(uint8_t data)
{
	while(!(UCSR0A & (1<<UDRE0))); 
	
	UDR0 = data; 
}

uint8_t USART_Receive(void)
{
	while(!(UCSR0A & (1<<RXC0))); 
	return UDR0; 
}

void uart_putchar(char c, FILE *stream)
{
	if(c == '\n')
		uart_putchar('\r', stream);
	
	USART_Transmit(c);
}