/*
 *  uart.c
 *  Created on: Oct 15, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			AVR ATmega32/L UART Driver
 *  			#supported to act as USART
 *  			#supports parity error & frame error in receiving
 *  			#Full Configurable
 *  			#Supports only polling (no interrupts)
 *  				because polling is better in sending & receiving in UART despite of time overhead
 */
/*==================================INCLUDES=============================================*/
#include "uart.h"
/*=============================FUNCTIONS DEFINITIONS=====================================*/
/* Description:
 * 					Function to initalize the UART or USART what ever the functionality
 * [Inputs]:		UART Configuration structure by address
 *
 * [No Returns]:
 */
void UART_init(UART_USART_Configuration_Struct *UART_USART_Configuration_Ptr)
{
	uint16 UBRR_value = 0; /*Register to set Baudarte value*/
	SET_BIT(UCSRA,U2X); /*Always the USART is in Double Transmission Speed mode*/
	SET_BIT(UCSRB,RXEN); /*Enable Receiving*/
	SET_BIT(UCSRB,TXEN); /*Enable Transmitting*/
	SET_BIT(UCSRC,URSEL); /*This must be set to 1 when writing to UCSRC*/
	/*Clear Character size selection bits initially*/
	CLEAR_BIT(UCSRB,UCSZ2);
	CLEAR_BIT(UCSRC,UCSZ0);
	CLEAR_BIT(UCSRC,UCSZ1);
	/*UART Character Size coditions*/
	switch(UART_USART_Configuration_Ptr->UART_CHARACTER_SIZE)
	{
		case NineBit:
			/*Set character size = 9 bits*/
			SET_BIT(UCSRB,UCSZ2);
			SET_BIT(UCSRC,UCSZ0);
			SET_BIT(UCSRC,UCSZ1);
			break;
		case EightBit:
			/*Set character size = 8 bits*/
			SET_BIT(UCSRC,UCSZ0);
			SET_BIT(UCSRC,UCSZ1);
			break;
		case SevenBit:
			/*Set character size = 7 bits*/
			SET_BIT(UCSRC,UCSZ1);
			break;
		case SixBit:
			/*Set character size = 6 bits*/
			SET_BIT(UCSRC,UCSZ0);
			break;
		case FiveBit:
			/*Set character size = 5 bit*/
			/*keep them set to 0*/
			break;
	}
	/*UART Synchronization coditions*/
	switch(UART_USART_Configuration_Ptr->USART_SYNCHRONIZATION)
	{
		case Asynchronous_Operation:
			CLEAR_BIT(UCSRC,UMSEL); /*Enable Asynchronous Mode*/
			break;
		case Synchronous_Operation:
			SET_BIT(UCSRC,UMSEL); /*Enable Synchronous Mode*/
			/*if in Synchronous Mode always transmit at rising edge and receive at falling edge*/
			CLEAR_BIT(UCSRC,UCPOL);
			break;
	}
	/*UART Polarity Mode coditions*/
	switch(UART_USART_Configuration_Ptr->UART_PARITY_MODE)
	{
	case Parity_Disabled:
		CLEAR_BIT(UCSRC,UPM0);
		CLEAR_BIT(UCSRC,UPM1);
		break;
	case Even_Parity:
		SET_BIT(UCSRC,UPM0);
		CLEAR_BIT(UCSRC,UPM1);
		break;
	case Odd_Parity:
		SET_BIT(UCSRC,UPM0);
		SET_BIT(UCSRC,UPM1);
		break;
	}
	/*UART stop bit coditions*/
	switch(UART_USART_Configuration_Ptr->UART_STOP_BIT_NUMBER)
	{
		case OneBit:
			CLEAR_BIT(UCSRC,USBS);
			break;
		case TwoBit:
			SET_BIT(UCSRC,USBS);
			break;
	}
	/*F_CPU is already declared quantity*/
	/*System Frequency must be 8 times greater than Baud rate*/
	UBRR_value = (uint16)(((F_CPU)/(UART_USART_Configuration_Ptr->UART_BAUDRATE * 8UL))-1);
	UBRRH = UBRR_value>>8;
	UBRRL = UBRR_value;
}
/*----------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to keep polling until receiving flag is set (received)
 * 					then return the received data
 * 					without using timeout for receiving
 * [No Inputs]:
 *
 * [Returns]:
 * 				Received Data
 *
 */
uint8 UART_receiveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA,RXC)); /*while receiving isn't complete (flag != 1)*/
	/*if there is no frame error or parity error*/
	//if(BIT_IS_CLEAR(UCSRA,PE) && BIT_IS_CLEAR(UCSRA,FE))
		return UDR;
}
/*----------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to keep polling until UDR register is empty
 * 					then send data to the UDR to be transmitted
 * 					then wait until data to be transmitted safely
 * 					then reset the transmitting flag and go out
 * 					to prevent any further overwrite
 * 					despite that this function has time overhead
 * [Inputs]:		the data needed to be transmitted
 *
 * [No Returns]:
 */
void UART_sendByte(uint8 data)
{
	while(BIT_IS_CLEAR(UCSRA,UDRE)); /*while UDR register is not empty*/
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)); /*while transmission isn't complete (flag != 1)*/
	SET_BIT(UCSRA,TXC);	/*clear complete transmission flag*/
}
/*----------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to transmit a string character by character using UART_sendByte function
 * [Inputs]:		pointer to the string wanted to be transmitted
 *
 * [No Returns]:
 */
void UART_sendString(const char* string)
{
	uint8 counter=0;
	/*transmit the string character by character*/
	while(string[counter] != '\0')
	{
		UART_sendByte(string[counter]);
		counter++;
	}
}
/*----------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to receive a string character by character using UART_receiveByte function
 * [Inputs]:		pointer to the string wanted to be received to store it in the address of the pointer
 *
 * [No Returns]:
 */
void UART_receiveString(char* string)
{
	uint8 counter=0;
	string[counter] = UART_receiveByte();
	/*receive the string character by character*/
	/*===========================This loop is Re-configurable=============================*/
	/*'#' is pre-agreement character chosen between the transimitter and the receiver*/
	/*to tell the end of the string*/
	while(string[counter] != '#')
	{
		counter++;
		string[counter] = UART_receiveByte();
	}
	string[counter]='\0';
}
