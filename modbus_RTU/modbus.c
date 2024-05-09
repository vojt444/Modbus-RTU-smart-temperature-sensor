/*
 * modbus.c
 *
 * Created: 27.04.2024 19:39:16
 *  Author: xpiroc03
 */ 
#include "modbus.h"

static uint8_t modbus_check_address(uint8_t *request)
{
	switch(request[1])
	{
		case READ_HOLDING_REGISTERS:
			if((request[2] == 0) && (request[3] <= MODBUS_REGISTER_SIZE) && (request[4] == 0) && (request[3] + request[5] <= MODBUS_REGISTER_SIZE + 1))
				return 0;
			else
				return 1;
		case WRITE_MULTIPLE_REGISTERS: 
			if((request[2] == 0) && (request[3] <= REGISTER_LOWER_LIMIT) && (request[3] >= REGISTER_UPPER_LIMIT) && (request[3] + request[5] <= REGISTER_LOWER_LIMIT + 1))
				return 0;
			else
				return 1;
		default:
			return 1;
	}
}

static uint16_t modbus_calculate_crc(uint8_t *data, uint8_t length)
{
	uint16_t crc_register = 0xFFFF;
	
	for(int i = 0; i < length; i++)
	{
		crc_register ^= (uint16_t)data[i];
		for (int j = 0; j < 8; j++)
		{
			if ((crc_register & 0x0001) == 0)
			{
				crc_register >>= 1;
			}
			else
			{
				crc_register >>= 1;
				crc_register ^= 0xA001;
			}
		}
	}

	return crc_register;
}

/*static uint16_t modbus_calculate_crc(uint8_t *data, uint8_t length)
{
	uint8_t CRC_reg_High = 0xFF;
	uint8_t CRC_reg_Low = 0xFF;
	unsigned LUT_index;
	
	static uint8_t LUT_CRC_High[] =
	{
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
	};
	
	static uint8_t LUT_CRC_Low[] =
	{
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
		0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
		0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
		0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
		0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
		0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
		0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
		0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
		0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
		0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
		0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
		0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
		0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
		0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
		0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
		0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
		0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
		0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
		0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
		0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
		0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
		0x43, 0x83, 0x41, 0x81, 0x80, 0x40
	};
	
	
	while(length--)
	{
		LUT_index = CRC_reg_High ^ *data++;
		CRC_reg_High = CRC_reg_Low ^ LUT_CRC_High[LUT_index];
		CRC_reg_Low = LUT_CRC_Low[LUT_index];
	}
	return (CRC_reg_Low << 8 | CRC_reg_High) ;
}*/

