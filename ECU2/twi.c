/*
 *  twi.c
 *  Created on: Oct 23, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			AVR ATmega32/L TWI Driver
 *  			#TWI Pins (SCL-SDL) automatically reserved by the TWI Protocol without GPIO Commands
 */
/*============================================INCLUDES==================================================*/
#include "twi.h"
/*======================================FUNCTIONS DEFINITIONS===========================================*/
/* Description:
 * 					Function to initialze TWI
 * 					This Function is Re-Configurable to adjust SCL
 * [No Inputs]:
 * [No Returns]:
 */
void TWI_init(TWI_CONFIG_STRUCT *TWI_Config_Type)
{

	uint16 twps = 1;
	for(uint8 i=0 ; i<TWI_Config_Type->TWI_Clock_Prescaler ; i++)
		twps = twps * 4;
	uint16 x = (F_CPU/(twps*2*(TWI_Config_Type->Bit_Rate))); /*Calculate First part of TWBR*/
	uint16 y = (8/twps);									 /*Calculate Second part of TWBR*/
	TWBR = (uint8)(x-y); 									 /*Calculate TWBR*/
	TWAR = TWI_Config_Type->Device_Address;
	TWSR = TWI_Config_Type->TWI_Clock_Prescaler;
	//TWBR = 0x02; /*Adjusted Bit Rate= 400Kbps in case of F_CPU = 8MHz*/
	//TWSR = 0x00; /*TWPS = 00 - No Prescaler*/
	//TWAR = 0b00000010; /*Set this MCU Address by 0x01*/
	SET_BIT(TWAR,TWGCE); /*Enable recognition of a General Call - in case of Slave*/
	TWCR = (1<<TWEN); /*Enable TWI*/
}
/*------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to Send Start Condition on SDL
 * [No Inputs]:
 * [No Returns]:
 */
void TWI_start(void)
{
	/*Clear TWCR Register Then ---> Clear TWI Flag by setting it by 1, Enable Start condition Bit, Enable TWI*/
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	/*Polling until TWI Flag is set which means Start Condition is sent succsessfully*/
	while(BIT_IS_CLEAR(TWCR,TWINT));
}
/*------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to Send Stop Condition on SDL to Selected Slave
 * [No Inputs]:
 * [No Returns]:
 */
void TWI_stop(void)
{
	/*Clear TWCR Register Then ---> Clear TWI Flag by setting it by 1, Enable Stop condition Bit, Enable TWI*/
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	/*We didn't use polling like previous TWI_Start becasue Flag doens't set after sending the Stop Condition*/
}
/*------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to transmit Byte on SDL to Slave
 * [Inputs]:
 * 					Byte to be transmitted
 * [No Returns]:
 */
void TWI_writeByte(uint8 data)
{
	TWDR = data;
	/*Clear TWCR Register Then ---> Clear TWI Flag by setting it by 1, Enable TWI*/
	/*NOTE: when TWI Flag is cleared, Automatically transmission process proceed*/
	TWCR = (1 << TWINT) | (1 << TWEN);
	/*Polling until TWI Flag is set which means the Byte is sent succsessfully*/
	while(BIT_IS_CLEAR(TWCR,TWINT));
}
/*------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to receive Byte on SDL from Slave then send ACK
 * [No Inputs]:
 * [Returns]:
 * 					Byte to be received
 */
uint8 TWI_readByteWithACK(void)
{
	/*Clear TWCR Register Then ---> Clear TWI Flag by setting it by 1, Enable ACK Bit , Enable TWI*/
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	/*Polling until TWI Flag is set which means the Byte is receievd succsessfully*/
	while(BIT_IS_CLEAR(TWCR,TWINT));
	/*Return byte (data) received*/
	return TWDR;
}
/*------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to receive Byte on SDL from Slave then send NACK (doesn't send ACK)
 * [No Inputs]:
 * [Returns]:
 * 					Byte to be received
 */
uint8 TWI_readByteWithNACK(void)
{
	/*Clear TWCR Register Then ---> Clear TWI Flag by setting it by 1, keep ACK Bit by 0, Enable TWI*/
	TWCR = (1 << TWINT) | (1 << TWEN);
	/*Polling until TWI Flag is set which means the Byte is receievd succsessfully*/
	while(BIT_IS_CLEAR(TWCR,TWINT));
	/*Return byte (data) received*/
	return TWDR;
}
/*------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to get the status of the TWI
 * [No Inputs]:
 * [Returns]:
 * 					TWI Status
 */
uint8 TWI_getStatus(void)
{
	uint8 status;
	/*get the first MS 5 Bits from TWSR Register to get status*/
	status = 0xF8 & TWSR;
	/*Return the status*/
	return status;
}
