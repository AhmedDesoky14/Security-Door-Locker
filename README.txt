# Security Door Locker System

This project is a **Security Door Locker System** based on a **layered architecture model** using two **AVR ATmega32 Microcontrollers**. The system communicates between the two microcontrollers via **UART Interface** and controls different devices such as an **external EEPROM (24C16)** using the **Two-Wire Interface (TWI)** protocol, an **LCD**, a **DC Motor**, and a **keypad** using the **DIO (Digital Input/Output)** peripheral of the microcontroller.

---

## Features

- **Password Management**: 
  - The system checks if there is a registered password in the external EEPROM memory. If not, it prompts the user to set a new password.
  - The user can change the password by entering the old password and the new password for registration.
  - The system allows the user to open the security locker by entering the registered password.

- **Security**:
  - If the user enters the wrong password in any of the cases (changing the password or opening the locker), the system stops accepting any input.
  - A **safety alarm** is triggered if the wrong password is entered, and it stops after **1 minute**.

- **Device Control**:
  - **EEPROM (24C16)**: Stores and retrieves the registered password using the TWI protocol.
  - **LCD Display**: Displays prompts and status messages.
  - **DC Motor**: Represents the security door locker, which is controlled by the microcontroller.
  - **Keypad**: Used for password entry.

- **Microcontroller Communication**:
  - The system uses UART for communication between the two AVR ATmega32 microcontrollers.

---

## Project Overview

The system is designed to ensure a high level of security for a door locker using a password-based mechanism. The password is stored in an external EEPROM (24C16) and can be changed or used to open the locker. The system stops accepting input after a failed attempt and triggers a safety alarm.

---

### Prerequisites:

1. **Hardware**:
   - 2x ATmega32 Microcontrollers
   - 24C16 External EEPROM (connected via TWI)
   - 16x2 LCD Display (connected via DIO or I2C)
   - DC Motor (controlled via DIO)
   - Keypad (connected to DIO pins)
   - Safety Alarm (could be a buzzer or speaker)
   - Jumper Wires and Power Supply

---

## Contact 

For questions or suggestions, please contact me, Ahmed Desoky at "ahmed0201150@gmail.com".
