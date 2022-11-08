/*
 *  Control_ECU2.c
 *  Created on: Nov 1, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			ECU2 main.c file
 *  			Sync messages between the 2 ECUs are used a lot making overhead
 *  			but to make sure connection is safe and synchronized
 */
/*=================================================INCLUDES======================================================*/
#include "uart.h"
#include "external_eeprom.h"
#include "timers.h"
#include "dcmotor.h"
/*================================================DEFINITIONS====================================================*/
#define PASSWORD_SIZE 6					/*Password array size*/
#define End_Point 	  5 				/*Location of the last element in password array*/
#define PASSWORD_START_ADDRESS 0x500	/*Address of the password in the EEPROM*/
#define TWI_This_Device_Address 0x01	/*This Device Address in TWI Protocol*/
#define TWI_Bit_Rate_KHz 400			/*TWI Bit Rate*/
#define Compare_Value_For_15_Seconds	46875	/*Compare Value given to the timer to count 15 seconds*/
#define Compare_Value_For_3_Seconds		9375	/*Compare Value given to the timer to count 3 seconds*/
/*Compare Value given to the timer to count 20 seconds for 3 times counts to get 60 seconds*/
#define Compare_Value_For_60_Seconds	62500
/*==============================================GLOBAL VARIABLES=================================================*/
/*UART Initialization Configuration*/
UART_USART_Configuration_Struct UART_USART_Configuration = {Asynchronous_Operation,B250000,\
EightBit,OneBit,Parity_Disabled};
/*---------------------------------------------------------------------------------------------------------------*/
/*TWI Initialization Configuration*/
TWI_CONFIG_STRUCT TWI_Config = {TWI_This_Device_Address,TWI_Bit_Rate_KHz,CLOCK_N1};
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
unsigned char Wrong_Entries_Number = 0; 		/*Variable to count number of wrong passwords entered*/
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
void ControlECU2_init(void)
{
	sei(); /*Enable Global Interrupt*/
	UART_init(&UART_USART_Configuration);
	TWI_init(&TWI_Config);
	TIMER1_setCallBackFunctionCOMP(Timer1_Control);	/*Set Timer1 (Compare mode) Call back function*/
	DcMotor_init();
	BUZZER_init();
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to receive new password from ECU1
 * [No Inputs]:
 * [No Returns]:
 */
void Receive_New_Password(void)
{
	while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};
	for(unsigned char i=0 ; i<PASSWORD_SIZE ; i++)
	{
		UART_sendByte(UART_SEND_OR_RECEIVE_READY);
		Password[i] = UART_receiveByte();
	}
	for(unsigned char i=0 ; i<PASSWORD_SIZE ; i++)
	{
		UART_sendByte(UART_SEND_OR_RECEIVE_READY);
		Password_check[i] = UART_receiveByte();
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Algorithm to check matching of any 2 passwords arrays
 * 					2 arrays from user or an array from user and another one stored in the EEPROM
 *
 * [Inputs]:		2 pointers to unsigned char arrays containing the password
 * 					and a variable containing their size
 *
 * [Returns]:		unsigned char variable to indicate if they are matched or not
 */
unsigned char Check_Password_Match (unsigned char* P1,unsigned char* P2,unsigned char size)
{
	for(unsigned char i=0 ; i<size ; i++)
		if(*(P1+i) != *(P2+i))
			return 0;	/*Not Matched*/
	return 1;			/*Matched*/
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to receive the registered password from ECU1
 * [No Inputs]:
 * [No Returns]:
 */
void Receive_Password(void)
{
	while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};
	for(unsigned char i=0 ; i<PASSWORD_SIZE ; i++)
	{
		UART_sendByte(UART_SEND_OR_RECEIVE_READY);
		Password_check[i] = UART_receiveByte();
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to store the password in EEPROM
 * [No Inputs]:
 * [No Returns]:
 */
void Store_Password_EEPROM(void)
{
	for(unsigned char i=0 ; i<PASSWORD_SIZE ; i++)
	{
		EEPROM_witeByte(PASSWORD_START_ADDRESS+i,Password[i]);
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to get the password from EEPROM
 * [No Inputs]:
 * [No Returns]:
 */
void Get_Password_EEPROM(void)
{
	for(unsigned char i=0 ; i<PASSWORD_SIZE ; i++)
	{
		EEPROM_readByte(PASSWORD_START_ADDRESS+i,&Password[i]);
	}
}
/*===================================================MAIN========================================================*/
int main(void)
{
	ControlECU2_init();					/*Initialization Function*/
	/*---------------------------------------------------------------------------------------------------*/
	EEPROM_readByte(PASSWORD_START_ADDRESS+End_Point,&Password[End_Point]); /*Read Password Array last element*/
	while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};				/*Polling until ECU1 is Synchronized*/
	UART_sendByte(Password[End_Point]);	   /*Send Password Array last element to check if it's first time or not*/
	/*---------------------------------------------------------------------------------------------------*/
	if(Password [End_Point] != '#')		/*If last element of the array is not # means no password is registered before*/
	{
		while(password_match != 1)
		{
			Receive_New_Password();					   /*Receive the new Password from ECU1*/
			password_match = Check_Password_Match(Password,Password_check,PASSWORD_SIZE);/*Check if password is matched*/
			UART_sendByte(UART_SEND_OR_RECEIVE_READY);	/*Send that this ECU is Synchronized*/
			while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU1 is Synchronized*/
			UART_sendByte(password_match);	/*Send password is matched or not*/
		}
		password_match = 0; /*End Matching Case*/
		Store_Password_EEPROM(); /*Store the password in the EEPROM*/
	}
	/*---------------------------------------------------------------------------------------------------*/
	while(1)
	{
		while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){}; /*Polling until ECU1 is Synchronized*/
		UART_sendByte(UART_SEND_OR_RECEIVE_READY);					/*Send that this ECU is Synchronized*/
		Main_Menu_Choice = UART_receiveByte();
		if(Main_Menu_Choice == '+')
		{
			while(1)
			{
				Receive_Password();		/*Receive entered password from ECU1*/
				Get_Password_EEPROM();	/*Get the stored password in EEPROM*/
				password_match = Check_Password_Match(Password,Password_check,PASSWORD_SIZE); /*Check password match*/
				UART_sendByte(UART_SEND_OR_RECEIVE_READY);	/*Send that this ECU is Synchronized*/
				while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU1 is Synchronized*/
				/*Beware at this point*/
				UART_sendByte(password_match);	/*Send password is matched or not*/
				if(password_match == 1)			/*If matched*/
				{
					while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU1 is Synchronized*/
					/*Beware at this point*/
					UART_sendByte(UART_SEND_OR_RECEIVE_READY); /*Send that this ECU is Synchronized*/
					/*-------------------------------------------------------------------------*/
					TIMER1_Config.Compare_Value16 = Compare_Value_For_15_Seconds;
					TIMER1_init(&TIMER1_Config);
					Timer1_ON = 1;
					DcMotor_Rotate(CW,100);
					while(Timer1_ON == 1){}
					/*-------------------------------------------------------------------------*/
					TIMER1_Config.Compare_Value16 = Compare_Value_For_3_Seconds;
					TIMER1_init(&TIMER1_Config);
					Timer1_ON = 1;
					DcMotor_Rotate(stop,0);
					while(Timer1_ON == 1){}
					/*-------------------------------------------------------------------------*/
					TIMER1_Config.Compare_Value16 = Compare_Value_For_15_Seconds;
					TIMER1_init(&TIMER1_Config);
					Timer1_ON = 1;
					DcMotor_Rotate(A_CW,100);
					while(Timer1_ON == 1){}
					DcMotor_Rotate(stop,0); /*Finally Stop the DC Motor*/
					/*-------------------------------------------------------------------------*/
					Wrong_Entries_Number = 0;	/*Reset number of wrong entries*/
					password_match = 0;			/*End Matching Case*/
					break; /*Exit the While Loop and ask again for options*/
				}
				else if(password_match == 0)
				{
					Wrong_Entries_Number++;
					if(Wrong_Entries_Number == 3)
					{
						while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU1 is Synchronized*/
						/*Beware at this point*/
						UART_sendByte(UART_SEND_OR_RECEIVE_READY); /*Send that this ECU is Synchronized*/
						TIMER1_Config.Compare_Value16 = Compare_Value_For_60_Seconds;
						/*3 times - 20secondsx3 times = 60 seconds*/
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						/*Starting the Alarm*/
						BUZZER_On();
						while(Timer1_ON == 1){};
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						while(Timer1_ON == 1){};
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						while(Timer1_ON == 1){};
						/*End the Alarm*/
						BUZZER_Off();
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
				Receive_Password();		/*Receive entered password from ECU1*/
				Get_Password_EEPROM();	/*Get the stored password in EEPROM*/
				password_match = Check_Password_Match(Password,Password_check,PASSWORD_SIZE); /*Check password match*/
				UART_sendByte(UART_SEND_OR_RECEIVE_READY);	/*Send that this ECU is Synchronized*/
				while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU1 is Synchronized*/
				/*Beware at this point*/
				UART_sendByte(password_match);	/*Send password is matched or not*/
				if(password_match == 1)			/*If matched*/
				{
					password_match = 0;	/*End Matching Case*/
					while(password_match != 1)
					{
						Receive_New_Password();					   /*Receive the new Password from ECU1*/
						password_match = Check_Password_Match(Password,Password_check,PASSWORD_SIZE);/*Check if password is matched*/
						UART_sendByte(UART_SEND_OR_RECEIVE_READY);	/*Send that this ECU is Synchronized*/
						while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU1 is Synchronized*/
						UART_sendByte(password_match);	/*Send password is matched or not*/
					}
					password_match = 0; /*End Matching Case*/
					Store_Password_EEPROM(); /*Store the password in the EEPROM*/
					Wrong_Entries_Number = 0;	/*Reset number of wrong entries*/
					break; /*Exit the While Loop and ask again for options*/

				}
				else if(password_match == 0)
				{
					Wrong_Entries_Number++;
					if(Wrong_Entries_Number == 3)
					{
						while(UART_receiveByte() != UART_SEND_OR_RECEIVE_READY){};/*Polling until ECU1 is Synchronized*/
						/*Beware at this point*/
						UART_sendByte(UART_SEND_OR_RECEIVE_READY); /*Send that this ECU is Synchronized*/
						TIMER1_Config.Compare_Value16 = Compare_Value_For_60_Seconds;
						/*3 times - 20secondsx3 times = 60 seconds*/
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						/*Starting the Alarm*/
						BUZZER_On();
						while(Timer1_ON == 1){};
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						while(Timer1_ON == 1){};
						TIMER1_init(&TIMER1_Config);
						Timer1_ON = 1;
						while(Timer1_ON == 1){};
						/*End the Alarm*/
						BUZZER_Off();
						/*-------------------------------------------------------------------------*/
						Wrong_Entries_Number = 0;	/*Reset number of wrong entries*/
						break; /*Exit the While Loop and ask again for options*/
					}
				}
				Main_Menu_Choice = 0;	/*Reset main menu choice*/
			}
		}
	}
}
