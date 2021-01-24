# ILI9488 LCD Example

This example shows how to use ILI9488 LCD display with ESP32.

## Functionality

This is a rewrite of ER-TFTM035-6 provided code example for use with esp-idf v4.1.
Useful to check your hardware and connections :)

## Hardware Setup

Tested on DOIT ESP32 DEVKIT1 board.
    
Connect ER-TFTM035-6 board to individual GPIOs:

  * CS - GPIO_5
  * SCL - GPIO_18
  * DC - GPIO_25
  * SDA - GPIO_23
  * RES - GPIO_26
  * BCKL - GPIO_27

For the rest consult ER-TFTM035-6 datasheet.
