/*
 *  dcmotor.h
 *  Created on: Oct 7, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			DC Motor Driver
 */
#ifndef DCMOTOR_H_
#define DCMOTOR_H_
/*==================================INCLUDES=============================================*/
#include "gpio.h"
#include "timers.h"
#include "std_types.h"
/*==============================ENUMS & STRUCTURES=======================================*/
typedef enum
{
	stop,CW,A_CW
}DcMotor_State;
/*===========================DEFINITIONS & CONFIGURATIONS================================*/
#define DCMOTOR_A_PORT_ID PORTB_ID
#define DCMOTOR_A_PIN_ID PIN1_ID
#define DCMOTOR_B_PORT_ID PORTB_ID
#define DCMOTOR_B_PIN_ID PIN0_ID
/*=================================FUNCTIONS DECLARATIONS================================*/
/*Functions Description in .c file*/
void DcMotor_init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);
#endif /* DCMOTOR_H_ */
