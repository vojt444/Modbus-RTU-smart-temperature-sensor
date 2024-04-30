# Simple Modbus-RTU smart temperature sensor with ATmega328p
ATmega328p acts as a Modbus server (it's just waiting for the request from client).
You can use any Modbus RTU client available on the internet. 

| Register   | Name   | Description     |
|------------|--------|-----------------|
|1  | Temperature   | Read temperature|
|2  | Lower limit   | Set lower limit of the temperature|
|3  | Upper limit   | Set the upper limit of the temperature|
|4  | Status word   | Bit 0 - watchdog (swap 0/1 after every read)|
|   |               | Bit 1 - Temperature between the limits|
|   |               | Bit 2 - Temperature above the upper limit|
|   |               | Bit 3 - Temperature under the lower limit|