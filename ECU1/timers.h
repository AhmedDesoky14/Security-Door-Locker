/*
 *  timers.h
 *  Created on: Oct 24, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			Full ATmega32/L Timers Driver, includes Timers 0,1,2 in their different modes
 *  			Overflow Mode, Compare Mode, PWM Mode, with their different clocks and pre-scalers
 */
#ifndef TIMERS_H_
#define TIMERS_H_
/*=================================================INCLUDES======================================================*/
#include "std_types.h"
#include "common_macros.h"
#include <avr/interrupt.h>
#include <avr/io.h>
/*=============================================ENUMS & STRUCTURES================================================*/
typedef enum	/*enum for different TIMER0 Clocks & Prescalers*/
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_N8,F_CPU_N32,F_CPU_N64,F_CPU_N128,F_CPU_N256,F_CPU_N1024,T0_EXTERNAL_CLOCK_FALLING,
	T0_EXTERNAL_CLOCK_RISING,T1_EXTERNAL_CLOCK_FALLING,T1_EXTERNAL_CLOCK_RISING
}TIMER_CLOCK_PRESCALER;
/*---------------------------------------------------------------------------------------------------------------*/
typedef enum	/*enum for different modes of TIMER0*/
{
	OVERFLOW_MODE,COMPARE_MODE,PWM_MODE
}TIMER_OPERATION_MODE;
/*===============================================================================================================*/
typedef struct	/*Structure contains TIMER0 Configurations*/
{
	uint8 Initial_value8;
	uint8 Compare_Value8;
	uint16 Initial_value16;
	uint16 Compare_Value16;
	TIMER_CLOCK_PRESCALER	 TIMER_Prescaler;
	TIMER_OPERATION_MODE 	 TIMER_Mode;
}TIMER_CONFIG_STRUCT;
/*===========================================FUNCTIONS DECLARATIONS==============================================*/
/*Functions Description in .c file*/
void TIMER0_init(TIMER_CONFIG_STRUCT *TIMER0_Config_Type);
void TIMER0_setCallBackFunctionsOVF(void(*a_FunctionPtr_OVF)(void));
void TIMER0_setCallBackFunctionCOMP(void(*a_FunctionPtr_COMP)(void));
void TIMER0_Deinit(void);
/*===============================================================================================================*/
void TIMER1_init(TIMER_CONFIG_STRUCT *TIMER1_Config_Type);
void TIMER1_setCallBackFunctionsOVF(void(*a_FunctionPtr_OVF)(void));
void TIMER1_setCallBackFunctionCOMP(void(*a_FunctionPtr_COMP)(void));
void TIMER1_Deinit(void);
/*===============================================================================================================*/
void TIMER2_init(TIMER_CONFIG_STRUCT *TIMER2_Config_Type);
void TIMER2_setCallBackFunctionsOVF(void(*a_FunctionPtr_OVF)(void));
void TIMER2_setCallBackFunctionCOMP(void(*a_FunctionPtr_COMP)(void));
void TIMER2_Deinit(void);
/*===============================================================================================================*/
#endif /* TIMERS_H_ */
