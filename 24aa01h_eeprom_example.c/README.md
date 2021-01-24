# 24AA01H EEPROM Chip Example

This example shows how to use 24AA01H EEPROM chip using ESP32.

## Functionality

7 bytes are writen to EEPROM chip, then that section of the code is commented in order to check at second run does EEPROM has it stored. 

## Hardware Setup

Connect 24AA01H chip to individual GPIOs:

  * SDA - GPIO21
  * SCL - GPIO22

The I2C lines should have a pull-up resistors (1k - 10k).

