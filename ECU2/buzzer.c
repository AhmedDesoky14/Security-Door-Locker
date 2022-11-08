/*
 *  buzzer.c
 *  Created on: Oct 31, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			Buzzer Driver to easily control the buzzer in a layered architecture model
 */
/*==================================================INCLUDES=====================================================*/
#include "buzzer.h"
/*============================================FUNCTIONS DEFINITIONS==============================================*/
/* Description:
 * 					Function to initialize the buzzer using gpio driver by determining its output pin
 * [No Inputs]:
 * [No Returns]:
 */
void BUZZER_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT); /*Set pin Output*/
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,BUZZER_OFF);			 /*Initially set buzzer off*/
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to Turn on the buzzer
 * [No Inputs]:
 * [No Returns]:
 */
void BUZZER_On(void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,BUZZER_ON);
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to Turn off the buzzer
 * [No Inputs]:
 * [No Returns]:
 */
void BUZZER_Off(void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,BUZZER_OFF);
}
