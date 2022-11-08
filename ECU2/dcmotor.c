/*
 *  dcmotor.c
 *  Created on: Oct 7, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			DC Motor Driver
 */
/*==================================INCLUDES=============================================*/
#include"dcmotor.h"
/*================================GLOBAL VARIABLES=======================================*/
TIMER_CONFIG_STRUCT TIMER0_Config = {0,255,0,0,F_CPU_N8,PWM_MODE}; /*Timer0 PWM Configuration*/
/*=============================FUNCTIONS DEFINITIONS=====================================*/
/* Description:
 * Function to initialize DC Motor
 * [No Inputs]
 * [No Returns]
 */
void DcMotor_init(void)
{
	GPIO_setupPinDirection(DCMOTOR_A_PORT_ID,DCMOTOR_A_PIN_ID,PIN_OUTPUT); /*Set Pin A of motor as output*/
	GPIO_setupPinDirection(DCMOTOR_B_PORT_ID,DCMOTOR_B_PIN_ID,PIN_OUTPUT); /*Set Pin B of motor as output*/
	GPIO_writePin(DCMOTOR_A_PORT_ID,DCMOTOR_A_PIN_ID,LOGIC_LOW); /*Stop the DC Motor initially*/
	GPIO_writePin(DCMOTOR_A_PORT_ID,DCMOTOR_B_PIN_ID,LOGIC_LOW);
}
/*---------------------------------------------------------------------------------------*/
/* Description:
 * 			Function to initialize DC Motor and make it run
 * [Inputs]:
 * 			desired state to rotate & speed
 * [No Returns]
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{

	switch(state)
	{
		case(stop):
			GPIO_writePin(DCMOTOR_A_PORT_ID,DCMOTOR_A_PIN_ID,LOGIC_LOW); /*Stop the DC Motor*/
			GPIO_writePin(DCMOTOR_A_PORT_ID,DCMOTOR_B_PIN_ID,LOGIC_LOW);
			break;
		case(CW):
			GPIO_writePin(DCMOTOR_A_PORT_ID,DCMOTOR_A_PIN_ID,LOGIC_LOW); /*DC Motor in clock wise*/
			GPIO_writePin(DCMOTOR_A_PORT_ID,DCMOTOR_B_PIN_ID,LOGIC_HIGH);
			break;
		case(A_CW):
			GPIO_writePin(DCMOTOR_A_PORT_ID,DCMOTOR_A_PIN_ID,LOGIC_HIGH); /*DC Motor in anti-clock wise*/
			GPIO_writePin(DCMOTOR_A_PORT_ID,DCMOTOR_B_PIN_ID,LOGIC_LOW);
			break;
	}
	speed = (uint8)(((uint16)(speed*255))/100);
	TIMER0_Config.Compare_Value8 = speed; /*edit configuration according to speed used*/
	TIMER0_init(&TIMER0_Config);
}
