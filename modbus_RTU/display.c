/*
 * display.c
 *
 * Created: 07.03.2024 9:12:36
 *  Author: xpiroc03
 */ 

#include "display.h"

static void write_4bits(uint8_t data)
{
	PORTD = (PORTD & 0x0F) | (data & 0xF0);
	LCD_EN_HIGH();
	delay(1000);
	LCD_EN_LOW();
}

void LCD_send_fprintf(char data, FILE* stream)
{
	LCD_write_char(data);
}

void LCD_init(void)
{
	delay(30000);
	
	DDRD |= (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7);
	DDRB |= (1<<DDB0) | (1<<DDB1);
	
	PORTB &= ~(1 << PINB1);
	PORTB &= ~(1 << PINB0);
	
	write_4bits(0x30);
	delay(8000);
	write_4bits(0x30);
	delay(1000);
	write_4bits(0x30);
	delay(1000);
	
	write_4bits(0x20);
	delay(1000);
	LCD_send_instruction(0x28);
	delay(1000);
	LCD_send_instruction(0x0C);
	delay(1000);
	LCD_send_instruction(0x01);
	delay(1000);
	//LCD_send_instruction(0x06);
}

void LCD_send_instruction(uint8_t data)
{
	LCD_RS_LOW();
	write_4bits(data);
	write_4bits(data << 4);
	delay(1000);
}

void LCD_write_char(char data)
{
	LCD_RS_HIGH(); 
	write_4bits(data); 
	write_4bits(data << 4);
	delay(1000);
}

void LCD_write_str(char *str)
{
	while(*str)
		LCD_write_char(*str++);
}

void LCD_clear(void)
{
	LCD_send_instruction(0x01);
	delay(2000);
}

void LCD_home(void)
{
	LCD_send_instruction(0x02);
	delay(2000);
}

void LCD_set_cursor(uint8_t row, uint8_t col)
{
	uint8_t address = (!row ? 0x00 : 0x40) + col;
	LCD_send_instruction(0x80 | address);
	delay(2000);
}

void LCD_shift_cursor_left(void)
{
	LCD_send_instruction(0x10);
	delay(2000);
}

void LCD_shift_cursor_right(void)
{
	LCD_send_instruction(0x14);
	delay(2000);
}

void LCD_shift_display_left(void)
{
	LCD_send_instruction(0x18);
	delay(2000);
}

void LCD_shift_display_right(void)
{
	LCD_send_instruction(0x1C);
	delay(2000);
}

uint16_t buttons_read(void)
{
	uint16_t button = 0;
	uint16_t adc_value = ADC_read(0);
	
	if(adc_value > 600 && adc_value < 700)
		button = 1; //SELECT
	if(adc_value > 350 && adc_value < 450)
		button = 2; //LEFT
	if(adc_value > 200 && adc_value < 300)
		button = 3; //DOWN
	if(adc_value > 50 && adc_value < 150)
		button = 4; //UP
	if(adc_value < 50)
		button = 5; //RIGHT
	
	if(button != 0)
	{
		while(adc_value < 1000)
			adc_value = ADC_read(0);
	}
	
	return button;
}