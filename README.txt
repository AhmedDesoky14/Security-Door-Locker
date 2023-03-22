Author: Ahmed Moustafa Desoky
/*=============================================================================================================*/
This is Security Door Locker System Program based on layered architecture model
consisted of 2 programmed microcontrollers, AVR ATmega32
they communicate with each other using UART Interface
and control different devices such as the external EEPROM memory 24C16 using Two Wire Interface Protocol 
LCD, DC Motor and keypad using DIO peripheral of the microcontroller
/*=============================================================================================================*/
the programs initially starts to check wether there's a registered password in the external memory 
if not, it asks the user to enter a new password
the user can change the password later by entering the old password and the new password to be registered,
or the user can open the locker which is represented by a motor using the registered password.
if in any of the two cases the user enters the required password wrong
the system stops receiving any input and initiate a safety alarm then stop after 1 minute.
