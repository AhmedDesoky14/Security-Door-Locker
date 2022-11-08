/*
 *  external_eeprom.c
 *  Created on: Oct 23, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			24CXX External EEPROM Driver
 *  			#interfacing using TWI Protocol
 */
/*====================================================INCLUDES========================================================*/
#include "external_eeprom.h"
/*==============================================FUNCTIONS DEFINITIONS=================================================*/
/* Description:
 * 				Function to write a byte in external EEPROM in certain location
 * [Inputs]:	data to be written, Address of the EEPROM and the location to write the byte
 *
 * [Returns]: 	Transmission Status
 */
uint8 EEPROM_witeByte(uint16 u16Address,uint8 data)
{
	/*Send Start condition*/
	TWI_start();
	/*If isn't complete return with ERROR*/
	if(TWI_getStatus() != TWI_START)
		return ERROR;
	/*Send 1010xxx0b - x xx stands for A10,A9,A8 Addresses in the external  EEPROM - last 0 stands for write*/
	/*Over all it expresses Device Address and Write*/
	TWI_writeByte((uint8)(0xA0 | ((u16Address & 0x700)>>7)));
	/*If status of writing with ACK isn't complete return with ERROR*/
	if(TWI_getStatus() != TWI_MST_SLA_W_ACK)
		return ERROR;
	/*Send Address where i want to the write the byte in the external EEPROM*/
	TWI_writeByte((uint8)(u16Address));
	/*If status of writing with ACK isn't complete return with ERROR*/
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;
	/*Write the byte in the external EEPROM*/
	TWI_writeByte(data);
	/*If status of writing with ACK isn't complete return with ERROR*/
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;
	/*Send Stop condition*/
	TWI_stop();
	/*Finally Successfully Sent then return with SUCCESS*/
	return SUCCESS;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 				Function to Read a byte from external EEPROM in certain location
 * [Inputs]:	pointer to uint8 to store data inside, Address of the EEPROM and the location to Read the byte from
 *
 * [Returns]: 	Transmission Status
 */
uint8 EEPROM_readByte(uint16 u16Address,uint8* data)
{
	/*Send Start condition*/
	TWI_start();
	/*If isn't complete return with ERROR*/
	if(TWI_getStatus() != TWI_START)
		return ERROR;
	/*Send 1010xxx0b - xxx stands for A10,A9,A8 Addresses in the external  EEPROM - last 0 stands for write*/
	/*Over all it expresses Device Address and Write*/
	TWI_writeByte((uint8)(0xA0 | ((u16Address & 0x700)>>7)));
	/*If status of writing with ACK isn't complete return with ERROR*/
	if(TWI_getStatus() != TWI_MST_SLA_W_ACK)
		return ERROR;
	/*Send Address where i want to the write the byte in the external EEPROM*/
	TWI_writeByte((uint8)(u16Address));
	/*If status of writing with ACK isn't complete return with ERROR*/
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;
	/*Send repreated Start Condition*/
	TWI_start();
	/*If isn't complete return with ERROR*/
	if(TWI_getStatus() != TWI_REP_START)
		return ERROR;
	/*Send again 1010xxx1b - xxx stands for A10,A9,A8 Addresses in the external  EEPROM - last 1 stands for Read*/
	TWI_writeByte((uint8)((0xA0 | ((u16Address & 0x700)>>7)) | 1));
	/*If status of writing with ACK isn't complete return with ERROR*/
	if(TWI_getStatus() != TWI_MST_SLA_R_ACK)
		return ERROR;
	/*Read data in the chosen location from the EEPROM*/
	*data = TWI_readByteWithNACK();
	/*If reading isn't complete return with ERROR*/
	if(TWI_getStatus() != TWI_MR_DATA_NACK)
			return ERROR;
	/*Send Stop condition*/
	TWI_stop();
	return SUCCESS;
}
