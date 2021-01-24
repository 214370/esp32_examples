# UART2 Echo Example

This is an example which echoes any data it receives on UART2 back to the sender.
It writes test string to the UART if letter 'g' is the first received character.

## Setup

1. Connect an external serial interface to an ESP32 board. The external interface should have 3.3V outputs. You may use e.g. 3.3V compatible USB-to-serial dongle:

  * UART_TXD  (GPIO_NUM_33)
  * UART_RXD  (GPIO_NUM_35)
  * GND       (GND)


