/*
 * 	keypad.h
 *  Created on: Oct 15, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			keypad configurable driver for 4x4 and 4x3 keypads
 *  			this driver only works with one port connected row and one port connected columns
 *  			for standard and non-standard keypads
 */
#ifndef KEYPAD_H_
#define KEYPAD_H_
/*==================================INCLUDES=============================================*/
#include "gpio.h"
#include "std_types.h"
#include <util/delay.h>
/*==============================Configuration Note=======================================*/
/*	to re-configure this driver go to line 20 in this .h file
 *  and go to line 77 in the .c file
 */
/*===========================DEFINITIONS & CONFIGURATIONS================================*/
//#define STANDARD_KEYPAD
#define KEYPAD_NUM_COLS 			4
#define KEYPAD_NUM_ROWS				4
#define KEYPAD_ROW_PORT_ID			PORTA_ID
#define KEYPAD_FIRST_ROW_PIN_ID		PIN0_ID
#define KEYPAD_COL_PORT_ID			PORTA_ID
#define KEYPAD_FIRST_COL_PIN_ID		PIN4_ID
#define KEYPAD_BUTTON_PRESSED		LOGIC_LOW		/*Due to pull up resistor method*/
#define KEYPAD_BUTTON_RELEASED		LOGIC_HIGH		/*if pull down, exchange the last two configurations*/
/*=================================FUNCTIONS DECLARATIONS================================*/
/*Functions Description in .c file*/
uint8 KEYPAD_getPressedKey(void);
#endif /* KEYPAD_H_ */
