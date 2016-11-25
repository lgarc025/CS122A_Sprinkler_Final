#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>
#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>

//FreeRTOS include files
//#include "FreeRTOS.h"
//#include "task.h"
//#include "croutine.h"



#define MIN_INTERVAL 2000;
bool LOW = 0;
bool HIGH = 1;


//8000 Cycle Ticks or 1 MS
uint32_t maxcycles = 16000;

//Date Storage
uint8_t data[5] = {0,0,0,0,0};

uint32_t expectPulse(bool level) 
{	
	uint32_t count = 0;
	// On AVR platforms use direct GPIO port access as it's much faster and better
	// for catching pulses that are 10's of microseconds in length:
	while ((PINC & 0x01) == level) 
	{
		if (count++ >= maxcycles)
		{
			return 0; // Exceeded timeout, fail.
		}
	}					

	return count;
}
 

int ReadTemp ()
{
	//clear 40 bits for data 
	data[0] = data[1] = data[2] = data[3] = data[4];
	
	// Go into high impedence state to let pull-up raise data line level and
	// start the reading process.
	//Set DDRB For OutPut
	DDRC = DDRC | 0x01;
	// Go into high impedence state to let pull-up raise data line level and
	// start the reading process.
	PORTC = PORTC | 0x01;
	_delay_ms(250);
	
	 // First set data line low for 20 milliseconds.
	 DDRC = DDRC| 0x01;
	 PORTC = PORTC & 0xFE;
	 _delay_ms(20);
	 
	 uint32_t cycles[80];
	 
	 // End the start signal by setting data line high for 40 microseconds.
	 PORTC = PORTC | 0x01;
	 _delay_us(40);
	 
	  // Now start reading the data line to get the value from the DHT sensor.
	  DDRC = DDRC & 0xFE;
	  // Delay a bit to let sensor pull data line low
	  _delay_us(10);
	  
	   // First expect a low signal for ~80 microseconds followed by a high signal
	   // for ~80 microseconds again.
	   if (expectPulse(LOW) == 0) {
		   
		   return false;
	   }
	   
	   if (expectPulse(HIGH) == 0) {
		   	   
			return false;
		}
		
    // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
    // microsecond low pulse followed by a variable length high pulse.  If the
    // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
    // then it's a 1.  We measure the cycle count of the initial 50us low pulse
    // and use that to compare to the cycle count of the high pulse to determine
    // if the bit is a 0 (high state cycle count < low state cycle count), or a
    // 1 (high state cycle count > low state cycle count). Note that for speed all
    // the pulses are read into a array and then examined in a later step.
    for (int i=0; i<80; i+=2)
	{
	    cycles[i]   = expectPulse(LOW);
	    cycles[i+1] = expectPulse(HIGH);
    }

	// Inspect pulses and determine which ones are 0 (high state cycle count < low
	// state cycle count), or 1 (high state cycle count > low state cycle count).
	for (int i=0; i<40; ++i) 
	{
		uint32_t lowCycles  = cycles[2*i];
		uint32_t highCycles = cycles[2*i+1];
		
		if ((lowCycles == 0) || (highCycles == 0)) 
		{
			return 0;
		}
		
		data[i/8] <<= 1;
		// Now compare the low and high cycle times to see if the bit is a 0 or 1.
		if (highCycles > lowCycles)
		{
			// High cycles are greater than 50us low cycle count, must be a 1.
			data[i/8] |= 1;
		}
		// Else high cycles are less than (or equal to, a weird case) the 50us low
		// cycle count so this must be a zero.  Nothing needs to be changed in the
		// stored data.
	}
	  // Check we read 40 bits and that the checksum matches.
	  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
	  {
		  return 2;
	  }
	  else 
	  {
		  return 0;
	  }
 }
 
float convertCtoF(float c) 
{
	return c * 1.8 + 32;
}
 

readTemperature(bool S) 
{
	float f = 0;
	f = data[2] & 0x7F;
	f *= 256;
	f += data[3];
	f *= 0.1;
	if (data[2] & 0x80)
	{
		f *= -1;
	}
	
	if(S) 
	{
		f = convertCtoF(f);
	}
			
	return f;
}

float readHumidity ()
{
	float f = 0;
	f = data[0];
	f *= 256;
	f += data[1];
	f *= 0.1;

	return f;
}

/*
int main(void)
{
	
	DDRA = 0xFF; PORTA = 0;
	//DDRD = 0x00; PORTB = 0;
	
	while (1)
	{
		//_delay_ms(2000);
		ReadTemp();
		PORTA = readTemperature(true);
		//int A = readHumiy();
		//PORTA = A;
		_delay_ms(2000);
	}
}
*/

	
		