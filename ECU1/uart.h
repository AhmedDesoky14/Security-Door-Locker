/*
 *  uart.h
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
#ifndef UART_H_
#define UART_H_
/*==================================INCLUDES=============================================*/
#include <avr/io.h> /*Registers are not updated in in the atmega32_registers.h file*/
#include "std_types.h"
#include <avr/interrupt.h> /*For Receiving Interrupt*/
#include "common_macros.h"
/*==============================Configuration Note=======================================*/
/*	to re-configure this driver go to line 58 in this .h file
 *  and go to line 63 in the .c file
 */
/*==============================ENUMS & STRUCTURES=======================================*/
typedef enum
{
	/*BAUD Rate Selection*/
	B2400=2400,B4800=4800,B9600=9600,B14400=14400,B19200=19200,B28800=28800,B38400=38400,
	B57600=57600,B76800=76800,B115200=115200,B230400=230400,B250000=250000,B500000=500000,
	B1000000=1000000
}UART_BAUDRATE_SELECTIONS;
/*---------------------------------------------------------------------------------------*/
typedef enum
{
	/*Synchronization Selection*/
	Asynchronous_Operation,Synchronous_Operation
}USART_SYNCHRONIZATION_SELECTION;
/*---------------------------------------------------------------------------------------*/
typedef enum
{
	Parity_Disabled,Even_Parity=2,Odd_Parity=3
}UART_PARITY_MODE_SELECTION;
/*---------------------------------------------------------------------------------------*/
typedef enum
{
	OneBit,TwoBit
}UART_STOP_BIT_NUMBER_SELECTION;
/*---------------------------------------------------------------------------------------*/
typedef enum
{
	FiveBit,SixBit,SevenBit,EightBit,NineBit=7
}UART_CHARACTER_SIZE_SELECTION;
/*---------------------------------------------------------------------------------------*/
typedef struct
{
	USART_SYNCHRONIZATION_SELECTION 			USART_SYNCHRONIZATION;
	UART_BAUDRATE_SELECTIONS 					UART_BAUDRATE;
	UART_CHARACTER_SIZE_SELECTION				UART_CHARACTER_SIZE;
	UART_STOP_BIT_NUMBER_SELECTION				UART_STOP_BIT_NUMBER; /*Only important for Asynchronous Operations*/
	UART_PARITY_MODE_SELECTION					UART_PARITY_MODE;	  /*Only important for Asynchronous Operations*/
}UART_USART_Configuration_Struct;
/*===========================DEFINITIONS & CONFIGURATIONS================================*/
#define UART_SEND_OR_RECEIVE_READY 0x10 /*pre-agreement value to be sent and received*/
/*=================================FUNCTIONS DECLARATIONS================================*/
/*Functions Description in .c file*/
void UART_init(UART_USART_Configuration_Struct *UART_USART_Configuration_Ptr);
uint8 UART_receiveByte(void);
void UART_sendByte(uint8 data);
void UART_receiveString(char* string);
void UART_sendString(const char* string);
#endif /* UART_H_ */
