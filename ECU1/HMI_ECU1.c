/*
 *  HMI_ECU1.c
 *  Created on: Nov 1, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			ECU1 main.c file
 *  			Sync messages between the 2 ECUs are used a lot making overhead
 *  			but to make sure connection is safe and synchronized
 */
/*=================================================INCLUDES======================================================*/
#include <util/delay.h>
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timers.h"
/*================================================DEFINITIONS====================================================*/
#define PASSWORD_SIZE 6							/*Password array size*/
#define End_Point 	  5							/*Location of the last element in password array*/
#define Compare_Value_For_15_Seconds	46875	/*Compare Value given to the timer to count 15 seconds*/
#define Compare_Value_For_3_Seconds		9375	/*Compare Value given to the timer to count 3 seconds*/
/*Compare Value given to the timer to count 20 seconds for 3 times counts to get 60 seconds*/
#define Compare_Value_For_60_Seconds	62500
/*==============================================GLOBAL VARIABLES=================================================*/
/*UART Initialization Configuration*/
UART_USART_Configuration_Struct UART_USART_Configuration = {Asynchronous_Operation,B250000,\
EightBit,OneBit,Parity_Disabled};
/*---------------------------------------------------------------------------------------------------------------*/
/*Timer1 Initialization Configuration*/
TIMER_CONFIG_STRUCT TIMER1_Config = {0,0,0,Compare_Value_For_15_Seconds,F_CPU_N256,COMPARE_MODE};
/*---------------------------------------------------------------------------------------------------------------*/
unsigned char Password [PASSWORD_SIZE];			/*Array of the password*/
unsigned char Password_check [PASSWORD_SIZE];	/*Array of the password second check*/
unsigned char password_match = 0;				/*variable to indicate to password match*/
unsigned char Main_Menu_Choice = 0; 			/*Variable to choose option in main menu*/
unsigned char Timer1_ON = 0;					/*Flag to state Timer1 is ON or OFF*/
unsigned char g_timer_counts = 0; 				/*timer counter for the call back function Timer1 Control*/
unsigned char Wrong_Entries_Number = 0;			/*Variable to count number of wrong passwords entered*/
/*============================================FUNCTIONS DEFINITIONS==============================================*/
/* Description:
 * 					call back function to control Timer1, if timer get 10 counts and fully count the
 * 					needed time it turns off the timer and reset all used global variables
 * [No Inputs]:
 * [No Returns]:
 */
