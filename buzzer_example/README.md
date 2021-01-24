# Buzzer (on LED controller) Example

This example shows how to control buzzer using ESP32's LEDC.

## Functionality

Just play some sounds :)

## Hardware Setup

Connect logic MOSFET's Gate on GPIO_NUM_4 and pull-down the pin with 10k resistor. Buzzer is in series with MOSFET, VCC-Buzzer-Drain, Source is on GND.


