/* 
	Buzzer on LEDC (LED Controller) example

   	This example code is in the Public Domain (or CC0 licensed, at your option.)

   	Unless required by applicable law or agreed to in writing, this
   	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

/*
 * 
 * Buzzer on GPIO_NUM_4 (pull-down 10k on Gate)
 *
 */

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_GPIO       GPIO_NUM_4
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_TEST_DUTY         (4000)


void app_main()
{
    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, 	// resolution of PWM duty
        .freq_hz = 4000,                      	// frequency of PWM signal
        .speed_mode = LEDC_HS_MODE,           	// timer mode
        .timer_num = LEDC_HS_TIMER            	// timer index
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    /*
     * Prepare individual configuration
     * for each channel of LED Controller
     * by selecting:
     * - controller's channel number
     * - output duty cycle, set initially to 0
     * - GPIO number where LED is connected to
     * - speed mode, either high or low
     * - timer servicing selected channel
     *   Note: if different channels use one timer,
     *         then frequency and bit_num of these channels
     *         will be the same
     */

    ledc_channel_config_t ledc_channel = {
            .channel    = LEDC_HS_CH0_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_HS_CH0_GPIO,
            .speed_mode = LEDC_HS_MODE,
            .timer_sel  = LEDC_HS_TIMER
    };

    // Set LED Controller with previously prepared configuration
    ledc_channel_config(&ledc_channel);


    while (1) 
    {
        printf("Buzzer set duty = %d\n", LEDC_TEST_DUTY);
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, LEDC_TEST_DUTY);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(50 / portTICK_PERIOD_MS);

        printf("Buzzer set duty = 0\n");
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
