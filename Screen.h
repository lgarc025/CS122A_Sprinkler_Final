#ifndef SCREEN_H_
#define SCREEN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/delay.h>
#include <string.h>

//#include "timer.h"
//#include "task.h"
#include "usart_ATmega1284.h"
#include "HX8357_1284.h"
#include "ControlSprinkler.h"

typedef struct _system {
	
	int Temp;
	int Humid;
	unsigned char Hours;
	unsigned char Minutes;
	bool Line_1_Status;
	bool Line_2_Status;
	bool Line_1_Run_Status;
	bool Line_2_Run_Status;
	unsigned char L1_Start_Minutes;
	unsigned char L1_Start_Hours;
	unsigned char L2_Start_Minutes;
	unsigned char L2_Start_Hours;
	int Line_1_RunTime;
	int Line_2_RunTime;
	
	sprinkler *SP1;
	sprinkler *SP2;
	
} system;


#endif 

void initSystem (system *s, int T, int H, unsigned char Hr, unsigned Min, int L1Run, int L2Run, bool s1, bool s2)
{
	s->Line_1_Status = false;
	s->Line_1_Status = true;
	s->Temp = T;
	s->Humid = H;
	s->Hours = Hr;
	s->Minutes = Min;
	s->Line_1_RunTime = L1Run;
	s->Line_2_RunTime = L2Run;
	s->L1_Start_Hours = 7;
	s->L1_Start_Minutes = 0;
	s->L2_Start_Hours = 8;
	s->L2_Start_Minutes = 30;
	s->SP1 = malloc (sizeof(sprinkler));
	s->SP2 = malloc (sizeof(sprinkler));
	s->Line_1_Run_Status = s1;
	s->Line_2_Run_Status = s2;
	s->SP1->LineNumber = 1;
	s->SP2->LineNumber = 2;
	
	initSprinkler(s->SP1,1,false,false,0,L1Run,s->L1_Start_Minutes,s->L1_Start_Hours);
	initSprinkler(s->SP2,2,false,false,0,L2Run,s->L2_Start_Minutes,s->L2_Start_Hours);
	
	
}

// System Functions
void PrintTime (system *s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Hours);
	strcpy(StringToPrint, "Current Time:");
	strcat(StringToPrint, str);
	strcat(StringToPrint, ":");
	if( s->Minutes < 10)
	{
		strcat(StringToPrint, "0");
		
	}
	sprintf(str,"%d", s->Minutes);
	strcat(StringToPrint, str);
	drawString(1,0, StringToPrint ,HX8357_GREEN, 2);
}

void PrintTemp (system *s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Temp);
	strcpy(StringToPrint, "Current Temp:");
	strcat(StringToPrint, str);
	strcat(StringToPrint, " F");
	drawString(1,25, StringToPrint ,HX8357_GREEN, 2);
}

void PrintHumid (system *s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Humid);
	strcpy(StringToPrint, "Current Humidity:");
	strcat(StringToPrint, str);
	strcat(StringToPrint, "%");
	drawString(1,50, StringToPrint ,HX8357_GREEN, 2);
}
//Delete Functions
void DeleteTime (system *s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Hours);
	strcpy(StringToPrint, "Current Time:");
	strcat(StringToPrint, str);
	strcat(StringToPrint, ":");
	if( s->Minutes < 10)
	{
		strcat(StringToPrint, "0");
		
	}
	sprintf(str,"%d", s->Minutes);
	strcat(StringToPrint, str);
	drawString(1,0, StringToPrint ,HX8357_BLACK, 2);
}

void DeleteTemp (system *s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Temp);
	strcpy(StringToPrint, "Current Temp:");
	strcat(StringToPrint, str);
	strcat(StringToPrint, " F");
	drawString(1,25, StringToPrint ,HX8357_BLACK, 2);
}

void DeleteHumid (system *s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Humid);
	strcpy(StringToPrint, "Current Humidity:");
	strcat(StringToPrint, str);
	strcat(StringToPrint, "%");
	drawString(1,50, StringToPrint ,HX8357_BLACK, 2);
}

//Line1 Functions
void PrintLineStart1(system * s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->L1_Start_Hours);
	strcpy(StringToPrint, "Line 1: Start @ ");
	strcat(StringToPrint, str);
	strcat(StringToPrint, ":");
	if( s->L1_Start_Minutes < 10)
	{
		strcat(StringToPrint, "0");
		
	}
	sprintf(str,"%d", s->L1_Start_Minutes);
	strcat(StringToPrint, str);
	drawString(1,100, StringToPrint ,HX8357_GREEN, 2);
	
}

void PrintLine1Run (system * s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Line_1_RunTime);
	strcpy(StringToPrint, "Line 1: Runs ");
	strcat(StringToPrint, str);
	strcat(StringToPrint, " Min");
	drawString(1,125, StringToPrint ,HX8357_GREEN, 2);
	
}

