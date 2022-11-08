/*
 *  lcd.c
 *  Created on: Oct 3, 2022
 *  Author: Ahmed Desoky
 *  Description:
 *  			LMB162ABC LCD Driver 4/8 Bit mode
 *  			This Driver only supports one port arranged connected data
 */
/*==================================INCLUDES=============================================*/
#include "lcd.h"
/*=============================FUNCTIONS DEFINITIONS=====================================*/
/* Description:
 * Function to Send Commands to LCD
 * [Inputs]:
 * 			A Command
 * [No Return]
 */
void LCD_sendCommand (uint8 command)
{
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID,LOGIC_LOW); /*RS=0 (Instructions mode)*/
	_delay_ms(1);	/* delay for processing Tas = 50ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID,LOGIC_HIGH); /*E=1 (Enable LCD)*/
	_delay_ms(1);	/* delay for processing Tpw - Tdws = 190ns */
#if(LCD_DATA_BITS_MODE == 4)
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID,GET_BIT(command,4));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID,GET_BIT(command,5));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID,GET_BIT(command,6));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID,GET_BIT(command,7));
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID,LOGIC_LOW); /*E=0 (Disable LCD)*/
	_delay_ms(1); /* delay for processing Th = 13ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID,LOGIC_HIGH); /*E=1 (Enable LCD)*/
	_delay_ms(1);	/* delay for processing Tpw - Tdws = 190ns */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID,GET_BIT(command,0));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID,GET_BIT(command,1));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID,GET_BIT(command,2));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID,GET_BIT(command,3));
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID,LOGIC_LOW); /*E=0 (Disable LCD)*/
	_delay_ms(1); /* delay for processing Th = 13ns */
#elif(LCD_DATA_BITS_MODE == 8)
	GPIO_writePort(LCD_DATA_PORT_ID, command); //command to be sent
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID,LOGIC_LOW); //E
	_delay_ms(1); /* delay for processing Th = 13ns */
#endif
}
/*----------------------------------------------------------------------------------------*/
/* Description:
 * Function to display Character on LCD at cursor location
 * [Inputs]:
 * 			A Character
 * [No Return]
 */
void LCD_displayCharacter(uint8 character)
{
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID,LOGIC_HIGH); /*RS=1 (Character mode)*/
	_delay_ms(1);	/* delay for processing Tas = 50ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID,LOGIC_HIGH); /*E=1 (Enable LCD)*/
	_delay_ms(1);	/* delay for processing Tpw - Tdws = 190ns */
#if(LCD_DATA_BITS_MODE == 4)
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID,GET_BIT(character,4));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID,GET_BIT(character,5));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID,GET_BIT(character,6));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID,GET_BIT(character,7));
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID,LOGIC_LOW); /*E=0 (Disable LCD)*/
	_delay_ms(1); /* delay for processing Th = 13ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID,LOGIC_HIGH); /*E=1 (Enable LCD)*/
	_delay_ms(1);	/* delay for processing Tpw - Tdws = 190ns */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID,GET_BIT(character,0));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID,GET_BIT(character,1));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID,GET_BIT(character,2));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID,GET_BIT(character,3));
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID,LOGIC_LOW); /*E=0 (Disable LCD)*/
	_delay_ms(1); /* delay for processing Th = 13ns */
#elif(LCD_DATA_BITS_MODE == 8)
	GPIO_writePort(LCD_DATA_PORT_ID, character); //command to be sent
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID,LOGIC_LOW); //E
	_delay_ms(1); /* delay for processing Th = 13ns */
#endif
}
/*----------------------------------------------------------------------------------------*/
/* Description:
 * Function to initialize the LCD
 * [No Inputs]
 * [No Return]
 */
void LCD_init(void)
{
#if(RW_GROUNDED == 0)
	GPIO_setupPinDirection(LCD_RW_PORT_ID,LCD_RW_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(LCD_RW_PORT_ID,LCD_RW_PIN_ID,LOGIC_LOW);
#endif
	GPIO_setupPinDirection(LCD_RS_PORT_ID,LCD_RS_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT_ID,LCD_E_PIN_ID,PIN_OUTPUT);
	_delay_ms(20); /* LCD Power ON delay always > 15ms */
#if(LCD_DATA_BITS_MODE == 4)
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,PIN_OUTPUT);
	LCD_sendCommand(LCD_TWO_LINES_4_BIT_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_4_BIT_MODE_INIT2);
	LCD_sendCommand(LCD_TWO_LINES_4_BIT_MODE);
#elif(LCD_DATA_BITS_MODE == 8)
	GPIO_setupPortDirection(PORTC_ID,PORT_OUTPUT);
	LCD_sendCommand(LCD_TWO_LINES_8_BIT_MODE); /*definitions to be made in header file*/
#endif
	LCD_sendCommand(LCD_CURSOR_OFF);
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}
/*-----------------------------------------------------------------------------------------*/
/* Description:
 * Function to display String on LCD at cursor location
 * [Inputs]:
 * 			A pointer to constant char (string)
 * [No Return]
 */
void LCD_displayString(const char *string)
{
	uint8 looper = 0;
	while(string[looper] != '\0')
	{
		LCD_displayCharacter(*(string+looper));
		looper++;
	}
}
/*-----------------------------------------------------------------------------------------*/
/* Description:
 * Function to move cursor to another location
 * [Inputs]:
 * 			row position, column position
 * [No Return]
 */
void LCD_moveCursor(uint8 row, uint8 col)
{
	uint8 lcd_memory_address;
	switch(row)
	{
		case 0:
			lcd_memory_address = col;
			break;
		case 1:
			lcd_memory_address = 0x40 + col;
			break;
		case 2:
			lcd_memory_address = 0x10 + col;
			break;
		case 3:
			lcd_memory_address = 0x50 + col;
			break;
	}
	LCD_sendCommand(lcd_memory_address | LCD_SET_CURSOR_LOCATION);
}
/*-----------------------------------------------------------------------------------------*/
/* Description:
 * Function to display String on LCD at chosen location
 * [Inputs]:
 * 			A pointer to constant char (string), row position, column position
 * [No Return]
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *string)
{
	LCD_moveCursor(row,col);
	LCD_displayString(string);
}
/*-----------------------------------------------------------------------------------------*/
/* Description:
 * Function to clear LCD
 * [No Inputs]
 * [No Return]
 */
void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}
/*-----------------------------------------------------------------------------------------*/
/* Description:
 * Function to display integers on LCD at cursor location
 * [Inputs]:
 * 			an integer
 * [No Return]
 */
void LCD_integerToString(int data)
{
	uint8 buffer[16]; /*char buffer[16]*/
	itoa(data,buffer,10); /*10 for ASCII in decimal*/
	LCD_displayString(buffer);
}
/*=========================================================================================*/
