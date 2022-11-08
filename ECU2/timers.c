/*
 *  timers.c
 *  Created on: Oct 24, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			Full ATmega32/L Timers Driver, includes Timers 0,1,2 in their different modes
 *  			Overflow Mode, Compare Mode, PWM Mode, with their different clocks and pre-scalers
 */
/*==================================================INCLUDES=====================================================*/
#include "timers.h"
/*==============================================GLOBAL VARIABLES=================================================*/
static volatile void (*TIMER0_Comp_g_callBackFunction_Ptr)(void) = NULL_PTR;
static volatile void (*TIMER0_Ovf_g_callBackFunction_Ptr)(void) = NULL_PTR;
static volatile void (*TIMER1_Comp_g_callBackFunction_Ptr)(void) = NULL_PTR;
static volatile void (*TIMER1_Ovf_g_callBackFunction_Ptr)(void) = NULL_PTR;
static volatile void (*TIMER2_Comp_g_callBackFunction_Ptr)(void) = NULL_PTR;
static volatile void (*TIMER2_Ovf_g_callBackFunction_Ptr)(void) = NULL_PTR;
/*====================================================ISRs=======================================================*/
/* Description:
 * 					ISRs of the Timers 0,1,2 to call back the function in the upper abstraction layer
 * [No Inputs]:
 * [No Returns]:
 */