void modbus_process_request(uint8_t *request) 
{
	uint8_t buffer[MODBUS_BUFFER_SIZE] = {0};
	uint8_t exception_code = 0;
	uint8_t buffer_size = 0;
	uint8_t byte_count = 0;
	uint8_t number_of_points = 0;
	uint8_t starting_address = 0;
	uint16_t crc = 0;
	uint16_t temperature = ADC_read(1);
	float temperature_float = get_temperature(ADC_read_voltage(1));
	buffer[0] = MODBUS_ADDRESS;
	buffer[1] = request[1];
	
	if(modbus_check_address(request) != 0)
	{
		exception_code = 0x02;
	}
	else
	{
		switch(request[1])
		{
			case READ_HOLDING_REGISTERS:
				number_of_points = request[5];
				byte_count = request[5] * 2;
				buffer[2] = byte_count;
				starting_address = request[3];	
				uint8_t buffer_data_index_hi = 3;
				uint8_t buffer_data_index_lo = 4;	
				for(uint8_t i = starting_address; i < starting_address + number_of_points; i++)
				{
					switch(i)
					{
						case REGISTER_TEMPERATURE:
							buffer[buffer_data_index_hi] = (uint8_t)(temperature >> 8);
							buffer_data_index_hi = buffer_data_index_hi + 2;
							buffer[buffer_data_index_lo] = (uint8_t)temperature;
							buffer_data_index_lo = buffer_data_index_lo + 2;
							break;
						case REGISTER_UPPER_LIMIT:
							buffer[buffer_data_index_hi] = (uint8_t)(upper_limit >> 8);
							buffer_data_index_hi = buffer_data_index_hi + 2;
							buffer[buffer_data_index_lo] = (uint8_t)upper_limit;
							buffer_data_index_lo = buffer_data_index_lo + 2;
							break;
						case REGISTER_LOWER_LIMIT:
							buffer[buffer_data_index_hi] = (uint8_t)(lower_limit >> 8);
							buffer_data_index_hi = buffer_data_index_hi + 2;
							buffer[buffer_data_index_lo] = (uint8_t)lower_limit;
							buffer_data_index_lo = buffer_data_index_lo + 2;
							break;
						case STATUS_WORD:
							buffer[buffer_data_index_hi] = 0x00;
							buffer_data_index_hi = buffer_data_index_hi + 2;
							// watchdog
							modbus_status_register ^= (1 << WATCHDOG_BIT);
							// temperature bits
							if(temperature_float < lower_limit)
							{
								modbus_status_register &= ~(1 << TEMP_OK);
								modbus_status_register &= ~(1 << TEMP_HIGH);
								modbus_status_register |= (1 << TEMP_LOW);
							}
							else if(temperature_float > upper_limit)
							{
								modbus_status_register &= ~(1 << TEMP_OK);
								modbus_status_register |= (1 << TEMP_HIGH);
								modbus_status_register &= ~(1 << TEMP_LOW);
							}
							else
							{
								modbus_status_register |= (1 << TEMP_OK);
								modbus_status_register &= ~(1 << TEMP_HIGH);
								modbus_status_register &= ~(1 << TEMP_LOW);
							}
							buffer[buffer_data_index_lo] = modbus_status_register;
							buffer_data_index_lo = buffer_data_index_lo + 2;
							break;
						default:
							break;
					}
				}
				crc = modbus_calculate_crc(buffer, byte_count + 3);
				buffer[buffer_data_index_hi] = (uint8_t)crc;
				buffer[buffer_data_index_lo] = (uint8_t)(crc >> 8);
				buffer_size = 3 + byte_count + 2;			
				break;
			case WRITE_MULTIPLE_REGISTERS:
				starting_address = request[3];
				number_of_points = request[5];
				byte_count = request[6];
				buffer[2] = 0x00;
				buffer[3] = starting_address;
				buffer[4] = 0x00;
				buffer[5] = starting_address;
				uint8_t request_data_index_hi = 7;
				uint8_t request_data_index_lo = 8;
				for(uint8_t i = starting_address; i < starting_address + number_of_points; i++)
				{
					switch(i)
					{
						case REGISTER_UPPER_LIMIT: 
							upper_limit = (request[request_data_index_hi] << 8) | request[request_data_index_lo];
							request_data_index_hi = request_data_index_hi + 2;
							request_data_index_lo = request_data_index_lo + 2;
							break;
						case REGISTER_LOWER_LIMIT:
							lower_limit = (request[request_data_index_hi] << 8) | request[request_data_index_lo];
							request_data_index_hi = request_data_index_hi + 2;
							request_data_index_lo = request_data_index_lo + 2;
							break;
					}
				}
				crc = modbus_calculate_crc(buffer, 6);
				buffer[6] = (uint8_t)crc;
				buffer[7] = (uint8_t)(crc >> 8);
				buffer_size = 8;
				break;
			default:
				exception_code = 0x01;
				break;
		}
	}
	
	if(exception_code != 0x00)
	{
		buffer_size = 5;
		buffer[1] |= 0x80;
		buffer[2] = exception_code;
		crc = modbus_calculate_crc(buffer, 3);
		buffer[3] = (uint8_t)crc;
		buffer[4] = (uint8_t)(crc >> 8);
	}
	USART_Transmit_array(buffer, buffer_size);
	received_index = 0;
	received_flag = false;
	TIMSK0 |= (1 << TOIE0);
}