void PrintLineStatus1 (system *s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Humid);
	if(s->Line_1_Status == 1)
	{
		strcpy(StringToPrint, "Line 1: No Errors!");
	}
	else
	{
		strcpy(StringToPrint, "Line 1: Check Flow!");
		
	}
	drawString(1,150, StringToPrint ,HX8357_GREEN, 2);
}

//Delete Functions
void DeleteLineStart1(system * s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->L1_Start_Hours);
	strcpy(StringToPrint, "Line 1: Start @ ");
	strcat(StringToPrint, str);
	strcat(StringToPrint, ":");
	if( s->L1_Start_Minutes < 10)
	{
		strcat(StringToPrint, "0");
		
	}
	sprintf(str,"%d", s->L1_Start_Minutes);
	strcat(StringToPrint, str);
	drawString(1,100, StringToPrint ,HX8357_BLACK, 2);
	
}

void DeleteLine1Run (system * s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Line_1_RunTime);
	strcpy(StringToPrint, "Line 1: Runs ");
	strcat(StringToPrint, str);
	strcat(StringToPrint, " Min");
	drawString(1,125, StringToPrint ,HX8357_BLACK, 2);
	
}

void DeleteLineStatus1 (system *s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Humid);
	if(s->Line_1_Status == 1)
	{
		strcpy(StringToPrint, "Line 1: No Errors!");
	}
	else
	{
		strcpy(StringToPrint, "Line 1: Check Flow!");
		
	}
	drawString(1,150, StringToPrint ,HX8357_BLACK, 2);
}

//Line2 Functions
void PrintLineStart2(system * s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->L2_Start_Hours);
	strcpy(StringToPrint, "Line 2: Start @ ");
	strcat(StringToPrint, str);
	strcat(StringToPrint, ":");
	if( s->L2_Start_Minutes < 10)
	{
		strcat(StringToPrint, "0");
		
	}
	sprintf(str,"%d", s->L2_Start_Minutes);
	strcat(StringToPrint, str);
	drawString(1,200, StringToPrint ,HX8357_GREEN, 2);
	
}

void PrintLine2Run (system * s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Line_2_RunTime);
	strcpy(StringToPrint, "Line 2: Runs ");
	strcat(StringToPrint, str);
	strcat(StringToPrint, " Min");
	drawString(1,225, StringToPrint ,HX8357_GREEN, 2);
	
}

void PrintLineStatus2 (system *s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Humid);
	if(s->Line_2_Status == 1)
	{
		strcpy(StringToPrint, "Line 2: No Errors!");
	}
	else
	{
		strcpy(StringToPrint, "Line 2: Check Flow!");
		
	}
	drawString(1,250, StringToPrint ,HX8357_GREEN, 2);
}
//Delete Functions
void DeleteLineStart2(system * s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->L2_Start_Hours);
	strcpy(StringToPrint, "Line 2: Start @ ");
	strcat(StringToPrint, str);
	strcat(StringToPrint, ":");
	if( s->L2_Start_Minutes < 10)
	{
		strcat(StringToPrint, "0");
		
	}
	sprintf(str,"%d", s->L2_Start_Minutes);
	strcat(StringToPrint, str);
	drawString(1,200, StringToPrint ,HX8357_BLACK, 2);
	
}

void DeleteLine2Run (system * s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Line_2_RunTime);
	strcpy(StringToPrint, "Line 2: Runs ");
	strcat(StringToPrint, str);
	strcat(StringToPrint, " Min");
	drawString(1,225, StringToPrint ,HX8357_BLACK, 2);
	
}

void DeleteLineStatus2 (system *s)
{
	char str[64];
	char StringToPrint[256];
	sprintf(str,"%d", s->Humid);
	if(s->Line_2_Status == 1)
	{
		strcpy(StringToPrint, "Line 2: No Errors!");
	}
	else
	{
		strcpy(StringToPrint, "Line 2: Check Flow!");
		
	}
	drawString(1,250, StringToPrint ,HX8357_BLACK, 2);
}

//Menu Functions
void PrintMessages (system*s)
{
	char StringToPrint[256];
	strcpy(StringToPrint, "Messages ");
	drawString(110,300, StringToPrint ,HX8357_GREEN, 2);
}

void PrintCurrentlyRunning (system * s)
{
	if(s->Line_1_Run_Status == true)
	{
		char str[64];
		char StringToPrint[256];
		//Total - has Pass
		sprintf(str,"%d", (s->SP1->RunLength - s->SP1->ElapsedTime));
		strcpy(StringToPrint, "Line 1: ");
		strcat(StringToPrint, str);
		strcat(StringToPrint, " Min Remaining");
		drawString(1,325, StringToPrint ,HX8357_GREEN, 2);
	}
	
	if(s->Line_2_Run_Status == true)
	{
		char str[64];
		char StringToPrint[256];
		//Total - has Pass
		sprintf(str,"%d", (s->SP2->RunLength - s->SP2->ElapsedTime));
		strcpy(StringToPrint, "Line 2: ");
		strcat(StringToPrint, str);
		strcat(StringToPrint, " Min Remaining");
		drawString(1,350, StringToPrint ,HX8357_GREEN, 2);
	}
	
}

