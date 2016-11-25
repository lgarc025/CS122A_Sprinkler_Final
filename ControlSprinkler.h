/*
 * ControlSprinkler.h
 *
 * Created: 11/24/2016 6:43:44 PM
 *  Author: Luis
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/delay.h>
#include <string.h>
#include "usart_ATmega1284.h""

#ifndef CONTROLSPRINKLER_H_
#define CONTROLSPRINKLER_H_


unsigned char PreviousTransmitVar = 0x00;

typedef struct _sprinkler {
	
	int LineNumber;
	bool RunStatus;
	bool ErrorStatus;
	int ElapsedTime;
	int RunLength;
	unsigned char StartMinutes;
	unsigned char StartHours;
	
} sprinkler;

void initSprinkler (sprinkler *s, int LN, bool R_S, bool ES, int ET, int RL, unsigned char SM, unsigned char SH  )
{
	s->LineNumber = LN;
	s->RunStatus = R_S;
	s->ErrorStatus = ES;
	s->ElapsedTime = ET;
	s->RunLength = RL;
	s->StartMinutes = SM;
	s->StartHours = SH;
}

void UpdateSprinklers (sprinkler *s1, bool status1, sprinkler *s2, bool status2  )
{
	unsigned char TransmitVar = 0x00;
	//PORTA = 0xF0;
	
	//Lin1
	if( status1 == true && (s1->LineNumber == 1) )
	{
		s1->RunStatus = true;
		TransmitVar = TransmitVar | 0xF0;
		
	}
	else if ( status1 == false && (s1->LineNumber == 1) )
	{
		s1->RunStatus = false;
		TransmitVar = TransmitVar & 0x0F;
		
	} 
	
	//Line2
	if( status2 == true && (s2->LineNumber == 2) )
	{
		s1->RunStatus = true;
		TransmitVar = TransmitVar | 0x0F;
			
	}
	else if ( status2 == false && (s2->LineNumber == 2) )
	{
		s2->RunStatus = false;
		TransmitVar = TransmitVar & 0xF0;
		
	}
	
	if(PreviousTransmitVar != TransmitVar)
	{
		//Send The Transmit Variable
		USART_Send(TransmitVar,1);
		while (!USART_HasTransmitted(1));
		PreviousTransmitVar = TransmitVar;
	}
	
}






#endif /* CONTROLSPRINKLER_H_ */