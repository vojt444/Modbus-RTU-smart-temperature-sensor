/*
 * modbus.h
 *
 * Created: 27.04.2024 19:39:25
 *  Author: xpiroc03
 */ 

#pragma once

#include <avr/io.h>
#include <stdbool.h>
#include "adc.h"
#include "uart.h"

#define MODBUS_ADDRESS				0x01

#define MODBUS_REGISTER_SIZE		4
#define MODBUS_BUFFER_SIZE			16
#define READ_HOLDING_REGISTERS		0x03
#define WRITE_MULTIPLE_REGISTERS	0x10

#define REGISTER_TEMPERATURE		1
#define REGISTER_UPPER_LIMIT		2
#define REGISTER_LOWER_LIMIT		3
#define STATUS_WORD					4

#define WATCHDOG_BIT				0
#define TEMP_OK						1
#define TEMP_HIGH					2
#define TEMP_LOW					3

void modbus_process_request(uint8_t *request);
void modbus_write_register(uint16_t address, uint16_t value);

extern uint8_t request[MODBUS_BUFFER_SIZE];
extern uint8_t received_value;
extern uint8_t received_index;
extern bool received_flag;

extern uint8_t modbus_status_register;
extern uint16_t upper_limit;
extern uint16_t lower_limit;