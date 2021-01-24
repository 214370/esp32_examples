/* 
	UART2 Echo Example

   	This example code is in the Public Domain (or CC0 licensed, at your option.)

   	Unless required by applicable law or agreed to in writing, this
   	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

/*
 * This is an example which echos any data it receives on UART back to the sender,
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: UART2
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below
 */

#define UART_TXD  (GPIO_NUM_33)
#define UART_RXD  (GPIO_NUM_35)
#define UART_RTS  (UART_PIN_NO_CHANGE)
#define UART_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE  (1024)

static void uart_task()
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    /* Choose UART port here */
    const int uart_num = UART_NUM_2;
    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, UART_TXD, UART_RXD, UART_RTS, UART_CTS);
    uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    char* test_str = "This is a test string.\n\r";

    while (1) {
    	// Read data from the UART
    	int len = uart_read_bytes(uart_num, data, BUF_SIZE, 20 / portTICK_RATE_MS);
    	// Write data to the UART
    	if(data[0] == 'g') {
    		uart_write_bytes(uart_num, (const char*) test_str, strlen(test_str));
    		//uart_write_bytes(uart_num, (const char *) data, len);
   		data[0] = 0;
    	}
    }
}

void app_main()
{
    xTaskCreate(&uart_task, "uart_task", 1024, NULL, 1, NULL);
}
