/*
 * ReceiveString.h
 *
 * Created: 11/23/2016 5:15:13 PM
 *  Author: Luis
 */ 


#ifndef RECEIVESTRING_H_
#define RECEIVESTRING_H_

char Recieve_String [256];

void SendString (char * temp)
{
	char string [256];
	strcpy(string, temp);
	for (int i = 0; string[i] != '\0'; i++)
	{
		USART_Send(string[i],0);
		while (!USART_HasTransmitted(0));
	}
	USART_Send('\n',0);
}

int RecieveString ()
{
	int i = 0;
	if (USART_HasReceived(0))
	{
		unsigned temp = USART_Receive(0);
		if(temp != '\n')
		{
			while (temp != '\n')
			{
				//PORTA = temp;
				Recieve_String [i] = temp;
				i++;
				temp = USART_Receive(0);
			}
			Recieve_String [i] = '\0';
		}
	}
	
	USART_Flush(0);
}


#endif /* RECEIVESTRING_H_ */