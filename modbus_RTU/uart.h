/*
 * usart.h
 *
 * Created: 25.02.2024 4:04:50
 *  Author: xpiroc03
 */ 


#pragma once

#include <stdio.h>
#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000
#endif
#define BAUD_RATE 38400
#define UBRR_VALUE ((F_CPU/(16*BAUD_RATE))-1)
#define UART_BUFFER_SIZE	16

extern FILE uart_out;

void USART_Init(void);
void USART_Transmit(uint8_t data);
uint8_t USART_Receive(void);

void uart_putchar(char c, FILE *stream);

