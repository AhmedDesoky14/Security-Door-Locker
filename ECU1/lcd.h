/*
 *  lcd.h
 *  Created on: Oct 3, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			LMB162ABC LCD Driver 4/8 Bit mode
 *  			This Driver only supports one port arranged connected data
 */
#ifndef LCD_H_
#define LCD_H_
/*==================================INCLUDES=============================================*/
#include "std_types.h"
#include "common_macros.h" /* For GET_BIT Macro */
#include "gpio.h"
#include <avr./delay.h> /* For delay functions */
/*===========================DEFINITIONS & CONFIGURATIONS================================*/
#define RW_GROUNDED 1 /*RW pin is connected to ground = 1 - not connected = 0*/
/*4 for four-bit mode LCD & 8 for eight-bit mode LCD*/
#define LCD_DATA_BITS_MODE 8 /*RE-CONFIGURABLE*/
#if ((LCD_DATA_BITS_MODE != 4) && (LCD_DATA_BITS_MODE != 8))
	#error "Number of Data bits should be equal to 4 or 8"
#endif
/*---------------------------------------------------------------------------------------*/
#define LCD_RS_PORT_ID PORTD_ID
#define LCD_RS_PIN_ID PIN2_ID
/*---------------------------------------------------------------------------------------*/
#if(RW_GROUNDED == 0)
#define LCD_RW_PORT_ID PORTB_ID
#define LCD_RW_PIN_ID PIN1_ID
#endif
/*---------------------------------------------------------------------------------------*/
#define LCD_E_PORT_ID PORTD_ID
#define LCD_E_PIN_ID PIN3_ID
/*---------------------------------------------------------------------------------------*/
#define LCD_DATA_PORT_ID PORTC_ID
#if(LCD_DATA_BITS_MODE == 4)
#define LCD_DB4_PIN_ID PIN3_ID
#define LCD_DB5_PIN_ID PIN4_ID
#define LCD_DB6_PIN_ID PIN5_ID
#define LCD_DB7_PIN_ID PIN6_ID
#endif
/*=======================================LCD COMMANDS====================================*/
/*For initialization commands*/
#define LCD_TWO_LINES_8_BIT_MODE 0x38
#define LCD_TWO_LINES_4_BIT_MODE 0x28
#define LCD_TWO_LINES_4_BIT_MODE_INIT1 0X33
#define LCD_TWO_LINES_4_BIT_MODE_INIT2 0X32
/*---------------------------------------------------------------------------------------*/
#define LCD_CLEAR_COMMAND	0x01
#define LCD_GO_TO_HOME 0x02
#define LCD_SET_CURSOR_LOCATION	0x80 /*Used to set cursor location at needed position*/
#define LCD_DISPLAY_OFF_CURSOR_OFF 0x08
/*---------------------------------------------------------------------------------------*/
/*LCD Cursor Commands*/
#define LCD_CURSOR_OFF 0x0C
#define LCD_CURSOR_ON 0x0E
#define LCD_CURSOR_Blinking 0x0F
#define LCD_SHIFT_CURSOR_LEFT 0x10
#define LCD_SHIFT_CURSOR_RIGHT 0x14
/*there's another Cursor shifting*/
#define LCD_SHIFT_CURSOR_LEFT2 0x04
#define LCD_SHIFT_CURSOR_RIGHT2 0x06
/*---------------------------------------------------------------------------------------*/
/*LCD Display Commands*/
#define LCD_SHIFT_DISPLAY_LEFT 0x18
#define LCD_SHIFT_DISPLAY_RIGHT 0x1C
/*there's another Display shifting*/
#define LCD_SHIFT_DISPLAY_LEFT2 0x07
#define LCD_SHIFT_DISPLAY_RIGHT2 0x05
/*=================================FUNCTIONS DECLARATIONS================================*/
/*Functions Description in .c file*/
void LCD_sendCommand (uint8 command);
void LCD_displayCharacter(uint8 character);
void LCD_init(void);
void LCD_displayString(const char *string);
void LCD_moveCursor(uint8 row, uint8 col);
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *string);
void LCD_clearScreen(void);
void LCD_integerToString(int data);
/*=======================================================================================*/
#endif /* LCD_H_ */
