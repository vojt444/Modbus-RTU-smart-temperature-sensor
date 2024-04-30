/*
 * display.h
 *
 * Created: 07.03.2024 9:12:52
 *  Author: xpiroc03
 */ 


#pragma once

#include <avr/io.h>
#include <stdio.h>
#include "timer.h"
#include "adc.h"

#define LCD_RS_HIGH()	(PORTB |= (1 << PORTB0))
#define LCD_RS_LOW()	(PORTB &= ~(1 << PORTB0))
#define LCD_EN_HIGH()	(PORTB |= (1 << PORTB1))
#define LCD_EN_LOW()	(PORTB &= ~(1 << PORTB1))

#define BUTTON_SELECT	1
#define BUTTON_LEFT		2
#define BUTTON_DOWN		3
#define BUTTON_UP		4
#define BUTTON_RIGHT	5

extern FILE lcd_out;

void LCD_send_fprintf(char data, FILE* stream);
void LCD_init(void);
void LCD_send_instruction(uint8_t data);
void LCD_write_char(char data);
void LCD_write_str(char *str);
void LCD_clear(void);
void LCD_home(void);
void LCD_set_cursor(uint8_t row, uint8_t col);
void LCD_shift_cursor_left(void);
void LCD_shift_cursor_right(void);
void LCD_shift_display_left(void);
void LCD_shift_display_right(void);

uint16_t buttons_read(void);
