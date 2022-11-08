/*
 *  buzzer.h
 *  Created on: Oct 31, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			Buzzer Driver to easily control the buzzer in a layered architecture model
 */
#ifndef BUZZER_H_
#define BUZZER_H_
/*=================================================INCLUDES======================================================*/
#include "gpio.h"
#include "std_types.h"
/*========================================DEFINITIONS & CONFIGURATIONS===========================================*/
#define BUZZER_PORT_ID	PORTA_ID
#define BUZZER_PIN_ID	PIN0_ID
#define BUZZER_ON		LOGIC_HIGH
#define BUZZER_OFF		LOGIC_LOW
/*=================================FUNCTIONS DECLARATIONS================================*/
/*Functions Description in .c file*/
void BUZZER_init(void);
void BUZZER_On(void);
void BUZZER_Off(void);
#endif /* BUZZER_H_ */
