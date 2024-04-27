/*
 * modbus.h
 *
 * Created: 27.04.2024 19:39:25
 *  Author: xpiroc03
 */ 

#pragma once

#define MODBUS_ADDRESS	1

#define READ_HOLDING_REGISTERS 0x03
#define WRITE_SINGLE_REGISTER 0x06

#define REGISTER_TEMPERATURE 1
#define REGISTER_UPPER_LIMIT 2
#define REGISTER_LOWER_LIMIT 3