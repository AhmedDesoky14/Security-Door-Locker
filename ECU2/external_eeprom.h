/*
 *  external_eeprom.h
 *  Created on: Oct 23, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			24CXX External EEPROM Driver
 *  			#interfacing using TWI Protocol
 */
#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_
/*=================================================INCLUDES===========================================================*/
#include "twi.h"
/*================================================DEFINITIONS=========================================================*/
#define ERROR 	0
#define SUCCESS 1
/*===========================================FUNCTIONS DECLARATIONS===================================================*/
/*Functions Description in .c file*/
uint8 EEPROM_witeByte(uint16 u16Address,uint8 data);
uint8 EEPROM_readByte(uint16 u16Address,uint8* data);
#endif /* EXTERNAL_EEPROM_H_ */