void DeleteCurrentlyRunning (system * s)
{
	if(s->Line_1_Run_Status == false)
	{
		char str[64];
		char StringToPrint[256];
		//Total - has Pass
		sprintf(str,"%d", (s->SP1->RunLength - s->SP1->ElapsedTime));
		strcpy(StringToPrint, "Line 1: ");
		strcat(StringToPrint, str);
		strcat(StringToPrint, " Min Remaining");
		drawString(1,325, StringToPrint ,HX8357_BLACK, 2);
	}
	
	if(s->Line_2_Run_Status == false)
	{
		char str[64];
		char StringToPrint[256];
		//Total - has Pass
		sprintf(str,"%d", (s->SP2->RunLength - s->SP2->ElapsedTime));
		strcpy(StringToPrint, "Line 2: ");
		strcat(StringToPrint, str);
		strcat(StringToPrint, " Min Remaining");
		drawString(1,350, StringToPrint ,HX8357_BLACK, 2);
	}
	
}

void initMenu (system *s)
{
	fillScreen(HX8357_BLACK);
	PrintTime(s);
	PrintTemp(s);
	PrintHumid(s);
	
	PrintLineStart1(s);
	PrintLine1Run(s);
	PrintLineStatus1(s); 
	
	PrintLineStart2(s);
	PrintLine2Run(s);
	PrintLineStatus2(s);
	
	PrintMessages(s);
	PrintCurrentlyRunning(s);
	
}

//Change Functions
void increaseTime (system *s)
{
	DeleteTime(s);
	if(s->Minutes == 59)
	{
		s->Minutes = 0;
		if(s->Hours == 23)
		{
			s->Hours = 0;
		}
		else
		{
			s->Hours++;
		}		
	}
	else
	{
		s->Minutes++;
	}
	
	PrintTime(s);
	
	
}

int ChangeTime (system *s, int H, int M)
{
	DeleteTime(s);
	int i = 0;
	if(M <= 59 && H <= 23)
	{
		s->Minutes = M;
		s->Hours = H;
		i = 1;
	}	
	
	PrintTime(s);
	
	return i;
	
}

void ChangeHumidity (system *s, int NewHum)
{
	DeleteHumid(s);
	s->Humid = NewHum;
	PrintHumid(s);
}

void ChangeTemp (system *s, int NewTemp)
{
	DeleteTemp(s);
	s->Temp = NewTemp;
	PrintTemp(s);
}

int ChangeLine1Run ( system *s, unsigned char NewTime)
{
	int i = 0;
	if(NewTime < 20)
	{
		DeleteLine1Run(s);
		s->Line_1_RunTime = NewTime;
		PrintLine1Run(s);
		i = 1;
	}	
	
	return i;	
}

int ChangeLine2Run ( system *s, unsigned char NewTime)
{
	int i = 0;
	if(NewTime < 20)
	{
		DeleteLine2Run(s);
		s->Line_2_RunTime = NewTime;
		PrintLine2Run(s);
		i = 1;
	}
	
	return i;
}

void ChangeLine1Status ( system *s, bool NewSat)
{
	if(s->Line_1_Status != NewSat)
	{
		DeleteLineStatus1(s);
		s->Line_1_Status = NewSat;
		PrintLineStatus1(s);
	}		
}

void ChangeLine2Status ( system *s, bool NewSat)
{
	if(s->Line_2_Status != NewSat)
	{
		
		DeleteLineStatus2(s);
		s->Line_2_Status = NewSat;
		PrintLineStatus2(s);
	}		
}

int ChangeLine1Start (system *s, unsigned char NewHour, unsigned char NewMin)
{
	int i = 0;
	if (NewHour <= 23 && NewMin <= 59)
	{
		DeleteLineStart1(s);
		s->L1_Start_Hours = NewHour;
		s->L1_Start_Minutes = NewMin;
		PrintLineStart1(s);
		i = 1;
	}
	
	return i;
}

int ChangeLine2Start (system *s, unsigned char NewHour, unsigned char NewMin)
{
	int i = 0;
	if (NewHour <= 23 && NewMin <= 59)
	{
		DeleteLineStart2(s);
		s->L2_Start_Hours = NewHour;
		s->L2_Start_Minutes = NewMin;
		PrintLineStart2(s);
		i = 1;
	}
	return i;
}

void UpdateSprinklerLineValues(system *s)
{
		s->SP1->RunLength = s->Line_1_RunTime;
		s->SP1->StartMinutes = s->L1_Start_Minutes;
		s->SP1->StartHours = s->L1_Start_Hours;
		s->SP1->RunStatus = s->Line_1_Run_Status;
		s->SP1->ErrorStatus =  s->Line_1_Status;
		
		s->SP2->RunLength = s->Line_2_RunTime;
		s->SP2->StartMinutes = s->L2_Start_Minutes;
		s->SP2->StartHours = s->L2_Start_Hours;
		s->SP2->RunStatus = s->Line_2_Run_Status;
		s->SP2->ErrorStatus =  s->Line_2_Status;
}