void Timer1_Control(void)
{
	g_timer_counts++;
	if(g_timer_counts == 10)
	{
		TIMER1_Deinit();
		g_timer_counts = 0;
		Timer1_ON = 0;
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Initialization Function to initialize any device
 * [No Inputs]:
 * [No Returns]:
 */
void HMIECU1_init(void)
{
	sei(); /*Enable Global Interrupt*/
	UART_init(&UART_USART_Configuration);
	LCD_init();
	TIMER1_setCallBackFunctionCOMP(Timer1_Control);	/*Set Timer1 (Compare mode) Call back function*/
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to print hello message to the user
 * [No Inputs]:
 * [No Returns]:
 */
void Hello_Message(void)
{
	LCD_displayString("Door Locker");
	LCD_displayStringRowColumn(1,0,"Security System");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayString(" ** Welcome ** ");
	_delay_ms(1000);
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to get new password from the user
 * [No Inputs]:
 * [No Returns]:
 */
void Enter_New_Password(void)
{
	unsigned char enter = 0;
	LCD_clearScreen();
	LCD_displayString("plz enter a new");
	LCD_moveCursor(1,0);
	LCD_displayString("password: ");
	for(unsigned char i=0 ; i<PASSWORD_SIZE-1 ; i++)
	{
		Password[i] = KEYPAD_getPressedKey();
		/*to prevent entering any other character except the digits*/
		if(Password[i] == '%' || Password[i] == '*' || Password[i] == '-' || Password[i] == '=' || Password[i] == '+' || Password[i] == 13)
		{
			i--;
			continue;
		}
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	do	/*Enter Button do while loop*/
	{
		enter = KEYPAD_getPressedKey();
		_delay_ms(500);
	}while(enter != 13);
	enter = 0;
	Password[End_Point] = '#';	/*end the password array with # to identify it as registered password*/
	LCD_clearScreen();
	LCD_displayString("plz re-enter the");
	LCD_moveCursor(1,0);
	LCD_displayString("same pass: ");
	for(unsigned char i=0 ; i<PASSWORD_SIZE-1 ; i++)
	{
		Password_check[i] = KEYPAD_getPressedKey();
		/*to prevent entering any other character except the digits*/
		if(Password[i] == '%' || Password[i] == '*' || Password[i] == '-' || Password[i] == '=' || Password[i] == '+' || Password[i] == 13)
		{
			i--;
			continue;
		}
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	do	/*Enter Button do while loop*/
	{
		enter = KEYPAD_getPressedKey();
		_delay_ms(500);
	}while(enter != 13);
	Password_check[End_Point] = '#';	/*end the password array with # to identify it as registered password*/
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to send new password to ECU2
 * [No Inputs]:
 * [No Returns]:
 */
void Send_New_Password(void)
{
	UART_sendByte(UART_SEND_OR_RECEIVE_READY);
	for(unsigned char i=0 ; i<PASSWORD_SIZE ; i++)
	{
		while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};
		UART_sendByte(Password[i]);
	}
	for(unsigned char i=0 ; i<PASSWORD_SIZE ; i++)
	{
		while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};
		UART_sendByte(Password_check[i]);
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to display main menu options to the user
 * [No Inputs]:
 * [No Returns]:
 */
void Main_Menu(void)
{
	LCD_clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_moveCursor(1,0);
	LCD_displayString("- : Change Pass");
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to get the registered password from the user
 * [No Inputs]:
 * [No Returns]:
 */
void Enter_Password(void)
{
	unsigned char enter = 0;
	LCD_clearScreen();
	LCD_displayString("plz enter the");
	LCD_moveCursor(1,0);
	LCD_displayString("password: ");
	for(unsigned char i=0 ; i<PASSWORD_SIZE-1 ; i++)
	{
		Password[i] = KEYPAD_getPressedKey();
		/*to prevent entering any other character except the digits*/
		if(Password[i] == '%' || Password[i] == '*' || Password[i] == '-' || Password[i] == '=' || Password[i] == '+' || Password[i] == 13)
		{
			i--;
			continue;
		}
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	do	/*Enter Button do while loop*/
	{
		enter = KEYPAD_getPressedKey();
		_delay_ms(500);
	}while(enter != 13);
	Password[End_Point] = '#';	/*end the password array with # to identify it as registered password*/
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to send the registered password to ECU2
 * [No Inputs]:
 * [No Returns]:
 */
void Send_Password(void)
{
	UART_sendByte(UART_SEND_OR_RECEIVE_READY);
	for(unsigned char i=0 ; i<PASSWORD_SIZE ; i++)
	{
		while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};
		UART_sendByte(Password[i]);
	}
}
/*===================================================MAIN========================================================*/
int main(void)
{
	HMIECU1_init();								/*Initialization Function*/
	Hello_Message();							/*Show hello message on the LCD*/
	/*---------------------------------------------------------------------------------------------------*/
	UART_sendByte(UART_SEND_OR_RECEIVE_READY);	/*Polling until ECU2 is Synchronized*/
	Password [End_Point] = UART_receiveByte();	/*Get password last element*/
	/*---------------------------------------------------------------------------------------------------*/
	if(Password [End_Point] != '#')		/*if last element is not # means no password is registered before*/
	{
		while(password_match != 1)
		{
			Enter_New_Password();	/*Ask the user to enter the new password*/
			Send_New_Password();		/*Send the new password to ECU2*/
			while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU2 is Synchronized*/
			/*Beware at this point*/
			UART_sendByte(UART_SEND_OR_RECEIVE_READY); /*Send that this ECU is Synchronized*/
			password_match = UART_receiveByte(); /*Receive password matching*/
			if(password_match != 1)
			{
				LCD_clearScreen();
				LCD_displayString("password entered");
				LCD_moveCursor(1,0);
				LCD_displayString("isn't matched");
				_delay_ms(1000);
			}
			else
			{
				LCD_clearScreen();
				LCD_displayString("password entered");
				LCD_moveCursor(1,0);
				LCD_displayString("successfully");
				_delay_ms(1000);
			}
		}
		password_match = 0; /*End Matching Case*/
	}
	/*---------------------------------------------------------------------------------------------------*/
	while(1)
	{
		Main_Menu();										/*Display Main Menu*/
		Main_Menu_Choice = KEYPAD_getPressedKey();			/*get Main Menu choice option*/
		UART_sendByte(UART_SEND_OR_RECEIVE_READY);			/*Send that this ECU is Synchronized*/
		while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};	/*Polling until ECU2 is Synchronized*/
		UART_sendByte(Main_Menu_Choice);
		if(Main_Menu_Choice == '+')
		{
			while(1)
			{
				Enter_Password();
				Send_Password();
				while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU2 is Synchronized*/
				/*Beware at this point*/
				UART_sendByte(UART_SEND_OR_RECEIVE_READY); /*Send that this ECU is Synchronized*/
				password_match = UART_receiveByte(); /*Receive password matching*/
				if(password_match == 1)
				{
					UART_sendByte(UART_SEND_OR_RECEIVE_READY);	/*Send that this ECU is Synchronized*/
					while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU2 is Synchronized*/
					/*-------------------------------------------------------------------------*/
					TIMER1_Config.Compare_Value16 = Compare_Value_For_15_Seconds;
					TIMER1_init(&TIMER1_Config);
					Timer1_ON = 1;
					LCD_clearScreen();
					LCD_displayString("Door is");
					LCD_moveCursor(1,0);
					LCD_displayString("Unlocking ...");
					while(Timer1_ON == 1){};	/*Polling Until Timer1 is OFF*/
					/*-------------------------------------------------------------------------*/
					TIMER1_Config.Compare_Value16 = Compare_Value_For_3_Seconds;
					TIMER1_init(&TIMER1_Config);
					Timer1_ON = 1;
					LCD_clearScreen();
					LCD_displayString("Door is Opened");
					while(Timer1_ON == 1){};
					/*-------------------------------------------------------------------------*/
					TIMER1_Config.Compare_Value16 = Compare_Value_For_15_Seconds;
					TIMER1_init(&TIMER1_Config);
					Timer1_ON = 1;
					LCD_clearScreen();
					LCD_displayString("Door is");
					LCD_moveCursor(1,0);
					LCD_displayString("Locking ...");
					while(Timer1_ON == 1){};
					/*-------------------------------------------------------------------------*/
					Wrong_Entries_Number = 0;	/*Reset number of wrong entries*/
					password_match = 0;			/*End Matching Case*/
					break; /*Exit the While Loop and ask again for options*/
				}
				else if (password_match == 0)
				{
					Wrong_Entries_Number++;
					LCD_clearScreen();
					LCD_displayString("Password Entered");
					LCD_moveCursor(1,0);
					LCD_displayString("is Wrong");
					_delay_ms(1000);
					if(Wrong_Entries_Number == 3)
					{
						UART_sendByte(UART_SEND_OR_RECEIVE_READY);	/*Send that this ECU is Synchronized*/
						while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU2 is Synchronized*/
						TIMER1_Config.Compare_Value16 = Compare_Value_For_60_Seconds;
						/*3 times - 20secondsx3 times = 60 seconds*/
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						LCD_clearScreen();
						LCD_displayString("     ERROR     ");
						LCD_moveCursor(1,0);
						LCD_displayString("     ALARM     ");
						while(Timer1_ON == 1){};
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						while(Timer1_ON == 1){};
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						while(Timer1_ON == 1){};
						/*-------------------------------------------------------------------------*/
						Wrong_Entries_Number = 0;	/*Reset number of wrong entries*/
						break; /*Exit the While Loop and ask again for options*/
					}
				}
			}
			Main_Menu_Choice = 0;	/*Reset main menu choice*/
		}
		else if (Main_Menu_Choice == '-')
		{
			while(1)
			{
				Enter_Password();
				Send_Password();
				while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU2 is Synchronized*/
				/*Beware at this point*/
				UART_sendByte(UART_SEND_OR_RECEIVE_READY); /*Send that this ECU is Synchronized*/
				password_match = UART_receiveByte(); /*Receive password matching*/
				if(password_match == 1)	/*If matched*/
				{
					password_match = 0;	/*End Matching Case*/
					while(password_match != 1)
					{
						Enter_New_Password();	/*Ask the user to enter the new password*/
						Send_New_Password();		/*Send the new password to ECU2*/
						while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU2 is Synchronized*/
						/*Beware at this point*/
						UART_sendByte(UART_SEND_OR_RECEIVE_READY); /*Send that this ECU is Synchronized*/
						password_match = UART_receiveByte(); /*Receive password matching*/
						if(password_match != 1)
						{
							LCD_clearScreen();
							LCD_displayString("password entered");
							LCD_moveCursor(1,0);
							LCD_displayString("isn't matched");
							_delay_ms(1000);
						}
						else
						{
							LCD_clearScreen();
							LCD_displayString("password entered");
							LCD_moveCursor(1,0);
							LCD_displayString("successfully");
							_delay_ms(1000);
						}
					}
					Wrong_Entries_Number = 0;	/*Reset number of wrong entries*/
					password_match = 0;			/*End Matching Case*/
					break; /*Exit the While Loop and ask again for options*/
				}
				else if (password_match == 0)
				{
					Wrong_Entries_Number++;
					LCD_clearScreen();
					LCD_displayString("Password Entered");
					LCD_moveCursor(1,0);
					LCD_displayString("is Wrong");
					_delay_ms(1000);
					if(Wrong_Entries_Number == 3)
					{
						UART_sendByte(UART_SEND_OR_RECEIVE_READY);	/*Send that this ECU is Synchronized*/
						while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU2 is Synchronized*/
						TIMER1_Config.Compare_Value16 = Compare_Value_For_60_Seconds;
						/*3 times - 20secondsx3 times = 60 seconds*/
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						LCD_clearScreen();
						LCD_displayString("     ERROR     ");
						LCD_moveCursor(1,0);
						LCD_displayString("     ALARM     ");
						while(Timer1_ON == 1){};
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						while(Timer1_ON == 1){};
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						while(Timer1_ON == 1){};
						/*-------------------------------------------------------------------------*/
						Wrong_Entries_Number = 0;	/*Reset number of wrong entries*/
						break; /*Exit the While Loop and ask again for options*/
					}
				}
			}
			Main_Menu_Choice = 0;	/*Reset main menu choice*/
		}
	}
}
