/*
 * relay.h
 *
 * Created: 09.04.2024 21:26:03
 *  Author: xpiroc03
 */ 
#pragma once

#include <avr/io.h>
#include <stdbool.h>

#define RELAY_HEATING_MASK	(1 << PORTB5)
#define RELAY_COOLING_MASK	(1 << PORTB4)

#define RELAY_INIT()			(DDRB |= RELAY_HEATING_MASK | RELAY_COOLING_MASK)
#define SET_RELAY_HEATING()		(PORTB |= RELAY_HEATING_MASK)
#define SET_RELAY_COOLING()		(PORTB |= RELAY_COOLING_MASK)
#define RESET_RELAY_HEATING()	(PORTB &= ~RELAY_HEATING_MASK)
#define RESET_RELAY_COOLING()	(PORTB &= ~RELAY_COOLING_MASK)

extern bool is_heating_relay_on;
extern bool is_cooling_relay_on;

void set_relay_heating(void);
void reset_relay_heating(void);
void set_relay_cooling(void);
void reset_relay_cooling(void);