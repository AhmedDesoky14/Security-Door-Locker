/*
 *  twi.h
 *  Created on: Oct 23, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			AVR ATmega32/L TWI Driver
 * 				#TWI Pins (SCL-SDL) automatically reserved by the TWI Protocol without GPIO Commands
 *
 */
#ifndef TWI_H_
#define TWI_H_
/*=================================================INCLUDES===========================================================*/
#include <avr/io.h>
#include "common_macros.h"
#include "std_types.h"
/*=============================================ENUMS & STRUCTURES================================================*/
typedef enum
{
	CLOCK_N1,CLOCK_N4,CLOCK_N16,CLOCK_N64
}TWI_CLOCK_PRESCALER;
/*---------------------------------------------------------------------------------------------------------------*/
typedef struct
{
	uint8 Device_Address;
	uint8 Bit_Rate;			/*in KHz*/
	TWI_CLOCK_PRESCALER TWI_Clock_Prescaler;
}TWI_CONFIG_STRUCT;
/*================================================DEFINITIONS=========================================================*/
#define TWI_START			0x08
#define TWI_REP_START		0x10
#define TWI_MST_SLA_W_ACK	0x18 /*Master transmitted (SLave Address+Write request) to slave + ACK received from slave*/
#define TWI_MST_SLA_W_NACK	0x20 /*Master transmitted (SLave Address+Write request) to slave + NACK received from slave*/
#define TWI_MST_SLA_R_ACK	0x40 /*Master transmitted (SLave Address+Read request) to slave + ACK received from slave*/
#define TWI_MST_SLA_R_NACK	0x48 /*Master transmitted (SLave Address+Read request) to slave + NACK received from slave*/
#define TWI_MSTR_ARB_LST	0x38 /*MaSTer Lost ARBitration-Enter Slave mode until Bus Released then send Start Condition*/
#define TWI_MT_DATA_ACK		0x28 /*Master transmitted data and ACK has been received from Slave*/
#define TWI_MT_DATA_NACK	0x30 /*Master transmitted data and NACK has been received from Slave*/
#define TWI_MR_DATA_ACK		0x50 /*Master received data and sent ACK to slave*/
#define TWI_MR_DATA_NACK	0x58 /*Master received data and sent NACK to slave*/
/*===========================================FUNCTIONS DECLARATIONS===================================================*/
/*Functions Description in .c file*/
void TWI_init(TWI_CONFIG_STRUCT *TWI_Config_Type);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);
#endif /* TWI_H_ */
