/*
 * Example for using I2C with 128x64 OLED display
 * CH32V003 Badge example
 * Daniel Quadros, SET-2024
 */

#define SSD1306_128X64

#include "ch32v003fun.h"

#include <stdio.h>
#include <string.h>

#include "ssd1306_i2c.h"
#include "ssd1306.h"

#include "storm.h"

#define FALSE 0
#define TRUE  1

// Badge I/O
#define SHUTDOWN_PORT 		GPIOC
#define SHUTDOWN_PIN			0
#define POWER_ON()				SHUTDOWN_PORT->BSHR = (1<<(SHUTDOWN_PIN))
#define POWER_OFF()				SHUTDOWN_PORT->BSHR = (1<<(16+SHUTDOWN_PIN))

#define LED1_PORT 				GPIOC
#define LED1_PIN					4
#define LED1_ON()					LED1_PORT->BSHR = (1 << (LED1_PIN))
#define LED1_OFF()				LED1_PORT->BSHR = (1 << (16+LED1_PIN))

#define LED2_PORT 				GPIOC
#define LED2_PIN					7
#define LED2_ON()					LED2_PORT->BSHR = (1 << (LED2_PIN))
#define LED2_OFF()				LED2_PORT->BSHR = (1 << (16+LED2_PIN))

#define LED3_PORT 				GPIOA
#define LED3_PIN					2
#define LED3_ON()					LED3_PORT->BSHR = (1 << (LED3_PIN))
#define LED3_OFF()				LED3_PORT->BSHR = (1 << (16+LED3_PIN))

#define LED4_PORT 				GPIOA
#define LED4_PIN					1
#define LED4_ON()					LED3_PORT->BSHR = (1 << (LED4_PIN))
#define LED4_OFF()				LED3_PORT->BSHR = (1 << (16+LED4_PIN))

#define SW1_PORT 				GPIOC
#define SW1_PIN					3
#define SW1_PRESSED()   ((SW1_PORT->INDR & (1 <<(SW1_PIN))) == 0)


// Initialize the pins
void pin_init() {
	// set up GPIO for LEDs
	LED1_PORT->CFGLR &= ~(0xf<<(4*LED1_PIN));
	LED1_PORT->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*LED1_PIN);
	LED1_OFF();
	LED2_PORT->CFGLR &= ~(0xf<<(4*LED2_PIN));
	LED2_PORT->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*LED2_PIN);
	LED2_OFF();
	LED3_PORT->CFGLR &= ~(0xf<<(4*LED3_PIN));
	LED3_PORT->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*LED3_PIN);
	LED3_OFF();
	LED4_PORT->CFGLR &= ~(0xf<<(4*LED4_PIN));
	LED4_PORT->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*LED4_PIN);
	LED4_OFF();

	// set up GPIO for User Button
	SW1_PORT->CFGLR &= ~0xf<<(4*SW1_PIN);
	SW1_PORT->CFGLR |= (GPIO_Speed_In | GPIO_CNF_IN_FLOATING)<<(4*SW1_PIN);

}

// Demo screens
#define	NSCREENS 2
void show_screen (int scr) {
	ssd1306_setbuf(0);
	switch (scr) {
		case 0:
			ssd1306_drawstr_sz(12,0, "DQSOFT", 1, fontsize_16x16);
			ssd1306_drawstr(0,24,"youtube.com/", 1);
			ssd1306_drawstr(8,32,"@DQSoft", 1);
			ssd1306_drawstr(0,48,"dqsoft.blogspot", 1);
			ssd1306_drawstr(8,56,".com", 1);
			break;
		case 1:
			ssd1306_drawImage(40, 8, storm, 48, 48, 0);
			break;
	}
	ssd1306_refresh();
}

int main() {
	// 48MHz internal clock
	SystemInit();
	Delay_Ms(10);

	// Enable GPIOD, GPIOC and GPIOA
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA;
	pin_init();
	Delay_Ms(10);

	// init pins and display
	ssd1306_i2c_init();
  ssd1306_init();

	// set up GPIO for SHUTDOWN
	SHUTDOWN_PORT->CFGLR &= ~(0xf<<(4*SHUTDOWN_PIN));
	SHUTDOWN_PORT->CFGLR |= (GPIO_Speed_2MHz | GPIO_CNF_OUT_PP)<<(4*SHUTDOWN_PIN);
	POWER_ON();

	// Show screen
	int scr = 0;
	show_screen(scr);

	// Main Loop
	int iled = 0;
  uint32_t last_time = SysTick->CNT;	// para auto-shutdown
	while (TRUE) {

		// Test time out		
		if ((SysTick->CNT - last_time) > Ticks_from_Ms(30000)) {
			break;	// auto-shutdown
		}

		// Test switch pressed
		if (SW1_PRESSED()) {
			Delay_Ms(100);		// debounce
			while (SW1_PRESSED()) {
				Delay_Ms(10);
			}
			// Move to next screen
			scr = (scr+1) % NSCREENS;
			show_screen(scr);
			last_time = SysTick->CNT;
		}

		// Show activity in LEDs
		switch (iled) {
			case 0:
				LED4_OFF();
				LED1_ON();
				break;
			case 1:
				LED1_OFF();
				LED2_ON();
				break;
			case 2:
				LED2_OFF();
				LED3_ON();
				break;
			case 3:
				LED3_OFF();
				LED4_ON();
				break;
		}
		iled = (iled + 1) & 3;

		// Main loop delay
		Delay_Ms(100);
	}

	// Stop
	LED4_ON();
	POWER_OFF();
	while(1)
		;

}
