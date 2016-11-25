#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include "TempHum.h"
#include "Screen.h"
#include "ReceiveString.h"

enum Screen_States {Screen_Start, Screen_Process};
enum Control_States {Control_Start, Control_Process};

//System Tick Variables
uint32_t TickPerSecond = 16000;
uint32_t CurrentTicks = 0;
uint32_t CurrentSecs = 0;

int Control_tick ();
int TimeKeeper ();
int WaterScheduler(system *s);

system MainSys;

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	//DDRC = 0x00; PORTC = 0x00;


	
	SPI_MasterInit();
	displayInit();
	initUSART(0);
	initUSART(1);
		
	sprinkler Sprinkler1;
	//MainSys.SP1 = &Sprinkler1;
	
	initSystem(&MainSys,2,30,6,58,58,10,0,0);
	initMenu(&MainSys);
	
	while(1)
	{
		Control_tick ();	
		UpdateSprinklers(MainSys.SP1, MainSys.Line_1_Run_Status, MainSys.SP2, MainSys.Line_2_Run_Status);
		TimeKeeper();	
	}
	
	
}


int Control_tick ()
{
		RecieveString();
		//PORTA = Recieve_String;	
		if(strlen(Recieve_String) < 2)
		{
			return 0;
		}			
			
		char Test[256];
		strcpy(Test, strtok (Recieve_String,":"));
		if (strcmp(Test, "SetTime") == 0)
		{
			int Hour = atoi(strtok (NULL, ",:"));
			int Min = atoi(strtok (NULL, ",:"));
			if (ChangeTime(&MainSys, Hour, Min))
			{
				SendString("Time Has Been Set!");
			}
			else
			{
				SendString("Error Setting Time!");
			}
			
			strcpy(Recieve_String, " "); 
		}
		else if(strcmp(Test, "SetStartTime") == 0)
		{
			int Line = atoi(strtok (NULL, ",:"));
			int Hour = atoi(strtok (NULL, ",:"));
			int Min = atoi(strtok (NULL, ",:"));
			if(Line == 1)
			{
				if(ChangeLine1Start(&MainSys, Hour, Min))
				{
					SendString("Start Time Has Been Set on Line 1!");
				}
				else
				{
					SendString("Error Setting Start Time!");
				}					
				
			}
			else if (Line == 2)
			{
				if(ChangeLine2Start(&MainSys, Hour, Min))
				{
					SendString("Start Time Has Been Set on Line 2!");
				}
				else
				{
					SendString("Error Setting Start Time!");
				}
			}
			else
			{
				SendString("Error Setting Start Time!");
			}
			
			strcpy(Recieve_String, " ");
		}
		else if(strcmp(Test,"SetRun") == 0)
		{
			//SendString("Here!");
			int Line = atoi(strtok (NULL, ",:"));
			int Min = atoi(strtok (NULL, ",:"));
			
			if(Line == 1)
			{
				if(ChangeLine1Run(&MainSys, Min))
				{
					SendString("Run Time Has Been Set on Line 1!");
				}
				else
				{
					SendString("Error Setting Run Time!");
				}
				
			}
			else if (Line == 2)
			{
				if(ChangeLine2Run(&MainSys, Min))
				{
					SendString("Run TIme Has Been Set on Line 2!");
				}
				else
				{
					SendString("Error Setting Run Time!");
				}
			}
			else
			{
				SendString("Error Setting Run Time!");
			}
			
			strcpy(Recieve_String, " ");
		}
		else if(strcmp(Test, "LineOn") == 0)
		{
			int Line = atoi(strtok (NULL, ",:"));
			if(Line == 1 && MainSys.Line_1_Run_Status == false)
			{
				MainSys.Line_1_Run_Status = true;
				SendString("Line 1 On!");
			}
			else if (Line == 2 && MainSys.Line_2_Run_Status == false)
			{
				MainSys.Line_2_Run_Status = true;
				SendString("Line 2 On!");
			}
			else
			{
				SendString("Error Setting Line On!");
			}
			
			PrintCurrentlyRunning(&MainSys);
	
			strcpy(Recieve_String, " ");
		}	
		else if(strcmp(Test, "LineOff") == 0)
		{
			int Line = atoi(strtok (NULL, ",:"));
			if(Line == 1 && MainSys.Line_1_Run_Status == true)
			{
				MainSys.Line_1_Run_Status = false;
				SendString("Line 1 Off!");
			}
			else if (Line == 2 && MainSys.Line_2_Run_Status == true)
			{
				MainSys.Line_2_Run_Status = false;
				SendString("Line 2 Off!");
			}
			else
			{
				SendString("Error Setting Line Off!");
			}
			
			DeleteCurrentlyRunning(&MainSys);
	
			strcpy(Recieve_String, " ");
		}	
		else
		{
			SendString("Error With Command!");
			strcpy(Recieve_String, " ");
		}
		
		UpdateSprinklerLineValues(&MainSys);		
	
	return 0;
}

int TimeKeeper ()
{
	if(CurrentTicks >= TickPerSecond)
	{
		CurrentTicks = 0;
		CurrentSecs++;
	}
	
	CurrentTicks++;
	
	if(CurrentSecs == 60)
	{
		CurrentSecs = 0;
		PORTA++;
		increaseTime(&MainSys);
		WaterScheduler(&MainSys);
	}
	
}

int WaterScheduler(system *s)
{
	unsigned char CurrentHour = s->Hours;
	unsigned char CurrentMin = s->Minutes;
	
	if(s->Line_1_Run_Status)
	{
		s->Line_1_Run_Status = false;
		DeleteCurrentlyRunning(s);
		s->SP1->ElapsedTime++;
		s->Line_1_Run_Status = true;
		PrintCurrentlyRunning(s);
	}
	else
	{
		s->SP1->ElapsedTime = 0;
	}
	
	if(s->Line_2_Run_Status)
	{
		s->Line_2_Run_Status = false;
		DeleteCurrentlyRunning(s);
		s->SP2->ElapsedTime++;
		s->Line_2_Run_Status = true;
		PrintCurrentlyRunning(s);
	}
	else
	{
		s->SP2->ElapsedTime = 0;
	}
	
	if(CurrentHour == s->SP1->StartHours && CurrentMin == s->SP1->StartMinutes)
	{
		s->Line_1_Run_Status = true;
		SendString("Line 1 On!");
		PrintCurrentlyRunning(s);
	}
	
	if(CurrentHour == s->SP2->StartHours && CurrentMin == s->SP2->StartMinutes)
	{
		s->Line_2_Run_Status = true;
		SendString("Line 2 On!");
		PrintCurrentlyRunning(s);
	}
	
	if((s->SP1->ElapsedTime >= s->SP1->RunLength ) && s->Line_1_Run_Status )
	{
		s->Line_1_Run_Status = false;
		SendString("Line 1 Off!");
		DeleteCurrentlyRunning(s);
	}
		
	if( (s->SP2->ElapsedTime >= s->SP2->RunLength ) && s->Line_2_Run_Status)
	{
		s->Line_2_Run_Status = false;
		SendString("Line 2 Off!");
		DeleteCurrentlyRunning(s);
	}

	
	
}