ISR(TIMER0_OVF_vect)
{
	if(TIMER0_Ovf_g_callBackFunction_Ptr != NULL_PTR)
	{
		(*TIMER0_Ovf_g_callBackFunction_Ptr)();
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
ISR(TIMER0_COMP_vect)
{
	if(TIMER0_Comp_g_callBackFunction_Ptr != NULL_PTR)
	{
		(*TIMER0_Comp_g_callBackFunction_Ptr)();
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
ISR(TIMER1_OVF_vect)
{
	if(TIMER1_Ovf_g_callBackFunction_Ptr != NULL_PTR)
	{
		(*TIMER1_Ovf_g_callBackFunction_Ptr)();
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
ISR(TIMER1_COMPA_vect)
{
	if(TIMER1_Comp_g_callBackFunction_Ptr != NULL_PTR)
	{
		(*TIMER1_Comp_g_callBackFunction_Ptr)();
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
ISR(TIMER2_OVF_vect)
{
	if(TIMER2_Ovf_g_callBackFunction_Ptr != NULL_PTR)
	{
		(*TIMER2_Ovf_g_callBackFunction_Ptr)();
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
ISR(TIMER2_COMP_vect)
{
	if(TIMER2_Comp_g_callBackFunction_Ptr != NULL_PTR)
	{
		(*TIMER2_Comp_g_callBackFunction_Ptr)();
	}
}
/*============================================FUNCTIONS DEFINITIONS==============================================*/
/* Description:
 * 					Function to Start Timer0 with certain configuration
 * [Inputs]:		TIMER0 Configuration structure by address
 *
 * [No Returns]:
 */
void TIMER0_init(TIMER_CONFIG_STRUCT *TIMER0_Config_Type)
{
	TCNT0 = TIMER0_Config_Type->Initial_value8;	/*Initial Value*/
	OCR0  = TIMER0_Config_Type->Compare_Value8;	/*Compare Value*/
	/*switch to choose clock pre-scaler*/
	switch(TIMER0_Config_Type->TIMER_Prescaler)
	{
		case F_CPU_CLOCK:
			SET_BIT(TCCR0,CS00);
			break;
		case F_CPU_N8:
			SET_BIT(TCCR0,CS01);
			break;
		case F_CPU_N64:
			SET_BIT(TCCR0,CS00);
			SET_BIT(TCCR0,CS01);
			break;
		case F_CPU_N256:
			SET_BIT(TCCR0,CS02);
			break;
		case F_CPU_N1024:
			SET_BIT(TCCR0,CS00);
			SET_BIT(TCCR0,CS02);
			break;
		case T0_EXTERNAL_CLOCK_FALLING:
			SET_BIT(TCCR0,CS01);
			SET_BIT(TCCR0,CS02);
			CLEAR_BIT(DDRB,PB0);
			break;
		case T0_EXTERNAL_CLOCK_RISING:
			SET_BIT(TCCR0,CS00);
			SET_BIT(TCCR0,CS01);
			SET_BIT(TCCR0,CS02);
			CLEAR_BIT(DDRB,PB0);
			break;
	}
	/*switch to choose mode*/
	switch(TIMER0_Config_Type->TIMER_Mode)
	{
		case OVERFLOW_MODE:
			SET_BIT(TIMSK,TOIE0);	/*Enable Overflow Mode Interrupt Module*/
			SET_BIT(TCCR0,FOC0);
			break;
		case COMPARE_MODE:
			SET_BIT(TIMSK,OCIE0);	/*Enable Compare Mode Interrupt Module*/
			SET_BIT(TCCR0,FOC0);
			SET_BIT(TCCR0,WGM01);
			break;
		case PWM_MODE:				/*FPWM=F_CPU/N*256*/
			SET_BIT(DDRB,PB3); 		/*Make OC0 as output pin*/
			SET_BIT(TCCR0,WGM00);
			SET_BIT(TCCR0,WGM01);
			SET_BIT(TCCR0,COM01);	/*Non-Inverting Mode*/
			break;
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Functions to set the call back functions
 * [Inputs]:		Pointers to functions that will be called from Timer0 ISR
 *
 * [No Returns]:
 */
void TIMER0_setCallBackFunctionsOVF(void(*a_FunctionPtr_OVF)(void))
{
	TIMER0_Ovf_g_callBackFunction_Ptr = a_FunctionPtr_OVF;
}
void TIMER0_setCallBackFunctionCOMP(void(*a_FunctionPtr_COMP)(void))
{
	TIMER0_Comp_g_callBackFunction_Ptr = a_FunctionPtr_COMP;
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to turn off Timer0
 * [No Inputs]:
 * [No Returns]:
 */
void TIMER0_Deinit(void)
{
	SET_BIT(DDRB,PB0);		/*Detach T0*/
	CLEAR_BIT(DDRB,PB3);	/*Detach OC0*/
	TCCR0 = 0;				/*Clear Timer0 Control Register*/
	CLEAR_BIT(TIMSK,OCIE0); /*Disable Overflow Mode Interrupt Module*/
	CLEAR_BIT(TIMSK,TOIE0); /*Disable Compare Mode Interrupt Module*/
	CLEAR_BIT(TIFR,OCF0);	/*Clear Compare Mode Flag*/
	CLEAR_BIT(TIFR,TOV0);	/*Clear Overflow Mode Flag*/
	OCR0  = 0;				/*Clear Compare Register*/
	TCNT0 = 0;				/*Clear Counter Register*/
}
/*===============================================================================================================*/
/* Description:
 * 					Function to Start Timer1 with certain configuration
 * [Inputs]:		TIMER1 Configuration structure by address
 *
 * [No Returns]:
 */
void TIMER1_init(TIMER_CONFIG_STRUCT *TIMER1_Config_Type)
{
	TCNT1 = TIMER1_Config_Type->Initial_value16;	/*Initial Value*/
	OCR1A = TIMER1_Config_Type->Compare_Value16;	/*Compare Value*/
	ICR1  = TIMER1_Config_Type->Compare_Value16;	/*Top value in PWM Mode*/
	/*switch to choose clock pre-scaler*/
	switch(TIMER1_Config_Type->TIMER_Prescaler)
	{
		case F_CPU_CLOCK:
			SET_BIT(TCCR1B,CS10);
			break;
		case F_CPU_N8:
			SET_BIT(TCCR1B,CS11);
			break;
		case F_CPU_N64:
			SET_BIT(TCCR1B,CS10);
			SET_BIT(TCCR1B,CS11);
			break;
		case F_CPU_N256:
			SET_BIT(TCCR1B,CS12);
			break;
		case F_CPU_N1024:
			SET_BIT(TCCR1B,CS10);
			SET_BIT(TCCR1B,CS12);
			break;
		case T1_EXTERNAL_CLOCK_FALLING:
			SET_BIT(TCCR1B,CS11);
			SET_BIT(TCCR1B,CS12);
			CLEAR_BIT(DDRB,PB1);
			break;
		case T1_EXTERNAL_CLOCK_RISING:
			SET_BIT(TCCR1B,CS10);
			SET_BIT(TCCR1B,CS11);
			SET_BIT(TCCR1B,CS12);
			CLEAR_BIT(DDRB,PB1);
			break;
	}
	/*switch to choose mode*/
	switch(TIMER1_Config_Type->TIMER_Mode)
	{
		case OVERFLOW_MODE:
			SET_BIT(TIMSK,TOIE1);	/*Enable Overflow Mode Interrupt Module*/
			SET_BIT(TCCR1A,FOC1A);
			SET_BIT(TCCR1A,FOC1B);
			break;
		case COMPARE_MODE:
			SET_BIT(TIMSK,OCIE1A);	/*Enable Compare Mode Interrupt Module*/
			SET_BIT(TCCR1A,FOC1A);
			SET_BIT(TCCR1A,FOC1B);
			SET_BIT(TCCR1B,WGM12);
			break;
		case PWM_MODE:				/*FPWM=F_CPU/N*256*/
			SET_BIT(DDRD,PD5); 		/*Make OC1A as output pin*/
			SET_BIT(TCCR1A,COM1A1);	/*Non-Inverting Mode*/
			SET_BIT(TCCR1A,COM1B1);
			SET_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1B,WGM13);
			break;
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Functions to set the call back functions
 * [Inputs]:		Pointers to functions that will be called from Timer1 ISR
 *
 * [No Returns]:
 */
void TIMER1_setCallBackFunctionsOVF(void(*a_FunctionPtr_OVF)(void))
{
	TIMER1_Ovf_g_callBackFunction_Ptr = a_FunctionPtr_OVF;
}
void TIMER1_setCallBackFunctionCOMP(void(*a_FunctionPtr_COMP)(void))
{
	TIMER1_Comp_g_callBackFunction_Ptr = a_FunctionPtr_COMP;
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to turn off Timer1
 * [No Inputs]:
 * [No Returns]:
 */
void TIMER1_Deinit(void)
{
	SET_BIT(DDRB,PB1);		/*Detach T1*/
	CLEAR_BIT(DDRD,PD5);	/*Detach OC1A*/
	TCCR1A = 0;				/*Clear Timer1 Control Register A*/
	TCCR1B = 0;				/*Clear Timer1 Control Register B*/
	CLEAR_BIT(TIMSK,TOIE1);	/*Disable Overflow Interrupt Module*/
	CLEAR_BIT(TIMSK,OCIE1A);/*Disable CompareA Interrupt Module*/
	CLEAR_BIT(TIFR,TOV0);	/*Clear Overflow Interrupt Flag*/
	CLEAR_BIT(TIFR,OCF1A);	/*Clear CompareA Interrupt Flag*/
	ICR1 = 0;				/*Clear Input Capture Register */
	OCR1A = 0;				/*Clear Compare Register 1A*/
	TCNT1 = 0;				/*Clear Timer1 COunter*/
}
/*===============================================================================================================*/
/* Description:
 * 					Function to Start Timer2 with certain configuration
 * [Inputs]:		TIMER2 Configuration structure by address
 *
 * [No Returns]:
 */
void TIMER2_init(TIMER_CONFIG_STRUCT *TIMER2_Config_Type)
{
	TCNT2 = TIMER2_Config_Type->Initial_value8;	/*Initial Value*/
	OCR2  = TIMER2_Config_Type->Compare_Value8;	/*Compare Value*/
	/*switch to choose clock pre-scaler*/
	switch(TIMER2_Config_Type->TIMER_Prescaler)
	{
		case F_CPU_CLOCK:
			SET_BIT(TCCR2,CS20);
			break;
		case F_CPU_N8:
			SET_BIT(TCCR2,CS21);
			break;
		case F_CPU_N32:
			SET_BIT(TCCR2,CS20);
			SET_BIT(TCCR2,CS21);
			break;
		case F_CPU_N64:
			SET_BIT(TCCR2,CS22);
			break;
		case F_CPU_N128:
			SET_BIT(TCCR2,CS20);
			SET_BIT(TCCR2,CS22);
			break;
		case F_CPU_N256:
			SET_BIT(TCCR2,CS21);
			SET_BIT(TCCR2,CS22);
			break;
		case F_CPU_N1024:
			SET_BIT(TCCR2,CS20);
			SET_BIT(TCCR2,CS21);
			SET_BIT(TCCR2,CS22);
			break;
	}
	/*switch to choose mode*/
	switch(TIMER2_Config_Type->TIMER_Mode)
	{
		case OVERFLOW_MODE:
			SET_BIT(TIMSK,TOIE2);	/*Enable Overflow Mode Interrupt Module*/
			SET_BIT(TCCR2,FOC2);
			break;
		case COMPARE_MODE:
			SET_BIT(TIMSK,OCIE2);	/*Enable Compare Mode Interrupt Module*/
			SET_BIT(TCCR2,FOC2);
			SET_BIT(TCCR2,WGM21);
			break;
		case PWM_MODE:				/*FPWM=F_CPU/N*256*/
			SET_BIT(DDRD,PD7); 		/*Make OC2 as output pin*/
			SET_BIT(TCCR2,WGM20);
			SET_BIT(TCCR2,WGM21);
			SET_BIT(TCCR2,COM21);	/*Non-Inverting Mode*/
			break;
	}
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Functions to set the call back functions
 * [Inputs]:		Pointers to functions that will be called from Timer2 ISR
 *
 * [No Returns]:
 */
void TIMER2_setCallBackFunctionsOVF(void(*a_FunctionPtr_OVF)(void))
{
	TIMER2_Ovf_g_callBackFunction_Ptr = a_FunctionPtr_OVF;
}
void TIMER2_setCallBackFunctionCOMP(void(*a_FunctionPtr_COMP)(void))
{
	TIMER2_Comp_g_callBackFunction_Ptr = a_FunctionPtr_COMP;
}
/*---------------------------------------------------------------------------------------------------------------*/
/* Description:
 * 					Function to turn off Timer2
 * [No Inputs]:
 * [No Returns]:
 */
void TIMER2_Deinit(void)
{
	CLEAR_BIT(DDRD,PD7);	/*Detach OC2*/
	TCCR2 = 0;				/*Clear Timer2 Control Register*/
	CLEAR_BIT(TIMSK,OCIE2); /*Disable Overflow Mode Interrupt Module*/
	CLEAR_BIT(TIMSK,TOIE2); /*Disable Compare Mode Interrupt Module*/
	CLEAR_BIT(TIFR,OCF2);	/*Clear Compare Mode Flag*/
	CLEAR_BIT(TIFR,TOV2);	/*Clear Overflow Mode Flag*/
	OCR2  = 0;				/*Clear Compare Register*/
	TCNT2 = 0;				/*Clear Counter Register*/
}
