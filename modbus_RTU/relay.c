/*
 * relay.c
 *
 * Created: 09.04.2024 21:25:54
 *  Author: xpiroc03
 */ 
#include "relay.h"

void set_relay_heating(void)
{
	SET_RELAY_HEATING();
	is_heating_relay_on = true;
}

void reset_relay_heating(void)
{
	RESET_RELAY_HEATING();
	is_heating_relay_on = false;
}

void set_relay_cooling(void)
{
	SET_RELAY_COOLING();
	is_cooling_relay_on = true;
}
void reset_relay_cooling(void)
{
	RESET_RELAY_COOLING();
	is_cooling_relay_on = false;
}