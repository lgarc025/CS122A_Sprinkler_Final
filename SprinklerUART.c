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

//FreeRTOS include files
#include "usart_ATmega1284.h"

bool Line1Status = 0;
bool Line2Status = 0;

unsigned char Line1Flow = 0;
unsigned char Line12low = 0;

enum flowstate {INIT,Wait,Flow,Release} flow_stateA;
enum flowstateB {INITB, WaitB, FlowB,ReleaseB } flow_stateB;
int counterA = 0;
int counterB = 0;

int FlowUpdateRate = 10;

unsigned char RecieveVar = 0;
unsigned char TrasmittVar = 0;

	
void FlowMeterA ()
{
	//actions
	switch(flow_stateA)
	{
		case INIT:
		counterA = 0;
		break;
		
		case Flow:
		counterA++;
		break;
	}
	
	//transitions
	switch(flow_stateA)
	{
		case INIT:
		flow_stateA = Wait;
		break;
		
		case Flow:
		flow_stateA = Release;
		break;
		
		case Release:
		if(!(PINA & 0x01))
		{
			flow_stateA = Wait;
		}
		break;
		
		
		case Wait:
		if((PINA & 0x01))
		{
			flow_stateA = Flow;
		}
		break;
	}		
}

void FlowMeterB ()
{
	//actions
	switch(flow_stateB)
	{
		case INITB:
		counterB = 0;
		break;
		
		case FlowB:
		counterB++;
		break;
	}
	
	//transitions
	switch(flow_stateB)
	{
		case INITB:
		flow_stateB = WaitB;
		break;
		
		case FlowB:
		flow_stateB = ReleaseB;
		break;
		
		case ReleaseB:
		if(!(PINB & 0x01))
		{
			flow_stateB = WaitB;
		}
		break;
		
		
		case WaitB:
		if((PINB & 0x01))
		{
			flow_stateB = FlowB;
		}
		break;
	}
}

void CheckLineStatuses ()
{
	if (USART_HasReceived(0))
	{
		RecieveVar = USART_Receive(0);
		
		if (RecieveVar & 0xF0)
		{
			Line1Status = 1;
		}
		else
		{
			Line1Status = 0;
		}
		
		if (RecieveVar & 0x0F)
		{
			Line2Status = 1;
		}
		else
		{
			Line2Status = 0;
		}
	}
}
void UpdateLines ()
{
	if (Line1Status)
	{
		PORTB = PORTB | 0x02;
	}
	else
	{
		PORTB = PORTB & 0xFD; 
	}
	
	if (Line2Status)
	{
		PORTA = PORTA | 0x02;
	}
	else
	{
		PORTA = PORTA & 0xFD;
	}
}

void SendFlow ()
{
	if(counterB >= FlowUpdateRate )
	{
		counterB = 0;
		TrasmittVar = TrasmittVar | 0xF0;
	}
	else
	{
		TrasmittVar = TrasmittVar & 0x0F;
	}
	
	if(counterA >= FlowUpdateRate )
	{
		counterA = 0;
		TrasmittVar = TrasmittVar | 0x0F;
	}
	else
	{
		TrasmittVar = TrasmittVar & 0xF0;
	}
	
	USART_Flush(0);
	while (!USART_IsSendReady(0));
	USART_Send(TrasmittVar,0);
	//Wait for USART to send Data
	while (!USART_HasTransmitted(0));
	
	
}

int main(void)
{
	DDRA = 0xFE; PORTA = 0xFF;
	DDRB = 0xFE; PORTB = 0xFF;
	
	initUSART(0);
	
	while(1)
	{
		CheckLineStatuses ();	
		UpdateLines();	
		
		FlowMeterA();
		FlowMeterB();
		
		SendFlow();


	}	
	
	
	return 0;
}