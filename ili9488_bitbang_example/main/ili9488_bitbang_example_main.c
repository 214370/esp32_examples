/* 
    ili9488_bitbang_example

    rewrite of ER-TFTM035-6 provided code example 
    for use with v4.0-beta2
    tested on ESP32 DEVKITV1 board
    useful to check your hardware and connections :)

    This example code is in the Public Domain (or CC0 licensed, at your option.)

    Unless required by applicable law or agreed to in writing, this
    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define BLACK		0x0000
#define BLUE		0x001F
#define GREEN		0x07E0
#define RED			0xF800
#define YELLOW		0xFFE0
#define WHITE		0xFFFF

#define BLINK_GPIO 	GPIO_NUM_2

#define CS 			GPIO_NUM_5
#define SCL			GPIO_NUM_18
#define DC			GPIO_NUM_25   
#define SDA			GPIO_NUM_23						
#define RES			GPIO_NUM_26  					// Reset
#define BCKL	    GPIO_NUM_27  					// Backlight

void CS_LOW()    { gpio_set_level(CS, 0);   }
void CS_HIGH()   { gpio_set_level(CS, 1);   }
void DC_LOW()    { gpio_set_level(DC, 0);   }
void DC_HIGH()   { gpio_set_level(DC, 1);   }
void SDA_LOW()   { gpio_set_level(SDA, 0);  }
void SDA_HIGH()  { gpio_set_level(SDA, 1);  }
void SCL_LOW()	 { gpio_set_level(SCL, 0);  }
void SCL_HIGH()	 { gpio_set_level(SCL, 1);  }
void RES_LOW()	 { gpio_set_level(RES, 0);  }
void RES_HIGH()	 { gpio_set_level(RES, 1);  }
void BCKL_LOW()	 { gpio_set_level(BCKL, 0); }
void BCKL_HIGH() { gpio_set_level(BCKL, 1); }

void init_pins() {
    gpio_pad_select_gpio(CS);
    gpio_set_direction(CS, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(DC);
    gpio_set_direction(DC, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(SDA);
    gpio_set_direction(SDA, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(SCL);
    gpio_set_direction(SCL, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(RES);
    gpio_set_direction(RES, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(BCKL);
    gpio_set_direction(BCKL, GPIO_MODE_OUTPUT);
}

// Write data byte
void Write_Data(uint8_t DH, uint8_t DL)
{
    uint8_t R1,G1,B1,i; uint16_t LD=0;
    // RGB565 TO RGB666 color conversion
    LD = DH << 8;
    LD |= DL;
    R1 = (0x1f & (LD >> 11)) * 2; 
    R1 <<= 2;
    G1 =0x3f & (LD >> 5);	  
    G1 <<= 2;
    B1 = (0x1f & LD) * 2; 	  
    B1 <<= 2;
		 
    CS_LOW();
    DC_HIGH();

    for(i=0;i<8;i++) {
        SCL_LOW();
        if (R1 & 0x80) SDA_HIGH();
       	else           SDA_LOW();
       	SCL_HIGH();
        R1 = R1 << 1;
    }

    for(i=0;i<8;i++) {
        SCL_LOW();
        if (G1 & 0x80) SDA_HIGH();
       	else           SDA_LOW();
       	SCL_HIGH();
        G1 = G1 << 1;
    }

    for(i=0;i<8;i++) {  
        SCL_LOW();
        if (B1 & 0x80) SDA_HIGH();
       	else           SDA_LOW();
       	SCL_HIGH();
        B1 = B1 << 1;
    }
    CS_HIGH();
}

// Write data word
void Write_Data_U16(uint16_t y) 
{
    uint8_t m,n;
    m = y >> 8;
    n = y;
    Write_Data(m,n);
}

// Write command
void Write_Cmd(uint8_t CMD) 
{ 
    uint8_t i;	 
    CS_LOW();
    DC_LOW();
    for(i=0;i<8;i++) {
        SCL_LOW();
        if (CMD & 0x80) SDA_HIGH();
       	else            SDA_LOW();
       	SCL_HIGH();
        CMD = CMD << 1;
    }
    CS_HIGH();
}

// Write parameter
void Write_Cmd_Data(uint8_t CMDP) 
{
    uint8_t i;	 
    CS_LOW();
    DC_HIGH();
    for(i=0;i<8;i++) {
        SCL_LOW();
        if (CMDP & 0x80) SDA_HIGH();
       	else             SDA_LOW(); 
       	SCL_HIGH();
        CMDP = CMDP << 1;
     }
     CS_HIGH();
}

void ili9488_init() 
{ 
    RES_HIGH();								// Hardware reset first
    vTaskDelay(5 / portTICK_PERIOD_MS);
    RES_LOW();
    vTaskDelay(10 / portTICK_PERIOD_MS);
    RES_HIGH();
    vTaskDelay(120 / portTICK_PERIOD_MS);

    Write_Cmd(0xE0);						// Positive Gamma Control
    Write_Cmd_Data(0x00);
    Write_Cmd_Data(0x03);
    Write_Cmd_Data(0x09);
    Write_Cmd_Data(0x08);
    Write_Cmd_Data(0x16);
    Write_Cmd_Data(0x0A);
    Write_Cmd_Data(0x3F);
    Write_Cmd_Data(0x78);
    Write_Cmd_Data(0x4C);
    Write_Cmd_Data(0x09);
    Write_Cmd_Data(0x0A);
    Write_Cmd_Data(0x08);
    Write_Cmd_Data(0x16);
    Write_Cmd_Data(0x1A);
    Write_Cmd_Data(0x0F);
	
    Write_Cmd(0xE1);						// Negative Gamma Control
    Write_Cmd_Data(0x00);
    Write_Cmd_Data(0x16);
    Write_Cmd_Data(0x19);
    Write_Cmd_Data(0x03);
    Write_Cmd_Data(0x0F);
    Write_Cmd_Data(0x05);
    Write_Cmd_Data(0x32);
    Write_Cmd_Data(0x45);
    Write_Cmd_Data(0x46);
    Write_Cmd_Data(0x04);
    Write_Cmd_Data(0x0E);
    Write_Cmd_Data(0x0D);
    Write_Cmd_Data(0x35);
    Write_Cmd_Data(0x37);
    Write_Cmd_Data(0x0F);
	
    Write_Cmd(0xC0);      					// Power Control 1 
    Write_Cmd_Data(0x17);   				// Vreg1out 
    Write_Cmd_Data(0x15);   				// Verg2out 
	 
    Write_Cmd(0xC1);      					// Power Control 2     
    Write_Cmd_Data(0x41);       			// VGH,VGL 
	 
    Write_Cmd(0xC5);      					// Power Control 3 
    Write_Cmd_Data(0x00); 
    Write_Cmd_Data(0x12);   				// Vcom 
    Write_Cmd_Data(0x80);
	 
    Write_Cmd(0x36);      					// Memory Access 
    Write_Cmd_Data(0x48); 
	 
    Write_Cmd(0x3A);      					// Interface Pixel Format 
    Write_Cmd_Data(0x66); 					// 18 bit    
	 
    Write_Cmd(0xB0);      					// Interface Mode Control 
    Write_Cmd_Data(0x80);   				// SDO not in use
	 
    Write_Cmd(0xB1);      					// Frame rate 
    Write_Cmd_Data(0xA0);   				// 60Hz 
	 
    Write_Cmd(0xB4);      					// Display Inversion Control 
    Write_Cmd_Data(0x02);   				// 2-dot 
	 
    Write_Cmd(0xB6);      					// Display Function Control RGB/MCU Interface Control 
    Write_Cmd_Data(0x02);   				// MCU 
    Write_Cmd_Data(0x02);   				// Source, Gate scan direction 
	  
    Write_Cmd(0xE9);      					// Set Image Function
    Write_Cmd_Data(0x00);   				// Disable 24 bit data
	 
    Write_Cmd(0xF7);      					// Adjust Control 
    Write_Cmd_Data(0xA9);     
    Write_Cmd_Data(0x51);     
    Write_Cmd_Data(0x2C);     
    Write_Cmd_Data(0x82);   				// D7 stream, loose 

    Write_Cmd(0x11); 						// Sleep out
    vTaskDelay(120 / portTICK_PERIOD_MS);
    Write_Cmd(0x29);						// Display On
}

void LCD_SetPos(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye) {
    Write_Cmd(0x2A);						// Column Address Set
    Write_Cmd_Data(xs>>8);
    Write_Cmd_Data(xs&0xff);
    Write_Cmd_Data(xe>>8);
    Write_Cmd_Data(xe&0xff);
    Write_Cmd(0x2B);						// Page Address Set
    Write_Cmd_Data(ys>>8);
    Write_Cmd_Data(ys&0xff);
    Write_Cmd_Data(ye>>8);
    Write_Cmd_Data(ye&0xff);
    Write_Cmd(0x2C);						// Memory Write
}

void Fill_Screen(uint16_t bColor) {
    uint16_t i,j;
    LCD_SetPos(0,319,0,479);
    for (i=0;i<320;i++) {
        for (j=0;j<480;j++)
            Write_Data_U16(bColor);
    }
}

void init_lcd()
{
    init_pins();							// Init all LCD pins as output pins
    ili9488_init();							// Initial sequence for ILI9488
    BCKL_HIGH();							// Turn on backlight
}

void blink_task(void *pvParameter)
{
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        // Blink off (output low)
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // Blink on (output high)
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void lcd_task(void *pvParameter)
{
    init_lcd();
    while(1) {
        Fill_Screen(BLACK);
		Fill_Screen(BLUE);
		Fill_Screen(GREEN);
		Fill_Screen(RED);
		Fill_Screen(YELLOW);
		Fill_Screen(WHITE);
    }
}


void app_main()
{
    xTaskCreate(&lcd_task, "lcd_task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
}




