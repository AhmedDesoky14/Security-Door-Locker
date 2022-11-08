/*
 * 	keypad.c
 *  Created on: Oct 15, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			keypad configurable driver for 4x4 and 4x3 keypads
 *  			this driver only works with one port connected row and one port connected columns
 *  			for standard and non-standard keypads
 */
/*==================================INCLUDES=============================================*/
#include "keypad.h"
/*========================PRIVATE FUNCTIONS DECLARATIONS=================================*/
#ifndef STANDARD_KEYPAD
#if (KEYPAD_NUM_COLS == 3)
	static uint8 KEYPAD_4x3_adjustKeyNumber (uint8 Button_number);
#elif (KEYPAD_NUM_COLS == 4)
	static uint8 KEYPAD_4x4_adjustKeyNumber (uint8 Button_number);
#endif /*endif of keypad size (number of columns)*/
#endif /*endif of standard keypad*/
/*=============================FUNCTIONS DEFINITIONS=====================================*/
/* Description:
 *				Function to get the pressed key value
 * [No Inputs]:
 *
 * [Returns]:
 * 				return the pressed key value
 */
uint8 KEYPAD_getPressedKey(void)
{
	uint8 col,row;
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+1,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+2,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+3,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+1,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+2,PIN_INPUT);
#if (KEYPAD_NUM_COLS == 4)
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+3,PIN_INPUT);
#endif
	while(1)
	{
		_delay_ms(50);	/*To solve the keypad latch in Proteus Simulation*/
		/*Loop on the rows*/
		for(row=0 ; row<KEYPAD_NUM_ROWS ; row++)
		{
			/*set the pins output in turn*/
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+row,PIN_OUTPUT);
			/*Set 0 to the row (pull up method)*/
			GPIO_writePin(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+row,LOGIC_LOW);
			/*Loop in the columns to read it*/
			for(col=0 ; col<KEYPAD_NUM_COLS ; col++)
			{
				/*Pressed the button*/
				if(GPIO_readPin(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+col) == KEYPAD_BUTTON_PRESSED)
				{
					#if (KEYPAD_NUM_COLS == 3)
						#ifdef STANDARD_KEYPAD
							return ((row*KEYPAD_NUM_COLS)+col+1));
						#else
							return (KEYPAD_4x3_adjustKeyNumber((row*KEYPAD_NUM_COLS)+col+1));
						#endif /*endif od standard keypad*/
					#elif (KEYPAD_NUM_COLS == 4)
						#ifdef STANDARD_KEYPAD
							return ((row*KEYPAD_NUM_COLS)+col+1));
						#else
							return (KEYPAD_4x4_adjustKeyNumber((row*KEYPAD_NUM_COLS)+col+1));
						#endif /*endif of standard keypad*/
					#endif /*endif of the keypad columns number*/
				}
			}
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+row,PIN_INPUT);
		}
	}
}
/*==================================PRIVATE FUNCTIONS====================================*/
/*======================*//*These Functions are Re-configurable*//*======================*/
#ifndef STANDARD_KEYPAD
#if (KEYPAD_NUM_COLS == 3)
/* Description:
 *				Function that takes the pressed key of non-standard keypad and return its actual value
 *				for 4x3 keypad
 * [Inputs]:
 *				pressed button number
 * [Returns]:
 * 				the actual value (meaning) of the pressed key
 */
static uint8 KEYPAD_4x3_adjustKeyNumber (uint8 Button_number)
{
	uint8 Button_After_Adjust = 0;
	switch(Button_number)
	{
		case 10:
			Button_After_Adjust = '*';
			break;
		case 11:
			Button_After_Adjust = 0;
			break;
		case 12:
			Button_After_Adjust = '#';
			break;
		default:
			Button_After_Adjust = Button_number;
			break;
	}
	return Button_After_Adjust;
}
#elif (KEYPAD_NUM_COLS == 4)
/* Description:
 *				Function that takes the pressed key of non-standard keypad and return its actual value
 *				for 4x4 keypad
 * [Inputs]:
 *				pressed button number
 * [Returns]:
 * 				the actual value (meaning) of the pressed key
 */
static uint8 KEYPAD_4x4_adjustKeyNumber (uint8 Button_number)
{
	uint8 Button_After_Adjust = 0;
	switch(Button_number)
	{
		case 1:
			Button_After_Adjust = 7;
			break;
		case 2:
			Button_After_Adjust = 8;
			break;
		case 3:
			Button_After_Adjust = 9;
			break;
		case 4:
			Button_After_Adjust = '%'; // ASCII Code of %
			break;
		case 5:
			Button_After_Adjust = 4;
			break;
		case 6:
			Button_After_Adjust = 5;
			break;
		case 7:
			Button_After_Adjust = 6;
			break;
		case 8:
			Button_After_Adjust = '*'; /* ASCII Code of '*' */
			break;
		case 9:
			Button_After_Adjust = 1;
			break;
		case 10:
			Button_After_Adjust = 2;
			break;
		case 11:
			Button_After_Adjust = 3;
			break;
		case 12:
			Button_After_Adjust = '-'; /* ASCII Code of '-' */
			break;
		case 13:
			Button_After_Adjust = 13;  /* ASCII of Enter */
			break;
		case 14:
			Button_After_Adjust = 0;
			break;
		case 15:
			Button_After_Adjust = '='; /* ASCII Code of '=' */
			break;
		case 16:
			Button_After_Adjust = '+'; /* ASCII Code of '+' */
			break;
		default:
			Button_After_Adjust = Button_number;
			break;
	}
	return Button_After_Adjust;
}
#endif /*endif of keypad size (number of columns)*/
#endif /*endif of standard keypad*/
